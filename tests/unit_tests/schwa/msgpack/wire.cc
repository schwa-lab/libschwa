#include <schwa/base.h>
#include <schwa/msgpack.h>

#include <boost/test/unit_test.hpp>

#define BYTE(c) static_cast<unsigned char>(c)

namespace mp = schwa::msgpack;

BOOST_AUTO_TEST_SUITE(schwa_msgpack_wire)

// ----------------------------------------------------------------------------
// write_raw_8
//  100 = 0110 0100
// -100 = 1001 1100
static const uint8_t UINT8_VAL = 100;
static const uint8_t UINT8_BYTES[1] = {0x64};
static const uint8_t INT8_VAL = -100;
static const uint8_t INT8_BYTES[1] = {0x9C};
BOOST_AUTO_TEST_CASE(write_raw_8_unsigned) {
  std::stringstream ss;
  mp::write_raw_8(ss, UINT8_VAL);
  const std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 1);
  BOOST_CHECK_EQUAL(BYTE(s[0]), UINT8_BYTES[0]);
}
BOOST_AUTO_TEST_CASE(write_raw_8_signed) {
  std::stringstream ss;
  mp::write_raw_8(ss, INT8_VAL);
  const std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 1);
  BOOST_CHECK_EQUAL(BYTE(s[0]), INT8_BYTES[0]);
}

// ----------------------------------------------------------------------------
// write_raw_16
//  4957 = 0001 0011 0101 1101
// -4957 = 1110 1100 1010 0011
static const uint16_t UINT16_VAL = 4957;
static const uint8_t  UINT16_BYTES[2] = {0x13, 0x5D};
static const uint16_t INT16_VAL = -4957;
static const uint8_t  INT16_BYTES[2] = {0xEC, 0xA3};
BOOST_AUTO_TEST_CASE(write_raw_16_unsigned) {
  std::stringstream ss;
  mp::write_raw_16(ss, UINT16_VAL);
  const std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 2);
  BOOST_CHECK_EQUAL(BYTE(s[0]), UINT16_BYTES[0]);
  BOOST_CHECK_EQUAL(BYTE(s[1]), UINT16_BYTES[1]);
}
BOOST_AUTO_TEST_CASE(write_raw_16_signed) {
  std::stringstream ss;
  mp::write_raw_16(ss, INT16_VAL);
  const std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 2);
  BOOST_CHECK_EQUAL(BYTE(s[0]), INT16_BYTES[0]);
  BOOST_CHECK_EQUAL(BYTE(s[1]), INT16_BYTES[1]);
}


// ----------------------------------------------------------------------------
// write_raw_32
//  584667347 = 0010 0010 1101 1001 0101 0000 1101 0011
// -584667347 = 1101 1101 0010 0110 1010 1111 0010 1101
static const uint32_t UINT32_VAL = 584667347;
static const uint8_t  UINT32_BYTES[4] = {0x22, 0xD9, 0x50, 0xD3};
static const uint32_t INT32_VAL = -584667347;
static const uint8_t  INT32_BYTES[4] = {0xDD, 0x26, 0xAF, 0x2D};
BOOST_AUTO_TEST_CASE(write_raw_32_unsigned) {
  std::stringstream ss;
  mp::write_raw_32(ss, UINT32_VAL);
  const std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 4);
  BOOST_CHECK_EQUAL(BYTE(s[0]), UINT32_BYTES[0]);
  BOOST_CHECK_EQUAL(BYTE(s[1]), UINT32_BYTES[1]);
  BOOST_CHECK_EQUAL(BYTE(s[2]), UINT32_BYTES[2]);
  BOOST_CHECK_EQUAL(BYTE(s[3]), UINT32_BYTES[3]);
}
BOOST_AUTO_TEST_CASE(write_raw_32_signed) {
  std::stringstream ss;
  mp::write_raw_32(ss, INT32_VAL);
  const std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 4);
  BOOST_CHECK_EQUAL(BYTE(s[0]), INT32_BYTES[0]);
  BOOST_CHECK_EQUAL(BYTE(s[1]), INT32_BYTES[1]);
  BOOST_CHECK_EQUAL(BYTE(s[2]), INT32_BYTES[2]);
  BOOST_CHECK_EQUAL(BYTE(s[3]), INT32_BYTES[3]);
}


