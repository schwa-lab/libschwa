/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_DYNAMIC_H_
#define SCHWA_MSGPACK_DYNAMIC_H_

#include <schwa/_base.h>
#include <schwa/msgpack/enums.h>

namespace schwa {
  namespace msgpack {

    class Array;
    class Map;
    class Raw;


    class Value {
    public:
      WireType type;
      union {
        Array *_array;
        Map *_map;
        Raw *_raw;
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
      Value(WireType type, Array *value) : type(type) { via._array = value; }
      Value(WireType type, Map *value) : type(type) { via._map = value; }
      Value(WireType type, Raw *value) : type(type) { via._raw = value; }
      Value(WireType type, bool value) : type(type) { via._bool = value; }
      Value(WireType type, int8_t value) : type(type) { via._int8 = value; }
      Value(WireType type, int16_t value) : type(type) { via._int16 = value; }
      Value(WireType type, int32_t value) : type(type) { via._int32 = value; }
      Value(WireType type, int64_t value) : type(type) { via._int64 = value; }
      Value(WireType type, uint8_t value) : type(type) { via._uint8 = value; }
      Value(WireType type, uint16_t value) : type(type) { via._uint16 = value; }
      Value(WireType type, uint32_t value) : type(type) { via._uint32 = value; }
      Value(WireType type, uint64_t value) : type(type) { via._uint64 = value; }
      Value(WireType type, float value) : type(type) { via._float = value; }
      Value(WireType type, double value) : type(type) { via._double = value; }

      Value(const Value& o);
      Value &operator =(const Value &o);

      ~Value(void);
    };


    class Array {
    private:
      const uint32_t _size;

      explicit Array(uint32_t size);
      DISALLOW_COPY_AND_ASSIGN(Array);

    public:
      ~Array(void);

      Value &get(size_t index);
      const Value &get(size_t index) const;

      inline uint32_t size(void) const { return _size; }

      Value &operator [](size_t index) { return get(index); }
      const Value &operator [](size_t index) const { return get(index); }

      void operator delete(void *ptr);
      static Array *create(uint32_t size);
    };


    class Map {
    public:
      class Pair {
      public:
        Value key;
        Value value;

        Pair(void) { }
        Pair(const Value &key, const Value& value) : key(key), value(value) { }
      };

    private:
      const uint32_t _size;

      explicit Map(uint32_t size);
      DISALLOW_COPY_AND_ASSIGN(Map);

    public:
      ~Map(void);

      Pair &get(size_t index);
      const Pair &get(size_t index) const;

      inline uint32_t size(void) const { return _size; }

      Pair &operator [](size_t index) { return get(index); }
      const Pair &operator [](size_t index) const { return get(index); }

      void operator delete(void *ptr);
      static Map *create(uint32_t size);
    };


    class Raw {
    private:
      const uint32_t _size;
      const char *const _value;

      Raw(uint32_t size, const char *value);
      DISALLOW_COPY_AND_ASSIGN(Raw);

    public:
      ~Raw(void);

      inline uint32_t size(void) const { return _size; }
      inline const char *value(void) const { return _value; }

      static Raw *create(uint32_t size, const char *value);
    };

  }
}

#endif  // SCHWA_MSGPACK_DYNAMIC_H_
