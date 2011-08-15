CC=/sw/bin/gcc-4
CFLAGS=-O3 -Wall -I/usr/local/include
LDFLAGS=-L/usr/local/lib -lboost_iostreams

CXX=/sw/bin/g++-4
CXXFLAGS=$(CFLAGS)

LEX=flex
LFLAGS=-Cfa -8 -p -v -s -b -B

RAGEL=ragel

OBJECTS = test_tokenizer.o tokenizer.o

test_tokenizer: $(OBJECTS) Makefile
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

tokenizer.cc: tokenizer.rl rules/*.rl Makefile
	$(RAGEL) -G2 -o $@ $<

tokenizer.o: tokenizer.cc token.h stream.h tokenizer.h state.h
test_tokenizer.o: test_tokenizer.cc *.h streams/*.h

experiment: experiment.o Makefile
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ experiment.o

experiment.cc: experiment.rl Makefile
	$(RAGEL) -G2 -o $@ $<

tok.lex: tokenizer.l
	$(LEX) $(LFLAGS) -otok.lex.c tokenizer.l
	$(CC) $(CFLAGS) -o tok.lex tok.lex.c
