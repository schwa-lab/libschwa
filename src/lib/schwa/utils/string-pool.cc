/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/utils/string-pool.h>

#include <cstring>


namespace schwa {

StringPool::StringPool(Pool &pool) : _pool(pool) { }

StringPool::~StringPool(void) { }


const uint8_t *
StringPool::_get_or_create(const uint8_t *const orig, const size_t nbytes) {
  // If the string already has a canonical version, return the canonical version.
  const auto &it = _canonical.find(orig);
  if (it != _canonical.end())
    return it->second;

  // Allocate space in the pool for the canonical version.
  uint8_t *canonical = _pool.alloc<uint8_t *>(nbytes);
  std::memcpy(canonical, orig, nbytes);

  // Insert the canonical representation into our map and return.
  _canonical.emplace(canonical, canonical);
  return canonical;
}

}  // namespace schwa
