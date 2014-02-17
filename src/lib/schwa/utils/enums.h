/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_ENUMS_H_
#define SCHWA_UTILS_ENUMS_H_

#include <type_traits>

namespace schwa {

  template <typename E>
  inline constexpr typename std::underlying_type<E>::type
  to_underlying(E e) {
    static_assert(std::is_enum<E>::value, "must be used with an enum");
    return static_cast<typename std::underlying_type<E>::type>(e);
  }

}

#endif  // SCHWA_UTILS_ENUMS_H_
