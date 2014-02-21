/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/op.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <schwa/config/exception.h>
#include <schwa/config/group.h>
#include <schwa/config/main.h>
#include <schwa/version.h>

namespace io = schwa::io;


namespace schwa {
namespace config {

Option::Option(Group &group, const std::string &name, const std::string &desc, const Flags flags, const bool has_default) :
    ConfigNode(name, desc, flags),
    _has_default(has_default),
    _was_mentioned(false),
    _was_assigned(false) {
  group.add(*this);
}


ConfigNode *
Option::find(const std::string &key) {
  return (key == _name) ? this : nullptr;
}


bool
Option::accepts_assignment(void) const {
  return true;
}


bool
Option::accepts_mention(void) const {
  return true;
}


void
Option::assign(const std::string &value) {
  _assign(value);
  _was_assigned = true;
}


void
Option::mention(void) {
  _was_mentioned = true;
}


bool
Option::validate(const Main &main) {
  if (!_was_assigned && accepts_assignment() && !optional()) {
    if (!_has_default) {
      std::ostringstream ss;
      ss << "Configuration option \"" << _name << "\" is unset";
      throw ConfigException(ss.str());
    }
    set_default();
  }
  return _validate(main);
}


// ============================================================================
// OpIStream
// ============================================================================
OpIStream::OpIStream(Group &group, const std::string &name, const std::string &desc, const Flags flags) : OpIStream(group, name, desc, STDIN_STRING, flags) { }

OpIStream::OpIStream(Group &group, const std::string &name, const std::string &desc, const std::string &default_, const Flags flags) :
    Op<std::string>(group, name, desc, default_, flags),
    _in(nullptr),
    _is_stdin(false)
  { }

OpIStream::~OpIStream(void) {
  if (!_is_stdin)
    delete _in;
}


bool
OpIStream::_validate(const Main &) {
  if (_value == STDIN_STRING) {
    _is_stdin = true;
    _in = &std::cin;
  }
  else {
    _is_stdin = false;
    _in = new std::ifstream(_value);
    if (!*_in)
      throw IOException("Could not open file for reading", _value);
  }
  return true;
}


// ============================================================================
// OpOStream
// ============================================================================
OpOStream::OpOStream(Group &group, const std::string &name, const std::string &desc, const Flags flags) : OpOStream(group, name, desc, STDOUT_STRING, flags) { }

OpOStream::OpOStream(Group &group, const std::string &name, const std::string &desc, const std::string &default_, const Flags flags) :
    Op<std::string>(group, name, desc, default_, flags),
    _out(nullptr),
    _is_std(false)
  { }

OpOStream::~OpOStream(void) {
  if (!_is_std)
    delete _out;
}


bool
OpOStream::_validate(const Main &) {
  if (_value == STDOUT_STRING) {
    _is_std = true;
    _out = &std::cout;
  }
  else if (_value == STDERR_STRING) {
    _is_std = true;
    _out = &std::cerr;
  }
  else {
    _is_std = false;
    _out = new std::ofstream(_value);
    if (!*_out)
      throw IOException("Could not open file for writing", _value);
  }
  return true;
}


// ============================================================================
// OpLogLevel
// ============================================================================
OpLogLevel::OpLogLevel(Group &group, const std::string &name, const std::string &desc, const std::string &default_) :
    OpChoices<std::string>(group, name, desc, {"critical", "error", "warning", "info", "debug"}, default_),
    _level(io::LogLevel::INFO)
  { }

OpLogLevel::~OpLogLevel(void) { }


bool
OpLogLevel::_validate(const Main &main) {
  if (!OpChoices<std::string>::_validate(main))
    return false;
  if (_value == "critical")
    _level = io::LogLevel::CRITICAL;
  else if (_value == "error")
    _level = io::LogLevel::ERROR;
  else if (_value == "warning")
    _level = io::LogLevel::WARNING;
  else if (_value == "info")
    _level = io::LogLevel::INFO;
  else if (_value == "debug")
    _level = io::LogLevel::DEBUG;
  else
    return false;
  return true;
}


// ============================================================================
// CommandOption
// ============================================================================
void
CommandOption::_assign(const std::string &) {
  throw Exception("should never be called");
}


void
CommandOption::_help_self(std::ostream &out, const unsigned int depth) const {
  for (unsigned int i = 0; i != depth; ++i)
    out << "  ";
  out << port::BOLD << "--" << _full_name << port::OFF << ": " << _desc;
}


void
CommandOption::_help(std::ostream &out, const unsigned int depth) const {
  _help_self(out, depth);
  out << std::endl;
}


bool
CommandOption::accepts_assignment(void) const {
  return false;
}


void
CommandOption::serialise(std::ostream &) const {
  throw Exception("should never be called");
}


void
CommandOption::set_default(void) {
  throw Exception("should never be called");
}


// ============================================================================
// OpHelp
// ============================================================================
bool
OpHelp::_validate(const Main &main) {
  if (_was_mentioned) {
    main.help(std::cerr);
    return false;
  }
  return true;
}


// ============================================================================
// OpVersion
// ============================================================================
bool
OpVersion::_validate(const Main &main) {
  if (_was_mentioned) {
    std::cerr << port::BOLD << main.name() << port::OFF << ": " << VERSION << std::endl;
    return false;
  }
  return true;
}

}  // namespace config
}  // namespace schwa
