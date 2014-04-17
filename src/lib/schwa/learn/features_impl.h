/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_FEATURES_IMPL_H_
#define SCHWA_LEARN_FEATURES_IMPL_H_

#include <iostream>
#include <sstream>
#include <string>

#include <schwa/_base.h>
#include <schwa/learn/feature_transformers.h>


namespace schwa {
  namespace learn {

    template <class TRANSFORM, typename VALUE>
    inline void
    Features<TRANSFORM, VALUE>::dump_crfsuite(std::ostream &out) const {
      for (auto &pair : _values) {
        out << '\t';

        std::stringstream key;
        key << pair.first;
        dump_crfsuite_feature(key.str(), out);

        out << ":" << pair.second;
      }
    }


  }
}

#endif  // SCHWA_LEARN_FEATURES_IMPL_H_
