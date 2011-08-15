
#include <stdint.h>
#include <string>
#include <iostream>

#include "source.h"

#include "token.h"
#include "stream.h"
#include "tokenizer.h"

#include "streams/text.h"
#include "streams/debug_text.h"

using namespace std;
using namespace schwa;

int
main(int argc, char **argv){
	token::Tokenizer tokenizer;

	if(argc != 1){
		token::DebugTextStream debug_stream(cout);
		tokenizer.tokenize_stream(debug_stream, cin, 10240);
	}else{
		token::TextStream text_stream(cout);
		tokenizer.tokenize_stream(text_stream, cin, 10240);
	}

	return 0;
}
