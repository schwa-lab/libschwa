/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TOKENIZER_EXCEPTION_H_
#define SCHWA_TOKENIZER_EXCEPTION_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>

namespace schwa {
  namespace tokenizer {

    class TokenError : public Exception {
    public:
      TokenError(const std::string &msg) : Exception(msg) { }
      TokenError(const TokenError &o) : Exception(o) { }
      virtual ~TokenError(void) throw() { }
    };

  }
}

#endif  // SCHWA_TOKENIZER_EXCEPTION_H_
