/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>
#include "test_utils.h"

#include <boost/test/unit_test.hpp>

namespace dr = schwa::dr;
namespace mp = schwa::msgpack;


// ============================================================================
// ============================================================================
class DocWithField : public dr::Doc {
public:
  std::string name;

  class Schema;
};

class DocWithField::Schema : public dr::Doc::Schema<DocWithField> {
public:
  DR_FIELD(&DocWithField::name) name;

  Schema(void) :
    dr::Doc::Schema<DocWithField>("Doc", "Some help text about this Doc class"),
    name(*this, "name", "some help text about name", dr::FieldMode::READ_WRITE, "name")
    { }
  virtual ~Schema(void) { }
};


// ============================================================================
// ============================================================================
class DocWithFieldWithSerial : public dr::Doc {
public:
  std::string name;

  class Schema;
};

class DocWithFieldWithSerial::Schema : public dr::Doc::Schema<DocWithFieldWithSerial> {
public:
  DR_FIELD(&DocWithFieldWithSerial::name) name;

  Schema(void) :
    dr::Doc::Schema<DocWithFieldWithSerial>("Doc", "Some help text about this Doc class"),
    name(*this, "name", "some help text about name", dr::FieldMode::READ_WRITE, "filename")
    { }
  virtual ~Schema(void) { }
};


// ============================================================================
// ============================================================================
class A : public dr::Ann {
public:
  std::string v_str;
  uint8_t v_uint8;
  bool v_bool;

  A(void) : v_uint8(0), v_bool(false) { }

  class Schema;
};

class Y : public dr::Ann {
public:
  dr::Pointer<A> p;

  class Schema;
};

class Z : public dr::Ann {
public:
  dr::Pointer<A> p;
  bool value;

  Z(void) : value(false) { }

  class Schema;
};

class DocWithA : public dr::Doc {
public:
  dr::Store<A> as;

  class Schema;
};

class DocWithAYZ : public dr::Doc {
public:
  dr::Store<A> as;
  dr::Store<Y> ys;
  dr::Store<Z> zs;

  class Schema;
};

class DocWithA::Schema : public dr::Doc::Schema<DocWithA> {
public:
  DR_STORE(&DocWithA::as) as;

  Schema(void) :
    dr::Doc::Schema<DocWithA>("Doc", "Some help text about this Doc class"),
    as(*this, "as", "some help text about as", dr::FieldMode::READ_WRITE, "as")
    { }
  virtual ~Schema(void) { }
};

class DocWithAYZ::Schema : public dr::Doc::Schema<DocWithAYZ> {
public:
  DR_STORE(&DocWithAYZ::as) as;
  DR_STORE(&DocWithAYZ::ys) ys;
  DR_STORE(&DocWithAYZ::zs) zs;

  Schema(void) :
    dr::Doc::Schema<DocWithAYZ>("Doc", "Some help text about this Doc class"),
    as(*this, "as", "some help text about as", dr::FieldMode::READ_WRITE, "as"),
    ys(*this, "ys", "some help text about ys", dr::FieldMode::READ_WRITE, "ys"),
    zs(*this, "zs", "some help text about zs", dr::FieldMode::READ_WRITE, "zs")
    { }
  virtual ~Schema(void) { }
};

class A::Schema : public dr::Ann::Schema<A> {
public:
  DR_FIELD(&A::v_str) v_str;
  DR_FIELD(&A::v_uint8) v_uint8;
  DR_FIELD(&A::v_bool) v_bool;

  Schema(void) :
    dr::Ann::Schema<A>("A", "Some help text about A", "A"),
    v_str(*this, "v_str", "some help text about v_str", dr::FieldMode::READ_WRITE, "v_str"),
    v_uint8(*this, "v_uint8", "some help text about v_uint8", dr::FieldMode::READ_WRITE, "v_uint8"),
    v_bool(*this, "v_bool", "some help text about v_bool", dr::FieldMode::READ_WRITE, "v_bool")
    { }
  virtual ~Schema(void) { }
};

class Y::Schema : public dr::Ann::Schema<Y> {
public:
  DR_POINTER(&Y::p, &DocWithAYZ::as) p;

  Schema(void) :
    dr::Ann::Schema<Y>("Y", "Some help text about Y", "Y"),
    p(*this, "p", "some help text about p", dr::FieldMode::READ_WRITE, "p")
    { }
  virtual ~Schema(void) { }
};

class Z::Schema : public dr::Ann::Schema<Z> {
public:
  DR_POINTER(&Z::p, &DocWithAYZ::as) p;
  DR_FIELD(&Z::value) value;

