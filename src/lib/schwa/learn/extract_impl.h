/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_EXTRACT_IMPL_H_
#define SCHWA_LEARN_EXTRACT_IMPL_H_

#include <functional>
#include <sstream>
#include <string>

#include <schwa/_base.h>
#include <schwa/dr/fields.h>
#include <schwa/learn/features.h>


namespace schwa {
  namespace learn {

    template <typename T, typename R, class TRANSFORM>
    void
    window(const std::string &name, const size_t i, const ptrdiff_t dl, const ptrdiff_t dr, const SentinelOffsets<T> &offsets, Features<TRANSFORM> &features, std::function<R(const SentinelOffsets<T> &, size_t, ptrdiff_t)> callback) {
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

  }
}

#endif  // SCHWA_LEARN_EXTRACT_IMPL_H_
