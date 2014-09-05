/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/paths.h>

#include <cerrno>
#include <memory>
#include <sstream>

#ifdef HAVE_LIBGEN_H
  #include <libgen.h>  // dirname
#endif
#ifdef HAVE_LIMITS_H
  #include <limits.h>  // PATH_MAX
#endif
#ifdef HAVE_LIBPROC_H
  #include <libproc.h>  // PROC_PIDPATHINFO_MAXSIZE, proc_pidpath
#endif
#include <stdlib.h>  // getenv
#include <string.h>  // memcpy, strerror, strlen, strsep
#include <unistd.h>  // getpid, readlink

#include <schwa/exception.h>


namespace schwa {
namespace io {

#if defined(__APPLE__)
std::string
abspath_to_argv0(void) {
  char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
  const pid_t pid = ::getpid();
  if (::proc_pidpath(pid, pathbuf, sizeof(pathbuf)) <= 0) {
    const int errnum = errno;
    std::ostringstream ss;
    ss << "Call to proc_pidpath failed: " << ::strerror(errnum);
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
    ss << "Call to readlink failed: " << ::strerror(errnum);
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

  const size_t orig_env_path_len = ::strlen(orig_env_path);
  std::unique_ptr<char[]> env_path(new char[orig_env_path_len + 1]);
  ::memcpy(env_path.get(), orig_env_path, orig_env_path_len + 1);
  for (char *ep = env_path.get(), *path = nullptr; (path = ::strsep(&ep, ":")) != nullptr; ) {
    if (*path != '\0')
      paths.push_back(path);
  }
}


std::string
path_dirname(const std::string &path) {
  std::unique_ptr<char[]> buf(new char[path.size() + 1]);
  ::memcpy(buf.get(), path.c_str(), path.size() + 1);
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


}  // namespace io
}  // namespace schwa