  Schema(void) :
    dr::Ann::Schema<Z>("Z", "Some help text about Z", "Z"),
    p(*this, "p", "some help text about p", dr::FieldMode::READ_WRITE, "p"),
    value(*this, "value", "some help text about value", dr::FieldMode::READ_WRITE, "value")
    { }
  virtual ~Schema(void) { }
};


// ============================================================================
// ============================================================================


BOOST_AUTO_TEST_SUITE(schwa_dr_reader)

BOOST_AUTO_TEST_CASE(DocWithField__name_is_null) {
  std::stringstream correct;
  correct << '\x02';  // <wire_version>
  correct << '\x91';  // <klasses>: 1-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "__meta__";  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
  correct << '\x91';  // <fields>: 1-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa4' << "name";  // 4-bytes of utf-8 encoded "name"
  correct << '\x90';  // <stores>: 0-element array
  correct << '\x01';  // <instance_nbytes>: 1 byte after this
  correct << '\x80';  // <instance>: 0-element map

  DocWithField *doc = nullptr;
  DocWithField::Schema schema;
  dr::Reader reader(correct, schema);

  doc = new DocWithField();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), true);
  delete doc;

  doc = new DocWithField();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), false);
  delete doc;
}


BOOST_AUTO_TEST_CASE(DocWithField__name) {
  std::stringstream correct;
  correct << '\x02';  // <wire_version>
  correct << '\x91';  // <klasses>: 1-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "__meta__";  // <klass_name>: utf-8 encoded "__meta__"
  correct << '\x91';  // <fields>: 1-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa4' << "name";  // utf-8 encoded "name"
  correct << '\x90';  // <stores>: 0-element array
  correct << '\x0e';  // <instance_nbytes>: 14 bytes after this
  correct << '\x81';  // <instance>: 1-element map
  correct << '\x00';  // 0: field number 0 (=> name;
  correct << '\xab' << "/etc/passwd";  // utf-8 encoded "/etc/passwd"

  DocWithField *doc = nullptr;
  DocWithField::Schema schema;
  dr::Reader reader(correct, schema);

  doc = new DocWithField();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), true);
  BOOST_CHECK_EQUAL(doc->name, "/etc/passwd");
  delete doc;

  doc = new DocWithField();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), false);
  delete doc;
}


BOOST_AUTO_TEST_CASE(DocWithFieldWithSerial__name_is_null) {
  std::stringstream correct;
  correct << '\x02';  // <wire_version>
  correct << '\x91';  // <klasses>: 1-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "__meta__";  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
  correct << '\x91';  // <fields>: 1-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa8' << "filename";  // utf-8 encoded "filename"
  correct << '\x90';  // <stores>: 0-element array
  correct << '\x01';  // <instance_nbytes>: 1 byte after this
  correct << '\x80';  // <instance>: 0-element map

  DocWithFieldWithSerial *doc = nullptr;
  DocWithFieldWithSerial::Schema schema;
  dr::Reader reader(correct, schema);

  doc = new DocWithFieldWithSerial();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), true);
  delete doc;

  doc = new DocWithFieldWithSerial();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), false);
  delete doc;
}


BOOST_AUTO_TEST_CASE(DocWithFieldWithSerial__name) {
  std::stringstream correct;
  correct << '\x02';  // <wire_version>
  correct << '\x91';  // <klasses>: 1-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "__meta__";  // <klass_name>: utf-8 encoded "__meta__"
  correct << '\x91';  // <fields>: 1-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa8' << "filename";  // utf-8 encoded "filename"
  correct << '\x90';  // <stores>: 0-element array
  correct << '\x0e';  // <instance_nbytes>: 14 bytes after this
  correct << '\x81';  // <instance>: 1-element map
  correct << '\x00';  // 0: field number 0 (=> name;
  correct << '\xab' << "/etc/passwd";  // utf-8 encoded "/etc/passwd"

  DocWithFieldWithSerial *doc = nullptr;
  DocWithFieldWithSerial::Schema schema;
  dr::Reader reader(correct, schema);

  doc = new DocWithFieldWithSerial();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), true);
  BOOST_CHECK_EQUAL(doc->name, "/etc/passwd");
  delete doc;

  doc = new DocWithFieldWithSerial();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), false);
  delete doc;
}


