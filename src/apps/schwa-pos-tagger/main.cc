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
namespace tg = ::schwa::tagger;


class POSTrainMain : public cf::Main {
public:
  cs::Doc::Schema schema;

  cf::Op<std::string> input_path;
  cf::Op<std::string> model_path;
  cf::Op<bool> retain_docs;
  tg::POSModelParams model_params;
  dr::DocrepGroup dr;

  POSTrainMain(void) :
      cf::Main("schwa-pos-tagger", "Schwa Lab POS tagger. Linear CRF backed by crfsuite."),
      input_path(*this, "input", 'i', "The input path", io::STDIN_STRING),
      model_path(*this, "model", 'm', "The model path"),
      retain_docs(*this, "retain-docs", "Read the documents into memory instead of reading multiple times from disk (useful if input is a pipe)", false),
      model_params(*this, "model-params", "The model path path"),
      dr(*this, schema)
    { }
  virtual ~POSTrainMain(void) { }
};


namespace schwa {
namespace tagger {

template <typename TRANSFORMER>
static void
pos_tagger(const POSTrainMain &cfg, TRANSFORMER &transformer, POSInputModel &model) {
  // Create the feature extractor.
  POSExtractor extractor(model);

  // Create the tagger.
  ln::CRFSuiteTagger<POSExtractor> tagger(extractor, model);

  {
    // Construct a resettable docrep reader over the provided stream.
    io::InputStream in(cfg.input_path());
    ln::ResettableDocrepReader<cs::Doc> doc_reader(in, cfg.schema, cfg.retain_docs());

    // Extract the features.
    tagger.tag<TRANSFORMER>(doc_reader, transformer);
  }
}

}  // namespace tagger
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  POSTrainMain cfg;

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Open the model. The validation of the model path needs to happen as early as possible.
    tg::POSInputModel model(cfg.model_path(), cfg.model_params);

    // Create the feature transformer.
    if (cfg.model_params.feature_hashing.was_mentioned()) {
      ln::HasherTransform<> transformer(cfg.model_params.feature_hashing());
      tg::pos_tagger(cfg, transformer, model);
    }
    else {
      ln::NoTransform transformer;
      tg::pos_tagger(cfg, transformer, model);
    }
  })
  return 0;
}
