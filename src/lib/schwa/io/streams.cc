/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/streams.h>

#include <cstdio>  // STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO
#include <cstring>
#include <fstream>
#include <iostream>

#include <fcntl.h>  // open
#include <sys/types.h>  // lseek
#include <sys/stat.h>  // open
#include <unistd.h>  // close, isatty, lseek

#include <schwa/exception.h>
#include <schwa/io/enums.h>


namespace schwa {
namespace io {

// ============================================================================
// InputStream
// ============================================================================
InputStream::InputStream(const char *const path, const std::ios_base::openmode mode) : _path(path), _stream(nullptr) {
  // Convert dash to human-friendly path name.
  if (_path == "-")
    _path = STDIN_STRING;

  // Attempt to open the input stream location.
  if (_path == STDIN_STRING) {
    _stream = &std::cin;
    _nbytes = -1;
  }
  else {
    std::ifstream *const in = new std::ifstream(path, mode);
    if (!*in) {
      delete in;
      throw IOException("Could not open file for reading", _path);
    }
    _stream = in;

    // Count how many bytes are in the file.
    in->seekg(0, in->end);
    _nbytes = in->tellg();
    in->seekg(0, in->beg);
  }
}

InputStream::~InputStream(void) {
  if (!is_stdin())
    delete _stream;
}


bool
InputStream::is_seekable(void) const {
  // Open a read-only file descriptor to the underlying path.
  int fd = ::open(_path.c_str(), O_RDONLY);
  if (fd == -1) {
    return false;
  }

  // Non-seekable file return ESPIPE from `lseek`.
  off_t offset = ::lseek(fd, 0, SEEK_SET);
  bool not_seekable = offset == -1 && errno == ESPIPE;

  // Close the file descriptor.
  ::close(fd);
  return !not_seekable;
}


bool
InputStream::is_stdin(void) const {
  return _stream->rdbuf() == std::cin.rdbuf();
}


bool
InputStream::is_tty(void) const {
  if (is_stdin())
    return ::isatty(STDIN_FILENO);
  return false;
}


void
InputStream::seek_start(void) {
  _stream->seekg(0, _stream->beg);
}


void
InputStream::seek_end(void) {
  _stream->seekg(0, _stream->end);
}


// ============================================================================
// OutputStream
// ============================================================================
OutputStream::OutputStream(const char *const path, const std::ios_base::openmode mode) : _path(path), _stream(nullptr) {
  // Convert dash to human-friendly path name.
  if (_path == "-")
    _path = STDOUT_STRING;

  // Attempt to open the output stream location.
  if (_path == STDOUT_STRING)
    _stream = &std::cout;
  else if (_path == STDERR_STRING)
    _stream = &std::cerr;
  else {
    std::ofstream *const out = new std::ofstream(path, mode);
    if (!*out) {
      delete out;
      throw IOException("Could not open file for writing", _path);
    }
    _stream = out;
  }
}

OutputStream::~OutputStream(void) {
  if (!is_stdout() && !is_stderr())
    delete _stream;
}


bool
OutputStream::is_stdout(void) const {
  return _stream->rdbuf() == std::cout.rdbuf();
}


bool
OutputStream::is_stderr(void) const {
  return _stream->rdbuf() == std::cerr.rdbuf();
}


bool
OutputStream::is_tty(void) const {
  if (is_stdout())
    return ::isatty(STDOUT_FILENO);
  else if (is_stderr())
    return ::isatty(STDERR_FILENO);
  return false;
}

}  // namespace io
}  // namespace schwa
