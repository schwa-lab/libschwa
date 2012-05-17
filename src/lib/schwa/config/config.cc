/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/config.h>
#include <schwa/cluster.h>


namespace schwa { namespace config {

std::string Main::PROGRAM_NAME;

typedef std::map<std::string, int> RecursiveCheck;


static bool
split_assignment(const std::string &s, std::string &path, std::string &value_str) {
  std::string::size_type i = s.find(ASSIGN);
  if (i == std::string::npos) {
    path = s;
    return false;
  }

  path = s.substr(0, i);
  value_str = s.substr(i + 1);
  return true;
}


static bool
valid_option(const char *s) {
  if (!s)
    die("illegal null string in command line arguments", "(null)");
  return *s == SEP;
}


template <class Cfg>
static void
load_config(Cfg &cfg, RecursiveCheck &recursive_check, std::istream &in, const std::string &uri) {
  if (recursive_check.count(uri)) {
    int pnlines = recursive_check[uri];
    throw ConfigError("the configuration file has included itself recursively", uri, pnlines);
  }

  int nlines = 0;
  std::string buffer;
  while (std::getline(in, buffer)) {
    ++nlines;
    recursive_check[uri] = nlines;

    // ignore blank lines and comments
    if (buffer.size() == 0 || buffer[0] == '#')
      continue;

    std::string name, value_str;
    split_assignment(buffer, name, value_str);

    name = strip_whitespace(name);
    value_str = strip_whitespace(strip_quotes(value_str));

    if (cfg.ignore_additional() && !cfg.has(name))
      continue;

    try {
      cfg.get(name).set(value_str);
    }
    catch (ConfigError e) {
      // add the configuration file information to this exception
      throw ConfigError(e.msg, e.option, uri, nlines);
    }
  }

  recursive_check.erase(uri);
}


// ============================================================================
// schwa::config::Cfg
// ============================================================================
Cfg::Cfg(const std::string &name, const std::string &desc, Flags flags, unsigned short order) : Node(name, desc, flags, order), _option_help(*this), _option_more_help(*this), _option_config(*this) { }

Cfg::Cfg(Cfg &cfg, const std::string &name, const std::string &desc, Flags flags, unsigned short order) : Node(name, desc, flags, order), _option_help(*this), _option_config(*this) {
  cfg.reg(*this);
}

std::string
Cfg::derived_path(const OpPath &base, const std::string &filename) const {
  return base() + schwa::port::DIR_SEP + filename;
}

std::string
Cfg::derived_temp_path(const OpPath &base, const OpPath &temp, const std::string &filename) const {
  if (schwa::cluster::USE_MPI)
    return temp() + schwa::port::DIR_SEP + filename + '.' + cluster::rank_str;
  return base() + schwa::port::DIR_SEP + filename;
}


void
Cfg::reg(Node &child, Flags flags) {
  for (Nodes::iterator i = _children.begin(); i != _children.end(); ++i) {
    if (*i && (*i)->order > child.order) {
      _children.insert(i, &child);
      return;
    }
  }

  _children.push_back(&child);
  child.flags |= flags;
}


bool
Cfg::has(const std::string &path) const {
  try {
    std::string tail;
    for (Nodes::const_iterator i = _children.begin(); i != _children.end(); ++i) {
      if (*i && (*i)->match(path, tail))
        return tail == "" || (*i)->has(tail);
    }
    return false;
  }
  catch(ConfigError e) {
    if (NAME == "")
      throw;
    throw ConfigError(e, NAME + SEP + e.option);
  }
}


Node &
Cfg::get(const std::string &path) {
  try {
    std::string tail;
    for (Nodes::const_iterator i = _children.begin(); i != _children.end(); ++i)
      if (*i && (*i)->match(path, tail)) {
        if (tail != "")
          return (*i)->get(tail);
        return **i;
      }

    if (ignore_missing())
      // return a dummy option if we are ignoring missing values
      return _option_dummy;
    else {
      schwa::config::die("option does not exist", path);
      // we never get here, but this stops the compiler warning
      return *static_cast<Node *>(0);
    }
  }
  catch(ConfigError e) {
    if (NAME == "")
      throw;
    throw ConfigError(e, NAME + SEP + e.option);
  }
}


void
Cfg::check(void) {
  try {
    for (Nodes::iterator i = _children.begin(); i != _children.end(); ++i)
      if (*i)
        (*i)->check();
  }
  catch(ConfigError e) {
    if (NAME == "")
      throw;
    throw ConfigError(e, NAME + SEP + e.option);
  }
}

void
Cfg::load(std::istream &in, const std::string &uri) {
  RecursiveCheck recursive_check;
  load_config(*this, recursive_check, in, uri);
}


void
Cfg::load(const std::string &uri, bool ignore_ioerror) {
  std::ifstream in(uri.c_str());
  if (!in) {
    if (ignore_ioerror)
      return;
    throw schwa::IOException("could not open model configuration file for reading", uri);
  }
  load(in, uri);
}


void
Cfg::write_config(std::ostream &out, std::string prefix, bool root) const {
  if (!root)
    prefix += NAME + SEP;
  for (Nodes::const_iterator i = _children.begin(); i != _children.end(); ++i) {
    assert(*i);
    if ((*i)->hide_config())
      continue;
    if ((*i)->add_space())
      out << '\n';
    out << "# " << (*i)->DESC << '\n';
    (*i)->writeln_config(out, prefix, false);
  }
}


void
Cfg::write_preface(std::ostream &out, std::string prefix, bool root) const {
  if (!root)
    prefix += NAME + SEP;
  for (Nodes::const_iterator i = _children.begin(); i != _children.end(); ++i) {
    assert(*i);
    if ((*i)->hide_preface())
      continue;
    if ((*i)->add_space())
      out << "#\n";
    (*i)->writeln_preface(out, prefix, false);
  }
}


void
Cfg::write_help(std::ostream &out, std::string prefix, bool full) const {
  if (!NAME.empty())
    prefix += NAME + SEP;
  for (Nodes::const_iterator i = _children.begin(); i != _children.end(); ++i) {
    assert(*i);
    if ((*i)->hide_help() || (!full && (*i)->has_children()))
      continue;
    if ((*i)->add_space())
      out << '\n';
    (*i)->writeln_help(out, prefix, full);
  }
}


// ============================================================================
// schwa::config::Main
// ============================================================================
void
Main::check(void) {
  try {
    Cfg::check();
  }
  catch (ConfigError &e) {
    std::cerr << schwa::port::BOLD << schwa::port::RED << PROGRAM_NAME << ':';
    std::cerr << e.option << ':' << e.msg;
    if (e.uri != "") {
      std::cerr << ':' << e.uri;
      if (e.line)
        std::cerr << ':' << e.line;
    }
    std::cerr << schwa::port::OFF << "\n\n";
    _option_help.usage(1, false);
  }
}


void
Main::parse(const int argc, char * const *argv) {
  for (int i = 1; i != argc; ++i) {
    if (!valid_option(argv[i]))
      schwa::config::die("unrecognised command line option", argv[i]);

    std::string arg = argv[i];

    std::string path;
    std::string value_str = "true";
    bool assigned_arg = split_assignment(arg, path, value_str);

    if (path.size() == 2)
      schwa::config::die("short options not implemented yet", path);
    else
      path = path.substr(1 + (path[1] == SEP));

    Node &node = get(path);
    bool force_arg = node.needs_arg() && !assigned_arg;

    if (i + 1 == argc) {
      if (force_arg)
        schwa::config::die("command line option requires an argument", arg);
    }
    else {
      if (!valid_option(argv[i + 1]) || force_arg)
        value_str = argv[++i];
    }

    node.set(value_str);
  }
}


// ============================================================================
// schwa::config::Main
// ============================================================================
Directory::Directory(const std::string &name, const std::string &desc, const OpPath *base, Flags flags) : Cfg(name, desc, flags), _loaded(false), path(name, "the " + name + " model(s) directory", base) {
  if (base)
    path.set_default("//" + name);
}

Directory::Directory(Cfg &cfg, const std::string &name, const std::string &desc, const OpPath *base, Flags flags) : Cfg(cfg, name, desc, flags), _loaded(false), path(name, "the " + name + " model(s) directory", base) {
  if (base)
    path.set_default("//" + name);
}

void
Directory::set(const std::string &value_str) {
  std::string config_file = value_str + schwa::port::DIR_SEP + "config";
  load(config_file, ignore_missing());
  _loaded = true;
  path.set(value_str);
}

void
Directory::check(void) {
  path.check();
  Cfg::check();
}

void
Directory::reload(void) {
  load(config(), true);
  _loaded = true;
}

void
Directory::write_help(std::ostream &out, std::string prefix, bool full) const {
  path.writeln_help(out, prefix, full);
  Cfg::write_help(out, prefix, full);
}

void
Directory::write_desc(std::ostream &out, bool full) const {
  path.write_desc(out, full);
}

} }
