/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <algorithm>
#include <sstream>
#include <string>

#include <schwa/learn.h>


namespace schwa {
namespace learn {

SUITE(schwa__learn__features) {

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

  std::stringstream ss;
  f.dump_crfsuite(ss);
  ss.seekg(0);
  const std::string s = ss.str();
  CHECK_EQUAL(4, std::count(s.begin(), s.end(), '\t'));
}


TEST(hasher_transform_default_hash) {
  Features<HasherTransform<>> f(HasherTransform<>(4));
  populate_features(f);

  std::stringstream ss;
  f.dump_crfsuite(ss);
  ss.seekg(0);
  const std::string s = ss.str();
  CHECK_EQUAL(4, std::count(s.begin(), s.end(), '\t'));
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

  std::stringstream ss;
  f.dump_crfsuite(ss);
  ss.seekg(0);
  CHECK_EQUAL("\t10:6.2", ss.str());
}

}  // SUITE

}  // namespace learn
}  // namespace schwa
