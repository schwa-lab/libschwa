/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>

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
  cf::Op<std::string> output_path;
  cf::Op<std::string> conll_path;
  cf::Op<std::string> model_path;
  ner::ModelParams model_params;
  cf::Op<bool> crf1_only;
  cf::OpSequenceTagEncoding conll_tag_encoding;
  dr::DocrepGroup dr;

  Main(void) :
      cf::Main("schwa-ner-tagger", "Schwa Lab NER tagger. Linear CRF backed by CRFsuite."),
      input_path(*this, "input", 'i', "The input path", io::STDIN_STRING),
      output_path(*this, "output", 'o', "The output path", io::STDOUT_STRING),
      conll_path(*this, "conll", 'c', "The output path for the CoNLL formatted tags", cf::Flags::OPTIONAL),
      model_path(*this, "model", 'm', "The model path"),
      model_params(*this, "model-params", "The model path path"),
      crf1_only(*this, "crf1-only", "Whether only the 1st stage CRF tagging should happen", false),
      conll_tag_encoding(*this, "conll-tag-encoding", "Which sequence tag encoding to use for the CoNLL output", "iob1"),
      dr(*this, schema)
    { }
  virtual ~Main(void) { }
};


namespace schwa {
namespace tagger {
namespace ner {


static const auto REVERSE_NES = DR_REVERSE_SLICES(&cs::Doc::named_entities, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Token::ne);
static const auto SEQUENCE_TAG_GOLD_NES = DR_SEQUENCE_TAGGER(&cs::Doc::named_entities, &cs::Doc::sentences, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Sentence::span, &cs::Token::ne, &cs::NamedEntity::label, &cs::Token::ne_label_crf1);
static const auto SEQUENCE_TAG_CRF2_NES = DR_SEQUENCE_TAGGER(&cs::Doc::named_entities, &cs::Doc::sentences, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Sentence::span, &cs::Token::ne, &cs::NamedEntity::label, &cs::Token::ne_label_crf2);
static const auto SEQUENCE_UNTAG_NES = DR_SEQUENCE_UNTAGGER(&cs::Doc::named_entities, &cs::Doc::sentences, &cs::NamedEntity::span, &cs::Sentence::span, &cs::NamedEntity::label, &cs::Token::ne_label);


template <typename TRANSFORMER>
static void
run_tagger(Main &cfg, TRANSFORMER &transformer, InputModel &model) {
  // Open the docrep output stream.
  io::OutputStream out(cfg.output_path());
  dr::Writer writer(out, cfg.schema);

  // Open the CoNLL output stream, if requested.
  std::unique_ptr<io::OutputStream> conll_out;
  if (cfg.conll_path.was_mentioned())
    conll_out.reset(new io::OutputStream(cfg.conll_path()));

  // Create the 1st stage feature extractor and tagger.
  std::string crf1_model_path = model.model_path();
  if (!cfg.crf1_only())
    crf1_model_path += ".crf1";
  Extractor extractor1(model, false, false);
  ln::CRFsuiteTagger<Extractor> tagger1(extractor1, crf1_model_path);

  // Create the 2nd stage feature extractor and tagger.
  Extractor extractor2(model, true, false);
  std::unique_ptr<ln::CRFsuiteTagger<Extractor>> tagger2;
  if (!cfg.crf1_only())
    tagger2.reset(new ln::CRFsuiteTagger<Extractor>(extractor2, model.model_path()));

  // Read in each of the docrep documents from the input stream, and pre-process them.
  io::InputStream in(cfg.input_path());
  dr::Reader reader(in, cfg.schema);
  while (true) {
    // Read in the doc.
    std::unique_ptr<cs::Doc> doc(new cs::Doc());
    if (!(reader >> *doc))
      break;

    // Pre-process the doc.
    ner::preprocess_doc(*doc, model.brown_clusters());

    // Run the 1st stage CRF and optionally the 2nd stage CRF.
    tagger1.tag(*doc, transformer);
    if (!cfg.crf1_only())
      tagger2->tag(*doc, transformer);

    // Do we want CoNLL output?
    if (conll_out == nullptr) {
      // Untag the CRF sequence labels.
      doc->named_entities.clear();
      SEQUENCE_UNTAG_NES(*doc);
    }
    else {
      const SequenceTagEncoding conll_tag_encoding = cfg.conll_tag_encoding.encoding();

      // Sequence tag the gold named entities into the `ne_label_crf1` attribute.
      REVERSE_NES(*doc);
      SEQUENCE_TAG_GOLD_NES(*doc, conll_tag_encoding);

      // Untag the CRF sequence labels and tag them back onto the tokens.
      doc->named_entities.clear();
      SEQUENCE_UNTAG_NES(*doc);
      REVERSE_NES(*doc);
      SEQUENCE_TAG_CRF2_NES(*doc, conll_tag_encoding);

      **conll_out << "-DOCSTART- -X- -X- O O" << std::endl;
      **conll_out << std::endl;
      for (const cs::Sentence &sentence : doc->sentences) {
        for (const cs::Token &token : sentence.span) {
          **conll_out << token.raw << ' ' << token.ne_normalised << ' ' << token.pos << ' ';
          **conll_out << token.ne_label_crf1 << ' ' << token.ne_label_crf2 << std::endl;
        }
        **conll_out << std::endl;
      }
    }

    // Write out the doc as docrep.
    writer << *doc;
  }
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
