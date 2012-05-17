namespace schwa {
  namespace msgpack {

    enum WireType {
      WIRE_FIXNUM_POSITIVE, WIRE_FIXNUM_NEGATIVE,
      WIRE_MAP_FIX, WIRE_MAP_16, WIRE_MAP_32,
      WIRE_ARRAY_FIX, WIRE_ARRAY_16, WIRE_ARRAY_32,
      WIRE_RAW_FIX, WIRE_RAW_16, WIRE_RAW_32,
      WIRE_NIL,
      WIRE_TRUE, WIRE_FALSE,
      WIRE_FLOAT, WIRE_DOUBLE,
      WIRE_UINT_8, WIRE_UINT_16, WIRE_UINT_32, WIRE_UINT_64,
      WIRE_INT_8, WIRE_INT_16, WIRE_INT_32, WIRE_INT_64,
      WIRE_RESERVED
    };

    namespace header {
      static const unsigned char MAP_FIXED   = 0x80;
      static const unsigned char ARRAY_FIXED = 0x90;
      static const unsigned char RAW_FIXED   = 0xa0;
      static const unsigned char NIL      = 0xc0;
      static const unsigned char TRUE     = 0xc2;
      static const unsigned char FALSE    = 0xc3;
      static const unsigned char FLOAT    = 0xca;
      static const unsigned char DOUBLE   = 0xcb;
      static const unsigned char UINT_8   = 0xcc;
      static const unsigned char UINT_16  = 0xcd;
      static const unsigned char UINT_32  = 0xce;
      static const unsigned char UINT_64  = 0xcf;
      static const unsigned char INT_8    = 0xd0;
      static const unsigned char INT_16   = 0xd1;
      static const unsigned char INT_32   = 0xd2;
      static const unsigned char INT_64   = 0xd3;
      static const unsigned char RAW_16   = 0xda;
      static const unsigned char RAW_32   = 0xdb;
      static const unsigned char ARRAY_16 = 0xdc;
      static const unsigned char ARRAY_32 = 0xdd;
      static const unsigned char MAP_16   = 0xde;
      static const unsigned char MAP_32   = 0xdf;
    }

    inline std::ostream &write_nil(std::ostream &out);
    inline std::ostream &write_boolean(std::ostream &out, const bool x);
    inline std::ostream &write_uint(std::ostream &out, const uint64_t x);
    inline std::ostream &write_int(std::ostream &out, const int64_t x);
    inline std::ostream &write_float(std::ostream &out, const float x);
    inline std::ostream &write_double(std::ostream &out, const double x);
    std::ostream &write_array(std::ostream &out, const ArrayObject &arr);
    std::ostream &write_map(std::ostream &out, const MapObject &map);
    std::ostream &write_raw(std::ostream &out, const RawObject &raw);

    std::ostream &write_object(std::ostream &out, const Object &obj);
    std::ostream &write_packed(std::ostream &out, const char *const data, const size_t size);

    inline std::ostream &write_raw_uint16(std::ostream &out, const uint16_t x);
    inline std::ostream &write_raw_uint32(std::ostream &out, const uint32_t x);
    inline std::ostream &write_raw_uint64(std::ostream &out, const uint64_t x);

    inline std::ostream &write_int_fixed(std::ostream &out, const int8_t x);
    inline std::ostream &write_int_8(std::ostream &out, const int8_t x);
    inline std::ostream &write_int_16(std::ostream &out, const int16_t x);
    inline std::ostream &write_int_32(std::ostream &out, const int32_t x);
    inline std::ostream &write_int_64(std::ostream &out, const int64_t x);

    inline std::ostream &write_uint_fixed(std::ostream &out, const uint8_t x);
    inline std::ostream &write_uint_8(std::ostream &out, const uint8_t x);
    inline std::ostream &write_uint_16(std::ostream &out, const uint16_t x);
    inline std::ostream &write_uint_32(std::ostream &out, const uint32_t x);
    inline std::ostream &write_uint_64(std::ostream &out, const uint64_t x);


    // ========================================================================
    // Inline implementations for speed benifit
    // ========================================================================
    template <typename T>
    inline std::ostream &
    write_raw_8(std::ostream &out, const T _x) {
      const unsigned char *x = reinterpret_cast<const unsigned char *>(&_x);
      return out.put(x[0]);
    }

    template <typename T>
    inline std::ostream &
    write_raw_16(std::ostream &out, const T _x) {
      const unsigned char *x = reinterpret_cast<const unsigned char *>(&_x);
      out.put(x[1]);
      return out.put(x[0]);
    }

    template <typename T>
    inline std::ostream &
    write_raw_32(std::ostream &out, const T _x) {
      const unsigned char *x = reinterpret_cast<const unsigned char *>(&_x);
      out.put(x[3]);
      out.put(x[2]);
      out.put(x[1]);
      return out.put(x[0]);
    }

    template <typename T>
    inline std::ostream &
    write_raw_64(std::ostream &out, const T _x) {
      const unsigned char *x = reinterpret_cast<const unsigned char *>(&_x);
      out.put(x[7]);
      out.put(x[6]);
      out.put(x[5]);
      out.put(x[4]);
      out.put(x[3]);
      out.put(x[2]);
      out.put(x[1]);
      return out.put(x[0]);
    }

    inline std::ostream &write_raw_uint8(std::ostream &out, const uint8_t x) { return write_raw_8<uint8_t>(out, x); }
    inline std::ostream &write_raw_uint16(std::ostream &out, const uint16_t x) { return write_raw_16<uint16_t>(out, x); }
    inline std::ostream &write_raw_uint32(std::ostream &out, const uint32_t x) { return write_raw_32<uint32_t>(out, x); }
    inline std::ostream &write_raw_uint64(std::ostream &out, const uint64_t x) { return write_raw_64<uint64_t>(out, x); }

    inline std::ostream &write_raw_int8(std::ostream &out, const int8_t x) { return write_raw_8<int8_t>(out, x); }
    inline std::ostream &write_raw_int16(std::ostream &out, const int16_t x) { return write_raw_16<int16_t>(out, x); }
    inline std::ostream &write_raw_int32(std::ostream &out, const int32_t x) { return write_raw_32<int32_t>(out, x); }
    inline std::ostream &write_raw_int64(std::ostream &out, const int64_t x) { return write_raw_64<int64_t>(out, x); }

    inline std::ostream &
    write_uint_fixed(std::ostream &out, const uint8_t x) {
      return write_raw_uint8(out, x & 0x7F);
    }

    inline std::ostream &
    write_uint_8(std::ostream &out, const uint8_t x) {
      out.put(header::UINT_8);
      return write_raw_uint8(out, x);
    }

    inline std::ostream &
    write_uint_16(std::ostream &out, const uint16_t x) {
      out.put(header::UINT_16);
      return write_raw_uint16(out, x);
    }

    inline std::ostream &
    write_uint_32(std::ostream &out, const uint32_t x) {
      out.put(header::UINT_32);
      return write_raw_uint32(out, x);
    }

    inline std::ostream &
    write_uint_64(std::ostream &out, const uint64_t x) {
      out.put(header::UINT_64);
      return write_raw_uint64(out, x);
    }

    inline std::ostream &
    write_int_fixed(std::ostream &out, const int8_t x) {
      return write_raw_uint8(out, ~(*reinterpret_cast<const uint8_t *>(&x)) & 0xE0);
    }

    inline std::ostream &
    write_int_8(std::ostream &out, const int8_t x) {
      out.put(header::INT_8);
      return write_raw_int8(out, x);
    }

    inline std::ostream &
    write_int_16(std::ostream &out, const int16_t x) {
      out.put(header::INT_16);
      return write_raw_int16(out, x);
    }

    inline std::ostream &
    write_int_32(std::ostream &out, const int32_t x) {
      out.put(header::INT_32);
      return write_raw_int32(out, x);
    }

    inline std::ostream &
    write_int_64(std::ostream &out, const int64_t x) {
      out.put(header::INT_64);
      return write_raw_int64(out, x);
    }

    // ========================================================================
    // ObjectType writing
    // ========================================================================
    inline std::ostream &
    write_nil(std::ostream &out) {
      return out.put(header::NIL);
    }

    inline std::ostream &
    write_boolean(std::ostream &out, const bool x) {
      return out.put(x ? header::TRUE : header::FALSE);
    }

    inline std::ostream &
    write_uint(std::ostream &out, const uint64_t x) {
      if (x <= 127)
        return write_uint_fixed(out, static_cast<uint8_t>(x));
      else if (x <= std::numeric_limits<uint8_t>::max())
        return write_uint_8(out, static_cast<uint8_t>(x));
      else if (x <= std::numeric_limits<uint16_t>::max())
        return write_uint_16(out, static_cast<uint16_t>(x));
      else if (x <= std::numeric_limits<uint32_t>::max())
        return write_uint_32(out, static_cast<uint32_t>(x));
      else
        return write_uint_64(out, x);
    }

    inline std::ostream &
    write_int(std::ostream &out, const int64_t x) {
      if (x >= -32 && x <= -1)
        return write_int_fixed(out, static_cast<int8_t>(x));
      else if (x >= std::numeric_limits<int8_t>::min() && x <= std::numeric_limits<int8_t>::max())
        return write_int_8(out, static_cast<int8_t>(x));
      else if (x >= std::numeric_limits<int16_t>::min() && x <= std::numeric_limits<int16_t>::max())
        return write_int_16(out, static_cast<int16_t>(x));
      else if (x >= std::numeric_limits<int32_t>::min() && x <= std::numeric_limits<int32_t>::max())
        return write_int_32(out, static_cast<int32_t>(x));
      else
        return write_int_64(out, x);
    }

    inline std::ostream &
    write_float(std::ostream &out, const float x) {
      out.put(header::FLOAT);
      return write_raw_32(out, x);
    }

    inline std::ostream &
    write_double(std::ostream &out, const double x) {
      out.put(header::DOUBLE);
      return write_raw_64(out, x);
    }

  }
}
