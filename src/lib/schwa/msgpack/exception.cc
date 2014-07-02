/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/msgpack/exception.h>

#include <iomanip>
#include <sstream>


namespace schwa {
namespace msgpack {

ReadError::ReadError(const std::string &msg) : IOException(msg), _read(-1), _expected(-1) { }

ReadError::ReadError(const std::string &msg, const int read, const int expected) : IOException(""), _read(read), _expected(expected) {
  std::ostringstream ss;
  ss << msg << ": got=0x" << std::hex << read << std::dec;
  if (expected != -1)
    ss << " expected=0x" << std::hex << expected << std::dec;
  _msg = ss.str();
}


ReadError::ReadError(const ReadError &o) : IOException(o), _read(o._read), _expected(o._expected) { }


WriteError::WriteError(const std::string &msg) : IOException(msg) { }

WriteError::WriteError(const WriteError &o) : IOException(o) { }

}  // namespace msgpack
}  // namespace schwa
