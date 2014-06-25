/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_WIRE_H_
#define SCHWA_MSGPACK_WIRE_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/msgpack/enums.h>
#include <schwa/pool.h>
#include <schwa/unicode.h>


namespace schwa {
  namespace msgpack {

    class Value;

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

    template <typename IN> inline std::string   read_bin(IN &in);

    template <typename IN> inline std::string   read_str(IN &in);
    template <typename IN> inline UnicodeString read_str(IN &in);

    inline int8_t                          read_val_fixint_negative(int h);
    template <typename IN> inline int8_t   read_val_int8(IN &in);
    template <typename IN> inline int16_t  read_val_int16(IN &in);
    template <typename IN> inline int32_t  read_val_int32(IN &in);
    template <typename IN> inline int64_t  read_val_int64(IN &in);

    inline uint8_t                         read_val_fixint_positive(int h);
    template <typename IN> inline uint8_t  read_val_uint8(IN &in);
    template <typename IN> inline uint16_t read_val_uint16(IN &in);
    template <typename IN> inline uint32_t read_val_uint32(IN &in);
    template <typename IN> inline uint64_t read_val_uint64(IN &in);

    template <typename IN> inline int8_t   read_fixint_negative(IN &in);
    template <typename IN> inline int8_t   read_int8(IN &in);
    template <typename IN> inline int16_t  read_int16(IN &in);
    template <typename IN> inline int32_t  read_int32(IN &in);
    template <typename IN> inline int64_t  read_int64(IN &in);

    template <typename IN> inline uint8_t  read_fixint_positive(IN &in);
    template <typename IN> inline uint8_t  read_uint8(IN &in);
    template <typename IN> inline uint16_t read_uint16(IN &in);
    template <typename IN> inline uint32_t read_uint32(IN &in);
    template <typename IN> inline uint64_t read_uint64(IN &in);

    template <typename IN, typename T>
    inline void read(IN &in, T &val);

    template <typename IN, typename OUT>
    inline bool read_lazy(IN &in, OUT &out);

    template <typename IN, typename OUT>
    inline bool read_lazy(IN &in, OUT &out, WireType &type);

    template <typename IN>
    inline Value *read_dynamic(IN &in, Pool &pool);


    // ========================================================================
    // Writing API
    // ========================================================================
    template <typename OUT> inline void write_nil(OUT &out);
    template <typename OUT> inline void write_bool(OUT &out, bool x);
    template <typename OUT> inline void write_int(OUT &out, int64_t x);
    template <typename OUT> inline void write_uint(OUT &out, uint64_t x);
    template <typename OUT> inline void write_float(OUT &out, float x);
    template <typename OUT> inline void write_double(OUT &out, double x);

    template <typename OUT> inline void write_array_size(OUT &out, size_t size);
    template <typename OUT> inline void write_map_size(OUT &out, size_t size);

    template <typename OUT> inline void write_raw(OUT &out, const std::string &data);
    template <typename OUT> inline void write_raw(OUT &out, const char *data, size_t size);
    template <typename OUT> inline void write_utf8(OUT &out, const UnicodeString &s);

    template <typename OUT> inline void write_fixint_negative(OUT &out, int8_t x);
    template <typename OUT> inline void write_int8(OUT &out, int8_t x);
    template <typename OUT> inline void write_int16(OUT &out, int16_t x);
    template <typename OUT> inline void write_int32(OUT &out, int32_t x);
    template <typename OUT> inline void write_int64(OUT &out, int64_t x);

    template <typename OUT> inline void write_fixint_positive(OUT &out, uint8_t x);
    template <typename OUT> inline void write_uint8(OUT &out, uint8_t x);
    template <typename OUT> inline void write_uint16(OUT &out, uint16_t x);
    template <typename OUT> inline void write_uint32(OUT &out, uint32_t x);
    template <typename OUT> inline void write_uint64(OUT &out, uint64_t x);

    template <typename OUT, typename T>
    inline void write(OUT &out, const T &val);

  }
}

#include <schwa/msgpack/wire_impl.h>

#endif  // SCHWA_MSGPACK_WIRE_H_
