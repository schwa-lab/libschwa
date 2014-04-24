/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cerrno>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <type_traits>
#include <utility>
#include <vector>

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <schwa/exception.h>
#include <schwa/port.h>
#include <schwa/version.h>

#include <dr-count/main.h>
#include <dr-dist/main.h>
#include <dr-grep/main.h>
#include <dr-head/main.h>
#include <dr-sample/main.h>
#include <dr-tail/main.h>
#include <dr-ui/main.h>

namespace port = schwa::port;

static const std::string TOOL_PREFIX = "dr-";


namespace {

// Smart pointer deleter for DIR structs.
struct closedir_delete {
  inline void operator ()(DIR *d) { if (d != nullptr) { ::closedir(d); } }
};


static void
find_tools_in_dir(const std::string dir_path, std::map<std::string, std::string> &tools) {
  struct stat st;

  // Open the directory.
  std::unique_ptr<DIR, closedir_delete> dir(::opendir(dir_path.c_str()));
  if (dir.get() == nullptr)
    return;

  // For each file in the directory.
  for (struct dirent *de; (de = ::readdir(dir.get())) != nullptr; ) {
    // Does the file start with the tool prefix?
    if (::strncmp(de->d_name, TOOL_PREFIX.c_str(), TOOL_PREFIX.size()) != 0)
      continue;

    // Is the file a regular file and executable?
    const std::string path = port::path_join(dir_path, de->d_name);
    if (::stat(path.c_str(), &st) != 0 || !S_ISREG(st.st_mode) || (st.st_mode & S_IXUSR) == 0)
      continue;

    // Insert it into the map of known tools if we have not already found a tool with this name.
    const char *tool_name = de->d_name + TOOL_PREFIX.size();
    auto it = tools.find(tool_name);
    if (it == tools.end())
      tools.emplace(tool_name, dir_path);
  }
}


static void
show_tools_help(std::ostream &out=std::cerr) {
  std::map<std::string, std::string> tools;

  // Search `dirname ${0}`.
  std::string path = port::path_dirname(port::abspath_to_argv0());
  find_tools_in_dir(path, tools);
  if (!tools.empty()) {
    out << "  Core tools:" << std::endl;
    for (const auto &pair : tools)
      out << "    " << port::BOLD << pair.first << port::OFF << std::endl;
  }
  tools.clear();

  // Search each of the paths in ${PATH}.
  std::vector<std::string> paths;
  port::get_env_paths(paths, "PATH");
  for (auto path : paths)
    find_tools_in_dir(path, tools);
  if (!tools.empty()) {
    out << "  Additional tools found:" << std::endl;
    for (const auto &pair : tools) {
      out << "    " << port::BOLD << pair.first << port::OFF;
      out << " (" << port::DARK_GREY << pair.second << port::OFF << ")" << std::endl;
    }
  }
}


static void
show_help(std::ostream &out=std::cerr) {
  out << port::BOLD << "dr" << port::OFF << ": " << "A dispatcher to other docrep processing tools." << std::endl;
  out << "  Usage: dr tool [tool-options]" << std::endl;
  show_tools_help(out);
  out << std::endl;
  out << "  " << port::BOLD << "-h, --help" << port::OFF << ": Displays the help text" << std::endl;
  out << "  " << port::BOLD << "--version" << port::OFF << ": Displays the version" << std::endl;
}


static void
show_version(std::ostream &out=std::cout) {
  out << port::BOLD << "dr" << port::OFF << ": " << schwa::VERSION << std::endl;
}


static void
do_main(int argc, char **argv) {
  // Construct the array to pass to execvp.
  // dr ui --help (argc == 3)
  std::unique_ptr<char *[]> args(new char*[argc]);
  std::memcpy(args.get() + 1, argv + 2, (argc - 2) * sizeof(char *));
  args[argc - 1] = nullptr;

  // Construct the name of the binary to exec.
  const size_t len = std::strlen(argv[1]);
  std::unique_ptr<char[]> app(new char[TOOL_PREFIX.size() + len + 1]);
  std::memcpy(app.get(), TOOL_PREFIX.c_str(), TOOL_PREFIX.size());
  std::memcpy(app.get() + TOOL_PREFIX.size(), argv[1], len + 1);

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
  if (errnum == ENOENT)
    ss << "Command '" << argv[1] << "' not found.";
  else
    ss << "Failed to exec dr-" << argv[1] << ": " << std::strerror(errnum) << ".";
  throw schwa::Exception(ss.str());
}

}


int
main(int argc, char **argv) {
  // Parse argv.
  if (argc == 1) {
    show_help();
    return 1;
  }
  else if (argc > 1) {
    if (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0) {
      show_help();
      return 0;
    }
    else if (std::strcmp(argv[1], "--version") == 0) {
      show_version();
      return 0;
    }
  }

  // Dispatch to main function.
  try {
    do_main(argc, argv);
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    show_help();
    return 1;
  }
  return 0;
}
