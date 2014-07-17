#include <schwa/encoding.h>

#include <cassert>
#include <cctype>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <utility>

#include <schwa/utils/enums.h>


#include <schwa/encoding_gen.h>

namespace schwa {

static const std::map<Encoding, std::string> ENCODING_NAMES = {{
  {Encoding::ASCII, "ASCII"},
  {Encoding::GB2312, "GB2312"},
  {Encoding::LATIN1, "ISO-8859-1"},
  {Encoding::LATIN2, "ISO-8859-2"},
  {Encoding::LATIN3, "ISO-8859-3"},
  {Encoding::LATIN4, "ISO-8859-4"},
  {Encoding::LATIN5, "ISO-8859-5"},
  {Encoding::LATIN6, "ISO-8859-6"},
  {Encoding::LATIN7, "ISO-8859-7"},
  {Encoding::LATIN8, "ISO-8859-8"},
  {Encoding::LATIN9, "ISO-8859-9"},
  {Encoding::LATIN10, "ISO-8859-10"},
  {Encoding::LATIN11, "ISO-8859-11"},
  {Encoding::LATIN13, "ISO-8859-13"},
  {Encoding::LATIN14, "ISO-8859-14"},
  {Encoding::LATIN15, "ISO-8859-15"},
  {Encoding::LATIN16, "ISO-8859-16"},
  {Encoding::KOI8_R, "KOI8-R"},
  {Encoding::KOI8_U, "KOI8-U"},
  {Encoding::UTF_8, "UTF-8"},
  {Encoding::WINDOWS_1251, "WINDOWS-1251"},
  {Encoding::WINDOWS_1252, "WINDOWS-1252"},
  {Encoding::WINDOWS_1253, "WINDOWS-1253"},
  {Encoding::WINDOWS_1254, "WINDOWS-1254"},
  {Encoding::WINDOWS_1255, "WINDOWS-1255"},
  {Encoding::WINDOWS_1256, "WINDOWS-1256"},
  {Encoding::WINDOWS_1257, "WINDOWS-1257"},
  {Encoding::WINDOWS_1258, "WINDOWS-1258"},
}};

static const std::unordered_map<std::string, Encoding> ENCODINGS = {{
  {"ASCII", Encoding::ASCII},

  {"CP1251", Encoding::WINDOWS_1251},
  {"CP1252", Encoding::WINDOWS_1252},
  {"CP1253", Encoding::WINDOWS_1253},
  {"CP1254", Encoding::WINDOWS_1254},
  {"CP1255", Encoding::WINDOWS_1255},
  {"CP1256", Encoding::WINDOWS_1256},
  {"CP1257", Encoding::WINDOWS_1257},
  {"CP1258", Encoding::WINDOWS_1258},

  {"GB2312", Encoding::GB2312},

  {"ISO-8859-1", Encoding::LATIN1},
  {"ISO-8859-2", Encoding::LATIN2},
  {"ISO-8859-3", Encoding::LATIN3},
  {"ISO-8859-4", Encoding::LATIN4},
  {"ISO-8859-5", Encoding::LATIN5},
  {"ISO-8859-6", Encoding::LATIN6},
  {"ISO-8859-7", Encoding::LATIN7},
  {"ISO-8859-8", Encoding::LATIN8},
  {"ISO-8859-9", Encoding::LATIN9},
  {"ISO-8859-10", Encoding::LATIN10},
  {"ISO-8859-11", Encoding::LATIN11},
  {"ISO-8859-13", Encoding::LATIN13},
  {"ISO-8859-14", Encoding::LATIN14},
  {"ISO-8859-15", Encoding::LATIN15},
  {"ISO-8859-16", Encoding::LATIN16},

  {"ISO8859-1", Encoding::LATIN1},
  {"ISO8859-2", Encoding::LATIN2},
  {"ISO8859-3", Encoding::LATIN3},
  {"ISO8859-4", Encoding::LATIN4},
  {"ISO8859-5", Encoding::LATIN5},
  {"ISO8859-6", Encoding::LATIN6},
  {"ISO8859-7", Encoding::LATIN7},
  {"ISO8859-8", Encoding::LATIN8},
  {"ISO8859-9", Encoding::LATIN9},
  {"ISO8859-10", Encoding::LATIN10},
  {"ISO8859-11", Encoding::LATIN11},
  {"ISO8859-13", Encoding::LATIN13},
  {"ISO8859-14", Encoding::LATIN14},
  {"ISO8859-15", Encoding::LATIN15},
  {"ISO8859-16", Encoding::LATIN16},

  {"KOI8-R", Encoding::KOI8_R},
  {"KOI8-U", Encoding::KOI8_U},

  {"LATIN1", Encoding::LATIN1},
  {"LATIN2", Encoding::LATIN2},
  {"LATIN3", Encoding::LATIN3},
  {"LATIN4", Encoding::LATIN4},
  {"LATIN5", Encoding::LATIN5},
  {"LATIN6", Encoding::LATIN6},
  {"LATIN7", Encoding::LATIN7},
  {"LATIN8", Encoding::LATIN8},
  {"LATIN9", Encoding::LATIN9},
  {"LATIN10", Encoding::LATIN10},
  {"LATIN11", Encoding::LATIN11},
  {"LATIN13", Encoding::LATIN13},
  {"LATIN14", Encoding::LATIN14},
  {"LATIN15", Encoding::LATIN15},
  {"LATIN16", Encoding::LATIN16},

  {"US-ASCII", Encoding::ASCII},
  {"UTF-8", Encoding::UTF_8},
  {"UTF8", Encoding::UTF_8},

  {"WINDOWS-1251", Encoding::WINDOWS_1251},
  {"WINDOWS-1252", Encoding::WINDOWS_1252},
  {"WINDOWS-1253", Encoding::WINDOWS_1253},
  {"WINDOWS-1254", Encoding::WINDOWS_1254},
  {"WINDOWS-1255", Encoding::WINDOWS_1255},
  {"WINDOWS-1256", Encoding::WINDOWS_1256},
  {"WINDOWS-1257", Encoding::WINDOWS_1257},
  {"WINDOWS-1258", Encoding::WINDOWS_1258},
}};


UnknownEncodingException::UnknownEncodingException(const Encoding encoding) : Exception("") {
  std::ostringstream ss;
  ss << "Unhandled encoding value " << to_underlying(encoding);
  _msg = ss.str();
}


std::ostream &
operator <<(std::ostream &out, const Encoding encoding) {
  return out << encoding_name(encoding);
}


EncodingResult::EncodingResult(const Encoding from_encoding) :
    _utf8(nullptr),
    _deltas(nullptr),
    _allocated(0),
    _consumed(0),
    _from_encoding(from_encoding)
  { }

EncodingResult::~EncodingResult(void) {
  delete [] _utf8;
  delete [] _deltas;
}


void
EncodingResult::_grow(const size_t new_nbytes) {
  uint8_t *utf8 = new uint8_t[new_nbytes];
  uint8_t *deltas = new uint8_t[new_nbytes];
  if (_consumed != 0) {
    std::memcpy(utf8, _utf8, _consumed);
    std::memcpy(deltas, _deltas, _consumed);
    delete [] _utf8;
    delete [] _deltas;
  }
  _utf8 = utf8;
  _deltas = deltas;
  _allocated = new_nbytes;
}


void
EncodingResult::grow(const size_t grow_size) {
  _grow(_allocated + grow_size);
}


void
EncodingResult::reserve(const size_t nbytes) {
  if (nbytes > _allocated)
    _grow(nbytes);
}


void
EncodingResult::reset(const Encoding from_encoding) {
  _consumed = 0;
  _from_encoding = from_encoding;
}


size_t
EncodingResult::write(const unicode_t code_point, const uint8_t nbytes_consumed) {
  // Write the UTF-8 for the code point to the temporary working buffer.
  const uint8_t n = static_cast<uint8_t>(write_utf8(code_point, _tmp));

  // Grow the output buffer if required.
  if (SCHWA_UNLIKELY(remaining() < n))
    grow();

  // Copy the UTF-8 from the temporary buffer to the output buffer, and insert the delta
  // values into the delta buffer.
  std::memcpy(_utf8 + _consumed, _tmp, n);
  _deltas[_consumed] = nbytes_consumed;
  if (n != 1)
    std::memset(_deltas + _consumed + 1, 0, n - 1);
  _consumed += n;

  // Return the number of UTF-8 bytes written out.
  return n;
}


size_t
EncodingResult::write(const unicode_t code_point, const uint8_t nbytes_consumed, const uint8_t utf8_nbytes_needed) {
  // Grow the output buffer if required.
  if (SCHWA_UNLIKELY(remaining() < utf8_nbytes_needed))
    grow();

  // Write the UTF-8 directly into the output buffer, and insert the delta values into the delta
  // buffer.
  const uint8_t n = static_cast<uint8_t>(write_utf8(code_point, _utf8 + _consumed));
  _deltas[_consumed] = nbytes_consumed;
  if (n != 1)
    std::memset(_deltas + _consumed + 1, 0, n - 1);
  _consumed += n;

  // Return the number of UTF-8 bytes written out.
  return n;
}


const std::string &
encoding_name(const Encoding encoding) {
  const auto &it = ENCODING_NAMES.find(encoding);
  if (it == ENCODING_NAMES.end())
    throw UnknownEncodingException(encoding);
  return it->second;
}


inline Encoding
get_encoding(const char *const name) {
  return get_encoding(std::string(name));
}


Encoding
get_encoding(const std::string &name) {
  std::string uname;
  uname.reserve(name.size());
  for (const char c : name)
    uname.push_back(std::toupper(c));

  const auto &it = ENCODINGS.find(uname);
  if (it == ENCODINGS.end())
    throw UnknownEncodingException(name);
  return it->second;
}


inline void
to_utf8(Encoding encoding, const char *encoded_bytes, EncodingResult &result) {
  to_utf8(encoding, reinterpret_cast<const uint8_t *>(encoded_bytes), std::strlen(encoded_bytes), result);
}


inline void
to_utf8(Encoding encoding, const std::string &encoded_bytes, EncodingResult &result) {
  to_utf8(encoding, reinterpret_cast<const uint8_t *>(encoded_bytes.c_str()), encoded_bytes.size(), result);
}


void
to_utf8(Encoding encoding, const uint8_t *encoded, size_t encoded_nbytes, EncodingResult &result) {
  switch (encoding) {
  case Encoding::UTF_8: utf_8_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::ASCII: ascii_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::GB2312: gb2312_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::KOI8_R: koi8_r_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::KOI8_U: koi8_u_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN1: latin1_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN2: latin2_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN3: latin3_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN4: latin4_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN5: latin5_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN6: latin6_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN7: latin7_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN8: latin8_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN9: latin9_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN10: latin10_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN11: latin11_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN13: latin13_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN14: latin14_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN15: latin15_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN16: latin16_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::WINDOWS_1251: windows_1251_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::WINDOWS_1252: windows_1252_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::WINDOWS_1253: windows_1253_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::WINDOWS_1254: windows_1254_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::WINDOWS_1255: windows_1255_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::WINDOWS_1256: windows_1256_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::WINDOWS_1257: windows_1257_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::WINDOWS_1258: windows_1258_to_utf8(encoded, encoded_nbytes, result); return;
  default:
    throw UnknownEncodingException(encoding);
  }
}


void
ascii_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.reset(Encoding::ASCII);

