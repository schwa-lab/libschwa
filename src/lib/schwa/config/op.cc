/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/op.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <schwa/exception.h>
#include <schwa/config/group.h>
#include <schwa/config/main.h>
#include <schwa/io/streams.h>
#include <schwa/version.h>

namespace io = schwa::io;


namespace schwa {
namespace config {

Option::Option(Group &group, const std::string &name, const std::string &desc, const Flags flags, const bool has_default) :
    ConfigNode(name, desc, flags),
    _has_default(has_default) {
  group.add(*this);
}

Option::Option(Group &group, const std::string &name, const char short_name, const std::string &desc, const Flags flags, const bool has_default) :
    ConfigNode(name, short_name, desc, flags),
    _has_default(has_default) {
  group.add(*this);
}


bool
Option::accepts_assignment(void) const {
  return true;
}


bool
Option::accepts_mention(void) const {
  return true;
}


bool
Option::requires_assignment(void) const {
  return !_has_default;
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
  if (!_was_assigned && accepts_assignment() && _has_default)
    set_default();
  return _validate(main);
}


// ============================================================================
// OpLogLevel
// ============================================================================
OpLogLevel::OpLogLevel(Group &group, const std::string &name, const std::string &desc, const std::string &default_) :
    OpChoices<std::string>(group, name, desc, {"critical", "error", "warning", "info", "debug"}, default_),
    _level(io::LogLevel::INFO)
  { }

OpLogLevel::OpLogLevel(Group &group, const std::string &name, const char short_name, const std::string &desc, const std::string &default_) :
    OpChoices<std::string>(group, name, short_name, desc, {"critical", "error", "warning", "info", "debug"}, default_),
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
// OpSequenceTagEncoding
// ============================================================================
OpSequenceTagEncoding::OpSequenceTagEncoding(Group &group, const std::string &name, const std::string &desc, const std::string &default_) :
    OpChoices<std::string>(group, name, desc, {"iob1", "iob2", "bmewo"}, default_),
    _encoding(SequenceTagEncoding::IOB2)
  { }

OpSequenceTagEncoding::OpSequenceTagEncoding(Group &group, const std::string &name, const char short_name, const std::string &desc, const std::string &default_) :
    OpChoices<std::string>(group, name, short_name, desc, {"iob1", "iob2", "bmewo"}, default_),
    _encoding(SequenceTagEncoding::IOB2)
  { }

OpSequenceTagEncoding::~OpSequenceTagEncoding(void) { }


bool
OpSequenceTagEncoding::_validate(const Main &main) {
  if (!OpChoices<std::string>::_validate(main))
    return false;
  if (_value == "iob1")
    _encoding = SequenceTagEncoding::IOB1;
  else if (_value == "iob2")
    _encoding = SequenceTagEncoding::IOB2;
  else if (_value == "bmewo")
    _encoding = SequenceTagEncoding::BMEWO;
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
  if (short_name())
    out << port::BOLD << '-' << short_name() << port::OFF << ", ";
  out << port::BOLD << SEPARATOR << full_name() << port::OFF << ": " << desc();
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
    main.help(std::cout);
    throw SystemExit(0);
  }
  return true;
}


// ============================================================================
// OpShortHelp
// ============================================================================
bool
OpShortHelp::_validate(const Main &main) {
  if (_was_mentioned) {
    main.help_short(std::cout);
    throw SystemExit(0);
  }
  return true;
}


// ============================================================================
// OpVersion
// ============================================================================
bool
OpVersion::_validate(const Main &main) {
  if (_was_mentioned) {
    std::cout << port::BOLD << main.name() << port::OFF << ": " << VERSION << std::endl;
    throw SystemExit(0);
  }
  return true;
}

}  // namespace config
}  // namespace schwa
