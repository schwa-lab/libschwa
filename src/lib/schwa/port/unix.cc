/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/port.h>

#include <cerrno>
#include <cstring>
#include <memory>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <schwa/exception.h>


namespace schwa {
namespace port {

static constexpr const char *const PATH_TO_SELF = "/proc/self/exe";


static void
throw_os_exception(const char *call, const int errnum) {
  std::ostringstream ss;
  ss << "Call to " << call << " failed: " << std::strerror(errnum);
  throw Exception(ss.str());
}


std::string
abspath_to_argv0(void) {
  struct stat s;

  // Attempt to stat the path to self.
  if (::lstat(PATH_TO_SELF, &s) == -1)
    throw_os_exception("lstat", errno);

  // Allocate a buffer for readlink and then call it.
  std::unique_ptr<char[]> buf(new char[s.st_size + 1]);
  const ssize_t r = ::readlink(PATH_TO_SELF, buf.get(), s.st_size + 1);
  if (r < 0)
    throw_os_exception("readlink", errno);
  else if (r > s.st_size) {
    std::ostringstream ss;
    ss << "Symlink " << PATH_TO_SELF << " increased in size between calls to lstat and readlink";
    throw Exception(ss.str());
  }
  buf[s.st_size] = '\0';

  return buf.get();
}

}  // namespace port
}  // namespace schwa
