/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_READER_H_
#define SCHWA_DR_READER_H_

#include <iosfwd>

#include <schwa/_base.h>

namespace schwa {
  namespace dr {

    class BaseDocSchema;
    class Doc;


    class Reader {
    public:
      static constexpr uint64_t WIRE_VERSION = 2;

    protected:
      std::istream &_in;
      BaseDocSchema &_dschema;
      bool _has_more;

    public:
      Reader(std::istream &in, BaseDocSchema &dschema);
      ~Reader(void) { }

      Reader &read(Doc &doc);

      inline operator bool(void) const { return _has_more; }
      inline Reader &operator >>(Doc &doc) { return read(doc); }

    private:
      DISALLOW_COPY_AND_ASSIGN(Reader);
    };

  }
}

#endif  // SCHWA_DR_READER_H_
