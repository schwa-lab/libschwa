/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/dr.h>


namespace schwa {
namespace dr {

SUITE(schwa__dr__self_pointer) {

TEST(self_pointer0) {
  class X : public Ann {
  public:
    Pointer<X> parent;
    Pointer<X> other;

    class Schema;
  };


  class TestDoc : public Doc {
  public:
    Store<X> xs1;
    Store<X> xs2;

    class Schema;
  };


  class X::Schema : public Ann::Schema<X> {
  public:
    DR_SELF(&X::parent) parent;
    DR_POINTER(&X::other, &TestDoc::xs2) other;

    Schema(void) :
      Ann::Schema<X>("X", "Some text about X"),
      parent(*this, "parent", "parent", FieldMode::READ_WRITE),
      other(*this, "other", "other", FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  class TestDoc::Schema : public Doc::Schema<TestDoc> {
  public:
    DR_STORE(&TestDoc::xs1) xs1;
    DR_STORE(&TestDoc::xs2) xs2;

    Schema(void) :
      Doc::Schema<TestDoc>("TestDoc", "Some text about TestDoc"),
      xs1(*this, "xs1", "some text about xs1", FieldMode::READ_WRITE),
      xs2(*this, "xs2", "some text about xs2", FieldMode::READ_WRITE)
      { }
    virtual ~Schema(void) { }
  };


  TestDoc doc0;
  doc0.xs1.create(3);
  doc0.xs2.create(2);

  doc0.xs1[1].parent = &doc0.xs1[0];
  doc0.xs1[2].parent = &doc0.xs1[1];
  doc0.xs2[1].parent = &doc0.xs2[0];

  doc0.xs1[0].other = &doc0.xs2[0];
  doc0.xs1[1].other = &doc0.xs2[0];
  doc0.xs1[2].other = &doc0.xs2[0];
  doc0.xs2[0].other = &doc0.xs2[0];
  doc0.xs2[1].other = &doc0.xs2[0];

  std::stringstream stream, correct;
  TestDoc::Schema schema;
  Writer writer(stream, schema);
  writer << doc0;

  correct << '\x02';
  correct << '\x92';  // <klasses>: 2-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "__meta__";  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
  correct << '\x90';  // <fields>: 0-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa1' << "X";  // <klass_name>: 8-bytes of utf-8 encoded "X"
  correct << '\x92';  // <fields>: 2-element array
  correct << '\x82';  // <field>: 2-element map
  correct << '\x00' << '\xa6' << "parent";
  correct << '\x03' << '\xc0';
  correct << '\x82';  // <field>: 2-element map
  correct << '\x00' << '\xa5' << "other";
  correct << '\x01' << '\x01';
  correct << '\x92';  // <stores>: 2-element array
  correct << '\x93' << '\xa3' << "xs1" << '\x01' << '\x03';
  correct << '\x93' << '\xa3' << "xs2" << '\x01' << '\x02';
  correct << '\x01';  // <instance_nbytes>: 1 byte after this
  correct << '\x80';  // <instance>: 0-element map
  correct << '\x0e';  // <instance_nbytes>: 1 byte after this
  correct << '\x93';  // <instance>: 3-element array
  correct << '\x81' << '\x01' << '\x00';
  correct << '\x82' << '\x00' << '\x00' << '\x01' << '\x00';
  correct << '\x82' << '\x00' << '\x01' << '\x01' << '\x00';
  correct << '\x09';  // <instance_nbytes>: 1 byte after this
  correct << '\x92';  // <instance>: 2-element array
  correct << '\x81' << '\x01' << '\x00';
  correct << '\x82' << '\x00' << '\x00' << '\x01' << '\x00';

  CHECK_COMPARE_BYTES2(correct.str(), stream.str());

  TestDoc doc1;
  Reader reader(stream, schema);
  reader >> doc1;

  CHECK_EQUAL(3, doc1.xs1.size());
  CHECK_EQUAL(2, doc1.xs2.size());
  CHECK_EQUAL((void *)nullptr, doc1.xs1[0].parent);
  CHECK_EQUAL( &doc1.xs2[0], doc1.xs1[0].other);
  CHECK_EQUAL(&doc1.xs1[0], doc1.xs1[1].parent);
  CHECK_EQUAL( &doc1.xs2[0], doc1.xs1[1].other);
  CHECK_EQUAL(&doc1.xs1[1], doc1.xs1[2].parent);
  CHECK_EQUAL( &doc1.xs2[0], doc1.xs1[2].other);
  CHECK_EQUAL((void *)nullptr, doc1.xs2[0].parent);
  CHECK_EQUAL( &doc1.xs2[0], doc1.xs2[0].other);
  CHECK_EQUAL(&doc1.xs2[0], doc1.xs2[1].parent);
  CHECK_EQUAL( &doc1.xs2[0], doc1.xs2[1].other);
}

}  // SUITE

}  // namespace dr
}  // namespace schwa