  for (size_t i = 0; i != encoded_nbytes; ++i) {
    const unicode_t code_point = *encoded_bytes++;
    result.write(code_point, 1, 1);
  }
}


void
utf_8_to_utf8(const uint8_t *const encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.reset(Encoding::UTF_8);

  unicode_t code_point;
  const uint8_t *start = encoded_bytes, *old_start;
  const uint8_t *end = start + encoded_nbytes;
  while (start != end) {
    old_start = start;
    try {
      code_point = read_utf8(&start, end);
    }
    catch (UnicodeException &e) {
      throw DecodeException(e.msg());
    }
    result.write(code_point, start - old_start, start - old_start);
  }
}


void
gb2312_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.reset(Encoding::GB2312);

  for (size_t i = 0; i != encoded_nbytes; ) {
    const uint8_t c0 = *encoded_bytes++;
    ++i;

    if ((c0 & 0x80) == 0)
      result.write(c0, 1, 1);
    else {
      if (SCHWA_UNLIKELY(i == encoded_nbytes))
        throw DecodeException("Failed to read second byte of two-byte GB2312 sequence");
      const uint8_t c1 = *encoded_bytes++;
      ++i;
      if (SCHWA_UNLIKELY((c1 & 0x80) == 0))
        throw DecodeException("Invalid second byte of two-byte GB2312 sequence");

      const unicode_t code_point = GB2312_TABLE[c0 - GB2312_DELTA][c1 - GB2312_DELTA];
      result.write(code_point, 2, GB2312_UTF8_NBYTES);
    }
  }
}


