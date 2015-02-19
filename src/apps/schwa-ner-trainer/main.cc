/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <schwa/canonical-schema.h>
#include <schwa/config.h>
#include <schwa/dr/config.h>
#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/learn.h>
#include <schwa/tagger/ner.h>

namespace cf = ::schwa::config;
namespace cs = ::schwa::canonical_schema;
namespace dr = ::schwa::dr;
namespace io = ::schwa::io;
namespace ln = ::schwa::learn;
namespace ner = ::schwa::tagger::ner;

static constexpr const unsigned int NFOLDS = 10;


class Main : public cf::Main {
public:
  cs::Doc::Schema schema;

  cf::Op<std::string> input_path;
  cf::Op<std::string> model_path;
  cf::Op<std::string> extracted_path;
  cf::Op<bool> extract_only;
  ner::ModelParams model_params;
  ln::CRFsuiteTrainerParams trainer_params;
  dr::DocrepGroup dr;

  Main(void) :
      cf::Main("schwa-ner-trainer", "Schwa Lab NER tag trainer. Linear CRF backed by CRFsuite."),
      input_path(*this, "input", 'i', "The input path", io::STDIN_STRING),
      model_path(*this, "model", 'm', "The model path"),
      extracted_path(*this, "dump-extracted", "The path to dump the extracted features in CRFsuite format", cf::Flags::OPTIONAL),
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


static std::string
get_fold_suffix(const unsigned int fold) {
  std::ostringstream ss;
  ss << ".fold" << fold;
  return ss.str();
}


template <typename IT, typename TRANSFORMER>
static void
run_trainer1(const Main &cfg, Extractor &extractor, OutputModel &model, const IT docs_begin, const IT docs_end, TRANSFORMER &transformer, const int fold=-1) {
  // Construct the path suffix if we're training a fold.
  std::string suffix;
  if (fold >= 0)
    suffix = get_fold_suffix(fold);

  if (fold >= 0)
    LOG(INFO) << "Training 1st stage NER classifier for fold " << fold << std::endl;
  else
    LOG(INFO) << "Training 1st stage NER classifier for all training data" << std::endl;

  // Create the trainer for the 1st stage classifier.
  ln::CRFsuiteTrainer<Extractor> trainer(extractor, model, cfg.trainer_params);

  // Extract the features for the 1st stage classifier.
  trainer.extract<IT, TRANSFORMER>(docs_begin, docs_end, transformer);

  // Optionally dump out the extracted features.
  if (cfg.extracted_path.was_mentioned()) {
    io::OutputStream out(cfg.extracted_path() + suffix);
    trainer.dump_crfsuite_data(out);
  }

  if (cfg.extract_only())
    return;

  // Train the model.
  if (fold >= 0)
    trainer.set_model_filename_suffix(suffix);
  trainer.train();
}


template <typename IT, typename TRANSFORMER>
static void
run_tagger1(Extractor &extractor, const std::string &model_path, const IT docs_begin, const IT docs_end, TRANSFORMER &transformer, const unsigned int fold) {
  LOG(INFO) << "Tagging with 1st stage NER classifier for fold " << fold << std::endl;

  // Create the tagger for the 1st stage classifier.
  ln::CRFsuiteTagger<Extractor> tagger(extractor, model_path + get_fold_suffix(fold));

  // Extract the features for the 1st stage classifier.
  tagger.tag<IT, TRANSFORMER>(docs_begin, docs_end, transformer);
  tagger.dump_accuracy();
}


template <typename TRANSFORMER>
static void
run_fold1(const Main &cfg, OutputModel &model, TRANSFORMER &transformer, std::vector<cs::Doc *> &docs, const int _fold) {
  // Create the feature extractor for the 1st stage classifier.
  Extractor extractor(model, false, true);

  // If we aren't running a fold, just train on all of the documents.
  if (_fold < 0) {
    // Train the model.
    run_trainer1(cfg, extractor, model, docs.begin(), docs.end(), transformer);
    return;
  }
  const unsigned int fold = static_cast<unsigned int>(_fold);

  // Split the docs into folds.
  unsigned int fold_size = docs.size() / NFOLDS;
  if (docs.size() % NFOLDS != 0)
    ++fold_size;

  std::vector<cs::Doc *> fold_docs, nonfold_docs;
  fold_docs.reserve(fold_size);
  nonfold_docs.reserve((NFOLDS - 1) * fold_size);

  // Place the documents that aren't in the current fold into a vector for use as training data.
  auto it = docs.begin();
  for (unsigned int f = 0; f != NFOLDS; ++f) {
    for (unsigned int s = 0; s != fold_size && it != docs.end(); ++s, ++it) {
      if (f == fold)
        fold_docs.push_back(*it);
      else
        nonfold_docs.push_back(*it);
    }
  }

  // Train the model.
  run_trainer1(cfg, extractor, model, nonfold_docs.begin(), nonfold_docs.end(), transformer, fold);

  // Tag the fold with the trained model.
  if (!cfg.extract_only())
    run_tagger1(extractor, model.model_path(), fold_docs.begin(), fold_docs.end(), transformer, fold);
}


template <typename TRANSFORMER>
static void
run_trainer(const Main &cfg, std::vector<TRANSFORMER> &transformers, OutputModel &model) {
  assert(transformers.size() == NFOLDS + 1);

  // Read in each of the docrep documents from the input stream, and pre-process them.
  std::vector<cs::Doc *> docs;
  {
    io::InputStream in(cfg.input_path());
    dr::Reader reader(in, cfg.schema);
    while (true) {
      cs::Doc *doc = new cs::Doc();
      if (reader >> *doc) {
        ner::preprocess_doc(*doc, model.brown_clusters());
        docs.push_back(doc);
      }
      else {
        delete doc;
        break;
      }
    }
  }

  // Ensure we have enough documents for training.
  if (docs.size() < NFOLDS) {
    std::ostringstream ss;
    ss << "Not enough documents (" << docs.size() << ") to perform training. Need at least " << NFOLDS << ".";
    throw ValueException(ss.str());
  }

  // Split the docs into folds.
  unsigned int fold_size = docs.size() / NFOLDS;
  if (docs.size() % NFOLDS != 0)
    ++fold_size;

  // 1st stage classifier.
  {
    for (cs::Doc *doc : docs)
      Extractor::do_phase2_bod(*doc, false, true, model.tag_encoding());

    std::vector<std::thread> threads;
    for (int i = -1; i != NFOLDS; ++i)
      threads.push_back(std::thread(&run_fold1<TRANSFORMER>, std::ref(cfg), std::ref(model), std::ref(transformers[i + 1]), std::ref(docs), i));
    for (auto &thread : threads)
      thread.join();
  }

  // Delete the read in docs.
  for (cs::Doc *doc : docs)
    delete doc;
  docs.clear();
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
    cfg.main<io::ThreadsafePrettyLogger>(argc, argv);

    // Open the model. The validation of the model path needs to happen as early as possible.
    ner::OutputModel model(cfg.model_path(), cfg.model_params, cfg);

    // Create the feature transformer.
    if (cfg.model_params.feature_hashing.was_mentioned()) {
      std::vector<ln::HasherTransform<>> transformers(NFOLDS + 1, ln::HasherTransform<>(cfg.model_params.feature_hashing()));
      ner::run_trainer(cfg, transformers, model);
    }
    else {
      std::vector<ln::NoTransform> transformers(NFOLDS + 1);
      ner::run_trainer(cfg, transformers, model);
    }
  })
  return 0;
}
