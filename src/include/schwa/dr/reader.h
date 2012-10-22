/* -*- Mode: C++; indent-tabs-mode: nil -*- */

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
    };

  }
}
