/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/mmaped_source.h>

#include <assert.h>    // assert
#include <errno.h>     // errno
#include <fcntl.h>     // open
#include <string.h>    // strerror
#include <sys/mman.h>  // mmap, munmap
#include <sys/stat.h>  // fstat
#include <unistd.h>    // close

#include <algorithm>
#include <string>

#include <schwa/exception.h>


namespace schwa {
namespace io {

class MMappedSource::Impl {
private:
  const int _fd;
  char *const _data;
  const size_t _size;
  size_t _upto;
  const std::string _filename;

  void open_file(void);

public:
  Impl(const char *filename);
  ~Impl(void);

  const char *data(void) const { return _data; }
  size_t size(void) const { return _size; }

  size_t read(char *buffer, const size_t nbytes);

private:
  DISALLOW_COPY_AND_ASSIGN(Impl);
};


MMappedSource::Impl::Impl(const char *filename) :
    _fd(0),
    _data(nullptr),
    _size(0),
    _upto(0),
    _filename(filename) {
  open_file();
}

MMappedSource::Impl::~Impl(void) {
  // unmap the region
  if (_data != nullptr) {
    const int ret = ::munmap(_data, _size);
    if (ret == -1)
      throw IOException(::strerror(errno), _filename);
  }

  // close the file descriptor
  if (_fd > 0) {
    const int ret = ::close(_fd);
    if (ret == -1)
      throw IOException(::strerror(errno), _filename);
  }
}


void
MMappedSource::Impl::open_file(void) {
  // open the file in read-only mode
  const_cast<int &>(_fd) = ::open(_filename.c_str(), O_RDONLY);
  if (_fd == -1)
    throw IOException(::strerror(errno), _filename);

  // discover the size of the file
  struct stat stat;
  int ret = ::fstat(_fd, &stat);
  if (ret == -1)
    throw IOException(::strerror(errno), _filename);
  const_cast<size_t &>(_size) = stat.st_size;

  // mmap the whole contents of the file
  void *const ptr = ::mmap(nullptr, _size, PROT_READ, MAP_FILE, _fd, 0);
  if (ptr == MAP_FAILED)
    throw IOException(::strerror(errno), _filename);

  // update internal information
  const_cast<char *&>(_data) = static_cast<char *>(ptr);
}


size_t
MMappedSource::Impl::read(char *buffer, size_t nbytes) {
  const size_t upto = std::max(_upto + nbytes, _size);
  const size_t delta = upto - _upto;
  ::memcpy(buffer, _data + _upto, delta);
  _upto = upto;
  return delta;
}


// ============================================================================
// MMappedSource
// ============================================================================
MMappedSource::MMappedSource(const char *filename) :
    _impl(new Impl(filename))
  {
  assert(_impl != nullptr);
}

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
