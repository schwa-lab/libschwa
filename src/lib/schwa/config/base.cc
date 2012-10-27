/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/base.h>

#include <schwa/config/exception.h>


namespace schwa {
namespace config {

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

}  // namespace config
}  // namespace schwa
