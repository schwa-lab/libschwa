/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/msgpack/dynamic.h>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <new>


namespace schwa {
namespace msgpack {

// ============================================================================
// Array
// ============================================================================
Array *
Array::create(const uint32_t size) {
  Array *ptr = static_cast<Array *>(std::malloc(sizeof(Array) + size*sizeof(Value)));
  if (ptr == nullptr)
    throw std::bad_alloc();
  return new (ptr) Array(size);
}

Array::Array(uint32_t size) : _size(size) { }

Array::~Array(void) {
  std::free(this);
}

Value *
Array::get(size_t index) {
  return reinterpret_cast<Value *>(this + 1) + index;
}

const Value *
Array::get(size_t index) const {
  return reinterpret_cast<const Value *>(this + 1) + index;
}


// ============================================================================
// Map
// ============================================================================
Map *
Map::create(const uint32_t size) {
  Map *ptr = static_cast<Map *>(std::malloc(sizeof(Map) + size*sizeof(Pair)));
  if (ptr == nullptr)
    throw std::bad_alloc();
  return new (ptr) Map(size);
}

Map::Map(uint32_t size) : _size(size) { }

Map::~Map(void) {
  std::free(this);
}

Map::Pair &
Map::get(size_t index) {
  return reinterpret_cast<Pair *>(this + 1)[index];
}

const Map::Pair &
Map::get(size_t index) const {
  return reinterpret_cast<const Pair *>(this + 1)[index];
}


// ============================================================================
// Raw
// ============================================================================
Raw *
Raw::create(uint32_t size, const char *value) {
  return new Raw(size, value);
}

Raw::Raw(uint32_t size, const char *value) : _size(size), _value(value) { }

Raw::~Raw(void) { }


// ============================================================================
// Value
// ============================================================================
Value::Value(const Value& o) {
  memcpy(this, &o, sizeof(Value));
}

Value &
Value::operator =(const Value& o) {
  memcpy(this, &o, sizeof(Value));
  return *this;
}

}  // namespace msgpack
}  // namespace schwa