// ----------------------------------------------------------------------------
// write_raw_64
//  8436114578613100000 = 0111 0101 0001 0011 0001 1001 1000 0011 0100 0110 1011 1010 0101 1101 1110 0000
// -8436114578613100000 = 1000 1010 1110 1100 1110 0110 0111 1100 1011 1001 0100 0101 1010 0010 0010 0000
static const uint64_t UINT64_VAL = 8436114578613100000;
static const uint8_t  UINT64_BYTES[8] = {0x75, 0x13, 0x19, 0x83, 0x46, 0xBA, 0x5D, 0xE0};
static const uint64_t INT64_VAL = -8436114578613100000;
static const uint8_t  INT64_BYTES[8] = {0x8A, 0xEC, 0xE6, 0x7C, 0xB9, 0x45, 0xA2, 0x20};
BOOST_AUTO_TEST_CASE(write_raw_64_unsigned) {
  std::stringstream ss;
  mp::write_raw_64(ss, UINT64_VAL);
  const std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 8);
  BOOST_CHECK_EQUAL(BYTE(s[0]), UINT64_BYTES[0]);
  BOOST_CHECK_EQUAL(BYTE(s[1]), UINT64_BYTES[1]);
  BOOST_CHECK_EQUAL(BYTE(s[2]), UINT64_BYTES[2]);
  BOOST_CHECK_EQUAL(BYTE(s[3]), UINT64_BYTES[3]);
  BOOST_CHECK_EQUAL(BYTE(s[4]), UINT64_BYTES[4]);
  BOOST_CHECK_EQUAL(BYTE(s[5]), UINT64_BYTES[5]);
  BOOST_CHECK_EQUAL(BYTE(s[6]), UINT64_BYTES[6]);
  BOOST_CHECK_EQUAL(BYTE(s[7]), UINT64_BYTES[7]);
}
BOOST_AUTO_TEST_CASE(write_raw_64_signed) {
  std::stringstream ss;
  mp::write_raw_64(ss, INT64_VAL);
  const std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 8);
  BOOST_CHECK_EQUAL(BYTE(s[0]), INT64_BYTES[0]);
  BOOST_CHECK_EQUAL(BYTE(s[1]), INT64_BYTES[1]);
  BOOST_CHECK_EQUAL(BYTE(s[2]), INT64_BYTES[2]);
  BOOST_CHECK_EQUAL(BYTE(s[3]), INT64_BYTES[3]);
  BOOST_CHECK_EQUAL(BYTE(s[4]), INT64_BYTES[4]);
  BOOST_CHECK_EQUAL(BYTE(s[5]), INT64_BYTES[5]);
  BOOST_CHECK_EQUAL(BYTE(s[6]), INT64_BYTES[6]);
  BOOST_CHECK_EQUAL(BYTE(s[7]), INT64_BYTES[7]);
}



// ----------------------------------------------------------------------------
// write_nil
BOOST_AUTO_TEST_CASE(write_nil) {
  std::stringstream ss;
  mp::write_nil(ss);
  std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 1);
  BOOST_CHECK_EQUAL(BYTE(s[0]), mp::header::NIL);
}


BOOST_AUTO_TEST_CASE(write_boolean_true) {
  std::stringstream ss;
  mp::write_boolean(ss, true);
  std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 1);
  BOOST_CHECK_EQUAL(BYTE(s[0]), mp::header::TRUE);
}

BOOST_AUTO_TEST_CASE(write_boolean_false) {
  std::stringstream ss;
  mp::write_boolean(ss, false);
  std::string s = ss.str();
  BOOST_CHECK_EQUAL(s.size(), 1);
  BOOST_CHECK_EQUAL(BYTE(s[0]), mp::header::FALSE);
}


BOOST_AUTO_TEST_SUITE_END()
