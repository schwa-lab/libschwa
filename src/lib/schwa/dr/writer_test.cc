/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/dr.h>

namespace mp = schwa::msgpack;


namespace schwa {
namespace dr {

namespace {

class DocWithField : public Doc {
public:
  std::string name;

  class Schema;
};

class DocWithField::Schema : public Doc::Schema<DocWithField> {
public:
  DR_FIELD(&DocWithField::name) name;

  Schema(void) :
    Doc::Schema<DocWithField>("Doc", "Some help text about this Doc class"),
    name(*this, "name", "some help text about name", FieldMode::READ_WRITE, "name")
    { }
  virtual ~Schema(void) { }
};


// ============================================================================
// ============================================================================
class DocWithFieldWithSerial : public Doc {
public:
  std::string name;

  class Schema;
};

class DocWithFieldWithSerial::Schema : public Doc::Schema<DocWithFieldWithSerial> {
public:
  DR_FIELD(&DocWithFieldWithSerial::name) name;

  Schema(void) :
    Doc::Schema<DocWithFieldWithSerial>("Doc", "Some help text about this Doc class"),
    name(*this, "name", "some help text about name", FieldMode::READ_WRITE, "filename")
    { }
  virtual ~Schema(void) { }
};


// ============================================================================
// ============================================================================
class A : public Ann {
public:
  std::string v_str;
  uint8_t v_uint8;
  bool v_bool;

  A(void) : v_uint8(0), v_bool(false) { }

  class Schema;
};

class Y : public Ann {
public:
  Pointer<A> p;

  class Schema;
};

class Z : public Ann {
public:
  Pointer<A> p;
  bool value;

  Z(void) : value(false) { }

  class Schema;
};

class DocWithA : public Doc {
public:
  Store<A> as;

  class Schema;
};

class DocWithAYZ : public Doc {
public:
  Store<A> as;
  Store<Y> ys;
  Store<Z> zs;

  class Schema;
};

class DocWithA::Schema : public Doc::Schema<DocWithA> {
public:
  DR_STORE(&DocWithA::as) as;

  Schema(void) :
    Doc::Schema<DocWithA>("Doc", "Some help text about this Doc class"),
    as(*this, "as", "some help text about as", FieldMode::READ_WRITE, "as")
    { }
  virtual ~Schema(void) { }
};

class DocWithAYZ::Schema : public Doc::Schema<DocWithAYZ> {
public:
  DR_STORE(&DocWithAYZ::as) as;
  DR_STORE(&DocWithAYZ::ys) ys;
  DR_STORE(&DocWithAYZ::zs) zs;

  Schema(void) :
    Doc::Schema<DocWithAYZ>("Doc", "Some help text about this Doc class"),
    as(*this, "as", "some help text about as", FieldMode::READ_WRITE, "as"),
    ys(*this, "ys", "some help text about ys", FieldMode::READ_WRITE, "ys"),
    zs(*this, "zs", "some help text about zs", FieldMode::READ_WRITE, "zs")
    { }
  virtual ~Schema(void) { }
};

class A::Schema : public Ann::Schema<A> {
public:
  DR_FIELD(&A::v_str) v_str;
  DR_FIELD(&A::v_uint8) v_uint8;
  DR_FIELD(&A::v_bool) v_bool;

  Schema(void) :
    Ann::Schema<A>("A", "Some help text about A", "A"),
    v_str(*this, "v_str", "some help text about v_str", FieldMode::READ_WRITE, "v_str"),
    v_uint8(*this, "v_uint8", "some help text about v_uint8", FieldMode::READ_WRITE, "v_uint8"),
    v_bool(*this, "v_bool", "some help text about v_bool", FieldMode::READ_WRITE, "v_bool")
    { }
  virtual ~Schema(void) { }
};

class Y::Schema : public Ann::Schema<Y> {
public:
  DR_POINTER(&Y::p, &DocWithAYZ::as) p;

  Schema(void) :
    Ann::Schema<Y>("Y", "Some help text about Y", "Y"),
    p(*this, "p", "some help text about p", FieldMode::READ_WRITE, "p")
    { }
  virtual ~Schema(void) { }
};

class Z::Schema : public Ann::Schema<Z> {
public:
  DR_POINTER(&Z::p, &DocWithAYZ::as) p;
  DR_FIELD(&Z::value) value;

