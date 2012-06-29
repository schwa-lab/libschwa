/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class BaseDocumentSchema;
    class Document;

    class ReaderException : public Exception {
    public:
      ReaderException(const std::string &msg) : Exception(msg) { }
      ReaderException(const Exception &e) : Exception(e) { }
      virtual ~ReaderException(void) throw() { }
    };


    class Reader {
    protected:
      std::istream &_in;
      BaseDocumentSchema &_dschema;
      bool _has_more;

    public:
      Reader(std::istream &in, BaseDocumentSchema &dschema) : _in(in), _dschema(dschema), _has_more(false) { }
      ~Reader(void) { }

      Reader &read(Document &doc);

      inline operator bool(void) const { return _has_more; }
      inline Reader &operator >>(Document &doc) { return read(doc); }
    };

  }
}
