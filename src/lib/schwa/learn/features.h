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

    enum class FeatureStorage : uint8_t {
      UNSPECIFIED, DENSE, SPARSE,
    };


    class FeatureType {
    private:
      const uint8_t _id;
      FeatureStorage _storage;

    public:
      explicit FeatureType(uint8_t id, FeatureStorage storage=FeatureStorage::UNSPECIFIED) : _id(id), _storage(storage) { }

      inline uint8_t id(void) const { return _id; }
      inline bool is_dense(void) const { return _storage == FeatureStorage::DENSE; }
      inline bool is_sparse(void) const { return _storage == FeatureStorage::SPARSE; }
      inline bool is_unspecified(void) const { return _storage == FeatureStorage::UNSPECIFIED; }
      inline FeatureStorage storage(void) const { return _storage; }

      inline void set_storage(FeatureStorage storage) { _storage = storage; }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(FeatureType);
    };


    using Label = uint16_t;


    template <typename ENTRY, size_t INDEX_BITS>
    class FeatureHashtable;


    void dump_crfsuite_feature(const std::string &feature, std::ostream &out);


    template <class TRANSFORM=NoTransform, typename VALUE=float>
    class Features {
    public:
      using transform_type = TRANSFORM;
      using key_type = typename TRANSFORM::value_type;
      using value_type = VALUE;

      using underlying_type = std::unordered_map<key_type, value_type>;
      using const_iterator = typename underlying_type::const_iterator;

    protected:
      const transform_type &_transformer;
      underlying_type _values;

    public:
      explicit Features(const transform_type &transformer=transform_type()) : _transformer(transformer) { }
      Features(const Features &o) : _transformer(o._transformer), _values(o._values) { }
      Features(const Features &&o) : _transformer(o._transformer), _values(o._values) { }

      const_iterator begin(void) const { return _values.begin(); }
      const_iterator end(void) const { return _values.end(); }
      const_iterator cbegin(void) const { return _values.cbegin(); }
      const_iterator cend(void) const { return _values.cend(); }

      inline value_type
      operator ()(const std::string &key, const value_type delta=1.0f) {
        return _values[_transformer(key)] += delta;
      }

      inline void clear(void) { _values.clear(); }
      void dump_crfsuite(std::ostream &out) const;
    };


    template <class TRANSFORM=NoTransform>
    class Instance {
    public:
      Features<TRANSFORM> features;
      std::string klass;

      explicit Instance(const TRANSFORM &transformer) : features(transformer) { }
      Instance(const Instance &o) : features(o.features), klass(o.klass) { }
      Instance(const Instance &&o) : features(o.features), klass(o.klass) { }

      void dump_crfsuite(std::ostream &out) const;
    };

  }
}

#include <schwa/learn/features_impl.h>

#endif  // SCHWA_LEARN_FEATURES_H_
