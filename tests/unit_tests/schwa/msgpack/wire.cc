#include <schwa/base.h>
#include <schwa/msgpack.h>

#include <boost/preprocessor/list.hpp>
#include <boost/test/unit_test.hpp>

#define BYTES_TEST_CHECK(r, data, i, elem) \
  BOOST_CHECK_EQUAL(static_cast<uint8_t>(data[i]), elem);

#define BYTES_TEST(function_call, list) \
  { \
    std::stringstream ss; \
    function_call; \
    const std::string s = ss.str(); \
    BOOST_CHECK_EQUAL(s.size(), BOOST_PP_LIST_SIZE(list)); \
    BOOST_PP_LIST_FOR_EACH_I(BYTES_TEST_CHECK, s, list); \
  }

namespace mp = schwa::msgpack;

template <typename OUT>
static OUT &
print_bytes(OUT &out, const uint8_t *bytes, const size_t size) {
  out << "{";
  for (size_t i = 0; i != size; ++i) {
    if (i != 0)
      out << ",";
    out<< "0x" << std::hex << static_cast<unsigned int>(bytes[i]);
  }
  return out << "}";
}

static boost::test_tools::predicate_result
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


BOOST_AUTO_TEST_SUITE(schwa_msgpack_wire)

// ----------------------------------------------------------------------------
// write_raw_8
//  100 = 0110 0100
// -100 = 1001 1100
BOOST_AUTO_TEST_CASE(write_raw_8_unsigned) {
  #define BYTES (0x64, BOOST_PP_NIL)
  BYTES_TEST(mp::write_raw_8(ss, static_cast<uint8_t>(100)), BYTES);
  #undef BYTES
}
BOOST_AUTO_TEST_CASE(write_raw_8_signed) {
  #define BYTES (0x9c, BOOST_PP_NIL)
  BYTES_TEST(mp::write_raw_8(ss, static_cast<int8_t>(-100)), BYTES);
  #undef BYTES
}

// ----------------------------------------------------------------------------
// write_raw_16
//  4957 = 0001 0011 0101 1101
// -4957 = 1110 1100 1010 0011
BOOST_AUTO_TEST_CASE(write_raw_16_unsigned) {
  #define BYTES (0x13, (0x5D, BOOST_PP_NIL))
  BYTES_TEST(mp::write_raw_16(ss, static_cast<uint16_t>(4957)), BYTES);
  #undef BYTES
}
BOOST_AUTO_TEST_CASE(write_raw_16_signed) {
  #define BYTES (0xEC, (0xA3, BOOST_PP_NIL))
  BYTES_TEST(mp::write_raw_16(ss, static_cast<int16_t>(-4957)), BYTES);
  #undef BYTES
}


// ----------------------------------------------------------------------------
// write_raw_32
//  584667347 = 0010 0010 1101 1001 0101 0000 1101 0011
// -584667347 = 1101 1101 0010 0110 1010 1111 0010 1101
BOOST_AUTO_TEST_CASE(write_raw_32_unsigned) {
  #define BYTES (0x22, (0xD9, (0x50, (0xD3, BOOST_PP_NIL))))
  BYTES_TEST(mp::write_raw_32(ss, static_cast<uint32_t>(584667347)), BYTES);
  #undef BYTES
}
BOOST_AUTO_TEST_CASE(write_raw_32_signed) {
  #define BYTES (0xDD, (0x26, (0xAF, (0x2D, BOOST_PP_NIL))))
  BYTES_TEST(mp::write_raw_32(ss, static_cast<int32_t>(-584667347)), BYTES);
  #undef BYTES
}


// ----------------------------------------------------------------------------
// write_raw_64
//  8436114578613100000 = 0111 0101 0001 0011 0001 1001 1000 0011 0100 0110 1011 1010 0101 1101 1110 0000
// -8436114578613100000 = 1000 1010 1110 1100 1110 0110 0111 1100 1011 1001 0100 0101 1010 0010 0010 0000
BOOST_AUTO_TEST_CASE(write_raw_64_unsigned) {
  #define BYTES (0x75, (0x13, (0x19, (0x83, (0x46, (0xBA, (0x5D, (0xE0, BOOST_PP_NIL))))))))
  BYTES_TEST(mp::write_raw_64(ss, static_cast<uint64_t>(8436114578613100000)), BYTES);
  #undef BYTES
}
BOOST_AUTO_TEST_CASE(write_raw_64_signed) {
  #define BYTES (0x8A, (0xEC, (0xE6, (0x7C, (0xB9, (0x45, (0xA2, (0x20, BOOST_PP_NIL))))))))
  BYTES_TEST(mp::write_raw_64(ss, static_cast<int64_t>(-8436114578613100000)), BYTES);
  #undef BYTES
}



