/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/learn/extract.h>

#include <iostream>
#include <string>

namespace u = schwa::unicode;


namespace schwa {
namespace learn {


template <typename T>
static std::string
_word_form(const T &string) {
  std::string form;
  form.reserve(6);
  u::CategoryName prev_category = u::CategoryName::Cn;

  for (const auto code_point : string) {
    const auto category = u::get_category(code_point);
    if (category != prev_category)
      form.append(u::get_category_name(category));
    prev_category = category;
  }
  return form;
}


std::string
word_form(const UnicodeString &s) {
  return _word_form(s);
}


std::string
word_form(const std::string &utf8) {
  return _word_form(UTF8Decoder(utf8));
}


}  // namespace learn
}  // namespace schwa
