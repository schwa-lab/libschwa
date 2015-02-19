/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_FEATURE_TRANSFORMERS_H_
#define SCHWA_LEARN_FEATURE_TRANSFORMERS_H_

#include <functional>
#include <string>

#include <schwa/_base.h>


namespace schwa {
  namespace learn {

    class NoTransform {
    public:
      using value_type = std::string;

      inline value_type
      operator ()(const std::string &feature) const {
        return feature;
      }
    };


    template <class HASHER=std::hash<std::string>>
    class HasherTransform {
    public:
      using value_type = typename HASHER::result_type;

    private:
      const value_type _nbits;
      const value_type _modulus;
      HASHER _hasher;

    public:
      explicit HasherTransform(value_type nbits, const HASHER &hasher=HASHER()) : _nbits(nbits), _modulus(1 << nbits), _hasher(hasher) { }
      HasherTransform(const HasherTransform &o) : _nbits(o._nbits), _modulus(o._modulus), _hasher(o._hasher) { }
      HasherTransform(const HasherTransform &&o) : _nbits(o._nbits), _modulus(o._modulus), _hasher(o._hasher) { }

      inline value_type modulus(void) const { return _modulus; }
      inline value_type nbits(void) const { return _nbits; }

      inline value_type
      operator ()(const std::string &feature) const {
        return _hasher(feature) % _modulus;
      }

      inline bool
      invert(const std::string &feature) const {
        return _hasher(feature) % 2 == 0;
      }
    };

  }
}

#endif  // SCHWA_LEARN_FEATURE_TRANSFORMERS_H_
