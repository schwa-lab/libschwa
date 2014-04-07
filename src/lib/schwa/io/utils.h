/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_UTILS_H_
#define SCHWA_IO_UTILS_H_

#include <ios>
#include <iosfwd>
#include <string>

#include <schwa/_base.h>


namespace schwa {
  namespace io {

    /**
     * Opens a std::ifstream to the provided filename. If the file fails to open for reading, am
     * IO::Exception is thrown. Otherwise, a pointer to the ifstream is returned. It is the
     * responsibility of the caller to delete the returned pointer.
     **/
    std::ifstream *safe_open_ifstream(const char *filename, const std::ios_base::openmode mode=std::ios_base::in);

    inline std::ifstream *
    safe_open_ifstream(const std::string &filename, const std::ios_base::openmode mode=std::ios_base::in) {
      return safe_open_ifstream(filename.c_str(), mode);
    }

    /**
     * Opens a std::ofstream to the provided filename. If the file fails to open for writing, am
     * IO::Exception is thrown. Otherwise, a pointer to the ofstream is returned. It is the
     * responsibility of the caller to delete the returned pointer.
     **/
    std::ofstream *safe_open_ofstream(const char *filename, const std::ios_base::openmode mode=std::ios_base::out);

    inline std::ofstream *
    safe_open_ofstream(const std::string &filename, const std::ios_base::openmode mode=std::ios_base::out) {
      return safe_open_ofstream(filename.c_str(), mode);
    }

  }
}

#endif  // SCHWA_IO_UTILS_H_
