/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/main.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <sstream>

#include <schwa/exception.h>
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


Main::Main(const std::string &name, const std::string &desc) : Group(name, desc) {
  _owned.push_back(_op_help = new OpHelp(*this));
  _owned.push_back(_op_version = new OpVersion(*this));
  _owned.push_back(_op_log = new Op<std::string>(*this, "log", "The file to log to", "/dev/stderr"));
  _owned.push_back(_op_log_level = new OpLogLevel(*this, "log-level", "The level to log at", "info"));
  _owned.push_back(_op_load_config = new OpLoadConfig(*this));
  _owned.push_back(_op_save_config = new OpSaveConfig(*this));
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
  out << port::BOLD << full_name() << port::OFF << ": " << desc() << std::endl;
  out << "  Usage: " << full_name() << " [options]";
  for (auto &c : _positional_arg_nodes)
    out << " [" << c->name() << "]";
  if (allow_unclaimed_args())
    out << " " << _unclaimed_args_desc;
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
  // Find the positional argument nodes.
  std::deque<std::string> positional_args;
  _positional_arg_nodes.clear();
  _get_positional_arg_nodes(_positional_arg_nodes);
  std::sort(
      _positional_arg_nodes.begin(),
      _positional_arg_nodes.end(),
      [](const ConfigNode *a, const ConfigNode *b) { return a->positional_precedence() < b->positional_precedence(); }
  );

  // Place the arguments into a queue for ease of processing.
  std::deque<std::string> args;
  for (decltype(_cmdline_args)::size_type i = 1; i != _cmdline_args.size(); ++i)
    args.push_back(_cmdline_args[i]);

  // Try and assign all of the arguments to nodes.
  while (!args.empty()) {
    const std::string key = args.front();
    args.pop_front();

    // Try and find the corresponding config node.
    ConfigNode *const node = _find(key);

    // If the config node wasn't found, store the value and move to the next argument.
    if (node == nullptr) {
      positional_args.push_back(key);
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
    if (node == _op_load_config)
      _op_load_config->load_config(args);
  }

  // Take care of the positional arguments.
  if (!positional_args.empty()) {
    for (auto &c : _positional_arg_nodes) {
      if (positional_args.empty())
        break;
      if (c->accepts_mention() && c->accepts_assignment() && !c->was_mentioned()) {
        c->mention();
        c->assign(positional_args.front());
        positional_args.pop_front();
      }
    }
  }

  // Validate each of the nodes.
  if (!validate(*this))
    return false;

  // If we're not allowing unclaimed args but there are still some positional arguments left, reject.
  if (!allow_unclaimed_args() && !positional_args.empty())
    throw_config_exception("Unknown option or value", positional_args.front());

  // Copy across the remaining positional arguments to the unclaimed arguments.
  _unclaimed_args.clear();
  while (!positional_args.empty()) {
    _unclaimed_args.push_back(positional_args.front());
    positional_args.pop_front();
  }

  // Perform the saving of the config, if required.
  _op_save_config->save_config(*this);

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
