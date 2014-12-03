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
  {Encoding::BIG5, "Big-5"},
  {Encoding::EUC_JP, "EUC-JP"},
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
  {Encoding::SHIFT_JIS, "Shift_JIS"},
  {Encoding::UTF_8, "UTF-8"},
  {Encoding::WINDOWS_1250, "WINDOWS-1250"},
  {Encoding::WINDOWS_1251, "WINDOWS-1251"},
  {Encoding::WINDOWS_1252, "WINDOWS-1252"},
  {Encoding::WINDOWS_1253, "WINDOWS-1253"},
  {Encoding::WINDOWS_1254, "WINDOWS-1254"},
  {Encoding::WINDOWS_1255, "WINDOWS-1255"},
  {Encoding::WINDOWS_1256, "WINDOWS-1256"},
  {Encoding::WINDOWS_1257, "WINDOWS-1257"},
  {Encoding::WINDOWS_1258, "WINDOWS-1258"},
}};

// The strings in this hash table are normalised to be in uppercase and to not have any hyphens or
// underscores. See `get_encoding` for the normalisation process.
static const std::unordered_map<std::string, Encoding> ENCODINGS = {{
  {"ASCII", Encoding::ASCII},

  {"BIG5", Encoding::BIG5},

  {"CP1250", Encoding::WINDOWS_1250},
  {"CP1251", Encoding::WINDOWS_1251},
  {"CP1252", Encoding::WINDOWS_1252},
  {"CP1253", Encoding::WINDOWS_1253},
  {"CP1254", Encoding::WINDOWS_1254},
  {"CP1255", Encoding::WINDOWS_1255},
  {"CP1256", Encoding::WINDOWS_1256},
  {"CP1257", Encoding::WINDOWS_1257},
  {"CP1258", Encoding::WINDOWS_1258},

  {"EUCJP", Encoding::EUC_JP},

  {"GB2312", Encoding::GB2312},

  {"ISO88591", Encoding::LATIN1},
  {"ISO88592", Encoding::LATIN2},
  {"ISO88593", Encoding::LATIN3},
  {"ISO88594", Encoding::LATIN4},
  {"ISO88595", Encoding::LATIN5},
  {"ISO88596", Encoding::LATIN6},
  {"ISO88597", Encoding::LATIN7},
  {"ISO88598", Encoding::LATIN8},
  {"ISO88598I", Encoding::LATIN8},
  {"ISO88599", Encoding::LATIN9},
  {"ISO885910", Encoding::LATIN10},
  {"ISO885911", Encoding::LATIN11},
  {"ISO885913", Encoding::LATIN13},
  {"ISO885914", Encoding::LATIN14},
  {"ISO885915", Encoding::LATIN15},
  {"ISO885916", Encoding::LATIN16},
  {"ISOLATIN1", Encoding::LATIN1},

  {"KOI8R", Encoding::KOI8_R},
  {"KOI8U", Encoding::KOI8_U},

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

  {"SJIS", Encoding::SHIFT_JIS},
  {"SHIFTJIS", Encoding::SHIFT_JIS},

  {"USASCII", Encoding::ASCII},
  {"UTF8", Encoding::UTF_8},

  {"WIN1251", Encoding::WINDOWS_1251},
  {"WINDOWS1250", Encoding::WINDOWS_1250},
  {"WINDOWS1251", Encoding::WINDOWS_1251},
  {"WINDOWS1252", Encoding::WINDOWS_1252},
  {"WINDOWS1253", Encoding::WINDOWS_1253},
  {"WINDOWS1254", Encoding::WINDOWS_1254},
  {"WINDOWS1255", Encoding::WINDOWS_1255},
  {"WINDOWS1256", Encoding::WINDOWS_1256},
  {"WINDOWS1257", Encoding::WINDOWS_1257},
  {"WINDOWS1258", Encoding::WINDOWS_1258},
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


// ================================================================================================
// EncodingResult
// ================================================================================================
EncodingResult::EncodingResult(const Encoding from_encoding, const size_t buffer_grow) : _buffer(buffer_grow), _from_encoding(from_encoding) { }


void
EncodingResult::clear(const Encoding from_encoding) {
  _from_encoding = from_encoding;
  _buffer.clear();
}


size_t
EncodingResult::write(const unicode_t code_point, const uint8_t nbytes_consumed) {
  // Write the UTF-8 for the code point to the temporary working buffer.
  const uint8_t n = static_cast<uint8_t>(write_utf8(code_point, _tmp));
  for (uint8_t i = 0; i != n; ++i)
    _buffer.write(_tmp[i], (i == 0) ? nbytes_consumed : 0);

  // Return the number of UTF-8 bytes written out.
  return n;
}


// ================================================================================================
// Functions.
// ================================================================================================
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
  std::string norm;
  norm.reserve(name.size());
  for (const char c : name)
    if (c != '-' && c != '_')
      norm.push_back(std::toupper(c));

  const auto &it = ENCODINGS.find(norm);
  if (it == ENCODINGS.end())
    throw UnknownEncodingException(name);
  return it->second;
}


void
to_utf8(Encoding encoding, const char *encoded_bytes, EncodingResult &result) {
  to_utf8(encoding, reinterpret_cast<const uint8_t *>(encoded_bytes), std::strlen(encoded_bytes), result);
}


void
to_utf8(Encoding encoding, const std::string &encoded_bytes, EncodingResult &result) {
  to_utf8(encoding, reinterpret_cast<const uint8_t *>(encoded_bytes.c_str()), encoded_bytes.size(), result);
}


void
to_utf8(Encoding encoding, const uint8_t *encoded, size_t encoded_nbytes, EncodingResult &result) {
  result.reserve(encoded_nbytes);
  switch (encoding) {
  case Encoding::UTF_8: utf_8_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::ASCII: ascii_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::BIG5: big5_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::EUC_JP: euc_jp_to_utf8(encoded, encoded_nbytes, result); return;
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
  case Encoding::SHIFT_JIS: shift_jis_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::WINDOWS_1250: windows_1250_to_utf8(encoded, encoded_nbytes, result); return;
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
  result.clear(Encoding::ASCII);

  for (size_t i = 0; i != encoded_nbytes; ++i) {
    const unicode_t code_point = *encoded_bytes++;
    result.write(code_point, 1);
  }
}


void
utf_8_to_utf8(const uint8_t *const encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.clear(Encoding::UTF_8);

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
    result.write(code_point, start - old_start);
  }
}


void
big5_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.clear(Encoding::BIG5);

  for (size_t i = 0; i != encoded_nbytes; ) {
    const uint8_t c0 = *encoded_bytes++;
    ++i;

    // Is it a one-byte encoding?
    if ((c0 & 0x80) == 0) {
      // Assume ASCII. Technically could be any single-byte encoding, but is almost always ASCII.
      result.write(c0, 1);
    }
    else if (0xa1 <= c0 && c0 <= 0xf9) {
      // Consume the second byte.
      if (SCHWA_UNLIKELY(i == encoded_nbytes))
        throw DecodeException("Failed to read second byte of two-byte Big-5 sequence");
      const uint8_t c1 = *encoded_bytes++;
      ++i;

      // Construct the Unicode code point, and write out the UTF-8.
      if (SCHWA_UNLIKELY(!((0x40 <= c1 && c1 <= 0x7e) || (0xa1 <= c1 && c1 <= 0xfe))))
        throw DecodeException("Invalid second byte of two-byte Big-5 sequence");
      const unicode_t code_point = BIG5_TABLE[c0 - 0xa1][c1 - 0x40];
      result.write(code_point, 2);
    }
    else {
      // Unmapped region of the space.
      throw DecodeException("Invalid byte in Big-5 sequence");
    }
  }
}


