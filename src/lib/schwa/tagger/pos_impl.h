/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TAGGER_POS_IMPL_H_
#define SCHWA_TAGGER_POS_IMPL_H_

#include <schwa/unicode.h>


namespace schwa {
  namespace tagger {

    // ========================================================================
    // POSExtractor
    // ========================================================================
    template <typename TRANSFORM, typename VALUE>
    inline void
    POSExtractor::phase2_extract(canonical_schema::Token &token, const size_t i, learn::Features<TRANSFORM, VALUE> &features) {
      // Get the utf8 text of the current token.
      const std::string &utf8 = _get_token_norm_raw(token);

      // Add w_{i}.
      features("w[i-2]=" + _offsets_token_norm_raw(i, -2));
      features("w[i-1]=" + _offsets_token_norm_raw(i, -1));
      features("w[i]=" + utf8);
      features("w[i+1]=" + _offsets_token_norm_raw(i,  1));
      features("w[i+2]=" + _offsets_token_norm_raw(i,  2));

      // Is the current token rare?
      if (_lexicon.count(utf8) < _rare_token_cutoff) {
        // Prefix and suffix of length up to 4.
        learn::add_affix_features(features, 4, 4, utf8);

        // Word form.
        features("wf[i]=" + learn::word_form(utf8));
      }
      else {
        // Add a feature to say it's not a rare word.
        features("in_lex=tokens");
      }
    }

  }
}

#endif  // SCHWA_TAGGER_POS_IMPL_H_
