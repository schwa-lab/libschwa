/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/std.h>
#include <schwa/port.h>

#if defined(__GNUC__) \
    && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))) \
    && !defined(__EDG_VERSION__) \
    && (defined(__GLIBCXX__) || defined(__GLIBCPP__))
  #define SCHWA_PORT_HAS_CXXABI
  #include <cxxabi.h>
#endif

namespace schwa { namespace port {

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
    std::cout << "before='" << typeid_name << "' after='" << out << "'" << std::endl;
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

} }
