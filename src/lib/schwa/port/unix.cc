/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/port.h>

#include <cerrno>
#include <cstring>
#include <sstream>

#include <limits.h>
#include <unistd.h>

#include <schwa/exception.h>


namespace schwa {
namespace port {

static constexpr const char *const PATH_TO_SELF = "/proc/self/exe";


std::string
abspath_to_argv0(void) {
  char buf[PATH_MAX];

  // Call readlink on the symlink to self.
  const ssize_t r = ::readlink(PATH_TO_SELF, buf, sizeof(buf));
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

}  // namespace port
}  // namespace schwa
