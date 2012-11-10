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


class DocProcessor {
private:
  static const char *NORM;
  static const char *SEP;

  const FauxDoc &_doc;
  std::ostream &_out;
  const unsigned int _doc_num;
  unsigned int _indent;

  void process_fields(const std::vector<dr::RTFieldDef *> &fields);
  void process_stores(const std::vector<dr::RTStoreDef *> &stores);
  void process_store(const dr::RTStoreDef &store);

  std::ostream &write_indent(void);
  std::ostream &write_field(const dr::RTFieldDef &field, const mp::Value &value);

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

const char *DocProcessor::NORM = "\033[0m";
const char *DocProcessor::SEP = "\t\033[1;30m # ";

void
DocProcessor::process_doc(void) {
  const dr::RTManager &rt = *_doc.rt();

  write_indent() << _doc_num << ": {" << SEP << "Document" << NORM << "\n";
  ++_indent;

  const dr::RTSchema *schema = rt.doc;
  assert(schema != nullptr);
  write_indent() << "is_lazy: " << schema->is_lazy() << "\n";
  // TODO document fields? where are they?
  //process_fields(schema->fields);
  process_stores(schema->stores);
  --_indent;
  write_indent() << "}\n";
}

void
DocProcessor::process_fields(const std::vector<dr::RTFieldDef *> &fields) {
  for (const dr::RTFieldDef *field : fields) {
    (void)field;
  }
}

void
DocProcessor::process_stores(const std::vector<dr::RTStoreDef *> &stores) {
  for (const dr::RTStoreDef *store : stores) {
    write_indent() << store->serial << ": {";
    _out << SEP << store->klass->serial;
    _out << " (" << store->lazy_nbytes << "B)"<< NORM << "\n";
    ++_indent;
    process_store(*store);
    --_indent;
    write_indent() << "},\n";
  }
}

void
DocProcessor::process_store(const dr::RTStoreDef &store) {
  assert(store.is_lazy());
  const dr::RTSchema &klass = *store.klass;

  schwa::Pool pool(4096);
  io::ArrayReader reader(store.lazy_data, store.lazy_nbytes);
  mp::Value *value = mp::read_dynamic(reader, pool);

  // <instances> ::= [ <instance> ]
  assert(is_array(value->type));
  const mp::Array &array = *value->via._array;
  for (uint32_t i = 0; i != array.size(); ++i) {
    assert(is_map(array[i].type));
    const mp::Map &map = *array[i].via._map;

    write_indent() << i << ": {\n";
    ++_indent;

    // <instance> ::= { <field_id> : <obj_val> }
    for (uint32_t j = 0; j != map.size(); ++j) {
      assert(is_uint(map.get(j).key.type));
      const mp::Map::Pair &pair = map.get(j);
      const dr::RTFieldDef &field = *klass.fields[pair.key.via._uint64];

      write_indent() << field.serial << ": ";
      write_field(field, pair.value) << "\n";
    }

    --_indent;
    write_indent() << "},\n";
  }
}

std::ostream &
DocProcessor::write_indent(void) {
  for (unsigned int i = 0; i != _indent; ++i)
    _out << "  ";
  return _out;
}

std::ostream &
DocProcessor::write_field(const dr::RTFieldDef &field, const mp::Value &value) {
  // keep track of the current flags so we can reset them at the end
  const auto flags = _out.flags();

  if (field.is_slice) {
    assert(is_array(value.type));
    const auto &array = *value.via._array;
    assert(array.size() == 2);
    assert(is_uint(array[0].type));
    assert(is_uint(array[1].type));
    const uint64_t a = array[0].via._uint64;
    const uint64_t b = array[1].via._uint64;
    _out << "[0x" << std::hex << a << ", 0x" << std::hex << (a + b) << "],";
    _out << SEP;
    if (field.points_into == nullptr)
      _out << "byte slice";
    else
      _out << "slice into " << field.points_into->serial;
    _out << " (" << std::dec << a << ", " << std::dec << (a + b) << ")" << NORM;
  }
  else {
    if (is_bool(value.type)) {
      _out << std::boolalpha << value.via._bool << ",";
    }
    else if (is_double(value.type)) {
      _out << value.via._double << ",";
      _out << SEP << "double" << NORM;
    }
    else if (is_float(value.type)) {
      _out << value.via._float << ",";
      _out << SEP << "float" << NORM;
    }
    else if (is_int(value.type)) {
      _out << "0x" << std::hex << value.via._int64 << std::dec << ",";
      _out << SEP << "int (" << value.via._int64 << ")" << NORM;
    }
    else if (is_nil(value.type)) {
      _out << "<nil>";
    }
    else if (is_uint(value.type)) {
      _out << "0x" << std::hex << value.via._uint64 << std::dec << ",";
      _out << SEP << "uint (" << value.via._uint64 << ")" << NORM;
    }
    else if (is_array(value.type)) {
    }
    else if (is_map(value.type)) {

    }
    else if (is_raw(value.type)) {
      const mp::Raw &raw = *value.via._raw;
      (_out << "'").write(raw.value(), raw.size()) << "',";
      _out << SEP << "raw (" << std::dec << raw.size() << "B)" << NORM;
    }
    else {
      _out << "<UNKNOWN TYPE>";
    }
  }

  _out.flags(flags);
  return _out;
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
