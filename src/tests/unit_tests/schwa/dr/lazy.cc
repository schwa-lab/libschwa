/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>
#include "test_utils.h"

#include <boost/test/unit_test.hpp>

namespace dr = schwa::dr;

class A : public dr::Ann {
public:
  std::string v_str;
  uint8_t v_uint8;
  bool v_bool;

  A(void) : dr::Ann(), v_uint8(0), v_bool(false) { }

  class Schema;
};


class B : public dr::Ann {
public:
  std::string word;
  std::string upper;
  bool is_first;

  B(void) : dr::Ann(), is_first(false) { }

  class Schema;
};


class DocA : public dr::Doc {
public:
  dr::Store<A> as;

  class Schema;
};


class DocB : public dr::Doc {
public:
  dr::Store<B> bs;

  class Schema;
};


class A::Schema : public dr::Ann::Schema<A> {
public:
  DR_FIELD(&A::v_str) v_str;
  DR_FIELD(&A::v_uint8) v_uint8;
  DR_FIELD(&A::v_bool) v_bool;

  Schema(void) :
    dr::Ann::Schema<A>("A", "Some text about A", "A"),
    v_str(*this, "v_str", "some text about v_str", dr::FieldMode::READ_WRITE, "v_str"),
    v_uint8(*this, "v_uint8", "some text about v_uint8", dr::FieldMode::READ_WRITE, "v_uint8"),
    v_bool(*this, "v_bool", "some text about v_bool", dr::FieldMode::READ_WRITE, "v_bool")
    { }
  virtual ~Schema(void) { }
};


class B::Schema : public dr::Ann::Schema<B> {
public:
  DR_FIELD(&B::word) word;
  DR_FIELD(&B::upper) upper;
  DR_FIELD(&B::is_first) is_first;

  Schema(void) :
    dr::Ann::Schema<B>("B", "Some text about B", "B"),
    word(*this, "word", "some text about word", dr::FieldMode::READ_ONLY, "word"),
    upper(*this, "upper", "some text about upper", dr::FieldMode::READ_WRITE, "upper"),
    is_first(*this, "is_first", "some text about is_first", dr::FieldMode::READ_ONLY, "is_first")
    { }
  virtual ~Schema(void) { }
};


class DocA::Schema : public dr::Doc::Schema<DocA> {
public:
  DR_STORE(&DocA::as) as;

  Schema(void) :
    dr::Doc::Schema<DocA>("DocA", "Some text about DocA"),
    as(*this, "as", "some text about as", dr::FieldMode::READ_WRITE, "as")
    { }
  virtual ~Schema(void) { }
};


class DocB::Schema : public dr::Doc::Schema<DocB> {
public:
  DR_STORE(&DocB::bs) bs;

  Schema(void) :
    dr::Doc::Schema<DocB>("DocB", "Some text about DocB"),
    bs(*this, "bs", "some text about bs", dr::FieldMode::READ_WRITE, "as")
    { }
  virtual ~Schema(void) { }
};


// ============================================================================
// ============================================================================
BOOST_AUTO_TEST_SUITE(schwa_dr_lazy)

BOOST_AUTO_TEST_CASE(lazy_test0) {
  static const char *WORDS[] = {"The", "quick", "brown", "fox", "jumped"};
  static constexpr size_t NWORDS = sizeof(WORDS)/sizeof(char *);
  std::stringstream stream0, stream1;

  DocA doc0;
  DocA::Schema schema0;
  dr::Writer writer0(stream0, schema0);

  doc0.as.create(NWORDS);
  for (uint8_t i = 0; i != NWORDS; ++i) {
    A &a = doc0.as[i];
    a.v_str = WORDS[i];
    a.v_uint8 = i;
    a.v_bool = i == 0;
  }
  writer0 << doc0;

  DocB::Schema schema1_in, schema1_out;
  schema1_in.types<B>().serial = "A";
  schema1_in.types<B>().word.serial = "v_str";
  schema1_in.types<B>().is_first.serial = "v_bool";
  dr::Reader reader1(stream0, schema1_in);
  dr::Writer writer1(stream1, schema1_out);

  unsigned int ndocs_read1 = 0;
  while (true) {
    DocB doc1;
    if (!(reader1 >> doc1))
      break;
    ++ndocs_read1;

    // check annotation values
    BOOST_REQUIRE_EQUAL(doc1.bs.size(), NWORDS);
    for (uint8_t i = 0; i != NWORDS; ++i) {
      B &b = doc1.bs[i];
      BOOST_CHECK_EQUAL(b.word, WORDS[i]);
      BOOST_CHECK(b.upper.empty());
      BOOST_CHECK_EQUAL(b.is_first, i == 0);
    }

    // check document lazy
    BOOST_CHECK_EQUAL(static_cast<const void *>(doc1.lazy()), static_cast<const void *>(nullptr));
    BOOST_CHECK_EQUAL(doc1.lazy_nbytes(), 0);
    BOOST_CHECK_EQUAL(doc1.lazy_nelem(), 0);

    // check annotation lazy
    for (uint8_t i = 0; i != NWORDS; ++i) {
      B &b = doc1.bs[i];
      BOOST_CHECK_NE(static_cast<const void *>(b.lazy()), static_cast<const void *>(nullptr));
      BOOST_CHECK_EQUAL(b.lazy_nelem(), 3);
      BOOST_CHECK_EQUAL(b.lazy_nbytes(), 3 + (b.word.size() + 1) + 2 + 1);
    }

    // modify some attributes, including lazy ones
    for (uint8_t i = 0; i != NWORDS; ++i) {
      B &b = doc1.bs[i];
      for (auto &c : b.word)
        b.upper.push_back(std::toupper(c));
      b.is_first = true;
    }

    writer1 << doc1;
  }
  BOOST_CHECK_EQUAL(ndocs_read1, 1);

  DocA::Schema schema2;
  schema2.types<A>().serial = "B";
  schema2.types<A>().v_str.serial = "word";
  schema2.types<A>().v_bool.serial = "is_first";
  dr::Reader reader2(stream1, schema2);

  unsigned int ndocs_read2 = 0;
  while (true) {
    DocA doc2;
    if (!(reader2 >> doc2))
      break;
    ++ndocs_read2;

    // check annotation values
    BOOST_REQUIRE_EQUAL(doc2.as.size(), NWORDS);
    for (uint8_t i = 0; i != NWORDS; ++i) {
      A &a = doc2.as[i];
      BOOST_CHECK_EQUAL(a.v_str, WORDS[i]);
      BOOST_CHECK_EQUAL(a.v_uint8, i);
      BOOST_CHECK_EQUAL(a.v_bool, i == 0);
    }

    // check document lazy
    BOOST_CHECK_EQUAL(static_cast<const void *>(doc2.lazy()), static_cast<const void *>(nullptr));
    BOOST_CHECK_EQUAL(doc2.lazy_nbytes(), 0);
    BOOST_CHECK_EQUAL(doc2.lazy_nelem(), 0);

    // check annotation lazy
    for (uint8_t i = 0; i != NWORDS; ++i) {
      A &a = doc2.as[i];
      BOOST_CHECK_NE(static_cast<const void *>(a.lazy()), static_cast<const void *>(nullptr));
      BOOST_CHECK_EQUAL(a.lazy_nelem(), 1);
      BOOST_CHECK_EQUAL(a.lazy_nbytes(), 1 + (a.v_str.size() + 1));
    }

  }
  BOOST_CHECK_EQUAL(ndocs_read2, 1);
}

BOOST_AUTO_TEST_SUITE_END()
