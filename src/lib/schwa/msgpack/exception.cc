/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/msgpack.h>


namespace schwa { namespace msgpack {

ReadException::ReadException(const std::string &msg, const int read, const int expected) : IOException(msg), read(read), expected(expected) {
  std::stringstream ss;
  ss << msg << ": got=0x" << std::hex << read << std::dec;
  if (expected != -1)
    ss << " expected=0x" << std::hex << expected << std::dec;
  const_cast<std::string &>(local_msg) = ss.str();
}


ReadException::ReadException(const ReadException &o) : IOException(o.msg), local_msg(o.local_msg), read(o.read), expected(o.expected) { }


} }
