/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/port.h>

#include <cerrno>
#include <cstring>
#include <sstream>

#include <libproc.h>
#include <unistd.h>

#include <schwa/exception.h>


namespace schwa {
namespace port {

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

}  // namespace port
}  // namespace schwa
