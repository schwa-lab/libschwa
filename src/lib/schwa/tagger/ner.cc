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

static const std::vector<std::string> GAZETTEER_PATHS = {{
  "lex-data/gazetteers/countries",
  "lex-data/gazetteers/illinois/ArtWork",
  "lex-data/gazetteers/illinois/Building",
  "lex-data/gazetteers/illinois/Building.SubArea",
  "lex-data/gazetteers/illinois/Clothes",
  "lex-data/gazetteers/illinois/CollectiveNouns",
  "lex-data/gazetteers/illinois/Colors",
  "lex-data/gazetteers/illinois/CompetitionsBattlesEvents",
  "lex-data/gazetteers/illinois/Conduits",
  "lex-data/gazetteers/illinois/Corporations",
  "lex-data/gazetteers/illinois/CriminalActs",
  "lex-data/gazetteers/illinois/Currency",
  "lex-data/gazetteers/illinois/Diseases",
  "lex-data/gazetteers/illinois/EthnicGroups",
  "lex-data/gazetteers/illinois/Films",
  "lex-data/gazetteers/illinois/Government.Australia",
  "lex-data/gazetteers/illinois/Government.Canada",
  "lex-data/gazetteers/illinois/Government.Germany",
  "lex-data/gazetteers/illinois/Government",
  "lex-data/gazetteers/illinois/Government.HongKong",
  "lex-data/gazetteers/illinois/Government.India",
  "lex-data/gazetteers/illinois/Government.NewZealand",
  "lex-data/gazetteers/illinois/Government.Norway",
  "lex-data/gazetteers/illinois/Government.Sweden",
  "lex-data/gazetteers/illinois/Government.Switzerland",
  "lex-data/gazetteers/illinois/Government.UK",
  "lex-data/gazetteers/illinois/Government.US",
  "lex-data/gazetteers/illinois/Languages",
  "lex-data/gazetteers/illinois/LiteraryGenre",
  "lex-data/gazetteers/illinois/Locations.Cities.Canada",
  "lex-data/gazetteers/illinois/Locations.Cities.Europe",
  "lex-data/gazetteers/illinois/Locations.Cities",
  "lex-data/gazetteers/illinois/Locations.Cities.India",
  "lex-data/gazetteers/illinois/Locations.Cities.Japan",
  "lex-data/gazetteers/illinois/Locations.Cities.US",
  "lex-data/gazetteers/illinois/Locations.Counties.US",
  "lex-data/gazetteers/illinois/Locations.Countries",
  "lex-data/gazetteers/illinois/Locations.Generic",
  "lex-data/gazetteers/illinois/Locations",
  "lex-data/gazetteers/illinois/Locations.Regions",
  "lex-data/gazetteers/illinois/Locations.States",
  "lex-data/gazetteers/illinois/ManMadeObjects",
  "lex-data/gazetteers/illinois/ManufacturingPlants",
  "lex-data/gazetteers/illinois/MarketIndices",
  "lex-data/gazetteers/illinois/Measurements",
  "lex-data/gazetteers/illinois/Nationalities",
  "lex-data/gazetteers/illinois/NumberCardinal",
  "lex-data/gazetteers/illinois/NumberOrdinal",
  "lex-data/gazetteers/illinois/Organizations.EducationalInstitutions",
  "lex-data/gazetteers/illinois/Organizations",
  "lex-data/gazetteers/illinois/Organizations.OnStockExchange",
  "lex-data/gazetteers/illinois/Organizations.Terrorist",
  "lex-data/gazetteers/illinois/Parks",
  "lex-data/gazetteers/illinois/Paths",
  "lex-data/gazetteers/illinois/People.Famous",
  "lex-data/gazetteers/illinois/People.FirstNames",
  "lex-data/gazetteers/illinois/People.Gender.Female",
  "lex-data/gazetteers/illinois/People.Gender.Male",
  "lex-data/gazetteers/illinois/People",
  "lex-data/gazetteers/illinois/People.Judges",
  "lex-data/gazetteers/illinois/People.Monarchs",
  "lex-data/gazetteers/illinois/People.Politicians",
  "lex-data/gazetteers/illinois/People.Politicians.US",
  "lex-data/gazetteers/illinois/People.Politicians.US.Presidents",
  "lex-data/gazetteers/illinois/People.Politicians.US.VicePresidents",
  "lex-data/gazetteers/illinois/Person",
  "lex-data/gazetteers/illinois/Person.Hyponyms",
  "lex-data/gazetteers/illinois/Person.JobsTitles",
  "lex-data/gazetteers/illinois/PhysicalBarriers",
  "lex-data/gazetteers/illinois/PoliticalParties",
  "lex-data/gazetteers/illinois/Temporal",
  "lex-data/gazetteers/illinois/TV.Programs",
  "lex-data/gazetteers/illinois/Vehicles",
  "lex-data/gazetteers/illinois/Weapons.Aircraft",
  "lex-data/gazetteers/illinois/Weapons.Artillery",
  "lex-data/gazetteers/illinois/Weapons.Firearms",
  "lex-data/gazetteers/illinois/Weapons",
  "lex-data/gazetteers/illinois/Weapons.Melee",
  "lex-data/gazetteers/illinois/Weapons.Missile",
  "lex-data/gazetteers/illinois/Weapons.Torpedoes",
}};


