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

    extern const WireType TABLE[256];


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
    inline WireType header_type(const int header);

    inline void     read_nil(std::istream &in);
    inline bool     read_bool(std::istream &in);
    inline float    read_float(std::istream &in);
    inline double   read_double(std::istream &in);
    inline int64_t  read_int(std::istream &in);
    inline uint64_t read_uint(std::istream &in);

    inline size_t   read_array_size(std::istream &in);
    inline size_t   read_map_size(std::istream &in);

    inline std::string read_raw(std::istream &in);

    inline int8_t   read_val_int_fixed(const int h);
    inline int8_t   read_val_int8(std::istream &in);
    inline int16_t  read_val_int16(std::istream &in);
    inline int32_t  read_val_int32(std::istream &in);
    inline int64_t  read_val_int64(std::istream &in);

    inline uint8_t  read_val_uint_fixed(const int h);
    inline uint8_t  read_val_uint8(std::istream &in);
    inline uint16_t read_val_uint16(std::istream &in);
    inline uint32_t read_val_uint32(std::istream &in);
    inline uint64_t read_val_uint64(std::istream &in);

    inline int8_t   read_int_fixed(std::istream &in);
    inline int8_t   read_int8(std::istream &in);
    inline int16_t  read_int16(std::istream &in);
    inline int32_t  read_int32(std::istream &in);
    inline int64_t  read_int64(std::istream &in);

    inline uint8_t  read_uint_fixed(std::istream &in);
    inline uint8_t  read_uint8(std::istream &in);
    inline uint16_t read_uint16(std::istream &in);
    inline uint32_t read_uint32(std::istream &in);
    inline uint64_t read_uint64(std::istream &in);

    template <typename T>
    inline void read(std::istream &in, T &val);

    template <> inline void read(std::istream &in, int8_t &val) { val = read_int8(in); }
    template <> inline void read(std::istream &in, int16_t &val) { val = read_int16(in); }
    template <> inline void read(std::istream &in, int32_t &val) { val = read_int32(in); }
    template <> inline void read(std::istream &in, int64_t &val) { val = read_int64(in); }
    template <> inline void read(std::istream &in, uint8_t &val) { val = read_uint8(in); }
    template <> inline void read(std::istream &in, uint16_t &val) { val = read_uint16(in); }
    template <> inline void read(std::istream &in, uint32_t &val) { val = read_uint32(in); }
    template <> inline void read(std::istream &in, uint64_t &val) { val = read_uint64(in); }
    template <> inline void read(std::istream &in, float &val) { val = read_float(in); }
    template <> inline void read(std::istream &in, double &val) { val = read_double(in); }
    template <> inline void read(std::istream &in, bool &val) { val = read_bool(in); }
    template <> inline void read(std::istream &in, std::string &val) { val = read_raw(in); }

    bool read_lazy(std::istream &in, std::ostream &out, WireType &type);

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
    inline void write_int8(std::ostream &out, const int8_t x);
    inline void write_int16(std::ostream &out, const int16_t x);
    inline void write_int32(std::ostream &out, const int32_t x);
    inline void write_int64(std::ostream &out, const int64_t x);

    inline void write_uint_fixed(std::ostream &out, const uint8_t x);
    inline void write_uint8(std::ostream &out, const uint8_t x);
    inline void write_uint16(std::ostream &out, const uint16_t x);
    inline void write_uint32(std::ostream &out, const uint32_t x);
    inline void write_uint64(std::ostream &out, const uint64_t x);


    template <typename T>
    inline void write(std::ostream &out, const T &val);

    template <> inline void write(std::ostream &out, const int8_t &val) { return write_int8(out, val); }
    template <> inline void write(std::ostream &out, const int16_t &val) { return write_int16(out, val); }
    template <> inline void write(std::ostream &out, const int32_t &val) { return write_int32(out, val); }
    template <> inline void write(std::ostream &out, const int64_t &val) { return write_int64(out, val); }
    template <> inline void write(std::ostream &out, const uint8_t &val) { return write_uint8(out, val); }
    template <> inline void write(std::ostream &out, const uint16_t &val) { return write_uint16(out, val); }
    template <> inline void write(std::ostream &out, const uint32_t &val) { return write_uint32(out, val); }
    template <> inline void write(std::ostream &out, const uint64_t &val) { return write_uint64(out, val); }
    template <> inline void write(std::ostream &out, const float &val) { return write_float(out, val); }
    template <> inline void write(std::ostream &out, const double &val) { return write_double(out, val); }
    template <> inline void write(std::ostream &out, const bool &val) { return write_bool(out, val); }
    template <> inline void write(std::ostream &out, const std::string &val) { return write_raw(out, val); }


    // ========================================================================
    // Reading API implementations
    // ========================================================================
    template <typename T>
    inline void
    read_bytes8(std::istream &in, T &x) {
      uint8_t tmp;
      in.read(reinterpret_cast<char *>(&tmp), 1);
      x = tmp;
    }

    template <typename T>
    inline void
    read_bytes16(std::istream &in, T &x) {
      uint16_t tmp;
      in.read(reinterpret_cast<char *>(&tmp), 2);
      x = port::be16_to_h(tmp);
    }

    template <typename T>
    inline void
    read_bytes32(std::istream &in, T &x) {
      uint32_t tmp;
      in.read(reinterpret_cast<char *>(&tmp), 4);
      x = port::be32_to_h(tmp);
    }

    template <typename T>
    inline void
    read_bytes64(std::istream &in, T &x) {
      uint64_t tmp;
      in.read(reinterpret_cast<char *>(&tmp), 8);
      x = port::be64_to_h(tmp);
    }

    inline WireType
    header_type(const int header) {
      if (header < 0 || header > 255)
        return WireType::RESERVED;
      return TABLE[header & 0xFF];
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
      read_bytes32(in, x);
      return x;
    }

    inline double
    read_double(std::istream &in) {
      double x;
      const int h = in.get();
      assert(h == header::DOUBLE);
      read_bytes64(in, x);
      return x;
    }

    inline int8_t
    read_val_int_fixed(const int h) {
      return static_cast<int8_t>(h & 0x1F);
    }

    inline int8_t
    read_val_int8(std::istream &in) {
      int8_t x;
      read_bytes8(in, x);
      return x;
    }

    inline int16_t
    read_val_int16(std::istream &in) {
      int16_t x;
      read_bytes16(in, x);
      return x;
    }

    inline int32_t
    read_val_int32(std::istream &in) {
      int32_t x;
      read_bytes32(in, x);
      return x;
    }

    inline int64_t
    read_val_int64(std::istream &in) {
      int64_t x;
      read_bytes64(in, x);
      return x;
    }

    inline int8_t
    read_int_fixed(std::istream &in) {
      const int h = in.get();
      if (header_type(h) != WireType::FIXNUM_NEGATIVE)
        throw ReadException("Failed to read FIXNUM_NEGATIVE", h);
      return read_val_int_fixed(h);
    }

    inline int8_t
    read_int8(std::istream &in) {
      const int h = in.get();
      if (header_type(h) != WireType::INT_8)
        throw ReadException("Failed to read INT_8", h, header::INT_8);
      return read_val_int8(in);
    }

    inline int16_t
    read_int16(std::istream &in) {
      const int h = in.get();
      if (header_type(h) != WireType::INT_16)
        throw ReadException("Failed to read INT_16", h, header::INT_16);
      return read_val_int16(in);
    }

    inline int32_t
    read_int32(std::istream &in) {
      const int h = in.get();
      if (header_type(h) != WireType::INT_32)
        throw ReadException("Failed to read INT_32", h, header::INT_32);
      return read_val_int32(in);
    }

    inline int64_t
    read_int64(std::istream &in) {
      const int h = in.get();
      if (header_type(h) != WireType::INT_64)
        throw ReadException("Failed to read INT_64", h, header::INT_64);
      return read_val_int64(in);
    }

    inline int64_t
    read_int(std::istream &in) {
      const int h = in.get();
      const WireType type = header_type(h);
      switch (type) {
      case WireType::FIXNUM_NEGATIVE: return read_val_int_fixed(h);
      case WireType::FIXNUM_POSITIVE: return read_val_uint_fixed(h);
      case WireType::INT_8: return read_val_int8(in);
      case WireType::INT_16: return read_val_int16(in);
      case WireType::INT_32: return read_val_int32(in);
      case WireType::INT_64: return read_val_int64(in);
      default:
        throw ReadException("Did not find an integer to read", h);
      }
    }

    inline uint8_t
    read_val_uint_fixed(const int h) {
      return static_cast<uint8_t>(h & 0x7F);
    }

    inline uint8_t
    read_val_uint8(std::istream &in) {
      uint8_t x;
      read_bytes8(in, x);
      return x;
    }

    inline uint16_t
    read_val_uint16(std::istream &in) {
      uint16_t x;
      read_bytes16(in, x);
      return x;
    }

    inline uint32_t
    read_val_uint32(std::istream &in) {
      uint32_t x;
      read_bytes32(in, x);
      return x;
    }

    inline uint64_t
    read_val_uint64(std::istream &in) {
      uint64_t x;
      read_bytes64(in, x);
      return x;
    }

    inline uint8_t
    read_uint_fixed(std::istream &in) {
      const int h = in.get();
      if (header_type(h) != WireType::FIXNUM_POSITIVE)
        throw ReadException("Failed to read FIXNUM_POSITIVE", h);
      return read_val_uint_fixed(h);
    }

    inline uint8_t
    read_uint8(std::istream &in) {
      const int h = in.get();
      if (header_type(h) != WireType::UINT_8)
        throw ReadException("Failed to read UINT_8", h, header::UINT_8);
      return read_val_uint8(in);
    }

    inline uint16_t
    read_uint16(std::istream &in) {
      const int h = in.get();
      if (header_type(h) != WireType::UINT_16)
        throw ReadException("Failed to read UINT_16", h, header::UINT_16);
      return read_val_uint16(in);
    }

    inline uint32_t
    read_uint32(std::istream &in) {
      const int h = in.get();
      if (header_type(h) != WireType::UINT_32)
        throw ReadException("Failed to read UINT_32", h, header::UINT_32);
      return read_val_uint32(in);
    }

    inline uint64_t
    read_uint64(std::istream &in) {
      const int h = in.get();
      if (header_type(h) != WireType::UINT_64)
        throw ReadException("Failed to read UINT_64", h, header::UINT_64);
      return read_val_uint64(in);
    }

    inline uint64_t
    read_uint(std::istream &in) {
      const int h = in.get();
      const WireType type = header_type(h);
      switch (type) {
      case WireType::FIXNUM_POSITIVE: return read_val_uint_fixed(h);
      case WireType::UINT_8: return read_val_uint8(in);
      case WireType::UINT_16: return read_val_uint16(in);
      case WireType::UINT_32: return read_val_uint32(in);
      case WireType::UINT_64: return read_val_uint64(in);
      default:
        throw ReadException("Did not find an unsigned integer to read", h);
      }
    }

    inline size_t
    read_array_size(std::istream &in) {
      const int header = in.get();
      const WireType type = header_type(header);
      uint16_t s16;
      uint32_t s32;
      switch (type) {
      case WireType::ARRAY_FIXED:
        return header & 0x0F;
      case WireType::ARRAY_16:
        read_bytes16(in, s16);
        return s16;
      case WireType::ARRAY_32:
        read_bytes32(in, s32);
        return s32;
      default:
        assert(!"header is not an array");
        return 0;
      }
    }

    inline size_t
    read_map_size(std::istream &in) {
      const int header = in.get();
      const WireType type = header_type(header);
      uint16_t s16;
      uint32_t s32;
      switch (type) {
      case WireType::MAP_FIXED:
        return header & 0x0F;
      case WireType::MAP_16:
        read_bytes16(in, s16);
        return s16;
      case WireType::MAP_32:
        read_bytes32(in, s32);
        return s32;
      default:
        assert(!"header is not a map");
        return 0;
      }
    }

    inline std::string
    read_raw(std::istream &in) {
      const int header = in.get();
      const WireType type = header_type(header);
      uint16_t s16;
      uint32_t s32;

      switch (type) {
      case WireType::RAW_FIXED:
        s32 = header & 0x1F;
        break;
      case WireType::RAW_16:
        read_bytes16(in, s16);
        s32 = s16;
        break;
      case WireType::RAW_32:
        read_bytes32(in, s32);
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
    write_uint8(std::ostream &out, const uint8_t x) {
      out.put(header::UINT_8);
      write_raw_uint8(out, x);
    }

    inline void
    write_uint16(std::ostream &out, const uint16_t x) {
      out.put(header::UINT_16);
      write_raw_uint16(out, x);
    }

    inline void
    write_uint32(std::ostream &out, const uint32_t x) {
      out.put(header::UINT_32);
      write_raw_uint32(out, x);
    }

    inline void
    write_uint64(std::ostream &out, const uint64_t x) {
      out.put(header::UINT_64);
      write_raw_uint64(out, x);
    }

    inline void
    write_int_fixed(std::ostream &out, const int8_t x) {
      write_raw_uint8(out, ~(*reinterpret_cast<const uint8_t *>(&x)) & 0xE0);
    }

    inline void
    write_int8(std::ostream &out, const int8_t x) {
      out.put(header::INT_8);
      write_raw_int8(out, x);
    }

    inline void
    write_int16(std::ostream &out, const int16_t x) {
      out.put(header::INT_16);
      write_raw_int16(out, x);
    }

    inline void
    write_int32(std::ostream &out, const int32_t x) {
      out.put(header::INT_32);
      write_raw_int32(out, x);
    }

    inline void
    write_int64(std::ostream &out, const int64_t x) {
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
        write_uint8(out, static_cast<uint8_t>(x));
      else if (x <= std::numeric_limits<uint16_t>::max())
        write_uint16(out, static_cast<uint16_t>(x));
      else if (x <= std::numeric_limits<uint32_t>::max())
        write_uint32(out, static_cast<uint32_t>(x));
      else
        write_uint64(out, x);
    }

    inline void
    write_int(std::ostream &out, const int64_t x) {
      if (x >= -32 && x <= -1)
        write_int_fixed(out, static_cast<int8_t>(x));
      else if (x >= 0 && x <= 127)
        write_uint_fixed(out, static_cast<uint8_t>(x));
      else if (x >= std::numeric_limits<int8_t>::min() && x <= std::numeric_limits<int8_t>::max())
        write_int8(out, static_cast<int8_t>(x));
      else if (x >= std::numeric_limits<int16_t>::min() && x <= std::numeric_limits<int16_t>::max())
        write_int16(out, static_cast<int16_t>(x));
      else if (x >= std::numeric_limits<int32_t>::min() && x <= std::numeric_limits<int32_t>::max())
        write_int32(out, static_cast<int32_t>(x));
      else
        write_int64(out, x);
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
