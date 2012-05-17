/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/config.h>


namespace schwa { namespace config {

bool
Node::match(const std::string &path, std::string &tail) {
  std::string::size_type len = NAME.size();

  if (path.substr(0, len) != NAME)
    return false;

  if (path.size() == len) {
    tail = "";
    return true;
  }

  if (path[len] == SEP) {
    tail = path.substr(len + 1);
    return true;
  }

  return false;
}


void
Node::write_desc(std::ostream &out, bool) const {
  out << DESC;
}

void
Node::writeln_help(std::ostream &out, std::string prefix, bool full) const {
  write_help(out, prefix, full);
  out << '\n';
}

void
Node::writeln_config(std::ostream &out, std::string prefix, bool root) const {
  write_config(out, prefix, root);
  out << '\n';
}

void
Node::writeln_preface(std::ostream &out, std::string prefix, bool root) const {
  write_preface(out, prefix, root);
  out << '\n';
}

void
Node::write_help(const std::string &filename, const std::string &PREFACE, bool full) const {
  std::ofstream out(filename.c_str());
  if (!out)
    throw schwa::IOException("could not open " + NAME + " file for writing help", filename);
  out << PREFACE << '\n';
  write_help(out, "", full);
}

void
Node::write_config(const std::string &filename, const std::string &PREFACE, bool root) const {
  std::ofstream out(filename.c_str());
  if (!out)
    throw schwa::IOException("could not open " + NAME + " file for writing config", filename);
  out << PREFACE << '\n';
  write_config(out, "", root);
}

void
Node::write_preface(const std::string &filename, const std::string &PREFACE, bool root) const {
  std::ofstream out(filename.c_str());
  if (!out)
    throw schwa::IOException("could not open " + NAME + " file for writing preface", filename);
  out << PREFACE << '\n';
  write_preface(out, "", root);
}

} }
