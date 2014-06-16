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

}  // namespace schwa
