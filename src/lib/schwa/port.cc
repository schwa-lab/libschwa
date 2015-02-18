/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/port.h>

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#ifdef HAVE_CXXABI_H
  #include <cxxabi.h>  // __cxa_demangle
#endif
#include <string.h>
#include <unistd.h>  // close, dup2, execvp, getenv, pipe

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>

#include <schwa/io/logging.h>
#include <schwa/io/streams.h>


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


bool
make_stdout_pager(io::OutputStream &out) {
  /// If the provided ostream is not stdout and not a tty, bail.
  if (!(out.is_stdout() && out.is_tty()))
    return false;

  // Ensure we actually have a pager.
  const char *const orig_pager = ::getenv("PAGER");
  if (orig_pager == nullptr)
    return false;
  const size_t orig_pager_len = std::strlen(orig_pager);
  std::unique_ptr<char[]> pager(new char[orig_pager_len + 1]);
  std::memcpy(pager.get(), orig_pager, orig_pager_len + 1);

  // Setup pipes.
  int parent_to_child[2], child_to_parent[2];  // [read, write]
  if (::pipe(parent_to_child) == -1 || ::pipe(child_to_parent) == -1) {
    const int errnum = errno;
    LOG(ERROR) << "Call to pipe failed: " << std::strerror(errnum) << std::endl;
    return false;
  }

  // Fork.
  const pid_t pid = ::fork();
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
    char *pager_args[3] = {pager.get(), ::strdup("-R"), nullptr};
    ::execvp(pager_args[0], pager_args);

    std::perror("execvp");
    std::exit(1);
  }

  return true;
}

}  // namespace port
}  // namespace schwa
