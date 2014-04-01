/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MPL_IF_H_
#define SCHWA_MPL_IF_H_

namespace schwa {
  namespace mpl {

    template <bool, typename T1, typename T2>
    struct if_impl {
      using type = T1;
    };

    template <typename T1, typename T2>
    struct if_impl<false, T1, T2> {
      using type = T2;
    };

    // Defines if_::type to be either T1 or T2 depending on the value of CONDITION.
    // CONDITION is expected to be an instance of std::integral_constant.
    // T1 and T2 can be of any type.
    template <typename CONDITION, typename T1, typename T2>
    struct if_ {
      using type = typename if_impl<CONDITION::value, T1, T2>::type;
    };

  }
}

#endif  // SCHWA_MPL_IF_H_
