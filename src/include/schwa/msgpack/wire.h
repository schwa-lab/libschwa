/* -*- Mode: C++; indent-tabs-mode: nil -*- */

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
      static const unsigned char FALSE    = 0xc2;
      static const unsigned char TRUE     = 0xc3;
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


    // ========================================================================
    // Reading API
    // ========================================================================
    inline WireType read_peek(std::istream &in);

    inline void     read_nil(std::istream &in);
    inline bool     read_boolean(std::istream &in);
    inline float    read_float(std::istream &in);
    inline double   read_double(std::istream &in);
    inline int64_t  read_int(std::istream &in);
    inline uint64_t read_uint(std::istream &in);

    inline size_t   read_array_header(std::istream &in);
    inline size_t   read_map_header(std::istream &in);

    inline int8_t   read_int_fixed(std::istream &in);
    inline int8_t   read_int_8(std::istream &in);
    inline int16_t  read_int_16(std::istream &in);
    inline int32_t  read_int_32(std::istream &in);
    inline int64_t  read_int_64(std::istream &in);

    inline uint8_t  read_uint_fixed(std::istream &in);
    inline uint8_t  read_uint_8(std::istream &in);
    inline uint16_t read_uint_16(std::istream &in);
    inline uint32_t read_uint_32(std::istream &in);
    inline uint64_t read_uint_64(std::istream &in);


    // ========================================================================
    // Writing API
    // ========================================================================
    inline std::ostream &write_nil(std::ostream &out);
    inline std::ostream &write_boolean(std::ostream &out, const bool x);
    inline std::ostream &write_int(std::ostream &out, const int64_t x);
    inline std::ostream &write_uint(std::ostream &out, const uint64_t x);
    inline std::ostream &write_float(std::ostream &out, const float x);
    inline std::ostream &write_double(std::ostream &out, const double x);

    inline std::ostream &write_array_header(std::ostream &out, const size_t size);
    inline std::ostream &write_map_header(std::ostream &out, const size_t size);

    //std::ostream &write_array(std::ostream &out, const ArrayObject &arr);
    //std::ostream &write_map(std::ostream &out, const MapObject &map);
    //inline std::ostream &write_raw(std::ostream &out, const RawObject &raw);
    //std::ostream &write_object(std::ostream &out, const Object &obj);

    std::ostream &write_raw(std::ostream &out, const char *const data, const size_t size);

    std::ostream &write_packed(std::ostream &out, const char *const data, const size_t size);

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


    template <typename T>
    inline std::ostream &write(std::ostream &out, const T &val);

    template <> inline std::ostream &write(std::ostream &out, const int8_t &val) { return write_int_8(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const int16_t &val) { return write_int_16(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const int32_t &val) { return write_int_32(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const int64_t &val) { return write_int_64(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const uint8_t &val) { return write_uint_8(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const uint16_t &val) { return write_uint_16(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const uint32_t &val) { return write_uint_32(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const uint64_t &val) { return write_uint_64(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const float &val) { return write_float(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const double &val) { return write_double(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const bool &val) { return write_boolean(out, val); }
    template <> inline std::ostream &write(std::ostream &out, const std::string &val) { return write_raw(out, val.c_str(), val.size()); }


    // ========================================================================
    // Reading API implementations
    // ========================================================================
    template <typename T>
    inline std::istream &
    read_raw_8(std::istream &in, T *_x) {
      char *x = reinterpret_cast<char *>(_x);
      return in.get(x[0]);
    }

    template <typename T>
    inline std::istream &
    read_raw_16(std::istream &in, T *_x) {
      char *x = reinterpret_cast<char *>(_x);
      in.get(x[1]);
      return in.get(x[0]);
    }

    template <typename T>
    inline std::istream &
    read_raw_32(std::istream &in, T *_x) {
      char *x = reinterpret_cast<char *>(_x);
      in.get(x[3]);
      in.get(x[2]);
      in.get(x[1]);
      return in.get(x[0]);
    }

    template <typename T>
    inline std::istream &
    read_raw_64(std::istream &in, T *_x) {
      char *x = reinterpret_cast<char *>(_x);
      in.get(x[7]);
      in.get(x[6]);
      in.get(x[5]);
      in.get(x[4]);
      in.get(x[3]);
      in.get(x[2]);
      in.get(x[1]);
      return in.get(x[0]);
    }

    inline WireType
    read_peek(std::istream &in) {
      const int c = in.peek();
      if ((c >> 7) == 0x00)
        return WIRE_FIXNUM_POSITIVE;
      else if ((c >> 5) == 0x07)
        return WIRE_FIXNUM_NEGATIVE;
      else if ((c >> 4) == 0x08)
        return WIRE_MAP_FIX;
      else if ((c >> 4) == 0x09)
        return WIRE_ARRAY_FIX;
      else if ((c >> 5) == 0x05)
        return WIRE_RAW_FIX;
      else if (c == header::NIL)
        return WIRE_NIL;
      else if (c == header::FALSE)
        return WIRE_FALSE;
      else if (c == header::TRUE)
        return WIRE_TRUE;
      else if (c == header::FLOAT)
        return WIRE_FLOAT;
      else if (c == header::DOUBLE)
        return WIRE_DOUBLE;
      else if (c == header::UINT_8)
        return WIRE_UINT_8;
      else if (c == header::UINT_16)
        return WIRE_UINT_16;
      else if (c == header::UINT_32)
        return WIRE_UINT_32;
      else if (c == header::UINT_64)
        return WIRE_UINT_64;
      else if (c == header::INT_8)
        return WIRE_INT_8;
      else if (c == header::INT_16)
        return WIRE_INT_16;
      else if (c == header::INT_32)
        return WIRE_INT_32;
      else if (c == header::INT_64)
        return WIRE_INT_64;
      else if (c == header::ARRAY_16)
        return WIRE_ARRAY_16;
      else if (c == header::ARRAY_32)
        return WIRE_ARRAY_32;
      else if (c == header::MAP_16)
        return WIRE_MAP_16;
      else if (c == header::MAP_32)
        return WIRE_MAP_32;
      else if (c == header::RAW_16)
        return WIRE_RAW_16;
      else if (c == header::RAW_32)
        return WIRE_RAW_32;
      else
        return WIRE_RESERVED;
    }

    inline void
    read_nil(std::istream &in) {
      const int h = in.get();
      assert(h == header::NIL);
    }

    inline bool
    read_boolean(std::istream &in) {
      const int h = in.get();
      if (h == header::TRUE)
        return true;
      else if (h == header::FALSE)
        return false;
      assert((h == header::TRUE || h == header::FALSE));
      return false;
    }

    inline float
    read_float(std::istream &in) {
      float x;
      const int h = in.get();
      assert(h == header::FLOAT);
      read_raw_32(in, &x);
      return x;
    }

    inline double
    read_double(std::istream &in) {
      double x;
      const int h = in.get();
      assert(h == header::DOUBLE);
      read_raw_64(in, &x);
      return x;
    }

    inline uint8_t
    read_uint_fixed(std::istream &in) {
      int c = in.get();
      assert((c & 0x80) == 0x00);
      return static_cast<uint8_t>(c);
    }

    inline uint8_t
    read_uint_8(std::istream &in) {
      uint8_t x;
      const int h = in.get();
      assert(h == header::UINT_8);
      read_raw_8(in, &x);
      return x;
    }

    inline uint16_t
    read_uint_16(std::istream &in) {
      uint16_t x;
      const int h = in.get();
      assert(h == header::UINT_16);
      read_raw_16(in, &x);
      return x;
    }

    inline uint32_t
    read_uint_32(std::istream &in) {
      uint32_t x;
      const int h = in.get();
      assert(h == header::UINT_32);
      read_raw_32(in, &x);
      return x;
    }

    inline uint64_t
    read_uint_64(std::istream &in) {
      uint64_t x;
      const int h = in.get();
      assert(h == header::UINT_64);
      read_raw_64(in, &x);
      return x;
    }

    inline uint64_t
    read_uint(std::istream &in) {
      const WireType type = read_peek(in);
      switch (type) {
      case WIRE_FIXNUM_POSITIVE: return read_uint_fixed(in);
      case WIRE_UINT_8: return read_uint_8(in);
      case WIRE_UINT_16: return read_uint_16(in);
      case WIRE_UINT_32: return read_uint_32(in);
      case WIRE_UINT_64: return read_uint_64(in);
      default:
        assert(!"Did not find a uint to read");
        return 0;
      }
    }

    // ========================================================================
    // Writing API implementations
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

    inline std::ostream &
    write_array_header(std::ostream &out, const size_t size) {
      if (size <= 15)
        out.put(static_cast<unsigned char>(header::ARRAY_FIXED | size));
      else if (size <= std::numeric_limits<uint16_t>::max()) {
        out.put(header::ARRAY_16);
        write_raw_uint16(out, size);
      }
      else {
        out.put(header::ARRAY_32);
        write_raw_uint32(out, size);
      }
      return out;
    }

    inline std::ostream &
    write_map_header(std::ostream &out, const size_t size) {
      if (size <= 15)
        out.put(static_cast<unsigned char>(header::MAP_FIXED | size));
      else if (size <= std::numeric_limits<uint16_t>::max()) {
        out.put(header::MAP_16);
        write_raw_uint16(out, size);
      }
      else {
        out.put(header::MAP_32);
        write_raw_uint32(out, size);
      }
      return out;
    }

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

    inline std::ostream &
    write_raw(std::ostream &out, const RawObject &raw) {
      return write_raw(out, raw.data, raw.size);
    }

  }
}
