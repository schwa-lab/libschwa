/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "test_utils.h"

#include <schwa/dr.h>

namespace dr = schwa::dr;


namespace schwatest {

SUITE(schwa__dr__self_pointer) {

TEST(self_pointer0) {
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

  std::stringstream stream, correct;
  Doc::Schema schema;
  dr::Writer writer(stream, schema);
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

  Doc doc1;
  dr::Reader reader(stream, schema);
  reader >> doc1;

  CHECK_EQUAL(3, doc1.xs1.size());
  CHECK_EQUAL(2, doc1.xs2.size());
  CHECK_EQUAL((void *)nullptr, doc1.xs1[0].parent.ptr);
  CHECK_EQUAL( &doc1.xs2[0], doc1.xs1[0].other.ptr);
  CHECK_EQUAL(&doc1.xs1[0], doc1.xs1[1].parent.ptr);
  CHECK_EQUAL( &doc1.xs2[0], doc1.xs1[1].other.ptr);
  CHECK_EQUAL(&doc1.xs1[1], doc1.xs1[2].parent.ptr);
  CHECK_EQUAL( &doc1.xs2[0], doc1.xs1[2].other.ptr);
  CHECK_EQUAL((void *)nullptr, doc1.xs2[0].parent.ptr);
  CHECK_EQUAL( &doc1.xs2[0], doc1.xs2[0].other.ptr);
  CHECK_EQUAL(&doc1.xs2[0], doc1.xs2[1].parent.ptr);
  CHECK_EQUAL( &doc1.xs2[0], doc1.xs2[1].other.ptr);
}

}  // SUITE

}  // namespace schwatest
