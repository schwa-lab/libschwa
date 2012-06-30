/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/port.h>


namespace schwa {


print_exception::print_exception(const Exception &e) : name(port::demangle_typeid(typeid(e).name())), e(e) { }
print_exception::print_exception(const std::string &name, const Exception &e) : name(name), e(e) { }

std::ostream &
print_exception::dump(std::ostream &out) const {
  return out << port::BOLD << '[' << name << "] " << port::RED << e.what() << port::OFF;
}


}
