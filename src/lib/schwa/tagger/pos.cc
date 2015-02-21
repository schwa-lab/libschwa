/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tagger/pos.h>

#include <schwa/io/logging.h>
#include <schwa/io/paths.h>


namespace schwa {
namespace tagger {
namespace pos {

// ============================================================================
// ModelParams
// ============================================================================
const unsigned int ModelParams::DEFAULT_RARE_TOKEN_CUTOFF = 5;

ModelParams::ModelParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags) :
    learn::ModelParams(group, name, desc, flags),
    feature_hashing(*this, "feature-hashing", 'H', "Number of bits to use for feature hashing", config::Flags::OPTIONAL),
    lexicon_path(*this, "lexicon-path", "Relative path to the lexicon file within the model directory", "./lexicon"),
    rare_token_cutoff(*this, "rare-word-cutoff", "Tokens which appear less than this number of times during training are considered as rare", DEFAULT_RARE_TOKEN_CUTOFF),
    brown_clusters_path(*this, "brown-clusters-path", "Path to the Brown cluster paths file", config::Flags::OPTIONAL)
  { }

ModelParams::~ModelParams(void) { }


// ============================================================================
// InputModel
// ============================================================================
InputModel::InputModel(const std::string &path, ModelParams &params) :
    learn::InputModel(path, params),
    _pool(4 * 1024 * 1024),
    _string_pool(_pool),
    _lexicon("tokens"),
    _brown_clusters(_string_pool)
  {
  // Set our member variables only after the parent class has loaded the config file.
  _lexicon_path = io::path_join(_path, params.lexicon_path());
  _rare_token_cutoff = params.rare_token_cutoff();

  // Load the lexicon.
  {
    io::InputStream in(io::path_join(_path, _lexicon_path));
    _lexicon.deserialise(in);
  }

  // Load the Brown clusters.
  if (params.brown_clusters_path.was_mentioned()) {
    io::InputStream in(params.brown_clusters_path());
    _brown_clusters.load(in);
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
    _lexicon_path(io::path_join(_path, params.lexicon_path())),
    _rare_token_cutoff(params.rare_token_cutoff()),
    _lexicon("tokens"),
    _brown_clusters(_string_pool)
  {
  // Load the Brown clusters.
  if (params.brown_clusters_path.was_mentioned()) {
    io::InputStream in(params.brown_clusters_path());
    _brown_clusters.load(in);
  }
}

OutputModel::~OutputModel(void) {
  io::OutputStream out(_lexicon_path);
  _lexicon.serialise(out);
}


// ============================================================================
// Extractor
// ============================================================================
Extractor::Extractor(InputModel &model) :
    _rare_token_cutoff(model.rare_token_cutoff()),
    _lexicon(model.lexicon()),
    _brown_clusters(model.brown_clusters()),
    _brown_cluster_path(nullptr),
    _brown_cluster_path_lengths(new unsigned int[_brown_clusters.npaths()]),
    _brown_cluster_feature(new char[8 + _brown_clusters.path_lengths().back() + 1]),
    _logger(*io::default_logger),
    _offsets_token_norm_raw(&_get_token_norm_raw)
  { }


Extractor::Extractor(OutputModel &model) :
    _rare_token_cutoff(model.rare_token_cutoff()),
    _lexicon(model.lexicon()),
    _brown_clusters(model.brown_clusters()),
    _brown_cluster_path(nullptr),
    _brown_cluster_path_lengths(new unsigned int[_brown_clusters.npaths()]),
    _brown_cluster_feature(new char[8 + _brown_clusters.path_lengths().back() + 1]),
    _logger(model.logger()),
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
Extractor::phase1_extract(canonical_schema::Sentence &, canonical_schema::Token &token) {
  // Add the current token to the lexicon.
  _lexicon.add(_get_token_norm_raw(token));
}


void
Extractor::phase3_bos(canonical_schema::Sentence &sentence) {
  _offsets_token_norm_raw.set_slice(sentence.span);
}

}  // namespace pos
}  // namespace tagger
}  // namespace schwa
