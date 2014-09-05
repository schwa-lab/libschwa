/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "processor.h"

#include <cassert>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <schwa/dr.h>
#include <schwa/io/array_reader.h>
#include <schwa/msgpack.h>
#include <schwa/pool.h>
#include <schwa/port.h>
#include <schwa/unicode.h>


namespace dr = schwa::dr;
namespace io = schwa::io;
namespace mp = schwa::msgpack;


namespace schwa {
namespace dr_less {

// ============================================================================
// Processor::Impl
// ============================================================================
class Processor::Impl {
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
  std::ostream &_write_field(const dr::RTFieldDef &field, const mp::Value &value);
  std::ostream &_write_field(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const mp::Value &value);

  void _write_pointer(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const mp::Value &value);
  void _write_primitive(const mp::Value &value);
  void _write_slice(const dr::RTFieldDef &field, const mp::Value &value);

public:
  Impl(std::ostream &out) : _out(out), _doc(nullptr), _indent(0) { }

  void process_doc(const dr::Doc &doc, uint32_t doc_num);
};


const std::string Processor::Impl::SEP(std::string("\t") + port::DARK_GREY + "# ");
const std::string Processor::Impl::REPR_NIL("<nil>");
const std::string Processor::Impl::REPR_UNKNOWN("<UNKNOWN VALUE>");


void
Processor::Impl::process_doc(const dr::Doc &doc, const uint32_t doc_num) {
  // Reset our state.
  _doc = &doc;
  _indent = 0;

  const dr::RTManager &rt = *_doc->rt();
  assert(rt.doc != nullptr);
  const dr::RTSchema &rtdschema = *(rt.doc);

  _write_indent() << port::BOLD << doc_num << ":" << port::OFF << " {" << SEP << "Document" << port::OFF << "\n";
  ++_indent;

  // Output any found fields on the doc.
  if (rtdschema.has_lazy_data())
    _process_doc_fields(rtdschema);

  // Output each store.
  for (const auto *store : rtdschema.stores)
    _process_store(*store);

  --_indent;
  _write_indent() << "},\n";
}


void
Processor::Impl::_process_doc_fields(const dr::RTSchema &rtdschema) {
  // Iterate through each field name to find the largest name so we can align all of the values
  // when printing out.
  unsigned int max_length = 0;
  for (const auto& field : rtdschema.fields)
    if (field->serial.size() > max_length)
      max_length = field->serial.size();

  // Decode the lazy document values into dynamic msgpack objects.
  Pool pool(4096);
  io::ArrayReader reader(rtdschema.lazy_data, rtdschema.lazy_nbytes);
  mp::Value *value = mp::read_dynamic(reader, pool);
  assert(value != nullptr);

  assert(is_map(value->type));
  const mp::Map &map = *value->via._map;

  _write_indent() << port::BOLD << rtdschema.serial << ":" << port::OFF << " {\n";
  ++_indent;

  // <instance> ::= { <field_id> : <obj_val> }
  for (uint32_t j = 0; j != map.size(); ++j) {
    assert(is_uint(map.get(j).key.type));
    const mp::Map::Pair &pair = map.get(j);
    const dr::RTFieldDef &field = *rtdschema.fields[pair.key.via._uint64];

    _write_indent() << port::BOLD << std::setw(max_length) << std::left << field.serial << ": " << port::OFF;
    _write_field(field, pair.value) << "\n";
  }

  --_indent;
  _write_indent() << "},\n";
}


void
Processor::Impl::_process_store(const dr::RTStoreDef &store) {
  assert(store.is_lazy());
  const dr::RTSchema &klass = *store.klass;

  // Iterate through each field name to find the largest name so we can align
  // all of the values when printing out.
  unsigned int max_length = 0;
  for (const auto& field : klass.fields)
    if (field->serial.size() > max_length)
      max_length = field->serial.size();

  // Decode the lazy store values into dynamic msgpack objects.
  Pool pool(4096);
  io::ArrayReader reader(store.lazy_data, store.lazy_nbytes);
  mp::Value *value = mp::read_dynamic(reader, pool);

  // <instances> ::= [ <instance> ]
  assert(is_array(value->type));
  const mp::Array &array = *value->via._array;

  // Write header.
  _write_indent() << port::BOLD << store.serial << ":" << port::OFF << " {";
  _out << SEP << klass.serial;
  _out << " (" << array.size() << ")"<< port::OFF << "\n";
  ++_indent;

  for (uint32_t i = 0; i != array.size(); ++i) {
    assert(is_map(array[i].type));
    const mp::Map &map = *array[i].via._map;

    _write_indent() << port::BOLD << "0x" << std::hex << i << std::dec << ":" << port::OFF << " {\n";
    ++_indent;


    // <instance> ::= { <field_id> : <obj_val> }
    for (uint32_t j = 0; j != map.size(); ++j) {
      assert(is_uint(map.get(j).key.type));
      const mp::Map::Pair &pair = map.get(j);
      const dr::RTFieldDef &field = *klass.fields[pair.key.via._uint64];

      _write_indent() << port::BOLD << std::setw(max_length) << std::left << field.serial << ": " << port::OFF;
      _write_field(store, field, pair.value) << "\n";
    }

    --_indent;
    _write_indent() << "},\n";
  }

  --_indent;
  _write_indent() << "},\n";
}


std::ostream &
Processor::Impl::_write_indent(void) {
  for (unsigned int i = 0; i != _indent; ++i)
    _out << "  ";
  return _out;
}


std::ostream &
Processor::Impl::_write_field(const dr::RTFieldDef &field, const mp::Value &value) {
  const auto flags = _out.flags();

  if (field.is_slice)
    _write_slice(field, value);
  else
    _write_primitive(value);

  _out.flags(flags);
  return _out;
}


std::ostream &
Processor::Impl::_write_field(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const mp::Value &value) {
  const auto flags = _out.flags();

  if (field.is_slice)
    _write_slice(field, value);
  else if (field.points_into != nullptr || field.is_self_pointer)
    _write_pointer(store, field, value);
  else
    _write_primitive(value);

  _out.flags(flags);
  return _out;
}


void
Processor::Impl::_write_slice(const dr::RTFieldDef &field, const mp::Value &value) {
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
Processor::Impl::_write_pointer(const dr::RTStoreDef &store, const dr::RTFieldDef &field, const mp::Value &value) {
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
Processor::Impl::_write_primitive(const mp::Value &value) {
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
  else if (is_str(value.type)) {
    const mp::Str &obj = *value.via._str;
    const UnicodeString s = UnicodeString::from_utf8(obj.data(), obj.nbytes());
    _out << "\"" << s << "\",";
    _out << SEP << "str (" << std::dec << obj.nbytes() << "B, " << s.size() << " code point(s))" << port::OFF;
  }
  else if (is_bin(value.type)) {
    const mp::Bin &obj = *value.via._bin;
    _out << "\"";
    for (uint32_t i = 0; i != obj.nbytes(); ++i) {
      const char c = obj.data()[i];
      if (std::isprint(c))
        _out << std::dec << c;
      else
        _out << "\\x" << std::hex << static_cast<unsigned int>(c);
    }
    _out << "\",";
    _out << SEP << "bin (" << std::dec << obj.nbytes() << "B)" << port::OFF;
  }
  else if (is_array(value.type)) {
    _out << port::RED << "TODO array" << port::OFF;
  }
  else if (is_map(value.type)) {
    _out << port::RED << "TODO map" << port::OFF;
  }
  else if (is_ext(value.type)) {
    const mp::Ext &obj = *value.via._ext;
    _out << "\"";
    for (uint32_t i = 0; i != obj.nbytes(); ++i) {
      const char c = obj.data()[i];
      if (std::isprint(c))
        _out << std::dec << c;
      else
        _out << "\\x" << std::hex << static_cast<unsigned int>(c);
    }
    _out << "\",";
    _out << SEP << "ext (" << std::dec << obj.nbytes() << "B)" << port::OFF;
  }
  else
    _out << port::RED << REPR_UNKNOWN << port::OFF;
}


// ============================================================================
// Processor
// ============================================================================
Processor::Processor(std::ostream &out) : _impl(new Processor::Impl(out)) { }

Processor::~Processor(void) {
  delete _impl;
}

void
Processor::process_doc(const dr::Doc &doc, const uint32_t doc_num) {
  _impl->process_doc(doc, doc_num);
}

}  // namespace drui
}  // namespace schwa
