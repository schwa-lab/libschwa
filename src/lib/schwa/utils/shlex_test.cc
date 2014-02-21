/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/utils/shlex.h>


namespace schwa {
namespace utils {

SUITE(schwa__utils__shlex) {

TEST(test_no_quote) {
  CHECK_EQUAL("hello", shlex_quote("hello"));
  CHECK_EQUAL("/dev/null", shlex_quote("/dev/null"));
  CHECK_EQUAL("./foo-bar_baz", shlex_quote("./foo-bar_baz"));
}

TEST(test_quote) {
  CHECK_EQUAL("'&'", shlex_quote("&"));
  CHECK_EQUAL("'one|two three;four'", shlex_quote("one|two three;four"));
}

TEST(test_nested_quote) {
  CHECK_EQUAL("'tim\"'\"s chicken'", shlex_quote("tim's chicken"));
}

}  // SUITE

}  // namespace utils
}  // namespace schwa
