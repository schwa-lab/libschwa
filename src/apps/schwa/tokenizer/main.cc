/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <memory>

#include <schwa/config.h>
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
  cf::OpMain cfg("tokenizer", "Schwa-Lab tokenizer");
  cf::IStreamOp input(cfg, "input", "The input file");
  cf::OStreamOp output(cfg, "output", "The output file");
  cf::EnumOp<std::string> printer(cfg, "printer", "Which printer to use as output", {"text", "debug", "docrep"}, "text");
  cf::Op<size_t> input_buffer(cfg, "input_buffer", "Tokenizer input buffer size (bytes)", tok::BUFFER_SIZE);
  cf::OStreamOp log(cfg, "log", "The file to log to", cf::OStreamOp::STDERR_STRING);
  cf::LogLevelOp log_level(cfg, "log_level", "The level to log at", "info");
  dr::DocrepOpGroup dr(cfg, schema);

  // Parse argv.
  cfg.main(argc, argv);

  // Configure logging.
  io::default_logger = new io::PrettyLogger(log.file());
  io::default_logger->threshold(log_level());

  // Configure the tokenizer printer.
  std::unique_ptr<tok::Stream> stream;
  if (printer() == "text")
    stream.reset(new tok::TextStream(output.file()));
  else if (printer() == "debug")
    stream.reset(new tok::DebugTextStream(output.file()));
  else if (printer() == "docrep")
    stream.reset(new tok::DocrepStream(output.file(), schema));
  else
    throw cf::ConfigException("Unknown value", "printer", printer());

  // Construct the tokenizer and tokenize the input.
  tok::Tokenizer t;
  const bool success = t.tokenize_stream(*stream, input.file(), input_buffer());
  return success ? 0 : 1;
}
