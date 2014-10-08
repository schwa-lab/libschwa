/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tokenizer/docrep_stream.h>

#include <iostream>


namespace schwa {
namespace tokenizer {

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


Paragraph::Schema::Schema(void) :
    dr::Ann::Schema<Paragraph>("Paragraph", "The paragraph class"),
    span(*this, "span", "the span of the paragraph", dr::FieldMode::READ_WRITE)
  { }

Paragraph::Schema::~Schema(void) { }


Doc::Schema::Schema(void) :
    dr::Doc::Schema<Doc>("Doc", "The document class"),
    tokens(*this, "tokens", "The store for the tokens", dr::FieldMode::READ_WRITE),
    sents(*this, "sentences", "The store for the sentences", dr::FieldMode::READ_WRITE),
    pars(*this, "paragraphs", "The store for the paragraphs", dr::FieldMode::READ_WRITE)
  { }

Doc::Schema::~Schema(void) { }


DocrepStream::DocrepStream(std::ostream &out, Doc::Schema &dschema, bool normalise) :
    Stream(),
    _out(out),
    _writer(out, dschema),
    _doc(nullptr),
    _normalise(normalise),
    _begin_sent(0),
    _begin_par(0)
  { }

DocrepStream::~DocrepStream(void) {
  delete _doc;
}


void
DocrepStream::add(Type, const uint8_t *raw, size_t begin, size_t len, const uint8_t *norm) {
  TmpToken t = {begin, begin + len, std::string(reinterpret_cast<const char *>(raw), len), ""};
  if (norm)
    t.norm = reinterpret_cast<const char *>(norm);
  _tokens.push_back(t);
}


void
DocrepStream::error(const uint8_t *, size_t, size_t) { }


void
DocrepStream::begin_sentence(void) {
  _begin_sent = _tokens.size();
}


void
DocrepStream::end_sentence(void) {
  const uint64_t ntokens = _tokens.size();
  if (_begin_sent != ntokens)
    _sents.push_back({_begin_sent, ntokens});
}


void
DocrepStream::begin_paragraph(void) {
  _begin_par = _sents.size();
}


void
DocrepStream::end_paragraph(void) {
  const uint64_t nsents = _sents.size();
  if (_begin_par != nsents)
    _pars.push_back({_begin_par, nsents});
}


void
DocrepStream::begin_heading(int) { }


void
DocrepStream::end_heading(int) { }


void
DocrepStream::begin_document(void) {
  if (_doc && !_tokens.empty())
    end_document();
  delete _doc;
  _doc = new Doc();
  _begin_sent = 0;
  _begin_par = 0;
}


void
DocrepStream::end_document(void) {
  // instantiate tokens
  const size_t ntokens = _tokens.size();
  _doc->tokens.create(ntokens);
  for (size_t i = 0; i != ntokens; ++i) {
    const TmpToken &tmp = _tokens[i];
    Token &t = _doc->tokens[i];
    t.span.start = tmp.start;
    t.span.stop = tmp.stop;
    t.raw = tmp.raw;
    t.norm = tmp.norm;
  }

  // instantiate sents
  const size_t nsents = _sents.size();
  _doc->sents.create(nsents);
  for (size_t i = 0; i != nsents; ++i) {
    const TmpSent &tmp = _sents[i];
    Sent &s = _doc->sents[i];
    s.span.start = &_doc->tokens[tmp.start];
    s.span.stop = &_doc->tokens[tmp.stop];
  }

  // instantiate pars
  const size_t npars = _pars.size();
  _doc->pars.create(npars);
  for (size_t i = 0; i != npars; ++i) {
    const TmpPar &tmp = _pars[i];
    Paragraph &p = _doc->pars[i];
    p.span.start = &_doc->sents[tmp.start];
    p.span.stop = &_doc->sents[tmp.stop];
  }

  _writer << *_doc;
  delete _doc;
  _doc = nullptr;
}

}  // namespace tokenizer
}  // namespace schwa
