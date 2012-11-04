/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>

#include <schwa/config.h>
#include <schwa/dr/config.h>
#include <schwa/tokenizer.h>
#include <schwa/tokenizer/debug_text_stream.h>
#include <schwa/tokenizer/docrep_stream.h>
#include <schwa/tokenizer/text_stream.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace tok = schwa::tokenizer;


namespace {

class Config : public cf::OpMain {
public:
  cf::IStreamOp input;
  cf::OStreamOp output;
  cf::EnumOp<std::string> printer;
  cf::Op<size_t> input_buffer;
  dr::DocrepOpGroup dr;

  Config(dr::BaseDocSchema &dschema) :
      cf::OpMain("tok", "Schwa-Lab tokenizer"),
      input(*this, "input", "input filename"),
      output(*this, "output", "output filename"),
      printer(*this, "printer", "which printer to use as output", {"text", "debug", "docrep"}, "text"),
      input_buffer(*this, "input_buffer", "input buffer size (bytes)", tok::BUFFER_SIZE),
      dr(*this, dschema)
    { }
  virtual ~Config(void) { }
};

}  // namespace


int
main(int argc, char *argv[]) {
  // instantiate a document schema for use in the config framework
  tok::Doc::Schema schema;

  // parse the command line options
  Config c(schema);
  c.main(argc, argv);

  // input and file files
  std::istream &in = c.input.file();
  std::ostream &out = c.output.file();

  // printer
  std::unique_ptr<tok::Stream> stream;
  if (c.printer() == "text")
    stream = std::unique_ptr<tok::Stream>(new tok::TextStream(out));
  else if (c.printer() == "debug")
    stream = std::unique_ptr<tok::Stream>(new tok::DebugTextStream(out));
  else if (c.printer() == "docrep")
    stream = std::unique_ptr<tok::Stream>(new tok::DocrepStream(out, schema));
  else
    throw cf::ConfigException("Unknown value", "printer", c.printer());

  tok::Tokenizer t;
  t.tokenize_stream(*stream, in, c.input_buffer());

  return 0;
}