// ============================================================================
// ModelParams
// ============================================================================
ModelParams::ModelParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags) :
    learn::ModelParams(group, name, desc, flags),
    tag_encoding(*this, "tag-encoding", 'E', "Sequence tag encoding scheme to use", "bmewo"),
    feature_hashing(*this, "feature-hashing", 'H', "Number of bits to use for feature hashing", config::Flags::OPTIONAL),
    brown_clusters_path(*this, "brown-cluster-path", "Absolute path to the Brown clusters file", "lex-data/brown-clusters/english-wikitext.c1000"),
    gazetteer_path(*this, "gazetteer-path", "Absolute path to the country-related n-gram gazetter file", "lex-data/gazetteers/countries"),
    word_embeddings_path(*this, "word-embeddings-path", "Absolute path to the word embeddings file", "lex-data/word-embeddings/cw.50dim.unscalled.double"),
    word_embeddings_sigma(*this, "word-embeddings-sigma", "Scaling factor for scaling the embeddings values", lex::WordEmbeddings::DEFAULT_SIGMA),
    use_brown_cluster_features(*this, "use-brown-cluster-features", "Whether or not to use the Brown cluster features", true),
    use_context_aggregation_features(*this, "use-context-aggregation-features", "Whether or not to use the context aggregation features", false),
    use_extended_prediction_history_features(*this, "use-extended-prediction-history-features", "Whether or not to use the extended prediction-history features", true),
    use_gazetteer_features(*this, "use-gazetteer-features", "Whether or not to use the gazetteer features", true),
    use_word_embeddings_features(*this, "use-word-embeddings-features", "Whether or not to use the word embeddings features", true),
    use_token_majority_features(*this, "use-token-majority-features", "Whether or not to use the 2nd stage CRF token majority features", true),
    use_entity_majority_features(*this, "use-entity-majority-features", "Whether or not to use the 2nd stage CRF entity majority features", true),
    use_superentity_majority_features(*this, "use-superentity-majority-features", "Whether or not to use the 2nd stage CRF superentity majority features", true),
    use_surrounding_label_features(*this, "use-surrounding-label-features", "Whether or not to use the 2nd stage CRF surrounding label features", true),
    use_crf1_label_feature(*this, "use-crf1-label-feature", "Whether or not to use the 2nd stage CRF CRF1 label feature", true)
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
    use_word_embeddings_features(false),
    use_token_majority_features(false),
    use_entity_majority_features(false),
    use_superentity_majority_features(false),
    use_surrounding_label_features(false),
    use_crf1_label_feature(false)
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
  if (params.use_brown_cluster_features()) {
    io::InputStream in(params.brown_clusters_path());
    _brown_clusters.load(in);
  }

  // Load the gazetteer.
  if (params.use_gazetteer_features()) {
    //io::InputStream in(params.gazetteer_path());
    //_gazetteer.load(in);
    for (const std::string &path : GAZETTEER_PATHS) {
      io::InputStream in(path);
      _gazetteer.load(in);
    }
  }

  // Load the word embeddings.
  if (params.use_word_embeddings_features()) {
    io::InputStream in(params.word_embeddings_path());
    _word_embeddings.load(in);
  }

  // Set the feature flags.
  _feature_flags.use_brown_cluster_features = params.use_brown_cluster_features();
  _feature_flags.use_context_aggregation_features = params.use_context_aggregation_features();
  _feature_flags.use_extended_prediction_history_features = params.use_extended_prediction_history_features();
  _feature_flags.use_gazetteer_features = params.use_gazetteer_features();
  _feature_flags.use_word_embeddings_features = params.use_word_embeddings_features();
  _feature_flags.use_token_majority_features = params.use_token_majority_features();
  _feature_flags.use_entity_majority_features = params.use_entity_majority_features();
  _feature_flags.use_superentity_majority_features = params.use_superentity_majority_features();
  _feature_flags.use_surrounding_label_features = params.use_surrounding_label_features();
  _feature_flags.use_crf1_label_feature = params.use_crf1_label_feature();
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
  if (params.use_brown_cluster_features()) {
    io::InputStream in(params.brown_clusters_path());
    _brown_clusters.load(in);
  }

  // Load the gazetteer.
  if (params.use_gazetteer_features()) {
    //io::InputStream in(params.gazetteer_path());
    //_gazetteer.load(in);
    for (const std::string &path : GAZETTEER_PATHS) {
      io::InputStream in(path);
      _gazetteer.load(in);
    }
  }

  // Load the word embeddings.
  if (params.use_word_embeddings_features()) {
    io::InputStream in(params.word_embeddings_path());
    _word_embeddings.load(in);
  }

  // Set the feature flags.
  _feature_flags.use_brown_cluster_features = params.use_brown_cluster_features();
  _feature_flags.use_context_aggregation_features = params.use_context_aggregation_features();
  _feature_flags.use_extended_prediction_history_features = params.use_extended_prediction_history_features();
  _feature_flags.use_gazetteer_features = params.use_gazetteer_features();
  _feature_flags.use_word_embeddings_features = params.use_word_embeddings_features();
  _feature_flags.use_token_majority_features = params.use_token_majority_features();
  _feature_flags.use_entity_majority_features = params.use_entity_majority_features();
  _feature_flags.use_superentity_majority_features = params.use_superentity_majority_features();
  _feature_flags.use_surrounding_label_features = params.use_surrounding_label_features();
  _feature_flags.use_crf1_label_feature = params.use_crf1_label_feature();
}

