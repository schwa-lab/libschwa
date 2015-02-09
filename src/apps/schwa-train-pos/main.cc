/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>
#include <vector>

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

namespace schwa {
namespace tagger {

template <typename TRANSFORMER>
static void
train_pos(io::InputStream &in, cs::Doc::Schema &schema, io::OutputStream &model, TRANSFORMER &transformer) {
  // Construct a docrep readerover the provided stream.
  dr::Reader reader(in, schema);

  // Read the documents off the input stream into memory.
  std::vector<cs::Doc *> docs;
  try {
    while (true) {
      cs::Doc *doc = new cs::Doc();
      if (!(reader >> *doc)) {
        delete doc;
        break;
      }
      docs.push_back(doc);
    }
  }
  catch (dr::ReaderException &) {
    LOG(WARNING) << "Failed to read document from '" << in.path() << "'" << std::endl;
  }

  // Create the feature extractor.
  POSExtractor extractor;

  // Create the trainer.
  learn::CRFSuiteTrainer<decltype(extractor)> trainer(extractor);

  // Extract the features.
  trainer.extract<TRANSFORMER>(docs, transformer);

  // Delete the docs.
  for (cs::Doc *doc : docs)
    delete doc;
  docs.clear();

  // Train the model.
  trainer.train(model.path());
}

}  // namespace tagger
}  // namespace schwa


int
main(int argc, char **argv) {
  // Instantiate a document schema for use in the docrep config framework.
  cs::Doc::Schema schema;

  // Construct an option parser.
  cf::Main cfg("schwa-train-pos", "Schwa Lab POS tag trainer. 1D linear CRF with LBFGS.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> model_path(cfg, "model", 'm', "The model path", io::STDOUT_STRING);
  cf::Op<size_t> feature_hashing(cfg, "feature-hashing", 'H', "Number of bits to use for feature hashing.", cf::Flags::OPTIONAL);
  dr::DocrepGroup dr(cfg, schema);

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Open the input data and model paths.
    io::InputStream in(input_path());
    io::OutputStream model(model_path());

    // Create the feature extractor.
    if (feature_hashing.was_mentioned()) {
      schwa::learn::HasherTransform<> transformer(feature_hashing());
      schwa::tagger::train_pos(in, schema, model, transformer);
    }
    else {
      schwa::learn::NoTransform transformer;
      schwa::tagger::train_pos(in, schema, model, transformer);
    }
  })
  return 0;
}
