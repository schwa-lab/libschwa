/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cstring>
#include <vector>

#include <schwa/unicode.h>
#include <schwa/unittest.h>


namespace schwa {
namespace unicode {

SUITE(schwa__unicode) {

TEST(test_read_write_utf8_single_cp_1byte) {
  const char *const utf8 = u8"$";
  const char *const end = utf8 + std::strlen(utf8);
  CHECK_EQUAL(1, end - utf8);
  const char *upto = utf8;
  unicode_t cp;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x0024, cp);
  CHECK_EQUAL(end, upto);

  char encoded[4];
  const size_t n = write_utf8(cp, encoded);
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(utf8[0], encoded[0]);
}


TEST(test_read_write_utf8_single_cp_2bytes) {
  const char *const utf8 = u8"¢";
  const char *const end = utf8 + std::strlen(utf8);
  CHECK_EQUAL(2, end - utf8);
  const char *upto = utf8;
  unicode_t cp;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x00A2, cp);
  CHECK_EQUAL(end, upto);

  char encoded[4];
  const size_t n = write_utf8(cp, encoded);
  CHECK_EQUAL(2, n);
  CHECK_EQUAL(utf8[0], encoded[0]);
  CHECK_EQUAL(utf8[1], encoded[1]);
}


TEST(test_read_write_utf8_single_cp_3bytes) {
  const char *const utf8 = u8"€";
  const char *const end = utf8 + std::strlen(utf8);
  CHECK_EQUAL(3, end - utf8);
  const char *upto = utf8;
  unicode_t cp;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x20AC, cp);
  CHECK_EQUAL(end, upto);

  char encoded[4];
  const size_t n = write_utf8(cp, encoded);
  CHECK_EQUAL(3, n);
  CHECK_EQUAL(utf8[0], encoded[0]);
  CHECK_EQUAL(utf8[1], encoded[1]);
  CHECK_EQUAL(utf8[2], encoded[2]);
}


TEST(test_read_write_utf8_single_cp_4bytes) {
  const char *const utf8 = u8"𤭢";
  const char *const end = utf8 + std::strlen(utf8);
  CHECK_EQUAL(4, end - utf8);
  const char *upto = utf8;
  unicode_t cp;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x24B62, cp);
  CHECK_EQUAL(end, upto);

  char encoded[4];
  const size_t n = write_utf8(cp, encoded);
  CHECK_EQUAL(4, n);
  CHECK_EQUAL(utf8[0], encoded[0]);
  CHECK_EQUAL(utf8[1], encoded[1]);
  CHECK_EQUAL(utf8[2], encoded[2]);
  CHECK_EQUAL(utf8[3], encoded[3]);
}


TEST(test_read_write_utf8_bounds) {
  const char *start, *end, *upto;
  char utf8[4];
  unicode_t cp;
  size_t n;

  start = "\0";
  upto = start;
  end = start + 1;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x00000000, cp);
  CHECK_EQUAL(end, upto);
  n = write_utf8(cp, utf8);
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(start[0], utf8[0]);

  start = u8"\U0000007F";
  upto = start;
  end = start + 1;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x0000007F, cp);
  CHECK_EQUAL(end, upto);
  n = write_utf8(cp, utf8);
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(start[0], utf8[0]);

  start = u8"\U00000080";
  upto = start;
  end = start + 2;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x00000080, cp);
  CHECK_EQUAL(end, upto);
  n = write_utf8(cp, utf8);
  CHECK_EQUAL(2, n);
  CHECK_EQUAL(start[0], utf8[0]);
  CHECK_EQUAL(start[1], utf8[1]);

  start = u8"\U000007FF";
  upto = start;
  end = start + 2;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x000007FF, cp);
  CHECK_EQUAL(end, upto);
  n = write_utf8(cp, utf8);
  CHECK_EQUAL(2, n);
  CHECK_EQUAL(start[0], utf8[0]);
  CHECK_EQUAL(start[1], utf8[1]);

  start = u8"\U00000800";
  upto = start;
  end = start + 3;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x00000800, cp);
  CHECK_EQUAL(end, upto);
  n = write_utf8(cp, utf8);
  CHECK_EQUAL(3, n);
  CHECK_EQUAL(start[0], utf8[0]);
  CHECK_EQUAL(start[1], utf8[1]);
  CHECK_EQUAL(start[2], utf8[2]);

  start = u8"\U0000FFFF";
  upto = start;
  end = start + 3;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x0000FFFF, cp);
  CHECK_EQUAL(end, upto);
  n = write_utf8(cp, utf8);
  CHECK_EQUAL(3, n);
  CHECK_EQUAL(start[0], utf8[0]);
  CHECK_EQUAL(start[1], utf8[1]);
  CHECK_EQUAL(start[2], utf8[2]);

  start = u8"\U00010000";
  upto = start;
  end = start + 4;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x00010000, cp);
  CHECK_EQUAL(end, upto);
  n = write_utf8(cp, utf8);
  CHECK_EQUAL(4, n);
  CHECK_EQUAL(start[0], utf8[0]);
  CHECK_EQUAL(start[1], utf8[1]);
  CHECK_EQUAL(start[2], utf8[2]);
  CHECK_EQUAL(start[3], utf8[3]);

  start = u8"\U0010FFFF";
  upto = start;
  end = start + 4;
  cp = read_utf8(&upto, end);
  CHECK_EQUAL(0x0010FFFF, cp);
  CHECK_EQUAL(end, upto);
  n = write_utf8(cp, utf8);
  CHECK_EQUAL(4, n);
  CHECK_EQUAL(start[0], utf8[0]);
  CHECK_EQUAL(start[1], utf8[1]);
  CHECK_EQUAL(start[2], utf8[2]);
  CHECK_EQUAL(start[3], utf8[3]);
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

  std::ostringstream ss;
  write_utf8(UnicodeString(code_points.begin(), code_points.end()), ss);
  const std::string utf8 = ss.str();
  CHECK_EQUAL(std::strlen(start), utf8.size());
  CHECK_EQUAL(start, utf8.c_str());
}


