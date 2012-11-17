/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_ISTREAM_SOURCE_H_
#define SCHWA_IO_ISTREAM_SOURCE_H_

#include <iosfwd>

#include <schwa/_base.h>
#include <schwa/io/source.h>

namespace schwa {
  namespace io {

    class IStreamSource : public Source {
    protected:
      std::istream &_istream;

    public:
      IStreamSource(std::istream &istream);
      virtual ~IStreamSource(void) { }

      virtual size_t read(char *buffer, size_t nbytes) override;

    private:
      DISALLOW_COPY_AND_ASSIGN(IStreamSource);
    };

  }
}

#endif  // SCHWA_IO_ISTREAM_SOURCE_H_
