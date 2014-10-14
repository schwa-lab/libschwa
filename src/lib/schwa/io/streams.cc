/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/streams.h>

#include <cstdio>  // STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO
#include <cstring>
#include <fstream>
#include <iostream>

#include <unistd.h>  // isatty

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
InputStream::is_stdin(void) const {
  return _stream->rdbuf() == std::cin.rdbuf();
}


bool
InputStream::is_tty(void) const {
  if (is_stdin())
    return ::isatty(STDIN_FILENO);
  return false;
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