TEST(test_utf8_decoder) {
  const std::string s(u8"κόσμε");
  UTF8Decoder d(s);

  {
    auto it = d.cbegin();
    auto end = d.cend();
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
  }

  {
    auto it = d.crbegin();
    auto end = d.crend();
    CHECK(it == it);
    CHECK(end == end);

    CHECK(it != end);
    CHECK_EQUAL(0x3B5, *it);
    ++it;
    CHECK(it != end);
    CHECK_EQUAL(0x3BC, *it);
    ++it;
    CHECK(it != end);
    CHECK_EQUAL(0x3C3, *it);
    ++it;
    CHECK(it != end);
    CHECK_EQUAL(0x1F79, *it);
    ++it;
    CHECK(it != end);
    CHECK_EQUAL(0x3BA, *it);
    ++it;
    CHECK(it == end);
    ++it;
    CHECK(it == end);
  }

  const UnicodeString us0(d.cbegin(), d.cend());
  CHECK_EQUAL(5, us0.size());
  CHECK_EQUAL(0x3BA, us0[0]);
  CHECK_EQUAL(0x1F79, us0[1]);
  CHECK_EQUAL(0x3C3, us0[2]);
  CHECK_EQUAL(0x3BC, us0[3]);
  CHECK_EQUAL(0x3B5, us0[4]);

  const UnicodeString us1 = UnicodeString::from_utf8(s);
  CHECK_EQUAL(us0, us1);

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


TEST(test_get_category_name) {
  CHECK_EQUAL("Cc", get_category_name(CategoryName::Cc));
  CHECK_EQUAL("Cf", get_category_name(CategoryName::Cf));
  CHECK_EQUAL("Cn", get_category_name(CategoryName::Cn));
  CHECK_EQUAL("Co", get_category_name(CategoryName::Co));
  CHECK_EQUAL("Cs", get_category_name(CategoryName::Cs));
  CHECK_EQUAL("Ll", get_category_name(CategoryName::Ll));
  CHECK_EQUAL("Lm", get_category_name(CategoryName::Lm));
  CHECK_EQUAL("Lo", get_category_name(CategoryName::Lo));
  CHECK_EQUAL("Lt", get_category_name(CategoryName::Lt));
  CHECK_EQUAL("Lu", get_category_name(CategoryName::Lu));
  CHECK_EQUAL("Mc", get_category_name(CategoryName::Mc));
  CHECK_EQUAL("Me", get_category_name(CategoryName::Me));
  CHECK_EQUAL("Mn", get_category_name(CategoryName::Mn));
  CHECK_EQUAL("Nd", get_category_name(CategoryName::Nd));
  CHECK_EQUAL("Nl", get_category_name(CategoryName::Nl));
  CHECK_EQUAL("No", get_category_name(CategoryName::No));
  CHECK_EQUAL("Pc", get_category_name(CategoryName::Pc));
  CHECK_EQUAL("Pd", get_category_name(CategoryName::Pd));
  CHECK_EQUAL("Pe", get_category_name(CategoryName::Pe));
  CHECK_EQUAL("Pf", get_category_name(CategoryName::Pf));
  CHECK_EQUAL("Pi", get_category_name(CategoryName::Pi));
  CHECK_EQUAL("Po", get_category_name(CategoryName::Po));
  CHECK_EQUAL("Ps", get_category_name(CategoryName::Ps));
  CHECK_EQUAL("Sc", get_category_name(CategoryName::Sc));
  CHECK_EQUAL("Sk", get_category_name(CategoryName::Sk));
  CHECK_EQUAL("Sm", get_category_name(CategoryName::Sm));
  CHECK_EQUAL("So", get_category_name(CategoryName::So));
  CHECK_EQUAL("Zl", get_category_name(CategoryName::Zl));
  CHECK_EQUAL("Zp", get_category_name(CategoryName::Zp));
  CHECK_EQUAL("Zs", get_category_name(CategoryName::Zs));
}


TEST(test_get_bidi_class_name) {
  CHECK_EQUAL("AL", get_bidi_class_name(BidiClassName::AL));
  CHECK_EQUAL("AN", get_bidi_class_name(BidiClassName::AN));
  CHECK_EQUAL("B", get_bidi_class_name(BidiClassName::B));
  CHECK_EQUAL("BN", get_bidi_class_name(BidiClassName::BN));
  CHECK_EQUAL("CS", get_bidi_class_name(BidiClassName::CS));
  CHECK_EQUAL("EN", get_bidi_class_name(BidiClassName::EN));
  CHECK_EQUAL("ES", get_bidi_class_name(BidiClassName::ES));
  CHECK_EQUAL("ET", get_bidi_class_name(BidiClassName::ET));
  CHECK_EQUAL("FSI", get_bidi_class_name(BidiClassName::FSI));
  CHECK_EQUAL("L", get_bidi_class_name(BidiClassName::L));
  CHECK_EQUAL("LRE", get_bidi_class_name(BidiClassName::LRE));
  CHECK_EQUAL("LRI", get_bidi_class_name(BidiClassName::LRI));
  CHECK_EQUAL("LRO", get_bidi_class_name(BidiClassName::LRO));
  CHECK_EQUAL("NSM", get_bidi_class_name(BidiClassName::NSM));
  CHECK_EQUAL("ON", get_bidi_class_name(BidiClassName::ON));
  CHECK_EQUAL("PDF", get_bidi_class_name(BidiClassName::PDF));
  CHECK_EQUAL("PDI", get_bidi_class_name(BidiClassName::PDI));
  CHECK_EQUAL("R", get_bidi_class_name(BidiClassName::R));
  CHECK_EQUAL("RLE", get_bidi_class_name(BidiClassName::RLE));
  CHECK_EQUAL("RLI", get_bidi_class_name(BidiClassName::RLI));
  CHECK_EQUAL("RLO", get_bidi_class_name(BidiClassName::RLO));
  CHECK_EQUAL("S", get_bidi_class_name(BidiClassName::S));
  CHECK_EQUAL("WS", get_bidi_class_name(BidiClassName::WS));
}


TEST(test_get_east_asian_width_name) {
  CHECK_EQUAL("A", get_east_asian_width_name(EastAsianWidthName::A));
  CHECK_EQUAL("F", get_east_asian_width_name(EastAsianWidthName::F));
  CHECK_EQUAL("H", get_east_asian_width_name(EastAsianWidthName::H));
  CHECK_EQUAL("N", get_east_asian_width_name(EastAsianWidthName::N));
  CHECK_EQUAL("Na", get_east_asian_width_name(EastAsianWidthName::Na));
  CHECK_EQUAL("W", get_east_asian_width_name(EastAsianWidthName::W));
}


TEST(test_is_alpha) {
  CHECK_EQUAL(false, is_alpha(0x0028));  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(false, is_alpha(0x002E));  // (Po) FULL STOP
  CHECK_EQUAL(false, is_alpha(0x0031));  // (Nd) DIGIT ONE
  CHECK_EQUAL(true,  is_alpha(0x0041));  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(true,  is_alpha(0x0068));  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(false, is_alpha(0x00A0));  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(false, is_alpha(0x00B9));  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(true,  is_alpha(0x00DF));  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(false, is_alpha(0x0E55));  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(true,  is_alpha(0x1F8A));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(true,  is_alpha(0x1F88));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_alpha(0x12462)); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(true,  is_alpha(0x16962)); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(true,  is_alpha(0x16F94)); // (Lm) MIAO LETTER TONE-3
}


