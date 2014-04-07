/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <type_traits>

#include <schwa/mpl/if.h>


namespace schwa {
namespace mpl {

SUITE(schwa__mpl__if) {

TEST(if_) {
  using T1 = typename if_<std::is_integral<int>, std::true_type, std::false_type>::type;
  CHECK_EQUAL(true, T1());

  using T2 = typename if_<std::is_integral<float>, std::true_type, std::false_type>::type;
  CHECK_EQUAL(false, T2());

  using T3 = typename if_<std::is_same<float, float>, std::true_type, std::false_type>::type;
  CHECK_EQUAL(true, T3());

  using T4 = typename if_<std::is_same<float, int>, std::true_type, std::false_type>::type;
  CHECK_EQUAL(false, T4());
}

}  // SUITE

}  // namespace mpl
}  // namespace schwa
