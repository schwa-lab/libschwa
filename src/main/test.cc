/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/tokenizer.h>
#include <schwa/dr.h>

using namespace schwa;

class Token : public dr::Annotation {
public:
  dr::Slice<uint64_t> slice;
  std::string raw;
  std::string norm;

  class Schema : public dr::Annotation::Schema {
  public:
    FIELD(&Token::slice) slice;
    FIELD(&Token::raw) raw;
    FIELD(&Token::norm) norm;

    Schema(void) :
      dr::Annotation::Schema("Token", "Some help text about Token", "Token"),
      slice(*this, "slice", "some help text about slice", dr::LOAD_RO, "slice"),
      raw(*this, "raw", "some help text about raw", dr::LOAD_RW, "raw"),
      norm(*this, "norm", "some help text about norm", dr::LOAD_RW, "norm")
      { }
    virtual ~Schema(void) { }
  };
};


class Doc : public dr::Document {
public:
  std::string filename;
  dr::Store<Token> tokens;

  class Schema : public dr::Document::Schema {
  public:
    FIELD(&Doc::filename) filename;
    STORE(&Doc::tokens) tokens;

    Schema(void) :
      dr::Document::Schema(),
      filename(*this, "filename", "some help text about filename", dr::LOAD_RO, "filename"),
      tokens(*this, "tokens", "some help text about tokens store", dr::LOAD_RW, "tokens")
      { }
    virtual ~Schema(void) { }
  };
};


int
main(void) {
  dr::Slice<uint64_t> slice;
  dr::Slice<Token *> ptr_slice;
  dr::Pointer<Token> ptr;
  dr::Pointers<Token> ptrs;
  dr::Store<Token> tokens;

  Doc d;

  return 0;
}
