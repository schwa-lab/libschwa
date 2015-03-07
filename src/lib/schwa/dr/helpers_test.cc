/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <sstream>
#include <vector>

#include <schwa/canonical-schema.h>
#include <schwa/dr.h>
#include <schwa/enums.h>

namespace cs = ::schwa::canonical_schema;


namespace schwa {
namespace dr {

namespace {

class Chunk;
class Sent;

class X { };

class Token : public Ann {
public:
  std::string raw;
  X *x;
  Sent *sent;
  std::vector<Sent *> sents;
  Chunk *chunk;
  std::string chunk_tag;

  Token(void) : Ann(), sent(nullptr), chunk(nullptr) { }
  Token(const char *lazy, const uint32_t nelem, const uint32_t nbytes) : Ann(lazy, nelem, nbytes), sent(nullptr), chunk(nullptr) { }
  Token(const Token &o) : Ann(o), raw(o.raw), sent(o.sent), sents(o.sents), chunk(o.chunk), chunk_tag(o.chunk_tag) { }
  Token(const Token &&o) : Ann(o), raw(o.raw), sent(o.sent), sents(o.sents), chunk(o.chunk), chunk_tag(o.chunk_tag) { }
  ~Token(void) { }

  Token &operator =(const Token &) { return *this; }

  class Schema;
};


class Sent : public Ann {
public:
  Slice<Token *> span;

  class Schema;
};


class Chunk : public Ann {
public:
  Slice<Token *> span;
  std::string type;

  class Schema;
};


class TestDoc : public Doc {
public:
  Store<Token> tokens;
  Store<Sent> sents;
  Store<Chunk> chunks;

  Store<Chunk> untagged_chunks;

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


class Chunk::Schema : public Ann::Schema<Chunk> {
public:
  DR_POINTER(&Chunk::span, &TestDoc::tokens) span;
  DR_FIELD(&Chunk::type) type;

  Schema(void) :
      Ann::Schema<Chunk>("Chunk", "The chunk class"),
      span(*this, "span", "the span of the chunk", FieldMode::READ_WRITE),
      type(*this, "type", "the type of the chunk", FieldMode::READ_WRITE)
    { }
  virtual ~Schema(void) { }
};


class TestDoc::Schema : public Doc::Schema<TestDoc> {
public:
  DR_STORE(&TestDoc::tokens) tokens;
  DR_STORE(&TestDoc::sents) sents;
  DR_STORE(&TestDoc::chunks) chunks;

