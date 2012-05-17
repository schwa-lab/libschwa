/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/config.h>


namespace schwa { namespace config {

const std::string Option::REQUIRED = std::string(" (") + schwa::port::BOLD + "REQUIRED" + schwa::port::OFF + ')';


// ============================================================================
// schwa::config::Option
// ============================================================================
Option::Option(const std::string &name, const std::string &desc, Flags flags) : Node(name, desc, flags) { }

Option::Option(Cfg &cfg, const std::string &name, const std::string &desc, Flags flags) : Node(name, desc, flags) {
  cfg.reg(*this);
}

void
Option::die_invalid(const std::string &val) const {
  die("option argument is invalid (argument '" + val + "')");
}

void
Option::check(void) {
  if (!is_valid())
    die("required option argument is undefined");
}

void
Option::write_config(std::ostream &out, std::string prefix, bool) const {
  out << prefix << NAME;
}

void
Option::write_preface(std::ostream &out, std::string prefix, bool root) const {
  out << "#    ";
  write_config(out, prefix, root);
}

void
Option::write_help(std::ostream &out, std::string prefix, bool full) const {
  out << prefix << NAME;
  if (needs_arg())
    out << " <arg>";
  out << ": ";
  write_desc(out, full);
}

void
Option::writeln_preface(std::ostream &out, std::string prefix, bool root) const {
  if (!has_changed())
    return;
  write_preface(out, prefix, root);
  out << '\n';
}


// ============================================================================
// schwa::config::Alias
// ============================================================================
Alias::Alias(Node &node, const std::string &name, const std::string &other) : Node(name, "(alias for " + other + ')', HIDE_CONFIG | HIDE_PREFACE), _node(node), _other(other) { }

Alias::Alias(Cfg &cfg, Node &node, const std::string &name, const std::string &other) : Node(name, "(alias for " + other + ')', HIDE_CONFIG | HIDE_PREFACE), _node(node), _other(other) {
  cfg.reg(*this);
}

Alias::Alias(Cfg &cfg, Flags flags, Node &node, const std::string &name, const std::string &other) : Node(name, "(alias for " + other + ')', flags | HIDE_CONFIG | HIDE_PREFACE), _node(node), _other(other) {
  cfg.reg(*this);
}

void
Alias::write_help(std::ostream &out, std::string prefix, bool full) const {
  out << prefix << NAME;
  if (needs_arg())
    out << " <arg>";
  out << ": ";
  write_desc(out, full);
}

void
Alias::write_desc(std::ostream &out, bool full) const {
  _node.write_desc(out, full);
  if (full && _node.NAME != NAME)
    out << " (alias for --" << _other << ')';
}

void
Alias::check(void) {
  try {
    _node.check();
  }
  catch(ConfigError e) {
    throw ConfigError(e, NAME);
  }
}


// ============================================================================
// schwa::config::OpVersion
// ============================================================================
void
OpVersion::set(const std::string &) {
  std::cerr << Main::PROGRAM_NAME << ' ' << VERSION << ' ' << BUILD << '\n';
  std::exit(0);
}

void
OpVersion::write_config(std::ostream &out, std::string prefix, bool) const {
  out << prefix << NAME << " = " << VERSION << ' ' << BUILD << '\n';
}


// ============================================================================
// schwa::config::OpLicence
// ============================================================================
void
OpLicence::set(const std::string &) {
  std::cerr << LICENCE;
  std::exit(0);
}


// ============================================================================
// schwa::config::OpHelp
// ============================================================================
void
OpHelp::usage(int exitcode, bool full) const {
  std::cerr << "usage: " << Main::PROGRAM_NAME << " [options]\n\n";
  if (_cfg) {
    _cfg->write_help(std::cerr, "  --", full);
    std::cerr << '\n';
  }
  std::exit(exitcode);
}

void
OpHelp::set(const std::string &) {
  usage(0, false);
}


// ============================================================================
// schwa::config::OpMoreHelp
// ============================================================================
void
OpMoreHelp::set(const std::string &) {
  usage(0, true);
}


// ============================================================================
// schwa::config::OpCfg
// ============================================================================
void
OpCfg::set(const std::string &value_str) {
  assert(_cfg);

  std::ifstream in(value_str.c_str());
  if (!in)
    schwa::IOException("could not open configuration file for reading", value_str);
  _cfg->load(in, value_str);
}


// ============================================================================
// schwa::config::OpPath
// ============================================================================
std::string
OpPath::get_value(void) const {
  if (BASE && value[0] == schwa::port::DIR_SEP && value[1] == schwa::port::DIR_SEP)
    return (*BASE)() + schwa::port::DIR_SEP + value.substr(2);
  else
    return value;
}

} }