#define CREATE_TABLE_TO_UTF8_FUNCTION(FN_NAME, NAME, ENCODING) \
  void \
  FN_NAME ## _to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) { \
    result.reset(ENCODING); \
\
    for (size_t i = 0; i != encoded_nbytes; ++i) { \
      const unicode_t code_point = NAME ## _TABLE[*encoded_bytes - NAME ## _DELTA]; \
      result.write(code_point, 1, NAME ## _UTF8_NBYTES); \
      ++encoded_bytes; \
    } \
  }

CREATE_TABLE_TO_UTF8_FUNCTION(koi8_r, KOI8_R, Encoding::KOI8_R)
CREATE_TABLE_TO_UTF8_FUNCTION(koi8_u, KOI8_U, Encoding::KOI8_U)

CREATE_TABLE_TO_UTF8_FUNCTION(latin1, LATIN1, Encoding::LATIN1)
CREATE_TABLE_TO_UTF8_FUNCTION(latin2, LATIN2, Encoding::LATIN2)
CREATE_TABLE_TO_UTF8_FUNCTION(latin3, LATIN3, Encoding::LATIN3)
CREATE_TABLE_TO_UTF8_FUNCTION(latin4, LATIN4, Encoding::LATIN4)
CREATE_TABLE_TO_UTF8_FUNCTION(latin5, LATIN5, Encoding::LATIN5)
CREATE_TABLE_TO_UTF8_FUNCTION(latin6, LATIN6, Encoding::LATIN6)
CREATE_TABLE_TO_UTF8_FUNCTION(latin7, LATIN7, Encoding::LATIN7)
CREATE_TABLE_TO_UTF8_FUNCTION(latin8, LATIN8, Encoding::LATIN8)
CREATE_TABLE_TO_UTF8_FUNCTION(latin9, LATIN9, Encoding::LATIN9)
CREATE_TABLE_TO_UTF8_FUNCTION(latin10, LATIN10, Encoding::LATIN10)
CREATE_TABLE_TO_UTF8_FUNCTION(latin11, LATIN11, Encoding::LATIN11)
CREATE_TABLE_TO_UTF8_FUNCTION(latin13, LATIN13, Encoding::LATIN13)
CREATE_TABLE_TO_UTF8_FUNCTION(latin14, LATIN14, Encoding::LATIN14)
CREATE_TABLE_TO_UTF8_FUNCTION(latin15, LATIN15, Encoding::LATIN15)
CREATE_TABLE_TO_UTF8_FUNCTION(latin16, LATIN16, Encoding::LATIN16)

CREATE_TABLE_TO_UTF8_FUNCTION(windows_1251, WINDOWS_1251, Encoding::WINDOWS_1251)
CREATE_TABLE_TO_UTF8_FUNCTION(windows_1252, WINDOWS_1252, Encoding::WINDOWS_1252)
CREATE_TABLE_TO_UTF8_FUNCTION(windows_1253, WINDOWS_1253, Encoding::WINDOWS_1253)
CREATE_TABLE_TO_UTF8_FUNCTION(windows_1254, WINDOWS_1254, Encoding::WINDOWS_1254)
CREATE_TABLE_TO_UTF8_FUNCTION(windows_1255, WINDOWS_1255, Encoding::WINDOWS_1255)
CREATE_TABLE_TO_UTF8_FUNCTION(windows_1256, WINDOWS_1256, Encoding::WINDOWS_1256)
CREATE_TABLE_TO_UTF8_FUNCTION(windows_1257, WINDOWS_1257, Encoding::WINDOWS_1257)
CREATE_TABLE_TO_UTF8_FUNCTION(windows_1258, WINDOWS_1258, Encoding::WINDOWS_1258)


#undef CREATE_TABLE_TO_UTF8_FUNCTION

}