  Schema(void) :
      Doc::Schema<TestDoc>("TestDoc", "The document class"),
      tokens(*this, "tokens", "The store for the tokens", FieldMode::READ_WRITE),
      sents(*this, "sentences", "The store for the sentences", FieldMode::READ_WRITE),
      chunks(*this, "chunks", "The store for the chunks", FieldMode::READ_WRITE)
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


TEST(test_sequence_tagging) {
  TestDoc::Schema schema;
  std::stringstream serialised;

  // x x x x x x x x x x x x | x x
  // -----   - --- - ---   -   -
  //   X     Y  Y  Y  X    Z   Z
  {
    TestDoc doc;
    doc.tokens.create(14);
    for (size_t i = 0; i != doc.tokens.size(); ++i) {
      std::stringstream ss;
      ss << i;
      doc.tokens[i].raw = ss.str();
    }
    doc.sents.create(2);
    doc.sents[0].span.start = &doc.tokens[0];
    doc.sents[0].span.stop = &doc.tokens[12];
    doc.sents[1].span.start = &doc.tokens[12];
    doc.sents[1].span.stop = &doc.tokens[14];
    doc.chunks.create(7);
    doc.chunks[0].span.start = &doc.tokens[0];
    doc.chunks[0].span.stop = &doc.tokens[3];
    doc.chunks[0].type = "X";
    doc.chunks[1].span.start = &doc.tokens[4];
    doc.chunks[1].span.stop = &doc.tokens[5];
    doc.chunks[1].type = "Y";
    doc.chunks[2].span.start = &doc.tokens[5];
    doc.chunks[2].span.stop = &doc.tokens[7];
    doc.chunks[2].type = "Y";
    doc.chunks[3].span.start = &doc.tokens[7];
    doc.chunks[3].span.stop = &doc.tokens[8];
    doc.chunks[3].type = "Y";
    doc.chunks[4].span.start = &doc.tokens[8];
    doc.chunks[4].span.stop = &doc.tokens[10];
    doc.chunks[4].type = "X";
    doc.chunks[5].span.start = &doc.tokens[11];
    doc.chunks[5].span.stop = &doc.tokens[12];
    doc.chunks[5].type = "Z";
    doc.chunks[6].span.start = &doc.tokens[12];
    doc.chunks[6].span.stop = &doc.tokens[13];
    doc.chunks[6].type = "Z";

    Writer writer(serialised, schema);
    writer << doc;
  }
  serialised.seekg(0);
  CHECK(!serialised.str().empty());

  const auto reverse_chunks = DR_REVERSE_SLICES(&TestDoc::chunks, &TestDoc::tokens, &Chunk::span, &Token::chunk);
  const auto reverse_sents = DR_REVERSE_SLICES(&TestDoc::sents, &TestDoc::tokens, &Sent::span, &Token::sents);
  const auto tagger = DR_SEQUENCE_TAGGER(&TestDoc::chunks, &TestDoc::sents, &TestDoc::tokens, &Chunk::span, &Sent::span, &Token::chunk, &Chunk::type, &Token::chunk_tag);
  const auto untagger = DR_SEQUENCE_UNTAGGER(&TestDoc::untagged_chunks, &TestDoc::sents, &Chunk::span, &Sent::span, &Chunk::type, &Token::chunk_tag);

  {
    TestDoc d;
    serialised.seekg(0);
    Reader reader(serialised, schema);
    CHECK(static_cast<bool>(reader >> d));

    reverse_sents(d);
    reverse_chunks(d);
    tagger(d, SequenceTagEncoding::IOB1);

    CHECK_EQUAL("I-X", d.tokens[0].chunk_tag);
    CHECK_EQUAL("I-X", d.tokens[1].chunk_tag);
    CHECK_EQUAL("I-X", d.tokens[2].chunk_tag);
    CHECK_EQUAL("O", d.tokens[3].chunk_tag);
    CHECK_EQUAL("I-Y", d.tokens[4].chunk_tag);
    CHECK_EQUAL("B-Y", d.tokens[5].chunk_tag);
    CHECK_EQUAL("I-Y", d.tokens[6].chunk_tag);
    CHECK_EQUAL("B-Y", d.tokens[7].chunk_tag);
    CHECK_EQUAL("I-X", d.tokens[8].chunk_tag);
    CHECK_EQUAL("I-X", d.tokens[9].chunk_tag);
    CHECK_EQUAL("O", d.tokens[10].chunk_tag);
    CHECK_EQUAL("I-Z", d.tokens[11].chunk_tag);
    CHECK_EQUAL("I-Z", d.tokens[12].chunk_tag);
    CHECK_EQUAL("O", d.tokens[13].chunk_tag);

    CHECK_EQUAL(0, d.untagged_chunks.size());
    untagger(d);
    CHECK_EQUAL(7, d.untagged_chunks.size());
    for (size_t i = 0; i != d.untagged_chunks.size(); ++i) {
      CHECK_EQUAL(d.chunks[i].type, d.untagged_chunks[i].type);
      CHECK_EQUAL(d.chunks[i].span.start, d.untagged_chunks[i].span.start);
      CHECK_EQUAL(d.chunks[i].span.stop, d.untagged_chunks[i].span.stop);
    }
  }

  {
    TestDoc d;
    serialised.seekg(0);
    Reader reader(serialised, schema);
    CHECK(static_cast<bool>(reader >> d));

    reverse_sents(d);
    reverse_chunks(d);
    tagger(d, SequenceTagEncoding::IOB2);

    CHECK_EQUAL("B-X", d.tokens[0].chunk_tag);
    CHECK_EQUAL("I-X", d.tokens[1].chunk_tag);
    CHECK_EQUAL("I-X", d.tokens[2].chunk_tag);
    CHECK_EQUAL("O", d.tokens[3].chunk_tag);
    CHECK_EQUAL("B-Y", d.tokens[4].chunk_tag);
    CHECK_EQUAL("B-Y", d.tokens[5].chunk_tag);
    CHECK_EQUAL("I-Y", d.tokens[6].chunk_tag);
    CHECK_EQUAL("B-Y", d.tokens[7].chunk_tag);
    CHECK_EQUAL("B-X", d.tokens[8].chunk_tag);
    CHECK_EQUAL("I-X", d.tokens[9].chunk_tag);
    CHECK_EQUAL("O", d.tokens[10].chunk_tag);
    CHECK_EQUAL("B-Z", d.tokens[11].chunk_tag);
    CHECK_EQUAL("B-Z", d.tokens[12].chunk_tag);
    CHECK_EQUAL("O", d.tokens[13].chunk_tag);

    CHECK_EQUAL(0, d.untagged_chunks.size());
    untagger(d);
    CHECK_EQUAL(7, d.untagged_chunks.size());
    for (size_t i = 0; i != d.untagged_chunks.size(); ++i) {
      CHECK_EQUAL(d.chunks[i].type, d.untagged_chunks[i].type);
      CHECK_EQUAL(d.chunks[i].span.start, d.untagged_chunks[i].span.start);
      CHECK_EQUAL(d.chunks[i].span.stop, d.untagged_chunks[i].span.stop);
    }
  }

  {
    TestDoc d;
    serialised.seekg(0);
    Reader reader(serialised, schema);
    CHECK(static_cast<bool>(reader >> d));

    reverse_sents(d);
    reverse_chunks(d);
    tagger(d, SequenceTagEncoding::BMEWO);

    CHECK_EQUAL("B-X", d.tokens[0].chunk_tag);
    CHECK_EQUAL("M-X", d.tokens[1].chunk_tag);
    CHECK_EQUAL("E-X", d.tokens[2].chunk_tag);
    CHECK_EQUAL("O", d.tokens[3].chunk_tag);
    CHECK_EQUAL("W-Y", d.tokens[4].chunk_tag);
    CHECK_EQUAL("B-Y", d.tokens[5].chunk_tag);
    CHECK_EQUAL("E-Y", d.tokens[6].chunk_tag);
    CHECK_EQUAL("W-Y", d.tokens[7].chunk_tag);
    CHECK_EQUAL("B-X", d.tokens[8].chunk_tag);
    CHECK_EQUAL("E-X", d.tokens[9].chunk_tag);
    CHECK_EQUAL("O", d.tokens[10].chunk_tag);
    CHECK_EQUAL("W-Z", d.tokens[11].chunk_tag);
    CHECK_EQUAL("W-Z", d.tokens[12].chunk_tag);
    CHECK_EQUAL("O", d.tokens[13].chunk_tag);

    CHECK_EQUAL(0, d.untagged_chunks.size());
    untagger(d);
    CHECK_EQUAL(7, d.untagged_chunks.size());
    for (size_t i = 0; i != d.untagged_chunks.size(); ++i) {
      CHECK_EQUAL(d.chunks[i].type, d.untagged_chunks[i].type);
      CHECK_EQUAL(d.chunks[i].span.start, d.untagged_chunks[i].span.start);
      CHECK_EQUAL(d.chunks[i].span.stop, d.untagged_chunks[i].span.stop);
    }

    d.untagged_chunks.clear();
    d.tokens[1].chunk_tag = "W-Q";
    CHECK_EQUAL(0, d.untagged_chunks.size());
    untagger(d);
    CHECK_EQUAL(9, d.untagged_chunks.size());
    CHECK_EQUAL("X", d.untagged_chunks[0].type);
    CHECK_EQUAL("Q", d.untagged_chunks[1].type);
    CHECK_EQUAL("X", d.untagged_chunks[2].type);
    CHECK_EQUAL(&d.tokens[0], d.untagged_chunks[0].span.start);
    CHECK_EQUAL(&d.tokens[1], d.untagged_chunks[0].span.stop);
    CHECK_EQUAL(&d.tokens[1], d.untagged_chunks[1].span.start);
    CHECK_EQUAL(&d.tokens[2], d.untagged_chunks[1].span.stop);
    CHECK_EQUAL(&d.tokens[2], d.untagged_chunks[2].span.start);
    CHECK_EQUAL(&d.tokens[3], d.untagged_chunks[2].span.stop);
    for (size_t i = 1; i != d.chunks.size(); ++i) {
      CHECK_EQUAL(d.chunks[i].type, d.untagged_chunks[i + 2].type);
      CHECK_EQUAL(d.chunks[i].span.start, d.untagged_chunks[i + 2].span.start);
      CHECK_EQUAL(d.chunks[i].span.stop, d.untagged_chunks[i + 2].span.stop);
    }
  }
}


TEST(test_sequence_tagging2) {
  static const auto TAGGER = DR_SEQUENCE_TAGGER(&cs::Doc::named_entities, &cs::Doc::sentences, &cs::Doc::tokens, &cs::NamedEntity::span, &cs::Sentence::span, &cs::Token::ne, &cs::NamedEntity::label, &cs::Token::ne_label);
  static const auto UNTAGGER = DR_SEQUENCE_UNTAGGER(&cs::Doc::named_entities_crf1, &cs::Doc::sentences, &cs::NamedEntity::span, &cs::Sentence::span, &cs::NamedEntity::label, &cs::Token::ne_label);

  // Taken from OntoNotes 5 bc/cctv/00/cctv_0000@0000@cctv@bc@en@on.
  static const char *TOKENS[27] = {
    "As", "its", "neighbor", "on", "Lantau", "Island", ",", "Hong", "Kong", "International", "Airport",
    "had", "to", "change", "its", "flight", "routes", "to", "make", "this", "area", "a", "no", "-",
    "fly", "zone", ".",
  };
  static const char *BMEWO_LABELS[27] = {
    "O", "O", "O", "O", "B-LOC", "E-LOC", "O", "B-ORG", "M-ORG", "M-ORG", "E-ORG",
    "O", "O", "O", "O", "O", "O", "O", "O", "O", "O", "O", "O", "O",
    "O", "O", "O",
  };
  static const char *IOB2_LABELS[27] = {
    "O", "O", "O", "O", "B-LOC", "I-LOC", "O", "B-ORG", "I-ORG", "I-ORG", "I-ORG",
    "O", "O", "O", "O", "O", "O", "O", "O", "O", "O", "O", "O", "O",
    "O", "O", "O",
  };

  // Construct the document structure.
  cs::Doc doc;
  doc.tokens.create(27);
  for (size_t i = 0; i != 27; ++i)
    doc.tokens[i].raw = TOKENS[i];
  doc.sentences.create(1);
  doc.sentences[0].span.start = &doc.tokens.front();
  doc.sentences[0].span.stop = doc.sentences[0].span.start + 27;
  doc.named_entities.create(2);
  doc.named_entities[0].span.start = &doc.tokens.front() + 4;
  doc.named_entities[0].span.stop = doc.named_entities[0].span.start + 2;
  doc.named_entities[0].label = "LOC";
  doc.named_entities[1].span.start = &doc.tokens.front() + 7;
  doc.named_entities[1].span.stop = doc.named_entities[1].span.start + 4;
  doc.named_entities[1].label = "ORG";

  // BMEWO tag the NE labels onto the tokens.
  TAGGER(doc, SequenceTagEncoding::BMEWO);
  for (size_t i = 0; i != 27; ++i)
    CHECK_EQUAL(BMEWO_LABELS[i], doc.tokens[i].ne_label);

  // Untag the BMEWO tags.
  doc.named_entities_crf1.clear();
  UNTAGGER(doc);
  CHECK_EQUAL(2, doc.named_entities_crf1.size());
  for (size_t i = 0; i != 2; ++i) {
    CHECK_EQUAL(doc.named_entities[i].span.start, doc.named_entities_crf1[i].span.start);
    CHECK_EQUAL(doc.named_entities[i].span.stop, doc.named_entities_crf1[i].span.stop);
    CHECK_EQUAL(doc.named_entities[i].label, doc.named_entities_crf1[i].label);
  }

  // IOB2 tag the NE labels onto the tokens.
  TAGGER(doc, SequenceTagEncoding::IOB2);
  for (size_t i = 0; i != 27; ++i)
    CHECK_EQUAL(IOB2_LABELS[i], doc.tokens[i].ne_label);

  // Untag the IOB2 tags.
  doc.named_entities_crf1.clear();
  UNTAGGER(doc);
  CHECK_EQUAL(2, doc.named_entities_crf1.size());
  for (size_t i = 0; i != 2; ++i) {
    CHECK_EQUAL(doc.named_entities[i].span.start, doc.named_entities_crf1[i].span.start);
    CHECK_EQUAL(doc.named_entities[i].span.stop, doc.named_entities_crf1[i].span.stop);
    CHECK_EQUAL(doc.named_entities[i].label, doc.named_entities_crf1[i].label);
  }
}


TEST(test_sequence_tagging3) {
  static const auto UNTAGGER = DR_SEQUENCE_UNTAGGER(&cs::Doc::named_entities, &cs::Doc::sentences, &cs::NamedEntity::span, &cs::Sentence::span, &cs::NamedEntity::label, &cs::Token::ne_label);

  static const char *TOKENS[22] = {
    "I", "went", "to", "China", "and", "saw", "WW", "II", "Landmarks", "on",
    "the", "Great", "Earth", "of", "China", ":", "Eternal", "Memories", "of", "Taihang",
    "Mountain", ".",
  };
  static const char *BMEWO_LABELS[22] = {
    "O", "O", "O", "E-ORG", "O", "O", "B-WORK_OF_ART", "M-WORK_OF_ART", "M-WORK_OF_ART", "M-WORK_OF_ART",
    "M-WORK_OF_ART", "M-WORK_OF_ART", "M-WORK_OF_ART", "M-WORK_OF_ART", "M-WORK_OF_ART", "M-WORK_OF_ART", "M-WORK_OF_ART", "M-WORK_OF_ART", "M-WORK_OF_ART", "M-WORK_OF_ART",
    "E-WORK_OF_ART", "O",
  };

  // Construct the document structure.
  cs::Doc doc;
  doc.tokens.create(22);
  for (size_t i = 0; i != 22; ++i) {
    doc.tokens[i].raw = TOKENS[i];
    doc.tokens[i].ne_label = BMEWO_LABELS[i];
  }
  doc.sentences.create(1);
  doc.sentences[0].span.start = &doc.tokens.front();
  doc.sentences[0].span.stop = doc.sentences[0].span.start + 22;

  // BMEWO tag the NE labels onto the tokens.
  UNTAGGER(doc);
  CHECK_EQUAL(2, doc.named_entities.size());
  CHECK_EQUAL("ORG", doc.named_entities[0].label);
  CHECK_EQUAL(&doc.tokens[3], doc.named_entities[0].span.start);
  CHECK_EQUAL(&doc.tokens[4], doc.named_entities[0].span.stop);
  CHECK_EQUAL("WORK_OF_ART", doc.named_entities[1].label);
  CHECK_EQUAL(&doc.tokens[6], doc.named_entities[1].span.start);
  CHECK_EQUAL(&doc.tokens[21], doc.named_entities[1].span.stop);
}

}  // SUITE

}  // namespace dr
}  // namespace schwa
