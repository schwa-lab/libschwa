/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/msgpack/exception.h>

#include <iomanip>
#include <sstream>


namespace schwa {
namespace msgpack {

ReadException::ReadException(const std::string &msg, const int read, const int expected) :
    IOException(msg),
    read(read),
    expected(expected) {
  std::stringstream ss;
  ss << msg << ": got=0x" << std::hex << read << std::dec;
  if (expected != -1)
    ss << " expected=0x" << std::hex << expected << std::dec;
  const_cast<std::string &>(local_msg) = ss.str();
}


ReadException::ReadException(const ReadException &o) :
    IOException(o.msg),
    local_msg(o.local_msg),
    read(o.read),
    expected(o.expected)
  { }


const char *
ReadException::what(void) const throw() {
  return local_msg.c_str();
}

}  // namespace msgpack
}  // namespace schwa
