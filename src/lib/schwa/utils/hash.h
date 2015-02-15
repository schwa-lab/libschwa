/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_HASH_H_
#define SCHWA_UTILS_HASH_H_

#include <cstring>
#include <functional>

#include <schwa/_base.h>
#include <schwa/third-party/cityhash/city.h>


namespace schwa {

  struct cstr_equal_to : public std::binary_function<const char *, const char *, bool> {
    inline bool
    operator ()(const char *const a, const char *const b) const {
      return std::strcmp(a, b) == 0;
    }
  };


  template <size_t SIZE_T_BYTES>
  struct _cstr_hash { };

  template <>
  struct _cstr_hash<8> : public std::unary_function<const char *, uint64_t> {
    inline uint64_t
    operator ()(const char *const str) const {
      return third_party::cityhash::CityHash64(str, std::strlen(str));
    }
  };

  template <>
  struct _cstr_hash<4> : public std::unary_function<const char *, uint32_t> {
    inline uint32_t
    operator ()(const char *const str) const {
      return third_party::cityhash::CityHash32(str, std::strlen(str));
    }
  };

  struct cstr_hash : public _cstr_hash<sizeof(size_t)> { };

  // using cstr_map<T> = std::unordered_map<const char *, T, cstr_hash, cstr_equal_to>;


  struct uint8str_equal_to : public std::binary_function<const uint8_t *, const uint8_t *, bool> {
    inline bool
    operator ()(const uint8_t *const a, const uint8_t *const b) const {
      const char *const x = reinterpret_cast<const char *>(a);
      const char *const y = reinterpret_cast<const char *>(b);
      return std::strcmp(x, y) == 0;
    }
  };


  template <size_t SIZE_T_BYTES>
  struct _uint8str_hash { };

  template <>
  struct _uint8str_hash<8> : public std::unary_function<const uint8_t *, uint64_t> {
    inline uint64_t
    operator ()(const uint8_t *const str) const {
      const char *const x = reinterpret_cast<const char *>(str);
      return third_party::cityhash::CityHash64(x, std::strlen(x));
    }
  };

  template <>
  struct _uint8str_hash<4> : public std::unary_function<const uint8_t *, uint32_t> {
    inline uint32_t
    operator ()(const uint8_t *const str) const {
      const char *const x = reinterpret_cast<const char *>(str);
      return third_party::cityhash::CityHash32(x, std::strlen(x));
    }
  };

  struct uint8str_hash : public _uint8str_hash<sizeof(size_t)> { };

  // using uint8str_map<T> = std::unordered_map<const uint8_t *, T, uint8str_hash, uint8str_equal_to>;
}

#endif  // SCHWA_UTILS_HASH_H_