void
euc_jp_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.clear(Encoding::EUC_JP);

  for (size_t i = 0; i != encoded_nbytes; ) {
    const uint8_t c0 = *encoded_bytes++;
    ++i;

    // How many bytes is the current encoded code point spread across?
    if ((c0 & 0x80) == 0) {
      // Single byte => ASCII.
      result.write(c0, 1);
    }
    else {
      // Consume the second byte.
      if (SCHWA_UNLIKELY(i == encoded_nbytes))
        throw DecodeException("Failed to read second byte of multi-byte EUC-JP sequence");
      const uint8_t c1 = *encoded_bytes++;
      ++i;

      if (c0 == 0x8e) {
        // Two bytes with leading 0x8e => JIS0201.
        if (SCHWA_UNLIKELY((c1 < 0xa1 || c1 > 0xdf)))
          throw DecodeException("Invalid second byte of two-byte EUC-JP sequence");

        result.write(JIS0201_TABLE[c1], 2);
      }
      else if (0xa1 <= c0 && c0 <= 0xf4) {
        // Two bytes both in the range [0xa1, 0xfe] => JIS0208.
        if (SCHWA_UNLIKELY((c1 < 0xa1 || c1 > 0xfe)))
          throw DecodeException("Invalid second byte of two-byte EUC-JP sequence");

        const unicode_t code_point = JIS0208_TABLE[c0 - 0xa1][c1 - 0xa1];
        result.write(code_point, 2);
      }
      else if (c0 == 0x8f) {
        // Three bytes with leading 0x8f => JIS0212.
        if (SCHWA_UNLIKELY(i == encoded_nbytes))
          throw DecodeException("Failed to read third byte of multi-byte EUC-JP sequence");
        const uint8_t c2 = *encoded_bytes++;
        ++i;

        if (SCHWA_UNLIKELY((c1 < 0xa2 || c1 > 0xed)))
          throw DecodeException("Invalid second byte of three-byte EUC-JP sequence");
        else if (SCHWA_UNLIKELY((c2 < 0xa1 || c2 > 0xfe)))
          throw DecodeException("Invalid third byte of three-byte EUC-JP sequence");

        const unicode_t code_point = JIS0212_TABLE[c1 - 0xa1][c2 - 0xa1];
        result.write(code_point, 3);
      }
      else {
        // Unmapped region of the space.
        throw DecodeException("Invalid byte in EUC-JP sequence");
      }
    }
  }
}


