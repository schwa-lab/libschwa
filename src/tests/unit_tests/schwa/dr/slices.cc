/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "test_utils.h"

#include <schwa/dr.h>

namespace dr = schwa::dr;


namespace schwatest {

SUITE(schwa__dr__slices) {

TEST(slices) {
  class Token : public dr::Ann {
  public:
    dr::Slice<uint64_t> span;
    std::string raw;

    class Schema;
  };


  class Sent : public dr::Ann {
  public:
    dr::Slice<Token *> span;
    uint32_t number;

    class Schema;

    Sent(void) : dr::Ann(), number(0) { }
  };


  class Doc : public dr::Doc {
  public:
    dr::Store<Token> tokens;
    dr::Store<Sent> sents;

    class Schema;
  };


  class Token::Schema : public dr::Ann::Schema<Token> {
  public:
    DR_FIELD(&Token::span) span;
    DR_FIELD(&Token::raw) raw;

    Schema(void) :
      dr::Ann::Schema<Token>("Token", "Some text about Token"),
      span(*this, "span", "span", dr::FieldMode::READ_WRITE),
      raw(*this, "raw", "raw", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class Sent::Schema : public dr::Ann::Schema<Sent> {
  public:
    DR_POINTER(&Sent::span, &Doc::tokens) span;
    DR_FIELD(&Sent::number) number;

    Schema(void) :
      dr::Ann::Schema<Sent>("Sent", "Some text about Sent"),
      span(*this, "span", "span", dr::FieldMode::READ_WRITE),
      number(*this, "number", "number", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class Doc::Schema : public dr::Doc::Schema<Doc> {
  public:
    DR_STORE(&Doc::tokens) tokens;
    DR_STORE(&Doc::sents) sents;

    Schema(void) :
      dr::Doc::Schema<Doc>("Doc", "Some text about Doc"),
      tokens(*this, "tokens", "some text about tokens", dr::FieldMode::READ_WRITE),
      sents(*this, "sents", "some text about sents", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  Doc doc0;

  doc0.tokens.create(10);
  doc0.tokens[0].span = dr::Slice<uint64_t>(0, 3);
  doc0.tokens[0].raw = "The";
  doc0.tokens[1].span = dr::Slice<uint64_t>(4, 9);
  doc0.tokens[1].raw = "quick";
  doc0.tokens[2].span = dr::Slice<uint64_t>(11, 16);
  doc0.tokens[2].raw = "brown";
  doc0.tokens[3].span = dr::Slice<uint64_t>(17, 20);
  doc0.tokens[3].raw = "fox";
  doc0.tokens[4].span = dr::Slice<uint64_t>(20, 21);
  doc0.tokens[4].raw = ".";
  doc0.tokens[5].span = dr::Slice<uint64_t>(22, 25);
  doc0.tokens[5].raw = "The";
  doc0.tokens[6].span = dr::Slice<uint64_t>(26, 30);
  doc0.tokens[6].raw = "lazy";
  doc0.tokens[7].span = dr::Slice<uint64_t>(31, 34);
  doc0.tokens[7].raw = "cat";
  doc0.tokens[8].span = dr::Slice<uint64_t>(35, 38);
  doc0.tokens[8].raw = "too";
  doc0.tokens[9].span = dr::Slice<uint64_t>(38, 39);
  doc0.tokens[9].raw = ".";

  doc0.sents.create(2);
  doc0.sents[0].span = dr::Slice<Token *>(&doc0.tokens[0], &doc0.tokens[5]);
  doc0.sents[1].span = dr::Slice<Token *>(&doc0.tokens[5], &doc0.tokens[10]);

  std::stringstream stream;
  Doc::Schema schema;
  dr::Writer writer(stream, schema);
  writer << doc0;

  static constexpr unsigned char correct[] = "\x02"
  "\x93"
    "\x92"
      "\xa8" "__meta__"
      "\x90"
    "\x92"
      "\xa5" "Token"
      "\x92"
        "\x82\x00\xa4" "span" "\x02\xc0"
        "\x81\x00\xa3" "raw"
    "\x92"
      "\xa4Sent"
      "\x92"
        "\x83\x00\xa4" "span" "\x01\x00\x02\xc0"
        "\x81\x00\xa6" "number"
  "\x92"
    "\x93\xa6" "tokens" "\x01\x0a"
    "\x93\xa5" "sents" "\x02\x02"
  "\x01"
    "\x80"
  "\x66"
    "\x9a"
      "\x82\x00\x92\x00\x03\x01\xa3" "The"
      "\x82\x00\x92\x04\x05\x01\xa5" "quick"
      "\x82\x00\x92\x0b\x05\x01\xa5" "brown"
      "\x82\x00\x92\x11\x03\x01\xa3" "fox"
      "\x82\x00\x92\x14\x01\x01\xa1" "."
      "\x82\x00\x92\x16\x03\x01\xa3" "The"
      "\x82\x00\x92\x1a\x04\x01\xa4" "lazy"
      "\x82\x00\x92\x1f\x03\x01\xa3" "cat"
      "\x82\x00\x92\x23\x03\x01\xa3" "too"
      "\x82\x00\x92\x26\x01\x01\xa1" "."
  "\x0f"
    "\x92"
      "\x82\x00\x92\x00\x05\x01\x00"
      "\x82\x00\x92\x05\x05\x01\x00";

  CHECK_COMPARE_BYTES3(correct, sizeof(correct)/sizeof(unsigned char) - 1, stream.str());

  Doc doc1;
  dr::Reader reader(stream, schema);
  reader >> doc1;

  CHECK_EQUAL(10, doc1.tokens.size());
  CHECK_EQUAL(2, doc1.sents.size());

  CHECK_EQUAL("The", doc1.tokens[0].raw);
  CHECK(doc1.tokens[0].span == dr::Slice<uint64_t>(0, 3));
  CHECK_EQUAL("quick", doc1.tokens[1].raw);
  CHECK(doc1.tokens[1].span == dr::Slice<uint64_t>(4, 9));
  CHECK_EQUAL("brown", doc1.tokens[2].raw);
  CHECK(doc1.tokens[2].span == dr::Slice<uint64_t>(11, 16));
  CHECK_EQUAL("fox", doc1.tokens[3].raw);
  CHECK(doc1.tokens[3].span == dr::Slice<uint64_t>(17, 20));
  CHECK_EQUAL(".", doc1.tokens[4].raw);
  CHECK(doc1.tokens[4].span == dr::Slice<uint64_t>(20, 21));
  CHECK_EQUAL("The", doc1.tokens[5].raw);
  CHECK(doc1.tokens[5].span == dr::Slice<uint64_t>(22, 25));
  CHECK_EQUAL("lazy", doc1.tokens[6].raw);
  CHECK(doc1.tokens[6].span == dr::Slice<uint64_t>(26, 30));
  CHECK_EQUAL("cat", doc1.tokens[7].raw);
  CHECK(doc1.tokens[7].span == dr::Slice<uint64_t>(31, 34));
  CHECK_EQUAL("too", doc1.tokens[8].raw);
  CHECK(doc1.tokens[8].span == dr::Slice<uint64_t>(35, 38));
  CHECK_EQUAL(".", doc1.tokens[9].raw);
  CHECK(doc1.tokens[9].span == dr::Slice<uint64_t>(38, 39));

  CHECK_EQUAL(&doc1.tokens[0], doc1.sents[0].span.start);
  CHECK_EQUAL(&doc1.tokens[5], doc1.sents[0].span.stop);
  CHECK_EQUAL(0, doc1.sents[0].number);
  CHECK_EQUAL(&doc1.tokens[5], doc1.sents[1].span.start);
  CHECK_EQUAL(&doc1.tokens[10], doc1.sents[1].span.stop);
  CHECK_EQUAL(0, doc1.sents[1].number);
}

}  // SUITE

}  // namespace schwatest
