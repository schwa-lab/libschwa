/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

#include <schwa/macros.h>
#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/io/array_reader.h>
#include <schwa/msgpack.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;
namespace mp = schwa::msgpack;


namespace {

class Config : public cf::OpMain {
public:
  cf::IStreamOp input;

  Config(void) :
      cf::OpMain("drui", "Schwa-Lab Docrep UI"),
      input(*this, "input", "input filename")
    { }
  virtual ~Config(void) { }
};

class FauxDoc : public dr::Doc {
public:
  class Schema;
};

class FauxDoc::Schema : public dr::Doc::Schema<FauxDoc> {
public:
  Schema(void) : dr::Doc::Schema<FauxDoc>("FauxDoc", "The document class.") { }
};


struct sp {
  const unsigned int count;
  const std::string value;

  explicit sp(unsigned int count, const char *value="  ") : count(count), value(value) { }
};

inline std::ostream &
operator <<(std::ostream &out, const sp &sp) {
  for (unsigned int i = 0; i != sp.count; ++i)
    out << sp.value;
  return out;
}


template <typename T>
struct write_value {
  const T &value;

  explicit write_value(const T &value) : value(value) { }
};

template <typename T>
inline std::ostream &
operator <<(std::ostream &out, const write_value<T> &wv) {
  return out << wv.value << ",";
}

template <>
inline std::ostream &
operator <<(std::ostream &out, const write_value<bool> &wv) {
  const auto flags = out.flags();
  out << std::boolalpha << wv.value;
  out.flags(flags);
  return out << ",";
}

template <typename T>
inline write_value<T> wv(const T &val) { return write_value<T>(val); }


class DocProcessor {
private:
  const FauxDoc &_doc;
  std::ostream &_out;
  const unsigned int _doc_num;
  unsigned int _indent;

  void process_doc(void);
  void process_fields(const std::vector<dr::RTFieldDef *> &fields);
  void process_stores(const std::vector<dr::RTStoreDef *> &stores);
  void process_store(const dr::RTStoreDef &store);

public:
  DocProcessor(const FauxDoc &doc, std::ostream &out, unsigned int doc_num) :
      _doc(doc),
      _out(out),
      _doc_num(doc_num),
      _indent(0)
    { }

  inline void operator ()(void) { process_doc(); }

private:
  DISALLOW_COPY_AND_ASSIGN(DocProcessor);
};


void
DocProcessor::process_doc(void) {
  const dr::RTManager &rt = *_doc.rt();

  _out << sp(_indent) << _doc_num << ": {\n";
  ++_indent;

  const dr::RTSchema *schema = rt.doc;
  assert(schema != nullptr);
  _out << sp(_indent) << "is_lazy: " << wv(schema->is_lazy()) << "\n";
  process_fields(schema->fields);
  process_stores(schema->stores);
  --_indent;
  _out << sp(_indent) << "}\n";
}

void
DocProcessor::process_fields(const std::vector<dr::RTFieldDef *> &fields) {
  _out << sp(_indent) << "fields: {\n";
  ++_indent;
  for (const dr::RTFieldDef *field : fields) {
    _out << sp(_indent) << field->serial << ": " << wv(field->is_lazy()) << "\n";
  }
  --_indent;
  _out << sp(_indent) << "},\n";
}

void
DocProcessor::process_stores(const std::vector<dr::RTStoreDef *> &stores) {
  _out << sp(_indent) << "stores: {\n";
  ++_indent;
  for (const dr::RTStoreDef *store : stores) {
    _out << sp(_indent) << store->serial << ": {\n";
    ++_indent;
    process_store(*store);
    --_indent;
    _out << sp(_indent) << "},\n";
  }
  --_indent;
  _out << sp(_indent) << "},\n";
}

void
DocProcessor::process_store(const dr::RTStoreDef &store) {
  assert(store.is_lazy());
  _out << sp(_indent) << "is_lazy: " << wv(store.is_lazy()) << "\n";

  io::ArrayReader reader(store.lazy_data, store.lazy_nbytes);

  // <instance> ::= { <field_id> : <obj_val> }
  const uint32_t size = mp::read_map_size(reader);
  for (uint32_t i = 0; i != size; ++i) {
    const uint32_t key = static_cast<uint32_t>(mp::read_uint(reader));
    (void)key;
  }
}

}  // namespace


int
main(int argc, char *argv[]) {
  // process args
  Config c;
  c.main(argc, argv);

  // construct a docrep reader over the provided input stream
  std::istream &in = c.input.file();
  FauxDoc::Schema schema;
  dr::Reader reader(in, schema);

  // read the documents off the input stream
  FauxDoc doc;
  for (unsigned int i = 0; reader >> doc; ++i)
    DocProcessor(doc, std::cout, i)();

  return 0;
}
