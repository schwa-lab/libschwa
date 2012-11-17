/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/exception.h>

#include <iostream>
#include <typeinfo>

#include <schwa/port.h>


namespace schwa {

// ============================================================================
// IOException
// ============================================================================
IOException::IOException(const std::string &msg, int line) : Exception(msg), line(line) { }

IOException::IOException(const std::string &msg, const std::string &uri, int line) : Exception(msg), uri(uri), line(line) { }

IOException::IOException(const IOException &other) : Exception(other), uri(other.uri), line(other.line) { }


// ============================================================================
// print_exception
// ============================================================================
print_exception::print_exception(const Exception &e) : print_exception(port::demangle_typeid(typeid(e).name()), e) { }

print_exception::print_exception(const std::string &name, const Exception &e) : name(name), e(e) { }


std::ostream &
print_exception::dump(std::ostream &out) const {
  return out << port::BOLD << '[' << name << "] " << port::RED << e.what() << port::OFF;
}


std::ostream &
operator <<(std::ostream &out, const print_exception &e) {
  return e.dump(out);
}

}  // namespace schwa
