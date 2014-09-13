/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_HASH_IMPL_H_
#define SCHWA_HASH_IMPL_H_

#include <cstring>
#include <functional>
#include <string>

#include <schwa/third-party/xxhash/xxhash.h>


namespace schwa {

  // ==========================================================================
  /**
   * Template specialisation of Hasher32 for C-style strings to use xxhash.
   * \see third_party::xxhash::XXH32
   **/
  template <>
  struct Hasher32<const char *> : public Hasher32Base<const char *> {
    using Hasher32Base<const char *>::argument_type;
    using Hasher32Base<const char *>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return third_party::xxhash::XXH32(obj, std::strlen(obj), 0);
    }
  };

  /**
   * Template specialisation of Hasher64 for C-style strings to use xxhash.
   * \see third_party::xxhash::XXH64
   **/
  template <>
  struct Hasher64<const char *> : public Hasher64Base<const char *> {
    using Hasher64Base<const char *>::argument_type;
    using Hasher64Base<const char *>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return third_party::xxhash::XXH64(obj, std::strlen(obj), 0);
    }
  };


  // ==========================================================================
  /**
   * Template specialisation of Hasher32 for std::string to use xxhash.
   * \see third_party::xxhash::XXH32
   **/
  template <>
  struct Hasher32<std::string> : public Hasher32Base<std::string> {
    using Hasher32Base<std::string>::argument_type;
    using Hasher32Base<std::string>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return third_party::xxhash::XXH32(obj.c_str(), obj.size(), 0);
    }
  };

  /**
   * Template specialisation of Hasher64 for std::string to use xxhash.
   * \see third_party::xxhash::XXH64
   **/
  template <>
  struct Hasher64<std::string> : public Hasher64Base<std::string> {
    using Hasher64Base<std::string>::argument_type;
    using Hasher64Base<std::string>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return third_party::xxhash::XXH64(obj.c_str(), obj.size(), 0);
    }
  };

}

#endif  // SCHWA_HASH_IMPL_H_
