/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>
#include <schwa/tokenizer.h>
#include <schwa/tokenizer/streams/debug_text.h>
#include <schwa/tokenizer/streams/text.h>

namespace cfg = schwa::config;
namespace tok = schwa::tokenizer;

class Config : public cfg::OpGroup {
public:
  cfg::Op<std::string> input;
  cfg::Op<std::string> output;
  cfg::Op<std::string> printer;
  cfg::Op<size_t> input_buffer;

  Config(void) :
    cfg::OpGroup("tok", "Schwa-Lab tokenizer"),
    input(*this, "input", "input filename", ""),
    output(*this, "output", "output filename", ""),
    printer(*this, "printer", "which printer to use as output {text, debug, docrep}", "text"),
    input_buffer(*this, "input_buffer", "input buffer size (bytes)", tok::BUFFER_SIZE)
    { }
  virtual ~Config(void) { }
};


int
main(int argc, char *argv[]) {
  Config c;
  try {
    if (!c.process(argc - 1, argv + 1))
      return 1;
  }
  catch (cfg::ConfigException &e) {
    std::cerr << schwa::print_exception("ConfigException", e);
    c.help(std::cerr);
    return 1;
  }

  // input file
  std::istream *in;
  if (c.input().empty())
    in = &std::cin;
  else
    in = new std::ifstream(c.input().c_str());

  // output file
  std::ostream *out;
  if (c.output().empty())
    out = &std::cout;
  else
    out = new std::ofstream(c.output().c_str());

  // printer
  tok::Stream *stream;
	if (c.printer() == "text")
		stream = new tok::TextStream(*out);
  else if (c.printer() == "debug")
		stream = new tok::DebugTextStream(*out);
  else if (c.printer() == "docrep")
    throw cfg::ConfigException("Unhandled value", "printer", c.printer());
  else
    throw cfg::ConfigException("Unknown value", "printer", c.printer());

	tok::Tokenizer t;
  t.tokenize_stream(*stream, *in, c.input_buffer());

  // cleanup
  delete stream;
  if (!c.output().empty())
    delete out;
  if (!c.input().empty())
    delete in;

	return 0;
}