TEST(test_is_cased) {
  CHECK_EQUAL(false, is_cased(0x0028));  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(false, is_cased(0x002E));  // (Po) FULL STOP
  CHECK_EQUAL(false, is_cased(0x0031));  // (Nd) DIGIT ONE
  CHECK_EQUAL(true,  is_cased(0x0041));  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(true,  is_cased(0x0068));  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(false, is_cased(0x00A0));  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(false, is_cased(0x00B9));  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(true,  is_cased(0x00DF));  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(false, is_cased(0x0E55));  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(true,  is_cased(0x1F88));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_cased(0x12462)); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(false, is_cased(0x16962)); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(false, is_cased(0x16F94)); // (Lm) MIAO LETTER TONE-3
  CHECK_EQUAL(true,  is_cased(0x1F8A));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
}


TEST(test_is_case_ignorable) {
  CHECK_EQUAL(false, is_case_ignorable(0x0028));  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(true,  is_case_ignorable(0x002E));  // (Po) FULL STOP
  CHECK_EQUAL(false, is_case_ignorable(0x0031));  // (Nd) DIGIT ONE
  CHECK_EQUAL(false, is_case_ignorable(0x0041));  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(false, is_case_ignorable(0x0068));  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(false, is_case_ignorable(0x00A0));  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(false, is_case_ignorable(0x00B9));  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(false, is_case_ignorable(0x00DF));  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(false, is_case_ignorable(0x0E55));  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(false, is_case_ignorable(0x1F88));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_case_ignorable(0x1F8A));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_case_ignorable(0x12462)); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(false, is_case_ignorable(0x16962)); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(true,  is_case_ignorable(0x16F94)); // (Lm) MIAO LETTER TONE-3
}