void
gb2312_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.clear(Encoding::GB2312);

  for (size_t i = 0; i != encoded_nbytes; ) {
    const uint8_t c0 = *encoded_bytes++;
    ++i;

    if ((c0 & 0x80) == 0)
      result.write(c0, 1);
    else {
      if (SCHWA_UNLIKELY(i == encoded_nbytes))
        throw DecodeException("Failed to read second byte of two-byte GB2312 sequence");
      const uint8_t c1 = *encoded_bytes++;
      ++i;
      if (SCHWA_UNLIKELY((c1 & 0x80) == 0))
        throw DecodeException("Invalid second byte of two-byte GB2312 sequence");

      const unicode_t code_point = GB2312_TABLE[c0 - GB2312_DELTA][c1 - GB2312_DELTA];
      result.write(code_point, 2);
    }
  }
}


void
shift_jis_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.clear(Encoding::SHIFT_JIS);

  for (size_t i = 0; i != encoded_nbytes; ) {
    const uint8_t c0 = *encoded_bytes++;
    ++i;

    // Is it a two-byte encoding?
    if ((0x81 <= c0 && c0 <= 0x9f) || (0xe0 <= c0 && c0 <= 0xef)) {
      // Consume the second byte.
      if (SCHWA_UNLIKELY(i == encoded_nbytes))
        throw DecodeException("Failed to read second byte of two-byte Shift_JIS sequence");
      const uint8_t c1 = *encoded_bytes++;
      ++i;

      // Convert the Shift_JIS code to its corresponding JIS0208 code.
      const uint8_t j0 = (c0 << 1) - ((c0 <= 0x9f) ? 0xe0 : 0x160) - (c1 < 0x9f ? 1 : 0);
      const uint8_t j1 = c1 - 0x1f - (c1 >= 0x7f ? 1 : 0) - (c1 >= 0x9f ? 0x5e : 0);

      if (SCHWA_UNLIKELY((j0 < 0x21) || (j0 > 0x74)))
        throw DecodeException("Invalid first byte of two-byte Shift_JIS sequence");
      else if (SCHWA_UNLIKELY((j1 < 0x21) || (j1 > 0x7e)))
        throw DecodeException("Invalid second byte of two-byte Shift_JIS sequence");

      // Construct the Unicode code point, and write out the UTF-8.
      const unicode_t code_point = JIS0208_TABLE[j0 - 0x21][j1 - 0x21];
      result.write(code_point, 2);
    }
    else if ((c0 <= 0x80) || (0xa0 <= c0 && c0 <= 0xdf) || (0xf0 <= c0)) {
      // Single byte => JIS0201.
      result.write(JIS0201_TABLE[c0], 1);
    }
    else {
      // Unmapped region of the space.
      throw DecodeException("Invalid byte in Shift_JIS sequence");
    }
  }
}


#define CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(FN_NAME, NAME, ENCODING) \
  void \
  FN_NAME ## _to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) { \
    result.clear(ENCODING); \
\
    for (size_t i = 0; i != encoded_nbytes; ++i) { \
      const unicode_t code_point = NAME ## _TABLE[*encoded_bytes - NAME ## _DELTA]; \
      result.write(code_point, 1); \
      ++encoded_bytes; \
    } \
  }

CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(koi8_r, KOI8_R, Encoding::KOI8_R)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(koi8_u, KOI8_U, Encoding::KOI8_U)

CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin1, LATIN1, Encoding::LATIN1)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin2, LATIN2, Encoding::LATIN2)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin3, LATIN3, Encoding::LATIN3)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin4, LATIN4, Encoding::LATIN4)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin5, LATIN5, Encoding::LATIN5)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin6, LATIN6, Encoding::LATIN6)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin7, LATIN7, Encoding::LATIN7)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin8, LATIN8, Encoding::LATIN8)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin9, LATIN9, Encoding::LATIN9)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin10, LATIN10, Encoding::LATIN10)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin11, LATIN11, Encoding::LATIN11)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin13, LATIN13, Encoding::LATIN13)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin14, LATIN14, Encoding::LATIN14)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin15, LATIN15, Encoding::LATIN15)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(latin16, LATIN16, Encoding::LATIN16)

CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(windows_1250, WINDOWS_1250, Encoding::WINDOWS_1250)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(windows_1251, WINDOWS_1251, Encoding::WINDOWS_1251)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(windows_1252, WINDOWS_1252, Encoding::WINDOWS_1252)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(windows_1253, WINDOWS_1253, Encoding::WINDOWS_1253)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(windows_1254, WINDOWS_1254, Encoding::WINDOWS_1254)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(windows_1255, WINDOWS_1255, Encoding::WINDOWS_1255)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(windows_1256, WINDOWS_1256, Encoding::WINDOWS_1256)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(windows_1257, WINDOWS_1257, Encoding::WINDOWS_1257)
CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION(windows_1258, WINDOWS_1258, Encoding::WINDOWS_1258)

#undef CREATE_CODE_PAGE_TABLE_TO_UTF8_FUNCTION

}
