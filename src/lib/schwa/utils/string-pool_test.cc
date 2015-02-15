/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/utils/string-pool.h>


namespace schwa {

SUITE(schwa__utils__string_pool) {

TEST(stringpool) {
  Pool pool(4 * 1024 * 1024);
  StringPool sp(pool);
  const uint8_t *s0, *s1;

  CHECK_EQUAL(true, sp.empty());
  CHECK_EQUAL(0, sp.size());

  s0 = sp.get("hello");
  CHECK_EQUAL(false, sp.empty());
  CHECK_EQUAL(1, sp.size());
  CHECK(s0 != nullptr);
  CHECK_EQUAL("hello", reinterpret_cast<const char *>(s0));

  s1 = sp.get("hello");
  CHECK_EQUAL(false, sp.empty());
  CHECK_EQUAL(1, sp.size());
  CHECK_EQUAL(static_cast<const void *>(s0), static_cast<const void *>(s1));
  CHECK(s1 != nullptr);

  s0 = sp.get("Hello");
  CHECK_EQUAL(false, sp.empty());
  CHECK_EQUAL(2, sp.size());
  CHECK(s0 != nullptr);
  CHECK(s0 != s1);
  CHECK_EQUAL("Hello", reinterpret_cast<const char *>(s0));

  s1 = sp.get(std::string("Hello"));
  CHECK_EQUAL(2, sp.size());
  CHECK(s1 != nullptr);
  CHECK_EQUAL(static_cast<const void *>(s0), static_cast<const void *>(s1));
}

}  // SUITE

}  // namespace schwa
