/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TAGGER_POS_IMPL_H_
#define SCHWA_TAGGER_POS_IMPL_H_

#include <schwa/lex/word-form.h>
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
        // Decode the UTF-8 sequence into to a Unicode string once only.
        const UnicodeString u = UnicodeString::from_utf8(utf8);

        // Prefix and suffix of length up to 4.
        learn::add_affix_features(features, 4, 4, utf8);

        // Word form.
        features("wf[i]=" + lex::word_form(u));

        // Contains a digit, hyphen, or uppercase code point.
        bool has_digit = false, has_hyphen = false, has_upper = false;
        for (const unicode_t cp : u) {
          if (!has_digit && unicode::is_digit(cp))
            has_digit = true;
          if (!has_hyphen && (unicode::is_hyphen(cp) || unicode::is_dash(cp)))
            has_hyphen = true;
          if (!has_upper && unicode::is_upper(cp))
            has_upper = true;
        }
        if (has_digit)
          features("has_digit");
        if (has_hyphen)
          features("has_hyphen");
        if (has_upper)
          features("has_upper");
      }
    }

  }
}

#endif  // SCHWA_TAGGER_POS_IMPL_H_
