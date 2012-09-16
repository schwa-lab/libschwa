/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class BaseDocSchema;
    class Doc;

    class ReaderException : public Exception {
    public:
      ReaderException(const std::string &msg) : Exception(msg) { }
      ReaderException(const Exception &e) : Exception(e) { }
      virtual ~ReaderException(void) throw() { }
    };


    class Reader {
    public:
      static constexpr uint64_t WIRE_VERSION = 2;

    protected:
      std::istream &_in;
      BaseDocSchema &_dschema;
      bool _has_more;

    public:
      Reader(std::istream &in, BaseDocSchema &dschema) : _in(in), _dschema(dschema), _has_more(false) { }
      ~Reader(void) { }

      Reader &read(Doc &doc);

      inline operator bool(void) const { return _has_more; }
      inline Reader &operator >>(Doc &doc) { return read(doc); }
    };

  }
}
