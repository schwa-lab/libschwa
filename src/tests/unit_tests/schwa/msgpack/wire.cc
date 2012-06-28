/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include "test_utils.h"

#include <boost/preprocessor/list.hpp>
#include <boost/test/unit_test.hpp>

#define BYTES_BEGIN() \
  std::stringstream ss; \
  uint8_t expected[]

#define VALUE_BEGIN(type) \
  const type value

#define BYTES_WRITE_CHECK() \
  const std::string s = ss.str(); \
  BOOST_CHECK_EQUAL(s.size(), sizeof(expected)/sizeof(uint8_t)); \
  for (size_t i = 0; i != sizeof(expected)/sizeof(uint8_t); ++i) \
    BOOST_CHECK_EQUAL(static_cast<uint8_t>(s[i]), expected[i]);

#define BYTES_READ_HEADER_CHECK(value) \
  ss.seekg(0); \
  BOOST_CHECK_EQUAL(mp::read_peek(ss), value)

namespace mp = schwa::msgpack;



BOOST_AUTO_TEST_SUITE(schwa_msgpack_wire)

// ----------------------------------------------------------------------------
// write_raw_8
//  100 = 0110 0100
// -100 = 1001 1100
BOOST_AUTO_TEST_CASE(test_raw_8_unsigned) {
  BYTES_BEGIN() = {0x64};
  mp::write_raw_8(ss, 100);
  BYTES_WRITE_CHECK();
}

BOOST_AUTO_TEST_CASE(test_raw_8_signed) {
  BYTES_BEGIN() = {0x9c};
  mp::write_raw_8(ss, -100);
  BYTES_WRITE_CHECK();
}


// ----------------------------------------------------------------------------
// write_raw_16
//  4957 = 0001 0011 0101 1101
// -4957 = 1110 1100 1010 0011
BOOST_AUTO_TEST_CASE(test_raw_16_unsigned) {
  BYTES_BEGIN() = {0x13, 0x5D};
  mp::write_raw_16(ss, 4957);
  BYTES_WRITE_CHECK();
}

BOOST_AUTO_TEST_CASE(test_raw_16_signed) {
  BYTES_BEGIN() = {0xEC, 0xA3};
  mp::write_raw_16(ss, -4957);
  BYTES_WRITE_CHECK();
}


// ----------------------------------------------------------------------------
// write_raw_32
//  584667347 = 0010 0010 1101 1001 0101 0000 1101 0011
// -584667347 = 1101 1101 0010 0110 1010 1111 0010 1101
BOOST_AUTO_TEST_CASE(test_raw_32_unsigned) {
  BYTES_BEGIN() = {0x22, 0xD9, 0x50, 0xD3};
  mp::write_raw_32(ss, 584667347);
  BYTES_WRITE_CHECK();
}

BOOST_AUTO_TEST_CASE(test_raw_32_signed) {
  BYTES_BEGIN() = {0xDD, 0x26, 0xAF, 0x2D};
  mp::write_raw_32(ss, -584667347);
  BYTES_WRITE_CHECK();
}


// ----------------------------------------------------------------------------
// write_raw_64
//  8436114578613100000 = 0111 0101 0001 0011 0001 1001 1000 0011 0100 0110 1011 1010 0101 1101 1110 0000
// -8436114578613100000 = 1000 1010 1110 1100 1110 0110 0111 1100 1011 1001 0100 0101 1010 0010 0010 0000
BOOST_AUTO_TEST_CASE(test_raw_64_unsigned) {
  BYTES_BEGIN() = {0x75, 0x13, 0x19, 0x83, 0x46, 0xBA, 0x5D, 0xE0};
  mp::write_raw_64(ss, 8436114578613100000ULL);
  BYTES_WRITE_CHECK();
}

BOOST_AUTO_TEST_CASE(test_raw_64_signed) {
  BYTES_BEGIN() = {0x8A, 0xEC, 0xE6, 0x7C, 0xB9, 0x45, 0xA2, 0x20};
  mp::write_raw_64(ss, -8436114578613100000ULL);
  BYTES_WRITE_CHECK();
}



// ----------------------------------------------------------------------------
// write_nil
BOOST_AUTO_TEST_CASE(test_nil) {
  BYTES_BEGIN() = {mp::header::NIL};
  mp::write_nil(ss);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_NIL);
}


// ----------------------------------------------------------------------------
// write_boolean
BOOST_AUTO_TEST_CASE(test_boolean_true) {
  BYTES_BEGIN() = {mp::header::TRUE};
  VALUE_BEGIN(bool) = true;
  mp::write_boolean(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_TRUE);
  BOOST_CHECK_EQUAL(mp::read_boolean(ss), value);
}

BOOST_AUTO_TEST_CASE(test_boolean_false) {
  BYTES_BEGIN() = {mp::header::FALSE};
  VALUE_BEGIN(bool) = false;
  mp::write_boolean(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_FALSE);
  BOOST_CHECK_EQUAL(mp::read_boolean(ss), value);
}


// ----------------------------------------------------------------------------
// write_uint
BOOST_AUTO_TEST_CASE(test_uint_fixed) {
  BYTES_BEGIN() = {0x64};
  VALUE_BEGIN(uint8_t) = 100;
  mp::write_uint_fixed(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_FIXNUM_POSITIVE);
  BOOST_CHECK_EQUAL(mp::read_uint_fixed(ss), value);
}

