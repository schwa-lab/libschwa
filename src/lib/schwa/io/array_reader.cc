/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/array_reader.h>

#include <algorithm>
#include <cstring>


namespace schwa {
namespace io {

ArrayReader::ArrayReader(const char *data, size_t nbytes) : _data(data), _nbytes(nbytes), _upto(_data), _left(nbytes), _gcount(0) { }


int
ArrayReader::get(void) {
  if (_left == 0)
    return EOF;
  const char c = *_upto++;
  --_left;
  return *reinterpret_cast<const unsigned char *>(&c);
}


int
ArrayReader::peek(void) {
  if (_left == 0)
    return EOF;
  return *reinterpret_cast<const unsigned char *>(_upto);
}


void
ArrayReader::read(char *const buf, const size_t amount) {
  _gcount = std::min(_left, amount);
  std::memcpy(buf, _upto, _gcount);
  _upto += _gcount;
  _left -= _gcount;
}


size_t
ArrayReader::ignore(const size_t amount) {
  _gcount = std::min(_left, amount);
  _upto += _gcount;
  _left -= _gcount;
  return _gcount;
}

}  // namespace io
}  // namespace schwa
