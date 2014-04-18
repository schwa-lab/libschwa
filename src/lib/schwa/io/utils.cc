/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/utils.h>

#include <fstream>
#include <ios>
#include <iostream>

#include <schwa/exception.h>


namespace schwa {
namespace io {

std::ifstream *
safe_open_ifstream(const char *const filename, const std::ios_base::openmode mode) {
  std::ifstream *const f = new std::ifstream(filename, mode);
  if (!f->is_open()) {
    delete f;
    throw IOException("Could not open file for reading", filename);
  }
  return f;
}


std::ofstream *
safe_open_ofstream(const char *const filename, const std::ios_base::openmode mode) {
  std::ofstream *const f = new std::ofstream(filename, mode);
  if (!f->is_open()) {
    delete f;
    throw IOException("Could not open file for writing", filename);
  }
  return f;
}

}  // namespace io
}  // namespace schwa
