/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEX_WORD_FORM_H_
#define SCHWA_LEX_WORD_FORM_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/unicode.h>


namespace schwa {
  namespace lex {

    std::string word_form(const std::string &utf8, bool collapse_adjacent=true);
    std::string word_form(const UnicodeString &s, bool collapse_adjacent=true);

  }
}

#endif  // SCHWA_LEX_WORD_FORM_H_
