/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRLESS_PROCESSOR_H_
#define SCHWA_DRLESS_PROCESSOR_H_

#include <iosfwd>
#include <vector>

#include <schwa/_base.h>


namespace schwa {
  namespace dr {
    class Doc;
    class RTFieldDef;
    class RTSchema;
    class RTStoreDef;
  }

  namespace msgpack {
    class Value;
  }

  namespace dr_less {

    class Processor {
    public:
      static const std::string SEP;
      static const std::string REPR_NIL;
      static const std::string REPR_UNKNOWN;

    private:
      std::ostream &_out;
      const dr::Doc *_doc;
      unsigned int _indent;

      void _process_doc_fields(const dr::RTSchema &schema);
      void _process_store(const dr::RTStoreDef &store);

      std::ostream &_write_indent(void);
      std::ostream &_write_field(const dr::RTFieldDef &field, const msgpack::Value &value);
      std::ostream &_write_field(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const msgpack::Value &value);

      void _write_pointer(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const msgpack::Value &value);
      void _write_primitive(const msgpack::Value &value);
      void _write_slice(const dr::RTFieldDef &field, const msgpack::Value &value);

    public:
      Processor(std::ostream &out);
      ~Processor(void);

      void process_doc(const dr::Doc &doc, uint32_t doc_num);
      inline void operator ()(const dr::Doc &doc, uint32_t doc_num) { process_doc(doc, doc_num); }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Processor);
    };

  }
}

#endif  // SCHWA_DRLESS_PROCESSOR_H_
