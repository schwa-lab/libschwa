/* -*- Mode: C++; indent-tabs-mode: nil -*- */

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
      Writer(std::ostream &out, BaseDocSchema &dschema) : _out(out), _dschema(dschema) { }
      ~Writer(void) { }

      void write(const Doc &doc);

      inline Writer &
      operator <<(const Doc &doc) {
        write(doc);
        return *this;
      }
    };

  }
}
