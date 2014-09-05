/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_WIRE_IMPL_H_
#define SCHWA_MSGPACK_WIRE_IMPL_H_

#include <cassert>
#include <limits>

#include <schwa/io/traits.h>
#include <schwa/mpl/if.h>
#include <schwa/msgpack/dynamic.h>
#include <schwa/msgpack/exception.h>
#include <schwa/msgpack/wire.h>
#include <schwa/port.h>


namespace schwa {
  namespace msgpack {

    constexpr const WireType HEADER_TYPES[256] = {
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE, WireType::FIXINT_POSITIVE,
      WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      ,
      WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      , WireType::MAP_FIXED      ,
      WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    ,
      WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    , WireType::ARRAY_FIXED    ,
      WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      ,
      WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      ,
      WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      ,
      WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      , WireType::STR_FIXED      ,
      WireType::NIL            , WireType::RESERVED       , WireType::FALSE          , WireType::TRUE           , WireType::BIN_8          , WireType::BIN_16         , WireType::BIN_32         , WireType::EXT_8          ,
      WireType::EXT_16         , WireType::EXT_32         , WireType::FLOAT          , WireType::DOUBLE         , WireType::UINT_8         , WireType::UINT_16        , WireType::UINT_32        , WireType::UINT_64        ,
      WireType::INT_8          , WireType::INT_16         , WireType::INT_32         , WireType::INT_64         , WireType::EXT_FIXED_8    , WireType::EXT_FIXED_16   , WireType::EXT_FIXED_32   , WireType::EXT_FIXED_64   ,
      WireType::EXT_FIXED_128  , WireType::STR_8          , WireType::STR_16         , WireType::STR_32         , WireType::ARRAY_16       , WireType::ARRAY_32       , WireType::MAP_16         , WireType::MAP_32         ,
      WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE,
      WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE,
      WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE,
      WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE, WireType::FIXINT_NEGATIVE,
    };

    namespace header {
      constexpr const unsigned char FIXINT_POSITIVE = 0x7f;
      constexpr const unsigned char MAP_FIXED       = 0x80;
      constexpr const unsigned char ARRAY_FIXED     = 0x90;
      constexpr const unsigned char STR_FIXED       = 0xa0;
      constexpr const unsigned char NIL             = 0xc0;
      constexpr const unsigned char RESERVED        = 0xc1;
      constexpr const unsigned char FALSE           = 0xc2;
      constexpr const unsigned char TRUE            = 0xc3;
      constexpr const unsigned char BIN_8           = 0xc4;
      constexpr const unsigned char BIN_16          = 0xc5;
      constexpr const unsigned char BIN_32          = 0xc6;
      constexpr const unsigned char EXT_8           = 0xc7;
      constexpr const unsigned char EXT_16          = 0xc8;
      constexpr const unsigned char EXT_32          = 0xc9;
      constexpr const unsigned char FLOAT           = 0xca;
      constexpr const unsigned char DOUBLE          = 0xcb;
      constexpr const unsigned char UINT_8          = 0xcc;
      constexpr const unsigned char UINT_16         = 0xcd;
      constexpr const unsigned char UINT_32         = 0xce;
      constexpr const unsigned char UINT_64         = 0xcf;
      constexpr const unsigned char INT_8           = 0xd0;
      constexpr const unsigned char INT_16          = 0xd1;
      constexpr const unsigned char INT_32          = 0xd2;
      constexpr const unsigned char INT_64          = 0xd3;
      constexpr const unsigned char EXT_FIXED_8     = 0xd4;
      constexpr const unsigned char EXT_FIXED_16    = 0xd5;
      constexpr const unsigned char EXT_FIXED_32    = 0xd6;
      constexpr const unsigned char EXT_FIXED_64    = 0xd7;
      constexpr const unsigned char EXT_FIXED_128   = 0xd8;
      constexpr const unsigned char STR_8           = 0xd9;
      constexpr const unsigned char STR_16          = 0xda;
      constexpr const unsigned char STR_32          = 0xdb;
      constexpr const unsigned char ARRAY_16        = 0xdc;
      constexpr const unsigned char ARRAY_32        = 0xdd;
      constexpr const unsigned char MAP_16          = 0xde;
      constexpr const unsigned char MAP_32          = 0xdf;
      constexpr const unsigned char FIXINT_NEGATIVE = 0xff;
    }


    // ========================================================================
    // Host <--> big endian low level functions.
    // ========================================================================
    template <typename IN, typename T>
    inline void
    _read_be8(IN &in, T *const h8) {
      static_assert(sizeof(T) == 1, "wrong sized pointer target");
      uint8_t be8;
      in.read(reinterpret_cast<char *>(&be8), 1);
      if (SCHWA_UNLIKELY(in.gcount() != 1))
        throw ReadError("Failed to read data", in.gcount(), 1);
      *reinterpret_cast<uint8_t *>(h8) = be8;
    }

