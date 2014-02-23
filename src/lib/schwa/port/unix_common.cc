/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/port.h>

#if defined(__GNUC__) \
    && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))) \
    && !defined(__EDG_VERSION__) \
    && (defined(__GLIBCXX__) || defined(__GLIBCPP__))
  #define SCHWA_PORT_HAS_CXXABI
  #include <cxxabi.h>
#endif

#include <cstring>
#include <memory>

#include <libgen.h>


namespace schwa {
namespace port {

#ifdef SCHWA_PORT_HAS_CXXABI
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

}  // namespace port
}  // namespace schwa
