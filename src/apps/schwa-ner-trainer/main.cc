/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
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
  cf::Op<bool> crf1_only;
  cf::Op<bool> crf2_only;
  cf::Op<bool> extract_only;
  cf::Op<bool> single_fold_only;
  cf::Op<unsigned int> nthreads;
  ner::ModelParams model_params;
  ln::CRFsuiteTrainerParams crf1_trainer_params;
  ln::CRFsuiteTrainerParams crf2_trainer_params;
  dr::DocrepGroup dr;

  Main(void) :
      cf::Main("schwa-ner-trainer", "Schwa Lab NER tag trainer. Linear CRF backed by CRFsuite."),
      input_path(*this, "input", 'i', "The input path", io::STDIN_STRING),
      model_path(*this, "model", 'm', "The model path"),
      extracted_path(*this, "dump-extracted", "The path to dump the extracted features in CRFsuite format", cf::Flags::OPTIONAL),
      crf1_only(*this, "crf1-only", "Whether only the 1st stage CRF training should happen", false),
      crf2_only(*this, "crf2-only", "Whether only the 2nd stage CRF training should happen", false),
      extract_only(*this, "extract-only", "Whether to perform feature extraction only and no training", false),
      single_fold_only(*this, "single-fold-only", "When performing crf1-only training, whether to train only a single fold or all folds", false),
      nthreads(*this, "nthreads", 'j', "How many threads to use to train the 1st stage CRFs", NFOLDS + 1),
      model_params(*this, "model-params", "Parameters controlling the contents of the produced model"),
      crf1_trainer_params(*this, "crf1-train-params", "Parameters to the CRFsuite training process for the 1st stage CRF"),
      crf2_trainer_params(*this, "crf2-train-params", "Parameters to the CRFsuite training process for the 2nd stage CRF"),
      dr(*this, schema)
    { }
  virtual ~Main(void) { }
};


