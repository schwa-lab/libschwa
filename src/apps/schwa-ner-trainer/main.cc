/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>

#include <schwa/canonical-schema.h>
#include <schwa/config.h>
#include <schwa/dr/config.h>
#include <schwa/io/logging.h>
#include <schwa/learn.h>
#include <schwa/tagger/ner.h>

namespace cf = ::schwa::config;
namespace cs = ::schwa::canonical_schema;
namespace dr = ::schwa::dr;
namespace io = ::schwa::io;
namespace ln = ::schwa::learn;
namespace ner = ::schwa::tagger::ner;


class Main : public cf::Main {
public:
  cs::Doc::Schema schema;

  cf::Op<std::string> input_path;
  cf::Op<std::string> model_path;
  cf::Op<std::string> extracted_path;
  cf::Op<bool> extract_only;
  ner::ModelParams model_params;
  ln::CRFSuiteTrainerParams trainer_params;
  dr::DocrepGroup dr;

  Main(void) :
      cf::Main("schwa-ner-trainer", "Schwa Lab NER tag trainer. Linear CRF backed by crfsuite."),
      input_path(*this, "input", 'i', "The input path", io::STDIN_STRING),
      model_path(*this, "model", 'm', "The model path"),
      extracted_path(*this, "dump-extracted", "The path to dump the extracted features in crfsuite format", cf::Flags::OPTIONAL),
      extract_only(*this, "extract-only", "Whether to perform feature extraction only and no training", false),
      model_params(*this, "model-params", "Parameters controlling the contents of the produced model"),
      trainer_params(*this, "train-params", "Parameters to the crfsuite training process"),
      dr(*this, schema)
    { }
  virtual ~Main(void) { }
};


namespace schwa {
namespace tagger {
namespace ner {

template <typename TRANSFORMER>
static void
run_trainer(const Main &cfg, TRANSFORMER &transformer, OutputModel &model) {
  // Create the feature extractor.
  Extractor extractor(model);

  // Create the trainer.
  ln::CRFSuiteTrainer<Extractor> trainer(extractor, model, cfg.trainer_params);

  {
    // Construct a resettable docrep reader over the provided stream.
    io::InputStream in(cfg.input_path());
    ln::ResettableDocrepReader<cs::Doc> doc_reader(in, cfg.schema, true);

    // Extract the features.
    trainer.extract<TRANSFORMER>(doc_reader, transformer);
  }

  // Optionally dump out the extracted features.
  if (cfg.extracted_path.was_mentioned()) {
    io::OutputStream out(cfg.extracted_path());
    trainer.dump_crfsuite_data(out);
  }

  if (cfg.extract_only())
    return;

  // Train the model.
  trainer.train();
}

}  // namespace ner
}  // namespace tagger
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  Main cfg;

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Open the model. The validation of the model path needs to happen as early as possible.
    ner::OutputModel model(cfg.model_path(), cfg.model_params, cfg);

    // Create the feature transformer.
    if (cfg.model_params.feature_hashing.was_mentioned()) {
      ln::HasherTransform<> transformer(cfg.model_params.feature_hashing());
      ner::run_trainer(cfg, transformer, model);
    }
    else {
      ln::NoTransform transformer;
      ner::run_trainer(cfg, transformer, model);
    }
  })
  return 0;
}
