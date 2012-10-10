/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>
#include "test_utils.h"

#include <boost/test/unit_test.hpp>

namespace dr = schwa::dr;

// ============================================================================
// ============================================================================
BOOST_AUTO_TEST_SUITE(schwa__dr__pointers)

BOOST_AUTO_TEST_CASE(self_pointer0) {
  class Token : public dr::Ann {
  public:
    dr::Slice<uint64_t> span;
    std::string raw;

    class Schema;
  };


  class X : public dr::Ann {
  public:
    dr::Pointers<Token> tokens;
    dr::Pointers<X> prior;

    class Schema;
  };


  class Doc : public dr::Doc {
  public:
    dr::Store<Token> tokens;
    dr::Store<X> xs;

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


  class X::Schema : public dr::Ann::Schema<X> {
  public:
    DR_POINTER(&X::tokens, &Doc::tokens) tokens;
    DR_SELF(&X::prior) prior;

    Schema(void) :
      dr::Ann::Schema<X>("X", "Some text about X"),
      tokens(*this, "tokens", "tokens", dr::FieldMode::READ_WRITE),
      prior(*this, "prior", "prior", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class Doc::Schema : public dr::Doc::Schema<Doc> {
  public:
    DR_STORE(&Doc::tokens) tokens;
    DR_STORE(&Doc::xs) xs;

    Schema(void) :
      dr::Doc::Schema<Doc>("Doc", "Some text about Doc"),
      tokens(*this, "tokens", "some text about tokens", dr::FieldMode::READ_WRITE),
      xs(*this, "xs", "some text about xs", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  Doc doc0;

  doc0.tokens.create(10);
  for (size_t i = 0; i != 10; ++i) {
    doc0.tokens[i].span = dr::Slice<uint64_t>(i, i + 1);
    doc0.tokens[i].raw += i + 'a';
  }

  doc0.xs.create(5);
  doc0.xs[0].tokens.items.push_back(&doc0.tokens[0]);
  doc0.xs[1].tokens.items.push_back(&doc0.tokens[0]);
  doc0.xs[1].tokens.items.push_back(&doc0.tokens[1]);
  doc0.xs[1].tokens.items.push_back(&doc0.tokens[2]);
  doc0.xs[2].tokens.items.push_back(&doc0.tokens[0]);
  doc0.xs[2].tokens.items.push_back(&doc0.tokens[1]);
  doc0.xs[2].tokens.items.push_back(&doc0.tokens[2]);
  doc0.xs[2].tokens.items.push_back(&doc0.tokens[3]);
  doc0.xs[2].tokens.items.push_back(&doc0.tokens[4]);
  doc0.xs[4].tokens.items.push_back(&doc0.tokens[9]);
  for (size_t i = 0; i != 5; ++i)
    for (size_t j = 0; j <= i; ++j)
      doc0.xs[i].prior.items.push_back(&doc0.xs[j]);

  std::stringstream stream, correct;
  Doc::Schema schema;
  dr::Writer writer(stream, schema);
  writer << doc0;

  correct << '\x02';
  correct << '\x93';  // <klasses>: 3-element array

  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "__meta__";  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
  correct << '\x90';  // <fields>: 0-element array

  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa5' << "Token";  // <klass_name>
  correct << '\x92';  // <fields>: 2-element array
    correct << '\x82';  // <field>: 2-element map
      correct << '\x00' << '\xa4' << "span";
      correct << '\x02' << '\xc0';
    correct << '\x81';  // <field>: 2-element map
      correct << '\x00' << '\xa3' << "raw";

  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa1' << "X";  // <klass_name>
  correct << '\x92';  // <fields>: 1-element array
    correct << '\x83';  // <field>: 3-element map
      correct << '\x00' << '\xa6' << "tokens";
      correct << '\x01' << '\x00';
      correct << '\x04' << '\xc0';
    correct << '\x83';  // <field>: 3-element map
      correct << '\x00' << '\xa5' << "prior";
      correct << '\x03' << '\xc0';
      correct << '\x04' << '\xc0';

  correct << '\x92';  // <stores>: 2-element array
    correct << '\x93' << '\xa6' << "tokens" << '\x01' << '\x0a';
    correct << '\x93' << '\xa2' << "xs" << '\x02' << '\x05';

  correct << '\x01';  // <instance_nbytes>
  correct << '\x80';  // <instance>

  correct << '\x51';  // <instances_nbytes>
  correct << '\x9a';  // <instances>
  correct << '\x82' << '\x00' << '\x92' << '\x00' << '\x01' << '\x01' << '\xa1' << "a";
  correct << '\x82' << '\x00' << '\x92' << '\x01' << '\x01' << '\x01' << '\xa1' << "b";
  correct << '\x82' << '\x00' << '\x92' << '\x02' << '\x01' << '\x01' << '\xa1' << "c";
  correct << '\x82' << '\x00' << '\x92' << '\x03' << '\x01' << '\x01' << '\xa1' << "d";
  correct << '\x82' << '\x00' << '\x92' << '\x04' << '\x01' << '\x01' << '\xa1' << "e";
  correct << '\x82' << '\x00' << '\x92' << '\x05' << '\x01' << '\x01' << '\xa1' << "f";
  correct << '\x82' << '\x00' << '\x92' << '\x06' << '\x01' << '\x01' << '\xa1' << "g";
  correct << '\x82' << '\x00' << '\x92' << '\x07' << '\x01' << '\x01' << '\xa1' << "h";
  correct << '\x82' << '\x00' << '\x92' << '\x08' << '\x01' << '\x01' << '\xa1' << "i";
  correct << '\x82' << '\x00' << '\x92' << '\x09' << '\x01' << '\x01' << '\xa1' << "j";

  correct << '\x31';  // <instances_nbytes>
  correct << '\x95';  // <instances>
  correct << '\x82'
    << '\x00' << '\x91' << '\x00'
    << '\x01' << '\x91' << '\x00';
  correct << '\x82'
    << '\x00' << '\x93' << '\x00' << '\x01' << '\x02'
    << '\x01' << '\x92' << '\x00' << '\x01';
  correct << '\x82'
    << '\x00' << '\x95' << '\x00' << '\x01' << '\x02' << '\x03' << '\x04'
    << '\x01' << '\x93' << '\x00' << '\x01' << '\x02';
  correct << '\x81'
    << '\x01' << '\x94' << '\x00' << '\x01' << '\x02' << '\x03';
  correct << '\x82'
    << '\x00' << '\x91' << '\x09'
    << '\x01' << '\x95' << '\x00' << '\x01' << '\x02' << '\x03' << '\x04';

  BOOST_CHECK( compare_bytes(stream.str(), correct.str()) );

  Doc doc1;
  dr::Reader reader(stream, schema);
  reader >> doc1;

  BOOST_CHECK_EQUAL(doc1.tokens.size(), 10);
  BOOST_CHECK_EQUAL(doc1.xs.size(), 5);

  BOOST_CHECK_EQUAL(doc1.tokens[0].raw, "a");
  BOOST_CHECK(doc1.tokens[0].span == dr::Slice<uint64_t>(0, 1));
  BOOST_CHECK_EQUAL(doc1.tokens[1].raw, "b");
  BOOST_CHECK(doc1.tokens[1].span == dr::Slice<uint64_t>(1, 2));
  BOOST_CHECK_EQUAL(doc1.tokens[2].raw, "c");
  BOOST_CHECK(doc1.tokens[2].span == dr::Slice<uint64_t>(2, 3));
  BOOST_CHECK_EQUAL(doc1.tokens[3].raw, "d");
  BOOST_CHECK(doc1.tokens[3].span == dr::Slice<uint64_t>(3, 4));
  BOOST_CHECK_EQUAL(doc1.tokens[4].raw, "e");
  BOOST_CHECK(doc1.tokens[4].span == dr::Slice<uint64_t>(4, 5));
  BOOST_CHECK_EQUAL(doc1.tokens[5].raw, "f");
  BOOST_CHECK(doc1.tokens[5].span == dr::Slice<uint64_t>(5, 6));
  BOOST_CHECK_EQUAL(doc1.tokens[6].raw, "g");
  BOOST_CHECK(doc1.tokens[6].span == dr::Slice<uint64_t>(6, 7));
  BOOST_CHECK_EQUAL(doc1.tokens[7].raw, "h");
  BOOST_CHECK(doc1.tokens[7].span == dr::Slice<uint64_t>(7, 8));
  BOOST_CHECK_EQUAL(doc1.tokens[8].raw, "i");
  BOOST_CHECK(doc1.tokens[8].span == dr::Slice<uint64_t>(8, 9));
  BOOST_CHECK_EQUAL(doc1.tokens[9].raw, "j");
  BOOST_CHECK(doc1.tokens[9].span == dr::Slice<uint64_t>(9, 10));

  BOOST_CHECK_EQUAL(doc1.xs[0].tokens.items.size(), 1);
  BOOST_CHECK_EQUAL(doc1.xs[1].tokens.items.size(), 3);
  BOOST_CHECK_EQUAL(doc1.xs[2].tokens.items.size(), 5);
  BOOST_CHECK_EQUAL(doc1.xs[3].tokens.items.size(), 0);
  BOOST_CHECK_EQUAL(doc1.xs[4].tokens.items.size(), 1);

  BOOST_CHECK_EQUAL(doc1.xs[0].tokens.items[0], &doc1.tokens[0]);
  BOOST_CHECK_EQUAL(doc1.xs[1].tokens.items[0], &doc1.tokens[0]);
  BOOST_CHECK_EQUAL(doc1.xs[1].tokens.items[1], &doc1.tokens[1]);
  BOOST_CHECK_EQUAL(doc1.xs[1].tokens.items[2], &doc1.tokens[2]);
  BOOST_CHECK_EQUAL(doc1.xs[2].tokens.items[0], &doc1.tokens[0]);
  BOOST_CHECK_EQUAL(doc1.xs[2].tokens.items[1], &doc1.tokens[1]);
  BOOST_CHECK_EQUAL(doc1.xs[2].tokens.items[2], &doc1.tokens[2]);
  BOOST_CHECK_EQUAL(doc1.xs[2].tokens.items[3], &doc1.tokens[3]);
  BOOST_CHECK_EQUAL(doc1.xs[2].tokens.items[4], &doc1.tokens[4]);
  BOOST_CHECK_EQUAL(doc1.xs[4].tokens.items[0], &doc1.tokens[9]);

  BOOST_CHECK_EQUAL(doc1.xs[0].prior.items.size(), 1);
  BOOST_CHECK_EQUAL(doc1.xs[1].prior.items.size(), 2);
  BOOST_CHECK_EQUAL(doc1.xs[2].prior.items.size(), 3);
  BOOST_CHECK_EQUAL(doc1.xs[3].prior.items.size(), 4);
  BOOST_CHECK_EQUAL(doc1.xs[4].prior.items.size(), 5);

  BOOST_CHECK_EQUAL(doc1.xs[0].prior.items[0], &doc1.xs[0]);
  BOOST_CHECK_EQUAL(doc1.xs[1].prior.items[0], &doc1.xs[0]);
  BOOST_CHECK_EQUAL(doc1.xs[1].prior.items[1], &doc1.xs[1]);
  BOOST_CHECK_EQUAL(doc1.xs[2].prior.items[0], &doc1.xs[0]);
  BOOST_CHECK_EQUAL(doc1.xs[2].prior.items[1], &doc1.xs[1]);
  BOOST_CHECK_EQUAL(doc1.xs[2].prior.items[2], &doc1.xs[2]);
  BOOST_CHECK_EQUAL(doc1.xs[3].prior.items[0], &doc1.xs[0]);
  BOOST_CHECK_EQUAL(doc1.xs[3].prior.items[1], &doc1.xs[1]);
  BOOST_CHECK_EQUAL(doc1.xs[3].prior.items[2], &doc1.xs[2]);
  BOOST_CHECK_EQUAL(doc1.xs[3].prior.items[3], &doc1.xs[3]);
  BOOST_CHECK_EQUAL(doc1.xs[4].prior.items[0], &doc1.xs[0]);
  BOOST_CHECK_EQUAL(doc1.xs[4].prior.items[1], &doc1.xs[1]);
  BOOST_CHECK_EQUAL(doc1.xs[4].prior.items[2], &doc1.xs[2]);
  BOOST_CHECK_EQUAL(doc1.xs[4].prior.items[3], &doc1.xs[3]);
  BOOST_CHECK_EQUAL(doc1.xs[4].prior.items[4], &doc1.xs[4]);
}

BOOST_AUTO_TEST_SUITE_END()
