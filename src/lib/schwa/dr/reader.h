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
      static constexpr uint64_t WIRE_VERSION = 3;
      static constexpr uint64_t LEGACY_WIRE_VERSION = 2;

    protected:
      std::istream &_in;
      const BaseDocSchema &_dschema;
      bool _has_more;
      uint64_t _version;

    public:
      Reader(std::istream &in, const BaseDocSchema &dschema);
      ~Reader(void) { }

      Reader &read(Doc &doc);

      inline operator bool(void) const { return _has_more; }
      inline Reader &operator >>(Doc &doc) { return read(doc); }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Reader);
    };


    /**
     * Lazily read a document from \p in without forming any objects, writing the read in data back
     * to \p out. Returns whether or not a document was successfully read and copied.
     **/
    bool read_lazy_doc(std::istream &in, std::ostream &out);

  }
}

#endif  // SCHWA_DR_READER_H_
