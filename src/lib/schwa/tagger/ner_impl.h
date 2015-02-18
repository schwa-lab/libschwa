/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TAGGER_NER_IMPL_H_
#define SCHWA_TAGGER_NER_IMPL_H_

#include <sstream>

#include <schwa/lex/word-form.h>
#include <schwa/unicode.h>


namespace schwa {
namespace tagger {
namespace ner {

// ========================================================================
// Extractor
// ========================================================================
template <typename TRANSFORM, typename VALUE>
inline void
Extractor::phase2_extract(canonical_schema::Token &token, const size_t i, learn::Features<TRANSFORM, VALUE> &features) {
  // Decode the UTF-8 sequence into to a Unicode string once only.
  const std::string &utf8 = _get_token_ne_normalised(token);
  const UnicodeString u = UnicodeString::from_utf8(utf8);
  std::stringstream ss;

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

  // Compute features that are in a +/-2 window of i.
  std::stringstream ss_ctx1, ss_ctx2;
  ss_ctx1 << "ctx[i]_1=";
  ss_ctx2 << "ctx[i]_2=";
  for (int offset = -2; offset != 3; ++offset) {
    const std::string &ne_normalised = _offsets_token_ne_normalised(i, offset);
    const bool is_sentinel = ne_normalised == learn::SENTINEL;
    //std::cout << "i=" << i << " offset=" << offset << " ne_normalised='" << ne_normalised << "' is_sentinel=" << is_sentinel << std::endl;

    // Add the NE-normalised token w_{i+j}.
    ss << ((offset < 0) ? "w[i" : "w[i+") << offset << "]=" << ne_normalised;
    features(ss.str());
    ss.str("");

    // Capitalisation patterns in a +/-2  and +/-1 window around w_{i}.
    if (!is_sentinel) {
      const std::string category = unicode::get_category_name(unicode::get_category(*UTF8Decoder(ne_normalised).cbegin()));
      ss_ctx2 << category;
      if (offset != -2 && offset != 2)
        ss_ctx1 << category;
    }
  }
  // Capitalisation patterns in a +/-2  and +/-1 window around w_{i}.
  features(ss_ctx1.str());
  features(ss_ctx2.str());

  // If the word contains a hyphen, add each of the components split by the hyphen.
  if (u.size() > 1) {
    UnicodeString part;
    part.reserve(u.size());
    unsigned int part_count = 0;
    for (size_t j = 0; j != u.size(); ++j) {
      const unicode_t cp = u[j];
      if (unicode::is_hyphen(cp)) {
        if (!part.empty()) {
          ss << "w[i]_part" << part_count << "=" << part.to_utf8();
          features(ss.str());
          ss.str("");
          ++part_count;
          part.clear();
        }
      }
      else
        part.push_back(cp);
    }
    if (part.size() != 0 && part.size() != u.size()) {
      ss << "w[i]_part" << part_count << "=" << part.to_utf8();
      features(ss.str());
      ss.str("");
    }
  }

  for (int offset = 0; offset != 1; ++offset) {
    const std::string &norm = _offsets_token_norm_raw(i, offset);

    // Add Brown cluster paths for w_{i}.
    const size_t npaths = _brown_clusters.get_paths(norm, &_brown_cluster_path, _brown_cluster_path_lengths);
    if (npaths != 0) {
      const int prefix = std::sprintf(_brown_cluster_feature, "bc[i%+d]=", offset);
      std::memcpy(_brown_cluster_feature + prefix, _brown_cluster_path, _brown_cluster_path_lengths[npaths - 1]);
      for (size_t p = 0; p != npaths; ++p) {
        _brown_cluster_feature[prefix + _brown_cluster_path_lengths[npaths - 1 - p]] = '\0';
        features(_brown_cluster_feature);
      }
    }

    // Add word embedding values for w_{i}
    const float *embeddings = _word_embeddings.get_embeddings(norm);
    if (SCHWA_LIKELY(embeddings != nullptr)) {
      char buf[14];  // we[i+2]=ddddd
      const int prefix = std::sprintf(buf, "we[i%+d]=", offset);
      for (uint32_t j = 0; j != _word_embeddings.ndimensions(); ++j) {
        std::sprintf(buf + prefix, "%u", j);
        features(buf, embeddings[j]);
      }
    }
  }

}

}  // namespace ner
}  // namespace tagger
}  // namespace schwa

#endif  // SCHWA_TAGGER_NER_IMPL_H_
