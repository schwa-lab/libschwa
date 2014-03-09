/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/main.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <sstream>

#include <schwa/config/exception.h>
#include <schwa/port.h>
#include <schwa/utils/shlex.h>
#include <schwa/version.h>


namespace schwa {
namespace config {

static void
throw_config_exception(const std::string &msg, const std::string &key) {
  std::ostringstream ss;
  ss << msg << " \"" << key << "\"";
  throw ConfigException(ss.str());
}


Main::Main(const std::string &name, const std::string &desc) :
    Group(name, desc),
    _allow_unclaimed_args(false) {
  _owned.push_back(new OpHelp(*this));
  _owned.push_back(new OpVersion(*this));
  _owned.push_back(_log = new Op<std::string>(*this, "log", "The file to log to", "/dev/stderr"));
  _owned.push_back(_log_level = new OpLogLevel(*this, "log-level", "The level to log at", "info"));
  _owned.push_back(_load_config = new OpLoadConfig(*this));
  _owned.push_back(_save_config = new OpSaveConfig(*this));
}


Main::~Main(void) {
  for (auto &ptr : _owned)
    delete ptr;
}


ConfigNode *
Main::find(const std::string &key) {
  for (auto &child : _options) {
    ConfigNode *const p = child->find(key);
    if (p != nullptr)
      return p;
  }
  for (auto &child : _groups) {
    ConfigNode *const p = child->find(key);
    if (p != nullptr)
      return p;
  }
  return nullptr;
}


void
Main::_post_add(ConfigNode &) { }


void
Main::_help(std::ostream &out, const unsigned int depth) const {
  if (depth != 0)
    out << std::endl;
  _help_self(out, depth);
  out << std::endl;
  auto end = _owned.cend();
  for (auto &child : _options)
    if (std::find(_owned.cbegin(), _owned.cend(), child) == end)
      child->_help(out, depth + 1);
  for (auto &child : _groups)
    if (std::find(_owned.cbegin(), _owned.cend(), child) == end)
      child->_help(out, depth + 1);
  out << std::endl;
  for (auto &child : _owned)
    child->_help(out, depth + 1);
}


void
Main::_help_self(std::ostream &out, const unsigned int) const {
  out << port::BOLD << _full_name << port::OFF << ": " << _desc << std::endl;
  out << "  Usage: " << _full_name << " [options]";
  if (_allow_unclaimed_args)
    out << " [args]";
  out << std::endl;
}


void
Main::help(std::ostream &out) const {
  _help(out, 0);
}


void
Main::serialise(std::ostream &out) const {
  out << "# $";
  serialise_cmdline_args(out);
  out << '\n';
  Group::serialise(out);
}


void
Main::serialise_cmdline_args(std::ostream &out) const {
  for (auto &arg : _cmdline_args)
    out << " " << utils::shlex_quote(arg);
}


ConfigNode *
Main::_find(const std::string &key) {
  if (is_short_option(key))
    return find(key[1]);
  else if (is_long_option(key))
    return find(key.substr(2));
  else
    return nullptr;
}


bool
Main::_main(void) {
  // Place the arguments into a queue for ease of processing.
  std::deque<std::string> args;
  for (decltype(_cmdline_args)::size_type i = 1; i != _cmdline_args.size(); ++i)
    args.push_back(_cmdline_args[i]);

  // Try and assign all of the arguments to nodes.
  while (!args.empty()) {
    const std::string &key = args.front();
    args.pop_front();

    // Try and find the corresponding config node.
    ConfigNode *const node = _find(key);

    // If the config node wasn't found, work out what to do.
    if (node == nullptr) {
      if (_allow_unclaimed_args) {
        _unclaimed_args.push_back(key);
        continue;
      }
      else
        throw_config_exception("Unknown option", key);
    }

    // If we're allowing unclaimed args and we've already started collecting some, continue adding to its collection.
    if (_allow_unclaimed_args && !_unclaimed_args.empty()) {
      _unclaimed_args.push_back(key);
      continue;
    }

    // Ensure the node can be mentioned.
    if (node->accepts_mention())
      node->mention();
    else
      throw_config_exception("Invalid option", key);

    // If the node can be assigned a value, try and assign it one.
    if (node->accepts_assignment()) {
      if (node->requires_assignment() && args.empty())
        throw_config_exception("Value missing for option", key);
      if (!args.empty()) {
        const std::string value = args.front();
        ConfigNode *const value_node = _find(value);
        if (node->requires_assignment() && value_node != nullptr)
          throw_config_exception("Value missing for option", key);
        if (node->requires_assignment() || value_node == nullptr) {
          args.pop_front();
          node->assign(value);
        }
      }
    }

    // If the node is the load-config node, load the config.
    if (node == _load_config)
      _load_config->load_config(args);
  }

  // Validate each of the nodes.
  if (!validate(*this))
    return false;

  // Perform the saving of the config, if required.
  _save_config->save_config(*this);

  return true;
}


inline bool
Main::is_short_option(const std::string &key) {
  return key.size() == 2 && key[0] == '-' && std::isalnum(key[1]);
}


inline bool
Main::is_long_option(const std::string &key) {
  return key.size() > 2 && key[0] == '-' && key[1] == '-';
}

}  // namespace config
}  // namespace schwa
