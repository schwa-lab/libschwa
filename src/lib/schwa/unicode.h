/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UNICODE_H_
#define SCHWA_UNICODE_H_

#include <functional>
#include <iostream>
#include <initializer_list>
#include <iterator>
#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>


namespace schwa {

  using unicode_t = char32_t;
  class UnicodeException;
  class UnicodeString;
  class UTF8Decoder;

  constexpr const unicode_t MAX_CODE_POINT = 0x110000;


  unicode_t read_utf8(const char **data, const char *end);
  unicode_t read_utf8_backwards(const char **data, const char *end);
  size_t write_utf8(unicode_t code_point, char utf8[4]);
  size_t write_utf8(unicode_t code_point, std::ostream &out);
  size_t write_utf8(const UnicodeString &s, std::ostream &out);

}  // namespace schwa

#include <schwa/unicode_gen.h>

namespace schwa {

  class UnicodeException : public Exception {
  protected:
    unicode_t _code_point;

  public:
    UnicodeException(const std::string &msg, unicode_t code_point);
    virtual ~UnicodeException(void) throw() { }

    inline unicode_t code_point(void) const { return _code_point; }

    static void throw_invalid_code_point(unicode_t code_point);
  };


  class UTF8Decoder {
  public:
    template <class F>
    class Iterator : public std::iterator<std::input_iterator_tag, unicode_t> {
    private:
      const char *_upto;
      const char *_end;
      unicode_t _code_point;
      F _functor;

      void
      _read(void) {
        if (_upto == nullptr)  // Are we already at the end?
          return;
        else if (_upto == _end) {  // Have we just consumed the last character?
          _upto = _end = nullptr;
          _code_point = MAX_CODE_POINT;
          return;
        }
        _code_point = _functor(&_upto, _end);
      }

    public:
      Iterator(void) : _upto(nullptr), _end(nullptr), _code_point(MAX_CODE_POINT) { _read(); }
      explicit Iterator(const char *start, const char *end) : _upto(start), _end(end), _code_point(MAX_CODE_POINT) { _read(); }
      Iterator(const Iterator &o) : _upto(o._upto), _end(o._end), _code_point(o._code_point) { }
      Iterator(const Iterator &&o) : _upto(o._upto), _end(o._end), _code_point(o._code_point) { }

      inline Iterator &operator++(void) { _read(); return *this; }
      inline bool operator ==(const Iterator &o) const { return _upto == o._upto && _end == o._end; }
      inline bool operator !=(const Iterator &o) const { return _upto != o._upto || _end != o._end; }
      value_type operator *(void) const { return _code_point; }

      Iterator &
      operator =(const Iterator &o) {
        _upto = o._upto;
        _end = o._end;
        _code_point = o._code_point;
        return *this;
      }
    };

    struct ReadUTF8Forwards {
      inline unicode_t operator ()(const char **ptr, const char *end) const { return schwa::read_utf8(ptr, end); }
    };

    struct ReadUTF8Backwards {
      inline unicode_t operator ()(const char **ptr, const char *end) const { return schwa::read_utf8_backwards(ptr, end); }
    };

    using ForwardIterator = Iterator<ReadUTF8Forwards>;
    using BackwardIterator = Iterator<ReadUTF8Backwards>;

    using iterator = ForwardIterator;
    using const_iterator = ForwardIterator;
    using reverse_iterator = BackwardIterator;
    using const_reverse_iterator = BackwardIterator;

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
    inline reverse_iterator rbegin(void) const { return reverse_iterator(_data + _size - 1, _data - 1); }
    inline reverse_iterator rend(void) const { return reverse_iterator(); }
    inline const_reverse_iterator crbegin(void) const { return const_reverse_iterator(_data + _size - 1, _data - 1); }
    inline const_reverse_iterator crend(void) const { return const_reverse_iterator(); }
  };


  class UnicodeString {
  public:
    using underlying_type = std::basic_string<unicode_t>;

