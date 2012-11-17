/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "test_utils.h"

#include <schwa/msgpack/wire.h>

#define BYTES_BEGIN() \
  std::stringstream ss; \
  uint8_t expected[]

#define VALUE_BEGIN(type) \
  const type value

#define BYTES_WRITE_CHECK() \
  const std::string s = ss.str(); \
  CHECK_COMPARE_BYTES3(expected, sizeof(expected)/sizeof(uint8_t), s)

#define BYTES_READ_HEADER_CHECK(expected) \
  ss.seekg(0); \
  CHECK_EQUAL(expected, mp::header_type(ss.peek()))

#define BYTES_CONSUMED_CHECK() \
  CHECK_EQUAL(sizeof(expected)/sizeof(uint8_t), ss.tellg())

namespace mp = schwa::msgpack;


namespace schwatest {

SUITE(schwa__msgpack__wire) {

// ----------------------------------------------------------------------------
// write_bytes_8
//  100 = 0110 0100
// -100 = 1001 1100
// ----------------------------------------------------------------------------
TEST(test_raw_8_unsigned) {
  BYTES_BEGIN() = {0x64};
  mp::write_bytes_8(ss, 100);
  BYTES_WRITE_CHECK();
}

TEST(test_raw_8_signed) {
  BYTES_BEGIN() = {0x9c};
  mp::write_bytes_8(ss, -100);
  BYTES_WRITE_CHECK();
}


// ----------------------------------------------------------------------------
// write_bytes_16
//  4957 = 0001 0011 0101 1101
// -4957 = 1110 1100 1010 0011
// ----------------------------------------------------------------------------
TEST(test_raw_16_unsigned) {
  BYTES_BEGIN() = {0x13, 0x5D};
  mp::write_bytes_16(ss, 4957);
  BYTES_WRITE_CHECK();
}

TEST(test_raw_16_signed) {
  BYTES_BEGIN() = {0xEC, 0xA3};
  mp::write_bytes_16(ss, -4957);
  BYTES_WRITE_CHECK();
}


// ----------------------------------------------------------------------------
// write_bytes_32
//  584667347 = 0010 0010 1101 1001 0101 0000 1101 0011
// -584667347 = 1101 1101 0010 0110 1010 1111 0010 1101
// ----------------------------------------------------------------------------
TEST(test_raw_32_unsigned) {
  BYTES_BEGIN() = {0x22, 0xD9, 0x50, 0xD3};
  mp::write_bytes_32(ss, 584667347);
  BYTES_WRITE_CHECK();
}

TEST(test_raw_32_signed) {
  BYTES_BEGIN() = {0xDD, 0x26, 0xAF, 0x2D};
  mp::write_bytes_32(ss, -584667347);
  BYTES_WRITE_CHECK();
}


// ----------------------------------------------------------------------------
// write_bytes_64
//  8436114578613100000 = 0111 0101 0001 0011 0001 1001 1000 0011 0100 0110 1011 1010 0101 1101 1110 0000
// -8436114578613100000 = 1000 1010 1110 1100 1110 0110 0111 1100 1011 1001 0100 0101 1010 0010 0010 0000
// ----------------------------------------------------------------------------
TEST(test_raw_64_unsigned) {
  BYTES_BEGIN() = {0x75, 0x13, 0x19, 0x83, 0x46, 0xBA, 0x5D, 0xE0};
  mp::write_bytes_64(ss, 8436114578613100000ULL);
  BYTES_WRITE_CHECK();
}

TEST(test_raw_64_signed) {
  BYTES_BEGIN() = {0x8A, 0xEC, 0xE6, 0x7C, 0xB9, 0x45, 0xA2, 0x20};
  mp::write_bytes_64(ss, -8436114578613100000ULL);
  BYTES_WRITE_CHECK();
}



// ----------------------------------------------------------------------------
// write_nil
// ----------------------------------------------------------------------------
TEST(test_nil) {
  BYTES_BEGIN() = {mp::header::NIL};
  mp::write_nil(ss);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::NIL);
  mp::read_nil(ss);
  BYTES_CONSUMED_CHECK();
}


// ----------------------------------------------------------------------------
// write_bool
// ----------------------------------------------------------------------------
TEST(test_boolean_true) {
  BYTES_BEGIN() = {mp::header::TRUE};
  VALUE_BEGIN(bool) = true;
  mp::write_bool(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::TRUE);
  CHECK_EQUAL(value, mp::read_bool(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_boolean_false) {
  BYTES_BEGIN() = {mp::header::FALSE};
  VALUE_BEGIN(bool) = false;
  mp::write_bool(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::FALSE);
  CHECK_EQUAL(value, mp::read_bool(ss));
  BYTES_CONSUMED_CHECK();
}


// ----------------------------------------------------------------------------
// write_uint
// ----------------------------------------------------------------------------
TEST(test_uint_fixed) {
  BYTES_BEGIN() = {0x64};
  VALUE_BEGIN(uint8_t) = 100;
  mp::write_uint_fixed(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::FIXNUM_POSITIVE);
  CHECK_EQUAL(value, mp::read_uint_fixed(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_uint_8_100) {
  BYTES_BEGIN() = {0x64};
  VALUE_BEGIN(uint64_t) = 100;
  mp::write_uint(ss, 100);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::FIXNUM_POSITIVE);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_8_128) {
  BYTES_BEGIN() = {mp::header::UINT_8, 0x80};
  VALUE_BEGIN(uint64_t) = 128;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_8);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_8_200) {
  BYTES_BEGIN() = {mp::header::UINT_8, 0xC8};
  VALUE_BEGIN(uint64_t) = 200;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_8);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_8_255) {
  BYTES_BEGIN() = {mp::header::UINT_8, 0xFF};
  VALUE_BEGIN(uint64_t) = 255;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_8);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_uint_16_256) {
  BYTES_BEGIN() = {mp::header::UINT_16, 0x01, 0x00};
  VALUE_BEGIN(uint64_t) = 256;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_16);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_16_4957) {
  BYTES_BEGIN() = {mp::header::UINT_16, 0x13, 0x5D};
  VALUE_BEGIN(uint64_t) = 4957;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_16);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_16_65535) {
  BYTES_BEGIN() = {mp::header::UINT_16, 0xFF, 0xFF};
  VALUE_BEGIN(uint64_t) = 65535;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_16);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_uint_32_65536) {
  BYTES_BEGIN() = {mp::header::UINT_32, 0x00, 0x01, 0x00, 0x00};
  VALUE_BEGIN(uint64_t) = 65536;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_32);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_32_584667347) {
  BYTES_BEGIN() = {mp::header::UINT_32, 0x22, 0xD9, 0x50, 0xD3};
  VALUE_BEGIN(uint64_t) = 584667347;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_32);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_32_4294967295) {
  BYTES_BEGIN() = {mp::header::UINT_32, 0xFF, 0xFF, 0xFF, 0xFF};
  VALUE_BEGIN(uint64_t) = 4294967295U;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_32);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_uint_64_4294967296) {
  BYTES_BEGIN() = {mp::header::UINT_64, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
  VALUE_BEGIN(uint64_t) = 4294967296ULL;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_64);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_64_8436114578613100000) {
  BYTES_BEGIN() = {mp::header::UINT_64, 0x75, 0x13, 0x19, 0x83, 0x46, 0xBA, 0x5D, 0xE0};
  VALUE_BEGIN(uint64_t) = 8436114578613100000ULL;
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_64);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_64_max) {
  BYTES_BEGIN() = {mp::header::UINT_64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  VALUE_BEGIN(uint64_t) = std::numeric_limits<uint64_t>::max();
  mp::write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::UINT_64);
  CHECK_EQUAL(value, mp::read_uint(ss));
  BYTES_CONSUMED_CHECK();
}


