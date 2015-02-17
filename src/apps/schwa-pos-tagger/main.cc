/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>

#include <schwa/canonical-schema.h>
#include <schwa/config.h>
#include <schwa/dr/config.h>
#include <schwa/io/logging.h>
#include <schwa/learn.h>
#include <schwa/tagger/pos.h>

namespace cf = ::schwa::config;
namespace cs = ::schwa::canonical_schema;
namespace dr = ::schwa::dr;
namespace io = ::schwa::io;
namespace ln = ::schwa::learn;
namespace pos = ::schwa::tagger::pos;


class TaggerMain : public cf::Main {
public:
  cs::Doc::Schema schema;

  cf::Op<std::string> input_path;
  cf::Op<std::string> model_path;
  cf::Op<bool> retain_docs;
  pos::ModelParams model_params;
  dr::DocrepGroup dr;

  TaggerMain(void) :
      cf::Main("schwa-pos-tagger", "Schwa Lab POS tagger. Linear CRF backed by crfsuite."),
      input_path(*this, "input", 'i', "The input path", io::STDIN_STRING),
      model_path(*this, "model", 'm', "The model path"),
      retain_docs(*this, "retain-docs", "Read the documents into memory instead of reading multiple times from disk (useful if input is a pipe)", false),
      model_params(*this, "model-params", "The model path path"),
      dr(*this, schema)
    { }
  virtual ~TaggerMain(void) { }
};


namespace schwa {
namespace tagger {
namespace pos {

template <typename TRANSFORMER>
static void
run_tagger(const TaggerMain &cfg, TRANSFORMER &transformer, InputModel &model) {
  // Create the feature extractor.
  Extractor extractor(model);

  // Create the tagger.
  ln::CRFSuiteTagger<Extractor> tagger(extractor, model);

  {
    // Construct a resettable docrep reader over the provided stream.
    io::InputStream in(cfg.input_path());
    ln::ResettableDocrepReader<cs::Doc> doc_reader(in, cfg.schema, cfg.retain_docs());

    // Extract the features.
    tagger.tag<TRANSFORMER>(doc_reader, transformer);
  }
}

}  // namespace pos
}  // namespace tagger
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  TaggerMain cfg;

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Open the model. The validation of the model path needs to happen as early as possible.
    pos::InputModel model(cfg.model_path(), cfg.model_params);

    // Create the feature transformer.
    if (cfg.model_params.feature_hashing.was_mentioned()) {
      ln::HasherTransform<> transformer(cfg.model_params.feature_hashing());
      pos::run_tagger(cfg, transformer, model);
    }
    else {
      ln::NoTransform transformer;
      pos::run_tagger(cfg, transformer, model);
    }
  })
  return 0;
}
