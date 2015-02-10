/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <algorithm>
#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <schwa/learn.h>


namespace schwa {
namespace learn {

SUITE(schwa__learn__features) {

template <typename TRANSFORM>
static std::vector<std::string>
_get_attributes(const Features<TRANSFORM> &f) {
  std::vector<std::string> units;
  for (const auto &pair : f) {
    std::ostringstream ss;
    ss << pair.first << ":" << pair.second;
    units.push_back(ss.str());
  }
  std::sort(units.begin(), units.end());
  return units;
}


template <typename F>
static void
populate_features(F &f) {
  f("a");
  f("b");
  f("c");
  f("d", 3.2);
}


TEST(no_transform) {
  Features<> f;
  populate_features(f);

  std::vector<std::string> attributes = _get_attributes(f);
  CHECK_EQUAL(4, attributes.size());
  CHECK_EQUAL("a:1", attributes[0]);
  CHECK_EQUAL("b:1", attributes[1]);
  CHECK_EQUAL("c:1", attributes[2]);
  CHECK_EQUAL("d:3.2", attributes[3]);
}


TEST(hasher_transform_default_hash) {
  Features<HasherTransform<>> f(HasherTransform<>(4));
  populate_features(f);

  std::vector<std::string> attributes = _get_attributes(f);
  CHECK_EQUAL(4, attributes.size());
  CHECK_EQUAL("0:1", attributes[0]);
  CHECK_EQUAL("10:1", attributes[1]);
  CHECK_EQUAL("13:1", attributes[2]);
  CHECK_EQUAL("9:3.2", attributes[3]);
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

  Features<HasherTransform<MyHash>> f(HasherTransform<MyHash>(4));
  populate_features(f);

  std::vector<std::string> attributes = _get_attributes(f);
  CHECK_EQUAL(1, attributes.size());
  CHECK_EQUAL("10:6.2", attributes[0]);
}

}  // SUITE

}  // namespace learn
}  // namespace schwa
