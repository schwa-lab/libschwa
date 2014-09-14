/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_HASH_IMPL_H_
#define SCHWA_HASH_IMPL_H_

#include <cstring>
#include <functional>
#include <string>

#include <schwa/unicode.h>


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

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      return third_party::xxhash::XXH32_update(state, obj, std::strlen(obj));
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

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      return third_party::xxhash::XXH64_update(state, obj, std::strlen(obj));
    }
  };


  // ==========================================================================
  /**
   * Template specialisation of Hasher32 for char arrays use xxhash.
   * \see third_party::xxhash::XXH32
   **/
  template <size_t N>
  struct Hasher32<char[N]> : public Hasher32Base<char[N]> {
    using typename Hasher32Base<char[N]>::argument_type;
    using typename Hasher32Base<char[N]>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return third_party::xxhash::XXH32(obj, N - 1, 0);
    }

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      return third_party::xxhash::XXH32_update(state, obj, N - 1);
    }
  };

  /**
   * Template specialisation of Hasher64 for char arrays to use xxhash.
   * \see third_party::xxhash::XXH64
   **/
  template <size_t N>
  struct Hasher64<char[N]> : public Hasher64Base<char[N]> {
    using typename Hasher64Base<char[N]>::argument_type;
    using typename Hasher64Base<char[N]>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return third_party::xxhash::XXH64(obj, N - 1, 0);
    }

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      return third_party::xxhash::XXH64_update(state, obj, N - 1);
    }
  };


  // ==========================================================================
  /**
   * Template specialisation of Hasher32 for std::basic_string to use xxhash.
   * \see third_party::xxhash::XXH32
   **/
  template <typename charT>
  struct Hasher32<std::basic_string<charT>> : public Hasher32Base<std::basic_string<charT>> {
    using typename Hasher32Base<std::basic_string<charT>>::argument_type;
    using typename Hasher32Base<std::basic_string<charT>>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return third_party::xxhash::XXH32(obj.c_str(), obj.size()*sizeof(charT), 0);
    }

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      return third_party::xxhash::XXH32_update(state, obj.c_str(), obj.size()*sizeof(charT));
    }
  };

  /**
   * Template specialisation of Hasher64 for std::basic_string to use xxhash.
   * \see third_party::xxhash::XXH64
   **/
  template <typename charT>
  struct Hasher64<std::basic_string<charT>> : public Hasher64Base<std::basic_string<charT>> {
    using typename Hasher64Base<std::basic_string<charT>>::argument_type;
    using typename Hasher64Base<std::basic_string<charT>>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return third_party::xxhash::XXH64(obj.c_str(), obj.size()*sizeof(charT), 0);
    }

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      return third_party::xxhash::XXH64_update(state, obj.c_str(), obj.size()*sizeof(charT));
    }
  };


  // ==========================================================================
  /**
   * Template specialisation of Hasher32 for UnicodeString to use xxhash over UTF-8 encoded data.
   * \see third_party::xxhash::XXH32
   **/
  template <>
  struct Hasher32<UnicodeString> : public Hasher32Base<UnicodeString> {
    using Hasher32Base<UnicodeString>::argument_type;
    using Hasher32Base<UnicodeString>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      const std::string utf8 = obj.to_utf8();
      return third_party::xxhash::XXH32(utf8.c_str(), utf8.size(), 0);
    }

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      const std::string utf8 = obj.to_utf8();
      return third_party::xxhash::XXH32_update(state, utf8.c_str(), utf8.size());
    }
  };

  /**
   * Template specialisation of Hasher64 for UnicodeString to use xxhash over UTF-8 encoded data.
   * \see third_party::xxhash::XXH64
   **/
  template <>
  struct Hasher64<UnicodeString> : public Hasher64Base<UnicodeString> {
    using Hasher64Base<UnicodeString>::argument_type;
    using Hasher64Base<UnicodeString>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      const std::string utf8 = obj.to_utf8();
      return third_party::xxhash::XXH64(utf8.c_str(), utf8.size(), 0);
    }

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      const std::string utf8 = obj.to_utf8();
      return third_party::xxhash::XXH64_update(state, utf8.c_str(), utf8.size());
    }
  };

}

#endif  // SCHWA_HASH_IMPL_H_
