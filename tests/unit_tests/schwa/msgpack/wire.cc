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



BOOST_AUTO_TEST_SUITE_END()
