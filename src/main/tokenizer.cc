#include <schwa/std.h>
#include <schwa/tokenizer.h>
#include <schwa/tokenizer/streams/debug_text.h>
#include <schwa/tokenizer/streams/text.h>

namespace st = schwa::tokenizer;

int
main(int argc, char **) {
	st::Tokenizer tokenizer;

	if (argc != 1) {
		st::DebugTextStream debug_stream(std::cout);
		tokenizer.tokenize_stream(debug_stream, std::cin, 10240);
	}
  else {
		st::TextStream text_stream(std::cout);
		tokenizer.tokenize_stream(text_stream, std::cin, 10240);
	}

	return 0;
}
