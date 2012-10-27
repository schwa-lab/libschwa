/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/group.h>

#include <iostream>

#include <schwa/config/exception.h>
#include <schwa/port.h>


namespace schwa {
namespace config {

OpGroup::OpGroup(OpGroup &group, const std::string &name, const std::string &desc) : OptionBase(name, desc) {
  group.add(this);
}


OptionBase *
OpGroup::find(const std::string &orig_key, const std::string &key) {
  const size_t pos = key.find(_name);
  if (pos != 0)
    return nullptr;
  else if (key.size() == _name.size())
    return this;
  else if (key[_name.size()] != '-')
    throw ConfigException("Invalid option key", orig_key);

  const std::string new_key = key.substr(_name.size() + 1);
  if (new_key.empty())
    throw ConfigException("Invalid option key", orig_key);

  for (auto &child : _children) {
    OptionBase *const p = child->find(orig_key, new_key);
    if (p != nullptr)
      return p;
  }
  return nullptr;
}


void
OpGroup::help(std::ostream &out, const std::string &prefix, unsigned int depth) const {
  const std::string me = prefix + _name + "-";
  if (depth != 0)
    out << std::endl;
  out << port::BOLD << prefix << _name << port::OFF << ": " << _desc << std::endl;
  for (auto &child : _children)
    child->help(out, me, depth + 1);
}


void
OpGroup::set(const std::string &value) {
  throw ConfigException("Cannot set the value of a option group", _name, value);
}


void
OpGroup::validate(void) {
  for (auto &child : _children)
    child->validate();
}

}  // namespace config
}  // namespace schwa
