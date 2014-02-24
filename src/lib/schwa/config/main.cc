/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/main.h>

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
  _owned.push_back(_log = new OpOStream(*this, "log", 'l', "The file to log to", OpOStream::STDERR_STRING));
  _owned.push_back(_log_level = new OpLogLevel(*this, "log-level", 'L', "The level to log at", "info"));
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

    // Is it a short name?
    ConfigNode *node = nullptr;
    if (key.size() == 2 && key[0] == '-' && std::isalnum(key[1]))
      node = find(key[1]);
    else {
      // Is it a long name?
      const size_t pos = key.find("--");
      if (pos != 0) {
        if (_allow_unclaimed_args) {
          _unclaimed_args.push_back(key);
          continue;
        }
        else
          throw_config_exception("Invalid option", key);
      }
      node = find(key.substr(2));
    }

    // Was the config node found?
    if (node == nullptr) {
      if (_allow_unclaimed_args) {
        _unclaimed_args.push_back(key);
        continue;
      }
      else
        throw_config_exception("Unknown option", key);
    }
    else if (_allow_unclaimed_args && !_unclaimed_args.empty()) {
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
      if (args.empty())
        throw_config_exception("Value missing for option", key);

      const std::string value = args.front();
      args.pop_front();
      node->assign(value);
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

}  // namespace config
}  // namespace schwa
