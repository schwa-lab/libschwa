/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

#include <schwa/macros.h>
#include <schwa/config.h>
#include <schwa/pool.h>
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

template <>
inline std::ostream &
operator <<(std::ostream &out, const write_value<mp::Value> &v) {
  if (is_bool(v.value.type))
    return out << wv(v.value.via._bool);
  else if (is_double(v.value.type))
    return out << wv(v.value.via._double);
  else if (is_float(v.value.type))
    return out << wv(v.value.via._float);
  else if (is_int(v.value.type))
    out << v.value.via._int64;
  else if (is_nil(v.value.type))
    out << "<nil>";
  else if (is_uint(v.value.type))
    out << v.value.via._uint64;
  else if (is_array(v.value.type)) {

  }
  else if (is_map(v.value.type)) {

  }
  else if (is_raw(v.value.type)) {

  }
  else {
    out << "<UNKNOWN TYPE>";
  }
  return out << ",";
}



class DocProcessor {
private:
  const FauxDoc &_doc;
  std::ostream &_out;
  const unsigned int _doc_num;
  unsigned int _indent;

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

  void process_doc(void);
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
    _out << sp(_indent) << store->serial << ": {    " << store->klass->serial << "\n";
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
  const dr::RTSchema &klass = *store.klass;
  _out << sp(_indent) << "klass.serial: " << wv(klass.serial) << "\n";
  _out << sp(_indent) << "klass.fields.size(): " << wv(klass.fields.size()) << "\n";

  schwa::Pool pool(4096);
  io::ArrayReader reader(store.lazy_data, store.lazy_nbytes);
  mp::Value *value = mp::read_dynamic(reader, pool);

  // <instances> ::= [ <instance> ]
  assert(is_array(value->type));
  const mp::Array &array = *value->via._array;
  for (uint32_t i = 0; i != array.size(); ++i) {
    assert(is_map(array[i].type));
    const mp::Map &map = *array[i].via._map;

    _out << sp(_indent) << i << ": {    " << map.size() << "\n";
    ++_indent;

    // <instance> ::= { <field_id> : <obj_val> }
    for (uint32_t j = 0; j != map.size(); ++j) {
      assert(is_uint(map.get(j).key.type));
      const mp::Map::Pair &pair = map.get(j);
      const uint64_t key = pair.key.via._uint64;

      _out << sp(_indent) << klass.fields[key]->serial << ": " << wv(pair.value);
      _out << "    (todo)";
      _out << "\n";
    }

    --_indent;
    _out << sp(_indent) << "},\n";
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
