/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "test_utils.h"

#include <iomanip>
#include <iostream>

#include <schwa/port.h>


namespace schwatest {

namespace {

class print_bytes {
private:
  const uint8_t *const _expected;
  const size_t _expected_size;
  const uint8_t *const _actual;
  const size_t _actual_size;

public:
  print_bytes(const uint8_t *expected, size_t expected_size, const uint8_t *actual, size_t actual_size) :
      _expected(expected),
      _expected_size(expected_size),
      _actual(actual),
      _actual_size(actual_size)
    { }

  std::ostream &
  operator ()(std::ostream &out) const {
    size_t a = 0, e = 0;

    // table header
    out << "   ] ";
    //      12345678901234567890123456789
    out << "           expected          " << " | ";
    out << "            actual           " << " || ";
    out << "           expected          " << " | ";
    out << "            actual            |" << std::endl;

    // table rows
    for (unsigned int row = 0; e != _expected_size || a != _actual_size; ++row) {
      out << std::dec << std::setw(3) << std::setfill(' ') << row << "]";

      std::stringstream e_row, a_row, e_str, a_str;
      for (unsigned int i = 0; i != 10; ++i) {
        e_row << ' ';
        a_row << ' ';
        e_str << ' ';
        a_str << ' ';

        bool differ;
        if (e == _expected_size && a == _actual_size)
          differ = false;
        else if (e == _expected_size)
          differ = true;
        else if (a == _actual_size)
          differ = true;
        else
          differ = _expected[e] != _actual[a];

        if (differ) {
          e_row << schwa::port::RED;
          a_row << schwa::port::RED;
          e_str << schwa::port::RED;
          a_str << schwa::port::RED;
        }

        if (e == _expected_size) {
          e_row << "  ";
          e_str << "  ";
        }
        else {
          e_row << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(_expected[e]);
          const char c = static_cast<char>(_expected[e]);
          if (std::isgraph(c))
            e_str << ' ' << c;
          else
            e_str << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(_expected[e]);
          ++e;
        }

        if (a == _actual_size) {
          a_row << "  ";
          a_str << "  ";
        }
        else {
          a_row << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(_actual[a]);
          const char c = static_cast<char>(_actual[a]);
          if (std::isgraph(c))
            a_str << ' ' << c;
          else
            a_str << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(_actual[a]);
          ++a;
        }

        if (differ) {
          e_row << schwa::port::OFF;
          a_row << schwa::port::OFF;
          e_str << schwa::port::OFF;
          a_str << schwa::port::OFF;
        }
      }

      out << e_row.str() << " |" << a_row.str() << " ||" << e_str.str() << " |" << a_str.str() << " |" << std::endl;
    }

    return out;
  }
};

inline std::ostream &
operator <<(std::ostream &out, const print_bytes &obj) {
  return obj(out);
}

}  // namespace


boost::test_tools::predicate_result
compare_bytes(const std::string &actual, const uint8_t *expected, const size_t expected_size) {
  if (actual.size() != expected_size) {
    boost::test_tools::predicate_result res(false);
    res.message() << "Different sizes [" << expected_size << " != " << actual.size() << "]\n";
    res.message() << print_bytes(expected, expected_size, reinterpret_cast<const uint8_t *>(actual.c_str()), actual.size());
    return res;
  }
  for (size_t i = 0; i != expected_size; ++i) {
    if (static_cast<uint8_t>(actual[i]) != expected[i]) {
      boost::test_tools::predicate_result res(false);
      res.message() << "Byte " << (i + 1) << " differs [" << static_cast<unsigned int>(expected[i]) << " != " << static_cast<unsigned int>(static_cast<uint8_t>(actual[i])) << "]\n";
      res.message() << print_bytes(expected, expected_size, reinterpret_cast<const uint8_t *>(actual.c_str()), actual.size());
      return res;
    }
  }
  return true;
}


boost::test_tools::predicate_result
compare_bytes(const std::string &str, const std::string &expected) {
  const uint8_t *const ptr = reinterpret_cast<const uint8_t *>(expected.c_str());
  return compare_bytes(str, ptr, expected.size());
}

}  // namespace schwatest