    using allocator_type = underlying_type::allocator_type;
    using const_iterator = underlying_type::const_iterator;
    using const_pointer = underlying_type::const_pointer;
    using const_reference = underlying_type::const_reference;
    using const_reverse_iterator = underlying_type::const_reverse_iterator;
    using difference_type = underlying_type::difference_type;
    using iterator = underlying_type::iterator;
    using pointer = underlying_type::pointer;
    using reference = underlying_type::reference;
    using reverse_iterator = underlying_type::reverse_iterator;
    using size_type = underlying_type::size_type;
    using traits_type = underlying_type::traits_type;
    using value_type = underlying_type::value_type;

    static const size_type npos = -1;

  private:
    underlying_type _str;

  public:
    explicit UnicodeString(const allocator_type &alloc=allocator_type()) : _str(alloc) { }
    UnicodeString(const UnicodeString &o) : _str(o._str) { }
    UnicodeString(const UnicodeString &o, const allocator_type &alloc) : _str(o._str, alloc) { }
    UnicodeString(const UnicodeString &o, size_type pos, size_type len=npos, const allocator_type &alloc=allocator_type()) : _str(o._str, pos, len, alloc) { }
    UnicodeString(const underlying_type &s) : _str(s) { }
    UnicodeString(const underlying_type &s, const allocator_type &alloc) : _str(s, alloc) { }
    UnicodeString(const underlying_type &s, size_type pos, size_type len=npos, const allocator_type &alloc=allocator_type()) : _str(s, pos, len, alloc) { }
    UnicodeString(const traits_type::char_type *s, const allocator_type &alloc=allocator_type()) : _str(s, alloc) { }
    UnicodeString(const traits_type::char_type *s, size_type n, const allocator_type &alloc=allocator_type()) : _str(s, n, alloc) { }
    UnicodeString(size_type n, traits_type::char_type c, const allocator_type &alloc=allocator_type()) : _str(n, c, alloc) { }
    template <class InputIterator>
      UnicodeString(InputIterator first, InputIterator last, const allocator_type &alloc=allocator_type()) : _str(first, last, alloc) { }
    UnicodeString(std::initializer_list<traits_type::char_type> il, const allocator_type &alloc=allocator_type()) : _str(il, alloc) { }
    UnicodeString(const UnicodeString &&o) noexcept : _str(o._str) { }
    UnicodeString(const UnicodeString &&o, const allocator_type &alloc) : _str(o._str, alloc) { }

    inline UnicodeString &operator =(const UnicodeString &o) { _str = o._str; return *this; }
    inline UnicodeString &operator =(const underlying_type &s) { _str = s; return *this; }
    inline UnicodeString &operator =(const traits_type::char_type *o) { _str = o; return *this; }
    inline UnicodeString &operator =(traits_type::char_type o) { _str = o; return *this; }
    inline UnicodeString &operator =(std::initializer_list<traits_type::char_type> il) { _str = il; return *this; }
    inline UnicodeString &operator =(const UnicodeString &&o) { _str = o._str; return *this; }
    inline UnicodeString &operator =(const underlying_type &&s) { _str = s; return *this; }

    inline iterator begin(void) noexcept { return _str.begin(); }
    inline const_iterator begin(void) const noexcept { return _str.begin(); }
    inline iterator end(void) noexcept { return _str.end(); }
    inline const_iterator end(void) const noexcept { return _str.end(); }
    inline reverse_iterator rbegin(void) noexcept { return _str.rbegin(); }
    inline const_reverse_iterator rbegin(void) const noexcept { return _str.rbegin(); }
    inline reverse_iterator rend(void) noexcept { return _str.rend(); }
    inline const_reverse_iterator rend(void) const noexcept { return _str.rend(); }
    inline const_iterator cbegin(void) const noexcept { return _str.cbegin(); }
    inline const_iterator cend(void) const noexcept { return _str.cend(); }
    inline const_reverse_iterator crbegin(void) const noexcept { return _str.crbegin(); }
    inline const_reverse_iterator crend(void) const noexcept { return _str.crend(); }

    inline size_type size(void) const noexcept { return _str.size(); }
    inline size_type length(void) const noexcept { return _str.length(); }
    inline size_type max_size(void) const noexcept { return _str.max_size(); }
    inline void resize (size_type n) { _str.resize(n); }
    inline void resize (size_type n, traits_type::char_type c) { _str.resize(n, c); }
    inline size_type capacity(void) const noexcept { return _str.capacity(); }
    inline void reserve (size_type n=0) { _str.reserve(n); }
    inline void clear(void) noexcept { _str.clear(); }
    inline bool empty(void) const noexcept { return _str.empty(); }
    inline void shrink_to_fit(void) { _str.shrink_to_fit(); }

