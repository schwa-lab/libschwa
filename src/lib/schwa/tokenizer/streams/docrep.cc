/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>
#include <schwa/tokenizer.h>
#include <schwa/tokenizer/streams/docrep.h>

namespace schwa { namespace tokenizer {


Token::Schema::Schema(void) :
  dr::Ann::Schema<Token>("Token", "The token class"),
  span(*this, "span", "the beginning and end byte offsets of the token", dr::FieldMode::READ_WRITE),
  raw(*this, "raw", "the raw token", dr::FieldMode::READ_WRITE),
  norm(*this, "norm", "the normalised token", dr::FieldMode::READ_WRITE)
  { }
Token::Schema::~Schema(void) { }


Sent::Schema::Schema(void) :
  dr::Ann::Schema<Sent>("Sentence", "The sentence class"),
  span(*this, "span", "the span of the sentence", dr::FieldMode::READ_WRITE)
  { }
Sent::Schema::~Schema(void) { }


Doc::Schema::Schema(void) :
  dr::Doc::Schema<Doc>("Doc", "The document class"),
  tokens(*this, "tokens", "The store for the tokens", dr::FieldMode::READ_WRITE),
  sents(*this, "sentences", "The store for the sentences", dr::FieldMode::READ_WRITE)
  { }
Doc::Schema::~Schema(void) { }



DocrepStream::DocrepStream(std::ostream &out, Doc::Schema &dschema, bool normalise) : Stream(), _out(out), _writer(out, dschema), _doc(nullptr), _normalise(normalise), _new_document(true), _ntokens(0), _begin_sent(0) { }


DocrepStream::~DocrepStream(void) {
  delete _doc;
}


void
DocrepStream::ensure_doc(void) {
  if (_new_document) {
    delete _doc;
    _doc = new Doc();
    _new_document = false;
    _ntokens = 0;
    _begin_sent = 0;
  }
}


void
DocrepStream::add(Type, const char *raw, offset_type begin, offset_type len, const char *norm) {
  //(std::cerr << "DocrepStream::add(").write(raw, len) << ", " << begin << ", " << len << ")" << std::endl;
  ensure_doc();

  // instantiate a token instance
  _doc->tokens.create(1);

  // populate the token instance
  Token &t = _doc->tokens.back();
  t.span.start = begin;
  t.span.stop = begin + len;
  t.raw = std::string(raw, len);
  if (norm)
    t.norm = norm;

  ++_ntokens;
}

void DocrepStream::error(const char *, offset_type, offset_type) { }

void DocrepStream::begin_sentence(void) {
  _begin_sent = _ntokens;
}
void DocrepStream::end_sentence(void) {
  //std::cerr << "DocrepStream::end_sentence(begin=" << _begin_sent << ", ntokens=" << _ntokens << ")" << std::endl;
  if (_begin_sent != _ntokens) {
    _doc->sents.create(1);
    Sent &s = _doc->sents.back();
    s.span.start = reinterpret_cast<Token *>(_begin_sent);
    s.span.stop  = reinterpret_cast<Token *>(_ntokens);
  }
}

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
  //std::cerr << "DocrepStream::begin_document" << std::endl;
  _new_document = true;
}

void DocrepStream::end_document(void) {
  //std::cerr << "DocrepStream::end_document" << std::endl;

  // correct the sentence pointers
  auto &tokens = _doc->tokens;
  for (auto &s : _doc->sents) {
    s.span.start = &tokens[reinterpret_cast<size_t>(s.span.start)];
    s.span.stop  = &tokens[reinterpret_cast<size_t>(s.span.stop)];
  }

  _writer << *_doc;
  delete _doc;
  _doc = nullptr;
}


} }
