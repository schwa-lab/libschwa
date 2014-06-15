/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/base.h>

#include <cctype>
#include <sstream>

#include <schwa/exception.h>


namespace schwa {
namespace config {

ConfigNode::ConfigNode(const std::string &name, const std::string &desc, const Flags flags) : ConfigNode(name, '\0', desc, flags) { }

ConfigNode::ConfigNode(const std::string &name, const char short_name, const std::string &desc, const Flags flags) :
    _name(name),
    _desc(desc),
    _full_name(name),
    _short_name(short_name),
    _flags(flags),
    _positional_precedence(-1),
    _was_mentioned(false),
    _was_assigned(false) {
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
  if (_name.find(SEPARATOR) != std::string::npos) {
    std::ostringstream ss;
    ss << "Option names cannot contain \"" << SEPARATOR << "\": \"" << _name << "\"";
    throw ConfigException(ss.str());
  }

  // Short names have to be an alnum, if set.
  if (_short_name && !std::isalnum(_short_name)) {
    std::ostringstream ss;
    ss << "Short option names must be an alnum: '" << _short_name << "'";
    throw ConfigException(ss.str());
  }
}


void
ConfigNode::set_positional_precedence(const int precedence) {
  if (precedence < 0)
    throw ConfigException("precedence value must be >= 0");
  _positional_precedence = precedence;
}


void
ConfigNode::assign(const std::string &value) {
  _assign(value);
  _was_assigned = true;
}


void
ConfigNode::mention(void) {
  _mention();
  _was_mentioned = true;
}


ConfigNode *
ConfigNode::find(const char short_name) {
  return (short_name == _short_name) ? this : nullptr;
}


ConfigNode *
ConfigNode::find(const std::string &key) {
  return (key == _name) ? this : nullptr;
}


void
ConfigNode::set_prefix(const std::string &prefix) {
  _full_name = prefix + SEPARATOR + _name;
}


void
ConfigNode::_get_positional_arg_nodes(std::vector<ConfigNode *> &nodes) {
  if (_positional_precedence != -1)
    nodes.push_back(this);
}

}  // namespace config
}  // namespace schwa