OutputModel::~OutputModel(void) { }


// ============================================================================
// Extractor
// ============================================================================
const RE2 Extractor::RE_ACRONYM("[A-Z]{2,}\\.?|([A-Z]\\.){2,}|[A-Z]+&[A-Z]+\\.?|([A-Z]\\.)+&([A-Z]+\\.)+");
const RE2 Extractor::RE_ORDINAL("\\pN+(?:st|ST|nd|ND|rd|RD|th|TH)");  // Identifying an ordinal expression.
const RE2 Extractor::RE_ROMAN_NUMERAL("M{0,4}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})");
const RE2 Extractor::RE_PERSON_INITIAL_1("[A-Z]\\.");
const RE2 Extractor::RE_PERSON_INITIAL_2("(([A-Z]\\.)?(al-|d'|Mc|O'))?[A-Z][a-z]+");

const RE2 Extractor::RE_TEMPORAL("Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday|January|Febuary|March|April|May|June|July|August|September|October|November|December");
const RE2 Extractor::RE_WHO_PRONOUN("who(m|se)?");


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
    _gazetteer_matches(_gazetteer.ngazetteers()),
    _word_embeddings(model.word_embeddings()),
    _logger(*io::default_logger),
    _offsets_token_ne_normalised(&_get_token_ne_normalised),
    _offsets_token_norm_raw(&_get_token_norm_raw),
    _offsets_token_ne_label_crf1(&_get_token_ne_label_crf1)
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
    _gazetteer_matches(_gazetteer.ngazetteers()),
    _word_embeddings(model.word_embeddings()),
    _logger(model.logger()),
    _offsets_token_ne_normalised(&_get_token_ne_normalised),
    _offsets_token_norm_raw(&_get_token_norm_raw),
    _offsets_token_ne_label_crf1(&_get_token_ne_label_crf1)
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
Extractor::prepare_doc(cs::Doc &doc, const bool is_train, const bool is_second_stage, const SequenceTagEncoding tag_encoding) {
  static const auto REVERSE_GOLD_NES = DR_REVERSE_SLICES(&cs::Doc::named_entities, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Token::ne);
  static const auto SEQUENCE_TAG_GOLD_NES = DR_SEQUENCE_TAGGER(&cs::Doc::named_entities, &cs::Doc::sentences, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Sentence::span, &cs::Token::ne, &cs::NamedEntity::label, &cs::Token::ne_label);
  static const auto SEQUENCE_TAG_CRF1_NES = DR_SEQUENCE_TAGGER(&cs::Doc::named_entities_crf1, &cs::Doc::sentences, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Sentence::span, &cs::Token::ne, &cs::NamedEntity::label, &cs::Token::ne_label_crf1);
  static const auto SEQUENCE_UNTAG_CRF1_NES = DR_SEQUENCE_UNTAGGER(&cs::Doc::named_entities_crf1, &cs::Doc::sentences, &cs::NamedEntity::span, &cs::Sentence::span, &cs::NamedEntity::label, &cs::Token::ne_label);

  // Move the 1st stage CRF tagging decisions from the ne_label attribute to the ne_label_crf1 attribute.
  if (is_second_stage) {
    SEQUENCE_UNTAG_CRF1_NES(doc);
    SEQUENCE_TAG_CRF1_NES(doc, tag_encoding);
  }

  // Reverse the gold NEs down onto the tokens, as well as the encoded NE label.
  if (is_train) {
    REVERSE_GOLD_NES(doc);
    SEQUENCE_TAG_GOLD_NES(doc, tag_encoding);
  }
}