// ----------------------------------------------------------------------------
// write_array_size
// ----------------------------------------------------------------------------
TEST(test_array_fixed_0) {
  BYTES_BEGIN() = {mp::header::ARRAY_FIXED};
  const size_t size = 0;
  mp::write_array_size(ss, size);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::ARRAY_FIXED);
  CHECK_EQUAL(size, mp::read_array_size(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_array_fixed_15) {
  BYTES_BEGIN() = {mp::header::ARRAY_FIXED | 0x0F, mp::header::NIL, mp::header::TRUE, mp::header::FALSE, mp::header::NIL, mp::header::TRUE, mp::header::FALSE, mp::header::NIL, mp::header::TRUE, mp::header::FALSE, mp::header::NIL, mp::header::TRUE, mp::header::FALSE, 8, mp::header::UINT_8, 200, mp::header::NIL};
  const size_t size = 15;
  mp::write_array_size(ss, size);
  for (int i = 0; i != 4; ++i) {
    mp::write_nil(ss);
    mp::write_bool(ss, true);
    mp::write_bool(ss, false);
  }
  mp::write_uint(ss, 8);
  mp::write_uint(ss, 200);
  mp::write_nil(ss);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::ARRAY_FIXED);
  CHECK_EQUAL(size, mp::read_array_size(ss));

  bool b;
  uint64_t u;
  for (int i = 0; i != 4; ++i) {
    mp::read_nil(ss);
    b = mp::read_bool(ss);
    CHECK_EQUAL(true, b);
    b = mp::read_bool(ss);
    CHECK_EQUAL(false, b);
  }
  u = mp::read_uint(ss);
  CHECK_EQUAL(8, u);
  u = mp::read_uint(ss);
  CHECK_EQUAL(200, u);
  mp::read_nil(ss);

  BYTES_CONSUMED_CHECK();
}

