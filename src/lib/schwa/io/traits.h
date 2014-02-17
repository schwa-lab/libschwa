/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_TRAITS_H_
#define SCHWA_IO_TRAITS_H_

#include <schwa/_base.h>
#include <schwa/io/write_buffer.h>

namespace schwa {
  namespace io {

    template <typename T>
    struct Traits {
      static inline void
      write_zerocopy(T &out, const char *data, const size_t nbytes) {
        out.write(data, nbytes);
      }
    };

    template <>
    struct Traits<WriteBuffer> {
      static inline void
      write_zerocopy(WriteBuffer &out, const char *data, const size_t nbytes) {
        out.write_zerocopy(data, nbytes);
      }
    };

  }
}

#endif  // SCHWA_IO_TRAITS_H_
