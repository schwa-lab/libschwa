/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_DYNAMIC_H_
#define SCHWA_MSGPACK_DYNAMIC_H_

#include <cstring>

#include <schwa/_base.h>
#include <schwa/pool.h>
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
        int64_t _int64;
        uint64_t _uint64;
        float _float;
        double _double;
      } via;

      Value(void) : type(WireType::RESERVED) { }
      explicit Value(WireType type) : type(type) { }
      Value(const Value &o) { std::memcpy(this, &o, sizeof(Value)); }
      Value(const Value &&o) { std::memcpy(this, &o, sizeof(Value)); }
      Value(WireType type, Array *value) : type(type) { via._array = value; }
      Value(WireType type, Map *value) : type(type) { via._map = value; }
      Value(WireType type, Raw *value) : type(type) { via._raw = value; }
      Value(WireType type, bool value) : type(type) { via._bool = value; }
      Value(WireType type, int64_t value) : type(type) { via._int64 = value; }
      Value(WireType type, uint64_t value) : type(type) { via._uint64 = value; }
      Value(WireType type, float value) : type(type) { via._float = value; }
      Value(WireType type, double value) : type(type) { via._double = value; }
      Value &operator =(const Value &o) { std::memcpy(this, &o, sizeof(Value)); return *this; }
      ~Value(void) { }

      inline void operator delete(void *) { }

      template <typename... Args>
      static inline Value *
      create(Pool &pool, Args... args) {
        void *ptr = pool.alloc(sizeof(Value));
        return new (ptr) Value(args...);
      }
    };


    class Array {
    private:
      const uint32_t _size;

      explicit Array(uint32_t size);

    public:
      ~Array(void) { }

      inline Value &get(size_t index) { return reinterpret_cast<Value *>(this + 1)[index]; }
      inline const Value &get(size_t index) const { return reinterpret_cast<const Value *>(this + 1)[index]; }

      inline uint32_t size(void) const { return _size; }

      inline Value &operator [](size_t index) { return get(index); }
      inline const Value &operator [](size_t index) const { return get(index); }

      inline void operator delete(void *) { }
      static Array *create(Pool &pool, uint32_t size);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Array);
    };


    class Map {
    public:
      class Pair {
      public:
        Value key;
        Value value;

        Pair(void) { }
        Pair(const Value &key, const Value& value) : key(key), value(value) { }
        ~Pair(void) { }
      };

    private:
      const uint32_t _size;

      explicit Map(uint32_t size);

    public:
      ~Map(void) { }

      inline Pair &get(size_t index) { return reinterpret_cast<Pair *>(this + 1)[index]; }
      inline const Pair &get(size_t index) const { return reinterpret_cast<const Pair *>(this + 1)[index]; }

      inline uint32_t size(void) const { return _size; }

      inline Pair &operator [](size_t index) { return get(index); }
      inline const Pair &operator [](size_t index) const { return get(index); }

      inline void operator delete(void *) { }
      static Map *create(Pool &pool, uint32_t size);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Map);
    };


    class Raw {
    private:
      const uint32_t _size;
      const char *const _value;

      Raw(uint32_t size, const char *value);

    public:
      ~Raw(void) { }

      inline uint32_t size(void) const { return _size; }
      inline const char *value(void) const { return _value; }

      inline void operator delete(void *) { }
      static Raw *create(Pool &pool, uint32_t size, const char *value);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Raw);
    };

  }
}

#endif  // SCHWA_MSGPACK_DYNAMIC_H_