    inline reference operator [](size_type pos) { return _str[pos]; }
    inline const_reference operator [](size_type pos) const { return _str[pos]; }
    inline reference at(size_type pos) { return _str.at(pos); }
    inline const_reference at(size_type pos) const { return _str.at(pos); }
    inline traits_type::char_type& back(void) { return _str.back(); }
    inline const traits_type::char_type& back(void) const { return _str.back(); }
    inline traits_type::char_type& front(void) { return _str.front(); }
    inline const traits_type::char_type& front(void) const { return _str.front(); }

    inline UnicodeString &operator +=(const UnicodeString &o) { _str += o._str; return *this; }
    inline UnicodeString &operator +=(const underlying_type &s) { _str += s; return *this; }
    inline UnicodeString &operator +=(const traits_type::char_type *s) { _str += s; return *this; }
    inline UnicodeString &operator +=(traits_type::char_type c) { _str += c; return *this; }
    inline UnicodeString &operator +=(std::initializer_list<traits_type::char_type> il) { _str += il; return *this; }
    inline UnicodeString &append(const UnicodeString &o) { _str.append(o._str); return *this; }
    inline UnicodeString &append(const underlying_type &s) { _str.append(s); return *this; }
    inline UnicodeString &append(const UnicodeString &o, size_type subpos, size_type sublen) { _str.append(o._str, subpos, sublen); return *this; }
    inline UnicodeString &append(const underlying_type &s, size_type subpos, size_type sublen) { _str.append(s, subpos, sublen); return *this; }
    inline UnicodeString &append(const traits_type::char_type *s) { _str.append(s); return *this; }
    inline UnicodeString &append(const traits_type::char_type *s, size_type n) { _str.append(s, n); return *this; }
    inline UnicodeString &append(size_type n, traits_type::char_type c) { _str.append(n, c); return *this; }
    template <class InputIterator>
      inline UnicodeString &append(InputIterator first, InputIterator last) { _str.append(first, last); return *this; }
    inline UnicodeString &append(std::initializer_list<traits_type::char_type> il) { _str.append(il); return *this; }
    inline UnicodeString &assign(const UnicodeString &o) { _str.assign(o._str); return *this; }
    inline UnicodeString &assign(const underlying_type &s) { _str.assign(s); return *this; }
    inline UnicodeString &assign(const UnicodeString &o, size_type subpos, size_type sublen) { _str.assign(o._str, subpos, sublen); return *this; }
    inline UnicodeString &assign(const underlying_type &s, size_type subpos, size_type sublen) { _str.assign(s, subpos, sublen); return *this; }
    inline UnicodeString &assign(const traits_type::char_type *s) { _str.assign(s); return *this; }
    inline UnicodeString &assign(const traits_type::char_type *s, size_type n) { _str.assign(s, n); return *this; }
    inline UnicodeString &assign(size_type n, traits_type::char_type c) { _str.assign(n, c); return *this; }
    template <class InputIterator>
      inline UnicodeString &assign(InputIterator first, InputIterator last) { _str.assign(first, last); return *this; }
    inline UnicodeString &assign(std::initializer_list<traits_type::char_type> il) { _str.assign(il); return *this; }
    inline UnicodeString &assign(const UnicodeString &&o) noexcept { _str.assign(o._str); return *this; }
    inline UnicodeString &assign(const underlying_type &&s) noexcept { _str.assign(s); return *this; }
    inline void push_back(traits_type::char_type c) { _str.push_back(c); }
    UnicodeString &insert(size_type pos, const UnicodeString &str) { _str.insert(pos, str._str); return *this; }
    UnicodeString &insert(size_type pos, const UnicodeString &str, size_type subpos, size_type sublen) { _str.insert(pos, str._str, subpos, sublen); return *this; }
    UnicodeString &insert(size_type pos, const traits_type::char_type *s) { _str.insert(pos, s); return *this; }
    UnicodeString &insert(size_type pos, const traits_type::char_type *s, size_type n) { _str.insert(pos, s, n); return *this; }
    UnicodeString &insert(size_type pos, size_type n, traits_type::char_type c) { _str.insert(pos, n, c); return *this; }
    iterator insert(const_iterator p, size_type n, traits_type::char_type c) { return _str.insert(p, n, c); }
    iterator insert(const_iterator p, traits_type::char_type c) { return _str.insert(p, c); }
    template <class InputIterator>
      iterator insert(iterator p, InputIterator first, InputIterator last) { return _str.insert(p, first, last); }
    UnicodeString &insert(const_iterator p, std::initializer_list<traits_type::char_type> il) { _str.insert(p, il); return *this; }
    inline UnicodeString &erase(size_type pos=0, size_type len=npos) { _str.erase(pos, len); return *this; }
    inline iterator erase(const_iterator p) { return _str.erase(p); }
    inline iterator erase(const_iterator first, const_iterator last) { return _str.erase(first, last); }
    UnicodeString &replace(size_type pos, size_type len, const UnicodeString &str) { _str.replace(pos, len, str._str); return *this; }
    UnicodeString &replace(const_iterator i1, const_iterator i2, const UnicodeString &str) { _str.replace(i1, i2, str._str); return *this; }
    UnicodeString &replace(size_type pos, size_type len, const UnicodeString &str, size_type subpos, size_type sublen) { _str.replace(pos, len, str._str, subpos, sublen); return *this; }
    UnicodeString &replace(size_type pos, size_type len, const traits_type::char_type *s) { _str.replace(pos, len, s); return *this; }
    UnicodeString &replace(const_iterator i1, const_iterator i2, const traits_type::char_type *s) { _str.replace(i1, i2, s); return *this; }
    UnicodeString &replace(size_type pos, size_type len, const traits_type::char_type *s, size_type n) { _str.replace(pos, len, s, n); return *this; }
    UnicodeString &replace(const_iterator i1, const_iterator i2, const traits_type::char_type *s, size_type n) { _str.replace(i1, i2, s, n); return *this; }
    UnicodeString &replace(size_type pos, size_type len, size_type n, traits_type::char_type c) { _str.replace(pos, len, n, c); return *this; }
    UnicodeString &replace(const_iterator i1, const_iterator i2, size_type n, traits_type::char_type c) { _str.replace(i1, i2, n, c); return *this; }
    template <class InputIterator>
      UnicodeString &replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last) { _str.replace(i1, i2, first, last); return *this; }
    UnicodeString &replace(const_iterator i1, const_iterator i2, std::initializer_list<traits_type::char_type> il) { _str.replace(i1, i2, il); return *this; }
    inline void swap(UnicodeString &o) { _str.swap(o._str); }
    inline void swap(underlying_type &s) { _str.swap(s); }
    inline void pop_back(void) { _str.pop_back(); }

