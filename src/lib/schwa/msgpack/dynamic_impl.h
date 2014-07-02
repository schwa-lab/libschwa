/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_DYNAMIC_IMPL_H_
#define SCHWA_MSGPACK_DYNAMIC_IMPL_H_

#include <schwa/msgpack/exception.h>


namespace schwa {
  namespace msgpack {

    // ============================================================================
    // Bin
    // ============================================================================
    template <typename IN>
    inline Bin *
    Bin::create(Pool &pool, const uint32_t nbytes, IN &in) {
      Bin *const obj = pool.alloc<Bin *>(sizeof(Bin) + nbytes);
      new (obj) Bin(nbytes);
      in.read(obj->data(), nbytes);
      if (SCHWA_UNLIKELY(in.gcount() != nbytes))
        throw ReadError("Failed to read all of the bin data in `read_dynamic`", in.gcount(), nbytes);
      return obj;
    }


    // ============================================================================
    // Ext
    // ============================================================================
    template <typename IN>
    inline Ext *
    Ext::create(Pool &pool, const int8_t type, const uint32_t nbytes, IN &in) {
      Ext *const obj = pool.alloc<Ext *>(sizeof(Ext) + nbytes);
      new (obj) Ext(nbytes, type);
      in.read(obj->data(), nbytes);
      if (SCHWA_UNLIKELY(in.gcount() != nbytes))
        throw ReadError("Failed to read all of the ext data in `read_dynamic`", in.gcount(), nbytes);
      return obj;
    }


    // ============================================================================
    // Str
    // ============================================================================
    template <typename IN>
    inline Str *
    Str::create(Pool &pool, const uint32_t nbytes, IN &in) {
      Str *const obj = pool.alloc<Str *>(sizeof(Str) + nbytes);
      new (obj) Str(nbytes);
      in.read(obj->data(), nbytes);
      if (SCHWA_UNLIKELY(in.gcount() != nbytes))
        throw ReadError("Failed to read all of the str data in `read_dynamic`", in.gcount(), nbytes);
      return obj;
    }

  }
}

#endif  // SCHWA_MSGPACK_DYNAMIC_IMPL_H_
