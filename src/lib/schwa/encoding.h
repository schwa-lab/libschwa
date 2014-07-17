/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_ENCODING_H_
#define SCHWA_ENCODING_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>
#include <schwa/unicode.h>


namespace schwa {

  /**
   * Enum class of encoding types that we know how to convert to UTF-8.
   **/
  enum class Encoding : uint8_t {
    ASCII = 1,
    CP1251 = 41,
    CP1252 = 42,
    CP1253 = 43,
    CP1254 = 44,
    CP1255 = 45,
    CP1256 = 46,
    CP1257 = 47,
    CP1258 = 48,
    GB2312 = 2,
    LATIN1 = 21,
    LATIN2 = 22,
    LATIN3 = 23,
    LATIN4 = 24,
    LATIN5 = 25,
    LATIN6 = 26,
    LATIN7 = 27,
    LATIN8 = 28,
    LATIN9 = 29,
    LATIN10 = 30,
    LATIN11 = 31,
    LATIN13 = 32,
    LATIN14 = 33,
    LATIN15 = 34,
    LATIN16 = 35,
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


  class EncodingResult {
  private:
    uint8_t *_utf8;
    uint8_t *_deltas;
    size_t _allocated;
    size_t _consumed;
    uint8_t _tmp[4];
    Encoding _from_encoding;

    void _grow(size_t new_nbytes);

  public:
    explicit EncodingResult(Encoding from_encoding=Encoding::__END__);
    ~EncodingResult(void);

    inline size_t allocated(void) const { return _allocated; }
    inline const uint8_t *deltas(void) const { return _deltas; }
    inline Encoding from_encoding(void) const { return _from_encoding; }
    inline size_t nbytes(void) const { return _consumed; }
    inline size_t remaining(void) const { return _allocated - _consumed; }
    inline const uint8_t *utf8(void) const { return _utf8; }

    void grow(size_t grow_size=4096);
    void reserve(size_t nbytes);
    void reset(Encoding from_encoding);

    size_t write(unicode_t code_point, uint8_t nbytes_consumed);
    size_t write(unicode_t code_point, uint8_t nbytes_consumed, uint8_t utf8_nbytes_needed);

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(EncodingResult);
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

  void to_utf8(Encoding encoding, const char *encoded_bytes, EncodingResult &result);
  void to_utf8(Encoding encoding, const std::string &encoded_bytes, EncodingResult &result);
  void to_utf8(Encoding encoding, const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);


  void ascii_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void cp1251_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void cp1252_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void cp1253_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void cp1254_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void cp1255_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void cp1256_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void cp1257_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void cp1258_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void gb2312_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin1_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin2_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin3_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin4_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin5_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin6_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin7_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin8_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin9_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin10_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin11_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin13_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin14_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin15_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void latin16_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);
  void utf_8_to_utf8(const uint8_t *encoded_bytes, size_t encoded_nbytes, EncodingResult &result);

}  // namespace schwa

#endif  // SCHWA_ENCODING_H_
