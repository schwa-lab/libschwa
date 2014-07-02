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
      size_t _gcount;

    public:
      ArrayReader(const char *data, size_t nbytes);
      ~ArrayReader(void) { }

      inline const char *data(void) const { return _data; }
      inline const char *upto(void) const { return _upto; }
      inline size_t gcount(void) const { return _gcount; }
      inline size_t nbytes(void) const { return _nbytes; }

      int get(void);
      int peek(void);

      size_t ignore(const size_t amount);
      void read(char *const buf, const size_t amount);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(ArrayReader);
    };

  }
}

#endif  // SCHWA_IO_ARRAY_READER_H_
