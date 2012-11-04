/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/msgpack/dynamic.h>

#include <cassert>
#include <cstdlib>
#include <new>


namespace schwa {
namespace msgpack {

Array *
Array::create(const uint32_t size) {
  Array *ptr = static_cast<Array *>(std::malloc(sizeof(Array) + size*sizeof(Value)));
  assert(ptr != nullptr);
  return new (ptr) Array(size);
}

Array::Array(uint32_t size) : _size(size) { }

Array::~Array(void) {
  std::free(this);
}

}  // namespace msgpack
}  // namespace schwa
