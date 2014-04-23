/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/learn/extract.h>

#include <iostream>
#include <string>

#include <schwa/unicode.h>

namespace u = schwa::unicode;


namespace schwa {
namespace learn {


template <typename ForwardIterator>
static std::string
_word_form(ForwardIterator begin, ForwardIterator end) {
  std::string form;
  form.reserve(6);
  u::CategoryName prev_category = u::CategoryName::Cn;

  for ( ; begin != end; ++begin) {
    const auto category = u::get_category(*begin);
    if (category != prev_category)
      form.append(u::get_category_name(category));
    prev_category = category;
  }
  return form;
}


std::string
word_form(const UnicodeString &s) {
  return _word_form(s.begin(), s.end());
}


std::string
word_form(const std::string &utf8) {
  UTF8Decoder d(utf8);
  return _word_form(d.begin(), d.end());
}


}  // namespace learn
}  // namespace schwa
