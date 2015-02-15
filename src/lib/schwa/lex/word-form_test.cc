/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/lex/word-form.h>


namespace schwa {
namespace lex {

SUITE(schwa__lex__word_form) {

TEST(word_form_collapsed) {
  CHECK_EQUAL("Ll", word_form("hello"));
  CHECK_EQUAL("Lu", word_form("HELLO"));
  CHECK_EQUAL("LuLl", word_form("Hello"));
  CHECK_EQUAL("LlLuLlLuLl", word_form("hElLo"));
  CHECK_EQUAL("LuLlNd", word_form("Hell9"));
  CHECK_EQUAL("", word_form(""));

  CHECK_EQUAL("Ll", word_form(UnicodeString(U"hello")));
  CHECK_EQUAL("Lu", word_form(UnicodeString(U"HELLO")));
  CHECK_EQUAL("LuLl", word_form(UnicodeString(U"Hello")));
  CHECK_EQUAL("LlLuLlLuLl", word_form(UnicodeString(U"hElLo")));
  CHECK_EQUAL("LuLlNd", word_form(UnicodeString(U"Hell9")));
  CHECK_EQUAL("", word_form(UnicodeString(U"")));
}


TEST(word_form_uncollapsed) {
  CHECK_EQUAL("LlLlLlLlLl", word_form("hello", false));
  CHECK_EQUAL("LuLuLuLuLu", word_form("HELLO", false));
  CHECK_EQUAL("LuLlLlLlLl", word_form("Hello", false));
  CHECK_EQUAL("LlLuLlLuLl", word_form("hElLo", false));
  CHECK_EQUAL("LuLlLlLlNd", word_form("Hell9", false));
  CHECK_EQUAL("", word_form("", false));
}

}  // SUITE

}  // namespace lex
}  // namespace schwa