BOOST_AUTO_TEST_CASE(test_uint_8_100) {
  BYTES_BEGIN() = {0x64};
  VALUE_BEGIN(uint64_t) = 100;
  mp::write_uint(ss, 100);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_FIXNUM_POSITIVE);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}
BOOST_AUTO_TEST_CASE(test_uint_8_128) {
  BYTES_BEGIN() = {mp::header::UINT_8, 0x80};
  VALUE_BEGIN(uint64_t) = 128;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_8);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}
BOOST_AUTO_TEST_CASE(test_uint_8_200) {
  BYTES_BEGIN() = {mp::header::UINT_8, 0xC8};
  VALUE_BEGIN(uint64_t) = 200;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_8);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}
BOOST_AUTO_TEST_CASE(test_uint_8_255) {
  BYTES_BEGIN() = {mp::header::UINT_8, 0xFF};
  VALUE_BEGIN(uint64_t) = 255;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_8);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}

BOOST_AUTO_TEST_CASE(test_uint_16_256) {
  BYTES_BEGIN() = {mp::header::UINT_16, 0x01, 0x00};
  VALUE_BEGIN(uint64_t) = 256;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_16);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}
BOOST_AUTO_TEST_CASE(test_uint_16_4957) {
  BYTES_BEGIN() = {mp::header::UINT_16, 0x13, 0x5D};
  VALUE_BEGIN(uint64_t) = 4957;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_16);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}
BOOST_AUTO_TEST_CASE(test_uint_16_65535) {
  BYTES_BEGIN() = {mp::header::UINT_16, 0xFF, 0xFF};
  VALUE_BEGIN(uint64_t) = 65535;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_16);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}

BOOST_AUTO_TEST_CASE(test_uint_32_65536) {
  BYTES_BEGIN() = {mp::header::UINT_32, 0x00, 0x01, 0x00, 0x00};
  VALUE_BEGIN(uint64_t) = 65536;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_32);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}
BOOST_AUTO_TEST_CASE(test_uint_32_584667347) {
  BYTES_BEGIN() = {mp::header::UINT_32, 0x22, 0xD9, 0x50, 0xD3};
  VALUE_BEGIN(uint64_t) = 584667347;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_32);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}
BOOST_AUTO_TEST_CASE(test_uint_32_4294967295) {
  BYTES_BEGIN() = {mp::header::UINT_32, 0xFF, 0xFF, 0xFF, 0xFF};
  VALUE_BEGIN(uint64_t) = 4294967295U;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_32);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}

BOOST_AUTO_TEST_CASE(test_uint_64_4294967296) {
  BYTES_BEGIN() = {mp::header::UINT_64, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
  VALUE_BEGIN(uint64_t) = 4294967296ULL;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_64);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}
BOOST_AUTO_TEST_CASE(test_uint_64_8436114578613100000) {
  BYTES_BEGIN() = {mp::header::UINT_64, 0x75, 0x13, 0x19, 0x83, 0x46, 0xBA, 0x5D, 0xE0};
  VALUE_BEGIN(uint64_t) = 8436114578613100000ULL;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_64);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}
BOOST_AUTO_TEST_CASE(test_uint_64_max) {
  BYTES_BEGIN() = {mp::header::UINT_64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  VALUE_BEGIN(uint64_t) = std::numeric_limits<uint64_t>::max();
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WIRE_UINT_64);
  BOOST_CHECK_EQUAL(mp::read_uint(ss), value);
}


#if 0
// ----------------------------------------------------------------------------
// write_array
BOOST_AUTO_TEST_CASE(test_array_fixed) {
  {
    mp::Object items[1];
    items[0].type = mp::TYPE_BOOLEAN; items[0].via._bool = true;
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
BOOST_AUTO_TEST_CASE(test_array_16) {
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
// write_raw
BOOST_AUTO_TEST_CASE(test_raw_fixed) {
  uint8_t expected[32];
  {
    size_t e = 0;
    uint8_t data[] = {0xC4};
    mp::RawObject raw = {1, reinterpret_cast<char *>(&data[0])};
    expected[e++] = 0xA1;
    expected[e++] = 0xC4;
    std::stringstream ss;
    mp::write_raw(ss, raw);
    BOOST_CHECK( compare_bytes(ss.str(), expected, e) );
  }
  {
    size_t e = 0;
    uint8_t data[31] = {0x8A, 0xAD, 0x00, 0x6D, 0xD, 0x37, 0x77, 0xDA, 0xC2, 0x99, 0xB6, 0xD8, 0x34, 0xD2, 0x9A, 0xB, 0x10, 0x68, 0xF2, 0xDE, 0x48, 0x2C, 0xF1, 0x9C, 0xCA, 0xC3, 0x93, 0xF, 0x70, 0x44, 0xAD};
    mp::RawObject raw = {31, reinterpret_cast<char *>(&data[0])};
    expected[e++] = 0xBF;
    std::memcpy(expected + 1, data, 31);
    e += 31;
    std::stringstream ss;
    mp::write_raw(ss, raw);
    BOOST_CHECK( compare_bytes(ss.str(), expected, e) );
  }
}
#endif


BOOST_AUTO_TEST_SUITE_END()
