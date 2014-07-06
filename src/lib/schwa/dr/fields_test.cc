/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <type_traits>

#include <schwa/dr.h>


namespace schwa {
namespace dr {

SUITE(schwa__dr__fields) {

TEST(slice_foreach_test0) {
  class Token : public Ann {
  public:
    class Schema;
  };

  class Sent : public Ann {
  public:
    Slice<Token *> span;
    class Schema;
  };

  class D : public Doc {
  public:
    Store<Token> tokens;
    Store<Sent> sentences;
    class Schema;
  };

  class Token::Schema : public Ann::Schema<Token> {
  public:
    Schema(void) : Ann::Schema<Token>("Token", "The token class") { }
    virtual ~Schema(void) { }
  };

  class Sent::Schema : public Ann::Schema<Sent> {
  public:
    DR_POINTER(&Sent::span, &D::tokens) span;

    Schema(void) :
        Ann::Schema<Sent>("Sentence", "The sentence class"),
        span(*this, "span", "the span of the sentence", FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };

  class D::Schema : public dr::Doc::Schema<D> {
  public:
    DR_STORE(&D::tokens) tokens;
    DR_STORE(&D::sentences) sentences;

    Schema(void) :
        Doc::Schema<D>("D", "The document class"),
        tokens(*this, "tokens", "The store for the tokens", FieldMode::READ_ONLY),
        sentences(*this, "sentences", "The store for the sentences", FieldMode::READ_ONLY)
      { }
    virtual ~Schema(void) { }
  };

  D d;
  d.tokens.create(10);
  d.sentences.create(1);
  Sent &s = d.sentences[0];
  s.span.start = &d.tokens.front();
  s.span.stop = &d.tokens.back() + 1;

  size_t ntokens = 0;
  for (auto &token : s.span) {
    static_assert(std::is_pointer<decltype(token)>::value == false, "for-each loop iterators not implemented correctly");
    (void)token;
    CHECK_EQUAL(&d.tokens[ntokens], &token);
    ++ntokens;
  }
  CHECK_EQUAL(10, ntokens);
}

}  // SUITE

}  // namespace dr
}  // namespace schwa
