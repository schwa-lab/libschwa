/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_BASE_H_
#define SCHWA_BASE_H_

#include <schwa/std.h>
#include <schwa/port.h>
#include <schwa/exception.h>
#include <schwa/version.h>

namespace schwa {

  template <typename E>
  inline constexpr typename std::underlying_type<E>::type to_underlying(E e) {
    static_assert(std::is_enum<E>::value, "must be used with an enum");
    return static_cast<typename std::underlying_type<E>::type>(e);
  }

}

#endif
