/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "processor.h"

#include <cassert>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <schwa/msgpack.h>
#include <schwa/pool.h>
#include <schwa/port.h>
#include <schwa/unicode.h>


namespace io = schwa::io;
namespace mp = schwa::msgpack;


namespace schwa {
namespace mp_less {

const std::string Processor::SEP(std::string("\t") + port::DARK_GREY + "# ");
const std::string Processor::REPR_NIL("<nil>");
const std::string Processor::REPR_UNKNOWN("<UNKNOWN VALUE>");

Processor::Processor(void) : _indent(0), _out(nullptr) { }

Processor::~Processor(void) { }


std::ostream &
Processor::_write_indent(void) {
  for (unsigned int i = 0; i != _indent; ++i)
    *_out << "  ";
  return *_out;
}


void
Processor::_write_value(const mp::Value &value, const bool add_description) {
  if (is_bool(value.type)) {
    *_out << std::boolalpha << value.via._bool;
    if (add_description)
      *_out << SEP << "bool" << port::OFF;
  }
  else if (is_double(value.type)) {
    *_out << value.via._double;
    if (add_description)
      *_out << SEP << "double" << port::OFF;
  }
  else if (is_float(value.type)) {
    *_out << value.via._float;
    if (add_description)
      *_out << SEP << "float" << port::OFF;
  }
  else if (is_nil(value.type)) {
    *_out << REPR_NIL;
    if (add_description)
      *_out << SEP << "nil" << port::OFF;
  }
  else if (is_sint(value.type)) {
    *_out << "0x" << std::hex << value.via._int64 << std::dec;
    if (add_description)
      *_out << SEP << "int (" << value.via._int64 << ")" << port::OFF;
  }
  else if (is_uint(value.type)) {
    *_out << "0x" << std::hex << value.via._uint64 << std::dec;
    if (add_description)
      *_out << SEP << "uint (" << value.via._uint64 << ")" << port::OFF;
  }
  else if (is_str(value.type)) {
    const mp::Str &obj = *value.via._str;
    const UnicodeString s = UnicodeString::from_utf8(obj.data(), obj.nbytes());
    *_out << "\"" << s << "\"";
    if (add_description)
      *_out << SEP << "str (" << std::dec << obj.nbytes() << "B, " << s.size() << " code point(s))" << port::OFF;
  }
  else if (is_bin(value.type)) {
    const mp::Bin &obj = *value.via._bin;
    *_out << "\"";
    for (uint32_t i = 0; i != obj.nbytes(); ++i) {
      const char c = obj.data()[i];
      if (std::isprint(c))
        *_out << c;
      else
        *_out << "\\x" << std::hex << static_cast<unsigned int>(c) << std::dec;
    }
    *_out << "\"";
    if (add_description)
      *_out << SEP << "bin (" << std::dec << obj.nbytes() << "B)" << port::OFF;
  }
  else if (is_array(value.type)) {
    const mp::Array &obj = *value.via._array;
    _write_indent() << port::BOLD << "[" << port::OFF;
    if (obj.size() == 0)
      *_out << port::BOLD << "]" << port::OFF;
    if (add_description)
      *_out << SEP << "array (" << std::dec << obj.size() << ")" << port::OFF;
    if (obj.size() != 0) {
      *_out << "\n";
      ++_indent;
      for (uint32_t i = 0; i != obj.size(); ++i) {
        _write_indent();
        _write_value(obj[i]);
        *_out << ",\n";
      }
      --_indent;
      _write_indent() << port::BOLD << "]" << port::OFF;
    }
  }
  else if (is_map(value.type)) {
    const mp::Map &obj = *value.via._map;
    _write_indent() << port::BOLD << "{" << port::OFF;
    if (obj.size() == 0)
      *_out << port::BOLD << "}" << port::OFF;
    if (add_description)
      *_out << SEP << "map (" << std::dec << obj.size() << ")" << port::OFF;
    if (obj.size() != 0) {
      *_out << "\n";
      ++_indent;
      for (uint32_t i = 0; i != obj.size(); ++i) {
        const mp::Map::Pair &pair = obj[i];
        _write_indent();
        _write_value(pair.key, false);
        *_out << port::BOLD << ": " << port::OFF;
        _write_value(pair.value);
        *_out << ",\n";
      }
      --_indent;
      _write_indent() << port::BOLD << "}" << port::OFF;
    }
  }
  else if (is_ext(value.type)) {
    const mp::Ext &obj = *value.via._ext;
    *_out << "\"";
    for (uint32_t i = 0; i != obj.nbytes(); ++i) {
      const char c = obj.data()[i];
      if (std::isprint(c))
        *_out << std::dec << c;
      else
        *_out << "\\x" << std::hex << static_cast<unsigned int>(c);
    }
    *_out << "\"";
    if (add_description)
      *_out << SEP << "ext (" << std::dec << obj.nbytes() << "B)" << port::OFF;
  }
  else
    *_out << port::RED << REPR_UNKNOWN << port::OFF;
}

void
Processor::process(std::istream &in, std::ostream &out) {
  // Reset our state.
  _indent = 0;
  _out = &out;

  Pool pool(4096);
  while (in) {
    // Read in the next MessagePack object.
    const auto at_byte = in.tellg();
    mp::Value *value = nullptr;
    try {
      value = mp::read_dynamic(in, pool);
      assert(value != nullptr);
    }
    catch (mp::ReadError &e) {
      *_out << port::RED << "ReadError at byte " << std::dec << in.tellg() << ": " << e.what() << port::OFF << std::endl;
      break;
    }

    // Pretty-print the object.
    *_out << port::DARK_GREY << "# at byte " << at_byte << port::OFF << "\n";
    _write_value(*value);
    *_out << "\n";
  }
}

}  // namespace mp_less
}  // namespace schwa
