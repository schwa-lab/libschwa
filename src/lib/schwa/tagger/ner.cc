/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tagger/ner.h>

#include <algorithm>
#include <array>
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
    word_embeddings_path(*this, "word-embeddings-path", "Absolute path to the word embeddings file", "lex-data/word-embeddings/cw.50dim.unscalled")
  { }

ModelParams::~ModelParams(void) { }


// ============================================================================
// InputModel
// ============================================================================
InputModel::InputModel(const std::string &path, ModelParams &params) :
    learn::InputModel(path, params),
    _pool(4 * 1024 * 1024),
    _string_pool(_pool),
    _brown_clusters(_string_pool),
    _word_embeddings(_string_pool),
    _tag_encoding(params.tag_encoding.encoding())
  {
  // Load the Brown clusters.
  {
    io::InputStream in(params.brown_clusters_path());
    _brown_clusters.load(in);
  }

  // Load the word embeddings.
  {
    io::InputStream in(params.word_embeddings_path());
    _word_embeddings.load(in);
  }
}

InputModel::~InputModel(void) { }


// ============================================================================
// OutputModel
// ============================================================================
OutputModel::OutputModel(const std::string &path, const ModelParams &params, const config::Main &main_config) :
    learn::OutputModel(path, params, main_config),
    _pool(4 * 1024 * 1024),
    _string_pool(_pool),
    _brown_clusters(_string_pool),
    _word_embeddings(_string_pool),
    _tag_encoding(params.tag_encoding.encoding())
  {
  // Load the Brown clusters.
  {
    io::InputStream in(params.brown_clusters_path());
    _brown_clusters.load(in);
  }

  // Load the word embeddings.
  {
    io::InputStream in(params.word_embeddings_path());
    _word_embeddings.load(in);
  }
}

OutputModel::~OutputModel(void) { }


// ============================================================================
// Extractor
// ============================================================================
Extractor::Extractor(InputModel &model) :
    _is_train(false),
    _tag_encoding(model.tag_encoding()),
    _brown_clusters(model.brown_clusters()),
    _brown_cluster_path(nullptr),
    _brown_cluster_path_lengths(new unsigned int[_brown_clusters.npaths()]),
    _brown_cluster_feature(new char[8 + _brown_clusters.path_lengths().back() + 1]),
    _word_embeddings(model.word_embeddings()),
    _logger(*io::default_logger),
    _offsets_token_ne_normalised(&_get_token_ne_normalised),
    _offsets_token_norm_raw(&_get_token_norm_raw)
  { }


Extractor::Extractor(OutputModel &model) :
    _is_train(true),
    _tag_encoding(model.tag_encoding()),
    _brown_clusters(model.brown_clusters()),
    _brown_cluster_path(nullptr),
    _brown_cluster_path_lengths(new unsigned int[_brown_clusters.npaths()]),
    _brown_cluster_feature(new char[8 + _brown_clusters.path_lengths().back() + 1]),
    _word_embeddings(model.word_embeddings()),
    _logger(model.logger()),
    _offsets_token_ne_normalised(&_get_token_ne_normalised),
    _offsets_token_norm_raw(&_get_token_norm_raw)
  { }

Extractor::~Extractor(void) {
  delete [] _brown_cluster_feature;
  delete [] _brown_cluster_path_lengths;
}


void
Extractor::phase1_begin(void) {
  LOG2(INFO, _logger) << "Extractor phase1_begin" << std::endl;
}


void
Extractor::phase1_end(void) {
  LOG2(INFO, _logger) << "Extractor phase1_end" << std::endl;
}