TEST(test_is_decimal) {
  CHECK_EQUAL(false, is_decimal(0x0028));  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(false, is_decimal(0x002E));  // (Po) FULL STOP
  CHECK_EQUAL(true,  is_decimal(0x0031));  // (Nd) DIGIT ONE
  CHECK_EQUAL(false, is_decimal(0x0041));  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(false, is_decimal(0x0068));  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(false, is_decimal(0x00A0));  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(false, is_decimal(0x00B9));  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(false, is_decimal(0x00DF));  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(true,  is_decimal(0x0E55));  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(false, is_decimal(0x1F88));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_decimal(0x1F8A));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_decimal(0x12462)); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(false, is_decimal(0x16962)); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(false, is_decimal(0x16F94)); // (Lm) MIAO LETTER TONE-3
}


TEST(test_get_decimal) {
  CHECK_EQUAL(1, get_decimal(0x0031));   // (Nd) DIGIT ONE
  CHECK_EQUAL(5, get_decimal(0x0E55));   // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(0, get_decimal(0x0));
}


TEST(test_is_digit) {
  CHECK_EQUAL(false, is_digit(0x0028));  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(false, is_digit(0x002E));  // (Po) FULL STOP
  CHECK_EQUAL(true,  is_digit(0x0031));  // (Nd) DIGIT ONE
  CHECK_EQUAL(false, is_digit(0x0041));  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(false, is_digit(0x0068));  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(false, is_digit(0x00A0));  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(true,  is_digit(0x00B9));  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(false, is_digit(0x00DF));  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(true,  is_digit(0x0E55));  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(false, is_digit(0x1F88));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_digit(0x1F8A));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_digit(0x12462)); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(false, is_digit(0x16962)); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(false, is_digit(0x16F94)); // (Lm) MIAO LETTER TONE-3
}


TEST(test_get_digit) {
  CHECK_EQUAL(1, get_digit(0x0031));   // (Nd) DIGIT ONE
  CHECK_EQUAL(5, get_digit(0x0E55));   // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(1, get_digit(0x00B9));   // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(0, get_digit(0x0));
}


TEST(test_is_lower) {
  CHECK_EQUAL(false, is_lower(0x0028));  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(false, is_lower(0x002E));  // (Po) FULL STOP
  CHECK_EQUAL(false, is_lower(0x0031));  // (Nd) DIGIT ONE
  CHECK_EQUAL(false, is_lower(0x0041));  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(true,  is_lower(0x0068));  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(false, is_lower(0x00A0));  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(false, is_lower(0x00B9));  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(true,  is_lower(0x00DF));  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(false, is_lower(0x0E55));  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(false, is_lower(0x1F88));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_lower(0x1F8A));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_lower(0x12462)); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(false, is_lower(0x16962)); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(false, is_lower(0x16F94)); // (Lm) MIAO LETTER TONE-3
}


