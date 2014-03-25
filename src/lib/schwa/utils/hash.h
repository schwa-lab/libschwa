/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_HASH_H_
#define SCHWA_UTILS_HASH_H_

#include <cstring>
#include <functional>


namespace schwa {

  struct cstr_equal_to : public std::binary_function<const char *, const char *, bool> {
    inline bool
    operator ()(const char *const a, const char *const b) const {
      return std::strcmp(a, b) == 0;
    }
  };


  struct cstr_hash : public std::unary_function<const char *, size_t> {
    inline size_t
    operator ()(const char *str) const {
      size_t hash = 5381;
      int c;
      while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
      return hash;
    }
  };

  // using cstr_map<T> = std::unordered_map<const char *, T, cstr_hash, cstr_equal_to>;

}

#endif  // SCHWA_UTILS_HASH_H_
