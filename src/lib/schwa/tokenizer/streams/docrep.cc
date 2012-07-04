/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>
#include <schwa/tokenizer.h>
#include <schwa/tokenizer/streams/docrep.h>

namespace schwa { namespace tokenizer {


Token::Schema::Schema(void) :
  dr::Ann::Schema<Token>("Token", "The token class", "Token"),
  slice(*this, "slice", "the beginning and end byte offsets of the token", dr::LOAD_RW, "slice"),
  raw(*this, "raw", "the raw token", dr::LOAD_RW, "raw"),
  norm(*this, "norm", "the normalised token", dr::LOAD_RW, "norm")
  { }
Token::Schema::~Schema(void) { }


Sent::Schema::Schema(void) :
  dr::Ann::Schema<Sent>("Sent", "The sentence class", "Sent"),
  slice(*this, "slice", "the slice of the sentence", dr::LOAD_RW, "slice")
  { }
Sent::Schema::~Schema(void) { }


Doc::Schema::Schema(void) :
  dr::Doc::Schema<Doc>("Doc", "The document class"),
  tokens(*this, "tokens", "The store for the tokens", dr::LOAD_RW, "tokens"),
  sents(*this, "sents", "The store for the sentences", dr::LOAD_RW, "sents")
  { }
Doc::Schema::~Schema(void) { }



DocrepStream::DocrepStream(std::ostream &out, Doc::Schema &dschema, bool normalise) : Stream(), _out(out), _writer(out, dschema), _doc(nullptr), _normalise(normalise), _new_document(true), _new_sentence(true) { }


DocrepStream::~DocrepStream(void) {
  delete _doc;
}


void
DocrepStream::ensure_doc(void) {
  if (_new_document) {
    delete _doc;
    _doc = new Doc();
    _new_document = false;
  }
}


void
DocrepStream::add(Type, const char *raw, offset_type begin, offset_type len, const char *norm) {
  (std::cerr << "DocrepStream::add(").write(raw, len) << ", " << begin << ", " << len << ")" << std::endl;
  ensure_doc();

  // instantiate a token instance
  _doc->tokens.create(1);

  // populate the token instance
  Token &t = _doc->tokens.back();
  t.slice.start = begin;
  t.slice.stop = begin + len;
  t.raw = std::string(raw, len);
  if (norm)
    t.norm = norm;
}

void DocrepStream::error(const char *, offset_type, offset_type) { }

void DocrepStream::begin_sentence(void) { }
void DocrepStream::end_sentence(void) { }

void DocrepStream::begin_paragraph(void) { }

void DocrepStream::end_paragraph(void) { }

void DocrepStream::begin_heading(int) { }
void DocrepStream::end_heading(int) { }

void DocrepStream::begin_list(void) { }
void DocrepStream::end_list(void) { }

void DocrepStream::begin_item(void) { }
void DocrepStream::end_item(void) { }


void
DocrepStream::begin_document(void) {
  std::cerr << "DocrepStream::begin_document" << std::endl;
  _new_document = true;
}

void DocrepStream::end_document(void) {
  std::cerr << "DocrepStream::end_document" << std::endl;
  _writer << *_doc;
  delete _doc;
  _doc = nullptr;
}


} }
