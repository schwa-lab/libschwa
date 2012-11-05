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
Array::Array(uint32_t size) : _size(size) {
  for (uint32_t i = 0; i != _size; ++i)
    new (&get(i)) Value();
}

Array::~Array(void) {
  for (uint32_t i = 0; i != _size; ++i)
    get(i).~Value();
}

Value &
Array::get(size_t index) {
  return reinterpret_cast<Value *>(this + 1)[index];
}

const Value &
Array::get(size_t index) const {
  return reinterpret_cast<const Value *>(this + 1)[index];
}

void
Array::operator delete(void *ptr) {
  std::free(ptr);
}

Array *
Array::create(const uint32_t size) {
  Array *ptr = static_cast<Array *>(std::malloc(sizeof(Array) + size*sizeof(Value)));
  if (ptr == nullptr)
    throw std::bad_alloc();
  return new (ptr) Array(size);
}


// ============================================================================
// Map
// ============================================================================
Map::Map(uint32_t size) : _size(size) {
  for (uint32_t i = 0; i != _size; ++i)
    new (&get(i)) Pair();
}

Map::~Map(void) {
  for (uint32_t i = 0; i != _size; ++i)
    get(i).~Pair();
}

Map::Pair &
Map::get(size_t index) {
  return reinterpret_cast<Pair *>(this + 1)[index];
}

const Map::Pair &
Map::get(size_t index) const {
  return reinterpret_cast<const Pair *>(this + 1)[index];
}

void
Map::operator delete(void *ptr) {
  std::free(ptr);
}

Map *
Map::create(const uint32_t size) {
  Map *ptr = static_cast<Map *>(std::malloc(sizeof(Map) + size*sizeof(Pair)));
  if (ptr == nullptr)
    throw std::bad_alloc();
  return new (ptr) Map(size);
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

Value::~Value(void) {
  switch (type) {
  case WireType::ARRAY_FIXED:
  case WireType::ARRAY_16:
  case WireType::ARRAY_32:
    delete via._array;
    break;
  case WireType::MAP_FIXED:
  case WireType::MAP_16:
  case WireType::MAP_32:
    delete via._map;
    break;
  case WireType::RAW_FIXED:
  case WireType::RAW_16:
  case WireType::RAW_32:
    delete via._raw;
    break;
  default:
    break;
  }
}

}  // namespace msgpack
}  // namespace schwa
