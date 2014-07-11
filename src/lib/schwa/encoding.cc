#include <schwa/encoding.h>

#include <array>
#include <cassert>
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include <utility>

#include <schwa/unicode.h>
#include <schwa/utils/enums.h>


#include <schwa/encoding_gen.h>

namespace schwa {

// Canonical names appear before aliases for the same encoding so that the encoding_name function
// returns the canonical name. UTF-8 appears first in this list as it will be the most frequently
// requested encoding.
static const std::array<std::pair<std::string, Encoding>, 14> ENCODINGS = {{
  {"UTF-8", Encoding::UTF_8},
  {"UTF8", Encoding::UTF_8},
  {"ASCII", Encoding::ASCII},
  {"GB2312", Encoding::GB2312},
  {"LATIN1", Encoding::LATIN1},
  {"ISO-8859-1", Encoding::LATIN1},
  {"ISO8859-1", Encoding::LATIN1},
  {"LATIN2", Encoding::LATIN2},
  {"ISO-8859-2", Encoding::LATIN2},
  {"ISO8859-2", Encoding::LATIN2},
  {"CP1251", Encoding::CP1251},
  {"WINDOWS-1251", Encoding::CP1251},
  {"CP1252", Encoding::CP1252},
  {"WINDOWS-1252", Encoding::CP1252},
}};


UnknownEncodingException::UnknownEncodingException(const Encoding encoding) : Exception("") {
  std::ostringstream ss;
  ss << "Unhandled encoding value " << to_underlying(encoding);
  _msg = ss.str();
}


inline std::ostream &
operator <<(std::ostream &out, const Encoding encoding) {
  return out << encoding_name(encoding);
}


const std::string &
encoding_name(const Encoding encoding) {
  for (const auto &pair : ENCODINGS)
    if (pair.second == encoding)
      return pair.first;
  throw UnknownEncodingException(encoding);
}


inline Encoding
get_encoding(const char *const name) {
  return get_encoding(std::string(name));
}


Encoding
get_encoding(const std::string &name) {
  size_t i;
  for (const auto &pair : ENCODINGS) {
    if (pair.first.size() != name.size())
      continue;
    for (i = 0; i != name.size(); ++i)
      if (std::toupper(name[i]) != pair.first[i])
        break;
    if (i == name.size())
      return pair.second;
  }
  throw UnknownEncodingException(name);
};


inline size_t
to_utf8(Encoding encoding, const char *encoded_bytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes) {
  return to_utf8(encoding, encoded_bytes, std::strlen(encoded_bytes), utf8_bytes, utf8_nbytes);
}


inline size_t
to_utf8(Encoding encoding, const std::string &encoded_bytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes) {
  return to_utf8(encoding, encoded_bytes.c_str(), encoded_bytes.size(), utf8_bytes, utf8_nbytes);
}


size_t
to_utf8(Encoding encoding, const char *encoded_bytes, size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes) {
  const uint8_t *encoded = reinterpret_cast<const uint8_t *>(encoded_bytes);
  switch (encoding) {
  case Encoding::UTF_8: return utf_8_to_utf8(encoded, encoded_nbytes, utf8_bytes, utf8_nbytes);
  case Encoding::ASCII: return ascii_to_utf8(encoded, encoded_nbytes, utf8_bytes, utf8_nbytes);
  case Encoding::CP1251: return cp1251_to_utf8(encoded, encoded_nbytes, utf8_bytes, utf8_nbytes);
  case Encoding::CP1252: return cp1252_to_utf8(encoded, encoded_nbytes, utf8_bytes, utf8_nbytes);
  case Encoding::GB2312: return gb2312_to_utf8(encoded, encoded_nbytes, utf8_bytes, utf8_nbytes);
  case Encoding::LATIN1: return latin1_to_utf8(encoded, encoded_nbytes, utf8_bytes, utf8_nbytes);
  case Encoding::LATIN2: return latin2_to_utf8(encoded, encoded_nbytes, utf8_bytes, utf8_nbytes);
  default:
    throw UnknownEncodingException(encoding);
  }
}


static void
_grow_output_buffer(uint8_t *&utf8_bytes, size_t &utf8_nbytes, uint8_t *&output, uint8_t *&output_end, const size_t grow_size=4096) {
  delete [] utf8_bytes;
  utf8_nbytes += grow_size;
  utf8_bytes = new uint8_t[utf8_nbytes];
  output = utf8_bytes + (utf8_nbytes - grow_size);
  output_end = utf8_bytes + utf8_nbytes;
}


size_t
ascii_to_utf8(const uint8_t *const encoded_bytes, const size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes) {
  if (utf8_nbytes < encoded_nbytes) {
    delete [] utf8_bytes;
    utf8_nbytes = encoded_nbytes;
    utf8_bytes = new uint8_t[utf8_nbytes];
  }
  std::memcpy(utf8_bytes, encoded_bytes, encoded_nbytes);
  return encoded_nbytes;
}


size_t
cp1251_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes) {
  uint8_t *output = utf8_bytes, *output_end = utf8_bytes + utf8_nbytes;
  for (size_t i = 0; i != encoded_nbytes; ++i) {
    if (SCHWA_UNLIKELY(output_end - output < CP1251_UTF8_NBYTES))
      _grow_output_buffer(utf8_bytes, utf8_nbytes, output, output_end);

    // Work out the Unicode code point for the current encoded input character.
    const unicode_t code_point = CP1251_TABLE[*encoded_bytes - CP1251_DELTA];

    // Write out the UTF-8.
    const size_t n = write_utf8(code_point, output);
    assert(0 < n && n <= CP1251_UTF8_NBYTES);  // FIXME remove this after unit tests
    output += n;
    ++encoded_bytes;
  }
  return output - utf8_bytes;
}


