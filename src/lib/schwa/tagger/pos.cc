/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tagger/pos.h>

#include <schwa/io/logging.h>
#include <schwa/io/paths.h>


namespace schwa {
namespace tagger {

// ============================================================================
// POSModelParams
// ============================================================================
const unsigned int POSModelParams::DEFAULT_RARE_TOKEN_CUTOFF = 5;

POSModelParams::POSModelParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags) :
    learn::ModelParams(group, name, desc, flags),
    feature_hashing(*this, "feature-hashing", 'H', "Number of bits to use for feature hashing", config::Flags::OPTIONAL),
    lexicon_path(*this, "lexicon-path", "Relative path to the lexicon file within the model directory", "./lexicon"),
    rare_token_cutoff(*this, "rare-word-cutoff", "Tokens which appear less than this number of times during training are considered as rare", DEFAULT_RARE_TOKEN_CUTOFF)
  { }

POSModelParams::~POSModelParams(void) { }


// ============================================================================
// POSInputModel
// ============================================================================
POSInputModel::POSInputModel(const std::string &path, POSModelParams &params) :
    learn::InputModel(path, params),
    _lexicon("tokens")
  {
  // Set our member variables only after the parent class has loaded the config file.
  _lexicon_path = io::path_join(_path, params.lexicon_path());
  _rare_token_cutoff = params.rare_token_cutoff();

  // Load the lexicon.
  {
    io::InputStream in(io::path_join(_path, _lexicon_path));
    _lexicon.deserialise(in);
  }
}

POSInputModel::~POSInputModel(void) { }


// ============================================================================
// POSOutputModel
// ============================================================================
POSOutputModel::POSOutputModel(const std::string &path, const POSModelParams &params, const config::Main &main_config) :
    learn::OutputModel(path, params, main_config),
    _lexicon_path(io::path_join(_path, params.lexicon_path())),
    _rare_token_cutoff(params.rare_token_cutoff()),
    _lexicon("tokens")
  { }

POSOutputModel::~POSOutputModel(void) {
  io::OutputStream out(_lexicon_path);
  _lexicon.serialise(out);
}


// ============================================================================
// POSExtractor
// ============================================================================
POSExtractor::POSExtractor(POSInputModel &model) :
    _is_train(false),
    _rare_token_cutoff(model.rare_token_cutoff()),
    _lexicon(model.lexicon()),
    _logger(*io::default_logger),
    _offsets_token_norm_raw(&_get_token_norm_raw)
  { }


POSExtractor::POSExtractor(POSOutputModel &model) :
    _is_train(true),
    _rare_token_cutoff(model.rare_token_cutoff()),
    _lexicon(model.lexicon()),
    _logger(model.logger()),
    _offsets_token_norm_raw(&_get_token_norm_raw)
  { }


void
POSExtractor::phase1_begin(void) {
  LOG2(INFO, _logger) << "POSExtractor phase1_begin" << std::endl;
}

void
POSExtractor::phase1_end(void) {
  LOG2(INFO, _logger) << "POSExtractor phase1_end" << std::endl;
}


void
POSExtractor::phase1_extract(canonical_schema::Token &token, size_t) {
  if (_is_train) {
    // Add the current token to the lexicon.
    _lexicon.add(_get_token_norm_raw(token));
  }
}


void
POSExtractor::phase2_begin(void) {
  LOG2(INFO, _logger) << "POSExtractor phase2_begin" << std::endl;
}


void
POSExtractor::phase2_bos(canonical_schema::Sentence &sentence) {
  _offsets_token_norm_raw.set_slice(sentence.span);
}


void
POSExtractor::phase2_end(void) {
  LOG2(INFO, _logger) << "POSExtractor phase2_end" << std::endl;
}

}  // namespace tagger
}  // namespace schwa
