/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/exception.h>

#include <sstream>


namespace schwa {
namespace config {

ConfigException::ConfigException(const std::string &msg) :
    ConfigException(msg, "", "")
  { }

ConfigException::ConfigException(const std::string &msg, const std::string &name) :
    ConfigException(msg, name, "")
  { }

ConfigException::ConfigException(const std::string &msg, const std::string &name, const std::string &value) :
    Exception(msg),
    msg(msg),
    name(name),
    value(value) {
  std::stringstream ss;
  ss << msg;
  if (!name.empty()) {
    ss << " for option \"" << name << "\"";
    if (!value.empty())
      ss << " (value \"" << value << "\")";
  }
  const_cast<std::string &>(_what) = ss.str();
}

ConfigException::ConfigException(const ConfigException &o) :
    Exception(o),
    msg(o.msg),
    name(o.name),
    value(o.value),
    _what(o._what)
  { }


const char *
ConfigException::what(void) const throw() {
  return _what.c_str();
}

}  // namespace config
}  // namespace schwa
