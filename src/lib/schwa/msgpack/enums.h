/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_ENUMS_H_
#define SCHWA_MSGPACK_ENUMS_H_

#include <iostream>

#include <schwa/_base.h>


namespace schwa {
  namespace msgpack {

    enum class WireType : uint8_t {
      FIXINT_POSITIVE, UINT_8, UINT_16, UINT_32, UINT_64,
      FIXINT_NEGATIVE, INT_8, INT_16, INT_32, INT_64,
      MAP_FIXED, MAP_16, MAP_32,
      ARRAY_FIXED, ARRAY_16, ARRAY_32,
      STR_FIXED, STR_8, STR_16, STR_32,
      BIN_8, BIN_16, BIN_32,
      NIL,
      TRUE, FALSE,
      FLOAT, DOUBLE,
      EXT_8, EXT_16, EXT_32,
      EXT_FIXED_8, EXT_FIXED_16, EXT_FIXED_32, EXT_FIXED_64, EXT_FIXED_128,
      RESERVED,
    };

    inline std::ostream &
    operator <<(std::ostream &out, const WireType type) {
      return out << static_cast<int>(type);
    }


    inline bool
    is_array(const WireType type) {
      return WireType::ARRAY_FIXED <= type && type <= WireType::ARRAY_32;
    }

    inline bool
    is_bin(const WireType type) {
      return WireType::BIN_8 <= type && type <= WireType::BIN_32;
    }

    inline bool
    is_bool(const WireType type) {
      return type == WireType::FALSE || type == WireType::TRUE;
    }

    inline bool
    is_double(const WireType type) {
      return type == WireType::DOUBLE;
    }

    inline bool
    is_ext(const WireType type) {
      return WireType::EXT_8 <= type && type <= WireType::EXT_FIXED_128;
    }

    inline bool
    is_float(const WireType type) {
      return type == WireType::FLOAT;
    }

    inline bool
    is_map(const WireType type) {
      return WireType::MAP_FIXED <= type && type <= WireType::MAP_32;
    }

    inline bool
    is_nil(const WireType type) {
      return type == WireType::NIL;
    }

    inline bool
    is_reserved(const WireType type) {
      return type == WireType::RESERVED;
    }

    inline bool
    is_sint(const WireType type) {
      return WireType::FIXINT_NEGATIVE <= type && type <= WireType::INT_64;
    }

    inline bool
    is_str(const WireType type) {
      return WireType::STR_FIXED <= type && type <= WireType::STR_32;
    }

    inline bool
    is_uint(const WireType type) {
      return WireType::FIXINT_POSITIVE <= type && type <= WireType::UINT_64;
    }

    inline bool
    is_int(const WireType type) {
      return is_uint(type) || is_int(type);
    }

  }
}

#endif  // SCHWA_MSGPACK_ENUMS_H_