TEST(test_array_fixed_16) {
  BYTES_BEGIN() = {mp::header::ARRAY_16, 0, 16, mp::header::NIL, mp::header::TRUE, mp::header::FALSE, mp::header::NIL, mp::header::TRUE, mp::header::FALSE, mp::header::NIL, mp::header::TRUE, mp::header::FALSE, mp::header::NIL, mp::header::TRUE, mp::header::FALSE, 8, mp::header::UINT_8, 200, mp::header::NIL, mp::header::NIL};
  const size_t size = 16;
  mp::write_array_size(ss, size);
  for (int i = 0; i != 4; ++i) {
    mp::write_nil(ss);
    mp::write_bool(ss, true);
    mp::write_bool(ss, false);
  }
  mp::write_uint(ss, 8);
  mp::write_uint(ss, 200);
  mp::write_nil(ss);
  mp::write_nil(ss);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::ARRAY_16);
  CHECK_EQUAL(size, mp::read_array_size(ss));

  bool b;
  uint64_t u;
  for (int i = 0; i != 4; ++i) {
    mp::read_nil(ss);
    b = mp::read_bool(ss);
    CHECK_EQUAL(true, b);
    b = mp::read_bool(ss);
    CHECK_EQUAL(false, b);
  }
  u = mp::read_uint(ss);
  CHECK_EQUAL(8, u);
  u = mp::read_uint(ss);
  CHECK_EQUAL(200, u);
  mp::read_nil(ss);
  mp::read_nil(ss);

  BYTES_CONSUMED_CHECK();
}


// ----------------------------------------------------------------------------
// write_map_size
// ----------------------------------------------------------------------------
TEST(test_map_fixed_0) {
  BYTES_BEGIN() = {mp::header::MAP_FIXED};
  const size_t size = 0;
  mp::write_map_size(ss, size);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::MAP_FIXED);
  CHECK_EQUAL(size, mp::read_map_size(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_map_fixed_15) {
  BYTES_BEGIN() = {mp::header::MAP_FIXED | 0x0F, 0, mp::header::TRUE, 1, mp::header::FALSE, 2, mp::header::TRUE, 3, mp::header::FALSE, 4, mp::header::TRUE, 5, mp::header::FALSE, 6, mp::header::TRUE, 7, mp::header::FALSE, 8, mp::header::TRUE, 9, mp::header::FALSE, 10, mp::header::TRUE, 11, mp::header::FALSE, 12, mp::header::TRUE, 13, mp::header::FALSE, 14, mp::header::TRUE};
  const size_t size = 15;
  mp::write_map_size(ss, size);
  for (size_t i = 0; i != size; ++i) {
    mp::write_uint(ss, i);
    mp::write_bool(ss, (i % 2) == 0);
  }
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::MAP_FIXED);
  CHECK_EQUAL(size, mp::read_map_size(ss));

  bool b;
  uint64_t u;
  for (size_t i = 0; i != size; ++i) {
    u = mp::read_uint(ss);
    CHECK_EQUAL(i, u);
    b = mp::read_bool(ss);
    CHECK_EQUAL((i % 2) == 0, b);
  }

  BYTES_CONSUMED_CHECK();
}

TEST(test_map_fixed_16) {
  BYTES_BEGIN() = {mp::header::MAP_16, 0, 16, 0, mp::header::TRUE, 1, mp::header::FALSE, 2, mp::header::TRUE, 3, mp::header::FALSE, 4, mp::header::TRUE, 5, mp::header::FALSE, 6, mp::header::TRUE, 7, mp::header::FALSE, 8, mp::header::TRUE, 9, mp::header::FALSE, 10, mp::header::TRUE, 11, mp::header::FALSE, 12, mp::header::TRUE, 13, mp::header::FALSE, 14, mp::header::TRUE, 15, mp::header::FALSE};
  const size_t size = 16;
  mp::write_map_size(ss, size);
  for (size_t i = 0; i != size; ++i) {
    mp::write_uint(ss, i);
    mp::write_bool(ss, (i % 2) == 0);
  }
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(mp::WireType::MAP_16);
  CHECK_EQUAL(size, mp::read_map_size(ss));

  bool b;
  uint64_t u;
  for (size_t i = 0; i != size; ++i) {
    u = mp::read_uint(ss);
    CHECK_EQUAL(i, u);
    b = mp::read_bool(ss);
    CHECK_EQUAL((i % 2) == 0, b);
  }

  BYTES_CONSUMED_CHECK();
}

}  // SUITE

}  // namespace schwatest
