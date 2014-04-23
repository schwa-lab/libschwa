/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_WIRE_H_
#define SCHWA_MSGPACK_WIRE_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/msgpack/dynamic.h>
#include <schwa/msgpack/enums.h>
#include <schwa/pool.h>
#include <schwa/unicode.h>


namespace schwa {
  namespace msgpack {

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

    template <typename IN> inline void     read_nil(IN &in);
    template <typename IN> inline bool     read_bool(IN &in);
    template <typename IN> inline float    read_float(IN &in);
    template <typename IN> inline double   read_double(IN &in);
    template <typename IN> inline int64_t  read_int(IN &in);
    template <typename IN> inline uint64_t read_uint(IN &in);

    template <typename IN> inline uint32_t read_array_size(IN &in);
    template <typename IN> inline uint32_t read_map_size(IN &in);

    template <typename IN> inline std::string   read_raw(IN &in);
    template <typename IN> inline UnicodeString read_utf8(IN &in);

    inline int8_t                          read_val_int_fixed(const int h);
    template <typename IN> inline int8_t   read_val_int8(IN &in);
    template <typename IN> inline int16_t  read_val_int16(IN &in);
    template <typename IN> inline int32_t  read_val_int32(IN &in);
    template <typename IN> inline int64_t  read_val_int64(IN &in);

    inline uint8_t                         read_val_uint_fixed(const int h);
    template <typename IN> inline uint8_t  read_val_uint8(IN &in);
    template <typename IN> inline uint16_t read_val_uint16(IN &in);
    template <typename IN> inline uint32_t read_val_uint32(IN &in);
    template <typename IN> inline uint64_t read_val_uint64(IN &in);

    template <typename IN> inline int8_t   read_int_fixed(IN &in);
    template <typename IN> inline int8_t   read_int8(IN &in);
    template <typename IN> inline int16_t  read_int16(IN &in);
    template <typename IN> inline int32_t  read_int32(IN &in);
    template <typename IN> inline int64_t  read_int64(IN &in);

    template <typename IN> inline uint8_t  read_uint_fixed(IN &in);
    template <typename IN> inline uint8_t  read_uint8(IN &in);
    template <typename IN> inline uint16_t read_uint16(IN &in);
    template <typename IN> inline uint32_t read_uint32(IN &in);
    template <typename IN> inline uint64_t read_uint64(IN &in);

    template <typename IN, typename T>
    inline void
    read(IN &in, T &val);

    template <typename IN, typename OUT>
    bool
    read_lazy(IN &in, OUT &out, WireType &type);

    template <typename IN>
    Value *
    read_dynamic(IN &in, Pool &pool);


    // ========================================================================
    // Writing API
    // ========================================================================
    template <typename OUT> inline void write_nil(OUT &out);
    template <typename OUT> inline void write_bool(OUT &out, const bool x);
    template <typename OUT> inline void write_int(OUT &out, const int64_t x);
    template <typename OUT> inline void write_uint(OUT &out, const uint64_t x);
    template <typename OUT> inline void write_float(OUT &out, const float x);
    template <typename OUT> inline void write_double(OUT &out, const double x);

    template <typename OUT> inline void write_array_size(OUT &out, const size_t size);
    template <typename OUT> inline void write_map_size(OUT &out, const size_t size);

    template <typename OUT> inline void write_raw(OUT &out, const std::string &data);
    template <typename OUT> inline void write_raw(OUT &out, const char *const data, const size_t size);
    template <typename OUT> inline void write_utf8(OUT &out, const UnicodeString &s);

    template <typename OUT> inline void write_int_fixed(OUT &out, const int8_t x);
    template <typename OUT> inline void write_int8(OUT &out, const int8_t x);
    template <typename OUT> inline void write_int16(OUT &out, const int16_t x);
    template <typename OUT> inline void write_int32(OUT &out, const int32_t x);
    template <typename OUT> inline void write_int64(OUT &out, const int64_t x);

    template <typename OUT> inline void write_uint_fixed(OUT &out, const uint8_t x);
    template <typename OUT> inline void write_uint8(OUT &out, const uint8_t x);
    template <typename OUT> inline void write_uint16(OUT &out, const uint16_t x);
    template <typename OUT> inline void write_uint32(OUT &out, const uint32_t x);
    template <typename OUT> inline void write_uint64(OUT &out, const uint64_t x);

    template <typename OUT, typename T>
    inline void write(OUT &out, const T &val);

  }
}

#include <schwa/msgpack/wire_impl.h>

#endif  // SCHWA_MSGPACK_WIRE_H_