void
Extractor::phase2_bod(cs::Doc &doc) {
  // Prepare the doc if we're not running in a multithreaded context.
  if (!_is_threaded)
    prepare_doc(doc, _is_train, _is_second_stage, _tag_encoding);

  // Only 2nd stage CRF things from here on.
  if (!_is_second_stage)
    return;

  // Entity majority and super-entity majority counts.
  std::unordered_map<UnicodeString, std::unordered_map<std::string, unsigned int>> entity_counts, superentity_counts;
  std::stringstream ss;
  for (const cs::NamedEntity &ne : doc.named_entities_crf1) {
    // Construct all possible n-gram strings.
    const size_t ntokens = ne.span.stop - ne.span.start;
    for (size_t start = 0; start != ntokens; ++start) {
      for (size_t stop = start + 1; stop != ntokens + 1; ++stop) {
        // Construct the n-gram string.
        for (size_t i = start; i != stop; ++i) {
          if (i != start)
            ss << ' ';
          ss << ne.span.start[i].ne_normalised;
        }
        const UnicodeString lower = UnicodeString::from_utf8(ss.str()).to_lower();
        ss.str("");

        // Increment the count of the sub-n-gram and possibly the full n-gram.
        superentity_counts[lower][ne.label] += 1;
        if (stop - start == ntokens)
          entity_counts[lower][ne.label] += 1;
      }
    }
  }

  // Place the most frequent entity label on each of the tokens covered by the entity.
  _entity_maj.resize(doc.tokens.size());
  _superentity_maj.resize(doc.tokens.size());
  for (const cs::NamedEntity &ne : doc.named_entities_crf1) {
    // Construct the entity n-gram string.
    for (const cs::Token &token : ne.span) {
      if (&token != ne.span.start)
        ss << ' ';
      ss << token.ne_normalised;
    }
    const UnicodeString lower = UnicodeString::from_utf8(ss.str()).to_lower();
    ss.str("");

    // Find the most frequent label for this case-normalised entity n-gram.
    unsigned int max_count = 0;
    std::string max_label;
    for (const auto &pair : entity_counts[lower]) {
      if (pair.second > max_count || (pair.second == max_count && pair.first == ne.label)) {
        max_label = pair.first;
        max_count = pair.second;
      }
    }

    // Assign the most frequent label to the tokens that this named entity spans.
    for (const cs::Token &token : ne.span)
      _entity_maj[&token - &doc.tokens.front()] = max_label;

    // Find the most frequent label for this case-normalised superentity n-gram.
    max_count = 0;
    for (const auto &pair : entity_counts[lower]) {
      if (pair.second > max_count || (pair.second == max_count && pair.first == ne.label)) {
        max_label = pair.first;
        max_count = pair.second;
      }
    }

    // Assign the most frequent label to the tokens that this named entity spans.
    for (const cs::Token &token : ne.span)
      _superentity_maj[&token - &doc.tokens.front()] = max_label;
  }

  // Place the most frequent entity label on each of the tokens not covered by an entity.
  for (size_t i = 0; i != _entity_maj.size(); ++i) {
    if (!_entity_maj[i].empty())
      continue;

    // Find the most frequent label for this token as a 1-gram named entity.
    const UnicodeString lower = UnicodeString::from_utf8(doc.tokens[i].ne_normalised).to_lower();
    const auto &it = entity_counts.find(lower);
    if (it == entity_counts.end())
      continue;

    unsigned int max_count = 0;
    std::string max_label;
    for (const auto &pair : entity_counts[lower]) {
      if (pair.second > max_count) {
        max_label = pair.first;
        max_count = pair.second;
      }
    }
    _entity_maj[i] = max_label;
  }

  // Place the most frequent superentity label on each of the tokens not covered by an entity.
  for (size_t i = 0; i != _superentity_maj.size(); ++i) {
    if (!_superentity_maj[i].empty())
      continue;

    // Find the most frequent label for this token as a 1-gram named entity.
    const UnicodeString lower = UnicodeString::from_utf8(doc.tokens[i].ne_normalised).to_lower();
    const auto &it = entity_counts.find(lower);
    if (it == entity_counts.end())
      continue;

    unsigned int max_count = 0;
    std::string max_label;
    for (const auto &pair : entity_counts[lower]) {
      if (pair.second > max_count) {
        max_label = pair.first;
        max_count = pair.second;
      }
    }
    _superentity_maj[i] = max_label;
  }
}


