/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/utils/string.h>


namespace schwa {

SUITE(schwa__utils__string) {

TEST(ltrim) {
  std::string s;
  s = "";
  CHECK_EQUAL("", ltrim(s));
  s = "meow";
  CHECK_EQUAL("meow", ltrim(s));
  s = " ";
  CHECK_EQUAL("", ltrim(s));
  s = "     ";
  CHECK_EQUAL("", ltrim(s));
  s = " \t  hello ";
  CHECK_EQUAL("hello ", ltrim(s));
  s = "             x y z a b c";
  CHECK_EQUAL("x y z a b c", ltrim(s));
}


TEST(rtrim) {
  std::string s;
  s = "";
  CHECK_EQUAL("", rtrim(s));
  s = "meow";
  CHECK_EQUAL("meow", rtrim(s));
  s = " ";
  CHECK_EQUAL("", rtrim(s));
  s = "    ";
  CHECK_EQUAL("", rtrim(s));
  s = " hello   \t  ";
  CHECK_EQUAL(" hello", rtrim(s));
  s = "x y z a b c                 ";
  CHECK_EQUAL("x y z a b c", rtrim(s));
}


TEST(trim) {
  std::string s;
  s = "";
  CHECK_EQUAL("", trim(s));
  s = "meow";
  CHECK_EQUAL("meow", trim(s));
  s = " ";
  CHECK_EQUAL("", trim(s));
  s = "      ";
  CHECK_EQUAL("", trim(s));
  s = "  hello     \t  ";
  CHECK_EQUAL("hello", trim(s));
  s = "    x y z a b c                 ";
  CHECK_EQUAL("x y z a b c", trim(s));
}

}  // SUITE

}  // namespace schwa
