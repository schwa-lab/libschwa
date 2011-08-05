
#include <stdint.h>
#include <string>
#include <iostream>

#include "tokens.h"
#include "stream.h"
#include "tokenizer.h"

#include "streams/text.h"
#include "streams/debug_text.h"

using namespace std;
using namespace schwa;

int
main(int argc, char **argv){
	tokens::Tokenizer tokenizer;

	if(argc != 1){
		tokens::DebugTextStream debug_stream(cout);
		tokenizer.tokenize(debug_stream, cin, 10240);
	}else{
		tokens::TextStream text_stream(cout);
		tokenizer.tokenize(text_stream, cin, 10240);
	}

	return 0;
}
