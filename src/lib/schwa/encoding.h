/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_ENCODING_H_
#define SCHWA_ENCODING_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>


namespace schwa {

  /**
   * Enum class of encoding types that we know how to convert to UTF-8.
   **/
  enum class Encoding : uint8_t {
    ASCII = 1,
    CP1251 = 2,
    CP1252 = 3,
    GB2312 = 4,
    LATIN1 = 5,
    LATIN2 = 6,
    UTF_8 = 0,
    __END__,
  };
  std::ostream &operator <<(std::ostream &out, Encoding encoding);

  /**
   * Specific exception to indicate that an unknown or invalid encoding was encountered.
   **/
  class UnknownEncodingException : public Exception {
  public:
    explicit UnknownEncodingException(const std::string &msg) : Exception(msg) { }
    explicit UnknownEncodingException(Encoding encoding);
    UnknownEncodingException(const UnknownEncodingException &o) : Exception(o) { }
    virtual ~UnknownEncodingException(void) throw() { }
  };

  /**
   * Specific exception to indicate that a text in a particular encoding failed to decode.
   **/
  class DecodeException : public IOException {
  public:
    explicit DecodeException(const std::string &msg) : IOException(msg) { }
    DecodeException(const DecodeException &o) : IOException(o) { }
    virtual ~DecodeException(void) throw() { }
  };


  /**
   * Returns the canonical name of the provided encoding.
   **/
  const std::string &encoding_name(Encoding encoding);

  /**
   * Returns the appropriate \p Encoding value given a string representation of the encoding. If
   * an unknown or invalid encoding name is provided, an \p UnknownEncodingException is thrown.
   **/
  Encoding get_encoding(const std::string &name);
  Encoding get_encoding(const char *name);

  size_t to_utf8(Encoding encoding, const char *encoded_bytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes);
  size_t to_utf8(Encoding encoding, const char *encoded_bytes, size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes);
  size_t to_utf8(Encoding encoding, const std::string &encoded_bytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes);


  size_t ascii_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes);
  size_t cp1251_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes);
  size_t cp1252_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes);
  size_t gb2312_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes);
  size_t latin1_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes);
  size_t latin2_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes);
  size_t utf_8_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, uint8_t *&utf8_bytes, size_t &utf8_nbytes);

}  // namespace schwa

#endif  // SCHWA_ENCODING_H_
