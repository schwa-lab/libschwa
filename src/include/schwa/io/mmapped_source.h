/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_MMAPED_SOURCE_H_
#define SCHWA_IO_MMAPED_SOURCE_H_

#include <schwa/_base.h>
#include <schwa/io/source.h>


namespace schwa {
  namespace io {

    class MMappedSource : public Source {
    private:
      class Impl;

      Impl *_impl;

    public:
      MMappedSource(const char *filename);
      virtual ~MMappedSource(void);

      const char *data(void) const;
      size_t size(void) const;

      size_t read(char *buffer, size_t nbytes) override;

    private:
      DISALLOW_COPY_AND_ASSIGN(MMappedSource);
    };

  }
}

#endif  // SCHWA_IO_MMAPED_SOURCE_H_
