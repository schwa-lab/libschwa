/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/file_source.h>


namespace schwa {
namespace io {

FileSource::FileSource(FILE *file) : _file(file) { }


size_t
FileSource::read(char *buffer, size_t nbytes) {
  return std::fread(buffer, sizeof(char), nbytes, _file);
}

}  // namespace io
}  // namespace schwa