// ----------------------------------------------------------------------------
// write_nil
BOOST_AUTO_TEST_CASE(write_nil) {
  #define BYTES (mp::header::NIL, BOOST_PP_NIL)
  BYTES_TEST(mp::write_nil(ss), BYTES);
  #undef BYTES
}


// ----------------------------------------------------------------------------
// write_boolean
BOOST_AUTO_TEST_CASE(write_boolean) {
  #define BYTES (mp::header::TRUE, BOOST_PP_NIL)
  BYTES_TEST(mp::write_boolean(ss, true), BYTES);
  #undef BYTES

  #define BYTES (mp::header::FALSE, BOOST_PP_NIL)
  BYTES_TEST(mp::write_boolean(ss, false), BYTES);
  #undef BYTES
}


// ----------------------------------------------------------------------------
// write_uint
BOOST_AUTO_TEST_CASE(write_uint_fixed) {
  #define BYTES (0x64, BOOST_PP_NIL)
  BYTES_TEST(mp::write_uint(ss, 100), BYTES);
  #undef BYTES
}
BOOST_AUTO_TEST_CASE(write_uint_8) {
  #define BYTES (mp::header::UINT_8, (0x80, BOOST_PP_NIL))
  BYTES_TEST(mp::write_uint(ss, 128), BYTES);
  #undef BYTES

  #define BYTES (mp::header::UINT_8, (0xC8, BOOST_PP_NIL))
  BYTES_TEST(mp::write_uint(ss, 200), BYTES);
  #undef BYTES

  #define BYTES (mp::header::UINT_8, (0xFF, BOOST_PP_NIL))
  BYTES_TEST(mp::write_uint(ss, 255), BYTES);
  #undef BYTES
}
BOOST_AUTO_TEST_CASE(write_uint_16) {
  #define BYTES (mp::header::UINT_16, (0x01, (0x00, BOOST_PP_NIL)))
  BYTES_TEST(mp::write_uint(ss, 256), BYTES);
  #undef BYTES

  #define BYTES (mp::header::UINT_16, (0x13, (0x5D, BOOST_PP_NIL)))
  BYTES_TEST(mp::write_uint(ss, 4957), BYTES);
  #undef BYTES

  #define BYTES (mp::header::UINT_16, (0xFF, (0xFF, BOOST_PP_NIL)))
  BYTES_TEST(mp::write_uint(ss, 65535), BYTES);
  #undef BYTES
}
BOOST_AUTO_TEST_CASE(write_uint_32) {
  #define BYTES (mp::header::UINT_32, (0x00, (0x01, (0x00, (0x00, BOOST_PP_NIL)))))
  BYTES_TEST(mp::write_uint(ss, 65536), BYTES);
  #undef BYTES

  #define BYTES (mp::header::UINT_32, (0x22, (0xD9, (0x50, (0xD3, BOOST_PP_NIL)))))
  BYTES_TEST(mp::write_uint(ss, 584667347), BYTES);
  #undef BYTES

  #define BYTES (mp::header::UINT_32, (0xFF, (0xFF, (0xFF, (0xFF, BOOST_PP_NIL)))))
  BYTES_TEST(mp::write_uint(ss, 4294967295), BYTES);
  #undef BYTES
}
BOOST_AUTO_TEST_CASE(write_uint_64) {
  #define BYTES (mp::header::UINT_64, (0x00, (0x00, (0x00, (0x01, (0x00, (0x00, (0x00, (0x00, BOOST_PP_NIL)))))))))
  BYTES_TEST(mp::write_uint(ss, 4294967296), BYTES);
  #undef BYTES

  #define BYTES (mp::header::UINT_64, (0x75, (0x13, (0x19, (0x83, (0x46, (0xBA, (0x5D, (0xE0, BOOST_PP_NIL)))))))))
  BYTES_TEST(mp::write_uint(ss, static_cast<uint64_t>(8436114578613100000)), BYTES);
  #undef BYTES

  #define BYTES (mp::header::UINT_64, (0xFF, (0xFF, (0xFF, (0xFF, (0xFF, (0xFF, (0xFF, (0xFF, BOOST_PP_NIL)))))))))
  BYTES_TEST(mp::write_uint(ss, std::numeric_limits<uint64_t>::max()), BYTES);
  #undef BYTES
}


