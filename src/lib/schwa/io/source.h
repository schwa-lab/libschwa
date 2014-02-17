/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_SOURCE_H_
#define SCHWA_IO_SOURCE_H_

#include <schwa/_base.h>

namespace schwa {
  namespace io {

    class Source {
    protected:
      Source(void) { }

    public:
      virtual ~Source(void) { }

      virtual size_t read(char *buffer, size_t nbytes) = 0;

    private:
      DISALLOW_COPY_AND_ASSIGN(Source);
    };

  }
}

#endif  // SCHWA_IO_SOURCE_H_
