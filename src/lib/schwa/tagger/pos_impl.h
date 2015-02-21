/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TAGGER_POS_IMPL_H_
#define SCHWA_TAGGER_POS_IMPL_H_

#include <cstdio>
#include <cstring>
#include <memory>

#include <schwa/lex/word-form.h>
#include <schwa/unicode.h>


namespace schwa {
namespace tagger {
namespace pos {

// ========================================================================
// Extractor
// ========================================================================
template <typename TRANSFORM, typename VALUE>
inline void
Extractor::phase3_extract(canonical_schema::Sentence &sentence, canonical_schema::Token &token, learn::Features<TRANSFORM, VALUE> &features) {
  // Get the utf8 text of the current token.
  const size_t i = &token - sentence.span.start;
  const std::string &utf8 = _get_token_norm_raw(token);

  // Add w_{i}.
  features("w[i-2]=" + _offsets_token_norm_raw(i, -2));
  features("w[i-1]=" + _offsets_token_norm_raw(i, -1));
  features("w[i]=" + utf8);
  features("w[i+1]=" + _offsets_token_norm_raw(i,  1));
  features("w[i+2]=" + _offsets_token_norm_raw(i,  2));

  // Add Brown cluster features.
  const size_t npaths = _brown_clusters.get_paths(utf8, &_brown_cluster_path, _brown_cluster_path_lengths);
  if (npaths != 0) {
    const int prefix = std::sprintf(_brown_cluster_feature, "bc[i]=");
    std::memcpy(_brown_cluster_feature + prefix, _brown_cluster_path, _brown_cluster_path_lengths[npaths - 1]);
    for (size_t p = 0; p != npaths; ++p) {
      _brown_cluster_feature[prefix + _brown_cluster_path_lengths[npaths - 1 - p]] = '\0';
      features(_brown_cluster_feature);
    }
  }

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

}  // namespace pos
}  // namespace tagger
}  // namespace schwa

#endif  // SCHWA_TAGGER_POS_IMPL_H_
