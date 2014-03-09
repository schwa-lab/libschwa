/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

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
  CHECK_EQUAL(expected, header_type(ss.peek()))

#define BYTES_CONSUMED_CHECK() \
  CHECK_EQUAL(sizeof(expected)/sizeof(uint8_t), ss.tellg())

namespace schwa {
namespace msgpack {

SUITE(schwa__msgpack__wire) {

// ----------------------------------------------------------------------------
// _write_be8
//  100 = 0110 0100
// -100 = 1001 1100
// ----------------------------------------------------------------------------
TEST(test_write_be8_unsigned) {
  BYTES_BEGIN() = {0x64};
  const uint8_t x = 100;
  _write_be8(ss, &x);
  BYTES_WRITE_CHECK();
}

TEST(test_write_be8_signed) {
  BYTES_BEGIN() = {0x9c};
  const int8_t x = -100;
  _write_be8(ss, &x);
  BYTES_WRITE_CHECK();
}


// ----------------------------------------------------------------------------
// _write_be16
//  4957 = 0001 0011 0101 1101
// -4957 = 1110 1100 1010 0011
// ----------------------------------------------------------------------------
TEST(test_write_be16_unsigned) {
  BYTES_BEGIN() = {0x13, 0x5D};
  const uint16_t x = 4957;
  _write_be16(ss, &x);
  BYTES_WRITE_CHECK();
}

TEST(test_write_be16_signed) {
  BYTES_BEGIN() = {0xEC, 0xA3};
  const int16_t x = -4957;
  _write_be16(ss, &x);
  BYTES_WRITE_CHECK();
}


// ----------------------------------------------------------------------------
// _write_be32
//  584667347 = 0010 0010 1101 1001 0101 0000 1101 0011
// -584667347 = 1101 1101 0010 0110 1010 1111 0010 1101
// ----------------------------------------------------------------------------
TEST(test_write_be32_unsigned) {
  BYTES_BEGIN() = {0x22, 0xD9, 0x50, 0xD3};
  const uint32_t x = 584667347;
  _write_be32(ss, &x);
  BYTES_WRITE_CHECK();
}

TEST(test_write_be32_signed) {
  BYTES_BEGIN() = {0xDD, 0x26, 0xAF, 0x2D};
  const int32_t x = -584667347;
  _write_be32(ss, &x);
  BYTES_WRITE_CHECK();
}


// ----------------------------------------------------------------------------
// _write_be64
//  8436114578613100000 = 0111 0101 0001 0011 0001 1001 1000 0011 0100 0110 1011 1010 0101 1101 1110 0000
// -8436114578613100000 = 1000 1010 1110 1100 1110 0110 0111 1100 1011 1001 0100 0101 1010 0010 0010 0000
// ----------------------------------------------------------------------------
TEST(test_write_be64_unsigned) {
  BYTES_BEGIN() = {0x75, 0x13, 0x19, 0x83, 0x46, 0xBA, 0x5D, 0xE0};
  const uint64_t x = 8436114578613100000ULL;
  _write_be64(ss, &x);
  BYTES_WRITE_CHECK();
}

TEST(test_write_be64_signed) {
  BYTES_BEGIN() = {0x8A, 0xEC, 0xE6, 0x7C, 0xB9, 0x45, 0xA2, 0x20};
  const int64_t x = -8436114578613100000LL;
  _write_be64(ss, &x);
  BYTES_WRITE_CHECK();
}



// ----------------------------------------------------------------------------
// write_nil
// ----------------------------------------------------------------------------
TEST(test_nil) {
  BYTES_BEGIN() = {header::NIL};
  write_nil(ss);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::NIL);
  read_nil(ss);
  BYTES_CONSUMED_CHECK();
}


// ----------------------------------------------------------------------------
// write_bool
// ----------------------------------------------------------------------------
TEST(test_boolean_true) {
  BYTES_BEGIN() = {header::TRUE};
  VALUE_BEGIN(bool) = true;
  write_bool(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::TRUE);
  CHECK_EQUAL(value, read_bool(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_boolean_false) {
  BYTES_BEGIN() = {header::FALSE};
  VALUE_BEGIN(bool) = false;
  write_bool(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::FALSE);
  CHECK_EQUAL(value, read_bool(ss));
  BYTES_CONSUMED_CHECK();
}


// ----------------------------------------------------------------------------
// write_uint
// ----------------------------------------------------------------------------
TEST(test_uint_fixed) {
  BYTES_BEGIN() = {0x64};
  VALUE_BEGIN(uint8_t) = 100;
  write_uint_fixed(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::FIXNUM_POSITIVE);
  CHECK_EQUAL(value, read_uint_fixed(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_uint_8_100) {
  BYTES_BEGIN() = {0x64};
  VALUE_BEGIN(uint64_t) = 100;
  write_uint(ss, 100);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::FIXNUM_POSITIVE);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_8_128) {
  BYTES_BEGIN() = {header::UINT_8, 0x80};
  VALUE_BEGIN(uint64_t) = 128;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_8);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_8_200) {
  BYTES_BEGIN() = {header::UINT_8, 0xC8};
  VALUE_BEGIN(uint64_t) = 200;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_8);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_8_255) {
  BYTES_BEGIN() = {header::UINT_8, 0xFF};
  VALUE_BEGIN(uint64_t) = 255;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_8);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_uint_16_256) {
  BYTES_BEGIN() = {header::UINT_16, 0x01, 0x00};
  VALUE_BEGIN(uint64_t) = 256;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_16);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_16_4957) {
  BYTES_BEGIN() = {header::UINT_16, 0x13, 0x5D};
  VALUE_BEGIN(uint64_t) = 4957;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_16);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_16_65535) {
  BYTES_BEGIN() = {header::UINT_16, 0xFF, 0xFF};
  VALUE_BEGIN(uint64_t) = 65535;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_16);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_uint_32_65536) {
  BYTES_BEGIN() = {header::UINT_32, 0x00, 0x01, 0x00, 0x00};
  VALUE_BEGIN(uint64_t) = 65536;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_32);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_32_584667347) {
  BYTES_BEGIN() = {header::UINT_32, 0x22, 0xD9, 0x50, 0xD3};
  VALUE_BEGIN(uint64_t) = 584667347;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_32);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_32_4294967295) {
  BYTES_BEGIN() = {header::UINT_32, 0xFF, 0xFF, 0xFF, 0xFF};
  VALUE_BEGIN(uint64_t) = 4294967295U;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_32);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_uint_64_4294967296) {
  BYTES_BEGIN() = {header::UINT_64, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
  VALUE_BEGIN(uint64_t) = 4294967296ULL;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_64);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_64_8436114578613100000) {
  BYTES_BEGIN() = {header::UINT_64, 0x75, 0x13, 0x19, 0x83, 0x46, 0xBA, 0x5D, 0xE0};
  VALUE_BEGIN(uint64_t) = 8436114578613100000ULL;
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_64);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}
TEST(test_uint_64_max) {
  BYTES_BEGIN() = {header::UINT_64, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  VALUE_BEGIN(uint64_t) = std::numeric_limits<uint64_t>::max();
  write_uint(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::UINT_64);
  CHECK_EQUAL(value, read_uint(ss));
  BYTES_CONSUMED_CHECK();
}


// ----------------------------------------------------------------------------
// write_float
// ----------------------------------------------------------------------------
TEST(test_float) {
  BYTES_BEGIN() = {0xCA, 0x46, 0x40, 0xE4, 0x90};
  VALUE_BEGIN(float) = 12345.141;
  write_float(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::FLOAT);
  CHECK_EQUAL(value, read_float(ss));
  BYTES_CONSUMED_CHECK();
}


// ----------------------------------------------------------------------------
// write_double
// ----------------------------------------------------------------------------
TEST(test_double) {
  BYTES_BEGIN() = {0xCB, 0x40, 0xC8, 0x1C, 0x92, 0x0C, 0x49, 0xBA, 0x5E};
  VALUE_BEGIN(double) = 12345.141;
  write_double(ss, value);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::DOUBLE);
  CHECK_EQUAL(value, read_double(ss));
  BYTES_CONSUMED_CHECK();
}