    template <typename IN, typename T>
    inline void
    _read_be16(IN &in, T *const h16) {
      static_assert(sizeof(T) == 2, "wrong sized pointer target");
      uint16_t be16;
      in.read(reinterpret_cast<char *>(&be16), 2);
      if (SCHWA_UNLIKELY(in.gcount() != 2))
        throw ReadError("Failed to read data", in.gcount(), 2);
      *reinterpret_cast<uint16_t *>(h16) = port::be16_to_h(be16);
    }

    template <typename IN, typename T>
    inline void
    _read_be32(IN &in, T *const h32) {
      static_assert(sizeof(T) == 4, "wrong sized pointer target");
      uint32_t be32;
      in.read(reinterpret_cast<char *>(&be32), 4);
      if (SCHWA_UNLIKELY(in.gcount() != 4))
        throw ReadError("Failed to read data", in.gcount(), 4);
      *reinterpret_cast<uint32_t *>(h32) = port::be32_to_h(be32);
    }

    template <typename IN, typename T>
    inline void
    _read_be64(IN &in, T *const h64) {
      static_assert(sizeof(T) == 8, "wrong sized pointer target");
      uint64_t be64;
      in.read(reinterpret_cast<char *>(&be64), 8);
      if (SCHWA_UNLIKELY(in.gcount() != 8))
        throw ReadError("Failed to read data", in.gcount(), 8);
      *reinterpret_cast<uint64_t *>(h64) = port::be64_to_h(be64);
    }

    template <typename OUT, typename T>
    inline void
    _write_be8(OUT &out, const T *const h8) {
      static_assert(sizeof(T) == 1, "wrong sized pointer target");
      const uint8_t be8 = *reinterpret_cast<const uint8_t *>(h8);
      out.write(reinterpret_cast<const char *>(&be8), 1);
    }

    template <typename OUT, typename T>
    inline void
    _write_be16(OUT &out, const T *const h16) {
      static_assert(sizeof(T) == 2, "wrong sized pointer target");
      const uint16_t be16 = port::h_to_be16(*reinterpret_cast<const uint16_t *>(h16));
      out.write(reinterpret_cast<const char *>(&be16), 2);
    }

    template <typename OUT, typename T>
    inline void
    _write_be32(OUT &out, const T *const h32) {
      static_assert(sizeof(T) == 4, "wrong sized pointer target");
      const uint32_t be32 = port::h_to_be32(*reinterpret_cast<const uint32_t *>(h32));
      out.write(reinterpret_cast<const char *>(&be32), 4);
    }

    template <typename OUT, typename T>
    inline void
    _write_be64(OUT &out, const T *const h64) {
      static_assert(sizeof(T) == 8, "wrong sized pointer target");
      const uint64_t be64 = port::h_to_be64(*reinterpret_cast<const uint64_t *>(h64));
      out.write(reinterpret_cast<const char *>(&be64), 8);
    }


    // ========================================================================
    // Helper functions.
    // ========================================================================
    template <size_t NBYTES, typename IN, typename OUT>
    inline void
    _read_write_nbytes(IN &in, OUT &out) {
      static_assert(NBYTES != 0, "nbytes must be positive");
      char buf[NBYTES];
      in.read(buf, NBYTES);
      if (SCHWA_UNLIKELY(in.gcount() != NBYTES))
        throw ReadError("Failed to read data", in.gcount(), NBYTES);
      out.write(buf, NBYTES);
    }


    // ========================================================================
    // Type traits.
    // ========================================================================
    namespace traits {
      template <typename T, bool>
      struct rw_signed_integral {
        template <typename IN>
        static inline void read(IN &in, T &val) { val = read_uint(in); }
        template <typename OUT>
        static inline void write(OUT &out, const T &val) { write_uint(out, val); }
      };

      template <typename T>
      struct rw_signed_integral<T, true> {
        template <typename IN>
        static inline void read(IN &in, T &val) { val = read_int(in); }
        template <typename OUT>
        static inline void write(OUT &out, const T &val) { write_int(out, val); }
      };

      template <typename T>
      struct rw_integral : rw_signed_integral<T, std::is_signed<T>::value> { };

      template <>
      struct rw_integral<bool> {
        template <typename IN>
        static inline void read(IN &in, bool &val) { val = read_bool(in); }
        template <typename OUT>
        static inline void write(OUT &out, const bool &val) { write_bool(out, val); }
      };

