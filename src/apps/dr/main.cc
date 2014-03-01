/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cerrno>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <type_traits>
#include <utility>
#include <vector>

#include <unistd.h>

#include <schwa/config.h>
#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/port.h>

#include <dr-count/main.h>
#include <dr-dist/main.h>
#include <dr-head/main.h>
#include <dr-tail/main.h>
#include <dr-ui/main.h>


namespace cf = schwa::config;
namespace io = schwa::io;
namespace port = schwa::port;

namespace {

// ============================================================================
// Config option parser
// ============================================================================
class Main : public cf::Main {
protected:
  std::vector<std::pair<const char *, const char *>> _commands;

  inline void _add(const char *name, const char *desc) { _commands.push_back(std::pair<const char *, const char *>(name + 3, desc)); }
  virtual void _help_self(std::ostream &out, const unsigned int) const override;

public:
  Main(const std::string &name, const std::string &desc);
};


Main::Main(const std::string &name, const std::string &desc) : cf::Main(name, desc) {
  _add(schwa::dr_count::PROGRAM_NAME, schwa::dr_count::PROGRAM_DESC);
  _add(schwa::dr_dist::PROGRAM_NAME, schwa::dr_dist::PROGRAM_DESC);
  _add(schwa::dr_head::PROGRAM_NAME, schwa::dr_head::PROGRAM_DESC);
  _add(schwa::dr_tail::PROGRAM_NAME, schwa::dr_tail::PROGRAM_DESC);
  _add(schwa::dr_ui::PROGRAM_NAME, schwa::dr_ui::PROGRAM_DESC);
}


void
Main::_help_self(std::ostream &out, const unsigned int) const {
  out << port::BOLD << _full_name << port::OFF << ": " << _desc << std::endl;
  //out << "  Usage: " << _full_name << " [options] (dist|head|list-stores|tail|ui) [command options]" << std::endl;
  out << "  Usage: " << _full_name << " [options] (";
  for (decltype(_commands)::size_type i = 0; i != _commands.size(); ++i) {
    if (i != 0)
      out << '|';
    out << _commands[i].first;
  }
  out << ") [command options]" << std::endl;
  for (auto &pair : _commands)
    out << "    " << port::BOLD << pair.first << port::OFF << ": " << pair.second << std::endl;
}


// ============================================================================
// main
// ============================================================================
static void
_main(int argc, char **argv) {
  // The first unclaimed argument has to be the name of the app to call.
  if (argc == 1)
    throw cf::ConfigException("Positional argument {command} is missing.");

  // Construct the array to pass to execvp.
  // dr ui --help (argc == 3)
  std::unique_ptr<char *[]> args(new char*[argc]);
  std::memcpy(args.get() + 1, argv + 2, (argc - 2) * sizeof(char *));
  args[argc - 1] = nullptr;

  // Construct the name of the binary to exec.
  const size_t len = std::strlen(argv[1]);
  std::unique_ptr<char[]> app(new char[3 + len + 1]);
  std::memcpy(app.get() + 0, "dr-", 3);
  std::memcpy(app.get() + 3, argv[1], len + 1);

  // First, try looking in the the directory of argv[0].
  std::string path = port::abspath_to_argv0();
  path = port::path_dirname(path);
  path = port::path_join(path, app.get());
  std::unique_ptr<char[]> abspath_app(new char[path.size() + 1]);
  args[0] = abspath_app.get();
  std::strcpy(args[0], path.c_str());
  execv(args[0], args.get());

  // If the first exaec attempt failed, try exec'ing using the current definition of PATH.
  args[0] = app.get();
  execvp(args[0], args.get());

  // If we still failed to find the executable, fail.
  const int errnum = errno;
  std::ostringstream ss;
  if (errnum == ENOENT) {
    ss << "Command '" << argv[1] << "' not found.";
    throw cf::ConfigException(ss.str());
  }
  else {
    ss << "Failed to exec dr-" << argv[1] << ": " << std::strerror(errnum) << ".";
    throw schwa::Exception(ss.str());
  }
}

}


int
main(int argc, char **argv) {
  // Construct an option parser.
  Main cfg("dr", "A dispatcher to other docrep processing tools.");

  // Parse argv.
  cfg.allow_unclaimed_args(true);
  cfg.main<io::PrettyLogger>(argc, argv);

  // Dispatch to main function.
  try {
    _main(argc, argv);
  }
  catch (cf::ConfigException &e) {
    std::cerr << schwa::print_exception("ConfigException", e) << std::endl;
    cfg.help(std::cerr);
    return 1;
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return 0;
}
