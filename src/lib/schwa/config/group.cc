/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>


namespace schwa { namespace config {

OpGroup::OpGroup(OpGroup &group, const std::string &name, const std::string &desc) : OptionBase(name, desc) {
  group.add(this);
}


OptionBase *
OpGroup::find(const std::string &orig_key, const std::string key) {
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



OptionBase *
OpMain::find(const std::string &orig_key, const std::string key) {
  for (auto &child : _children) {
    OptionBase *const p = child->find(orig_key, key);
    if (p != nullptr)
      return p;
  }
  return nullptr;
}


void
OpMain::help(std::ostream &out, const std::string &prefix, unsigned int depth) const {
  out << port::BOLD << _name << port::OFF << ": " << _desc << std::endl;
  for (auto &child : _children)
    child->help(out, prefix, depth);
}


bool
OpMain::process(const int argc, const char *const argv[], std::ostream &help_ostream) {
  // do an initial pass for "--help"
  for (int i = 0; i != argc; ++i)
    if (std::strcmp(argv[i], "--help") == 0) {
      help(help_ostream);
      return false;
    }

  // ensure we have key/value pairs
  if (argc % 2 != 0) {
    std::stringstream ss;
    ss << "Arguments must be key/value pairs. Invalid number of arguments provided: " << argc;
    throw ConfigException(ss.str());
  }

  // assign out all of the key/value pairs
  for (int i = 0; i != argc; ) {
    const std::string key(argv[i++]);
    const std::string val(argv[i++]);
    if (key.size() <= 2 || key[0] != '-' || key[1] != '-')
      throw ConfigException("Invalid option key", key, val);
    OptionBase *const node = find(key, key.substr(2));
    if (node == nullptr)
      throw ConfigException("Unknown option", _name, key.substr(2));
    node->set(val);
  }

  // validate each of the nodes
  validate();
  return true;
}


} }
