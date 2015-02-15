/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/lex/word-form.h>


namespace schwa {
namespace lex {

template <typename T>
static std::string
_word_form(const T &string, const bool collapse_adjacent) {
  std::string form;
  form.reserve(6);
  unicode::CategoryName prev_category = unicode::CategoryName::Cn;

  for (const auto code_point : string) {
    const auto category = unicode::get_category(code_point);
    if (!collapse_adjacent || category != prev_category)
      form.append(unicode::get_category_name(category));
    prev_category = category;
  }
  return form;
}


std::string
word_form(const UnicodeString &s, const bool collapse_adjacent) {
  return _word_form(s, collapse_adjacent);
}


std::string
word_form(const std::string &utf8, const bool collapse_adjacent) {
  return _word_form(UTF8Decoder(utf8), collapse_adjacent);
}

}  // namespace lex
}  // namespace schwa
