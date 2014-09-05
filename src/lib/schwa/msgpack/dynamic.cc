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
// Array
// ============================================================================
Array::Array(uint32_t size) : _size(size) {
  for (uint32_t i = 0; i != _size; ++i)
    new (&get(i)) Value();
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


Map *
Map::create(Pool &pool, const uint32_t size) {
  void *ptr = pool.alloc(sizeof(Map) + size*sizeof(Pair));
  return new (ptr) Map(size);
}

}  // namespace msgpack
}  // namespace schwa
