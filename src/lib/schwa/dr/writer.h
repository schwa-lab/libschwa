/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_WRITER_H_
#define SCHWA_DR_WRITER_H_

#include <iosfwd>

#include <schwa/_base.h>

namespace schwa {
  namespace dr {

    class BaseDocSchema;
    class Doc;


    class Writer {
    public:
      static constexpr uint64_t WIRE_VERSION = 2;

    protected:
      std::ostream &_out;
      BaseDocSchema &_dschema;

    public:
      Writer(std::ostream &out, BaseDocSchema &dschema);
      ~Writer(void) { }

      void write(const Doc &doc);

      inline Writer &
      operator <<(const Doc &doc) {
        write(doc);
        return *this;
      }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Writer);
    };

  }
}

#endif  // SCHWA_DR_WRITER_H_