// ----------------------------------------------------------------------------
// write_array_size
// ----------------------------------------------------------------------------
TEST(test_array_fixed_0) {
  BYTES_BEGIN() = {header::ARRAY_FIXED};
  const size_t size = 0;
  write_array_size(ss, size);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::ARRAY_FIXED);
  CHECK_EQUAL(size, read_array_size(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_array_fixed_15) {
  BYTES_BEGIN() = {header::ARRAY_FIXED | 0x0F, header::NIL, header::TRUE, header::FALSE, header::NIL, header::TRUE, header::FALSE, header::NIL, header::TRUE, header::FALSE, header::NIL, header::TRUE, header::FALSE, 8, header::UINT_8, 200, header::NIL};
  const size_t size = 15;
  write_array_size(ss, size);
  for (int i = 0; i != 4; ++i) {
    write_nil(ss);
    write_bool(ss, true);
    write_bool(ss, false);
  }
  write_uint(ss, 8);
  write_uint(ss, 200);
  write_nil(ss);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::ARRAY_FIXED);
  CHECK_EQUAL(size, read_array_size(ss));

  bool b;
  uint64_t u;
  for (int i = 0; i != 4; ++i) {
    read_nil(ss);
    b = read_bool(ss);
    CHECK_EQUAL(true, b);
    b = read_bool(ss);
    CHECK_EQUAL(false, b);
  }
  u = read_uint(ss);
  CHECK_EQUAL(8, u);
  u = read_uint(ss);
  CHECK_EQUAL(200, u);
  read_nil(ss);

  BYTES_CONSUMED_CHECK();
}

