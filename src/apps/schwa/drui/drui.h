/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRIU_DRUI_H_
#define SCHWA_DRIU_DRUI_H_

#include <iosfwd>
#include <vector>

#include <schwa/_base.h>
#include <schwa/dr.h>
#include <schwa/msgpack.h>


namespace schwa {
  namespace drui {

    class FauxDoc : public dr::Doc {
    public:
      class Schema;
    };


    class FauxDoc::Schema : public dr::Doc::Schema<FauxDoc> {
    public:
      Schema(void);
    };


    class FauxDocProcessor {
    private:
      static const std::string SEP;
      static constexpr const char *const REPR_NIL = "<nil>";
      static constexpr const char *const REPR_UNKNOWN = "<UNKNOWN VALUE>";

      std::ostream &_out;
      const FauxDoc *_doc;
      unsigned int _doc_num;
      unsigned int _indent;

      void _process_fields(const std::vector<dr::RTFieldDef *> &fields);
      void _process_store(const dr::RTStoreDef &store);

      std::ostream &_write_indent(void);
      std::ostream &_write_field(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const msgpack::Value &value);

      void _write_pointer(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const msgpack::Value &value);
      void _write_primitive(const msgpack::Value &value);
      void _write_slice(const dr::RTFieldDef &field, const msgpack::Value &value);

    public:
      FauxDocProcessor(std::ostream &out);

      void process_doc(const FauxDoc &doc, unsigned int doc_num);
      inline void operator ()(const FauxDoc &doc, unsigned int doc_num) { process_doc(doc, doc_num); }

    private:
      DISALLOW_COPY_AND_ASSIGN(FauxDocProcessor);

      friend class FauxDocProcessorTest;
    };

  }
}

#endif  // SCHWA_DRIU_DRUI_H_