    inline const traits_type::char_type* c_str(void) const noexcept { return _str.c_str(); }
    inline const traits_type::char_type* data(void) const noexcept { return _str.data(); }
    inline allocator_type get_allocator(void) const noexcept { return _str.get_allocator(); }
    inline size_type copy(traits_type::char_type* s, size_type len, size_type pos=0) const { return _str.copy(s, len, pos); }
    inline size_type find(const UnicodeString &o, size_type pos=0) const noexcept { return _str.find(o._str, pos); }
    inline size_type find(const traits_type::char_type *s, size_type pos=0) const { return _str.find(s, pos); }
    inline size_type find(const traits_type::char_type *s, size_type pos, size_type n) const { return _str.find(s, pos, n); }
    inline size_type find(traits_type::char_type c, size_type pos=0) const noexcept { return _str.find(c, pos); }
    inline size_type rfind(const UnicodeString &o, size_type pos=npos) const noexcept { return _str.rfind(o._str, pos); }
    inline size_type rfind(const traits_type::char_type *s, size_type pos=npos) const { return _str.rfind(s, pos); }
    inline size_type rfind(const traits_type::char_type *s, size_type pos, size_type n) const { return _str.rfind(s, pos, n); }
    inline size_type rfind(traits_type::char_type c, size_type pos=npos) const noexcept { return _str.rfind(c, pos); }
    inline size_type find_first_of(const UnicodeString &o, size_type pos=0) const noexcept { return _str.find_first_of(o._str, pos); }
    inline size_type find_first_of(const traits_type::char_type *s, size_type pos=0) const { return _str.find_first_of(s, pos); }
    inline size_type find_first_of(const traits_type::char_type *s, size_type pos, size_type n) const { return _str.find_first_of(s, pos, n); }
    inline size_type find_first_of(traits_type::char_type c, size_type pos=0) const noexcept { return _str.find_first_of(c, pos); }
    inline size_type find_last_of(const UnicodeString &o, size_type pos=npos) const noexcept { return _str.find_last_of(o._str, pos); }
    inline size_type find_last_of(const traits_type::char_type *s, size_type pos=npos) const { return _str.find_last_of(s, pos); }
    inline size_type find_last_of(const traits_type::char_type *s, size_type pos, size_type n) const { return _str.find_last_of(s, pos, n); }
    inline size_type find_last_of(traits_type::char_type c, size_type pos=npos) const noexcept { return _str.find_last_of(c, pos); }
    inline size_type find_first_not_of(const UnicodeString &o, size_type pos=0) const noexcept { return _str.find_first_not_of(o._str, pos); }
    inline size_type find_first_not_of(const traits_type::char_type *s, size_type pos=0) const { return _str.find_first_not_of(s, pos); }
    inline size_type find_first_not_of(const traits_type::char_type *s, size_type pos, size_type n) const { return _str.find_first_not_of(s, pos, n); }
    inline size_type find_first_not_of(traits_type::char_type c, size_type pos=0) const noexcept { return _str.find_first_not_of(c, pos); }
    inline size_type find_last_not_of(const UnicodeString &o, size_type pos=npos) const noexcept { return _str.find_last_not_of(o._str, pos); }
    inline size_type find_last_not_of(const traits_type::char_type *s, size_type pos=npos) const { return _str.find_last_not_of(s, pos); }
    inline size_type find_last_not_of(const traits_type::char_type *s, size_type pos, size_type n) const { return _str.find_last_not_of(s, pos, n); }
    inline size_type find_last_not_of(traits_type::char_type c, size_type pos=npos) const noexcept { return _str.find_last_not_of(c, pos); }
    inline UnicodeString substr(size_type pos=0, size_type len=npos) const { return _str.substr(pos, len); }
    inline int compare(const UnicodeString &o) const noexcept { return _str.compare(o._str); }
    inline int compare(const underlying_type &s) const noexcept { return _str.compare(s); }
    inline int compare(size_type pos, size_type len, const UnicodeString &o) const { return _str.compare(pos, len, o._str); }
    inline int compare(size_type pos, size_type len, const underlying_type &s) const { return _str.compare(pos, len, s); }
    inline int compare(size_type pos, size_type len, const UnicodeString &o, size_type subpos, size_type sublen) const { return _str.compare(pos, len, o._str, subpos, sublen); }
    inline int compare(size_type pos, size_type len, const underlying_type &s, size_type subpos, size_type sublen) const { return _str.compare(pos, len, s, subpos, sublen); }
    inline int compare(const traits_type::char_type *s) const { return _str.compare(s); }
    inline int compare(size_type pos, size_type len, const traits_type::char_type *s) const { return _str.compare(pos, len, s); }
    inline int compare(size_type pos, size_type len, const traits_type::char_type *s, size_type n) const { return _str.compare(pos, len, s, n); }

