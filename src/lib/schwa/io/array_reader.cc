/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/array_reader.h>

#include <algorithm>
#include <cstring>


namespace schwa {
namespace io {

int
ArrayReader::get(void) {
  if (_left == 0)
    return EOF;
  const char c = *_upto++;
  --_left;
  return *reinterpret_cast<const unsigned char *>(&c);
}


void
ArrayReader::read(char *const buf, const size_t amount) {
  const size_t n = std::min(_left, amount);
  std::memcpy(buf, _upto, n);
  _upto += n;
  _left -= n;
}


size_t
ArrayReader::ignore(const size_t amount) {
  const size_t n = std::min(_left, amount);
  _upto += n;
  _left -= n;
  return n;
}

}  // namespace io
}  // namespace schwa