size_t
cp1252_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes) {
  uint8_t *output = utf8_bytes, *output_end = utf8_bytes + utf8_nbytes;
  for (size_t i = 0; i != encoded_nbytes; ++i) {
    if (SCHWA_UNLIKELY(output_end - output < CP1252_UTF8_NBYTES))
      _grow_output_buffer(utf8_bytes, utf8_nbytes, output, output_end);

    // Work out the Unicode code point for the current encoded input character.
    const unicode_t code_point = CP1252_TABLE[*encoded_bytes - CP1252_DELTA];

    // Write out the UTF-8.
    const size_t n = write_utf8(code_point, output);
    assert(0 < n && n <= CP1252_UTF8_NBYTES);  // FIXME remove this after unit tests
    output += n;
    ++encoded_bytes;
  }
  return output - utf8_bytes;
}


size_t
gb2312_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes) {
  uint8_t *output = utf8_bytes, *output_end = utf8_bytes + utf8_nbytes;
  for (size_t i = 0; i != encoded_nbytes; ) {
    const uint8_t c0 = *encoded_bytes;
    if ((c0 & 0x80) == 0) {
      if (SCHWA_UNLIKELY(output_end - output == 0))
        _grow_output_buffer(utf8_bytes, utf8_nbytes, output, output_end);
      *output++ = c0;
      ++encoded_bytes;
      ++i;
    }
    else {
      if (SCHWA_UNLIKELY(output_end - output < GB2312_UTF8_NBYTES))
        _grow_output_buffer(utf8_bytes, utf8_nbytes, output, output_end);
      ++encoded_bytes;
      ++i;

      if (SCHWA_UNLIKELY(i == encoded_nbytes))
        throw DecodeException("Failed to read second byte of two-byte GB2312 sequence");
      const uint8_t c1 = *encoded_bytes;
      ++encoded_bytes;
      ++i;
      if (SCHWA_UNLIKELY((c1 & 0x80) == 0))
        throw DecodeException("Invalid second byte of two-byte GB2312 sequence");

      // Work out the Unicode code point for the current encoded input character.
      const unicode_t code_point = GB2312_TABLE[c0 - GB2312_DELTA][c1 - GB2312_DELTA];

      // Write out the UTF-8.
      const size_t n = write_utf8(code_point, output);
      assert(0 < n && n <= GB2312_UTF8_NBYTES);  // FIXME remove this after unit tests
      output += n;
    }
  }
  return output - utf8_bytes;
}


size_t
latin1_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes) {
  uint8_t *output = utf8_bytes, *output_end = utf8_bytes + utf8_nbytes;
  for (size_t i = 0; i != encoded_nbytes; ++i) {
    if (SCHWA_UNLIKELY(output_end - output < LATIN1_UTF8_NBYTES))
      _grow_output_buffer(utf8_bytes, utf8_nbytes, output, output_end);

    // Work out the Unicode code point for the current encoded input character.
    const unicode_t code_point = LATIN1_TABLE[*encoded_bytes - LATIN1_DELTA];

    // Write out the UTF-8.
    const size_t n = write_utf8(code_point, output);
    assert(0 < n && n <= LATIN1_UTF8_NBYTES);  // FIXME remove this after unit tests
    output += n;
    ++encoded_bytes;
  }
  return output - utf8_bytes;
}


size_t
latin2_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes) {
  uint8_t *output = utf8_bytes, *output_end = utf8_bytes + utf8_nbytes;
  for (size_t i = 0; i != encoded_nbytes; ++i) {
    // Grow the output buffer if needed. ISO-8859-1 will only ever write up to 2 UTF-8 bytes of output.
    if (SCHWA_UNLIKELY(output_end - output < LATIN2_UTF8_NBYTES))
      _grow_output_buffer(utf8_bytes, utf8_nbytes, output, output_end);

    // Work out the Unicode code point for the current encoded input character.
    const unicode_t code_point = LATIN2_TABLE[*encoded_bytes - LATIN2_DELTA];

    // Write out the UTF-8.
    const size_t n = write_utf8(code_point, output);
    assert(0 < n && n <= LATIN2_UTF8_NBYTES);  // FIXME remove this after unit tests
    output += n;
    ++encoded_bytes;
  }
  return output - utf8_bytes;
}


size_t
utf_8_to_utf8(const uint8_t *const encoded_bytes, const size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes) {
  if (utf8_nbytes < encoded_nbytes) {
    delete [] utf8_bytes;
    utf8_nbytes = encoded_nbytes;
    utf8_bytes = new uint8_t[utf8_nbytes];
  }
  std::memcpy(utf8_bytes, encoded_bytes, encoded_nbytes);
  return encoded_nbytes;
}

}
