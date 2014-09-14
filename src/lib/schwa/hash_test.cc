/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <type_traits>

#include <schwa/hash.h>
#include <schwa/unicode.h>
#include <schwa/unittest.h>


namespace schwa {

static constexpr const uint64_t MEOW_HASH64 = 13551622899263484255ULL;
static constexpr const uint64_t WOOF_HASH64 = 6574843345855820858ULL;
static constexpr const uint32_t MEOW_HASH32 = 1806104304U;
static constexpr const uint32_t WOOF_HASH32 = 768325760U;

SUITE(schwa__hash) {

TEST(test_string_hash) {
  const std::string meow("meow");
  const std::string woof("woof");

  Hasher64<std::string> h64;
  static_assert(std::is_same<decltype(h64)::argument_type, std::string>::value, "argument_type incorrect");
  static_assert(std::is_same<decltype(h64)::result_type, uint64_t>::value, "result_type incorrect");
  CHECK_EQUAL(MEOW_HASH64, h64(meow));
  CHECK_EQUAL(WOOF_HASH64, h64(woof));
  CHECK_EQUAL(MEOW_HASH64, hash64(meow));
  CHECK_EQUAL(WOOF_HASH64, hash64(woof));

  Hasher32<std::string> h32;
  static_assert(std::is_same<decltype(h32)::argument_type, std::string>::value, "argument_type incorrect");
  static_assert(std::is_same<decltype(h32)::result_type, uint32_t>::value, "result_type incorrect");
  CHECK_EQUAL(MEOW_HASH32, h32(meow));
  CHECK_EQUAL(WOOF_HASH32, h32(woof));
  CHECK_EQUAL(MEOW_HASH32, hash32(meow));
  CHECK_EQUAL(WOOF_HASH32, hash32(woof));
}


TEST(test_cstring_hash) {
  const char *const meow = "meow";
  const char *const woof = "woof";

  Hasher64<const char *> h64;
  static_assert(std::is_same<decltype(h64)::argument_type, const char *>::value, "argument_type incorrect");
  static_assert(std::is_same<decltype(h64)::result_type, uint64_t>::value, "result_type incorrect");
  CHECK_EQUAL(MEOW_HASH64, h64(meow));
  CHECK_EQUAL(WOOF_HASH64, h64(woof));
  CHECK_EQUAL(MEOW_HASH64, hash64(meow));
  CHECK_EQUAL(WOOF_HASH64, hash64(woof));

  Hasher32<const char *> h32;
  static_assert(std::is_same<decltype(h32)::argument_type, const char *>::value, "argument_type incorrect");
  static_assert(std::is_same<decltype(h32)::result_type, uint32_t>::value, "result_type incorrect");
  CHECK_EQUAL(MEOW_HASH32, h32(meow));
  CHECK_EQUAL(WOOF_HASH32, h32(woof));
  CHECK_EQUAL(MEOW_HASH32, hash32(meow));
  CHECK_EQUAL(WOOF_HASH32, hash32(woof));
}


TEST(test_unicodestring_hash) {
  const UnicodeString meow("meow");
  const UnicodeString woof("woof");

  Hasher64<UnicodeString> h64;
  static_assert(std::is_same<decltype(h64)::argument_type, UnicodeString>::value, "argument_type incorrect");
  static_assert(std::is_same<decltype(h64)::result_type, uint64_t>::value, "result_type incorrect");
  CHECK_EQUAL(MEOW_HASH64, h64(meow));
  CHECK_EQUAL(WOOF_HASH64, h64(woof));
  CHECK_EQUAL(MEOW_HASH64, hash64(meow));
  CHECK_EQUAL(WOOF_HASH64, hash64(woof));

  Hasher32<UnicodeString> h32;
  static_assert(std::is_same<decltype(h32)::argument_type, UnicodeString>::value, "argument_type incorrect");
  static_assert(std::is_same<decltype(h32)::result_type, uint32_t>::value, "result_type incorrect");
  CHECK_EQUAL(MEOW_HASH32, h32(meow));
  CHECK_EQUAL(WOOF_HASH32, h32(woof));
  CHECK_EQUAL(MEOW_HASH32, hash32(meow));
  CHECK_EQUAL(WOOF_HASH32, hash32(woof));
}


TEST(test_class) {
  class X {
  private:
    std::string _s;

  public:
    explicit X(const std::string &s) : _s(s) { }

    inline uint32_t hash32(void) const { return schwa::hash32(_s); }
    inline uint64_t hash64(void) const { return schwa::hash64(_s); }
  };

  const X meow("meow");
  const X woof("woof");

  Hasher64<X> h64;
  static_assert(std::is_same<decltype(h64)::argument_type, X>::value, "argument_type incorrect");
  static_assert(std::is_same<decltype(h64)::result_type, uint64_t>::value, "result_type incorrect");
  CHECK_EQUAL(MEOW_HASH64, h64(meow));
  CHECK_EQUAL(WOOF_HASH64, h64(woof));
  CHECK_EQUAL(MEOW_HASH64, hash64(meow));
  CHECK_EQUAL(WOOF_HASH64, hash64(woof));

  Hasher32<X> h32;
  static_assert(std::is_same<decltype(h32)::argument_type, X>::value, "argument_type incorrect");
  static_assert(std::is_same<decltype(h32)::result_type, uint32_t>::value, "result_type incorrect");
  CHECK_EQUAL(MEOW_HASH32, h32(meow));
  CHECK_EQUAL(WOOF_HASH32, h32(woof));
  CHECK_EQUAL(MEOW_HASH32, hash32(meow));
  CHECK_EQUAL(WOOF_HASH32, hash32(woof));
}

}  // SUITE

}  // namespace schwa
