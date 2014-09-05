/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/dr.h>


namespace schwa {
namespace dr {

SUITE(schwa__dr__lazy) {

TEST(lazy_test0) {
  class A : public Ann {
  public:
    std::string v_str;
    uint8_t v_uint8;
    bool v_bool;

    A(void) : Ann(), v_uint8(0), v_bool(false) { }

    class Schema;
  };


  class B : public Ann {
  public:
    std::string word;
    std::string upper;
    bool is_first;

    B(void) : Ann(), is_first(false) { }

    class Schema;
  };


  class DocA : public Doc {
  public:
    Store<A> as;

    class Schema;
  };


  class DocB : public Doc {
  public:
    Store<B> bs;

    class Schema;
  };


  class DocAB : public Doc {
  public:
    Store<A> as;
    Store<B> bs;

    class Schema;
  };


  class A::Schema : public Ann::Schema<A> {
  public:
    DR_FIELD(&A::v_str) v_str;
    DR_FIELD(&A::v_uint8) v_uint8;
    DR_FIELD(&A::v_bool) v_bool;

    Schema(void) :
      Ann::Schema<A>("A", "Some text about A"),
      v_str(*this, "v_str", "some text about v_str", FieldMode::READ_WRITE),
      v_uint8(*this, "v_uint8", "some text about v_uint8", FieldMode::READ_WRITE),
      v_bool(*this, "v_bool", "some text about v_bool", FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class B::Schema : public Ann::Schema<B> {
  public:
    DR_FIELD(&B::word) word;
    DR_FIELD(&B::upper) upper;
    DR_FIELD(&B::is_first) is_first;

    Schema(void) :
      Ann::Schema<B>("B", "Some text about B"),
      word(*this, "word", "some text about word", FieldMode::READ_ONLY),
      upper(*this, "upper", "some text about upper", FieldMode::READ_WRITE),
      is_first(*this, "is_first", "some text about is_first", FieldMode::READ_ONLY)
      { }
    virtual ~Schema(void) { }
  };


  class DocA::Schema : public Doc::Schema<DocA> {
  public:
    DR_STORE(&DocA::as) as;

    Schema(void) :
      Doc::Schema<DocA>("DocA", "Some text about DocA"),
      as(*this, "as", "some text about as", FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class DocB::Schema : public Doc::Schema<DocB> {
  public:
    DR_STORE(&DocB::bs) bs;

    Schema(void) :
      Doc::Schema<DocB>("DocB", "Some text about DocB"),
      bs(*this, "bs", "some text about bs", FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class DocAB::Schema : public Doc::Schema<DocAB> {
  public:
    DR_STORE(&DocAB::as) as;
    DR_STORE(&DocAB::bs) bs;

    Schema(void) :
      Doc::Schema<DocAB>("DocAB", "Some text about DocAB"),
      as(*this, "as", "some text about as", FieldMode::READ_ONLY),
      bs(*this, "bs", "some text about bs", FieldMode::READ_ONLY)
      { }
    virtual ~Schema(void) { }
  };
  static const char *WORDS[] = {"The", "quick", "brown", "fox", "jumped"};
  static constexpr size_t NWORDS = sizeof(WORDS)/sizeof(char *);
  static constexpr uint8_t stream0_expected[] = {
    0x03,
    0x92,
      0x92, 0xA8, '_', '_', 'm', 'e', 't', 'a', '_', '_', 0x90,
      0x92, 0xA1, 'A', 0x93, 0x81, 0x00, 0xA5, 'v', '_', 's', 't', 'r', 0x81, 0x00, 0xA7, 'v', '_', 'u', 'i', 'n', 't', '8', 0x81, 0x00, 0xA6, 'v', '_', 'b', 'o', 'o', 'l',
    0x91,
      0x93, 0xA2, 'a', 's', 0x01, 0x05,
    0x01,
      0x80,
    0x3A, 0x95,
      0x83, 0x00, 0xA3, 'T', 'h', 'e', 0x01, 0x00, 0x02, 0xC3,
      0x83, 0x00, 0xA5, 'q', 'u', 'i', 'c', 'k', 0x01, 0x01, 0x02, 0xC2,
      0x83, 0x00, 0xA5, 'b', 'r', 'o', 'w', 'n', 0x01, 0x02, 0x02, 0xC2,
      0x83, 0x00, 0xA3, 'f', 'o', 'x', 0x01, 0x03, 0x02, 0xC2,
      0x83, 0x00, 0xA6, 'j', 'u', 'm', 'p', 'e', 'd', 0x01, 0x04, 0x02, 0xC2,
  };
  static constexpr uint8_t stream1_expected[] = {
    0x03,
    0x92,
      0x92, 0xA8, '_', '_', 'm', 'e', 't', 'a', '_', '_', 0x90,
      0x92, 0xA1, 'B', 0x94, 0x81, 0x00, 0xA4, 'w', 'o', 'r', 'd', 0x81, 0x00, 0xA7, 'v', '_', 'u', 'i', 'n', 't', '8', 0x81, 0x00, 0xA8, 'i', 's', '_', 'f', 'i', 'r', 's', 't', 0x81, 0x00, 0xA5, 'u', 'p', 'p', 'e', 'r',
    0x91,
      0x93, 0xA2, 'b', 's', 0x01, 0x05,
    0x01,
      0x80,
    0x5A, 0x95,
      0x84, 0x00, 0xA3, 'T', 'h', 'e', 0x01, 0x00, 0x02, 0xC3, 0x03, 0xA3, 'T', 'H', 'E',
      0x84, 0x00, 0xA5, 'q', 'u', 'i', 'c', 'k', 0x01, 0x01, 0x02, 0xC2, 0x03, 0xA5, 'Q', 'U', 'I', 'C', 'K',
      0x84, 0x00, 0xA5, 'b', 'r', 'o', 'w', 'n', 0x01, 0x02, 0x02, 0xC2, 0x03, 0xA5, 'B', 'R', 'O', 'W', 'N',
      0x84, 0x00, 0xA3, 'f', 'o', 'x', 0x01, 0x03, 0x02, 0xC2, 0x03, 0xA3, 'F', 'O', 'X',
      0x84, 0x00, 0xA6, 'j', 'u', 'm', 'p', 'e', 'd', 0x01, 0x04, 0x02, 0xC2, 0x03, 0xA6, 'J', 'U', 'M', 'P', 'E', 'D',
  };
  std::stringstream stream0, stream1;

  DocA doc0;
  DocA::Schema schema0;
  Writer writer0(stream0, schema0);

  doc0.as.create(NWORDS);
  for (uint8_t i = 0; i != NWORDS; ++i) {
    A &a = doc0.as[i];
    a.v_str = WORDS[i];
    a.v_uint8 = i;
    a.v_bool = i == 0;
  }
  writer0 << doc0;

  CHECK_COMPARE_BYTES3(stream0_expected, sizeof(stream0_expected)/sizeof(uint8_t), stream0.str());

  DocB::Schema schema1_in, schema1_out;
  schema1_in.bs.serial = "as";
  schema1_in.types<B>().serial = "A";
  schema1_in.types<B>().word.serial = "v_str";
  schema1_in.types<B>().is_first.serial = "v_bool";
  Reader reader1(stream0, schema1_in);
  Writer writer1(stream1, schema1_out);

  unsigned int ndocs_read1 = 0;
  while (true) {
    DocB doc1;
    if (!(reader1 >> doc1))
      break;
    ++ndocs_read1;

    // check annotation values
    CHECK_EQUAL(NWORDS, doc1.bs.size());
    for (uint8_t i = 0; i != NWORDS; ++i) {
      B &b = doc1.bs[i];
      CHECK_EQUAL(WORDS[i], b.word);
      CHECK(b.upper.empty());
      CHECK_EQUAL(i == 0, b.is_first);
    }

    // check document lazy
    CHECK_EQUAL(static_cast<const void *>(nullptr), static_cast<const void *>(doc1.lazy_data()));
    CHECK_EQUAL(0, doc1.lazy_nbytes());
    CHECK_EQUAL(0, doc1.lazy_nelem());

    // check annotation lazy
    for (uint8_t i = 0; i != NWORDS; ++i) {
      B &b = doc1.bs[i];
      CHECK(static_cast<const void *>(b.lazy_data()) != static_cast<const void *>(nullptr));
      CHECK_EQUAL(3, b.lazy_nelem());
      CHECK_EQUAL(3 + (b.word.size() + 1) + 1 + 1, b.lazy_nbytes());
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
  CHECK_EQUAL(1, ndocs_read1);

  CHECK_COMPARE_BYTES3(stream1_expected, sizeof(stream1_expected)/sizeof(uint8_t), stream1.str());

  DocA::Schema schema2;
  schema2.as.serial = "bs";
  schema2.types<A>().serial = "B";
  schema2.types<A>().v_str.serial = "word";
  schema2.types<A>().v_bool.serial = "is_first";
  Reader reader2(stream1, schema2);

  unsigned int ndocs_read2 = 0;
  while (true) {
    DocA doc2;
    if (!(reader2 >> doc2))
      break;
    ++ndocs_read2;

    // check annotation values
    CHECK_EQUAL(NWORDS, doc2.as.size());
    for (uint8_t i = 0; i != NWORDS; ++i) {
      A &a = doc2.as[i];
      CHECK_EQUAL(WORDS[i], a.v_str);
      CHECK_EQUAL(i, a.v_uint8);
      CHECK_EQUAL(i == 0, a.v_bool);
    }

    // check document lazy
    CHECK_EQUAL(static_cast<const void *>(nullptr), static_cast<const void *>(doc2.lazy_data()));
    CHECK_EQUAL(0, doc2.lazy_nbytes());
    CHECK_EQUAL(0, doc2.lazy_nelem());

    // check annotation lazy
    for (uint8_t i = 0; i != NWORDS; ++i) {
      A &a = doc2.as[i];
      CHECK(static_cast<const void *>(a.lazy_data()) != static_cast<const void *>(nullptr));
      CHECK_EQUAL(1, a.lazy_nelem());
      CHECK_EQUAL(1 + (a.v_str.size() + 1), a.lazy_nbytes());
    }

  }
  CHECK_EQUAL(1, ndocs_read2);
}


TEST(lazy_test1) {
  class A : public Ann {
  public:
    std::string v_str;
    uint8_t v_uint8;
    bool v_bool;

    A(void) : Ann(), v_uint8(0), v_bool(false) { }

    class Schema;
  };


  class B : public Ann {
  public:
    std::string word;
    std::string upper;
    bool is_first;

    B(void) : Ann(), is_first(false) { }

    class Schema;
  };


  class DocA : public Doc {
  public:
    Store<A> as;

    class Schema;
  };


  class DocB : public Doc {
  public:
    Store<B> bs;

    class Schema;
  };


  class DocAB : public Doc {
  public:
    Store<A> as;
    Store<B> bs;

    class Schema;
  };


  class A::Schema : public Ann::Schema<A> {
  public:
    DR_FIELD(&A::v_str) v_str;
    DR_FIELD(&A::v_uint8) v_uint8;
    DR_FIELD(&A::v_bool) v_bool;

    Schema(void) :
      Ann::Schema<A>("A", "Some text about A"),
      v_str(*this, "v_str", "some text about v_str", FieldMode::READ_WRITE),
      v_uint8(*this, "v_uint8", "some text about v_uint8", FieldMode::READ_WRITE),
      v_bool(*this, "v_bool", "some text about v_bool", FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class B::Schema : public Ann::Schema<B> {
  public:
    DR_FIELD(&B::word) word;
    DR_FIELD(&B::upper) upper;
    DR_FIELD(&B::is_first) is_first;

    Schema(void) :
      Ann::Schema<B>("B", "Some text about B"),
      word(*this, "word", "some text about word", FieldMode::READ_WRITE),
      upper(*this, "upper", "some text about upper", FieldMode::READ_WRITE),
      is_first(*this, "is_first", "some text about is_first", FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class DocA::Schema : public Doc::Schema<DocA> {
  public:
    DR_STORE(&DocA::as) as;

    Schema(void) :
      Doc::Schema<DocA>("DocA", "Some text about DocA"),
      as(*this, "as", "some text about as", FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class DocB::Schema : public Doc::Schema<DocB> {
  public:
    DR_STORE(&DocB::bs) bs;

    Schema(void) :
      Doc::Schema<DocB>("DocB", "Some text about DocB"),
      bs(*this, "bs", "some text about bs", FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class DocAB::Schema : public Doc::Schema<DocAB> {
  public:
    DR_STORE(&DocAB::as) as;
    DR_STORE(&DocAB::bs) bs;

    Schema(void) :
      Doc::Schema<DocAB>("DocAB", "Some text about DocAB"),
      as(*this, "as", "some text about as", FieldMode::READ_ONLY),
      bs(*this, "bs", "some text about bs", FieldMode::READ_ONLY)
      { }
    virtual ~Schema(void) { }
  };
  static const char *WORDS[] = {"How", "now", "brown", "cow"};
  static constexpr size_t NWORDS = sizeof(WORDS)/sizeof(char *);
  static constexpr uint8_t stream0_expected[] = {
    0x03,
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
    0x03,
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
  Writer writer0(stream0, schema0);

  doc0.as.create(NWORDS);
  for (uint8_t i = 0; i != NWORDS; ++i) {
    A &a = doc0.as[i];
    a.v_str = WORDS[i];
    a.v_uint8 = i;
    a.v_bool = i % 2 == 0;
  }
  writer0 << doc0;

  CHECK_COMPARE_BYTES3(stream0_expected, sizeof(stream0_expected)/sizeof(uint8_t), stream0.str());

  DocB::Schema schema1;
  Reader reader1(stream0, schema1);
  Writer writer1(stream1, schema1);

  unsigned int ndocs_read1 = 0;
  while (true) {
    DocB doc1;
    if (!(reader1 >> doc1))
      break;
    ++ndocs_read1;

    // check annotation values
    CHECK_EQUAL(0, doc1.bs.size());

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
  CHECK_EQUAL(1, ndocs_read1);

  CHECK_COMPARE_BYTES3(stream1_expected, sizeof(stream1_expected)/sizeof(uint8_t), stream1.str());

  DocAB::Schema schema2;
  Reader reader2(stream1, schema2);

  unsigned int ndocs_read2 = 0;
  while (true) {
    DocAB doc2;
    if (!(reader2 >> doc2))
      break;
    ++ndocs_read2;

    // check annotation values
    CHECK_EQUAL(NWORDS, doc2.as.size());
    CHECK_EQUAL(NWORDS, doc2.bs.size());
    for (uint8_t i = 0; i != NWORDS; ++i) {
      A &a = doc2.as[i];
      CHECK_EQUAL(WORDS[i], a.v_str);
      CHECK_EQUAL(i, a.v_uint8);
      CHECK_EQUAL(i % 2 == 0, a.v_bool);

      B &b = doc2.bs[i];
      CHECK_EQUAL(WORDS[i], b.word);
      CHECK_EQUAL(b.word.size(), b.upper.size());
      CHECK_EQUAL(i == 0, b.is_first);
    }
  }
  CHECK_EQUAL(1, ndocs_read2);
}


TEST(lazy_test_pointer_to_0) {
  std::stringstream stream;
  stream << '\x03'; // version
  stream << '\x92'; // <klasses>: 2-element array
    stream << '\x92'; // <klass>
      stream << '\xA8' << "__meta__"; // <klass_name>
      stream << '\x91'; // <fields>
        stream << '\x82'; // 2 attributes
        stream << '\x00' << '\xA6' << "as_ptr"; // name
        stream << '\x01' << '\x00'; // ptr to store 0
    stream << '\x92'; // <klass>
      stream << '\xA1' << 'A'; // <klass_name>
      stream << '\x90'; // <fields>
  stream << '\x91'; // <stores>: 1-element array
    stream << '\x93' << '\xA2' << "as" << '\x01' << '\x00'; // 0-element store
  stream << "\x01\x80"; // Empty document
  stream << "\x01\x90"; // Empty store

  std::stringstream output;
  FauxDoc doc;
  FauxDoc::Schema schema;
  Reader reader(stream, schema);
  reader >> doc;
  Writer writer(output, schema);
  writer << doc;

  CHECK_COMPARE_BYTES2(stream.str(), output.str());
}


TEST(lazy_version_upgrade) {
  std::stringstream input, output;
  input << '\x02'; // version
  input << '\x92'; // <klasses>: 2-element array
    input << '\x92'; // <klass>
      input << '\xA8' << "__meta__"; // <klass_name>
      input << '\x91'; // <fields>
        input << '\x82'; // 2 attributes
        input << '\x00' << '\xA6' << "as_ptr"; // name
        input << '\x01' << '\x00'; // ptr to store 0
    input << '\x92'; // <klass>
      input << '\xA1' << 'A'; // <klass_name>
      input << '\x90'; // <fields>
  input << '\x91'; // <stores>: 1-element array
    input << '\x93' << '\xA2' << "as" << '\x01' << '\x00'; // 0-element store
  input << "\x01\x80"; // Empty document
  input << "\x01\x90"; // Empty store

  output << '\x03'; // version
  output << '\x92'; // <klasses>: 2-element array
    output << '\x92'; // <klass>
      output << '\xA8' << "__meta__"; // <klass_name>
      output << '\x91'; // <fields>
        output << '\x82'; // 2 attributes
        output << '\x00' << '\xA6' << "as_ptr"; // name
        output << '\x01' << '\x00'; // ptr to store 0
    output << '\x92'; // <klass>
      output << '\xA1' << 'A'; // <klass_name>
      output << '\x90'; // <fields>
  output << '\x91'; // <stores>: 1-element array
    output << '\x93' << '\xA2' << "as" << '\x01' << '\x00'; // 0-element store
  output << "\x01\x80"; // Empty document
  output << "\x01\x90"; // Empty store

  std::stringstream tmp;
  FauxDoc doc;
  FauxDoc::Schema schema;
  Reader reader(input, schema);
  reader >> doc;
  Writer writer(tmp, schema);
  writer << doc;

  CHECK_COMPARE_BYTES2(output.str(), tmp.str());
}

}  // SUITE

}  // namespace dr
}  // namespace schwa