void
Extractor::phase2_bos(cs::Sentence &sentence) {
  // Update the token offsets objects to know about the new sentence.
  _offsets_token_ne_normalised.set_slice(sentence.span);
  _offsets_token_norm_raw.set_slice(sentence.span);
  _offsets_token_ne_label_crf1.set_slice(sentence.span);
}


void
Extractor::phase2_extract(cs::Sentence &sentence, cs::Token &token) {
  // Find the identical token forms for use in the context aggregation
  const UnicodeString lower = UnicodeString::from_utf8(_get_token_ne_normalised(token)).to_lower();
  auto &array = _token_context_aggregations[lower][&token];

  const ptrdiff_t sentence_length = sentence.span.stop - sentence.span.start;
  const ptrdiff_t i = &token - sentence.span.start;
  array[0] = (i >= 2) ? &token - 2 : nullptr;
  array[1] = (i >= 1) ? &token - 1 : nullptr;
  array[2] = (i <= sentence_length - 2) ? &token + 1 : nullptr;
  array[3] = (i <= sentence_length - 3) ? &token + 2 : nullptr;

  // Token majority features from the 1st stage CRF.
  if (_is_second_stage) {
    _token_maj_counts[lower][token.ne_label_crf1] += 1;
  }
}


void
Extractor::phase3_bos(cs::Sentence &sentence) {
  // Update the token offsets objects to know about the new sentence.
  _offsets_token_ne_normalised.set_slice(sentence.span);
  _offsets_token_norm_raw.set_slice(sentence.span);
  _offsets_token_ne_label_crf1.set_slice(sentence.span);

  // Everything from here is for the gazetteer features.
  if (!_feature_flags.use_gazetteer_features)
    return;

  // How many tokens long is the sentence?
  const size_t sentence_length = sentence.span.stop - sentence.span.start;

  // Reset the gazetteer match flags.
  for (unsigned int gaz = 0; gaz != _gazetteer.ngazetteers(); ++gaz) {
    _gazetteer_matches[gaz].resize(sentence_length);
    std::fill(_gazetteer_matches[gaz].begin(), _gazetteer_matches[gaz].end(), 0);
  }

  // Lookup n-gram gazetteer matches for all n-grams in the sentence.
  for (cs::Token &token0 : sentence.span) {
    // For each gazetteer.
    for (unsigned int gaz = 0; gaz != _gazetteer.ngazetteers(); ++gaz) {
      // Are there any potential gazetteer n-gram matches for the current token?
      const std::vector<std::vector<const uint8_t *>> *ngrams = _gazetteer.get_ngrams(gaz, _get_token_ne_normalised(token0));
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
          _gazetteer_matches[gaz][t + i] |= m;
        }
      }
    }
  }
}


void
Extractor::phase3_update_history(canonical_schema::Sentence &, canonical_schema::Token &token, const std::string &label_string) {
  // Increment the <token, label> pair frequency counter.
  _token_label_counts[token.ne_normalised][label_string] += 1;
}


void
Extractor::phase3_eod(cs::Doc &) {
  // Reset the per-document aggregations.
  _token_label_counts.clear();
  _token_context_aggregations.clear();

  // Reset the 2nd stage CRF per-document aggregations.
  if (_is_second_stage) {
    _token_maj_counts.clear();
    _entity_maj.clear();
    _superentity_maj.clear();
  }
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
