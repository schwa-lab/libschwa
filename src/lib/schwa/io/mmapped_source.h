/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_MMAPED_SOURCE_H_
#define SCHWA_IO_MMAPED_SOURCE_H_

#include <schwa/_base.h>
#include <schwa/io/source.h>


namespace schwa {
  namespace io {

    /**
     * Concrete Source implementation which attempts to mmap a file, and then reads from the mapped
     * region. If the file failes to be opened or mapped, the constructor throws an IOException.
     **/
    class MMappedSource : public Source {
    private:
      class Impl;

      Impl *_impl;

    public:
      MMappedSource(const char *filename);
      virtual ~MMappedSource(void);

      size_t read(char *buffer, size_t nbytes) override;

      /** Returns a pointer to the underlying mmapped region. */
      const char *data(void) const;

      /** Returns the size (in bytes) of the mmapped region. */
      size_t size(void) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(MMappedSource);
    };

  }
}

#endif  // SCHWA_IO_MMAPED_SOURCE_H_
