/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_ENUMS_H_
#define SCHWA_MSGPACK_ENUMS_H_

#include <schwa/_base.h>

namespace schwa {
  namespace msgpack {

    enum class WireType : uint8_t {
      FIXNUM_POSITIVE, FIXNUM_NEGATIVE,
      MAP_FIXED, MAP_16, MAP_32,
      ARRAY_FIXED, ARRAY_16, ARRAY_32,
      RAW_FIXED, RAW_16, RAW_32,
      NIL,
      TRUE, FALSE,
      FLOAT, DOUBLE,
      UINT_8, UINT_16, UINT_32, UINT_64,
      INT_8, INT_16, INT_32, INT_64,
      RESERVED
    };

  }
}

#endif  // SCHWA_MSGPACK_ENUMS_H_
