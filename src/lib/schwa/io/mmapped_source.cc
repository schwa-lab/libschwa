/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/mmapped_source.h>

#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <string>

#include <fcntl.h>     // open
#include <sys/mman.h>  // mmap, munmap
#include <sys/stat.h>  // fstat
#include <unistd.h>    // close

#include <schwa/exception.h>


namespace schwa {
namespace io {

class MMappedSource::Impl {
private:
  int _fd;
  char *_data;
  size_t _size;
  size_t _upto;
  const std::string _filename;

public:
  Impl(const char *filename);
  ~Impl(void);

  const char *data(void) const { return _data; }
  size_t size(void) const { return _size; }

  size_t read(char *buffer, const size_t nbytes);

private:
  SCHWA_DISALLOW_COPY_AND_ASSIGN(Impl);
};


MMappedSource::Impl::Impl(const char *const filename) : _data(nullptr), _filename(filename) {
  // Open the file in read-only mode.
  _fd = ::open(_filename.c_str(), O_RDONLY);
  if (_fd == -1)
    throw IOException(errno, _filename);

  // Discover the size of the file.
  struct stat stat;
  if (::fstat(_fd, &stat) == -1)
    throw IOException(errno, _filename);
  _size = stat.st_size;
  _upto = 0;

  // mmap the whole contents of the file.
  _data = static_cast<char *>(::mmap(nullptr, _size, PROT_READ, MAP_FILE | MAP_SHARED, _fd, 0));
  if (_data == MAP_FAILED)
    throw IOException(errno, _filename);
}

MMappedSource::Impl::~Impl(void) {
  // unmap the region.
  if (_data != nullptr && ::munmap(_data, _size) == -1)
    throw IOException(errno, _filename);

  // Close the file descriptor.
  if (_fd > 0 && ::close(_fd) == -1)
    throw IOException(errno, _filename);
}


size_t
MMappedSource::Impl::read(char *buffer, size_t nbytes) {
  const size_t upto = std::max(_upto + nbytes, _size);
  const size_t delta = upto - _upto;
  std::memcpy(buffer, _data + _upto, delta);
  _upto = upto;
  return delta;
}


// ============================================================================
// MMappedSource
// ============================================================================
MMappedSource::MMappedSource(const char *filename) : _impl(new Impl(filename)) { }

MMappedSource::~MMappedSource(void) {
  delete _impl;
}

const char *
MMappedSource::data(void) const {
  return _impl->data();
}

size_t
MMappedSource::size(void) const {
  return _impl->size();
}

size_t
MMappedSource::read(char *buffer, size_t nbytes) {
  return _impl->read(buffer, nbytes);
}

}  // namespace io
}  // namespace schwa