    inline bool operator ==(const UnicodeString &o) const noexcept { return _str == o._str; }
    inline bool operator !=(const UnicodeString &o) const noexcept { return _str != o._str; }
    inline bool operator <(const UnicodeString &o) const noexcept { return _str < o._str; }
    inline bool operator <=(const UnicodeString &o) const noexcept { return _str <= o._str; }
    inline bool operator >(const UnicodeString &o) const noexcept { return _str > o._str; }
    inline bool operator >=(const UnicodeString &o) const noexcept { return _str >= o._str; }
    inline bool operator ==(const underlying_type &s) const noexcept { return _str == s; }
    inline bool operator !=(const underlying_type &s) const noexcept { return _str != s; }
    inline bool operator <(const underlying_type &s) const noexcept { return _str < s; }
    inline bool operator <=(const underlying_type &s) const noexcept { return _str <= s; }
    inline bool operator >(const underlying_type &s) const noexcept { return _str > s; }
    inline bool operator >=(const underlying_type &s) const noexcept { return _str >= s; }
    inline bool operator ==(const traits_type::char_type *s) const noexcept { return _str == s; }
    inline bool operator !=(const traits_type::char_type *s) const noexcept { return _str != s; }
    inline bool operator <(const traits_type::char_type *s) const noexcept { return _str < s; }
    inline bool operator <=(const traits_type::char_type *s) const noexcept { return _str <= s; }
    inline bool operator >(const traits_type::char_type *s) const noexcept { return _str > s; }
    inline bool operator >=(const traits_type::char_type *s) const noexcept { return _str >= s; }