TEST(test_is_numeric) {
  CHECK_EQUAL(false, is_numeric(0x0028));  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(false, is_numeric(0x002E));  // (Po) FULL STOP
  CHECK_EQUAL(true,  is_numeric(0x0031));  // (Nd) DIGIT ONE
  CHECK_EQUAL(false, is_numeric(0x0041));  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(false, is_numeric(0x0068));  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(false, is_numeric(0x00A0));  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(true,  is_numeric(0x00B9));  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(false, is_numeric(0x00DF));  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(true,  is_numeric(0x0E55));  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(false, is_numeric(0x1F88));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_numeric(0x1F8A));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(true,  is_numeric(0x12462)); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(false, is_numeric(0x16962)); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(false, is_numeric(0x16F94)); // (Lm) MIAO LETTER TONE-3
}


TEST(test_get_numeric) {
  CHECK_CLOSE(1.0, get_numeric(0x0031), 0.01);   // (Nd) DIGIT ONE
  CHECK_CLOSE(5.0, get_numeric(0x0E55), 0.01);   // (Nd) THAI DIGIT FIVE
  CHECK_CLOSE(1.0, get_numeric(0x00B9), 0.01);   // (No) SUPERSCRIPT ONE
  CHECK_CLOSE(0.25, get_numeric(0x12462), 0.01); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(0.0, get_numeric(0x0));
}


TEST(test_is_space) {
  CHECK_EQUAL(false, is_space(0x0028));  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(false, is_space(0x002E));  // (Po) FULL STOP
  CHECK_EQUAL(false, is_space(0x0031));  // (Nd) DIGIT ONE
  CHECK_EQUAL(false, is_space(0x0041));  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(false, is_space(0x0068));  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(true,  is_space(0x00A0));  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(false, is_space(0x00B9));  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(false, is_space(0x00DF));  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(false, is_space(0x0E55));  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(false, is_space(0x1F88));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_space(0x1F8A));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_space(0x12462)); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(false, is_space(0x16962)); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(false, is_space(0x16F94)); // (Lm) MIAO LETTER TONE-3
}


TEST(test_is_title) {
  CHECK_EQUAL(false, is_title(0x0028));  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(false, is_title(0x002E));  // (Po) FULL STOP
  CHECK_EQUAL(false, is_title(0x0031));  // (Nd) DIGIT ONE
  CHECK_EQUAL(false, is_title(0x0041));  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(false, is_title(0x0068));  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(false, is_title(0x00A0));  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(false, is_title(0x00B9));  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(false, is_title(0x00DF));  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(false, is_title(0x0E55));  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(true,  is_title(0x1F88));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(true,  is_title(0x1F8A));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_title(0x12462)); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(false, is_title(0x16962)); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(false, is_title(0x16F94)); // (Lm) MIAO LETTER TONE-3
}


TEST(test_is_upper) {
  CHECK_EQUAL(false, is_upper(0x0028));  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(false, is_upper(0x002E));  // (Po) FULL STOP
  CHECK_EQUAL(false, is_upper(0x0031));  // (Nd) DIGIT ONE
  CHECK_EQUAL(true,  is_upper(0x0041));  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(false, is_upper(0x0068));  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(false, is_upper(0x00A0));  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(false, is_upper(0x00B9));  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(false, is_upper(0x00DF));  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(false, is_upper(0x0E55));  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(false, is_upper(0x1F88));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_upper(0x1F8A));  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(false, is_upper(0x12462)); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(false, is_upper(0x16962)); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(false, is_upper(0x16F94)); // (Lm) MIAO LETTER TONE-3
}


TEST(test_to_lower_single_cp) {
  unicode_t code_points[3];
  size_t n;

  n = to_lower(0x0028, code_points);  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0028, code_points[0]);

  n = to_lower(0x002E, code_points);  // (Po) FULL STOP
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x002E, code_points[0]);

  n = to_lower(0x0031, code_points);  // (Nd) DIGIT ONE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0031, code_points[0]);

  n = to_lower(0x0041, code_points);  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0061, code_points[0]);

  n = to_lower(0x0068, code_points);  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0068, code_points[0]);

  n = to_lower(0x00A0, code_points);  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x00A0, code_points[0]);

  n = to_lower(0x00B9, code_points);  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x00B9, code_points[0]);

  n = to_lower(0x00DF, code_points);  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x00DF, code_points[0]);

  n = to_lower(0x0E55, code_points);  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0E55, code_points[0]);

  n = to_lower(0x1F88, code_points);  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x1F80, code_points[0]);

  n = to_lower(0x1F8A, code_points);  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x1F82, code_points[0]);

  n = to_lower(0x12462, code_points); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x12462, code_points[0]);

  n = to_lower(0x16962, code_points); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x16962, code_points[0]);

  n = to_lower(0x16F94, code_points); // (Lm) MIAO LETTER TONE-3
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x16F94, code_points[0]);
}


