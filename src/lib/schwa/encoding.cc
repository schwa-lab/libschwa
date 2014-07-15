#include <schwa/encoding.h>

#include <array>
#include <cassert>
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include <utility>

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
}


inline void
to_utf8(Encoding encoding, const char *encoded_bytes, EncodingResult &result) {
  to_utf8(encoding, encoded_bytes, std::strlen(encoded_bytes), result);
}


inline void
to_utf8(Encoding encoding, const std::string &encoded_bytes, EncodingResult &result) {
  to_utf8(encoding, encoded_bytes.c_str(), encoded_bytes.size(), result);
}


void
to_utf8(Encoding encoding, const char *encoded_bytes, size_t encoded_nbytes, EncodingResult &result) {
  const uint8_t *encoded = reinterpret_cast<const uint8_t *>(encoded_bytes);
  switch (encoding) {
  case Encoding::UTF_8: utf_8_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::ASCII: ascii_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::CP1251: cp1251_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::CP1252: cp1252_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::GB2312: gb2312_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN1: latin1_to_utf8(encoded, encoded_nbytes, result); return;
  case Encoding::LATIN2: latin2_to_utf8(encoded, encoded_nbytes, result); return;
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
cp1251_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.reset(Encoding::CP1251);

  for (size_t i = 0; i != encoded_nbytes; ++i) {
    const unicode_t code_point = CP1251_TABLE[*encoded_bytes - CP1251_DELTA];
    result.write(code_point, 1, CP1251_UTF8_NBYTES);
    ++encoded_bytes;
  }
}


void
cp1252_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.reset(Encoding::CP1252);

  for (size_t i = 0; i != encoded_nbytes; ++i) {
    const unicode_t code_point = CP1252_TABLE[*encoded_bytes - CP1252_DELTA];
    result.write(code_point, 1, CP1252_UTF8_NBYTES);
    ++encoded_bytes;
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


void
latin1_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.reset(Encoding::LATIN1);

  for (size_t i = 0; i != encoded_nbytes; ++i) {
    const unicode_t code_point = LATIN1_TABLE[*encoded_bytes - LATIN1_DELTA];
    result.write(code_point, 1, LATIN1_UTF8_NBYTES);
    ++encoded_bytes;
  }
}


void
latin2_to_utf8(const uint8_t *encoded_bytes, const size_t encoded_nbytes, EncodingResult &result) {
  result.reset(Encoding::LATIN2);

  for (size_t i = 0; i != encoded_nbytes; ++i) {
    const unicode_t code_point = LATIN2_TABLE[*encoded_bytes - LATIN2_DELTA];
    result.write(code_point, 1, LATIN2_UTF8_NBYTES);
    ++encoded_bytes;
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

}
