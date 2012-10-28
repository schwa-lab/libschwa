/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "test_utils.h"

#include <type_traits>

#include <schwa/mpl/if.h>

#include <boost/test/unit_test.hpp>

namespace mpl = schwa::mpl;


namespace schwatest {

BOOST_AUTO_TEST_SUITE(schwa__mpl__if)

BOOST_AUTO_TEST_CASE(if_) {
  typedef typename mpl::if_<std::is_integral<int>, std::true_type, std::false_type>::type T1;
  BOOST_CHECK_EQUAL(T1(), true);

  typedef typename mpl::if_<std::is_integral<float>, std::true_type, std::false_type>::type T2;
  BOOST_CHECK_EQUAL(T2(), false);

  typedef typename mpl::if_<std::is_same<float, float>, std::true_type, std::false_type>::type T3;
  BOOST_CHECK_EQUAL(T3(), true);

  typedef typename mpl::if_<std::is_same<float, int>, std::true_type, std::false_type>::type T4;
  BOOST_CHECK_EQUAL(T4(), false);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace schwatest
