/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_HASH_H_
#define SCHWA_HASH_H_

#include <functional>

#include <schwa/_base.h>
#include <schwa/third-party/xxhash/xxhash.h>


namespace schwa {

  template <typename T>
  using Hasher32Base = std::unary_function<T, uint32_t>;

  template <typename T, bool IS_INTEGRAL_OR_FLOAT>
  struct _Hasher32 : public Hasher32Base<T> {
    using typename Hasher32Base<T>::argument_type;
    using typename Hasher32Base<T>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return obj.hash32();
    }

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      return obj.xxhash32(state);
    }
  };

  template <typename T>
  struct _Hasher32<T, true> : public Hasher32Base<T> {
    using typename Hasher32Base<T>::argument_type;
    using typename Hasher32Base<T>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return third_party::xxhash::XXH32(&obj, sizeof(T), 0);
    }

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      return third_party::xxhash::XXH32_update(state, &obj, sizeof(T));
    }
  };

  /**
   * Template functor for returning a 32-bit hash function for objects. This templated
   * implementation calls the <tt>.hash32</tt> method on the provided object. This functor can be
   * specialised to provide type-specific non-method-invocation hashing.
   **/
  template <typename T>
  struct Hasher32 : public _Hasher32<T, std::is_integral<T>::value || std::is_floating_point<T>::value> { };


  template <typename T>
  using Hasher64Base = std::unary_function<T, uint64_t>;

  template <typename T, bool IS_INTEGRAL_OR_FLOAT>
  struct _Hasher64 : public Hasher64Base<T> {
    using typename Hasher64Base<T>::argument_type;
    using typename Hasher64Base<T>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return obj.hash64();
    }

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      return obj.xxhash64(state);
    }
  };

  template <typename T>
  struct _Hasher64<T, true> : public Hasher64Base<T> {
    using typename Hasher64Base<T>::argument_type;
    using typename Hasher64Base<T>::result_type;

    inline result_type
    operator ()(const argument_type &obj) const {
      return third_party::xxhash::XXH64(&obj, sizeof(T), 0);
    }

    inline third_party::xxhash::XXH_errorcode
    operator ()(const argument_type &obj, void *state) const {
      return third_party::xxhash::XXH64_update(state, &obj, sizeof(T));
    }
  };

  /**
   * Template functor for returning a 64-bit hash function for objects. This templated
   * implementation calls the <tt>.hash64</tt> method on the provided object. This functor can be
   * specialised to provide type-specific non-method-invocation hashing.
   **/
  template <typename T>
  struct Hasher64 : public _Hasher64<T, std::is_integral<T>::value || std::is_floating_point<T>::value> { };


  /**
   * Convenience function for returning a 32-bit hash function for an object. This creates a
   * temporary Hasher32 functor and evaluates it with the provided argument.
   **/
  template <typename T>
  inline uint32_t
  hash32(const T &obj) {
    return schwa::Hasher32<T>()(obj);
  }

  /**
   * Convenience function for returning a 64-bit hash function for an object. This creates a
   * temporary Hasher64 functor and evaluates it with the provided argument.
   **/
  template <typename T>
  inline uint64_t
  hash64(const T &obj) {
    return schwa::Hasher64<T>()(obj);
  }

  /**
   * Convenience function for invoking the 32-bit hash-mixing function given a pointer to the
   * xxhash state object.
   **/
  template <typename T>
  inline third_party::xxhash::XXH_errorcode
  xxhash32(const T &obj, void *state) {
    return schwa::Hasher32<T>()(obj, state);
  }

  /**
   * Convenience function for invoking the 64-bit hash-mixing function given a pointer to the
   * xxhash state object.
   **/
  template <typename T>
  inline third_party::xxhash::XXH_errorcode
  xxhash64(const T &obj, void *state) {
    return schwa::Hasher64<T>()(obj, state);
  }
}

#include <schwa/hash_impl.h>

#endif  // SCHWA_HASH_H_
