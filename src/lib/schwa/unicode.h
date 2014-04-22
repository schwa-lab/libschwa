/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UNICODE_H_
#define SCHWA_UNICODE_H_

#include <iostream>
#include <iterator>
#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>


namespace schwa {

  static_assert(std::is_unsigned<char32_t>::value, "char32_t is expected to be unsigned");
  using unicode_t = char32_t;
  using UnicodeString = std::basic_string<unicode_t>;

  constexpr const unicode_t MAX_CODE_POINT = 0x110000;


  class UnicodeException : public Exception {
  protected:
    unicode_t _code_point;

  public:
    UnicodeException(const std::string &msg, unicode_t code_point);
    virtual ~UnicodeException(void) throw() { }

    inline unicode_t code_point(void) const { return _code_point; }

    static inline void
    throw_invalid_code_point(const unicode_t code_point) {
      throw UnicodeException("Invalid code point", code_point);
    }
  };


  unicode_t read_utf8(const char **data, const char *end);
  size_t write_utf8(unicode_t code_point, char utf8[4]);
  size_t write_utf8(unicode_t code_point, std::ostream &out);
  size_t write_utf8(const UnicodeString &s, std::ostream &out);


  class UTF8Decoder {
  public:
    class Iterator : public std::iterator<std::input_iterator_tag, unicode_t> {
    private:
      const char *_upto;
      const char *_end;
      unicode_t _code_point;

      void _read(void);

    public:
      Iterator(void) : _upto(nullptr), _end(nullptr), _code_point(MAX_CODE_POINT) { _read(); }
      explicit Iterator(const char *start, const char *end) : _upto(start), _end(end), _code_point(MAX_CODE_POINT) { _read(); }
      Iterator(const Iterator &o) : _upto(o._upto), _end(o._end), _code_point(o._code_point) { }
      Iterator(const Iterator &&o) : _upto(o._upto), _end(o._end), _code_point(o._code_point) { }

      Iterator &operator =(const Iterator &o);
      inline Iterator &operator++(void) { _read(); return *this; }
      inline bool operator ==(const Iterator &o) const { return _upto == o._upto && _end == o._end; }
      inline bool operator !=(const Iterator &o) const { return _upto != o._upto || _end != o._end; }
      value_type operator *(void) const { return _code_point; }
    };

    using iterator = Iterator;
    using const_iterator = Iterator;

  private:
    const char *const _data;
    const size_t _size;

  public:
    explicit UTF8Decoder(const std::string &s) : _data(s.c_str()), _size(s.size()) { }
    UTF8Decoder(const char *data, size_t size) : _data(data), _size(size) { }
    UTF8Decoder(const UTF8Decoder &o) : _data(o._data), _size(o._size) { }
    UTF8Decoder(const UTF8Decoder &&o) : _data(o._data), _size(o._size) { }

    inline iterator begin(void) const { return iterator(_data, _data + _size); }
    inline iterator end(void) const { return iterator(); }
    inline const_iterator cbegin(void) const { return const_iterator(_data, _data + _size); }
    inline const_iterator cend(void) const { return const_iterator(); }

    template <typename... T>
    static inline UnicodeString
    to_string(T&&... args) {
      UTF8Decoder d(args...);
      return UnicodeString(d.begin(), d.end());
    }
  };

}  // namespace schwa


//<! Add support for writing UTF-8.
inline std::ostream &
operator <<(std::ostream &out, const schwa::UnicodeString &s) {
  for (const auto c : s)
    schwa::write_utf8(c, out);
  return out;
}

#include <schwa/unicode_gen.h>

#endif  // SCHWA_UNICODE_H_
