/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_EXTRACT_IMPL_H_
#define SCHWA_LEARN_EXTRACT_IMPL_H_

#include <algorithm>
#include <cstdio>
#include <functional>
#include <sstream>
#include <string>

#include <schwa/_base.h>
#include <schwa/dr/fields.h>
#include <schwa/exception.h>
#include <schwa/learn/features.h>


namespace schwa {
  namespace learn {

    template <typename T>
    inline contextual_callback<T>
    create_unigram_callback(void) {
      const auto fn = [](const learn::SentinelOffsets<T> &offsets, size_t i, ptrdiff_t delta) {
        return offsets(i, delta);
      };
      return contextual_callback<T>(fn);
    }


    template <typename T>
    inline contextual_callback<T>
    create_bigram_callback(void) {
      const auto fn = [](const learn::SentinelOffsets<T> &offsets, size_t i, ptrdiff_t delta) {
        return offsets(i, delta) + " " + offsets(i, delta + 1);
      };
      return contextual_callback<T>(fn);
    }


    template <typename T>
    inline contextual_callback<T>
    create_trigram_callback(void) {
      const auto fn = [](const learn::SentinelOffsets<T> &offsets, size_t i, ptrdiff_t delta) {
        return offsets(i, delta) + " " + offsets(i, delta + 1) + " " + offsets(i, delta + 2);
      };
      return contextual_callback<T>(fn);
    }


    template <typename T, typename R, typename TRANSFORM, typename VALUE>
    inline void
    Windower::operator ()(const SentinelOffsets<T> &offsets, contextual_callback<T, R> callback, Features<TRANSFORM, VALUE> &features, const size_t i) const {
      std::stringstream attr;
      for (int8_t delta = _delta_left; delta <= _delta_right; ++delta) {
        attr.str(_prefix);
        attr << callback(offsets, i, delta);
        features(attr.str());
      }
    }


    // ============================================================================
    // Windower2
    // ============================================================================
    template <typename T>
    Windower2<T>::Windower2(const std::string &name, const int8_t delta0, const int8_t delta1, SentinelOffsets<T> &offsets) :
        _delta0(delta0),
        _delta1(delta1),
        _offsets(offsets)
      {
      // Construct the attribute prefix.
      char buffer[256];
      const int nchars = std::snprintf(buffer, sizeof(buffer), "%s[i%+d,i%+d]=", name.c_str(), delta0, delta1);
      if (SCHWA_UNLIKELY(nchars < 0 || static_cast<unsigned int>(nchars) >= sizeof(buffer)))
        throw schwa::ValueException("Feature name was too long for buffer");
      _prefix.assign(buffer);
    }


    template <typename T> template <typename TRANSFORM, typename VALUE>
    inline void
    Windower2<T>::operator ()(Features<TRANSFORM, VALUE> &features, const size_t i) const {
      _attr << _prefix << _offsets(i, _delta0) << ' ' << _offsets(i, _delta1);
      features(_attr.str());
      _attr.str("");
    }


    // ============================================================================
    // Windower3
    // ============================================================================
    template <typename T>
    Windower3<T>::Windower3(const std::string &name, const int8_t delta0, const int8_t delta1, const int8_t delta2, SentinelOffsets<T> &offsets) :
        _delta0(delta0),
        _delta1(delta1),
        _delta2(delta2),
        _offsets(offsets)
      {
      // Construct the attribute prefix.
      char buffer[256];
      const int nchars = std::snprintf(buffer, sizeof(buffer), "%s[i%+d,i%+d,i%+d]=", name.c_str(), delta0, delta1, delta2);
      if (SCHWA_UNLIKELY(nchars < 0 || static_cast<unsigned int>(nchars) >= sizeof(buffer)))
        throw schwa::ValueException("Feature name was too long for buffer");
      _prefix.assign(buffer);
    }


    template <typename T> template <typename TRANSFORM, typename VALUE>
    inline void
    Windower3<T>::operator ()(Features<TRANSFORM, VALUE> &features, const size_t i) const {
      _attr << _prefix << _offsets(i, _delta0) << ' ' << _offsets(i, _delta1) << ' ' << _offsets(i, _delta2);
      features(_attr.str());
      _attr.str("");
    }


    // ============================================================================
    // add_affix_features
    // ============================================================================
    template <typename TRANSFORM, typename VALUE, typename T>
    inline void
    _add_affix_features(Features<TRANSFORM, VALUE> &features, const size_t nprefix, const size_t nsuffix, const T &string) {
      std::stringstream attr;
      UnicodeString affix;
      affix.reserve(std::max(nprefix, nsuffix));

      auto fit = string.cbegin();
      const auto fend = string.cend();
      for (size_t i = 0; i != nprefix; ++i, ++fit) {
        if (fit == fend)
          break;
        affix += *fit;

        attr << "prefix=" << affix;
        features(attr.str());
        attr.str("");
      }
      affix.clear();

      auto rit = string.crbegin();
      const auto rend = string.crend();
      for (size_t i = 0; i != nsuffix; ++i, ++rit) {
        if (rit == rend)
          break;
        affix += *rit;

        attr << "suffix=" << affix;
        features(attr.str());
        attr.str("");
      }
    }

    template <typename TRANSFORM, typename VALUE>
    inline void
    add_affix_features(Features<TRANSFORM, VALUE> &features, const size_t nprefix, const size_t nsuffix, const std::string &s) {
      return _add_affix_features(features, nprefix, nsuffix, UTF8Decoder(s));
    }

    template <typename TRANSFORM, typename VALUE>
    inline void
    add_affix_features(Features<TRANSFORM, VALUE> &features, const size_t nprefix, const size_t nsuffix, const UnicodeString &s) {
      return _add_affix_features(features, nprefix, nsuffix, s);
    }

  }
}

#endif  // SCHWA_LEARN_EXTRACT_IMPL_H_
