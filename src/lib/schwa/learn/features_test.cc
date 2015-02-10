/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <algorithm>
#include <functional>
#include <map>
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

  std::map<unsigned int, double> unique;
  std::hash<std::string> hasher;
  unique[(hasher("a") % (1 << 4))] += 1;
  unique[(hasher("b") % (1 << 4))] += 1;
  unique[(hasher("c") % (1 << 4))] += 1;
  unique[(hasher("d") % (1 << 4))] += 3.2;

  std::vector<std::string> expected;
  for (const auto &pair : unique) {
    std::ostringstream tmp;
    tmp << pair.first << ":" << pair.second;
    expected.push_back(tmp.str());
  }
  std::sort(expected.begin(), expected.end());

  std::vector<std::string> attributes = _get_attributes(f);
  CHECK_EQUAL(expected.size(), attributes.size());
  for (size_t i = 0; i != std::min(expected.size(), attributes.size()); ++i)
    CHECK_EQUAL(expected[i], attributes[i]);
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