  Schema(void) :
    Ann::Schema<Z>("Z", "Some help text about Z", "Z"),
    p(*this, "p", "some help text about p", FieldMode::READ_WRITE, "p"),
    value(*this, "value", "some help text about value", FieldMode::READ_WRITE, "value")
    { }
  virtual ~Schema(void) { }
};

}  // namespace


// ============================================================================
// ============================================================================

SUITE(schwa__dr__writer) {

TEST(DocWithField__name_is_null) {
  std::stringstream out, correct;
  DocWithField::Schema schema;
  Writer writer(out, schema);

  DocWithField d;
  writer << d;

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

  CHECK_COMPARE_BYTES2(correct.str(), out.str());
}


TEST(DocWithField__name) {
  std::stringstream out, correct;
  DocWithField::Schema schema;
  Writer writer(out, schema);

  DocWithField d;
  d.name = "/etc/passwd";
  writer << d;

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

  CHECK_COMPARE_BYTES2(correct.str(), out.str());
}


TEST(DocWithFieldWithSerial__name_is_null) {
  std::stringstream out, correct;
  DocWithFieldWithSerial::Schema schema;
  Writer writer(out, schema);

  DocWithFieldWithSerial d;
  writer << d;

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

  CHECK_COMPARE_BYTES2(correct.str(), out.str());
}


TEST(DocWithFieldWithSerial__name) {
  std::stringstream out, correct;
  DocWithFieldWithSerial::Schema schema;
  Writer writer(out, schema);

  DocWithFieldWithSerial d;
  d.name = "/etc/passwd";
  writer << d;

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

  CHECK_COMPARE_BYTES2(correct.str(), out.str());
}


TEST(DocWithA__empty) {
  std::stringstream out, correct;
  DocWithA::Schema schema;
  Writer writer(out, schema);

  schema.types<A>().serial = "writer.A";

  DocWithA d;
  writer << d;

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

  CHECK_COMPARE_BYTES2(correct.str(), out.str());
}


TEST(DocWithA__four_elements) {
  std::stringstream out, correct;
  DocWithA::Schema schema;
  Writer writer(out, schema);

  schema.types<A>().serial = "writer.A";

  DocWithA d;
  d.as.create(4);
  d.as[0].v_str = "first";
  d.as[1].v_uint8 = 2;
  d.as[3].v_bool = true;
  writer << d;

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

  CHECK_COMPARE_BYTES2(correct.str(), out.str());
}


TEST(DocWithAYZ__empty) {
  std::stringstream out, correct;
  DocWithAYZ::Schema schema;
  Writer writer(out, schema);

  schema.types<A>().serial = "writer.A";
  schema.types<Y>().serial = "writer.Y";
  schema.types<Z>().serial = "writer.Z";
  schema.types<Z>().p.serial = "zp";

  DocWithAYZ d;
  writer << d;

  correct << '\x02';  // <wire_version>
  correct << '\x94';  // <klasses>: 4-element array
  correct << '\x92';  // <klass>: 2-element array

  correct << '\xa8' << "__meta__";  // <klass_name>: utf-8 encoded "__meta__"
  correct << '\x90';  // <fields>: 0-element array

  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "writer.A";  // <klass_name>: utf-8 encoded "writer.A"
  correct << '\x93';  // <fields>: 1-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa5' << "v_str";  // utf-8 encoded "v_str"
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa7' << "v_uint8";  // utf-8 encoded "v_uint8"
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa6' << "v_bool";  // utf-8 encoded "v_bool"

  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "writer.Y";  // <klass_name>: utf-8 encoded "writer.Y"
  correct << '\x91';  // <fields>: 1-element array
  correct << '\x82';  // <field>: 2-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa1' << "p";  // utf-8 encoded "p"
  correct << '\x01';  // 1: POINTER_TO
  correct << '\x00';  // <store_id>: 0

  correct << '\x92';  // <klass>: 2-element array
  correct << '\xa8' << "writer.Z";  // <klass_name>: utf-8 encoded "writer.Z"
  correct << '\x92';  // <fields>: 2-element array
  correct << '\x82';  // <field>: 2-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa2' << "zp";  // utf-8 encoded "zp"
  correct << '\x01';  // 1: POINTER_TO
  correct << '\x00';  // <store_id>: 0
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa5' << "value";  // utf-8 encoded "value"

  correct << '\x93';  // <stores>: 3-element array

  correct << '\x93';  // <store>: 3-element array
  correct << '\xa2' << "as";  // <store_name>: utf-8 encoded "as"
  correct << '\x01';  // <klass_id>: 1
  correct << '\x00';  // <store_nelem>: 0

  correct << '\x93';  // <store>: 3-element array
  correct << '\xa2' << "ys";  // <store_name>: utf-8 encoded "ys"
  correct << '\x02';  // <klass_id>: 2
  correct << '\x00';  // <store_nelem>: 0

  correct << '\x93';  // <store>: 3-element array
  correct << '\xa2' << "zs";  // <store_name>: utf-8 encoded "zs"
  correct << '\x03';  // <klass_id>: 3
  correct << '\x00';  // <store_nelem>: 0

  correct << '\x01';  // <instance_nbytes>: 1 byte after this for the document
  correct << '\x80';  // <instance>: 0-element map

  correct << '\x01';  // <instance_nbytes>: 1 byte after this for the "as" store
  correct << '\x90';  // <instance>: 0-element array

  correct << '\x01';  // <instance_nbytes>: 1 byte after this for the "ys" store
  correct << '\x90';  // <instance>: 0-element array

  correct << '\x01';  // <instance_nbytes>: 1 byte after this for the "zs" store
  correct << '\x90';  // <instance>: 0-element array

  CHECK_COMPARE_BYTES2(correct.str(), out.str());
}

}  // SUITE

}  // namespace dr
}  // namespace schwa