// ----------------------------------------------------------------------------
// write_array
BOOST_AUTO_TEST_CASE(write_array_fixed) {
  {
    mp::Object items[1] = { {mp::TYPE_BOOLEAN, {._bool=true}} };
    mp::ArrayObject array = {1, items};
    #define BYTES (0x91, (mp::header::TRUE, BOOST_PP_NIL))
    BYTES_TEST(mp::write_array(ss, array), BYTES);
    #undef BYTES
  }
  {
    uint8_t expected[1024];
    size_t e = 0;
    expected[e++] = 0x9F;
    mp::Object items[15];
    for (size_t i = 0; i != 15; ++i) {
      if (i % 6) {
        items[i].type = mp::TYPE_NIL;
        expected[e++] = mp::header::NIL;
      }
      else if (i % 4) {
        items[i].type = mp::TYPE_BOOLEAN;
        if (i % 8) {
          items[i].via._bool = true;
          expected[e++] = mp::header::TRUE;
        }
        else {
          items[i].via._bool = false;
          expected[e++] = mp::header::FALSE;
        }
      }
      else {
        items[i].type = mp::TYPE_UINT;
        items[i].via._uint = i;
        expected[e++] = static_cast<uint8_t>(i);
      }
    }
    mp::ArrayObject array = {15, items};
    std::stringstream ss;
    mp::write_array(ss, array);
    BOOST_CHECK( compare_bytes(ss.str(), expected, e) );
  }
}
BOOST_AUTO_TEST_CASE(write_array_16) {
  uint8_t expected[4 * 1024 * 1024];
  {
    size_t e = 0;
    expected[e++] = mp::header::ARRAY_16;
    expected[e++] = 0x00;
    expected[e++] = 0x10;
    mp::Object items[16];
    for (size_t i = 0; i != 16; ++i) {
      items[i].type = mp::TYPE_UINT;
      items[i].via._uint = i;
      expected[e++] = static_cast<uint8_t>(i);
    }
    mp::ArrayObject array = {16, items};
    std::stringstream ss;
    mp::write_array(ss, array);
    BOOST_CHECK( compare_bytes(ss.str(), expected, e) );
  }
  {
    size_t e = 0;
    expected[e++] = mp::header::ARRAY_16;
    expected[e++] = 0xFF;
    expected[e++] = 0xFF;
    mp::Object items[65535];
    for (size_t i = 0; i != 65535; ++i) {
      items[i].type = mp::TYPE_UINT;
      items[i].via._uint = i;
      if (i <= 127)
        expected[e++] = static_cast<uint8_t>(i);
      else if (i <= 255) {
        expected[e++] = mp::header::UINT_8;
        expected[e++] = static_cast<uint8_t>(i);
      }
      else {
        const uint8_t *x = reinterpret_cast<const uint8_t *>(&i);
        expected[e++] = mp::header::UINT_16;
        expected[e++] = x[1];
        expected[e++] = x[0];
      }
    }
    mp::ArrayObject array = {65535, items};
    std::stringstream ss;
    mp::write_array(ss, array);
    BOOST_CHECK( compare_bytes(ss.str(), expected, e) );
  }
}


// ----------------------------------------------------------------------------
// write_array
BOOST_AUTO_TEST_CASE(write_raw_fixed) {
  uint8_t expected[32];
  {
    size_t e = 0;
    char data[] = {0xC4};
    mp::RawObject raw = {1, data};
    expected[e++] = 0xA1;
    expected[e++] = 0xC4;
    std::stringstream ss;
    mp::write_raw(ss, raw);
    BOOST_CHECK( compare_bytes(ss.str(), expected, e) );
  }
  {
    size_t e = 0;
    char data[31] = {0x8A, 0xAD, 0x00, 0x6D, 0xD, 0x37, 0x77, 0xDA, 0xC2, 0x99, 0xB6, 0xD8, 0x34, 0xD2, 0x9A, 0xB, 0x10, 0x68, 0xF2, 0xDE, 0x48, 0x2C, 0xF1, 0x9C, 0xCA, 0xC3, 0x93, 0xF, 0x70, 0x44, 0xAD};
    mp::RawObject raw = {31, data};
    expected[e++] = 0xBF;
    std::memcpy(expected + 1, data, 31);
    e += 31;
    std::stringstream ss;
    mp::write_raw(ss, raw);
    BOOST_CHECK( compare_bytes(ss.str(), expected, e) );
  }
}

BOOST_AUTO_TEST_SUITE_END()
