/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/msgpack/dynamic.h>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <new>

#include <schwa/pool.h>

namespace schwa {
namespace msgpack {

// ============================================================================
// Value
// ============================================================================
Value::Value(const Value &o) {
  memcpy(this, &o, sizeof(Value));
}

Value &
Value::operator =(const Value &o) {
  memcpy(this, &o, sizeof(Value));
  return *this;
}


// ============================================================================
// Array
// ============================================================================
Array::Array(uint32_t size) : _size(size) {
  for (uint32_t i = 0; i != _size; ++i)
    new (&get(i)) Value();
}


Value &
Array::get(size_t index) {
  return reinterpret_cast<Value *>(this + 1)[index];
}


const Value &
Array::get(size_t index) const {
  return reinterpret_cast<const Value *>(this + 1)[index];
}


Array *
Array::create(Pool &pool, const uint32_t size) {
  void *ptr = pool.alloc(sizeof(Array) + size*sizeof(Value));
  return new (ptr) Array(size);
}


// ============================================================================
// Map
// ============================================================================
Map::Map(uint32_t size) : _size(size) {
  for (uint32_t i = 0; i != _size; ++i)
    new (&get(i)) Pair();
}


Map::Pair &
Map::get(size_t index) {
  return reinterpret_cast<Pair *>(this + 1)[index];
}


const Map::Pair &
Map::get(size_t index) const {
  return reinterpret_cast<const Pair *>(this + 1)[index];
}


Map *
Map::create(Pool &pool, const uint32_t size) {
  void *ptr = pool.alloc(sizeof(Map) + size*sizeof(Pair));
  return new (ptr) Map(size);
}


// ============================================================================
// Raw
// ============================================================================
Raw::Raw(uint32_t size, const char *value) : _size(size), _value(value) { }


Raw *
Raw::create(Pool &pool, uint32_t size, const char *value) {
  void *ptr = pool.alloc(sizeof(Raw));
  return new (ptr) Raw(size, value);
}

}  // namespace msgpack
}  // namespace schwa
