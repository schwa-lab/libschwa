/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tagger/ner.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <schwa/dr.h>
#include <schwa/io/logging.h>
#include <schwa/io/paths.h>
#include <schwa/third-party/re2/re2.h>
#include <schwa/unicode.h>

using ::schwa::third_party::re2::RE2;
namespace cs = ::schwa::canonical_schema;


namespace schwa {
namespace tagger {
namespace ner {

// ============================================================================
// ModelParams
// ============================================================================
ModelParams::ModelParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags) :
    learn::ModelParams(group, name, desc, flags),
    tag_encoding(*this, "tag-encoding", 'E', "Sequence tag encoding scheme to use", "bmewo"),
    feature_hashing(*this, "feature-hashing", 'H', "Number of bits to use for feature hashing", config::Flags::OPTIONAL),
    brown_clusters_path(*this, "brown-cluster-path", "Absolute path to the Brown clusters file", "lex-data/brown-clusters/rcv1.c1000"),
    gazetteer_path(*this, "gazetteer-path", "Absolute path to the country-related n-gram gazetter file", "lex-data/gazetteers/countries"),
    word_embeddings_path(*this, "word-embeddings-path", "Absolute path to the word embeddings file", "lex-data/word-embeddings/cw.50dim.unscalled.double"),
    word_embeddings_sigma(*this, "word-embeddings-sigma", "Scaling factor for scaling the embeddings values", lex::WordEmbeddings::DEFAULT_SIGMA),
    use_brown_cluster_features(*this, "use-brown-cluster-features", "Whether or not to use the Brown cluster features", true),
    use_context_aggregation_features(*this, "use-context-aggregation-features", "Whether or not to use the context aggregation features", true),
    use_extended_prediction_history_features(*this, "use-extended-prediction-history-features", "Whether or not to use the extended prediction-history features", true),
    use_gazetteer_features(*this, "use-gazetteer-features", "Whether or not to use the gazetteer features", true),
    use_word_embeddings_features(*this, "use-word-embeddings-features", "Whether or not to use the word embeddings features", true)
  { }

ModelParams::~ModelParams(void) { }


// ============================================================================
// FeatureFlags
// ============================================================================
FeatureFlags::FeatureFlags(void) :
    use_brown_cluster_features(false),
    use_context_aggregation_features(false),
    use_extended_prediction_history_features(false),
    use_gazetteer_features(false),
    use_word_embeddings_features(false)
  { }


// ============================================================================
// InputModel
// ============================================================================
InputModel::InputModel(const std::string &path, ModelParams &params) :
    learn::InputModel(path, params),
    _pool(4 * 1024 * 1024),
    _string_pool(_pool),
    _brown_clusters(_string_pool),
    _gazetteer(_string_pool),
    _word_embeddings(_string_pool, params.word_embeddings_sigma()),
    _tag_encoding(params.tag_encoding.encoding())
  {
  // Load the Brown clusters.
  {
    io::InputStream in(params.brown_clusters_path());
    _brown_clusters.load(in);
  }

  // Load the gazetteer.
  {
    io::InputStream in(params.gazetteer_path());
    _gazetteer.load(in);
  }

  // Load the word embeddings.
  {
    io::InputStream in(params.word_embeddings_path());
    _word_embeddings.load(in);
  }

  // Set the feature flags.
  _feature_flags.use_brown_cluster_features = params.use_brown_cluster_features();
  _feature_flags.use_context_aggregation_features = params.use_context_aggregation_features();
  _feature_flags.use_extended_prediction_history_features = params.use_extended_prediction_history_features();
  _feature_flags.use_gazetteer_features = params.use_gazetteer_features();
  _feature_flags.use_word_embeddings_features = params.use_word_embeddings_features();
}

InputModel::~InputModel(void) { }


// ============================================================================
// OutputModel
// ============================================================================
OutputModel::OutputModel(const std::string &path, const ModelParams &params, const config::Main &main_config) :
    learn::OutputModel(path, params, main_config, true),
    _pool(4 * 1024 * 1024),
    _string_pool(_pool),
    _brown_clusters(_string_pool),
    _gazetteer(_string_pool),
    _word_embeddings(_string_pool, params.word_embeddings_sigma()),
    _tag_encoding(params.tag_encoding.encoding())
  {
  // Load the Brown clusters.
  {
    io::InputStream in(params.brown_clusters_path());
    _brown_clusters.load(in);
  }

  // Load the gazetteer.
  {
    io::InputStream in(params.gazetteer_path());
    _gazetteer.load(in);
  }

  // Load the word embeddings.
  {
    io::InputStream in(params.word_embeddings_path());
    _word_embeddings.load(in);
  }

  // Set the feature flags.
  _feature_flags.use_brown_cluster_features = params.use_brown_cluster_features();
  _feature_flags.use_context_aggregation_features = params.use_context_aggregation_features();
  _feature_flags.use_extended_prediction_history_features = params.use_extended_prediction_history_features();
  _feature_flags.use_gazetteer_features = params.use_gazetteer_features();
  _feature_flags.use_word_embeddings_features = params.use_word_embeddings_features();
}

OutputModel::~OutputModel(void) { }


// ============================================================================
// Extractor
// ============================================================================
const RE2 Extractor::RE_ACRONYM("[A-Z]{2,}\\.?|([A-Z]\\.){2,}|[A-Z]+&[A-Z]+\\.?|([A-Z]\\.)+&([A-Z]+\\.)+");
const RE2 Extractor::RE_ORDINAL("\\pN+(?:st|ST|nd|ND|rd|RD|th|TH)");  // Identifying an ordinal expression.
const RE2 Extractor::RE_ROMAN_NUMERAL("M{0,4}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})");
const RE2 Extractor::RE_PERSON_INITIAL_1("[A-Z]\\.");    // Match "Cricket: P. Smith and A. Fitz blah blah"
const RE2 Extractor::RE_PERSON_INITIAL_2("[A-Z][a-z]+");


Extractor::Extractor(InputModel &model, bool is_second_stage, bool is_threaded) :
    _is_train(false),
    _is_second_stage(is_second_stage),
    _is_threaded(is_threaded),
    _tag_encoding(model.tag_encoding()),
    _feature_flags(model.feature_flags()),
    _brown_clusters(model.brown_clusters()),
    _brown_cluster_path(nullptr),
    _brown_cluster_path_lengths(new unsigned int[_brown_clusters.npaths()]),
    _brown_cluster_feature(new char[8 + _brown_clusters.path_lengths().back() + 1]),
    _gazetteer(model.gazetteer()),
    _word_embeddings(model.word_embeddings()),
    _logger(*io::default_logger),
    _offsets_token_ne_normalised(&_get_token_ne_normalised),
    _offsets_token_norm_raw(&_get_token_norm_raw)
  {
  _check_regular_expressions();
}

Extractor::Extractor(OutputModel &model, bool is_second_stage, bool is_threaded) :
    _is_train(true),
    _is_second_stage(is_second_stage),
    _is_threaded(is_threaded),
    _tag_encoding(model.tag_encoding()),
    _feature_flags(model.feature_flags()),
    _brown_clusters(model.brown_clusters()),
    _brown_cluster_path(nullptr),
    _brown_cluster_path_lengths(new unsigned int[_brown_clusters.npaths()]),
    _brown_cluster_feature(new char[8 + _brown_clusters.path_lengths().back() + 1]),
    _gazetteer(model.gazetteer()),
    _word_embeddings(model.word_embeddings()),
    _logger(model.logger()),
    _offsets_token_ne_normalised(&_get_token_ne_normalised),
    _offsets_token_norm_raw(&_get_token_norm_raw)
  {
  _check_regular_expressions();
}

Extractor::~Extractor(void) {
  delete [] _brown_cluster_feature;
  delete [] _brown_cluster_path_lengths;
}


void
Extractor::_check_regular_expressions(void) const {
  assert(RE_ACRONYM.ok());
  assert(RE_ORDINAL.ok());
  assert(RE_ROMAN_NUMERAL.ok());
  assert(RE_PERSON_INITIAL_1.ok());
  assert(RE_PERSON_INITIAL_2.ok());
}


void
Extractor::prepare_doc(cs::Doc &doc, const bool is_second_stage, const bool is_train, const SequenceTagEncoding tag_encoding) {
  static const auto REVERSE_GOLD_NES = DR_REVERSE_SLICES(&cs::Doc::named_entities, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Token::ne);
  static const auto SEQUENCE_TAG_GOLD_NES = DR_SEQUENCE_TAGGER(&cs::Doc::named_entities, &cs::Doc::sentences, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Sentence::span, &cs::Token::ne, &cs::NamedEntity::label, &cs::Token::ne_label);

  // Move the 1st stage CRF tagging decisions from the ne_label attribute to the ne_label_crf1 attribute.
  if (is_second_stage) {
    for (cs::Token &token : doc.tokens)
      token.ne_label_crf1 = token.ne_label;
  }

  // Reverse the gold NEs down onto the tokens, as well as the encoded NE label.
  if (is_train) {
    REVERSE_GOLD_NES(doc);
    SEQUENCE_TAG_GOLD_NES(doc, tag_encoding);
  }
}


void
Extractor::phase2_bod(cs::Doc &doc) {
  if (!_is_threaded)
    prepare_doc(doc, _is_second_stage, _is_train, _tag_encoding);
}


void
Extractor::phase2_bos(cs::Sentence &sentence) {
  // Update the token offsets objects to know about the new sentence.
  _offsets_token_ne_normalised.set_slice(sentence.span);
  _offsets_token_norm_raw.set_slice(sentence.span);

  // How many tokens long is the sentence?
  const size_t sentence_length = sentence.span.stop - sentence.span.start;
  _gazetteer_match.resize(sentence_length);

  // Reset the gazetteer match flags.
  std::fill(_gazetteer_match.begin(), _gazetteer_match.end(), 0);

  // Lookup n-gram gazetteer matches for all n-grams in the sentence.
  for (cs::Token &token0 : sentence.span) {
    // Are there any potential gazetteer n-gram matches for the current token?
    const std::vector<std::vector<const uint8_t *>> *ngrams = _gazetteer.get_ngrams(_get_token_ne_normalised(token0));
    if (ngrams == nullptr)
      continue;

    // Consider each n-gram.
    const size_t t = &token0 - sentence.span.start;
    for (const std::vector<const uint8_t *> &ngram : *ngrams) {
      // Can the n-gram fit?
      if (t + ngram.size() > sentence_length)
        continue;

      // Check each of the tokens in turn.
      cs::Token *it_t = &token0;
      auto it_n = ngram.begin();
      bool success = true;
      for (unsigned int i = 0; i != ngram.size(); ++i, ++it_t, ++it_n) {
        const std::string &norm_t = _get_token_ne_normalised(*it_t);
        const char *norm_n = reinterpret_cast<const char *>(*it_n);
        if (norm_t != norm_n) {
          success = false;
          break;
        }
      }
      if (!success)
        continue;

      // If we found a match in the gazetteer, indicate that we found at match for each of the tokens in the n-gram.
      for (unsigned int i = 0; i != ngram.size(); ++i) {
        uint8_t m;
        if (ngram.size() == 1)
          m = (1 << 0);
        else if (i == 0)
          m = (1 << 1);
        else if (i == ngram.size() - 1)
          m = (1 << 2);
        else
          m = (1 << 3);
        _gazetteer_match[t + i] |= m;
      }
    }
  }
}


void
Extractor::phase2_extract(cs::Sentence &sentence, cs::Token &token) {
  // FIXME Find the identical token forms for use in the context aggregation
  const UnicodeString lower = UnicodeString::from_utf8(_get_token_ne_normalised(token)).to_lower();
  auto &array = _token_context_aggregations[lower][&token];

  const ptrdiff_t sentence_length = sentence.span.stop - sentence.span.start;
  const ptrdiff_t i = &token - sentence.span.start;
  array[0] = (i >= 2) ? &token - 2 : nullptr;
  array[1] = (i >= 1) ? &token - 1 : nullptr;
  array[2] = (i <= sentence_length - 2) ? &token + 1 : nullptr;
  array[3] = (i <= sentence_length - 3) ? &token + 2 : nullptr;
}


void
Extractor::phase3_bos(cs::Sentence &sentence) {
  // Update the token offsets objects to know about the new sentence.
  _offsets_token_ne_normalised.set_slice(sentence.span);
  _offsets_token_norm_raw.set_slice(sentence.span);
}


void
Extractor::phase3_update_history(canonical_schema::Sentence &sentence, canonical_schema::Token &token, const std::string &label_string) {
  // Don't include the first token in a sentence as it's ambiguous to begin with.
  if (&token != sentence.span.start) {
    auto &list = _token_label_counts[token.ne_normalised];
    bool found = false;
    for (auto &pair : list) {
      if (std::get<0>(pair) == label_string) {
        std::get<1>(pair) += 1;
        found = true;
        break;
      }
    }
    if (!found)
      list.push_back(std::make_tuple(label_string, 1));
  }
}


void
Extractor::phase3_eod(cs::Doc &) {
  // Reset the per-document aggregations.
  _token_label_counts.clear();
  _token_context_aggregations.clear();
}



// ============================================================================
// preprocess_doc
// ============================================================================
static void
attempt_truecase_token(cs::Token &token, cs::Sentence &sentence, const lex::BrownClusters &brown_clusters, const std::unordered_map<UnicodeString, std::unordered_map<std::string, unsigned int>> &capitalisation_counts) {
  // Compute the possible truecased forms of the token: all lower, all upper, or initial upper.
  const std::string &orig = token.get_norm_raw();
  const UnicodeString lower = UnicodeString::from_utf8(orig).to_lower();
  const UnicodeString title = UnicodeString::from_utf8(orig).to_title();
  const UnicodeString upper = UnicodeString::from_utf8(orig).to_upper();
  std::array<std::string, 3> options = {{
      lower.to_utf8(),
      title.to_utf8(),
      upper.to_utf8()
  }};

  // If there's a title-case version of the word in the document, that's pretty indicative.
  {
    const auto &it = capitalisation_counts.find(lower);
    if (it != capitalisation_counts.end()) {
      const auto &it2 = it->second.find(options[1]);
      if (it2 != it->second.end()) {
        token.ne_normalised = options[1];
        return;
      }
    }
  }

  // Get the frequencies of the three capitalisation variants from the Brown clusters, and sort them in ascending order of frequency.
  std::array<std::tuple<unsigned int, std::string *>, 3> brown_counts = {{
      std::make_tuple(brown_clusters.get_frequency(options[0]), &options[0]),
      std::make_tuple(brown_clusters.get_frequency(options[1]), &options[1]),
      std::make_tuple(brown_clusters.get_frequency(options[2]), &options[2])
  }};
  std::sort(brown_counts.begin(), brown_counts.end());

  // Assign the most likely word based on the frequency counts, optionally indicating that the document-local counts should be used instead.
  bool fallback_to_local_counts = false;
  if (&token == sentence.span.start) {
    if (std::get<0>(brown_counts[2]) == 0)
      fallback_to_local_counts = true;
    else if (std::get<1>(brown_counts[2]) == &options[0])
      token.ne_normalised = *std::get<1>(brown_counts[1]);
    else
      token.ne_normalised = *std::get<1>(brown_counts[2]);
  }
  else {
    if (std::get<0>(brown_counts[2]) == 0)
      fallback_to_local_counts = true;
    else
      token.ne_normalised = *std::get<1>(brown_counts[2]);
  }

  // Do we need to fall back to the document-local counts as the word is unseen in Brown?
  if (fallback_to_local_counts) {
    const auto &it = capitalisation_counts.find(lower);
    if (it == capitalisation_counts.end()) {
      if (&token == sentence.span.start)
        token.ne_normalised = options[1];
      else
        token.ne_normalised = options[0];
    }
    else {
      unsigned int freq = 0;
      for (const auto &pair : it->second) {
        if (pair.second > freq) {
          token.ne_normalised = pair.first;
          freq = pair.second;
        }
      }
    }
  }
}


void
preprocess_doc(cs::Doc &doc, const lex::BrownClusters &brown_clusters) {
  // Perform an initial pass over the sentences to see which ones are all uppercase.
  std::vector<bool> sent_contains_lower(doc.sentences.size());
  for (size_t s = 0; s != doc.sentences.size(); ++s) {
    cs::Sentence &sentence = doc.sentences[s];
    bool contains_lower = false, contains_alpha = false;
    for (cs::Token &token : sentence.span) {
      for (const unicode_t cp : UTF8Decoder(token.get_norm_raw())) {
        if (unicode::is_lower(cp)) {
          contains_lower = true;
          contains_alpha = true;
          break;
        }
        else if (unicode::is_alpha(cp))
          contains_alpha = true;
      }
      if (contains_lower)
        break;
    }
    sent_contains_lower[s] = !(!contains_lower && contains_alpha);
  }

  // Construct capitalisation distribution counts for each token in the document that's not in an all uppercase sentence.
  std::unordered_map<UnicodeString, std::unordered_map<std::string, unsigned int>> capitalisation_counts;
  for (size_t s = 0; s != doc.sentences.size(); ++s) {
    if (!sent_contains_lower[s])
      continue;
    cs::Sentence &sentence = doc.sentences[s];
    for (cs::Token &token : sentence.span) {
      const std::string &orig = token.get_norm_raw();
      const UnicodeString u = UnicodeString::from_utf8(orig);
      // Don't trust the capitalisation of the first token in the sentence, unless it's all-caps.
      if (&token == sentence.span.start && !u.is_upper())
        continue;
      capitalisation_counts[u.to_lower()][orig] += 1;
    }
  }

  // Perform some lexical cleaning and normalisation.
  uint8_t utf8[4];
  for (size_t s = 0; s != doc.sentences.size(); ++s) {
    cs::Sentence &sentence = doc.sentences[s];

    if (sent_contains_lower[s]) {
      // No truecasing required for normal looking sentences.
      for (cs::Token &token : sentence.span)
        token.ne_normalised = token.get_norm_raw();
    }
    else {
      // Attempt to truecase the uppercase sentence.
      for (cs::Token &token : sentence.span)
        attempt_truecase_token(token, sentence, brown_clusters, capitalisation_counts);
    }  // end else

    // Replace all digits with "9" and all orginals with "9th".
    for (cs::Token &token : sentence.span) {
      std::string ne_normalised;
      ne_normalised.reserve(token.ne_normalised.size());

      // Is it an ordinal?
      if (RE2::FullMatch(token.ne_normalised, Extractor::RE_ORDINAL)) {
        // Replace the whole token value with "9th".
        ne_normalised = "9th";
      }
      else {
        // Replace all digits with "9".
        for (const unicode_t cp : UTF8Decoder(token.ne_normalised)) {
          if (unicode::is_digit(cp))
            ne_normalised.push_back('9');
          else {
            const size_t nbytes = write_utf8(cp, utf8);
            ne_normalised.append(reinterpret_cast<char *>(utf8), nbytes);
          }
        }
      }

      token.ne_normalised = ne_normalised;
    }  // for each token
  }  // for each sentence
}

}  // namesapce ner
}  // namespace tagger
}  // namespace schwa
