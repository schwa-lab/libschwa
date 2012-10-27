/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_UNSAFE_ARRAY_WRITER_H_
#define SCHWA_IO_UNSAFE_ARRAY_WRITER_H_

#include <cstdlib>

#include <schwa/_base.h>

namespace schwa {
  namespace io {

    class UnsafeArrayWriter {
    private:
      char *const _data;
      char *_upto;

    public:
      explicit UnsafeArrayWriter(char *data) : _data(data), _upto(data) { }
      ~UnsafeArrayWriter(void) { }

      inline char *data(void) const { return _data; }
      inline char *upto(void) const { return _upto; }

      inline void
      put(const char c) {
        *_upto++ = c;
      }

      inline void
      write(const char *const data, const size_t nbytes) {
        std::memcpy(_upto, data, nbytes);
        _upto += nbytes;
      }

    private:
      DISALLOW_COPY_AND_ASSIGN(UnsafeArrayWriter);
    };

  }
}

#endif  // SCHWA_IO_UNSAFE_ARRAY_WRITER_H_
