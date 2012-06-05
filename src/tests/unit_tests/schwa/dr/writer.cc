/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/dr.h>
#include "test_utils.h"

#include <boost/test/unit_test.hpp>

namespace dr = schwa::dr;
namespace mp = schwa::msgpack;


class DocWithField : public dr::Document {
public:
  std::string name;

  class Schema;
};

class DocWithField::Schema : public dr::DocumentSchema<DocWithField> {
public:
  DR_FIELD(&DocWithField::name) name;

  Schema(void) :
    dr::DocumentSchema<DocWithField>("Document", "Some help text about this Document class"),
    name(*this, "name", "some help text about name", dr::LOAD_RW, "name")
    { }
  virtual ~Schema(void) { }
};


class DocWithFieldWithSerial : public dr::Document {
public:
  std::string name;

  class Schema;
};

class DocWithFieldWithSerial::Schema : public dr::DocumentSchema<DocWithFieldWithSerial> {
public:
  DR_FIELD(&DocWithFieldWithSerial::name) name;

  Schema(void) :
    dr::DocumentSchema<DocWithFieldWithSerial>("Document", "Some help text about this Document class"),
    name(*this, "name", "some help text about name", dr::LOAD_RW, "filename")
    { }
  virtual ~Schema(void) { }
};


BOOST_AUTO_TEST_SUITE(schwa_dr_writer)

BOOST_AUTO_TEST_CASE(DocWithField__name_is_null) {
  std::stringstream out, correct;
  DocWithField::Schema schema;
  dr::Writer writer(out, schema);

  DocWithField d;
  writer << d;

  correct << '\x91';  // <klasses>: 1-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << "\xa8__meta__";  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
  correct << '\x91';  // <fields>: 1-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << "\xa4name";  // 4-bytes of utf-8 encoded "name"
  correct << '\x90';  // <stores>: 0-element array
  correct << '\x01';  // <instance_nbytes>: 1 byte after this
  correct << '\x80';  // <instance>: 0-element map

  BOOST_CHECK( compare_bytes(out.str(), correct.str()) );
}


BOOST_AUTO_TEST_CASE(DocWithField__name) {
  std::stringstream out, correct;
  DocWithField::Schema schema;
  dr::Writer writer(out, schema);

  DocWithField d;
  d.name = "/etc/passwd";
  writer << d;

  correct << '\x91';  // <klasses>: 1-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << "\xa8__meta__";  // <klass_name>: utf-8 encoded "__meta__"
  correct << '\x91';  // <fields>: 1-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << "\xa4name";  // utf-8 encoded "name"
  correct << '\x90';  // <stores>: 0-element array
  correct << '\x0e';  // <instance_nbytes>: 14 bytes after this
  correct << '\x81';  // <instance>: 1-element map
  correct << '\x00';  // 0: field number 0 (=> name;
  correct << "\xab/etc/passwd";  // utf-8 encoded "/etc/passwd"

  BOOST_CHECK( compare_bytes(out.str(), correct.str()) );
}


BOOST_AUTO_TEST_CASE(DocWithFieldWithSerial__name_is_null) {
  std::stringstream out, correct;
  DocWithFieldWithSerial::Schema schema;
  dr::Writer writer(out, schema);

  DocWithFieldWithSerial d;
  writer << d;

  correct << '\x91';  // <klasses>: 1-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << "\xa8__meta__";  // <klass_name>: 8-bytes of utf-8 encoded "__meta__"
  correct << '\x91';  // <fields>: 1-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa8' << "filename";  // utf-8 encoded "filename"
  correct << '\x90';  // <stores>: 0-element array
  correct << '\x01';  // <instance_nbytes>: 1 byte after this
  correct << '\x80';  // <instance>: 0-element map

  BOOST_CHECK( compare_bytes(out.str(), correct.str()) );
}


BOOST_AUTO_TEST_CASE(DocWithFieldWithSerial__name) {
  std::stringstream out, correct;
  DocWithFieldWithSerial::Schema schema;
  dr::Writer writer(out, schema);

  DocWithFieldWithSerial d;
  d.name = "/etc/passwd";
  writer << d;

  correct << '\x91';  // <klasses>: 1-element array
  correct << '\x92';  // <klass>: 2-element array
  correct << "\xa8__meta__";  // <klass_name>: utf-8 encoded "__meta__"
  correct << '\x91';  // <fields>: 1-element array
  correct << '\x81';  // <field>: 1-element map
  correct << '\x00';  // 0: NAME
  correct << '\xa8' << "filename";  // utf-8 encoded "filename"
  correct << '\x90';  // <stores>: 0-element array
  correct << '\x0e';  // <instance_nbytes>: 14 bytes after this
  correct << '\x81';  // <instance>: 1-element map
  correct << '\x00';  // 0: field number 0 (=> name;
  correct << "\xab/etc/passwd";  // utf-8 encoded "/etc/passwd"

  BOOST_CHECK( compare_bytes(out.str(), correct.str()) );
}


BOOST_AUTO_TEST_SUITE_END()