TEST(test_to_title_single_cp) {
  unicode_t code_points[3];
  size_t n;

  n = to_title(0x0028, code_points);  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0028, code_points[0]);

  n = to_title(0x002E, code_points);  // (Po) FULL STOP
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x002E, code_points[0]);

  n = to_title(0x0031, code_points);  // (Nd) DIGIT ONE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0031, code_points[0]);

  n = to_title(0x0041, code_points);  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0041, code_points[0]);

  n = to_title(0x0068, code_points);  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0048, code_points[0]);

  n = to_title(0x00A0, code_points);  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x00A0, code_points[0]);

  n = to_title(0x00B9, code_points);  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x00B9, code_points[0]);

  n = to_title(0x00DF, code_points);  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(2, n);
  CHECK_EQUAL(0x0053, code_points[0]);
  CHECK_EQUAL(0x0073, code_points[1]);

  n = to_title(0x0E55, code_points);  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0E55, code_points[0]);

  n = to_title(0x1F88, code_points);  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x1F88, code_points[0]);

  n = to_title(0x1F8A, code_points);  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x1F8A, code_points[0]);

  n = to_title(0x12462, code_points); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x12462, code_points[0]);

  n = to_title(0x16962, code_points); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x16962, code_points[0]);

  n = to_title(0x16F94, code_points); // (Lm) MIAO LETTER TONE-3
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x16F94, code_points[0]);
}


TEST(test_to_upper_single_cp) {
  unicode_t code_points[3];
  size_t n;

  n = to_upper(0x0028, code_points);  // (Ps) LEFT PARENTHESIS
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0028, code_points[0]);

  n = to_upper(0x002E, code_points);  // (Po) FULL STOP
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x002E, code_points[0]);

  n = to_upper(0x0031, code_points);  // (Nd) DIGIT ONE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0031, code_points[0]);

  n = to_upper(0x0041, code_points);  // (Lu) LATIN CAPITAL LETTER A
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0041, code_points[0]);

  n = to_upper(0x0068, code_points);  // (Ll) LATIN SMALL LETTER H
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0048, code_points[0]);

  n = to_upper(0x00A0, code_points);  // (Zs) NO-BREAK SPACE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x00A0, code_points[0]);

  n = to_upper(0x00B9, code_points);  // (No) SUPERSCRIPT ONE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x00B9, code_points[0]);

  n = to_upper(0x00DF, code_points);  // (Ll) LATIN SMALL LETTER SHARP S
  CHECK_EQUAL(2, n);
  CHECK_EQUAL(0x0053, code_points[0]);
  CHECK_EQUAL(0x0053, code_points[1]);

  n = to_upper(0x0E55, code_points);  // (Nd) THAI DIGIT FIVE
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x0E55, code_points[0]);

  n = to_upper(0x1F88, code_points);  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
  CHECK_EQUAL(2, n);
  CHECK_EQUAL(0x1F08, code_points[0]);
  CHECK_EQUAL(0x0399, code_points[1]);

  n = to_upper(0x1F8A, code_points);  // (Lt) GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
  CHECK_EQUAL(2, n);
  CHECK_EQUAL(0x1F0A, code_points[0]);
  CHECK_EQUAL(0x0399, code_points[1]);

  n = to_upper(0x12462, code_points); // (Nl) CUNEIFORM NUMERIC SIGN OLD ASSYRIAN ONE QUARTER
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x12462, code_points[0]);

  n = to_upper(0x16962, code_points); // (Lo) BAMUM LETTER PHASE-D NTUU
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x16962, code_points[0]);

  n = to_upper(0x16F94, code_points); // (Lm) MIAO LETTER TONE-3
  CHECK_EQUAL(1, n);
  CHECK_EQUAL(0x16F94, code_points[0]);
}


TEST(test_ctype_invalid_code_point) {
  CHECK_THROW(is_upper(MAX_CODE_POINT), UnicodeException);
}


