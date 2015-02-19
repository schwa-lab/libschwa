/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <schwa/canonical-schema.h>
#include <schwa/config.h>
#include <schwa/dr.h>
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
  ner::ModelParams model_params;
  dr::DocrepGroup dr;

  Main(void) :
      cf::Main("schwa-ner-tagger", "Schwa Lab NER tagger. Linear CRF backed by CRFsuite."),
      input_path(*this, "input", 'i', "The input path", io::STDIN_STRING),
      model_path(*this, "model", 'm', "The model path"),
      model_params(*this, "model-params", "The model path path"),
      dr(*this, schema)
    { }
  virtual ~Main(void) { }
};


namespace schwa {
namespace tagger {
namespace ner {

template <typename TRANSFORMER>
static void
run_tagger(const Main &cfg, TRANSFORMER &transformer, InputModel &model) {
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

  // Create the 1st stage feature extractor.
  Extractor extractor1(model, false, false);

  // Create the tagger.
  ln::CRFSuiteTagger<Extractor> tagger1(extractor1, model.model_path());

  static const auto SEQUENCE_UNTAG_CRF1_NES = DR_SEQUENCE_UNTAGGER(&cs::Doc::named_entities_crf1, &cs::Doc::sentences, &cs::NamedEntity::span, &cs::Sentence::span, &cs::NamedEntity::label, &cs::Token::ne_label);
  static const auto SEQUENCE_TAG_GOLD_NES = DR_SEQUENCE_TAGGER(&cs::Doc::named_entities, &cs::Doc::sentences, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Sentence::span, &cs::Token::ne, &cs::NamedEntity::label, &cs::Token::ne_label);
  static const auto SEQUENCE_TAG_CRF1_NES = DR_SEQUENCE_TAGGER(&cs::Doc::named_entities_crf1, &cs::Doc::sentences, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Sentence::span, &cs::Token::ne, &cs::NamedEntity::label, &cs::Token::ne_label_crf1);

  // Tag all of the docs.
  for (cs::Doc *doc : docs) {
    tagger1.tag(*doc, transformer);
    SEQUENCE_UNTAG_CRF1_NES(*doc);
    SEQUENCE_TAG_GOLD_NES(*doc, SequenceTagEncoding::IOB2);
    SEQUENCE_TAG_CRF1_NES(*doc, SequenceTagEncoding::IOB2);

    std::cout << "-DOCSTART- -DOCSTART- -X- O O" << std::endl;
    std::cout << std::endl;
    for (const cs::Sentence &sentence : doc->sentences) {
      for (const cs::Token &token : sentence.span) {
        std::cout << token.raw << ' ' << token.ne_normalised << ' ' << token.pos << ' ';
        std::cout << token.ne_label << ' ' << token.ne_label_crf1 << std::endl;
      }
      std::cout << std::endl;
    }
  }
  tagger1.dump_accuracy();

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
    cfg.main<io::PrettyLogger>(argc, argv);

    // Open the model. The validation of the model path needs to happen as early as possible.
    ner::InputModel model(cfg.model_path(), cfg.model_params);

    // Create the feature transformer.
    if (cfg.model_params.feature_hashing.was_mentioned()) {
      ln::HasherTransform<> transformer(cfg.model_params.feature_hashing());
      ner::run_tagger(cfg, transformer, model);
    }
    else {
      ln::NoTransform transformer;
      ner::run_tagger(cfg, transformer, model);
    }
  })
  return 0;
}
