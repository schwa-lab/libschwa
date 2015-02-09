/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TAGGER_POS_IMPL_H_
#define SCHWA_TAGGER_POS_IMPL_H_


namespace schwa {
  namespace tagger {

    // ========================================================================
    // POSExtractor
    // ========================================================================
    template <typename TRANSFORM, typename VALUE>
    inline void
    POSExtractor::phase2_extract(canonical_schema::Token &token, size_t, learn::Features<TRANSFORM, VALUE> &features) {
      // Get the utf8 text of the current token.
      const std::string &utf8 = _get_token_norm_raw(token);

      // Add w_{i}.
      features("w[i]=" + utf8);

      // Prefix and suffix of length up to 5.
      learn::add_affix_features(features, 5, 5, utf8);

      // Is the current token in the lexicon?
      if (_lex_token.contains(utf8))
        features("in_lex=tokens");
    }

  }
}

#endif  // SCHWA_TAGGER_POS_IMPL_H_
