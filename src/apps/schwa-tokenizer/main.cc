/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>
#include <vector>

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
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::OpChoices<std::string> printer(cfg, "printer", 'p', "Which printer to use as output", {"text", "debug", "docrep"}, "text");
  cf::Op<size_t> input_buffer(cfg, "input-buffer", "Tokenizer input buffer size (bytes)", tok::DEFAULT_BUFFER_SIZE);
  dr::DocrepGroup dr(cfg, schema);

  cfg.allow_unclaimed_args("[input-path...]");

  bool success = true;
  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Work out which input paths to read from.
    std::vector<std::string> input_paths;
    if (input_path.was_mentioned() || cfg.unclaimed_args().empty())
      input_paths.push_back(input_path());
    else
      input_paths = cfg.unclaimed_args();

    // Open the ouptut stream.
    io::OutputStream out(output_path());

    // Configure the tokenizer printer.
    std::unique_ptr<tok::Stream> stream;
    if (printer() == "text")
      stream.reset(new tok::TextStream(out));
    else if (printer() == "debug")
      stream.reset(new tok::DebugTextStream(out));
    else if (printer() == "docrep")
      stream.reset(new tok::DocrepStream(out, schema));

    // Construct the tokenizer and tokenize the input.
    tok::Tokenizer t;
    for (const std::string &input_path : input_paths) {
      io::InputStream in(input_path);
      success = t.tokenize_stream(*stream, in, input_buffer());
      if (!success)
        break;
    }
  })
  return success ? 0 : 1;
}
