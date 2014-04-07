/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TOKENIZER_COMMON_H_
#define SCHWA_TOKENIZER_COMMON_H_

#include <schwa/_base.h>

namespace schwa {
  namespace tokenizer {

    static constexpr const size_t DEFAULT_BUFFER_SIZE = 4*1024;
    static constexpr const size_t MAX_SENTENCE_LENGTH = 256;

    enum class OnError : uint8_t {
      SKIP,
      CALL,
      THROW,
    };

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
