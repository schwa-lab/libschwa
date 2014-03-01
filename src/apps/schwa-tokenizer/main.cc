/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>

#include <schwa/config.h>
#include <schwa/exception.h>
#include <schwa/dr/config.h>
#include <schwa/io/logging.h>
#include <schwa/tokenizer.h>
#include <schwa/tokenizer/debug_text_stream.h>
#include <schwa/tokenizer/docrep_stream.h>
#include <schwa/tokenizer/text_stream.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;
namespace tok = schwa::tokenizer;


int
main(int argc, char **argv) {
  // Instantiate a document schema for use in the docrep config framework.
  tok::Doc::Schema schema;

  // Construct an option parser.
  cf::Main cfg("schwa-tokenizer", "Schwa Lab tokenizer. Tokenizes a utf-8 input file, optionally accounting for markup.");
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::OpChoices<std::string> printer(cfg, "printer", 'p', "Which printer to use as output", {"text", "debug", "docrep"}, "text");
  cf::Op<size_t> input_buffer(cfg, "input-buffer", "Tokenizer input buffer size (bytes)", tok::BUFFER_SIZE);
  dr::DocrepGroup dr(cfg, schema);

  // Parse argv.
  cfg.main<io::PrettyLogger>(argc, argv);

  // Configure the tokenizer printer.
  std::unique_ptr<tok::Stream> stream;
  if (printer() == "text")
    stream.reset(new tok::TextStream(output.file()));
  else if (printer() == "debug")
    stream.reset(new tok::DebugTextStream(output.file()));
  else if (printer() == "docrep")
    stream.reset(new tok::DocrepStream(output.file(), schema));

  // Construct the tokenizer and tokenize the input.
  bool success;
  try {
    tok::Tokenizer t;
    success = t.tokenize_stream(*stream, input.file(), input_buffer());
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return success ? 0 : 1;
}
