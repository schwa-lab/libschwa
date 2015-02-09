/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/learn/extract.h>

#include <iostream>
#include <sstream>
#include <string>

namespace u = schwa::unicode;


namespace schwa {
namespace learn {

const std::string SENTINEL = "__SENTINEL__";


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


// ============================================================================
// Windower
// ============================================================================
Windower::Windower(const std::string &name, const int8_t delta_left, const int8_t delta_right) :
    _delta_left(delta_left),
    _delta_right(delta_right)
  {
  // Construct the attribute prefix.
  std::stringstream prefix;
  prefix << name << '[';
  for (int8_t delta = _delta_left; delta <= _delta_right; ++delta) {
    if (delta != _delta_left)
      prefix << ',';
    prefix << 'i';
    if (delta < 0)
      prefix << delta;
    else if (delta > 0)
      prefix << '+' << delta;
  }
  prefix << '=';
  _prefix = prefix.str();
}

}  // namespace learn
}  // namespace schwa
