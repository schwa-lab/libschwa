/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_ENUMS_H_
#define SCHWA_MSGPACK_ENUMS_H_

#include <iosfwd>

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

    std::ostream &operator <<(std::ostream &out, const WireType type);

    inline bool
    is_array(const WireType type) {
      return type == WireType::ARRAY_FIXED ||
             type == WireType::ARRAY_16 ||
             type == WireType::ARRAY_32;
    }

    inline bool
    is_bool(const WireType type) {
      return type == WireType::FALSE ||
             type == WireType::TRUE;
    }

    inline bool
    is_double(const WireType type) {
      return type == WireType::DOUBLE;
    }

    inline bool
    is_float(const WireType type) {
      return type == WireType::FLOAT;
    }

    inline bool
    is_map(const WireType type) {
      return type == WireType::MAP_FIXED ||
             type == WireType::MAP_16 ||
             type == WireType::MAP_32;
    }

    inline bool
    is_nil(const WireType type) {
      return type == WireType::NIL;
    }

    inline bool
    is_raw(const WireType type) {
      return type == WireType::RAW_FIXED ||
             type == WireType::RAW_16 ||
             type == WireType::RAW_32;
    }

    inline bool
    is_sint(const WireType type) {
      return type == WireType::FIXNUM_NEGATIVE ||
             type == WireType::INT_8 ||
             type == WireType::INT_16 ||
             type == WireType::INT_32 ||
             type == WireType::INT_64;
    }

    inline bool
    is_uint(const WireType type) {
      return type == WireType::FIXNUM_POSITIVE ||
             type == WireType::UINT_8 ||
             type == WireType::UINT_16 ||
             type == WireType::UINT_32 ||
             type == WireType::UINT_64;
    }

    inline bool
    is_int(const WireType type) {
      return is_uint(type) || is_int(type);
    }

  }
}

#endif  // SCHWA_MSGPACK_ENUMS_H_
