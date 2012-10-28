/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/main.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include <schwa/config/exception.h>
#include <schwa/port.h>
#include <schwa/version.h>


namespace schwa {
namespace config {

OptionBase *
OpMain::find(const std::string &orig_key, const std::string &key) {
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
OpMain::process(const int argc, char **const argv, std::ostream &help_ostream) {
  // do an initial pass for "--help" and "--version"
  for (int i = 0; i != argc; ++i) {
    if (std::strcmp(argv[i], "--help") == 0) {
      help(help_ostream);
      return false;
    }
    if (std::strcmp(argv[i], "--version") == 0) {
      help_ostream << VERSION << std::endl;
      return false;
    }
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


void
OpMain::main(int argc, char **argv) {
  try {
    if (!process(argc - 1, argv + 1, std::cerr))
      std::exit(1);
  }
  catch (ConfigException &e) {
    std::cerr << schwa::print_exception("ConfigException", e) << std::endl;
    help(std::cerr);
    std::exit(1);
  }
}

}  // namespace config
}  // namespace schwa