    // ctype methods.
    bool is_alnum(void) const;
    bool is_alpha(void) const;
    bool is_cased(void) const;
    bool is_case_ignorable(void) const;
    bool is_decimal(void) const;
    bool is_digit(void) const;
    bool is_lower(void) const;
    bool is_numeric(void) const;
    bool is_printable(void) const;
    bool is_space(void) const;
    bool is_upper(void) const;
    UnicodeString to_lower(void) const;
    UnicodeString to_title(void) const;
    UnicodeString to_upper(void) const;

    // Get the underlying basic_string<unicode_t>.
    inline underlying_type &get_underlying(void) { return _str; }
    inline const underlying_type &get_underlying(void) const { return _str; }

    // UTF-8 encoding and decoding.
    explicit UnicodeString(const std::string &utf8, const allocator_type &alloc=allocator_type());
    explicit UnicodeString(const char *utf8, const allocator_type &alloc=allocator_type());
    UnicodeString &operator +=(const char *utf8);
    UnicodeString &operator +=(const char c);

    std::string to_utf8(void) const;

    template <typename... T>
    static inline UnicodeString
    from_utf8(T&&... args) {
      UTF8Decoder d(args...);
      return UnicodeString(d.begin(), d.end());
    }
  };

  inline UnicodeString operator +(const UnicodeString &a, const UnicodeString &b) { UnicodeString x(a); x += b; return x; }
  inline UnicodeString operator +(const UnicodeString &a, const UnicodeString::underlying_type &b) { UnicodeString x(a); x += b; return x; }
  inline UnicodeString operator +(const UnicodeString &a, const UnicodeString::traits_type::char_type *b) { UnicodeString x(a); x += b; return x; }
  inline UnicodeString operator +(const UnicodeString &a, UnicodeString::traits_type::char_type b) { UnicodeString x(a); x += b; return x; }
  inline UnicodeString operator +(const UnicodeString::underlying_type &b, const UnicodeString &a) { UnicodeString x(a); x += b; return x; }
  inline UnicodeString operator +(const UnicodeString::traits_type::char_type *b, const UnicodeString &a) { UnicodeString x(a); x += b; return x; }
  inline UnicodeString operator +(UnicodeString::traits_type::char_type b, const UnicodeString &a) { UnicodeString x(a); x += b; return x; }

  inline void swap(UnicodeString &a, UnicodeString &b) { a.swap(b); }

  inline std::ostream &
  operator <<(std::ostream &out, const UnicodeString &s) {
    write_utf8(s, out);
    return out;
  }

  template <class charT, class traits>
  inline std::basic_ostream<charT, traits> &
  operator <<(std::basic_ostream<charT, traits> &out, const UnicodeString &s) {
    return out << s.get_underlying();
  }

}  // namespace schwa


namespace std {

  /**
   * Specialisation of std::hash for schwa::UnicodeString.
   **/
  template <>
  struct hash<schwa::UnicodeString> {
  public:
    using argument_type = schwa::UnicodeString;
    using result_type = hash<schwa::UnicodeString::underlying_type>::result_type;

  private:
    hash<schwa::UnicodeString::underlying_type> _hash;

  public:
    hash(void) { }
    inline result_type operator ()(const argument_type &x) const { return _hash(x.get_underlying()); }
  };

}  // namespace std

#endif  // SCHWA_UNICODE_H_
