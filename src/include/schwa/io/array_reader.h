/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_ARRAY_READER_H_
#define SCHWA_IO_ARRAY_READER_H_

#include <schwa/_base.h>

namespace schwa {
  namespace io {

    class ArrayReader {
    private:
      const char *const _data;
      const size_t _nbytes;
      const char *_upto;
      size_t _left;

    public:
      ArrayReader(const char *data, size_t nbytes) : _data(data), _nbytes(nbytes), _upto(_data), _left(nbytes) { }
      ~ArrayReader(void) { }

      inline const char *data(void) const { return _data; }
      inline const char *upto(void) const { return _upto; }
      inline size_t nbytes(void) const { return _nbytes; }

      inline int get(void);
      inline void read(char *const buf, const size_t amount);

    private:
      DISALLOW_COPY_AND_ASSIGN(ArrayReader);
    };

  }
}

#endif  // SCHWA_IO_ARRAY_READER_H_
