#include <schwa/base.h>

#include "test_utils.h"


boost::test_tools::predicate_result
compare_bytes(const std::string &str, const uint8_t *expected, const size_t size) {
  if (str.size() != size) {
    boost::test_tools::predicate_result res(false);
    res.message() << "Different sizes [" << str.size() << " != " << size << "]\n";
    if (size <= 80) {
      res.message() << "expected="; print_bytes(res.message(), expected, size) << "\n";
      res.message() << "     got="; print_bytes(res.message(), reinterpret_cast<const uint8_t *>(str.c_str()), str.size());
    }
    return res;
  }
  for (size_t i = 0; i != size; ++i) {
    if (static_cast<uint8_t>(str[i]) != expected[i]) {
      boost::test_tools::predicate_result res(false);
      res.message() << "Byte " << i << " differs [" << static_cast<unsigned int>(static_cast<uint8_t>(str[i])) << " != " << static_cast<unsigned int>(expected[i]) << "]\n";
      res.message() << "expected="; print_bytes(res.message(), expected, size) << "\n";
      res.message() << "     got="; print_bytes(res.message(), reinterpret_cast<const uint8_t *>(str.c_str()), str.size());
      return res;
    }
  }
  return true;
}

