/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <cstring>
#include <vector>

#include <schwa/unicode.h>


namespace schwa {

SUITE(schwa__unicode) {

TEST(test_read_utf8_single_cp_1byte) {
  const char *const start = u8"$";
  const char *const end = start + std::strlen(start);
  CHECK_EQUAL(1, end - start);
  const char *upto = start;
  unicode_t cp;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x0024, cp);
  CHECK_EQUAL(end, upto);
}


TEST(test_read_utf8_single_cp_2bytes) {
  const char *const start = u8"¢";
  const char *const end = start + std::strlen(start);
  CHECK_EQUAL(2, end - start);
  const char *upto = start;
  unicode_t cp;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x00A2, cp);
  CHECK_EQUAL(end, upto);
}


TEST(test_read_utf8_single_cp_3bytes) {
  const char *const start = u8"€";
  const char *const end = start + std::strlen(start);
  CHECK_EQUAL(3, end - start);
  const char *upto = start;
  unicode_t cp;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x20AC, cp);
  CHECK_EQUAL(end, upto);
}


TEST(test_read_utf8_single_cp_4bytes) {
  const char *const start = u8"𤭢";
  const char *const end = start + std::strlen(start);
  CHECK_EQUAL(4, end - start);
  const char *upto = start;
  unicode_t cp;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x24B62, cp);
  CHECK_EQUAL(end, upto);
}


TEST(test_read_utf8_bounds) {
  const char *start, *end, *upto;
  unicode_t cp;

  start = u8"\U00000000";
  upto = start;
  end = start + 1;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x00000000, cp);
  CHECK_EQUAL(end, upto);

  start = u8"\U0000007F";
  upto = start;
  end = start + 1;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x0000007F, cp);
  CHECK_EQUAL(end, upto);

  start = u8"\U00000080";
  upto = start;
  end = start + 2;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x00000080, cp);
  CHECK_EQUAL(end, upto);

  start = u8"\U000007FF";
  upto = start;
  end = start + 2;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x000007FF, cp);
  CHECK_EQUAL(end, upto);

  start = u8"\U00000800";
  upto = start;
  end = start + 3;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x00000800, cp);
  CHECK_EQUAL(end, upto);

  start = u8"\U0000FFFF";
  upto = start;
  end = start + 3;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x0000FFFF, cp);
  CHECK_EQUAL(end, upto);

  start = u8"\U00010000";
  upto = start;
  end = start + 4;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x00010000, cp);
  CHECK_EQUAL(end, upto);

  start = u8"\U0010FFFF";
  upto = start;
  end = start + 4;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x0010FFFF, cp);
  CHECK_EQUAL(end, upto);
}


TEST(test_read_utf8_multiple) {
  const char *const start = u8"κόσμε";
  const char *const end = start + std::strlen(start);
  const char *upto = start;
  std::vector<unicode_t> code_points;
  while (upto != end)
    code_points.push_back(read_utf8(&upto, end));
  CHECK_EQUAL(5, code_points.size());
  CHECK_EQUAL(0x3BA, code_points[0]);
  CHECK_EQUAL(0x1F79, code_points[1]);
  CHECK_EQUAL(0x3C3, code_points[2]);
  CHECK_EQUAL(0x3BC, code_points[3]);
  CHECK_EQUAL(0x3B5, code_points[4]);
}


TEST(test_utf8_decoder) {
  const std::string s(u8"κόσμε");
  UTF8Decoder d(s);
  UTF8Decoder::const_iterator it = d.cbegin();
  UTF8Decoder::const_iterator end = d.cend();
  CHECK(it == it);
  CHECK(end == end);

  CHECK(it != end);
  CHECK_EQUAL(0x3BA, *it);
  ++it;
  CHECK(it != end);
  CHECK_EQUAL(0x1F79, *it);
  ++it;
  CHECK(it != end);
  CHECK_EQUAL(0x3C3, *it);
  ++it;
  CHECK(it != end);
  CHECK_EQUAL(0x3BC, *it);
  ++it;
  CHECK(it != end);
  CHECK_EQUAL(0x3B5, *it);
  ++it;
  CHECK(it == end);
  ++it;
  CHECK(it == end);

  UnicodeString us0(d.cbegin(), d.cend());
  CHECK_EQUAL(5, us0.size());
  CHECK_EQUAL(0x3BA, us0[0]);
  CHECK_EQUAL(0x1F79, us0[1]);
  CHECK_EQUAL(0x3C3, us0[2]);
  CHECK_EQUAL(0x3BC, us0[3]);
  CHECK_EQUAL(0x3B5, us0[4]);

  const UnicodeString us1 = UTF8Decoder::to_string(s);
  CHECK_EQUAL(5, us1.size());
  CHECK_EQUAL(0x3BA, us1[0]);
  CHECK_EQUAL(0x1F79, us1[1]);
  CHECK_EQUAL(0x3C3, us1[2]);
  CHECK_EQUAL(0x3BC, us1[3]);
  CHECK_EQUAL(0x3B5, us1[4]);

  std::vector<unicode_t> code_points;
  for (auto cp : UTF8Decoder(s))
    code_points.push_back(cp);
  CHECK_EQUAL(5, code_points.size());
  CHECK_EQUAL(0x3BA, code_points[0]);
  CHECK_EQUAL(0x1F79, code_points[1]);
  CHECK_EQUAL(0x3C3, code_points[2]);
  CHECK_EQUAL(0x3BC, code_points[3]);
  CHECK_EQUAL(0x3B5, code_points[4]);
}

}  // SUITE

}  // namespace schwa
