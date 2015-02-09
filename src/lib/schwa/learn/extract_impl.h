/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_EXTRACT_IMPL_H_
#define SCHWA_LEARN_EXTRACT_IMPL_H_

#include <algorithm>
#include <functional>
#include <sstream>
#include <string>

#include <schwa/_base.h>
#include <schwa/dr/fields.h>
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


    template <typename T, typename R, class TRANSFORM>
    inline void
    window(const std::string &name, const size_t i, const ptrdiff_t dl, const ptrdiff_t dr, const SentinelOffsets<T> &offsets, Features<TRANSFORM> &features, contextual_callback<T, R> callback) {
      std::stringstream key;
      for (ptrdiff_t delta = dl; delta <= dr; ++delta) {
        key << name << "[i";
        if (delta < 0)
          key << delta;
        else if (delta > 0)
          key << "+" << delta;
        key << "]=";
        key << callback(offsets, i, delta);
        features(key.str());
        key.str("");
      }
    }


    template <class TRANSFORM, typename VALUE, typename T>
    inline void
    _add_affix_features(Features<TRANSFORM, VALUE> &features, const size_t nprefix, const size_t nsuffix, const T &string) {
      std::stringstream key;
      UnicodeString affix;
      affix.reserve(std::max(nprefix, nsuffix));

      auto fit = string.cbegin();
      const auto fend = string.cend();
      for (size_t i = 0; i != nprefix; ++i, ++fit) {
        if (fit == fend)
          break;
        affix += *fit;

        key << "prefix=" << affix;
        features(key.str());
        key.str("");
      }
      affix.clear();

      auto rit = string.crbegin();
      const auto rend = string.crend();
      for (size_t i = 0; i != nsuffix; ++i, ++rit) {
        if (rit == rend)
          break;
        affix += *rit;

        key << "suffix=" << affix;
        features(key.str());
        key.str("");
      }
    }

    template <class TRANSFORM, typename VALUE>
    inline void
    add_affix_features(Features<TRANSFORM, VALUE> &features, const size_t nprefix, const size_t nsuffix, const std::string &s) {
      return _add_affix_features(features, nprefix, nsuffix, UTF8Decoder(s));
    }

    template <class TRANSFORM, typename VALUE>
    inline void
    add_affix_features(Features<TRANSFORM, VALUE> &features, const size_t nprefix, const size_t nsuffix, const UnicodeString &s) {
      return _add_affix_features(features, nprefix, nsuffix, s);
    }

  }
}

#endif  // SCHWA_LEARN_EXTRACT_IMPL_H_
