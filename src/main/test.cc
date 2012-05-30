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

  class Schema;
};


class Token::Schema : public dr::AnnotationSchema<Token> {
public:
  DR_FIELD(&Token::slice) slice;
  DR_FIELD(&Token::raw) raw;
  DR_FIELD(&Token::norm) norm;
  DR_POINTER(&Token::parent, &Doc::tokens) parent;

  Schema(void) :
    dr::AnnotationSchema<Token>("Token", "Some help text about Token", "Token"),
    slice(*this, "slice", "some help text about slice", dr::LOAD_RO, "slice"),
    raw(*this, "raw", "some help text about raw", dr::LOAD_RW, "raw"),
    norm(*this, "norm", "some help text about norm", dr::LOAD_RW, "norm"),
    parent(*this, "parent", "some help text about parent", dr::LOAD_RW, "parent")
    { }
  virtual ~Schema(void) { }
};


class Doc::Schema : public dr::DocumentSchema<Doc> {
public:
  DR_FIELD(&Doc::filename) filename;
  DR_STORE(&Doc::tokens) tokens;

  Schema(void) :
    dr::DocumentSchema<Doc>("Document", "Some help text about this Document class"),
    filename(*this, "filename", "some help text about filename", dr::LOAD_RO, "filename"),
    tokens(*this, "tokens", "some help text about Token store", dr::LOAD_RW, "tokens")
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

  Doc::Schema schema;
  schema.filename.serial = "foo";
  schema.types<Token>().serial = "PTBToken";
  schema.types<Token>().raw.serial = "real_raw";

  Doc d;
  d.tokens.create(2);
  Token &t1 = d.tokens[0];
  Token &t2 = d.tokens[1];
  t1.raw = "hello";
  t2.raw = "world";
  t2.parent = &t1;

  dr::Writer writer(std::cout, schema);
  writer << d;

  return 0;
}
