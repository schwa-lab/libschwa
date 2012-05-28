/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class Document;
    class TypeRegistry;


    class Writer {
    protected:
      std::ostream &_out;
      TypeRegistry &_reg;

      void write_klass_header(const Schema &schema, const std::map<TypeInfo, size_t> &types);

    public:
      Writer(std::ostream &out, TypeRegistry &reg) : _out(out), _reg(reg) { }
      ~Writer(void) { }

      void write(const Document &doc);

      inline Writer &
      operator <<(const Document &doc) {
        write(doc);
        return *this;
      }
    };

  }
}
