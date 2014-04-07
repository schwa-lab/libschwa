/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_SOURCE_H_
#define SCHWA_IO_SOURCE_H_

#include <schwa/_base.h>

namespace schwa {
  namespace io {

    /**
     * A generic read(2) wrapper abstract base class that allows a source-agnostic way to read a
     * certain number of bytes from an input source. IOException might be thrown from both the
     * constructor and Source::read if access the underlying source has any issues.
     **/
    class Source {
    protected:
      Source(void) { }

    public:
      virtual ~Source(void) { }

      /**
       * Attempts to read \p nbytes from the underlying source (file descriptor, mmapped region,
       * std::istream, socket, etc) into \p buffer. The number of bytes writted into \p buffer is
       * returned.
       **/
      virtual size_t read(char *buffer, size_t nbytes) = 0;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Source);
    };

  }
}

#endif  // SCHWA_IO_SOURCE_H_
