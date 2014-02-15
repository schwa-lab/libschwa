/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/op.h>

#include <fstream>
#include <iostream>

#include <schwa/config/exception.h>
#include <schwa/config/group.h>

namespace io = schwa::io;


namespace schwa {
namespace config {

OpBase::OpBase(OpGroup &group, const std::string &name, const std::string &desc, const bool has_default) :
    OptionBase(name, desc),
    _has_default(has_default),
    _is_set(false) {
  group.add(this);
}


OptionBase *
OpBase::find(const std::string &, const std::string &key) {
  const size_t pos = key.find(_name);
  if (key.empty() || pos != 0 || key.size() != _name.size())
    return nullptr;
  return this;
}


void
OpBase::set(const std::string &value) {
  _set(value);
  _is_set = true;
}


void
OpBase::validate(void) {
  if (!_is_set) {
    if (!_has_default)
      throw ConfigException("Unset configuration option", _name);
    set_default();
  }
  _validate();
}


// ============================================================================
// IStreamOp
// ============================================================================
IStreamOp::~IStreamOp(void) {
  if (!_is_stdin)
    delete _in;
}


void
IStreamOp::_validate(void) {
  if (_value == STDIN_STRING) {
    _is_stdin = true;
    _in = &std::cin;
  }
  else {
    _is_stdin = false;
    _in = new std::ifstream(_value);
    if (!*_in)
      throw ConfigException("Could not open file for reading", _name, _value);
  }
}


// ============================================================================
// OStreamOp
// ============================================================================
OStreamOp::~OStreamOp(void) {
  if (!_is_std)
    delete _out;
}


void
OStreamOp::_validate(void) {
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
      throw ConfigException("Could not open file for writing", _name, _value);
  }
}


// ============================================================================
// LogLevelOp
// ============================================================================
LogLevelOp::LogLevelOp(OpGroup &group, const std::string &name, const std::string &desc, const std::string &default_) :
    EnumOp<std::string>(group, name, desc, {"critical", "error", "warning", "info", "debug"}, default_),
    _level(io::LogLevel::INFO)
  { }

LogLevelOp::~LogLevelOp(void) { }

void
LogLevelOp::_validate(void) {
  EnumOp<std::string>::_validate();
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
}

}  // namespace config
}  // namespace schwa