      template <typename T>
      struct rw_not_integral { };

      template <>
      struct rw_not_integral<float> {
        template <typename IN>
        static inline void read(IN &in, float &val) { val = read_float(in); }
        template <typename OUT>
        static inline void write(OUT &out, const float &val) { write_float(out, val); }
      };

      template <>
      struct rw_not_integral<double> {
        template <typename IN>
        static inline void read(IN &in, double &val) { val = read_double(in); }
        template <typename OUT>
        static inline void write(OUT &out, const double &val) { write_double(out, val); }
      };

      template <>
      struct rw_not_integral<std::string> {
        template <typename IN>
        static inline void read(IN &in, std::string &val) { val = read_str(in); }
        template <typename OUT>
        static inline void write(OUT &out, const std::string &val) { write_str(out, val); }
      };

      template <>
      struct rw_not_integral<UnicodeString> {
        template <typename IN>
        static inline void read(IN &in, UnicodeString &val) { val = read_utf8(in); }
        template <typename OUT>
        static inline void write(OUT &out, const UnicodeString &val) { write_utf8(out, val); }
      };

      template <typename T>
      struct rw : mpl::if_<std::is_integral<T>, rw_integral<T>, rw_not_integral<T>>::type { };
    }


    // ========================================================================
    // Reading API implementations
    // ========================================================================
    template <typename IN, typename T>
    inline void read(IN &in, T &val) {
      traits::rw<T>::read(in, val);
    }

    inline WireType
    header_type(const int header) {
      if (SCHWA_UNLIKELY(header == EOF))
        throw ReadError("Read EOF", header);
      return HEADER_TYPES[header & 0xff];
    }

    template <typename IN>
    inline void
    read_nil(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(h != header::NIL))
        throw ReadError("Failed to `read_nil`", h);
    }

    template <typename IN>
    inline bool
    read_bool(IN &in) {
      const int h = in.get();
      if (h == header::TRUE)
        return true;
      else if (h == header::FALSE)
        return false;
      else
        throw ReadError("Failed to `read_bool`", h);
    }

    template <typename IN>
    inline float
    read_float(IN &in) {
      float x;
      const int h = in.get();
      if (SCHWA_UNLIKELY(h != header::FLOAT))
        throw ReadError("Failed to `read_float`", h);
      _read_be32(in, &x);
      return x;
    }

    template <typename IN>
    inline double
    read_double(IN &in) {
      double x;
      const int h = in.get();
      if (SCHWA_UNLIKELY(h != header::DOUBLE))
        throw ReadError("Failed to `read_double`", h);
      _read_be64(in, &x);
      return x;
    }

    inline int8_t
    read_val_fixint_negative(const int h) {
      int8_t n;
      *reinterpret_cast<uint8_t *>(&n) = static_cast<uint8_t>(h & header::FIXINT_NEGATIVE);
      return n;
    }

    template <typename IN>
    inline int8_t
    read_val_int8(IN &in) {
      int8_t x;
      _read_be8(in, &x);
      return x;
    }

    template <typename IN>
    inline int16_t
    read_val_int16(IN &in) {
      int16_t x;
      _read_be16(in, &x);
      return x;
    }

    template <typename IN>
    inline int32_t
    read_val_int32(IN &in) {
      int32_t x;
      _read_be32(in, &x);
      return x;
    }

    template <typename IN>
    inline int64_t
    read_val_int64(IN &in) {
      int64_t x;
      _read_be64(in, &x);
      return x;
    }

