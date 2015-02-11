/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unicode.h>

#include <cstring>
#include <sstream>


namespace schwa {

// ============================================================================
// UnicodeException
// ============================================================================
UnicodeException::UnicodeException(const std::string &msg, const unicode_t code_point) : Exception(""), _code_point(code_point) {
  std::ostringstream ss;
  ss << msg << "(0x" << std::hex << code_point << ")";
  _msg = ss.str();
}


// ============================================================================
// UnicodeString
// ============================================================================
UnicodeString::UnicodeString(const std::string &utf8, const allocator_type &alloc) {
  UTF8Decoder d(utf8);
  _str = underlying_type(d.begin(), d.end(), alloc);
}


UnicodeString::UnicodeString(const uint8_t *const utf8, const allocator_type &alloc) {
  UTF8Decoder d(utf8, std::strlen(reinterpret_cast<const char *>(utf8)));
  _str = underlying_type(d.begin(), d.end(), alloc);
}


UnicodeString &
UnicodeString::operator +=(const char *const utf8) {
  UTF8Decoder d(utf8, std::strlen(utf8));
  for (const auto c : d)
    push_back(c);
  return *this;
}


UnicodeString &
UnicodeString::operator +=(const uint8_t *const utf8) {
  UTF8Decoder d(utf8, std::strlen(reinterpret_cast<const char *>(utf8)));
  for (const auto c : d)
    push_back(c);
  return *this;
}


UnicodeString &
UnicodeString::operator +=(const char c) {
  push_back(c);
  return *this;
}


UnicodeString &
UnicodeString::operator +=(const uint8_t c) {
  push_back(c);
  return *this;
}


std::string
UnicodeString::to_utf8(void) const {
  uint8_t utf8[4];
  std::string out;
  out.reserve(size());
  for (const auto c : *this) {
    const size_t n = write_utf8(c, utf8);
    out.append(reinterpret_cast<char *>(utf8), n);
  }
  return out;
}


UnicodeString
UnicodeString::to_lower(void) const {
  unicode_t code_points[3];
  UnicodeString ret;
  ret.reserve(size());
  for (const auto c : *this) {
    const size_t n = unicode::to_lower(c, code_points);  // TODO account for Final_Sigma.
    ret.append(code_points, n);
  }
  return ret;
}


UnicodeString
UnicodeString::to_title(void) const {
  unicode_t code_points[3];
  UnicodeString ret;
  ret.reserve(size());

  bool prev_is_cased = false;
  for (const auto c : *this) {
    const size_t n = prev_is_cased ? unicode::to_lower(c, code_points) : unicode::to_title(c, code_points);
    ret.append(code_points, n);
    prev_is_cased = unicode::is_cased(c);
  }
  return ret;
}


UnicodeString
UnicodeString::to_upper(void) const {
  unicode_t code_points[3];
  UnicodeString ret;
  ret.reserve(size());
  for (const auto c : *this) {
    const size_t n = unicode::to_upper(c, code_points);
    ret.append(code_points, n);
  }
  return ret;
}


template <typename F>
static bool
unicodestring_ctype(const UnicodeString &string, F predicate) {
  for (const auto c : string)
    if (!predicate(c))
      return false;
  return true;
}

bool UnicodeString::is_alnum(void) const { return unicodestring_ctype(*this, unicode::is_alnum); }
bool UnicodeString::is_alpha(void) const { return unicodestring_ctype(*this, unicode::is_alpha); }
bool UnicodeString::is_cased(void) const { return unicodestring_ctype(*this, unicode::is_cased); }
bool UnicodeString::is_case_ignorable(void) const { return unicodestring_ctype(*this, unicode::is_case_ignorable); }
bool UnicodeString::is_decimal(void) const { return unicodestring_ctype(*this, unicode::is_decimal); }
bool UnicodeString::is_digit(void) const { return unicodestring_ctype(*this, unicode::is_digit); }
bool UnicodeString::is_lower(void) const { return unicodestring_ctype(*this, unicode::is_lower); }
bool UnicodeString::is_numeric(void) const { return unicodestring_ctype(*this, unicode::is_numeric); }
bool UnicodeString::is_printable(void) const { return unicodestring_ctype(*this, unicode::is_printable); }
bool UnicodeString::is_space(void) const { return unicodestring_ctype(*this, unicode::is_space); }
bool UnicodeString::is_upper(void) const { return unicodestring_ctype(*this, unicode::is_upper); }


// ============================================================================
// read_utf8
// ============================================================================
unicode_t
read_utf8(const uint8_t **ptr, const uint8_t *const end) {
  const uint8_t *const data = *ptr;
  const size_t remaining = end - data;
  if (remaining == 0)
    throw ValueException("No data left to read.");

  unicode_t code_point;
  if (remaining >= 1 && ((data[0] & 0x80) == 0)) {
    code_point = (data[0] & 0x7F);
    *ptr += 1;
  }
  else if (remaining >= 2 && ((data[0] & 0xE0) == 0xC0) && ((data[1] & 0xC0) == 0x80)) {
    code_point =  ((data[0] & 0x1F) << 6);
    code_point |= ((data[1] & 0x3F) << 0);
    *ptr += 2;
  }
  else if (remaining >= 3 && ((data[0] & 0xF0) == 0xE0) && ((data[1] & 0xC0) == 0x80) && ((data[2] & 0xC0) == 0x80)) {
    code_point =  ((data[0] & 0x0F) << 12);
    code_point |= ((data[1] & 0x3F) <<  6);
    code_point |= ((data[2] & 0x3F) <<  0);
    *ptr += 3;
  }
  else if (remaining >= 4 && ((data[0] & 0xF8) == 0xF0) && ((data[1] & 0xC0) == 0x80) && ((data[2] & 0xC0) == 0x80) && ((data[3] & 0xC0) == 0x80)) {
    code_point =  ((data[0] & 0x07) << 18);
    code_point |= ((data[1] & 0x3F) << 12);
    code_point |= ((data[2] & 0x3F) <<  6);
    code_point |= ((data[3] & 0x3F) <<  0);
    *ptr += 4;
  }
  else
    throw UnicodeException("Could not decode UTF-8 sequence", MAX_CODE_POINT);
  return code_point;
}


size_t
read_utf8(const uint8_t **data, const uint8_t *end, uint8_t utf8[4]) {
  const unicode_t code_point = read_utf8(data, end);
  return write_utf8(code_point, utf8);
}


unicode_t
read_utf8_backwards(const uint8_t **ptr, const uint8_t *const end) {
  const uint8_t *const data = *ptr;
  const size_t remaining = data - end;
  if (remaining == 0)
    throw ValueException("No data left to read.");

  unicode_t code_point;
  if (remaining >= 1 && ((data[-1] & 0x80) == 0)) {
    code_point = (data[-1] & 0x7F);
    *ptr -= 1;
  }
  else if (remaining >= 2 && ((data[-2] & 0xE0) == 0xC0) && ((data[-1] & 0xC0) == 0x80)) {
    code_point =  ((data[-2] & 0x1F) << 6);
    code_point |= ((data[-1] & 0x3F) << 0);
    *ptr -= 2;
  }
  else if (remaining >= 3 && ((data[-3] & 0xF0) == 0xE0) && ((data[-2] & 0xC0) == 0x80) && ((data[-1] & 0xC0) == 0x80)) {
    code_point =  ((data[-3] & 0x0F) << 12);
    code_point |= ((data[-2] & 0x3F) <<  6);
    code_point |= ((data[-1] & 0x3F) <<  0);
    *ptr -= 3;
  }
  else if (remaining >= 4 && ((data[-4] & 0xF8) == 0xF0) && ((data[-3] & 0xC0) == 0x80) && ((data[-2] & 0xC0) == 0x80) && ((data[-1] & 0xC0) == 0x80)) {
    code_point =  ((data[-4] & 0x07) << 18);
    code_point |= ((data[-3] & 0x3F) << 12);
    code_point |= ((data[-2] & 0x3F) <<  6);
    code_point |= ((data[-1] & 0x3F) <<  0);
    *ptr -= 4;
  }
  else
    throw UnicodeException("Could not decode UTF-8 sequence", MAX_CODE_POINT);
  return code_point;
}


// ============================================================================
// write_utf8
// ============================================================================
size_t
write_utf8(const unicode_t code_point, std::ostream &out) {
  uint8_t utf8[4];
  const size_t n = write_utf8(code_point, utf8);
  out.write(reinterpret_cast<char *>(utf8), n);
  return n;
}


size_t
write_utf8(const UnicodeString &s, std::ostream &out) {
  uint8_t utf8[4];
  size_t nbytes = 0;
  for (const auto c : s) {
    const size_t n = write_utf8(c, utf8);
    out.write(reinterpret_cast<char *>(utf8), n);
    nbytes += n;
  }
  return nbytes;
}


size_t
write_utf8(const unicode_t code_point, uint8_t utf8[4]) {
  if (code_point <= 0x007F) {
    utf8[0] = (code_point & 0x7F);
    return 1;
  }
  else if (code_point <= 0x07FF) {
    utf8[0] = (0xC0 | ((code_point >>  6) & 0x1F));
    utf8[1] = (0x80 | ((code_point >>  0) & 0x3F));
    return 2;
  }
  else if (code_point <= 0xFFFF) {
    utf8[0] = (0xE0 | ((code_point >> 12) & 0x0F));
    utf8[1] = (0x80 | ((code_point >>  6) & 0x3F));
    utf8[2] = (0x80 | ((code_point >>  0) & 0x3F));
    return 3;
  }
  else if (code_point <= 0x1FFFFF) {
    utf8[0] = (0xF0 | ((code_point >> 18) & 0x07));
    utf8[1] = (0x80 | ((code_point >> 12) & 0x3F));
    utf8[2] = (0x80 | ((code_point >>  6) & 0x3F));
    utf8[3] = (0x80 | ((code_point >>  0) & 0x3F));
    return 4;
  }
  else
    throw UnicodeException("Could not encode UTF-8 sequence", code_point);
}


// ============================================================================
// These are all taken from src/ragel/tokenizer/rules/punctuation.rl which are
// defined for the purpose of tokenization.
// ============================================================================
namespace unicode {

bool
is_bracket(const unicode_t code_point) {
  switch (code_point) {
  case 0x0028:  // U+0028 left parenthesis (()
  case 0x0029:  // U+0029 right parenthesis ())
  case 0x005b:  // U+005b left square bracket ([)
  case 0x005d:  // U+005d right square bracket (])
  case 0x007b:  // U+007b left curly bracket ({)
  case 0x007d:  // U+007d right curly bracket (})
    return true;
  default:
    return false;
  }
}


bool
is_colon(const unicode_t code_point) {
  switch (code_point) {
  case 0x003a:  // U+003a colon (:)
  case 0x02d0:  // U+02d0 modifier letter triangular colon (ː)
  case 0xff1a:  // U+ff1a fullwidth colon (：)
    return true;
  default:
    return false;
  }
}


bool
is_comma(const unicode_t code_point) {
  switch (code_point) {
  case 0x002c:  // U+002c comma (,)
  case 0x02bb:  // U+02bb modifier letter turned comma (ʻ)
  case 0x02bd:  // U+02bd modifier letter reversed comma (ʽ)
  case 0x0312:  // U+0312 combining turned comma above (̒)
  case 0x0313:  // U+0313 combining comma above (̓)
  case 0x0314:  // U+0314 combining reversed comma above (̔)
  case 0x0315:  // U+0315 combining comma above right (̕)
  case 0x0326:  // U+0326 combining comma below (̦)
  case 0x055d:  // U+055d armenian comma (՝)
  case 0x060c:  // U+060c arabic comma (،)
  case 0x07f8:  // U+07f8 nko comma (߸)
  case 0x1363:  // U+1363 ethiopic comma (፣)
  case 0x1802:  // U+1802 mongolian comma (᠂)
  case 0x1808:  // U+1808 mongolian manchu comma (᠈)
  case 0x3001:  // U+3001 ideographic comma (、)
  case 0xa4fe:  // U+a4fe lisu punctuation comma (꓾)
  case 0xa60d:  // U+a60d vai comma (꘍)
  case 0xa6f5:  // U+a6f5 bamum comma (꛵)
  case 0xfe10:  // U+fe10 presentation form for vertical comma (︐)
  case 0xfe11:  // U+fe11 presentation form for vertical ideographic comma (︑)
  case 0xfe50:  // U+fe50 small comma (﹐)
  case 0xfe51:  // U+fe51 small ideographic comma (﹑)
  case 0xff0c:  // U+ff0c fullwidth comma (，)
  case 0xff64:  // U+ff64 halfwidth ideographic comma (､)
    return true;
  default:
    return false;
  }
}


bool
is_dash(const unicode_t code_point) {
  switch (code_point) {
  case 0x2012:  // U+2012 figure dash (‒)
  case 0x2013:  // U+2013 en dash (–)
  case 0x2014:  // U+2014 em dash (—)
  case 0x2015:  // U+2015 horizontal bar (―)
  case 0x2053:  // U+2053 swung dash (⁓)
  case 0x2e3a:  // U+2e3a two-em dash (⸺)
  case 0x2e3b:  // U+2e3b three-em dash (⸻)
    return true;
  default:
    return false;
  }
}


bool
is_ellipsis(const unicode_t code_point) {
  switch (code_point) {
  case 0x0e2f:  // U+0e2f thai character paiyannoi (ฯ)
  case 0x0eaf:  // U+0eaf lao ellipsis (ຯ)
  case 0x1801:  // U+1801 mongolian ellipsis (᠁)
  case 0x2026:  // U+2026 horizontal ellipsis (…)
    return true;
  default:
    return false;
  }
}


bool
is_exclamation_mark(const unicode_t code_point) {
  switch (code_point) {
  case 0x055c:  // U+055c armenian exclamation mark (՜)
  case 0x07f9:  // U+07f9 nko exclamation mark (߹)
  case 0x1944:  // U+1944 limbu exclamation mark (᥄)
  case 0x203c:  // U+203c double exclamation mark (‼)
  case 0xfe15:  // U+fe15 presentation form for vertical exclamation mark (︕)
  case 0xfe57:  // U+fe57 small exclamation mark (﹗)
  case 0xff01:  // U+ff01 fullwidth exclamation mark (！)
    return true;
  default:
    return false;
  }
}


bool
is_full_stop(const unicode_t code_point) {
  switch (code_point) {
  case 0x2024:  // U+2024 one dot leader (․)
  case 0xfe52:  // U+fe52 small full stop (﹒)
  case 0xff0e:  // U+ff0e fullwidth full stop (．)
    return true;
  default:
    return false;
  }
}


bool
is_hyphen(const unicode_t code_point) {
  switch (code_point) {
  case 0x002d:  // U+002d hyphen-minus (-)
  case 0x00ad:  // U+00ad soft hyphen (­)
  case 0x00b7:  // U+00b7 middle dot (·)
  case 0x058a:  // U+058a armenian hyphen (֊)
  case 0x1400:  // U+1400 canadian syllabics hyphen (᐀)
  case 0x1806:  // U+1806 mongolian todo soft hyphen (᠆)
  case 0x2010:  // U+2010 hyphen (‐)
  case 0x2011:  // U+2011 non-breaking hyphen (‑)
  case 0x2027:  // U+2027 hyphenation point (‧)
  case 0x2043:  // U+2043 hyphen bullet (⁃)
  case 0x2212:  // U+2212 minus sign (−)
    return true;
  default:
    return false;
  }
}


bool
is_inverted_exclamation_mark(const unicode_t code_point) {
  switch (code_point) {
  case 0x00a1:  // U+00a1 inverted exclamation mark (¡)
    return true;
  default:
    return false;
  }
}


bool
is_inverted_question_mark(const unicode_t code_point) {
  switch (code_point) {
  case 0x00a1:  // U+00a1 inverted exclamation mark (¡)
  case 0x00bf:  // U+00bf inverted question mark (¿)
  case 0x2e18:  // U+2e18 inverted interrobang (⸘)
    return true;
  default:
    return false;
  }
}


bool
is_question_mark(const unicode_t code_point) {
  switch (code_point) {
  case 0x037e:  // U+037e greek question mark (;)
  case 0x055e:  // U+055e armenian question mark (՞)
  case 0x061f:  // U+061f arabic question mark (؟)
  case 0x1367:  // U+1367 ethiopic question mark (፧)
  case 0x1945:  // U+1945 limbu question mark (᥅)
  case 0x203d:  // U+203d interrobang (‽)
  case 0x2047:  // U+2047 double question mark (⁇)
  case 0x2048:  // U+2048 question exclamation mark (⁈)
  case 0x2049:  // U+2049 exclamation question mark (⁉)
  case 0x2cfa:  // U+2cfa coptic old nubian direct question mark (⳺)
  case 0x2cfb:  // U+2cfb coptic old nubian indirect question mark (⳻)
  case 0x2e2e:  // U+2e2e reversed question mark (⸮)
  case 0xa60f:  // U+a60f vai question mark (꘏)
  case 0xa6f7:  // U+a6f7 bamum question mark (꛷)
  case 0xfe16:  // U+fe16 presentation form for vertical question mark (︖)
  case 0xfe56:  // U+fe56 small question mark (﹖)
  case 0xff1f:  // U+ff1f fullwidth question mark (？)
    return true;
  default:
    return false;
  }
}


bool
is_semicolon(const unicode_t code_point) {
  switch (code_point) {
  case 0x003b:  // U+003b semicolon (;)
  case 0x061b:  // U+061b arabic semicolon (؛)
  case 0x1364:  // U+1364 ethiopic semicolon (፤)
  case 0x204f:  // U+204f reversed semicolon (⁏)
  case 0x236e:  // U+236e apl functional symbol semicolon underbar (⍮)
  case 0x2e35:  // U+2e35 turned semicolon (⸵)
  case 0xa6f6:  // U+a6f6 bamum semicolon (꛶)
  case 0xfe14:  // U+fe14 presentation form for vertical semicolon (︔)
  case 0xfe54:  // U+fe54 small semicolon (﹔)
  case 0xff1b:  // U+ff1b fullwidth semicolon (；)
  case 0xe003b:  // U+e003b tag semicolon (󠀻)
    return true;
  default:
    return false;
  }
}

}  // namespace unicode
}  // namespace schwa
