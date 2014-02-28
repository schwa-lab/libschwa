/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_FILE_SOURCE_H_
#define SCHWA_IO_FILE_SOURCE_H_

#include <cstdio>

#include <schwa/_base.h>
#include <schwa/io/source.h>


namespace schwa {
  namespace io {

    class FileSource : public Source {
    protected:
      FILE *_file;

    public:
      FileSource(FILE *file);
      virtual ~FileSource(void) { }

      virtual size_t read(char *buffer, size_t nbytes) override;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(FileSource);
    };

  }
}

#endif  // SCHWA_IO_FILE_SOURCE_H_
