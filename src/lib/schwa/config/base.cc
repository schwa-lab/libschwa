/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/base.h>

#include <sstream>

#include <schwa/config/exception.h>


namespace schwa {
namespace config {

ConfigNode::ConfigNode(const std::string &name, const std::string &desc) : _name(name), _desc(desc) {
  // Option names cannot be empty.
  if (name.empty())
    throw ConfigException("Option names cannot be empty");

  // Option names cannot contain whitespace.
  for (auto &c : name) {
    if (std::isspace(c)) {
      std::ostringstream ss;
      ss << "Option names cannot contain whitespace characters: \"" << _name << "\"";
      throw ConfigException(ss.str());
    }
  }

  // Option names cannot contain two dashes as they are the group-level separator.
  if (name.find("--") != std::string::npos) {
    std::ostringstream ss;
    ss << "Option names cannot contain \"--\": \"" << _name << "\"";
    throw ConfigException(ss.str());
  }
}

}  // namespace config
}  // namespace schwa
