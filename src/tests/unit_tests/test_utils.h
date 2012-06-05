#include <boost/test/unit_test.hpp>


template <typename OUT>
static OUT &
print_bytes(OUT &out, const uint8_t *bytes, const size_t size) {
  out << "{";
  for (size_t i = 0; i != size; ++i) {
    if (i != 0)
      out << ",";
    out << "0x" << std::hex << static_cast<unsigned int>(bytes[i]);
    out << " (" << bytes[i] << ")";
  }
  return out << "}";
}


boost::test_tools::predicate_result
compare_bytes(const std::string &str, const uint8_t *expected, const size_t size);


inline boost::test_tools::predicate_result
compare_bytes(const std::string &str, const std::string &expected) {
  const uint8_t *const ptr = reinterpret_cast<const uint8_t *>(expected.c_str());
  return compare_bytes(str, ptr, expected.size());
}