namespace schwa {
namespace tagger {
namespace ner {

static std::queue<int> thread_work_queue;
static std::mutex thread_work_queue_mutex;


static std::string
get_crf1_model_suffix(const Main &cfg, const int fold) {
  std::ostringstream ss;
  if (!cfg.crf1_only())
    ss << ".crf1";
  if (fold >= 0)
    ss << ".fold" << fold;
  return ss.str();
}


static void
split_docs_into_folds(const std::vector<cs::Doc *> &docs, const unsigned int fold, std::vector<cs::Doc *> &fold_docs, std::vector<cs::Doc *> &nonfold_docs) {
  // Split the docs into folds.
  unsigned int fold_size = docs.size() / NFOLDS;
  if (docs.size() % NFOLDS != 0)
    ++fold_size;

  // Allocate space for the documents which are in and not in the current fold.
  fold_docs.clear();
  fold_docs.reserve(fold_size);
  nonfold_docs.clear();
  nonfold_docs.reserve((NFOLDS - 1) * fold_size);

  // Separate out the documents which are part of this fold from the ones that are not.
  auto it = docs.begin();
  for (unsigned int f = 0; f != NFOLDS; ++f) {
    for (unsigned int s = 0; s != fold_size && it != docs.end(); ++s, ++it) {
      if (f == fold)
        fold_docs.push_back(*it);
      else
        nonfold_docs.push_back(*it);
    }
  }
}


template <typename IT, typename TRANSFORMER>
static void
run_trainer1(const Main &cfg, Extractor &extractor, OutputModel &model, const IT docs_begin, const IT docs_end, TRANSFORMER &transformer, const int fold=-1) {
  // Construct the path suffix if we're training a fold.
  const std::string suffix = get_crf1_model_suffix(cfg, fold);

  if (fold >= 0)
    LOG(INFO) << "Training 1st stage NER classifier for fold " << fold << std::endl;
  else
    LOG(INFO) << "Training 1st stage NER classifier using all training data" << std::endl;

  // Create the trainer for the 1st stage classifier.
  ln::CRFsuiteTrainer<Extractor> trainer(extractor, model, cfg.crf1_trainer_params);

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
  trainer.set_model_filename_suffix(suffix);
  trainer.train();
}


template <typename IT, typename TRANSFORMER>
static void
run_trainer2(const Main &cfg, Extractor &extractor, OutputModel &model, const IT docs_begin, const IT docs_end, TRANSFORMER &transformer) {
  LOG(INFO) << "Training 2nd stage NER classifier" << std::endl;

  // Create the trainer for the 1st stage classifier.
  ln::CRFsuiteTrainer<Extractor> trainer(extractor, model, cfg.crf2_trainer_params);

  // Extract the features for the 1st stage classifier.
  trainer.extract<IT, TRANSFORMER>(docs_begin, docs_end, transformer);

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


template <typename IT, typename TRANSFORMER>
static void
run_tagger1(const Main &cfg, Extractor &extractor, const std::string &model_path, const IT docs_begin, const IT docs_end, TRANSFORMER &transformer, const unsigned int fold) {
  LOG(INFO) << "Tagging with 1st stage NER classifier for fold " << fold << std::endl;

  // Create the tagger for the 1st stage classifier.
  ln::CRFsuiteTagger<Extractor> tagger(extractor, model_path + get_crf1_model_suffix(cfg, fold));

  // Extract the features for the 1st stage classifier.
  tagger.tag<IT, TRANSFORMER>(docs_begin, docs_end, transformer);
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

  // Allocate space for the documents which are in and not in the current fold.
  std::vector<cs::Doc *> fold_docs, nonfold_docs;
  split_docs_into_folds(docs, fold, fold_docs, nonfold_docs);

  // Train a model on the documents that are not in the current fold.
  run_trainer1(cfg, extractor, model, nonfold_docs.begin(), nonfold_docs.end(), transformer, fold);

  if (cfg.extract_only())
    return;

  // Tag the documents which are in the current fold with the newly trained model.
  run_tagger1(cfg, extractor, model.model_path(), fold_docs.begin(), fold_docs.end(), transformer, fold);
}


template <typename TRANSFORMER>
static void
run_fold1_thread(const Main &cfg, OutputModel &model, TRANSFORMER &transformer, std::vector<cs::Doc *> &docs) {
  // Loop until there's no more work to do on the work queue.
  int fold = 0;
  bool finished = false;
  while (!finished) {
    // Critical section while we examine the thread work queue.
    {
      std::lock_guard<std::mutex> lock(thread_work_queue_mutex);
      if (thread_work_queue.empty())
        finished = true;
      else {
        fold = thread_work_queue.front();
        thread_work_queue.pop();
      }
    }

    if (!finished)
      run_fold1<TRANSFORMER>(cfg, model, transformer, docs, fold);
  }
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

  // 1st stage classifier.
  if (!cfg.crf2_only()) {
    // Prepare the docs, running with threads and without 2nd stage CRF.
    for (cs::Doc *doc : docs)
      Extractor::prepare_doc(*doc, true, false, model.tag_encoding());

    // Do we only need to train a single fold or all folds?
    if (cfg.single_fold_only()) {
      // Train over all of the documents.
      run_fold1<TRANSFORMER>(cfg, model, transformers[0], docs, -1);
    }
    else {
      // Push work onto the the thread work queue.
      for (int fold = -1; fold != NFOLDS; ++fold)
        thread_work_queue.push(fold);

      // Fire up n threads (n \in [1, NFOLDS + 1]) and wait for them to finish.
      const unsigned int nthreads = std::max(1u, std::min(NFOLDS + 1, cfg.nthreads()));
      LOG(DEBUG) << "Firing up " << nthreads << " threads..." << std::endl;
      std::vector<std::thread> threads;
      for (unsigned int i = 0; i != nthreads; ++i)
        threads.push_back(std::thread(&run_fold1_thread<TRANSFORMER>, std::ref(cfg), std::ref(model), std::ref(transformers[i + 1]), std::ref(docs)));
      for (auto &thread : threads)
        thread.join();
    }
  }

  // 2nd stage classifier.
  if (!cfg.crf1_only()) {
    // If we're only running the 2nd stage CRF, tag the documents with the 1st stage models.
    if (cfg.crf2_only()) {
      std::vector<cs::Doc *> fold_docs, nonfold_docs;
      Extractor extractor(model, false, true);
      for (unsigned int fold = 0; fold != NFOLDS; ++fold) {
        split_docs_into_folds(docs, fold, fold_docs, nonfold_docs);
        run_tagger1(cfg, extractor, model.model_path(), fold_docs.begin(), fold_docs.end(), transformers[0], fold);
      }
    }

    // Create the feature extractor for the 2nd stage classifier and run it with the trainer.
    Extractor extractor(model, true, false);
    run_trainer2(cfg, extractor, model, docs.begin(), docs.end(), transformers[0]);
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
