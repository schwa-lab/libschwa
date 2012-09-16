/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>
#include "test_utils.h"

#include <boost/test/unit_test.hpp>

namespace dr = schwa::dr;

// ============================================================================
// ============================================================================
BOOST_AUTO_TEST_SUITE(schwa__dr__self_pointer)

BOOST_AUTO_TEST_CASE(self_pointer0) {
  class X : public dr::Ann {
  public:
    dr::Pointer<X> parent;
    dr::Pointer<X> other;

    class Schema;
  };


  class Doc : public dr::Doc {
  public:
    dr::Store<X> xs1;
    dr::Store<X> xs2;

    class Schema;
  };


  class X::Schema : public dr::Ann::Schema<X> {
  public:
    DR_SELF(&X::parent) parent;
    DR_POINTER(&X::other, &Doc::xs2) other;

    Schema(void) :
      dr::Ann::Schema<X>("X", "Some text about X"),
      parent(*this, "parent", "parent", dr::FieldMode::READ_WRITE),
      other(*this, "other", "other", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class Doc::Schema : public dr::Doc::Schema<Doc> {
  public:
    DR_STORE(&Doc::xs1) xs1;
    DR_STORE(&Doc::xs2) xs2;

    Schema(void) :
      dr::Doc::Schema<Doc>("Doc", "Some text about Doc"),
      xs1(*this, "xs1", "some text about xs1", dr::FieldMode::READ_WRITE),
      xs2(*this, "xs2", "some text about xs2", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  Doc doc0;
  doc0.xs1.create(3);
  doc0.xs2.create(2);

  doc0.xs1[1].parent.ptr = &doc0.xs1[0];
  doc0.xs1[2].parent.ptr = &doc0.xs1[1];
  doc0.xs2[1].parent.ptr = &doc0.xs2[0];

  doc0.xs1[0].other.ptr = &doc0.xs2[0];
  doc0.xs1[1].other.ptr = &doc0.xs2[0];
  doc0.xs1[2].other.ptr = &doc0.xs2[0];
  doc0.xs2[0].other.ptr = &doc0.xs2[0];
  doc0.xs2[1].other.ptr = &doc0.xs2[0];

  std::stringstream stream0;
  Doc::Schema schema0;
  dr::Writer writer0(stream0, schema0);
  writer0 << doc0;

#if 0
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

  BOOST_CHECK( compare_bytes(stream0.str(), stream0_expected, sizeof(stream0_expected)/sizeof(uint8_t)) );

  DocB::Schema schema1_in, schema1_out;
  schema1_in.bs.serial = "as";
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
    BOOST_CHECK_EQUAL(static_cast<const void *>(doc1.lazy_data()), static_cast<const void *>(nullptr));
    BOOST_CHECK_EQUAL(doc1.lazy_nbytes(), 0);
    BOOST_CHECK_EQUAL(doc1.lazy_nelem(), 0);

    // check annotation lazy
    for (uint8_t i = 0; i != NWORDS; ++i) {
      B &b = doc1.bs[i];
      BOOST_CHECK_NE(static_cast<const void *>(b.lazy_data()), static_cast<const void *>(nullptr));
      BOOST_CHECK_EQUAL(b.lazy_nelem(), 3);
      BOOST_CHECK_EQUAL(b.lazy_nbytes(), 3 + (b.word.size() + 1) + 1 + 1);
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

  BOOST_CHECK( compare_bytes(stream1.str(), stream1_expected, sizeof(stream1_expected)/sizeof(uint8_t)) );

  DocA::Schema schema2;
  schema2.as.serial = "bs";
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
    BOOST_CHECK_EQUAL(static_cast<const void *>(doc2.lazy_data()), static_cast<const void *>(nullptr));
    BOOST_CHECK_EQUAL(doc2.lazy_nbytes(), 0);
    BOOST_CHECK_EQUAL(doc2.lazy_nelem(), 0);

    // check annotation lazy
    for (uint8_t i = 0; i != NWORDS; ++i) {
      A &a = doc2.as[i];
      BOOST_CHECK_NE(static_cast<const void *>(a.lazy_data()), static_cast<const void *>(nullptr));
      BOOST_CHECK_EQUAL(a.lazy_nelem(), 1);
      BOOST_CHECK_EQUAL(a.lazy_nbytes(), 1 + (a.v_str.size() + 1));
    }

  }
  BOOST_CHECK_EQUAL(ndocs_read2, 1);
}


BOOST_AUTO_TEST_CASE(lazy_test1) {
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


  class DocAB : public dr::Doc {
  public:
    dr::Store<A> as;
    dr::Store<B> bs;

    class Schema;
  };


  class A::Schema : public dr::Ann::Schema<A> {
  public:
    DR_FIELD(&A::v_str) v_str;
    DR_FIELD(&A::v_uint8) v_uint8;
    DR_FIELD(&A::v_bool) v_bool;

    Schema(void) :
      dr::Ann::Schema<A>("A", "Some text about A"),
      v_str(*this, "v_str", "some text about v_str", dr::FieldMode::READ_WRITE),
      v_uint8(*this, "v_uint8", "some text about v_uint8", dr::FieldMode::READ_WRITE),
      v_bool(*this, "v_bool", "some text about v_bool", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class B::Schema : public dr::Ann::Schema<B> {
  public:
    DR_FIELD(&B::word) word;
    DR_FIELD(&B::upper) upper;
    DR_FIELD(&B::is_first) is_first;

    Schema(void) :
      dr::Ann::Schema<B>("B", "Some text about B"),
      word(*this, "word", "some text about word", dr::FieldMode::READ_WRITE),
      upper(*this, "upper", "some text about upper", dr::FieldMode::READ_WRITE),
      is_first(*this, "is_first", "some text about is_first", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class DocA::Schema : public dr::Doc::Schema<DocA> {
  public:
    DR_STORE(&DocA::as) as;

    Schema(void) :
      dr::Doc::Schema<DocA>("DocA", "Some text about DocA"),
      as(*this, "as", "some text about as", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class DocB::Schema : public dr::Doc::Schema<DocB> {
  public:
    DR_STORE(&DocB::bs) bs;

    Schema(void) :
      dr::Doc::Schema<DocB>("DocB", "Some text about DocB"),
      bs(*this, "bs", "some text about bs", dr::FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class DocAB::Schema : public dr::Doc::Schema<DocAB> {
  public:
    DR_STORE(&DocAB::as) as;
    DR_STORE(&DocAB::bs) bs;

    Schema(void) :
      dr::Doc::Schema<DocAB>("DocAB", "Some text about DocAB"),
      as(*this, "as", "some text about as", dr::FieldMode::READ_ONLY),
      bs(*this, "bs", "some text about bs", dr::FieldMode::READ_ONLY)
      { }
    virtual ~Schema(void) { }
  };
  static const char *WORDS[] = {"How", "now", "brown", "cow"};
  static constexpr size_t NWORDS = sizeof(WORDS)/sizeof(char *);
  static constexpr uint8_t stream0_expected[] = {
    0x92,
      0x92, 0xA8, '_', '_', 'm', 'e', 't', 'a', '_', '_', 0x90,
      0x92, 0xA1, 'A', 0x93, 0x81, 0x00, 0xA5, 'v', '_', 's', 't', 'r', 0x81, 0x00, 0xA7, 'v', '_', 'u', 'i', 'n', 't', '8', 0x81, 0x00, 0xA6, 'v', '_', 'b', 'o', 'o', 'l',
    0x91,
      0x93, 0xA2, 'a', 's', 0x01, 0x04,
    0x01,
      0x80,
    0x2B, 0x94,
      0x83, 0x00, 0xA3, 'H', 'o', 'w', 0x01, 0x00, 0x02, 0xC3,
      0x83, 0x00, 0xA3, 'n', 'o', 'w', 0x01, 0x01, 0x02, 0xC2,
      0x83, 0x00, 0xA5, 'b', 'r', 'o', 'w', 'n', 0x01, 0x02, 0x02, 0xC3,
      0x83, 0x00, 0xA3, 'c', 'o', 'w', 0x01, 0x03, 0x02, 0xC2,
  };
  static constexpr uint8_t stream1_expected[] = {
    0x93,
      0x92, 0xA8, '_', '_', 'm', 'e', 't', 'a', '_', '_', 0x90,
      0x92, 0xA1, 'A', 0x93, 0x81, 0x00, 0xA5, 'v', '_', 's', 't', 'r', 0x81, 0x00, 0xA7, 'v', '_', 'u', 'i', 'n', 't', '8', 0x81, 0x00, 0xA6, 'v', '_', 'b', 'o', 'o', 'l',
      0x92, 0xA1, 'B', 0x93, 0x81, 0x00, 0xA4, 'w', 'o', 'r', 'd', 0x81, 0x00, 0xA5, 'u', 'p', 'p', 'e', 'r', 0x81, 0x00, 0xA8, 'i', 's', '_', 'f', 'i', 'r', 's', 't',
    0x92,
      0x93, 0xA2, 'a', 's', 0x01, 0x04,
      0x93, 0xA2, 'b', 's', 0x02, 0x04,
    0x01,
      0x80,
    0x2B, 0x94,
      0x83, 0x00, 0xA3, 'H', 'o', 'w', 0x01, 0x00, 0x02, 0xC3,
      0x83, 0x00, 0xA3, 'n', 'o', 'w', 0x01, 0x01, 0x02, 0xC2,
      0x83, 0x00, 0xA5, 'b', 'r', 'o', 'w', 'n', 0x01, 0x02, 0x02, 0xC3,
      0x83, 0x00, 0xA3, 'c', 'o', 'w', 0x01, 0x03, 0x02, 0xC2,
    0x39, 0x94,
      0x83, 0x00, 0xA3, 'H', 'o', 'w', 0x01, 0xA3, 'H', 'O', 'W', 0x02, 0xC3,
      0x83, 0x00, 0xA3, 'n', 'o', 'w', 0x01, 0xA3, 'N', 'O', 'W', 0x02, 0xC2,
      0x83, 0x00, 0xA5, 'b', 'r', 'o', 'w', 'n', 0x01, 0xA5, 'B', 'R', 'O', 'W', 'N', 0x02, 0xC2,
      0x83, 0x00, 0xA3, 'c', 'o', 'w', 0x01, 0xA3, 'C', 'O', 'W', 0x02, 0xC2,
  };
  std::stringstream stream0, stream1;

  DocA doc0;
  DocA::Schema schema0;
  dr::Writer writer0(stream0, schema0);

  doc0.as.create(NWORDS);
  for (uint8_t i = 0; i != NWORDS; ++i) {
    A &a = doc0.as[i];
    a.v_str = WORDS[i];
    a.v_uint8 = i;
    a.v_bool = i % 2 == 0;
  }
  writer0 << doc0;

  BOOST_CHECK( compare_bytes(stream0.str(), stream0_expected, sizeof(stream0_expected)/sizeof(uint8_t)) );

  DocB::Schema schema1;
  dr::Reader reader1(stream0, schema1);
  dr::Writer writer1(stream1, schema1);

  unsigned int ndocs_read1 = 0;
  while (true) {
    DocB doc1;
    if (!(reader1 >> doc1))
      break;
    ++ndocs_read1;

    // check annotation values
    BOOST_REQUIRE_EQUAL(doc1.bs.size(), 0);

    // modify some attributes, including lazy ones
    doc1.bs.create(NWORDS);
    for (uint8_t i = 0; i != NWORDS; ++i) {
      B &b = doc1.bs[i];
      b.word = WORDS[i];
      b.is_first = i == 0;
      for (auto &c : b.word)
        b.upper.push_back(std::toupper(c));
    }

    writer1 << doc1;
  }
  BOOST_CHECK_EQUAL(ndocs_read1, 1);

  BOOST_CHECK( compare_bytes(stream1.str(), stream1_expected, sizeof(stream1_expected)/sizeof(uint8_t)) );

  DocAB::Schema schema2;
  dr::Reader reader2(stream1, schema2);

  unsigned int ndocs_read2 = 0;
  while (true) {
    DocAB doc2;
    if (!(reader2 >> doc2))
      break;
    ++ndocs_read2;

    // check annotation values
    BOOST_REQUIRE_EQUAL(doc2.as.size(), NWORDS);
    BOOST_REQUIRE_EQUAL(doc2.bs.size(), NWORDS);
    for (uint8_t i = 0; i != NWORDS; ++i) {
      A &a = doc2.as[i];
      BOOST_CHECK_EQUAL(a.v_str, WORDS[i]);
      BOOST_CHECK_EQUAL(a.v_uint8, i);
      BOOST_CHECK_EQUAL(a.v_bool, i % 2 == 0);

      B &b = doc2.bs[i];
      BOOST_CHECK_EQUAL(b.word, WORDS[i]);
      BOOST_CHECK_EQUAL(b.upper.size(), b.word.size());
      BOOST_CHECK_EQUAL(b.is_first, i == 0);
    }
  }
  BOOST_CHECK_EQUAL(ndocs_read2, 1);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
