/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/main.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>
#include <sstream>

#include <schwa/config/exception.h>
#include <schwa/port.h>
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


bool
Main::process(const int argc, char **const argv) {
  // Place the arguments into a queue for ease of processing.
  std::queue<std::string> args;
  for (int i = 0; i != argc; ++i)
    args.push(argv[i]);

  // Try and assign all of the arguments to nodes.
  while (!args.empty()) {
    const std::string &key = args.front();
    args.pop();

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
      args.pop();
      node->assign(value);
    }
  }

  // Validate each of the nodes.
  return validate(*this);
}

}  // namespace config
}  // namespace schwa
