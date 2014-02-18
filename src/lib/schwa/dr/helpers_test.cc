/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <sstream>
#include <vector>

#include <schwa/dr.h>


namespace schwa {
namespace dr {

namespace {

class Sent;

class X { };

class Token : public Ann {
public:
  std::string raw;
  X *x;
  Sent *sent;
  std::vector<Sent *> sents;

  explicit Token(void) : Ann(), sent(nullptr) { }
  Token(const char *lazy, const uint32_t nelem, const uint32_t nbytes) : Ann(lazy, nelem, nbytes), sent(nullptr) { }
  Token(const Token &o) : Ann(o), raw(o.raw), sent(o.sent), sents(o.sents) { }
  Token(const Token &&o) : Ann(o), raw(o.raw), sent(o.sent), sents(o.sents) { }
  ~Token(void) { }

  Token &operator =(const Token &) { return *this; }

  class Schema;
};


class Sent : public Ann {
public:
  Slice<Token *> span;

  class Schema;
};


class TestDoc : public Doc {
public:
  Store<Token> tokens;
  Store<Sent> sents;

  class Schema;
};


class Token::Schema : public Ann::Schema<Token> {
public:
  DR_FIELD(&Token::raw) raw;

  Schema(void) :
      Ann::Schema<Token>("Token", "The token class"),
      raw(*this, "raw", "the raw token", FieldMode::READ_WRITE)
    { }
  virtual ~Schema(void) { }
};


class Sent::Schema : public Ann::Schema<Sent> {
public:
  DR_POINTER(&Sent::span, &TestDoc::tokens) span;

  Schema(void) :
      Ann::Schema<Sent>("Sentence", "The sentence class"),
      span(*this, "span", "the span of the sentence", FieldMode::READ_WRITE)
    { }
  virtual ~Schema(void) { }
};


class TestDoc::Schema : public Doc::Schema<TestDoc> {
public:
  DR_STORE(&TestDoc::tokens) tokens;
  DR_STORE(&TestDoc::sents) sents;

  Schema(void) :
      Doc::Schema<TestDoc>("TestDoc", "The document class"),
      tokens(*this, "tokens", "The store for the tokens", FieldMode::READ_WRITE),
      sents(*this, "sentences", "The store for the sentences", FieldMode::READ_WRITE)
    { }
  virtual ~Schema(void) { }
};

}


SUITE(schwa__dr__helpers) {

TEST(test_reverse_slices_mutex) {
  TestDoc doc;
  TestDoc::Schema schema;

  doc.tokens.create(8);
  doc.tokens[0].raw = "a";
  doc.tokens[1].raw = "b";
  doc.tokens[2].raw = "c";
  doc.tokens[3].raw = "There";
  doc.tokens[4].raw = "is";
  doc.tokens[5].raw = "a";
  doc.tokens[6].raw = "dog";
  doc.tokens[7].raw = ".";
  doc.sents.create(2);
  doc.sents[0].span.start = &doc.tokens[0];
  doc.sents[0].span.stop = &doc.tokens[3];
  doc.sents[1].span.start = &doc.tokens[3];
  doc.sents[1].span.stop = &doc.tokens[8];

  std::stringstream ss;
  Writer writer(ss, schema);
  writer << doc;
  ss.seekg(0);
  CHECK(!ss.str().empty());

  for (Token &t : doc.tokens)
    CHECK(t.sent == nullptr);

  DR_REVERSE_SLICES(&TestDoc::sents, &TestDoc::tokens, &Sent::span, &Token::sent)(doc);

  for (Token &t : doc.tokens)
    CHECK(t.sent != nullptr);

  CHECK(doc.tokens[0].sent == &doc.sents[0]);
  CHECK(doc.tokens[1].sent == &doc.sents[0]);
  CHECK(doc.tokens[2].sent == &doc.sents[0]);
  CHECK(doc.tokens[3].sent == &doc.sents[1]);
  CHECK(doc.tokens[4].sent == &doc.sents[1]);
  CHECK(doc.tokens[5].sent == &doc.sents[1]);
  CHECK(doc.tokens[6].sent == &doc.sents[1]);
  CHECK(doc.tokens[7].sent == &doc.sents[1]);
}

TEST(test_reverse_slices) {
  TestDoc doc;
  TestDoc::Schema schema;

  doc.tokens.create(8);
  doc.tokens[0].raw = "a";
  doc.tokens[1].raw = "b";
  doc.tokens[2].raw = "c";
  doc.tokens[3].raw = "d";
  doc.tokens[4].raw = "e";
  doc.tokens[5].raw = "f";
  doc.tokens[6].raw = "g";
  doc.tokens[7].raw = "再见";
  doc.sents.create(3);
  doc.sents[0].span.start = &doc.tokens[0];
  doc.sents[0].span.stop = &doc.tokens[2];
  doc.sents[1].span.start = &doc.tokens[2];
  doc.sents[1].span.stop = &doc.tokens[6];
  doc.sents[2].span.start = &doc.tokens[1];
  doc.sents[2].span.stop = &doc.tokens[5];

  std::stringstream ss;
  Writer writer(ss, schema);
  writer << doc;
  ss.seekg(0);
  CHECK(!ss.str().empty());

  for (Token &t : doc.tokens)
    CHECK(t.sents.empty());

  DR_REVERSE_SLICES(&TestDoc::sents, &TestDoc::tokens, &Sent::span, &Token::sents)(doc);

  CHECK_EQUAL(1, doc.tokens[0].sents.size());
  CHECK_EQUAL(2, doc.tokens[1].sents.size());
  CHECK_EQUAL(2, doc.tokens[2].sents.size());
  CHECK_EQUAL(2, doc.tokens[3].sents.size());
  CHECK_EQUAL(2, doc.tokens[4].sents.size());
  CHECK_EQUAL(1, doc.tokens[5].sents.size());
  CHECK_EQUAL(0, doc.tokens[6].sents.size());
  CHECK_EQUAL(0, doc.tokens[7].sents.size());
  CHECK_EQUAL(&doc.sents[0], doc.tokens[0].sents[0]);
  CHECK_EQUAL(&doc.sents[0], doc.tokens[1].sents[0]);
  CHECK_EQUAL(&doc.sents[2], doc.tokens[1].sents[1]);
  CHECK_EQUAL(&doc.sents[1], doc.tokens[2].sents[0]);
  CHECK_EQUAL(&doc.sents[2], doc.tokens[2].sents[1]);
  CHECK_EQUAL(&doc.sents[1], doc.tokens[3].sents[0]);
  CHECK_EQUAL(&doc.sents[2], doc.tokens[3].sents[1]);
  CHECK_EQUAL(&doc.sents[1], doc.tokens[4].sents[0]);
  CHECK_EQUAL(&doc.sents[2], doc.tokens[4].sents[1]);
  CHECK_EQUAL(&doc.sents[1], doc.tokens[5].sents[0]);
}

}  // SUITE

}  // namespace dr
}  // namespace schwa
