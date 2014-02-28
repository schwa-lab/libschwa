/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <type_traits>

#include <schwa/mpl/if.h>


namespace schwa {
namespace mpl {

SUITE(schwa__mpl__if) {

TEST(if_) {
  typedef typename if_<std::is_integral<int>, std::true_type, std::false_type>::type T1;
  CHECK_EQUAL(true, T1());

  typedef typename if_<std::is_integral<float>, std::true_type, std::false_type>::type T2;
  CHECK_EQUAL(false, T2());

  typedef typename if_<std::is_same<float, float>, std::true_type, std::false_type>::type T3;
  CHECK_EQUAL(true, T3());

  typedef typename if_<std::is_same<float, int>, std::true_type, std::false_type>::type T4;
  CHECK_EQUAL(false, T4());
}

}  // SUITE

}  // namespace mpl
}  // namespace schwa
