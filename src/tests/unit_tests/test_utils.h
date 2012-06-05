#include <boost/test/unit_test.hpp>


boost::test_tools::predicate_result
compare_bytes(const std::string &str, const uint8_t *expected, const size_t size);


inline boost::test_tools::predicate_result
compare_bytes(const std::string &str, const std::string &expected) {
  const uint8_t *const ptr = reinterpret_cast<const uint8_t *>(expected.c_str());
  return compare_bytes(str, ptr, expected.size());
}