TEST(test_array_fixed_16) {
  BYTES_BEGIN() = {header::ARRAY_16, 0, 16, header::NIL, header::TRUE, header::FALSE, header::NIL, header::TRUE, header::FALSE, header::NIL, header::TRUE, header::FALSE, header::NIL, header::TRUE, header::FALSE, 8, header::UINT_8, 200, header::NIL, header::NIL};
  const size_t size = 16;
  write_array_size(ss, size);
  for (int i = 0; i != 4; ++i) {
    write_nil(ss);
    write_bool(ss, true);
    write_bool(ss, false);
  }
  write_uint(ss, 8);
  write_uint(ss, 200);
  write_nil(ss);
  write_nil(ss);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::ARRAY_16);
  CHECK_EQUAL(size, read_array_size(ss));

  bool b;
  uint64_t u;
  for (int i = 0; i != 4; ++i) {
    read_nil(ss);
    b = read_bool(ss);
    CHECK_EQUAL(true, b);
    b = read_bool(ss);
    CHECK_EQUAL(false, b);
  }
  u = read_uint(ss);
  CHECK_EQUAL(8, u);
  u = read_uint(ss);
  CHECK_EQUAL(200, u);
  read_nil(ss);
  read_nil(ss);

  BYTES_CONSUMED_CHECK();
}


// ----------------------------------------------------------------------------
// write_map_size
// ----------------------------------------------------------------------------
TEST(test_map_fixed_0) {
  BYTES_BEGIN() = {header::MAP_FIXED};
  const size_t size = 0;
  write_map_size(ss, size);
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::MAP_FIXED);
  CHECK_EQUAL(size, read_map_size(ss));
  BYTES_CONSUMED_CHECK();
}

TEST(test_map_fixed_15) {
  BYTES_BEGIN() = {header::MAP_FIXED | 0x0F, 0, header::TRUE, 1, header::FALSE, 2, header::TRUE, 3, header::FALSE, 4, header::TRUE, 5, header::FALSE, 6, header::TRUE, 7, header::FALSE, 8, header::TRUE, 9, header::FALSE, 10, header::TRUE, 11, header::FALSE, 12, header::TRUE, 13, header::FALSE, 14, header::TRUE};
  const size_t size = 15;
  write_map_size(ss, size);
  for (size_t i = 0; i != size; ++i) {
    write_uint(ss, i);
    write_bool(ss, (i % 2) == 0);
  }
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::MAP_FIXED);
  CHECK_EQUAL(size, read_map_size(ss));

  bool b;
  uint64_t u;
  for (size_t i = 0; i != size; ++i) {
    u = read_uint(ss);
    CHECK_EQUAL(i, u);
    b = read_bool(ss);
    CHECK_EQUAL((i % 2) == 0, b);
  }

  BYTES_CONSUMED_CHECK();
}

TEST(test_map_fixed_16) {
  BYTES_BEGIN() = {header::MAP_16, 0, 16, 0, header::TRUE, 1, header::FALSE, 2, header::TRUE, 3, header::FALSE, 4, header::TRUE, 5, header::FALSE, 6, header::TRUE, 7, header::FALSE, 8, header::TRUE, 9, header::FALSE, 10, header::TRUE, 11, header::FALSE, 12, header::TRUE, 13, header::FALSE, 14, header::TRUE, 15, header::FALSE};
  const size_t size = 16;
  write_map_size(ss, size);
  for (size_t i = 0; i != size; ++i) {
    write_uint(ss, i);
    write_bool(ss, (i % 2) == 0);
  }
  BYTES_WRITE_CHECK();
  BYTES_READ_HEADER_CHECK(WireType::MAP_16);
  CHECK_EQUAL(size, read_map_size(ss));

  bool b;
  uint64_t u;
  for (size_t i = 0; i != size; ++i) {
    u = read_uint(ss);
    CHECK_EQUAL(i, u);
    b = read_bool(ss);
    CHECK_EQUAL((i % 2) == 0, b);
  }

  BYTES_CONSUMED_CHECK();
}

}  // SUITE

}  // namespace msgpack
}  // namespace schwa
