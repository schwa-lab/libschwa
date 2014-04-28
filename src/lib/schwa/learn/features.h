/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_FEATURES_H_
#define SCHWA_LEARN_FEATURES_H_

#include <iosfwd>
#include <string>
#include <type_traits>
#include <unordered_map>

#include <schwa/_base.h>
#include <schwa/learn/feature_transformers.h>


namespace schwa {
  namespace learn {

    void dump_crfsuite_feature(const std::string &feature, std::ostream &out);


    template <class TRANSFORM=NoTransform, typename VALUE=float>
    class Features {
    public:
      using transform_type = TRANSFORM;
      using key_type = typename TRANSFORM::value_type;
      using value_type = VALUE;

    protected:
      const transform_type &_transformer;
      std::unordered_map<key_type, value_type> _values;

    public:
      explicit Features(const transform_type &transformer=transform_type()) : _transformer(transformer) { }

      inline value_type
      operator ()(const std::string &key, const value_type delta=1.0f) {
        return _values[_transformer(key)] += delta;
      }

      inline void clear(void) { _values.clear(); }
      void dump_crfsuite(std::ostream &out) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Features);
    };


    template <class TRANSFORM=NoTransform>
    class Instance {
    public:
      Features<TRANSFORM> features;
      std::string klass;

      explicit Instance(const TRANSFORM &transformer) : features(transformer) { }

      void dump_crfsuite(std::ostream &out) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Instance);
    };

  }
}

#include <schwa/learn/features_impl.h>

#endif  // SCHWA_LEARN_FEATURES_H_
