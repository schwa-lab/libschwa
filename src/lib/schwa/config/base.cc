/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/config.h>


namespace schwa { namespace config {

const char *
ConfigException::what(void) const throw() {
  std::stringstream ss;
  ss << msg;
  if (!name.empty()) {
    ss << ": " << name;
    if (!value.empty())
      ss << " to value \"" << value << "\"";
  }
  return ss.str().c_str();
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
OptionBase::OptionBase(const std::string &name, const std::string &desc) : _name(name), _desc(desc) {
  if (name.empty())
    throw ConfigException("Option names cannot be empty", _name);

  if (name.size() > 1) {
    for (auto &c : name) {
      if (c == '-')
        throw ConfigException("Option names cannot contain dashes", _name);
      if (std::isspace(c))
        throw ConfigException("Option names cannot contain spaces", _name);
    }
  }
}

} }
