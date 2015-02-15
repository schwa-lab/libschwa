/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_STRING_POOL_H_
#define SCHWA_UTILS_STRING_POOL_H_

#include <string>
#include <unordered_map>

#include <schwa/_base.h>
#include <schwa/pool.h>
#include <schwa/utils/hash.h>


namespace schwa {

  /**
   * Keeps a canonical version of string values so that the same string is not allocated more than
   * once. This is mainly useful for optimising RAM usage. If a number of long-standing objects
   * contain a lot of the same string, keeping those strings in a StringPool will save on space. A
   * good example of this is word embedding keys, brown cluster keys, etc. The tokens used to to
   * create these resources will have a reasonable overlap in their keyspace.
   **/
  class StringPool {
  private:
    Pool &_pool;
    std::unordered_map<const uint8_t *, const uint8_t *, uint8str_hash, uint8str_equal_to> _canonical;

    const uint8_t *_get_or_create(const uint8_t *orig, size_t nbytes);

  public:
    explicit StringPool(Pool &pool);
    ~StringPool(void);

    inline const uint8_t *
    get(const uint8_t *str) {
      return _get_or_create(str, std::strlen(reinterpret_cast<const char *>(str)) + 1);
    }

    inline const uint8_t *
    get(const char *str) {
      return _get_or_create(reinterpret_cast<const uint8_t *>(str), std::strlen(str) + 1);
    }

    inline const uint8_t *
    get(const std::string &str) {
      return _get_or_create(reinterpret_cast<const uint8_t *>(str.c_str()), str.size() + 1);
    }

    inline bool empty(void) const { return _canonical.empty(); }
    inline size_t size(void) const { return _canonical.size(); }
  };

}

#endif  // SCHWA_UTILS_STRING_POOL_H_
