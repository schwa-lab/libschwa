/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TOKENIZER_COMMON_H_
#define SCHWA_TOKENIZER_COMMON_H_

#include <schwa/_base.h>

namespace schwa {
  namespace tokenizer {

    typedef int64_t offset_type;

    enum Type {
      WORD,
      NUMBER,
      UNIT,
      ABBREVIATION,
      CONTRACTION,
      POSSESSIVE,
      PUNCTUATION,
      QUOTE,
      DASH,
      TERMINATOR
    };

  }
}

#endif  // SCHWA_TOKENIZER_COMMON_H_
