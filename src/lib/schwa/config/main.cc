/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/main.h>

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


Main::Main(const std::string &name, const std::string &desc) : Group(name, desc) {
  _owned.push_back(new OpHelp(*this));
  _owned.push_back(new OpVersion(*this));
  _owned.push_back(_log = new OpOStream(*this, "log", "The file to log to", OpOStream::STDERR_STRING));
  _owned.push_back(_log_level = new OpLogLevel(*this, "log-level", "The level to log at", "info"));
  _owned.push_back(_load_config = new OpLoadConfig(*this, "load-config", "The file to load config from"));
  _owned.push_back(_save_config = new OpSaveConfig(*this, "save-config", "The file to save the config to"));
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

    const size_t pos = key.find("--");
    if (pos != 0)
      throw_config_exception("Invalid option", key);

    // Find the config node corresponding to the key.
    ConfigNode *const node = find(key.substr(2));
    if (node == nullptr)
      throw_config_exception("Unknown option", key);

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
