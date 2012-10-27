/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/istream_source.h>

#include <iostream>


namespace schwa {
namespace io {

IStreamSource::IStreamSource(std::istream &istream) : _istream(istream) { }


size_t
IStreamSource::read(char *buffer, size_t nbytes) {
  return _istream.read(buffer, nbytes).gcount();
}

}  // namespace io
}  // namespace schwa