BOOST_AUTO_TEST_CASE(DocWithA__empty) {
  std::stringstream correct;
  correct << '\x02';  // <wire_version>
  correct << '\x92';  // <klasses>: 2-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "__meta__";  // <klass_name>: utf-8 encoded "__meta__"
  correct << '\x90';  // <fields>: 0-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "writer.A";  // <klass_name>: utf-8 encoded "writer.A"
  correct << '\x93';  // <fields>: 3-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa5' << "v_str";  // utf-8 encoded "v_str"
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa7' << "v_uint8";  // utf-8 encoded "v_uint8"
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa6' << "v_bool";  // utf-8 encoded "v_bool"
  correct << '\x91';  // <stores>: 1-element array
  correct << '\x93';  // <store>: 3-element array
  correct << '\xa2' << "as";  // <store_name>: utf-8 encoded "as"
  correct << '\x01';  // <klass_id>: 1
  correct << '\x00';  // <store_nelem>: 0
  correct << '\x01';  // <instance_nbytes>: 1 byte after this for the document
  correct << '\x80';  // <instance>: 0-element map
  correct << '\x01';  // <instance_nbytes>: 1 byte after this for the "as" store
  correct << '\x90';  // <instance>: 0-element array

  DocWithA::Schema schema;
  schema.types<A>().serial = "writer.A";

  DocWithA *doc = nullptr;
  dr::Reader reader(correct, schema);

  doc = new DocWithA();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), true);
  delete doc;

  doc = new DocWithA();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), false);
  delete doc;
}


BOOST_AUTO_TEST_CASE(DocWithA__four_elements) {
  std::stringstream correct;
  correct << '\x02';  // <wire_version>
  correct << '\x92';  // <klasses>: 2-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "__meta__";  // <klass_name>: utf-8 encoded "__meta__"
  correct << '\x90';  // <fields>: 0-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "writer.A";  // <klass_name>: utf-8 encoded "writer.A"
  correct << '\x93';  // <fields>: 3-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa5' << "v_str";  // utf-8 encoded "v_str"
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa7' << "v_uint8";  // utf-8 encoded "v_uint8"
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa6' << "v_bool";  // utf-8 encoded "v_bool"
  correct << '\x91';  // <stores>: 1-element array
  correct << '\x93';  // <store>: 3-element array
  correct << '\xa2' << "as";  // <store_name>: utf-8 encoded "as"
  correct << '\x01';  // <klass_id>: 1
  correct << '\x04';  // <store_nelem>: 4
  correct << '\x01';  // <instance_nbytes>: 1 byte after this for the document
  correct << '\x80';  // <instance>: 0-element map
  correct << '\x1c';  // <instance_nbytes>: 28 byte after this for the "as" store
  correct << '\x94';  // <instance>: 4-element array
  correct << '\x83' << '\x00' << '\xa5' << "first" << '\x01' << '\x00' << '\x02' << '\xc2'; // {0: 'first', 1: 0, 2: false}
  correct << '\x82' << '\x01' << '\x02' << '\x02' << '\xc2'; // {1: 2, 2: false}
  correct << '\x82' << '\x01' << '\x00' << '\x02' << '\xc2'; // {1: 0, 2: false}
  correct << '\x82' << '\x01' << '\x00' << '\x02' << '\xc3'; // {1: 0, 2: true}

  DocWithA::Schema schema;
  schema.types<A>().serial = "writer.A";

  DocWithA *doc = nullptr;
  dr::Reader reader(correct, schema);

  doc = new DocWithA();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), true);
  BOOST_CHECK_EQUAL(doc->as.size(), 4);
  BOOST_CHECK_EQUAL(doc->as[0].v_str, "first");
  BOOST_CHECK_EQUAL(doc->as[0].v_uint8, 0);
  BOOST_CHECK_EQUAL(doc->as[0].v_bool, false);
  BOOST_CHECK_EQUAL(doc->as[1].v_str, "");
  BOOST_CHECK_EQUAL(doc->as[1].v_uint8, 2);
  BOOST_CHECK_EQUAL(doc->as[1].v_bool, false);
  BOOST_CHECK_EQUAL(doc->as[2].v_str, "");
  BOOST_CHECK_EQUAL(doc->as[2].v_uint8, 0);
  BOOST_CHECK_EQUAL(doc->as[2].v_bool, false);
  BOOST_CHECK_EQUAL(doc->as[3].v_str, "");
  BOOST_CHECK_EQUAL(doc->as[3].v_uint8, 0);
  BOOST_CHECK_EQUAL(doc->as[3].v_bool, true);
  delete doc;

  doc = new DocWithA();
  reader >> *doc;
  BOOST_CHECK_EQUAL(static_cast<bool>(reader), false);
  delete doc;
}

BOOST_AUTO_TEST_SUITE_END()