    template <typename IN>
    inline int8_t
    read_fixint_negative(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(header_type(h) != WireType::FIXINT_NEGATIVE))
        throw ReadError("Failed to `read_fixint_negative`", h);
      return read_val_fixint_negative(h);
    }

    template <typename IN>
    inline int8_t
    read_int8(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(header_type(h) != WireType::INT_8))
        throw ReadError("Failed to `read_int8`", h, header::INT_8);
      return read_val_int8(in);
    }

    template <typename IN>
    inline int16_t
    read_int16(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(header_type(h) != WireType::INT_16))
        throw ReadError("Failed to `read_int16`", h, header::INT_16);
      return read_val_int16(in);
    }

    template <typename IN>
    inline int32_t
    read_int32(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(header_type(h) != WireType::INT_32))
        throw ReadError("Failed to `read_int32`", h, header::INT_32);
      return read_val_int32(in);
    }

    template <typename IN>
    inline int64_t
    read_int64(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(header_type(h) != WireType::INT_64))
        throw ReadError("Failed to `read_int64`", h, header::INT_64);
      return read_val_int64(in);
    }

    template <typename IN>
    inline int64_t
    read_int(IN &in) {
      const int h = in.get();
      const WireType type = header_type(h);
      switch (type) {
      case WireType::FIXINT_NEGATIVE: return read_val_fixint_negative(h);
      case WireType::FIXINT_POSITIVE: return read_val_fixint_positive(h);
      case WireType::INT_8: return read_val_int8(in);
      case WireType::INT_16: return read_val_int16(in);
      case WireType::INT_32: return read_val_int32(in);
      case WireType::INT_64: return read_val_int64(in);
      case WireType::UINT_8: return static_cast<int64_t>(read_val_uint8(in));
      case WireType::UINT_16: return static_cast<int64_t>(read_val_uint16(in));
      case WireType::UINT_32: return static_cast<int64_t>(read_val_uint32(in));
      case WireType::UINT_64:
        {
          const uint64_t n = read_val_uint64(in);
          if (n > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
            throw ReadError("Cannot `read_int`: unsigned integer value exceeds capacity of signed integer data type. Use `read_uint` instead.");
          return static_cast<int64_t>(n);
        }
      default:
        throw ReadError("Failed to `read_int`", h);
      }
    }

    inline uint8_t
    read_val_fixint_positive(const int h) {
      return static_cast<uint8_t>(h & header::FIXINT_POSITIVE);
    }

    template <typename IN>
    inline uint8_t
    read_val_uint8(IN &in) {
      uint8_t x;
      _read_be8(in, &x);
      return x;
    }

    template <typename IN>
    inline uint16_t
    read_val_uint16(IN &in) {
      uint16_t x;
      _read_be16(in, &x);
      return x;
    }

    template <typename IN>
    inline uint32_t
    read_val_uint32(IN &in) {
      uint32_t x;
      _read_be32(in, &x);
      return x;
    }

    template <typename IN>
    inline uint64_t
    read_val_uint64(IN &in) {
      uint64_t x;
      _read_be64(in, &x);
      return x;
    }

    template <typename IN>
    inline uint8_t
    read_fixint_positive(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(header_type(h) != WireType::FIXINT_POSITIVE))
        throw ReadError("Failed to `read_fixint_positive`", h);
      return read_val_fixint_positive(h);
    }

    template <typename IN>
    inline uint8_t
    read_uint8(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(header_type(h) != WireType::UINT_8))
        throw ReadError("Failed to `read_uint8`", h, header::UINT_8);
      return read_val_uint8(in);
    }

    template <typename IN>
    inline uint16_t
    read_uint16(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(header_type(h) != WireType::UINT_16))
        throw ReadError("Failed to `read_uint16`", h, header::UINT_16);
      return read_val_uint16(in);
    }

    template <typename IN>
    inline uint32_t
    read_uint32(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(header_type(h) != WireType::UINT_32))
        throw ReadError("Failed to `read_uint32`", h, header::UINT_32);
      return read_val_uint32(in);
    }

    template <typename IN>
    inline uint64_t
    read_uint64(IN &in) {
      const int h = in.get();
      if (SCHWA_UNLIKELY(header_type(h) != WireType::UINT_64))
        throw ReadError("Failed to `read_uint64`", h, header::UINT_64);
      return read_val_uint64(in);
    }

    template <typename IN>
    inline uint64_t
    read_uint(IN &in) {
      const int h = in.get();
      const WireType type = header_type(h);
      switch (type) {
      case WireType::FIXINT_POSITIVE: return read_val_fixint_positive(h);
      case WireType::UINT_8: return read_val_uint8(in);
      case WireType::UINT_16: return read_val_uint16(in);
      case WireType::UINT_32: return read_val_uint32(in);
      case WireType::UINT_64: return read_val_uint64(in);
      default:
        throw ReadError("Failed to `read_uint`", h);
      }
    }

    template <typename IN>
    inline uint32_t
    read_array_size(IN &in) {
      const int h = in.get();
      const WireType type = header_type(h);
      uint16_t s16 = 0;
      uint32_t s32 = 0;
      switch (type) {
      case WireType::ARRAY_FIXED:
        return h & 0x0f;
      case WireType::ARRAY_16:
        _read_be16(in, &s16);
        return s16;
      case WireType::ARRAY_32:
        _read_be32(in, &s32);
        return s32;
      default:
        throw ReadError("Failed to `read_array_size`", h);
      }
    }

    template <typename IN>
    inline uint32_t
    read_map_size(IN &in) {
      const int h = in.get();
      const WireType type = header_type(h);
      uint16_t s16 = 0;
      uint32_t s32 = 0;
      switch (type) {
      case WireType::MAP_FIXED:
        return h & 0x0f;
      case WireType::MAP_16:
        _read_be16(in, &s16);
        return s16;
      case WireType::MAP_32:
        _read_be32(in, &s32);
        return s32;
      default:
        throw ReadError("Failed to `read_map_size`", h);
      }
    }


    template <typename IN>
    inline std::string
    read_str(IN &in) {
      const int h = in.get();
      const WireType type = header_type(h);

      uint32_t s32 = 0;
      switch (type) {
      case WireType::STR_FIXED:
        s32 = h & 0x1f;
        break;
      case WireType::STR_8:
        uint8_t s8;
        _read_be8(in, &s8);
        s32 = s8;
        break;
      case WireType::STR_16:
        uint16_t s16;
        _read_be16(in, &s16);
        s32 = s16;
        break;
      case WireType::STR_32:
        _read_be32(in, &s32);
        break;
      default:
        throw ReadError("Failed to `read_str`", h);
      }

      std::string s(s32, '\0');
      in.read(&s[0], s32);
      if (SCHWA_UNLIKELY(in.gcount() != s32))
        throw ReadError("Failed to read all of the str data in `read_str`", in.gcount(), s32);
      return s;
    }


    template <typename IN>
    inline UnicodeString
    read_utf8(IN &in) {
      return UnicodeString::from_utf8(read_str(in));
    }


    template <typename IN>
    inline std::string
    read_bin(IN &in) {
      const int h = in.get();
      const WireType type = header_type(h);

      uint32_t s32 = 0;
      switch (type) {
      case WireType::BIN_8:
        uint8_t s8;
        _read_be8(in, &s8);
        s32 = s8;
        break;
      case WireType::BIN_16:
        uint16_t s16;
        _read_be16(in, &s16);
        s32 = s16;
        break;
      case WireType::BIN_32:
        _read_be32(in, &s32);
        break;
      default:
        throw ReadError("Failed to `read_bin`", h);
      }

      std::string s(s32, '\0');
      in.read(&s[0], s32);
      if (SCHWA_UNLIKELY(in.gcount() != s32))
        throw ReadError("Failed to read all of the bin data in `read_bin`", in.gcount(), s32);
      return s;
    }


    template <typename IN, typename OUT>
    inline bool
    read_lazy(IN &in, OUT &out) {
      WireType type;
      return read_lazy(in, out, type);
    }


    template <typename IN, typename OUT>
    inline bool
    read_lazy(IN &in, OUT &out, WireType &type) {
      uint8_t s8 = 0;
      uint16_t s16 = 0;
      uint32_t s32 = 0;
      uint64_t s64 = 0;
      bool recurse = false, remaining_bytes = false;

      const int h = in.get();
      if (h == EOF)
        return false;
      out.put(h);

      type = header_type(h);
      switch (type) {
      case WireType::FIXINT_POSITIVE:
      case WireType::FIXINT_NEGATIVE:
      case WireType::NIL:
      case WireType::TRUE:
      case WireType::FALSE:
      case WireType::RESERVED:
        break;
      case WireType::EXT_FIXED_8:
      case WireType::INT_8:
      case WireType::UINT_8:
        _read_write_nbytes<1, IN, OUT>(in, out);
        break;
      case WireType::EXT_FIXED_16:
      case WireType::INT_16:
      case WireType::UINT_16:
        _read_write_nbytes<2, IN, OUT>(in, out);
        break;
      case WireType::EXT_FIXED_32:
      case WireType::FLOAT:
      case WireType::INT_32:
      case WireType::UINT_32:
        _read_write_nbytes<4, IN, OUT>(in, out);
        break;
      case WireType::DOUBLE:
      case WireType::EXT_FIXED_64:
      case WireType::INT_64:
      case WireType::UINT_64:
        _read_write_nbytes<8, IN, OUT>(in, out);
        break;
      case WireType::EXT_FIXED_128:
        _read_write_nbytes<16, IN, OUT>(in, out);
        break;
      case WireType::ARRAY_FIXED:
        s64 = h & 0x0f;
        recurse = true;
        break;
      case WireType::ARRAY_16:
        _read_be16(in, &s16);
        _write_be16(out, &s16);
        s64 = s16;
        recurse = true;
        break;
      case WireType::ARRAY_32:
        _read_be32(in, &s32);
        _write_be32(out, &s32);
        s64 = s32;
        recurse = true;
        break;
      case WireType::MAP_FIXED:
        s64 = 2 * (h & 0x0f);
        recurse = true;
        break;
      case WireType::MAP_16:
        _read_be16(in, &s16);
        _write_be16(out, &s16);
        s64 = 2 * s16;
        recurse = true;
        break;
      case WireType::MAP_32:
        _read_be32(in, &s32);
        _write_be32(out, &s32);
        s64 = 2 * s32;
        recurse = true;
        break;
      case WireType::STR_FIXED:
        s32 = h & 0x1f;
        remaining_bytes = true;
        break;
      case WireType::BIN_8:
      case WireType::EXT_8:
      case WireType::STR_8:
        _read_be8(in, &s8);
        _write_be8(out, &s8);
        s32 = s8;
        remaining_bytes = true;
        break;
      case WireType::BIN_16:
      case WireType::EXT_16:
      case WireType::STR_16:
        _read_be16(in, &s16);
        _write_be16(out, &s16);
        s32 = s16;
        remaining_bytes = true;
        break;
      case WireType::BIN_32:
      case WireType::EXT_32:
      case WireType::STR_32:
        _read_be32(in, &s32);
        _write_be32(out, &s32);
        remaining_bytes = true;
        break;
      }

      // Read the extension type information.
      if (is_ext(type)) {
        remaining_bytes = true;
        _read_write_nbytes<1, IN, OUT>(in, out);
      }

      if (recurse) {
        // Read in the `s64` additional msgpack objects.
        for (uint64_t i = 0; i != s64; ++i)
          if (!read_lazy(in, out))
            return false;
      }
      else if (remaining_bytes) {
        // Read in the `s32` remaining bytes of data that make up the current msgpack object.
        char buf[128];
        for (uint32_t remaining = s32; remaining != 0; ) {
          const uint32_t nbytes = std::min(static_cast<uint32_t>(sizeof(buf)), remaining);
          in.read(buf, nbytes);
          if (in.gcount() != nbytes)
            return false;
          out.write(buf, nbytes);
          remaining -= nbytes;
        }
      }

      return true;
    }


    template <typename IN>
    inline Value *
    _read_dynamic(IN &in, Pool &pool, Value *value) {
      const int h = in.peek();
      if (h == EOF)
        return nullptr;

      const WireType type = header_type(h);
      if (value == nullptr)
        value = Value::create(pool, type);
      value->type = type;

      uint8_t s8 = 0;
      uint16_t s16 = 0;
      uint32_t s32 = 0;
      switch (type) {
      case WireType::NIL:
        read_nil(in);
        break;
      case WireType::TRUE:
      case WireType::FALSE:
        value->via._bool = read_bool(in);
        break;
      case WireType::FIXINT_POSITIVE:
      case WireType::UINT_8:
      case WireType::UINT_16:
      case WireType::UINT_32:
      case WireType::UINT_64:
        value->via._uint64 = read_uint(in);
        break;
      case WireType::FIXINT_NEGATIVE:
      case WireType::INT_8:
      case WireType::INT_16:
      case WireType::INT_32:
      case WireType::INT_64:
        value->via._int64 = read_int(in);
        break;
      case WireType::FLOAT:
        value->via._float = read_float(in);
        break;
      case WireType::DOUBLE:
        value->via._double = read_double(in);
        break;
      case WireType::ARRAY_FIXED:
        in.get();
        s32 = h & 0x0f;
        break;
      case WireType::MAP_FIXED:
        in.get();
        s32 = h & 0x0f;
        break;
      case WireType::STR_FIXED:
        in.get();
        s32 = h & 0x1f;
        break;
      case WireType::BIN_8:
      case WireType::EXT_8:
      case WireType::STR_8:
        in.get();
        _read_be8(in, &s8);
        s32 = s8;
        break;
      case WireType::ARRAY_16:
      case WireType::BIN_16:
      case WireType::EXT_16:
      case WireType::MAP_16:
      case WireType::STR_16:
        in.get();
        _read_be16(in, &s16);
        s32 = s16;
        break;
      case WireType::ARRAY_32:
      case WireType::BIN_32:
      case WireType::EXT_32:
      case WireType::MAP_32:
      case WireType::STR_32:
        in.get();
        _read_be32(in, &s32);
        break;
      case WireType::EXT_FIXED_8:
        in.get();
        s32 = 1;
        break;
      case WireType::EXT_FIXED_16:
        in.get();
        s32 = 2;
        break;
      case WireType::EXT_FIXED_32:
        in.get();
        s32 = 4;
        break;
      case WireType::EXT_FIXED_64:
        in.get();
        s32 = 8;
        break;
      case WireType::EXT_FIXED_128:
        in.get();
        s32 = 16;
        break;
      case WireType::RESERVED:
        in.get();
        break;
      }

      if (is_array(type)) {
        Array *array = Array::create(pool, s32);
        value->via._array = array;
        for (uint32_t i = 0; i != s32; ++i)
          _read_dynamic(in, pool, &array->get(i));
      }
      else if (is_map(type)) {
        Map *map = Map::create(pool, s32);
        value->via._map = map;
        for (uint32_t i = 0; i != s32; ++i) {
          Map::Pair &pair = map->get(i);
          _read_dynamic(in, pool, &pair.key);
          _read_dynamic(in, pool, &pair.value);
        }
      }
      else if (is_bin(type)) {
        Bin *obj = Bin::create(pool, s32, in);
        value->via._bin = obj;
      }
      else if (is_str(type)) {
        Str *obj = Str::create(pool, s32, in);
        value->via._str = obj;
      }
      else if (is_ext(type)) {
        const int8_t type = read_int8(in);
        Ext *obj = Ext::create(pool, type, s32, in);
        value->via._ext = obj;
      }

      return value;
    }


    template <typename IN>
    inline Value *
    read_dynamic(IN &in, Pool &pool) {
      return _read_dynamic(in, pool, nullptr);
    }


    // ========================================================================
    // Writing API implementations
    // ========================================================================
    template <typename OUT, typename T>
    inline void
    write(OUT &out, const T &val) {
      traits::rw<T>::write(out, val);
    }

    template <typename OUT> inline void write_raw_uint8(OUT &out, const uint8_t x) { _write_be8<OUT, uint8_t>(out, &x); }
    template <typename OUT> inline void write_raw_uint16(OUT &out, const uint16_t x) { _write_be16<OUT, uint16_t>(out, &x); }
    template <typename OUT> inline void write_raw_uint32(OUT &out, const uint32_t x) { _write_be32<OUT, uint32_t>(out, &x); }
    template <typename OUT> inline void write_raw_uint64(OUT &out, const uint64_t x) { _write_be64<OUT, uint64_t>(out, &x); }

    template <typename OUT> inline void write_raw_int8(OUT &out, const int8_t x) { _write_be8<OUT, int8_t>(out, &x); }
    template <typename OUT> inline void write_raw_int16(OUT &out, const int16_t x) { _write_be16<OUT, int16_t>(out, &x); }
    template <typename OUT> inline void write_raw_int32(OUT &out, const int32_t x) { _write_be32<OUT, int32_t>(out, &x); }
    template <typename OUT> inline void write_raw_int64(OUT &out, const int64_t x) { _write_be64<OUT, int64_t>(out, &x); }

    template <typename OUT>
    inline void
    write_fixint_positive(OUT &out, const uint8_t x) {
      write_raw_uint8(out, x);
    }


    template <typename OUT>
    inline void
    write_uint8(OUT &out, const uint8_t x) {
      out.put(header::UINT_8);
      write_raw_uint8(out, x);
    }


    template <typename OUT>
    inline void
    write_uint16(OUT &out, const uint16_t x) {
      out.put(header::UINT_16);
      write_raw_uint16(out, x);
    }


    template <typename OUT>
    inline void
    write_uint32(OUT &out, const uint32_t x) {
      out.put(header::UINT_32);
      write_raw_uint32(out, x);
    }


    template <typename OUT>
    inline void
    write_uint64(OUT &out, const uint64_t x) {
      out.put(header::UINT_64);
      write_raw_uint64(out, x);
    }


    template <typename OUT>
    inline void
    write_fixint_negative(OUT &out, const int8_t x) {
      write_raw_int8(out, x);
    }


    template <typename OUT>
    inline void
    write_int8(OUT &out, const int8_t x) {
      out.put(header::INT_8);
      write_raw_int8(out, x);
    }


    template <typename OUT>
    inline void
    write_int16(OUT &out, const int16_t x) {
      out.put(header::INT_16);
      write_raw_int16(out, x);
    }


    template <typename OUT>
    inline void
    write_int32(OUT &out, const int32_t x) {
      out.put(header::INT_32);
      write_raw_int32(out, x);
    }


    template <typename OUT>
    inline void
    write_int64(OUT &out, const int64_t x) {
      out.put(header::INT_64);
      write_raw_int64(out, x);
    }


    template <typename OUT>
    inline void
    write_array_size(OUT &out, const size_t size) {
      if (size <= 15)
        out.put(static_cast<unsigned char>(header::ARRAY_FIXED | size));
      else if (size <= std::numeric_limits<uint16_t>::max()) {
        out.put(header::ARRAY_16);
        write_raw_uint16(out, size);
      }
      else if (size <= std::numeric_limits<uint32_t>::max()) {
        out.put(header::ARRAY_32);
        write_raw_uint32(out, size);
      }
      else
        throw WriteError("Msgpack does not support arrays of length >= 2^32 elements.");
    }


    template <typename OUT>
    inline void
    write_map_size(OUT &out, const size_t size) {
      if (size <= 15)
        out.put(static_cast<unsigned char>(header::MAP_FIXED | size));
      else if (size <= std::numeric_limits<uint16_t>::max()) {
        out.put(header::MAP_16);
        write_raw_uint16(out, size);
      }
      else if (size <= std::numeric_limits<uint32_t>::max()) {
        out.put(header::MAP_32);
        write_raw_uint32(out, size);
      }
      else
        throw WriteError("Msgpack does not support maps of length >= 2^32 elements.");
    }


    template <typename OUT>
    inline void
    write_nil(OUT &out) {
      out.put(header::NIL);
    }


    template <typename OUT>
    inline void
    write_bool(OUT &out, const bool x) {
      out.put(x ? header::TRUE : header::FALSE);
    }


    template <typename OUT>
    inline void
    write_uint(OUT &out, const uint64_t x) {
      if (x <= 127)
        write_fixint_positive(out, static_cast<uint8_t>(x));
      else if (x <= std::numeric_limits<uint8_t>::max())
        write_uint8(out, static_cast<uint8_t>(x));
      else if (x <= std::numeric_limits<uint16_t>::max())
        write_uint16(out, static_cast<uint16_t>(x));
      else if (x <= std::numeric_limits<uint32_t>::max())
        write_uint32(out, static_cast<uint32_t>(x));
      else
        write_uint64(out, x);
    }


    template <typename OUT>
    inline void
    write_int(OUT &out, const int64_t x) {
      if (x >= -32 && x <= -1)
        write_fixint_negative(out, static_cast<int8_t>(x));
      else if (x >= 0 && x <= 127)
        write_fixint_positive(out, static_cast<uint8_t>(x));
      else if (x >= std::numeric_limits<int8_t>::min() && x <= std::numeric_limits<int8_t>::max())
        write_int8(out, static_cast<int8_t>(x));
      else if (x >= std::numeric_limits<int16_t>::min() && x <= std::numeric_limits<int16_t>::max())
        write_int16(out, static_cast<int16_t>(x));
      else if (x >= std::numeric_limits<int32_t>::min() && x <= std::numeric_limits<int32_t>::max())
        write_int32(out, static_cast<int32_t>(x));
      else
        write_int64(out, x);
    }


    template <typename OUT>
    inline void
    write_float(OUT &out, const float x) {
      out.put(header::FLOAT);
      _write_be32(out, &x);
    }


    template <typename OUT>
    inline void
    write_double(OUT &out, const double x) {
      out.put(header::DOUBLE);
      _write_be64(out, &x);
    }


    template <typename OUT>
    inline void
    write_str(OUT &out, const char *const data, const size_t nbytes) {
      if (nbytes <= 31)
        out.put(static_cast<unsigned char>(header::STR_FIXED | nbytes));
      else if (nbytes <= std::numeric_limits<uint8_t>::max()) {
        out.put(header::STR_8);
        write_raw_uint8(out, nbytes);
      }
      else if (nbytes <= std::numeric_limits<uint16_t>::max()) {
        out.put(header::STR_16);
        write_raw_uint16(out, nbytes);
      }
      else if (nbytes <= std::numeric_limits<uint32_t>::max()) {
        out.put(header::STR_32);
        write_raw_uint32(out, nbytes);
      }
      else
        throw WriteError("Msgpack does not support UTF-8 strings of length >= 2^32 bytes.");
      io::Traits<OUT>::write_zerocopy(out, data, nbytes);
    }

    template <typename OUT>
    inline void
    write_str(OUT &out, const std::string &data) {
      write_str(out, data.c_str(), data.size());
    }

    template <typename OUT>
    inline void
    write_utf8(OUT &out, const UnicodeString &s) {
      const std::string utf8 = s.to_utf8();
      write_str(out, utf8.c_str(), utf8.size());
    }


    template <typename OUT>
    inline void
    write_bin(OUT &out, const char *const data, const size_t nbytes) {
      if (nbytes <= std::numeric_limits<uint8_t>::max()) {
        out.put(header::BIN_8);
        write_raw_uint8(out, nbytes);
      }
      else if (nbytes <= std::numeric_limits<uint16_t>::max()) {
        out.put(header::BIN_16);
        write_raw_uint16(out, nbytes);
      }
      else if (nbytes <= std::numeric_limits<uint32_t>::max()) {
        out.put(header::BIN_32);
        write_raw_uint32(out, nbytes);
      }
      else
        throw WriteError("Msgpack does not support binary strings of length >= 2^32 bytes.");
      io::Traits<OUT>::write_zerocopy(out, data, nbytes);
    }

  }
}

#endif  // SCHWA_MSGPACK_WIRE_IMPL_H_
