/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace tokenizer {

    class TokenError : public Exception {
    public:
      TokenError(const std::string &msg) : Exception(msg) { }
      TokenError(const TokenError &other) : Exception(other.msg) { }
      virtual ~TokenError(void) throw() { }
    };

  }
}
