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
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/io/paths.h>
#include <schwa/port.h>
#include <schwa/version.h>

namespace io = schwa::io;
namespace port = schwa::port;

static const std::string TOOL_PREFIX = "dr-";


namespace {

/**
 * Smart pointer deleter for DIR structs.
 **/
struct closedir_delete {
  inline
  void operator ()(DIR *d) {
    if (d != nullptr)
      ::closedir(d);
  }
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
    const std::string path = io::path_join(dir_path, de->d_name);
    if (::stat(path.c_str(), &st) != 0 || !S_ISREG(st.st_mode) || (st.st_mode & S_IXUSR) == 0)
      continue;

    // Insert it into the map of known tools if we have not already found a tool with this name.
    const char *tool_name = de->d_name + TOOL_PREFIX.size();
    auto it = tools.find(tool_name);
    if (it == tools.end())
      tools.emplace(tool_name, dir_path);
  }
}


static std::string
get_short_help(const std::string &dir_path, const std::string &tool_name) {
  const std::string path = io::path_join(dir_path, "dr-" + tool_name);
  std::string short_help;

  // Setup pipe.
  int child_to_parent[2];  // [read, write]
  if (::pipe(child_to_parent) == -1) {
    const int errnum = errno;
    LOG(ERROR) << "Call to pipe failed: " << std::strerror(errnum) << std::endl;
    return "";
  }

  // Fork.
  const pid_t pid = ::fork();
  if (pid < 0) {  // Call to fork() failed.
    const int errnum = errno;
    LOG(ERROR) << "Call to fork failed: " << std::strerror(errnum) << std::endl;
    return "";
  }
  else if (pid == 0) {  // Child process.
    // Redirect stdin and stderr to be /dev/null.
    ::close(STDIN_FILENO);
    ::open("/dev/null", O_RDONLY);
    ::close(STDERR_FILENO);
    ::open("/dev/null", O_RDWR);

    // Bind stdout to the writing end of the pipe.
    ::close(child_to_parent[0]);
    ::close(STDOUT_FILENO);
    ::dup2(child_to_parent[1], STDOUT_FILENO);

    // Attempt to exec the tool, invoking the --short-help flag.
    char *args[3] = {::strdup(path.c_str()), ::strdup("--short-help"), nullptr};
    ::execvp(args[0], args);
    std::perror("execvp");
    std::exit(1);
  }
  else {  // Parent process.
    // Wait for the child process to terminate.
    int status;
    ::waitpid(pid, &status, 0);

    // Did the process exited successfully?
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      // Try and read from the pipe, copying the output chars into `short_help`.
      char buf[BUFSIZ];
      while (true) {
        const ssize_t nread = ::read(child_to_parent[0], buf, sizeof(buf));
        for (ssize_t i = 0; i != nread; ++i)
          if (buf[i] != '\n')
            short_help += buf[i];
        if (nread != sizeof(buf))
          break;
      }
    }

    // Close the pipe.
    ::close(child_to_parent[0]);
  }

  return short_help;
}


static void
show_tools_help(std::ostream &out=std::cerr) {
  std::map<std::string, std::string> core_tools, extra_tools;

  // Search `dirname ${0}`.
  const std::string argv0_path = io::path_dirname(io::abspath_to_argv0());
  find_tools_in_dir(argv0_path, core_tools);
  if (!core_tools.empty()) {
    out << "  Core tools:" << std::endl;
    for (const auto &pair : core_tools) {
      const std::string short_help = get_short_help(pair.second, pair.first);

      out << "    " << port::BOLD << pair.first << port::OFF;
      if (!short_help.empty())
        out << ": " << short_help;
      out << std::endl;
    }
  }

  // Search each of the paths in ${PATH}.
  std::vector<std::string> paths;
  io::get_env_paths(paths, "PATH");
  for (const std::string &path : paths)
    find_tools_in_dir(path, extra_tools);

  // Remove the core tools from the set of additional tools found.
  for (const auto &pair : core_tools)
    extra_tools.erase(pair.first);

  // Output any extra tools found.
  if (!extra_tools.empty()) {
    out << "  Additional tools found:" << std::endl;
    for (const auto &pair : extra_tools) {
      out << "    " << port::BOLD << pair.first << port::OFF;
      out << " (" << port::DARK_GREY << pair.second << port::OFF << ")" << std::endl;
    }
  }
}


static void
show_help(const bool short_help=false, std::ostream &out=std::cout) {
  static const std::string DESC = "A dispatcher to other docrep processing tools.";
  if (short_help) {
    out << DESC << std::endl;
    return;
  }
  out << port::BOLD << "dr" << port::OFF << ": " << DESC << std::endl;
  out << "  Usage: dr tool [tool-options]" << std::endl;
  show_tools_help(out);
  out << std::endl;
  out << "  " << port::BOLD << "-h, --help" << port::OFF << ": Displays the help text" << std::endl;
  out << "  " << port::BOLD << "--short-help" << port::OFF << ": Displays the short help text" << std::endl;
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
  std::string path = io::abspath_to_argv0();
  path = io::path_dirname(path);
  path = io::path_join(path, app.get());
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
    if (std::strcmp(argv[1], "--short-help") == 0) {
      show_help(true);
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
