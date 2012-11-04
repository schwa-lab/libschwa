/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_DYNAMIC_H_
#define SCHWA_MSGPACK_DYNAMIC_H_

#include <schwa/_base.h>
#include <schwa/msgpack/enums.h>

namespace schwa {
  namespace msgpack {

    class Value;

    class Array {
    private:
      const uint32_t _size;

      Array(uint32_t size);

    public:
      ~Array(void);

      Value *operator [](size_t index);
      const Value *operator [](size_t index) const;

      inline uint32_t size(void) const { return _size; }

      static Array *create(uint32_t size);

    private:
      DISALLOW_COPY_AND_ASSIGN(Array);
    };


    class Map {
    public:
    };


    class Raw {
    };


    class Value {
    public:
      WireType type;
      union {
        Array *array;
        Map *map;
        Raw *raw;
        bool _bool;
        int8_t _int8;
        int16_t _int16;
        int32_t _int32;
        int64_t _int64;
        uint8_t _uint8;
        uint16_t _uint16;
        uint32_t _uint32;
        uint64_t _uint64;
        float _float;
        double _double;
      } via;

      Value(void) : type(WireType::RESERVED) { }
      explicit Value(WireType type) : type(type) { }

    private:
      DISALLOW_COPY_AND_ASSIGN(Value);
    };

  }
}

#endif  // SCHWA_MSGPACK_DYNAMIC_H_
