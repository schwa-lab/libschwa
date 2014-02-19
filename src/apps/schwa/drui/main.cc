/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/io/array_reader.h>
#include <schwa/io/logging.h>
#include <schwa/macros.h>
#include <schwa/msgpack.h>
#include <schwa/pool.h>
#include <schwa/port.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;
namespace mp = schwa::msgpack;
namespace port = schwa::port;


namespace {

class Config : public cf::Main {
public:
  cf::IStreamOp input;
  cf::Op<int> limit;

  Config(void) :
      cf::Main("drui", "Schwa-Lab Docrep UI"),
      input(*this, "input", "input filename"),
      limit(*this, "limit", "upper bound on the number of documents to process from the input stream", -1)
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
  static constexpr const char *const SEP = "\t\033[1;30m # ";
  static constexpr const char *const REPR_NIL = "<nil>";
  static constexpr const char *const REPR_UNKNOWN = "<UNKNOWN VALUE>";

  const FauxDoc &_doc;
  std::ostream &_out;
  const unsigned int _doc_num;
  unsigned int _indent;

  void process_fields(const std::vector<dr::RTFieldDef *> &fields);
  void process_store(const dr::RTStoreDef &store);

  std::ostream &write_indent(void);
  std::ostream &write_field(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const mp::Value &value);

  void write_pointer(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const mp::Value &value);
  void write_primitive(const mp::Value &value);
  void write_slice(const dr::RTFieldDef &field, const mp::Value &value);

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

  friend class DocProcessorTest;
};


void
DocProcessor::process_doc(void) {
  const dr::RTManager &rt = *_doc.rt();

  write_indent() << port::BOLD << _doc_num << ":" << port::OFF << " {" << SEP << "Document" << port::OFF << "\n";
  ++_indent;

  const dr::RTSchema *schema = rt.doc;
  assert(schema != nullptr);
  // TODO document fields? where are they?
  //process_fields(schema->fields);
  for (const auto *store : schema->stores)
    process_store(*store);

  --_indent;
  write_indent() << "},\n";
}

void
DocProcessor::process_fields(const std::vector<dr::RTFieldDef *> &fields) {
  for (const dr::RTFieldDef *field : fields) {
    (void)field;
  }
}

void
DocProcessor::process_store(const dr::RTStoreDef &store) {
  assert(store.is_lazy());
  const dr::RTSchema &klass = *store.klass;

  // iterate through each field name to find the largest name so we can align
  // all of the values when printing out.
  unsigned int max_length = 0;
  for (const auto& field : klass.fields)
    if (field->serial.size() > max_length)
      max_length = field->serial.size();

  // decode the lazy store values into dynamic msgpack objects
  schwa::Pool pool(4096);
  io::ArrayReader reader(store.lazy_data, store.lazy_nbytes);
  mp::Value *value = mp::read_dynamic(reader, pool);

  // <instances> ::= [ <instance> ]
  assert(is_array(value->type));
  const mp::Array &array = *value->via._array;

  // write header
  write_indent() << port::BOLD << store.serial << ":" << port::OFF << " {";
  _out << SEP << klass.serial;
  _out << " (" << array.size() << ")"<< port::OFF << "\n";
  ++_indent;

  for (uint32_t i = 0; i != array.size(); ++i) {
    assert(is_map(array[i].type));
    const mp::Map &map = *array[i].via._map;

    write_indent() << port::BOLD << "0x" << std::hex << i << std::dec << ":" << port::OFF << " {\n";
    ++_indent;


    // <instance> ::= { <field_id> : <obj_val> }
    for (uint32_t j = 0; j != map.size(); ++j) {
      assert(is_uint(map.get(j).key.type));
      const mp::Map::Pair &pair = map.get(j);
      const dr::RTFieldDef &field = *klass.fields[pair.key.via._uint64];

      write_indent() << port::BOLD << std::setw(max_length) << std::left << field.serial << ": " << port::OFF;
      write_field(store, field, pair.value) << "\n";
    }

    --_indent;
    write_indent() << "},\n";
  }

  --_indent;
  write_indent() << "},\n";
}

std::ostream &
DocProcessor::write_indent(void) {
  for (unsigned int i = 0; i != _indent; ++i)
    _out << "  ";
  return _out;
}

std::ostream &
DocProcessor::write_field(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const mp::Value &value) {
  const auto flags = _out.flags();

  if (field.is_slice)
    write_slice(field, value);
  else if (field.points_into != nullptr || field.is_self_pointer)
    write_pointer(store, field, value);
  else
    write_primitive(value);

  _out.flags(flags);
  return _out;
}

void
DocProcessor::write_slice(const dr::RTFieldDef &field, const mp::Value &value) {
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
  _out << " (" << std::dec << a << ", " << std::dec << (a + b) << ")" << port::OFF;
}

void
DocProcessor::write_pointer(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const mp::Value &value) {
  if (field.is_collection) {
    assert(is_array(value.type));
    const mp::Array &array = *value.via._array;
    _out << "[";
    for (uint32_t i = 0; i != array.size(); ++i) {
      const mp::Value &v = array[i];
      if (i != 0)
        _out << ", ";
      if (is_nil(v.type) || is_uint(v.type)) {
        if (is_nil(v.type))
          _out << REPR_NIL;
        else
          _out << "0x" << std::hex << v.via._uint64;
      }
      else
        _out << port::RED << v.type << port::OFF;
    }
    _out << "]";
  }
  else {
    if (is_nil(value.type) || is_uint(value.type)) {
      if (is_nil(value.type))
        _out << REPR_NIL;
      else
        _out << "0x" << std::hex << value.via._uint64;
    }
    else
      _out << port::RED << value.type << port::OFF;
  }

  _out << SEP;
  if (field.is_self_pointer)
    _out << "self-pointer" << (field.is_collection ? "s" : "") << " into " << store.serial;
  else
    _out << "pointer" << (field.is_collection ? "s" : "") << " into " << field.points_into->serial;
  _out << port::OFF;
}

void
DocProcessor::write_primitive(const mp::Value &value) {
  if (is_bool(value.type)) {
    _out << std::boolalpha << value.via._bool << ",";
  }
  else if (is_double(value.type)) {
    _out << value.via._double << ",";
    _out << SEP << "double" << port::OFF;
  }
  else if (is_float(value.type)) {
    _out << value.via._float << ",";
    _out << SEP << "float" << port::OFF;
  }
  else if (is_nil(value.type)) {
    _out << REPR_NIL;
  }
  else if (is_sint(value.type)) {
    _out << "0x" << std::hex << value.via._int64 << std::dec << ",";
    _out << SEP << "int (" << value.via._int64 << ")" << port::OFF;
  }
  else if (is_uint(value.type)) {
    _out << "0x" << std::hex << value.via._uint64 << std::dec << ",";
    _out << SEP << "uint (" << value.via._uint64 << ")" << port::OFF;
  }
  else if (is_array(value.type)) {
    _out << port::RED << "TODO array" << port::OFF;
  }
  else if (is_map(value.type)) {
    _out << port::RED << "TODO map" << port::OFF;
  }
  else if (is_raw(value.type)) {
    const mp::Raw &raw = *value.via._raw;
    (_out << "\"").write(raw.value(), raw.size()) << "\",";
    _out << SEP << "raw (" << std::dec << raw.size() << "B)" << port::OFF;
  }
  else
    _out << port::RED << REPR_UNKNOWN << port::OFF;
}

}  // namespace


int
main(int argc, char *argv[]) {
  // process args
  Config c;
  c.main<io::PrettyLogger>(argc, argv);

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
