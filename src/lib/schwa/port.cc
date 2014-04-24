/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <config.h>
#include <schwa/port.h>

#ifdef HAVE_CXXABI_H
  #include <cxxabi.h>
#endif
#ifdef HAVE_LIBGEN_H
  #include <libgen.h>
#endif
#ifdef HAVE_LIMITS_H
  #include <limits.h>
#endif
#ifdef HAVE_LIBPROC_H
  #include <libproc.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>

#include <schwa/exception.h>
#include <schwa/io/logging.h>


namespace schwa {
namespace port {

// Assume that we always can output colours for the moment.
const char *BOLD = "\033[1m";
const char *DARK_GREY = "\033[1;30m";
const char *OFF = "\033[0m";
const char *RED = "\033[31m";


#ifdef HAVE_CXXABI_H
std::string
demangle_typeid(const char *const typeid_name) {
  char *name;
  size_t size;
  int ret;
  name = abi::__cxa_demangle(typeid_name, nullptr, &size, &ret);
  if (name != nullptr) {
    const std::string out(name);
    std::free(name);
    return out;
  }
  return typeid_name;
}
#else
std::string
demangle_typeid(const char *const typeid_name) {
  return typeid_name;
}
#endif


#if defined(__APPLE__)
std::string
abspath_to_argv0(void) {
  char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
  const pid_t pid = ::getpid();
  if (::proc_pidpath(pid, pathbuf, sizeof(pathbuf)) <= 0) {
    const int errnum = errno;
    std::ostringstream ss;
    ss << "Call to proc_pidpath failed: " << std::strerror(errnum);
    throw Exception(ss.str());
  }
  return pathbuf;
}
#elif defined(__linux)
std::string
abspath_to_argv0(void) {
  char buf[PATH_MAX];

  // Call readlink on the symlink to self.
  const ssize_t r = ::readlink("/proc/self/exe", buf, sizeof(buf));
  if (r < 0) {
    const int errnum = errno;
    std::ostringstream ss;
    ss << "Call to readlink failed: " << std::strerror(errnum);
    throw Exception(ss.str());
  }
  else if (static_cast<size_t>(r) > sizeof(buf)) {
    std::ostringstream ss;
    ss << "Buffer was not large enough for call to readlink (" << sizeof(buf) << " bytes)";
    throw Exception(ss.str());
  }
  buf[r] = '\0';

  return buf;
}
#else
  #error Do not know how to find the absolute path to argv[0] on your platform.
#endif


void
get_env_paths(std::vector<std::string> &paths, const char *const env_var) {
  const char *const orig_env_path = ::getenv(env_var);
  if (orig_env_path == nullptr)
    return;

  const size_t orig_env_path_len = std::strlen(orig_env_path);
  std::unique_ptr<char[]> env_path(new char[orig_env_path_len + 1]);
  std::memcpy(env_path.get(), orig_env_path, orig_env_path_len + 1);
  for (char *ep = env_path.get(), *path = nullptr; (path = ::strsep(&ep, ":")) != nullptr; ) {
    if (*path != '\0')
      paths.push_back(path);
  }
}


std::string
path_dirname(const std::string &path) {
  std::unique_ptr<char[]> buf(new char[path.size() + 1]);
  std::strcpy(buf.get(), path.c_str());
  return ::dirname(buf.get());
}


std::string
path_join(const std::string &a, const std::string &b) {
  std::string path = a;
  if (!b.empty() && b.front() == '/')
    path = b;
  else if (path.empty() || path.back() == '/')
    path += b;
  else
    path += "/" + b;
  return path;
}


bool
make_stdout_pager(std::ostream &out) {
  // If the provided ostream is not stdout, bail.
  if (out.rdbuf() != std::cout.rdbuf())
    return false;

  // If stdout is not a tty, bail.
  if (!::isatty(STDOUT_FILENO))
    return false;

  // Ensure we actually have a pager.
  char *pager_unsafe = ::getenv("PAGER");
  if (pager_unsafe == NULL)
    return false;
  char *const pager = new char[std::strlen(pager_unsafe) + 1];
  std::strcpy(pager, pager_unsafe);

  // Setup pipes.
  int parent_to_child[2], child_to_parent[2];  // [read, write]
  if (::pipe(parent_to_child) == -1 || ::pipe(child_to_parent) == -1) {
    const int errnum = errno;
    LOG(ERROR) << "Call to pipe failed: " << std::strerror(errnum) << std::endl;
    return false;
  }

  // Fork.
  const pid_t pid = fork();
  if (pid < 0) {  // Call to fork() failed.
    const int errnum = errno;
    LOG(ERROR) << "Call to fork failed: " << std::strerror(errnum) << std::endl;
    return false;
  }
  else if (pid == 0) {  // Child process.
    // Correct the file descriptors.
    ::close(parent_to_child[1]);
    ::close(child_to_parent[0]);
    ::close(STDOUT_FILENO);
    ::dup2(child_to_parent[1], STDOUT_FILENO);
  }
  else {  // Parent process.
    // Correct the file descriptors.
    ::close(parent_to_child[0]);
    ::close(child_to_parent[1]);
    ::close(STDIN_FILENO);
    ::dup2(child_to_parent[0], STDIN_FILENO);

    // Attempt to exec the pager.
    char *pager_args[3] = {pager, new char[3], nullptr};
    std::memcpy(pager_args[1], "-R", 3);
    execvp(pager_args[0], pager_args);

    perror("execvp");
    std::exit(1);
  }

  return true;
}

}  // namespace port
}  // namespace schwa
