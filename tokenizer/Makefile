SCHWA = /n/schwafs/home/schwa

CC = gcc
CFLAGS = -O3 -Wall -Wextra -I$(SCHWA)/include
LDFLAGS = -L$(SCHWA)/lib -lboost_iostreams

CXX = g++
CXXFLAGS = $(CFLAGS)

RAGEL = $(SCHWA)/bin/ragel

OBJECTS = test_tokenizer.o tokenizer.o
BINARIES = test_tokenizer experiment

.PHONY: all clean


all: test_tokenizer

clean:
	-rm -f tokenizer.cc
	-rm -f $(OBJECTS)
	-rm -f $(BINARIES)

test_tokenizer: $(OBJECTS) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

tokenizer.cc: tokenizer.rl rules/*.rl Makefile
	$(RAGEL) -G2 -o $@ $<

tokenizer.o: tokenizer.cc token.h stream.h tokenizer.h state.h
test_tokenizer.o: test_tokenizer.cc *.h streams/*.h
