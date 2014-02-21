/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/group.h>

#include <cassert>
#include <iostream>
#include <sstream>

#include <schwa/config/exception.h>
#include <schwa/port.h>


namespace schwa {
namespace config {

Group::Group(Group &group, const std::string &name, const std::string &desc) : ConfigNode(name, desc) {
  group.add(*this);
}


ConfigNode *
Group::find(const std::string &key) {
  const size_t pos = key.find(_name);
  if (pos != 0)
    return nullptr;
  if (key == _name)
    return this;

  std::string new_key = key.substr(_name.size());
  if (new_key.find(SEPARATOR) != 0)
    return nullptr;
  new_key = new_key.substr(2);

  for (auto &child : _options) {
    ConfigNode *const p = child->find(new_key);
    if (p != nullptr)
      return p;
  }
  for (auto &child : _groups) {
    ConfigNode *const p = child->find(new_key);
    if (p != nullptr)
      return p;
  }
  return nullptr;
}


void
Group::_help_self(std::ostream &out, const unsigned int depth) const {
  for (unsigned int i = 0; i != depth; ++i)
    out << "  ";
  out << port::BOLD;
  if (accepts_mention())
    out << "--";
  out << _full_name << port::OFF << ": " << _desc;
}


void
Group::_help(std::ostream &out, const unsigned int depth) const {
  if (depth != 0)
    out << std::endl;
  _help_self(out, depth);
  out << std::endl;
  for (auto &child : _options)
    child->_help(out, depth + 1);
  for (auto &child : _groups)
    child->_help(out, depth + 1);
}


void
Group::_pre_add(ConfigNode &child) {
  const std::string &child_name = child.name();
  for (auto &c : _options) {
    if (c->name() == child_name) {
      std::ostringstream ss;
      ss << "Duplicate config option name \"" << child_name << "\"";
      throw ConfigException(ss.str());
    }
  }
  for (auto &c : _groups) {
    if (c->name() == child_name) {
      std::ostringstream ss;
      ss << "Duplicate config option name \"" << child_name << "\"";
      throw ConfigException(ss.str());
    }
  }
}


void
Group::_post_add(ConfigNode &child) {
  child.set_prefix(_name);
}


void
Group::add(Group &child) {
  _pre_add(child);
  _groups.push_back(&child);
  _post_add(child);
}


void
Group::add(Option &child) {
  _pre_add(child);
  _options.push_back(&child);
  _post_add(child);
}


bool
Group::accepts_assignment(void) const {
  return false;
}


bool
Group::accepts_mention(void) const {
  return false;
}


void
Group::assign(const std::string &) {
  assert(false);
}


void
Group::mention(void) {
  assert(false);
}


bool
Group::validate(const Main &main) {
  for (auto &child : _options)
    if (!child->validate(main))
      return false;
  for (auto &child : _groups)
    if (!child->validate(main))
      return false;
  return true;
}

}  // namespace config
}  // namespace schwa
