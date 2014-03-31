/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/utils.h>

#include <fstream>

#include <schwa/exception.h>


namespace schwa {
namespace io {

std::ifstream *
safe_open_ifstream(const char *const filename) {
  std::ifstream *const f = new std::ifstream(filename);
  if (!*f) {
    delete f;
    throw IOException("Could not open file for reading", filename);
  }
  return f;
}


std::ofstream *
safe_open_ofstream(const char *const filename) {
  std::ofstream *const f = new std::ofstream(filename);
  if (!*f) {
    delete f;
    throw IOException("Could not open file for writing", filename);
  }
  return f;
}

}  // namespace io
}  // namespace schwa