TEST(test_get_bidi_class) {
  CHECK(BidiClassName::AL == get_bidi_class(0xFEFC));
  CHECK(BidiClassName::AN == get_bidi_class(0x10E7E));
  CHECK(BidiClassName::B == get_bidi_class(0x2029));
  CHECK(BidiClassName::BN == get_bidi_class(0xFEFF));
  CHECK(BidiClassName::CS == get_bidi_class(0xFF1A));
  CHECK(BidiClassName::EN == get_bidi_class(0xFF19));
  CHECK(BidiClassName::ES == get_bidi_class(0xFF0D));
  CHECK(BidiClassName::ET == get_bidi_class(0xFFE6));
  CHECK(BidiClassName::FSI == get_bidi_class(0x2068));
  CHECK(BidiClassName::L == get_bidi_class(0xFFFFD));
  CHECK(BidiClassName::LRE == get_bidi_class(0x202A));
  CHECK(BidiClassName::LRI == get_bidi_class(0x2066));
  CHECK(BidiClassName::LRO == get_bidi_class(0x202D));
  CHECK(BidiClassName::NSM == get_bidi_class(0xFE26));
  CHECK(BidiClassName::ON == get_bidi_class(0xFFFD));
  CHECK(BidiClassName::PDF == get_bidi_class(0x202C));
  CHECK(BidiClassName::PDI == get_bidi_class(0x2069));
  CHECK(BidiClassName::R == get_bidi_class(0xFB4F));
  CHECK(BidiClassName::RLE == get_bidi_class(0x202B));
  CHECK(BidiClassName::RLI == get_bidi_class(0x2067));
  CHECK(BidiClassName::RLO == get_bidi_class(0x202E));
  CHECK(BidiClassName::S == get_bidi_class(0x001F));
  CHECK(BidiClassName::WS == get_bidi_class(0x3000));
}


TEST(test_get_category) {
  CHECK(CategoryName::Cc == get_category(0x009F));
  CHECK(CategoryName::Cf == get_category(0xFFFB));
  CHECK(CategoryName::Co == get_category(0xFFFFD));
  CHECK(CategoryName::Cs == get_category(0xDFFF));
  CHECK(CategoryName::Ll == get_category(0xFF5A));
  CHECK(CategoryName::Lm == get_category(0xFF9F));
  CHECK(CategoryName::Lo == get_category(0xFFDC));
  CHECK(CategoryName::Lt == get_category(0x1FFC));
  CHECK(CategoryName::Lu == get_category(0xFF3A));
  CHECK(CategoryName::Mc == get_category(0xABEC));
  CHECK(CategoryName::Me == get_category(0xA672));
  CHECK(CategoryName::Mn == get_category(0xFE26));
  CHECK(CategoryName::Nd == get_category(0xFF19));
  CHECK(CategoryName::Nl == get_category(0xA6EF));
  CHECK(CategoryName::No == get_category(0xA835));
  CHECK(CategoryName::Pc == get_category(0xFF3F));
  CHECK(CategoryName::Pd == get_category(0xFF0D));
  CHECK(CategoryName::Pe == get_category(0xFF63));
  CHECK(CategoryName::Pf == get_category(0x2E21));
  CHECK(CategoryName::Pi == get_category(0x2E20));
  CHECK(CategoryName::Po == get_category(0xFF65));
  CHECK(CategoryName::Ps == get_category(0xFF62));
  CHECK(CategoryName::Sc == get_category(0xFFE6));
  CHECK(CategoryName::Sk == get_category(0xFFE3));
  CHECK(CategoryName::Sm == get_category(0xFFEC));
  CHECK(CategoryName::So == get_category(0xFFFD));
  CHECK(CategoryName::Zl == get_category(0x2028));
  CHECK(CategoryName::Zp == get_category(0x2029));
  CHECK(CategoryName::Zs == get_category(0x3000));
}


