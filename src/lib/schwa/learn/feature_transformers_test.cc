/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <sstream>
#include <string>

#include <schwa/learn.h>


namespace schwa {
namespace learn {

SUITE(schwa__learn__feature_transformers) {

TEST(no_transform) {
  NoTransform t;

  for (unsigned int i = 0; i != 1000; ++i) {
    std::stringstream ss;
    ss << i;
    const auto v = t(ss.str());
    CHECK_EQUAL(ss.str(), v);
  }
}


TEST(hasher_transform_default_hash) {
  HasherTransform<> t(4);
  CHECK_EQUAL(4, t.nbits());
  CHECK_EQUAL(16, t.modulus());

  for (unsigned int i = 0; i != 1000; ++i) {
    std::stringstream ss;
    ss << i;
    const auto v = t(ss.str());
    CHECK(0 <= v);
    CHECK(v < t.modulus());
  }
}


TEST(hasher_transform_custom_hash) {
  struct MyHash {
    using result_type = size_t;
    using argument_type = std::string;

    inline result_type
    operator ()(const argument_type &) const {
      return 42;
    }
  };

  HasherTransform<MyHash> t(5);
  CHECK_EQUAL(5, t.nbits());
  CHECK_EQUAL(32, t.modulus());

  for (unsigned int i = 0; i != 1000; ++i) {
    std::stringstream ss;
    ss << i;
    const auto v = t(ss.str());
    CHECK_EQUAL(42 % t.modulus(), v);
  }
}

}  // SUITE

}  // namespace learn
}  // namespace schwa
