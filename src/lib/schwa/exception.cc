/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/exception.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <typeinfo>

#include <schwa/port.h>


namespace schwa {

// ============================================================================
// IOException
// ============================================================================
IOException::IOException(const std::string &msg, const int linenum) : IOException(msg, "", linenum) { }

IOException::IOException(int errno_, const std::string &uri, int linenum) : IOException(std::strerror(errno_), uri, linenum) { }

IOException::IOException(const std::string &msg, const std::string &uri, const int linenum) : Exception(""), _uri(uri), _linenum(linenum) {
  std::ostringstream ss;
  ss << msg;
  if (!uri.empty()) {
    ss << " (file=" << _uri;
    if (linenum != -1)
      ss << ":" << linenum;
    ss << ")";
  }
  _msg = ss.str();
}

IOException::IOException(const IOException &o) : Exception(o), _uri(o._uri), _linenum(o._linenum) { }


// ============================================================================
// SystemExit
// ============================================================================
SystemExit::SystemExit(const int exit_code) : _exit_code(exit_code) {
  std::ostringstream ss;
  ss << "System exit (status=" << _exit_code << ")";
  _msg = ss.str();
}


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
