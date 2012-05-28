/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/port.h>
#include <schwa/msgpack.h>
#include <schwa/tokenizer.h>
#include <schwa/dr.h>

using namespace schwa;


class Token : public dr::Annotation {
public:
  dr::Slice<uint64_t> slice;
  std::string raw;
  std::string norm;
  dr::Pointer<Token> parent;

  class Schema;
};


class X : public dr::Annotation {
};


class Doc : public dr::Document {
public:
  std::string filename;
  dr::Store<Token> tokens;
  dr::Store<X> xs;

  class Schema : public dr::Document::Schema {
  public:
    DR_FIELD(&Doc::filename) filename;
    DR_STORE(&Doc::tokens) tokens;

    Schema(void) :
      dr::Document::Schema("Document", "Some help text about this Document class", dr::TypeInfo::create<Doc>()),
      filename(*this, "filename", "some help text about filename", dr::LOAD_RO, "filename"),
      tokens(*this, "tokens", "some help text about Token store", dr::LOAD_RW, "tokens")
      { }
    virtual ~Schema(void) { }
  };
};


class Token::Schema : public dr::Annotation::Schema {
public:
  DR_FIELD(&Token::slice) slice;
  DR_FIELD(&Token::raw) raw;
  DR_FIELD(&Token::norm) norm;
  DR_FIELD2(&Token::parent, &Doc::tokens) parent;

  Schema(void) :
    dr::Annotation::Schema("Token", "Some help text about Token", "Token", dr::TypeInfo::create<Token>()),
    slice(*this, "slice", "some help text about slice", dr::LOAD_RO, "slice"),
    raw(*this, "raw", "some help text about raw", dr::LOAD_RW, "raw"),
    norm(*this, "norm", "some help text about norm", dr::LOAD_RW, "norm"),
    parent(*this, "parent", "some help text about parent", dr::LOAD_RW, "parent")
    { }
  virtual ~Schema(void) { }
};


int
main(void) {
  dr::Slice<uint64_t> slice;
  dr::Slice<Token *> ptr_slice;
  dr::Pointer<Token> ptr;
  dr::Pointers<Token> ptrs;
  dr::Store<Token> tokens;

  dr::TypeRegistry reg = dr::TypeRegistry::create<Doc>();
  dr::Schema &s_tok = reg.add<Token>();
  (void)s_tok;

  try {
    s_tok.set_serial("foo");
  }
  catch (schwa::Exception &e) {
    std::cerr << port::RED << port::BOLD << "schwa::Exception: " << e.what() << port::OFF << std::endl;
    return 1;
  }

  Doc d;
  Token &t1 = d.tokens.create();
  Token &t2 = d.tokens.create();
  t1.raw = "hello";
  t2.raw = "world";
  t2.parent = &t1;

  dr::Writer writer(std::cout, reg);
  writer << d;

  return 0;
}