void
Extractor::phase1_bod(cs::Doc &doc) {
  static const auto REVERSE_GOLD_NES = DR_REVERSE_SLICES(&cs::Doc::named_entities, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Token::ne);
  static const auto SEQUENCE_TAG_GOLD_NES = DR_SEQUENCE_TAGGER(&cs::Doc::named_entities, &cs::Doc::sentences, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Sentence::span, &cs::Token::ne, &cs::NamedEntity::label, &cs::Token::ne_label);

  // Reverse the gold NEs down onto the tokens, as well as the encoded NE label.
  if (_is_train) {
    REVERSE_GOLD_NES(doc);
    SEQUENCE_TAG_GOLD_NES(doc, _tag_encoding);
  }

  // Perform an initial pass over the sentences to see which ones are all uppercase.
  std::vector<bool> sent_contains_lower(doc.sentences.size());
  for (size_t s = 0; s != doc.sentences.size(); ++s) {
    cs::Sentence &sentence = doc.sentences[s];
    bool contains_lower = false;
    for (cs::Token &token : sentence.span) {
      for (const unicode_t cp : UTF8Decoder(_get_token_norm_raw(token))) {
        if (unicode::is_lower(cp)) {
          contains_lower = true;
          break;
        }
      }
      if (contains_lower)
        break;
    }
    sent_contains_lower[s] = contains_lower;
  }

  // Construct capitalisation distribution counts for each token in the document that's not in an all uppercase sentence.
  std::unordered_map<UnicodeString, std::unordered_map<std::string, unsigned int>> capitalisation_counts;
  for (size_t s = 0; s != doc.sentences.size(); ++s) {
    if (!sent_contains_lower[s])
      continue;
    cs::Sentence &sentence = doc.sentences[s];
    for (cs::Token &token : sentence.span) {
      const std::string &orig = _get_token_norm_raw(token);
      const UnicodeString lower = UnicodeString::from_utf8(orig).to_lower();
      capitalisation_counts[lower][orig] += 1;
    }
  }

  // Regex for identifying an ordinal expression.
  static const RE2 RE_ORDINAL("\\pN+(?:st|ST|nd|ND|rd|RD|th|TH)");

  // Perform some lexical cleaning and normalisation.
  uint8_t utf8[4];
  for (size_t s = 0; s != doc.sentences.size(); ++s) {
    cs::Sentence &sentence = doc.sentences[s];

    if (sent_contains_lower[s]) {
      // No truecasing required for normal looking sentences.
      for (cs::Token &token : sentence.span)
        token.ne_normalised = _get_token_norm_raw(token);
    }
    else {
      // Attempt to truecase the uppercase sentence.
      for (cs::Token &token : sentence.span) {
        const std::string &orig = _get_token_norm_raw(token);
        const UnicodeString lower = UnicodeString::from_utf8(orig).to_lower();
        const UnicodeString title = UnicodeString::from_utf8(orig).to_title();
        const UnicodeString upper = UnicodeString::from_utf8(orig).to_upper();
        std::array<std::string, 3> options = {{
            lower.to_utf8(),
            title.to_utf8(),
            upper.to_utf8()
        }};

        // Get the frequencies of the three capitalisation variants from the Brown clusters, and sort them in ascending order of frequency.
        std::array<std::tuple<unsigned int, std::string *>, 3> brown_counts = {{
            std::make_tuple(_brown_clusters.get_frequency(options[0]), &options[0]),
            std::make_tuple(_brown_clusters.get_frequency(options[1]), &options[1]),
            std::make_tuple(_brown_clusters.get_frequency(options[2]), &options[2])
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
      }  // for each token
    }  // end else

    // Replace all digits with "9" and all orginals with "9th".
    for (cs::Token &token : sentence.span) {
      std::string ne_normalised;
      ne_normalised.reserve(token.ne_normalised.size());

      // Is it an ordinal?
      if (RE2::FullMatch(token.ne_normalised, RE_ORDINAL)) {
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


void
Extractor::phase1_bos(cs::Sentence &sentence) {
  _offsets_token_ne_normalised.set_slice(sentence.span);
  _offsets_token_norm_raw.set_slice(sentence.span);
}


void
Extractor::phase2_begin(void) {
  LOG2(INFO, _logger) << "Extractor phase2_begin" << std::endl;
}


void
Extractor::phase2_bos(cs::Sentence &sentence) {
  _offsets_token_ne_normalised.set_slice(sentence.span);
  _offsets_token_norm_raw.set_slice(sentence.span);
}


void
Extractor::phase2_end(void) {
  LOG2(INFO, _logger) << "Extractor phase2_end" << std::endl;
}

}  // namesapce ner
}  // namespace tagger
}  // namespace schwa
