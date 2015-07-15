/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <schwa/canonical-schema.h>
#include <schwa/config.h>
#include <schwa/dr/config.h>
#include <schwa/encoding.h>
#include <schwa/formats/plain-text.h>
#include <schwa/io/logging.h>
#include <schwa/tokenizer/tokenizer.h>
#include <schwa/utils/buffer.h>
#include <schwa/learn.h>
#include <schwa/tagger/ner.h>

namespace cf = ::schwa::config;
namespace cs = ::schwa::canonical_schema;
namespace dr = ::schwa::dr;
namespace fm = ::schwa::formats;
namespace io = ::schwa::io;
namespace tk = ::schwa::tokenizer;
namespace ln = ::schwa::learn;
namespace ner = ::schwa::tagger::ner;

class Main : public cf::Main {
public:
  cs::Doc::Schema schema;

  cf::Op<std::string> input_path;
  cf::Op<std::string> output_path;
  cf::Op<std::string> conll_path;
  cf::Op<std::string> model_path;
  cf::Op<std::string> input_encoding;
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
      input_encoding(*this, "input-encoding", 'e', "The encoding of the input stream", "utf-8"),
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
run_tagger(Main &cfg, TRANSFORMER &transformer, InputModel &model, std::unique_ptr<cs::Doc> &doc) {
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

}  // namespace ner
}  // namespace tagger
}  // namespace schwa

namespace schwa {
namespace tokenizer {

static std::unique_ptr<cs::Doc>
tokenize(EncodingResult &er) {
  // Run the plain text lexer over the input to get paragraph bounds.
  fm::PlainTextLexer lexer;
  lexer.lex(er.buffer());

  // Construct the document object.
  std::unique_ptr<cs::Doc> doc(new cs::Doc());

  // Construct the tokenizer object.
  Tokenizer tokenizer;

  // For each paragraph boundary pair.
  for (const auto &pair : lexer.paragraph_indexes()) {
    // Find the newline boundaries in the paragraph.
    OffsetInputStream<> ois(pair.second - pair.first);
    ois.write(er.buffer().begin() + pair.first, er.buffer().begin() + pair.second);

    // Tokenize the paragraph.
    const size_t nsentences_before = doc->sentences.size();
    tokenizer.tokenize(ois, *doc);
    const size_t nsentences_after = doc->sentences.size();

    // Create the Paragraph and Block objects, and add them to the document.
    if (nsentences_before != nsentences_after) {
      cs::Paragraph paragraph;
      paragraph.span.start = reinterpret_cast<cs::Sentence *>(nsentences_before + 1);
      paragraph.span.stop = reinterpret_cast<cs::Sentence *>(nsentences_after + 1);
      doc->paragraphs.push_back(paragraph);

      cs::Block block;
      block.paragraph = reinterpret_cast<cs::Paragraph *>(doc->paragraphs.size() - 1 + 1);
      doc->blocks.push_back(block);
    }
  }

  // Unswizzle the document pointers and output the document.
  doc->unswizzle_pointers();
  return doc;
}

}  // namespace tokenizer
}  // namespace schwa


int
main(int argc, char **argv) {
  // Instantiate a document schema for use in the docrep config framework.
  cs::Doc::Schema schema;
  Main cfg;

    /*
  // Construct an option parser.
  cf::Main cfg("schwa-tokenizer", "Schwa Lab PTB-style tokenizer. Tokenizes plain text input into docrep or plain text formats.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> input_encoding(cfg, "input-encoding", 'e', "The encoding of the input stream", "utf-8");
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::Op<std::string> conll_path(cfg, "conll", 'c', "The conll output path", io::STDOUT_STRING);
  cf::OpChoices<std::string> printer_name(cfg, "printer", 'p', "Which printer to use as output", {"text", "debug", "docrep"}, "text");
  cf::Op<std::string> model_path(cfg, "model", 'm', "The model path");
  ner::ModelParams model_params(cfg, "model-params", "The model path path");
  cf::Op<bool> crf1_only(cfg, "crf1-only", "Whether only the 1st stage CRF tagging should happen", false);
  cf::OpSequenceTagEncoding conll_tag_encoding(cfg, "conll-tag-encoding", "Which sequence tag encoding to use for the CoNLL output", "iob1");
  dr::DocrepGroup dr(cfg, schema);
  cfg.allow_unclaimed_args("[input-path...]");
*/
  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Open the model. The validation of the model path needs to happen as early as possible.
    ner::InputModel model(cfg.model_path(), cfg.model_params);

    // Convert the input encoding string to an encoding enum value.
    const auto encoding = schwa::get_encoding(cfg.input_encoding());

    // Open the output stream.
    io::OutputStream out(cfg.output_path());

    // Work out which input paths to read from.
    std::vector<std::string> input_paths;
    if (cfg.input_path.was_mentioned() || cfg.unclaimed_args().empty())
      input_paths.push_back(cfg.input_path());
    else
      input_paths = cfg.unclaimed_args();

    if (input_paths[0] == io::STDIN_STRING) {
      // Dispatch to the main function.
        io::InputStream in(input_paths[0]);
        while (true) {
          // Read the input stream into a buffer, and re-encode it into UTF-8.
          schwa::EncodingResult er;
          {
            schwa::Buffer<> in_bytes(4 * 1024 * 1024);
            in_bytes.consume(in);
            to_utf8(encoding, in_bytes, er);
          }

          auto doc = tk::tokenize(er);
          if (doc->tokens.size() == 0) {
            continue;
          }
          // Create the feature transformer.
          if (cfg.model_params.feature_hashing.was_mentioned()) {
            ln::HasherTransform<> transformer(cfg.model_params.feature_hashing());
            ner::run_tagger(cfg, transformer, model, doc);
          }
          else {
            ln::NoTransform transformer;
            ner::run_tagger(cfg, transformer, model, doc);
          }
          std::cerr << "tagged" << std::endl;
        }
    }  
    
  })
  return 0;
}
