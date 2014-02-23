/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cerrno>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <type_traits>

#include <unistd.h>

#include <schwa/config.h>
#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/port.h>

namespace cf = schwa::config;
namespace io = schwa::io;
namespace port = schwa::port;

namespace {

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
  args[0] = app.get();
  std::memcpy(app.get() + 0, "dr-", 3);
  std::memcpy(app.get() + 3, argv[1], len + 1);

  // First try exec'ing using the current definition of PATH.
  execvp(app.get(), args.get());

  // If the first exec attempt failed, try looking in the the directory of argv[0].
  std::string path = port::abspath_to_argv0();
  path = port::path_dirname(path);
  path = port::path_join(path, app.get());

  // Try exec'ing again in the new directory.
  app.reset(new char[path.size() + 1]);
  args[0] = app.get();
  std::strcpy(app.get(), path.c_str());
  execv(app.get(), args.get());

  // If we still failed to find the executable, fail.
  const int errnum = errno;
  std::ostringstream ss;
  ss << "Failed to exec dr-" << argv[1] << ": " << std::strerror(errnum) << ".";
  throw schwa::Exception(ss.str());
}

}


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr", "A dispatcher to other docrep binaries. Usage: dr {command} args...", true);

  // Parse argv.
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
