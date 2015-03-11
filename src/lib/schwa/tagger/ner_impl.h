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
Extractor::phase3_extract(canonical_schema::Sentence &sentence, canonical_schema::Token &token, learn::Features<TRANSFORM, VALUE> &features) {
  using ::schwa::third_party::re2::RE2;

  // Decode the UTF-8 sequence into to a Unicode string once only.
  const size_t i = &token - sentence.span.start;
  const std::string &utf8 = _get_token_ne_normalised(token);
  const UnicodeString u = UnicodeString::from_utf8(utf8);
  const UnicodeString lower = u.to_lower();
  std::stringstream ss;

  // Prefix and suffix.
  learn::add_affix_features(features, 5, 5, utf8);

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

  // Person-initial features to help match "Cricket: P. Smith and A. Fitz blah blah"
  //                                                 ^^^^^^^^     ^^^^^^^
  if (sentence.span.start + i != sentence.span.stop && RE2::FullMatch(utf8, RE_PERSON_INITIAL_1) && RE2::FullMatch(_get_token_ne_normalised(*(sentence.span.start + i + 1)), RE_PERSON_INITIAL_2))
    features("person_initial_1");
  if (i != 0 && RE2::FullMatch(utf8, RE_PERSON_INITIAL_2) && RE2::FullMatch(_get_token_ne_normalised(*(sentence.span.start + i - 1)), RE_PERSON_INITIAL_1))
    features("person_initial_2");

#if 0
  // Obvious start-of-sentence surname attribution: Duran , whose 99-99 record
  //                                                ^^^^^
  if (i == 0 && sentence.span.stop - sentence.span.start >= 3 && _get_token_ne_normalised(sentence.span.start[1]) == "," && RE2::FullMatch(_get_token_ne_normalised(sentence.span.start[2]), RE_WHO_PRONOUN) && RE2::FullMatch(utf8, RE_PERSON_INITIAL_2))
    features("person_who");
#endif

  // Obvious locations: ... at Lord 's
  //                           ^^^^
  if (i != 0 && _get_token_ne_normalised(*(sentence.span.start + i - 1)) == "at" && RE2::FullMatch(utf8, RE_PERSON_INITIAL_2) && !RE2::FullMatch(utf8, RE_TEMPORAL))
    features("at_location");

  // Does the token look like an acronym?
  if (RE2::FullMatch(utf8, RE_ACRONYM))
    features("acronym");

  // Is the current token a roman numeral looking thing which occurs after a possessive or capitalised word?
  if (i != 0 && RE2::FullMatch(utf8, RE_ROMAN_NUMERAL)) {
    const std::string &prev_ne_normalised = _get_token_ne_normalised(*(sentence.span.start + i - 1));
    if (RE2::FullMatch(prev_ne_normalised, RE_PERSON_INITIAL_2) || prev_ne_normalised == "'s")
      features("roman_numeral");
  }

  // Compute features that are in a +/-2 window of i.
  std::stringstream ss_ctx1, ss_ctx2;
  ss_ctx1 << "ctx[i]_1=";
  ss_ctx2 << "ctx[i]_2=";
  for (int offset = -2; offset != 3; ++offset) {
    const std::string &ne_normalised = _offsets_token_ne_normalised(i, offset);
    const bool is_sentinel = ne_normalised == learn::SENTINEL;

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

  // Add Brown cluster paths for w_{i}.
  if (_feature_flags.use_brown_cluster_features) {
    const size_t npaths = _brown_clusters.get_paths(utf8, &_brown_cluster_path, _brown_cluster_path_lengths);
    if (npaths != 0) {
      const int prefix = std::sprintf(_brown_cluster_feature, "bc[i%+d]=", 0);
      std::memcpy(_brown_cluster_feature + prefix, _brown_cluster_path, _brown_cluster_path_lengths[npaths - 1]);
      for (size_t p = 0; p != npaths; ++p) {
        _brown_cluster_feature[prefix + _brown_cluster_path_lengths[npaths - 1 - p]] = '\0';
        features(_brown_cluster_feature);
      }
    }
  }

  // Add Clark cluster for w_{i}.
  if (_feature_flags.use_clark_cluster_features) {
    unsigned int cluster;
    if (_clark_clusters.get_cluster(utf8, &cluster, nullptr)) {
      ss << "cc[i]=" << cluster;
      features(ss.str());
      ss.str("");
    }
  }

  // Add word embedding values for w_{i}
  if (_feature_flags.use_word_embeddings_features) {
    const double *embeddings = _word_embeddings.get_embeddings(utf8);
    if (SCHWA_LIKELY(embeddings != nullptr)) {
      char buf[14];  // we[i+2]=ddddd
      const int prefix = std::sprintf(buf, "we[i%+d]=", 0);
      for (uint32_t j = 0; j != _word_embeddings.ndimensions(); ++j) {
        std::sprintf(buf + prefix, "%u", j);
        features(buf, embeddings[j]);
      }
    }
  }

  // Extended prediction history (Ratinov & Roth, CoNLL 2009).
  if (_feature_flags.use_extended_prediction_history_features) {
    const auto &it = _token_label_counts.find(utf8);
    if (it != _token_label_counts.end()) {
      third_party::crfsuite::floatval_t total = 0;
      for (const auto &pair : it->second)
        total += pair.second;
      for (const auto &pair : it->second) {
        ss << "eph=" << pair.first;
        features(ss.str(), pair.second/total);
        ss.str("");
      }
    }
  }

  // Gazetteer match.
  if (_feature_flags.use_gazetteer_features) {
    static constexpr const char GAZ_PREFIXES[4] = {'W', 'B', 'E', 'M'};
    char buf[11];  // gaz[ddd]=p
    for (unsigned int gaz = 0; gaz != _gazetteer.ngazetteers(); ++gaz) {
      uint8_t m = _gazetteer_matches[gaz][i];
      if (m == 0)
        continue;
      const int prefix = std::sprintf(buf, "gaz[%u]=", gaz);
      buf[prefix + 1] = '\0';
      for (uint8_t p = 0; p != 4; ++p) {
        if ((m & 0x01) != 0) {
          buf[prefix] = GAZ_PREFIXES[p];
          features(buf);
        }
        m >>= 1;
      }
    }
  }

  // Context aggregation of the surrounding tokens.
  if (_feature_flags.use_context_aggregation_features) {
    const auto &it = _token_context_aggregations.find(lower);
    if (it != _token_context_aggregations.end()) {
      for (const auto &pair : it->second) {
        if (pair.first == &token)
          continue;
        for (ptrdiff_t i = 0; i != 4; ++i) {
          ss << "ctxagg_w[i";
          if (i < 2)
            ss << (-2 + i);
          else
            ss << "+" << (i - 1);
          ss << "]=" << (pair.second[i] == nullptr ? learn::SENTINEL : _get_token_ne_normalised(*pair.second[i]));
          features.add_unique(ss.str());
          ss.str("");
        }
      }
    }
  }

  // Are we on the 2nd stage CRF?
  if (_is_second_stage) {
    // Token majority from 1st stage CRF.
    if (_feature_flags.use_token_majority_features) {
      const auto &it = _token_maj_counts.find(u);
      unsigned int max_count = 0;
      std::string max_label;
      for (const auto &pair : it->second) {
        if (pair.second > max_count || (pair.second == max_count && pair.first == token.ne_label_crf1)) {
          max_label = pair.first;
          max_count = pair.second;
        }
      }
      ss << "token_maj=" << max_label;
      features(ss.str());
      ss.str("");
    }

    // Entity majority from 1st stage CRF.
    if (_feature_flags.use_entity_majority_features) {
      const std::string entity_maj = _entity_maj[&token - &_phase3_doc->tokens.front()];
      if (!entity_maj.empty())
        features("entity_maj=" + entity_maj);
    }

    // Superentity majority from 1st stage CRF.
    if (_feature_flags.use_superentity_majority_features) {
      const std::string superentity_maj = _superentity_maj[&token - &_phase3_doc->tokens.front()];
      if (!superentity_maj.empty())
        features("superentity_maj=" + superentity_maj);
    }

    // Use the NEs from the 1st stage CRF in a +/-2 window.
    if (_feature_flags.use_surrounding_label_features) {
      for (int offset = -2; offset != 3; ++offset) {
        ss << "crf1_ne[i";
        if (i < 2)
          ss << (-2 + i);
        else
          ss << "+" << (i - 1);
        ss << "]=" << _offsets_token_ne_label_crf1(i, offset);
        features(ss.str());
        ss.str("");
      }
    }

    if (_feature_flags.use_crf1_label_feature) {
      ss << "crf1_ne=" << token.ne_label_crf1;
      features(ss.str());
      ss.str("");
    }
  }
}

}  // namespace ner
}  // namespace tagger
}  // namespace schwa

#endif  // SCHWA_TAGGER_NER_IMPL_H_
