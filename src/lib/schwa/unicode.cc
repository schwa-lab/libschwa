/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unicode.h>

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
// UTF8Decoder
// ============================================================================
void
UTF8Decoder::Iterator::_read(void) {
  if (_upto == nullptr)  // Are we already at the end?
    return;
  else if (_upto == _end) {  // Have we just consumed the last character?
    _upto = _end = nullptr;
    _code_point = MAX_CODE_POINT;
    return;
  }
  _code_point = read_utf8(&_upto, _end);
}

UTF8Decoder::Iterator &
UTF8Decoder::Iterator::operator =(const UTF8Decoder::Iterator &o) {
  _upto = o._upto;
  _end = o._end;
  _code_point = o._code_point;
  return *this;
}


// ============================================================================
// read_utf8
// ============================================================================
unicode_t
read_utf8(const char **ptr, const char *const end) {
  const char *const data = *ptr;
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


// ============================================================================
// write_utf8
// ============================================================================
size_t
write_utf8(const unicode_t code_point, std::ostream &out) {
  char utf8[4];
  const size_t n = write_utf8(code_point, utf8);
  out.write(utf8, n);
  return n;
}


size_t
write_utf8(const UnicodeString &s, std::ostream &out) {
  char utf8[4];
  size_t nbytes = 0;
  for (const auto c : s) {
    const size_t n = write_utf8(c, utf8);
    out.write(utf8, n);
    nbytes += n;
  }
  return nbytes;
}


size_t
write_utf8(const unicode_t code_point, char utf8[4]) {
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