TEST(test_get_combining) {
  CHECK_EQUAL(0, get_combining(0xFFFFD));
  CHECK_EQUAL(1, get_combining(0x20EB));
  CHECK_EQUAL(7, get_combining(0xA9B3));
  CHECK_EQUAL(8, get_combining(0x309A));
  CHECK_EQUAL(9, get_combining(0xABED));
  CHECK_EQUAL(10, get_combining(0x05B0));
  CHECK_EQUAL(11, get_combining(0x05B1));
  CHECK_EQUAL(12, get_combining(0x05B2));
  CHECK_EQUAL(13, get_combining(0x05B3));
  CHECK_EQUAL(14, get_combining(0x05B4));
  CHECK_EQUAL(15, get_combining(0x05B5));
  CHECK_EQUAL(16, get_combining(0x05B6));
  CHECK_EQUAL(17, get_combining(0x05B7));
  CHECK_EQUAL(18, get_combining(0x05C7));
  CHECK_EQUAL(19, get_combining(0x05BA));
  CHECK_EQUAL(20, get_combining(0x05BB));
  CHECK_EQUAL(21, get_combining(0x05BC));
  CHECK_EQUAL(22, get_combining(0x05BD));
  CHECK_EQUAL(23, get_combining(0x05BF));
  CHECK_EQUAL(24, get_combining(0x05C1));
  CHECK_EQUAL(25, get_combining(0x05C2));
  CHECK_EQUAL(26, get_combining(0xFB1E));
  CHECK_EQUAL(27, get_combining(0x08F0));
  CHECK_EQUAL(28, get_combining(0x08F1));
  CHECK_EQUAL(29, get_combining(0x08F2));
  CHECK_EQUAL(30, get_combining(0x064E));
  CHECK_EQUAL(31, get_combining(0x064F));
  CHECK_EQUAL(32, get_combining(0x0650));
  CHECK_EQUAL(33, get_combining(0x0651));
  CHECK_EQUAL(34, get_combining(0x0652));
  CHECK_EQUAL(35, get_combining(0x0670));
  CHECK_EQUAL(36, get_combining(0x0711));
  CHECK_EQUAL(84, get_combining(0x0C55));
  CHECK_EQUAL(91, get_combining(0x0C56));
  CHECK_EQUAL(103, get_combining(0x0E39));
  CHECK_EQUAL(107, get_combining(0x0E4B));
  CHECK_EQUAL(118, get_combining(0x0EB9));
  CHECK_EQUAL(122, get_combining(0x0ECB));
  CHECK_EQUAL(129, get_combining(0x0F71));
  CHECK_EQUAL(130, get_combining(0x0F80));
  CHECK_EQUAL(132, get_combining(0x0F74));
  CHECK_EQUAL(202, get_combining(0x1DD0));
  CHECK_EQUAL(214, get_combining(0x1DCE));
  CHECK_EQUAL(216, get_combining(0x1D172));
  CHECK_EQUAL(218, get_combining(0x302A));
  CHECK_EQUAL(220, get_combining(0xAAB4));
  CHECK_EQUAL(222, get_combining(0x302D));
  CHECK_EQUAL(224, get_combining(0x302F));
  CHECK_EQUAL(226, get_combining(0x1D16D));
  CHECK_EQUAL(228, get_combining(0x302B));
  CHECK_EQUAL(230, get_combining(0xFE26));
  CHECK_EQUAL(232, get_combining(0x302C));
  CHECK_EQUAL(233, get_combining(0x1DFC));
  CHECK_EQUAL(234, get_combining(0x1DCD));
  CHECK_EQUAL(240, get_combining(0x0345));
}


TEST(test_get_east_asian_width) {
  CHECK(EastAsianWidthName::A == get_east_asian_width(0xFFFD));
  CHECK(EastAsianWidthName::F == get_east_asian_width(0xFFE6));
  CHECK(EastAsianWidthName::H == get_east_asian_width(0xFFEE));
  CHECK(EastAsianWidthName::N == get_east_asian_width(0xFFFC));
  CHECK(EastAsianWidthName::Na == get_east_asian_width(0x2986));
  CHECK(EastAsianWidthName::W == get_east_asian_width(0xFE6B));
}


TEST(test_is_mirrored) {
  CHECK_EQUAL(false, is_mirrored(0xFFFFD));
  CHECK_EQUAL(true,  is_mirrored(0xFF63));
}


TEST(test_to_lower_title_upper) {
  std::string s;
  UnicodeString expected, actual;

  s        = u8"ßüß Well-then 你好 Σ Σe eΣ oς σe σ";
  expected =  U"ßüß well-then 你好 σ σe eσ oς σe σ";  // FIXME this is incorrect as Final_Sigma isn't implemented. "ßüß well-then 你好 σ σe eς oς σe σ"
  actual = UnicodeString::from_utf8(s).to_lower();
  CHECK_EQUAL(expected, actual);
  expected =  U"Ssüß Well-Then 你好 Σ Σe Eσ Oς Σe Σ";
  actual = UnicodeString::from_utf8(s).to_title();
  CHECK_EQUAL(expected, actual);
  expected =  U"SSÜSS WELL-THEN 你好 Σ ΣE EΣ OΣ ΣE Σ";
  actual = UnicodeString::from_utf8(s).to_upper();
  CHECK_EQUAL(expected, actual);
}

}  // SUITE

}  // namespace unicode
}  // namespace schwa
