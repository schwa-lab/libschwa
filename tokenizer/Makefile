SCHWA = /n/schwafs/home/schwa

CC = gcc
CFLAGS = -O3 -Wall -Wextra -I$(SCHWA)/include
LDFLAGS = -L$(SCHWA)/lib -lboost_iostreams

CXX = g++
CXXFLAGS = $(CFLAGS)

LEX = flex
LFLAGS = -Cfa -8 -p -v -s -b -B

RAGEL = $(SCHWA)/bin/ragel

OBJECTS = test_tokenizer.o tokenizer.o
BINARIES = test_tokenizer experiment

.PHONY: all clean


all: test_tokenizer

clean:
	-rm -f experiment.cc tokenizer.cc
	-rm -f $(OBJECTS)
	-rm -f $(BINARIES)

test_tokenizer: $(OBJECTS) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

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
