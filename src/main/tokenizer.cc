/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/tokenizer.h>
#include <schwa/tokenizer/streams/debug_text.h>
#include <schwa/tokenizer/streams/text.h>

namespace tok = schwa::tokenizer;

int
main(int argc, char **) {
	tok::Tokenizer tokenizer;
  tok::Stream *stream;

	if (argc != 1)
		stream = new tok::DebugTextStream(std::cout);
  else
		stream = new tok::TextStream(std::cout);

  tokenizer.tokenize_stream(*stream, std::cin, 10240);

  delete stream;

	return 0;
}
