/* -*- Mode: C++; indent-tabs-mode: nil -*- */

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

    inline std::ostream &operator <<(std::ostream &out, const WireType &t) { return out << static_cast<uint8_t>(t); }


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
    inline WireType peek_type(std::istream &in);

    inline void     read_nil(std::istream &in);
    inline bool     read_bool(std::istream &in);
    inline float    read_float(std::istream &in);
    inline double   read_double(std::istream &in);
    inline int64_t  read_int(std::istream &in);
    inline uint64_t read_uint(std::istream &in);

    inline size_t   read_array_size(std::istream &in);
    inline size_t   read_map_size(std::istream &in);

    inline std::string read_raw(std::istream &in);

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

    template <typename T>
    inline void read(std::istream &in, T &val);

    template <> inline void read(std::istream &in, int8_t &val) { val = read_int_8(in); }
    template <> inline void read(std::istream &in, int16_t &val) { val = read_int_16(in); }
    template <> inline void read(std::istream &in, int32_t &val) { val = read_int_32(in); }
    template <> inline void read(std::istream &in, int64_t &val) { val = read_int_64(in); }
    template <> inline void read(std::istream &in, uint8_t &val) { val = read_uint_8(in); }
    template <> inline void read(std::istream &in, uint16_t &val) { val = read_uint_16(in); }
    template <> inline void read(std::istream &in, uint32_t &val) { val = read_uint_32(in); }
    template <> inline void read(std::istream &in, uint64_t &val) { val = read_uint_64(in); }
    template <> inline void read(std::istream &in, float &val) { val = read_float(in); }
    template <> inline void read(std::istream &in, double &val) { val = read_double(in); }
    template <> inline void read(std::istream &in, bool &val) { val = read_bool(in); }
    template <> inline void read(std::istream &in, std::string &val) { val = read_raw(in); }


    // ========================================================================
    // Writing API
    // ========================================================================
    inline void write_nil(std::ostream &out);
    inline void write_bool(std::ostream &out, const bool x);
    inline void write_int(std::ostream &out, const int64_t x);
    inline void write_uint(std::ostream &out, const uint64_t x);
    inline void write_float(std::ostream &out, const float x);
    inline void write_double(std::ostream &out, const double x);

    inline void write_array_size(std::ostream &out, const size_t size);
    inline void write_map_size(std::ostream &out, const size_t size);

    inline void write_raw(std::ostream &out, const std::string &data);
    inline void write_raw(std::ostream &out, const char *const data, const size_t size);

    inline void write_int_fixed(std::ostream &out, const int8_t x);
    inline void write_int_8(std::ostream &out, const int8_t x);
    inline void write_int_16(std::ostream &out, const int16_t x);
    inline void write_int_32(std::ostream &out, const int32_t x);
    inline void write_int_64(std::ostream &out, const int64_t x);

    inline void write_uint_fixed(std::ostream &out, const uint8_t x);
    inline void write_uint_8(std::ostream &out, const uint8_t x);
    inline void write_uint_16(std::ostream &out, const uint16_t x);
    inline void write_uint_32(std::ostream &out, const uint32_t x);
    inline void write_uint_64(std::ostream &out, const uint64_t x);


    template <typename T>
    inline void write(std::ostream &out, const T &val);

    template <> inline void write(std::ostream &out, const int8_t &val) { return write_int_8(out, val); }
    template <> inline void write(std::ostream &out, const int16_t &val) { return write_int_16(out, val); }
    template <> inline void write(std::ostream &out, const int32_t &val) { return write_int_32(out, val); }
    template <> inline void write(std::ostream &out, const int64_t &val) { return write_int_64(out, val); }
    template <> inline void write(std::ostream &out, const uint8_t &val) { return write_uint_8(out, val); }
    template <> inline void write(std::ostream &out, const uint16_t &val) { return write_uint_16(out, val); }
    template <> inline void write(std::ostream &out, const uint32_t &val) { return write_uint_32(out, val); }
    template <> inline void write(std::ostream &out, const uint64_t &val) { return write_uint_64(out, val); }
    template <> inline void write(std::ostream &out, const float &val) { return write_float(out, val); }
    template <> inline void write(std::ostream &out, const double &val) { return write_double(out, val); }
    template <> inline void write(std::ostream &out, const bool &val) { return write_bool(out, val); }
    template <> inline void write(std::ostream &out, const std::string &val) { return write_raw(out, val); }


    // ========================================================================
    // Reading API implementations
    // ========================================================================
    template <typename T>
    inline void
    read_bytes_8(std::istream &in, T *_x) {
      char *x = reinterpret_cast<char *>(_x);
      in.get(x[0]);
    }

    template <typename T>
    inline void
    read_bytes_16(std::istream &in, T *_x) {
      uint16_t x;
      in.read(reinterpret_cast<char *>(&x), 2);
      *_x = port::be16_to_h(x);
    }

    template <typename T>
    inline void
    read_bytes_32(std::istream &in, T *_x) {
      uint32_t x;
      in.read(reinterpret_cast<char *>(&x), 4);
      *_x = port::be32_to_h(x);
    }

    template <typename T>
    inline void
    read_bytes_64(std::istream &in, T *_x) {
      uint64_t x;
      in.read(reinterpret_cast<char *>(&x), 8);
      *_x = port::be64_to_h(x);
    }

    inline WireType
    peek_type(std::istream &in) {
      static const WireType TABLE[256] = {
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::MAP_FIXED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::ARRAY_FIXED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::RAW_FIXED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::NIL, WireType::RESERVED, WireType::FALSE, WireType::TRUE, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::FLOAT, WireType::DOUBLE, WireType::UINT_8, WireType::UINT_16, WireType::UINT_32, WireType::UINT_64,
        WireType::INT_8, WireType::INT_16, WireType::INT_32, WireType::INT_64, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RAW_16, WireType::RAW_32, WireType::ARRAY_16, WireType::ARRAY_32, WireType::MAP_16, WireType::MAP_32,
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED,
        WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED
      };
      const int c = in.peek();
      if ((c >> 7) == 0x00)
        return WireType::FIXNUM_POSITIVE;
      else if ((c >> 5) == 0x07)
        return WireType::FIXNUM_NEGATIVE;
      else if ((c >> 4) == 0x08)
        return WireType::MAP_FIXED;
      else if ((c >> 4) == 0x09)
        return WireType::ARRAY_FIXED;
      else if ((c >> 5) == 0x05)
        return WireType::RAW_FIXED;
      else if (c < 0 || c > 255)
        return WireType::RESERVED;
      else
        return TABLE[c & 0xFF];
    }

    inline void
    read_nil(std::istream &in) {
      const int h = in.get();
      assert(h == header::NIL);
    }

    inline bool
    read_bool(std::istream &in) {
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
      read_bytes_32(in, &x);
      return x;
    }

    inline double
    read_double(std::istream &in) {
      double x;
      const int h = in.get();
      assert(h == header::DOUBLE);
      read_bytes_64(in, &x);
      return x;
    }

    inline int8_t
    read_int_fixed(std::istream &in) {
      int c = in.get();
      assert((c & 0xE0) == 0xE0);
      return static_cast<int8_t>(c & 0x1F);
    }

    inline int8_t
    read_int_8(std::istream &in) {
      int8_t x;
      const int h = in.get();
      assert(h == header::INT_8);
      read_bytes_8(in, &x);
      return x;
    }

    inline int16_t
    read_int_16(std::istream &in) {
      int16_t x;
      const int h = in.get();
      assert(h == header::INT_16);
      read_bytes_16(in, &x);
      return x;
    }

    inline int32_t
    read_int_32(std::istream &in) {
      int32_t x;
      const int h = in.get();
      assert(h == header::INT_32);
      read_bytes_32(in, &x);
      return x;
    }

    inline int64_t
    read_int_64(std::istream &in) {
      int64_t x;
      const int h = in.get();
      assert(h == header::INT_64);
      read_bytes_64(in, &x);
      return x;
    }

    inline int64_t
    read_int(std::istream &in) {
      const WireType type = peek_type(in);
      switch (type) {
      case WireType::FIXNUM_NEGATIVE: return read_int_fixed(in);
      case WireType::FIXNUM_POSITIVE: return read_uint_fixed(in);
      case WireType::INT_8: return read_int_8(in);
      case WireType::INT_16: return read_int_16(in);
      case WireType::INT_32: return read_int_32(in);
      case WireType::INT_64: return read_int_64(in);
      default:
        assert(!"Did not find an int to read");
        return 0;
      }
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
      read_bytes_8(in, &x);
      return x;
    }

    inline uint16_t
    read_uint_16(std::istream &in) {
      uint16_t x;
      const int h = in.get();
      assert(h == header::UINT_16);
      read_bytes_16(in, &x);
      return x;
    }

    inline uint32_t
    read_uint_32(std::istream &in) {
      uint32_t x;
      const int h = in.get();
      assert(h == header::UINT_32);
      read_bytes_32(in, &x);
      return x;
    }

    inline uint64_t
    read_uint_64(std::istream &in) {
      uint64_t x;
      const int h = in.get();
      assert(h == header::UINT_64);
      read_bytes_64(in, &x);
      return x;
    }

    inline uint64_t
    read_uint(std::istream &in) {
      const WireType type = peek_type(in);
      switch (type) {
      case WireType::FIXNUM_POSITIVE: return read_uint_fixed(in);
      case WireType::UINT_8: return read_uint_8(in);
      case WireType::UINT_16: return read_uint_16(in);
      case WireType::UINT_32: return read_uint_32(in);
      case WireType::UINT_64: return read_uint_64(in);
      default:
        assert(!"Did not find a uint to read");
        return 0;
      }
    }

    inline size_t
    read_array_size(std::istream &in) {
      const WireType type = peek_type(in);
      const int h = in.get();
      uint16_t s16;
      uint32_t s32;
      switch (type) {
      case WireType::ARRAY_FIXED:
        return h & 0x0F;
      case WireType::ARRAY_16:
        read_bytes_16(in, &s16);
        return s16;
      case WireType::ARRAY_32:
        read_bytes_32(in, &s32);
        return s32;
      default:
        assert(!"header is not an array");
        return 0;
      }
    }

    inline size_t
    read_map_size(std::istream &in) {
      const WireType type = peek_type(in);
      const int h = in.get();
      uint16_t s16;
      uint32_t s32;
      switch (type) {
      case WireType::MAP_FIXED:
        return h & 0x0F;
      case WireType::MAP_16:
        read_bytes_16(in, &s16);
        return s16;
      case WireType::MAP_32:
        read_bytes_32(in, &s32);
        return s32;
      default:
        assert(!"header is not a map");
        return 0;
      }
    }

    inline std::string
    read_raw(std::istream &in) {
      const WireType type = peek_type(in);
      const int h = in.get();
      uint16_t s16;
      uint32_t s32;

      switch (type) {
      case WireType::RAW_FIXED:
        s32 = h & 0x1F;
        break;
      case WireType::MAP_16:
        read_bytes_16(in, &s16);
        s32 = s16;
        break;
      case WireType::MAP_32:
        read_bytes_32(in, &s32);
        break;
      default:
        assert(!"header is not a raw");
        return "";
      }

      std::string s;
      s.resize(s32);
      in.read(&s[0], s32);
      return s;
    }


    // ========================================================================
    // Writing API implementations
    // ========================================================================
    template <typename T>
    inline void
    write_bytes_8(std::ostream &out, const T _x) {
      const unsigned char *x = reinterpret_cast<const unsigned char *>(&_x);
      out.put(x[0]);
    }

    template <typename T>
    inline void
    write_bytes_16(std::ostream &out, const T _x) {
      const uint16_t x = port::h_to_be16(static_cast<uint16_t>(_x));
      out.write(reinterpret_cast<const char *>(&x), 2);
    }

    template <typename T>
    inline void
    write_bytes_32(std::ostream &out, const T _x) {
      const uint32_t x = port::h_to_be32(static_cast<uint32_t>(_x));
      out.write(reinterpret_cast<const char *>(&x), 4);
    }

    template <typename T>
    inline void
    write_bytes_64(std::ostream &out, const T _x) {
      const uint64_t x = port::h_to_be64(static_cast<uint64_t>(_x));
      out.write(reinterpret_cast<const char *>(&x), 8);
    }

    inline void write_raw_uint8(std::ostream &out, const uint8_t x) { write_bytes_8<uint8_t>(out, x); }
    inline void write_raw_uint16(std::ostream &out, const uint16_t x) { write_bytes_16<uint16_t>(out, x); }
    inline void write_raw_uint32(std::ostream &out, const uint32_t x) { write_bytes_32<uint32_t>(out, x); }
    inline void write_raw_uint64(std::ostream &out, const uint64_t x) { write_bytes_64<uint64_t>(out, x); }

    inline void write_raw_int8(std::ostream &out, const int8_t x) { write_bytes_8<int8_t>(out, x); }
    inline void write_raw_int16(std::ostream &out, const int16_t x) { write_bytes_16<int16_t>(out, x); }
    inline void write_raw_int32(std::ostream &out, const int32_t x) { write_bytes_32<int32_t>(out, x); }
    inline void write_raw_int64(std::ostream &out, const int64_t x) { write_bytes_64<int64_t>(out, x); }

    inline void
    write_uint_fixed(std::ostream &out, const uint8_t x) {
      write_raw_uint8(out, x & 0x7F);
    }

    inline void
    write_uint_8(std::ostream &out, const uint8_t x) {
      out.put(header::UINT_8);
      write_raw_uint8(out, x);
    }

    inline void
    write_uint_16(std::ostream &out, const uint16_t x) {
      out.put(header::UINT_16);
      write_raw_uint16(out, x);
    }

    inline void
    write_uint_32(std::ostream &out, const uint32_t x) {
      out.put(header::UINT_32);
      write_raw_uint32(out, x);
    }

    inline void
    write_uint_64(std::ostream &out, const uint64_t x) {
      out.put(header::UINT_64);
      write_raw_uint64(out, x);
    }

    inline void
    write_int_fixed(std::ostream &out, const int8_t x) {
      write_raw_uint8(out, ~(*reinterpret_cast<const uint8_t *>(&x)) & 0xE0);
    }

    inline void
    write_int_8(std::ostream &out, const int8_t x) {
      out.put(header::INT_8);
      write_raw_int8(out, x);
    }

    inline void
    write_int_16(std::ostream &out, const int16_t x) {
      out.put(header::INT_16);
      write_raw_int16(out, x);
    }

    inline void
    write_int_32(std::ostream &out, const int32_t x) {
      out.put(header::INT_32);
      write_raw_int32(out, x);
    }

    inline void
    write_int_64(std::ostream &out, const int64_t x) {
      out.put(header::INT_64);
      write_raw_int64(out, x);
    }

    inline void
    write_array_size(std::ostream &out, const size_t size) {
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
    }

    inline void
    write_map_size(std::ostream &out, const size_t size) {
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
    }

    inline void
    write_nil(std::ostream &out) {
      out.put(header::NIL);
    }

    inline void
    write_bool(std::ostream &out, const bool x) {
      out.put(x ? header::TRUE : header::FALSE);
    }

    inline void
    write_uint(std::ostream &out, const uint64_t x) {
      if (x <= 127)
        write_uint_fixed(out, static_cast<uint8_t>(x));
      else if (x <= std::numeric_limits<uint8_t>::max())
        write_uint_8(out, static_cast<uint8_t>(x));
      else if (x <= std::numeric_limits<uint16_t>::max())
        write_uint_16(out, static_cast<uint16_t>(x));
      else if (x <= std::numeric_limits<uint32_t>::max())
        write_uint_32(out, static_cast<uint32_t>(x));
      else
        write_uint_64(out, x);
    }

    inline void
    write_int(std::ostream &out, const int64_t x) {
      if (x >= -32 && x <= -1)
        write_int_fixed(out, static_cast<int8_t>(x));
      else if (x >= 0 && x <= 127)
        write_uint_fixed(out, static_cast<uint8_t>(x));
      else if (x >= std::numeric_limits<int8_t>::min() && x <= std::numeric_limits<int8_t>::max())
        write_int_8(out, static_cast<int8_t>(x));
      else if (x >= std::numeric_limits<int16_t>::min() && x <= std::numeric_limits<int16_t>::max())
        write_int_16(out, static_cast<int16_t>(x));
      else if (x >= std::numeric_limits<int32_t>::min() && x <= std::numeric_limits<int32_t>::max())
        write_int_32(out, static_cast<int32_t>(x));
      else
        write_int_64(out, x);
    }

    inline void
    write_float(std::ostream &out, const float x) {
      out.put(header::FLOAT);
      write_bytes_32(out, x);
    }

    inline void
    write_double(std::ostream &out, const double x) {
      out.put(header::DOUBLE);
      write_bytes_64(out, x);
    }

    inline void
    write_raw(std::ostream &out, const char *const data, const size_t size) {
      if (size <= 31)
        out.put(static_cast<unsigned char>(header::RAW_FIXED | size));
      else if (size <= std::numeric_limits<uint16_t>::max()) {
        out.put(header::RAW_16);
        write_raw_uint16(out, size);
      }
      else {
        out.put(header::RAW_32);
        write_raw_uint32(out, size);
      }
      out.write(data, size);
    }

    inline void
    write_raw(std::ostream &out, const std::string &data) {
      write_raw(out, data.c_str(), data.size());
    }
  }
}
