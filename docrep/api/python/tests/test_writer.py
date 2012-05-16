# vim: set ts=2 et:
"""
Manual testing of the Writer. Some hand-written serialisations of various
situations.
"""
import StringIO
import unittest

from schwa import dr


class DocWithField(dr.Document):
  name = dr.Field()

  class Meta:
    name = 'writer.DocWithField'


class DocWithFieldWithSerial(dr.Document):
  name = dr.Field(serial='filename')

  class Meta:
    name = 'writer.DocWithFieldWithSerial'


class A(dr.Annotation):
  value = dr.Field()

  class Meta:
    name = 'writer.A'


class B(dr.Annotation):
  value = dr.Field()

  class Meta:
    name = 'writer.B'


class Y(dr.Annotation):
  p = dr.Pointer(A)

  class Meta:
    name = 'writer.Y'


class Z(dr.Annotation):
  p = dr.Pointer(A, serial='zp')
  value = dr.Field()

  class Meta:
    name = 'writer.Z'


class DocWithA(dr.Document):
  as_ = dr.Store(A, serial='as')

  class Meta:
    name = 'writer.DocWithA'


class DocWithAYZ(dr.Document):
  as_ = dr.Store(A, serial='as')
  ys = dr.Store(Y)
  zs = dr.Store(Z)

  class Meta:
    name = 'writer.DocWithAYZ'


# =============================================================================
# unit testing code
# =============================================================================
def serialise(doc):
  f = StringIO.StringIO()
  dr.Writer(f).write_doc(doc)
  return f.getvalue()


class TestDocWithField(unittest.TestCase):
  def test_nameisnull(self):
    d = DocWithField()
    s = serialise(d)

    correct = StringIO.StringIO()
    correct.write('\x91') # <klasses>: 1-element array
    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8__meta__') # <klass_name>: 8-bytes of utf-8 encoded "__meta__"
    correct.write('\x91') # <fields>: 1-element array
    correct.write('\x81') # <field>: 1-element map
    correct.write('\x00') # 0: NAME
    correct.write('\xa4name') # 4-bytes of utf-8 encoded "name"
    correct.write('\x90') # <stores>: 0-element array
    correct.write('\x01') # <instance_nbytes>: 1 byte after this
    correct.write('\x80') # <instance>: 0-element map

    self.assertEqual(s, correct.getvalue())

  def test_name(self):
    d = DocWithField(name='/etc/passwd')
    s = serialise(d)

    correct = StringIO.StringIO()
    correct.write('\x91') # <klasses>: 1-element array
    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8__meta__') # <klass_name>: utf-8 encoded "__meta__"
    correct.write('\x91') # <fields>: 1-element array
    correct.write('\x81') # <field>: 1-element map
    correct.write('\x00') # 0: NAME
    correct.write('\xa4name') # utf-8 encoded "name"
    correct.write('\x90') # <stores>: 0-element array
    correct.write('\x0e') # <instance_nbytes>: 14 bytes after this
    correct.write('\x81') # <instance>: 1-element map
    correct.write('\x00') # 0: field number 0 (=> name)
    correct.write('\xab/etc/passwd') # utf-8 encoded "/etc/passwd"

    self.assertEqual(s, correct.getvalue())


class TestDocWithFieldSerial(unittest.TestCase):
  def test_nameisnull(self):
    d = DocWithFieldWithSerial()
    s = serialise(d)

    correct = StringIO.StringIO()
    correct.write('\x91') # <klasses>: 1-element array
    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8__meta__') # <klass_name>: utf-8 encoded "__meta__"
    correct.write('\x91') # <fields>: 1-element array
    correct.write('\x81') # <field>: 1-element map
    correct.write('\x00') # 0: NAME
    correct.write('\xa8filename') # utf-8 encoded "filename"
    correct.write('\x90') # <stores>: 0-element array
    correct.write('\x01') # <instance_nbytes>: 1 byte after this
    correct.write('\x80') # <instance>: 0-element map

    self.assertEqual(s, correct.getvalue())

  def test_name(self):
    d = DocWithFieldWithSerial(name='/etc/passwd')
    s = serialise(d)

    correct = StringIO.StringIO()
    correct.write('\x91') # <klasses>: 1-element array
    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8__meta__') # <klass_name>: utf-8 encoded "__meta__"
    correct.write('\x91') # <fields>: 1-element array
    correct.write('\x81') # <field>: 1-element map
    correct.write('\x00') # 0: NAME
    correct.write('\xa8filename') # utf-8 encoded "filename"
    correct.write('\x90') # <stores>: 0-element array
    correct.write('\x0e') # <instance_nbytes>: 14 bytes after this
    correct.write('\x81') # <instance>: 1-element map
    correct.write('\x00') # 0: field number 0 (=> name)
    correct.write('\xab/etc/passwd') # utf-8 encoded "/etc/passwd"

    self.assertEqual(s, correct.getvalue())


class TestDocWithA(unittest.TestCase):
  def test_empty(self):
    d = DocWithA()
    s = serialise(d)

    correct = StringIO.StringIO()
    correct.write('\x92') # <klasses>: 2-element array
    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8__meta__') # <klass_name>: utf-8 encoded "__meta__"
    correct.write('\x90') # <fields>: 0-element array
    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8writer.A') # <klass_name>: utf-8 encoded "writer.A"
    correct.write('\x91') # <fields>: 1-element array
    correct.write('\x81') # <field>: 1-element map
    correct.write('\x00') # 0: NAME
    correct.write('\xa5value') # utf-8 encoded "value"
    correct.write('\x91') # <stores>: 1-element array
    correct.write('\x93') # <store>: 3-element array
    correct.write('\xa2as') # <store_name>: utf-8 encoded "as"
    correct.write('\x01') # <klass_id>: 1
    correct.write('\x00') # <store_nelem>: 0
    correct.write('\x01') # <instance_nbytes>: 1 byte after this for the document
    correct.write('\x80') # <instance>: 0-element map
    correct.write('\x01') # <instance_nbytes>: 1 byte after this for the "as" store
    correct.write('\x90') # <instance>: 0-element array

    self.assertEqual(s, correct.getvalue())

  def test_four_elements(self):
    d = DocWithA()
    d.as_.create(value='first')
    d.as_.create(value=2)
    d.as_.create()
    d.as_.create(value=True)
    s = serialise(d)

    correct = StringIO.StringIO()
    correct.write('\x92') # <klasses>: 2-element array
    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8__meta__') # <klass_name>: utf-8 encoded "__meta__"
    correct.write('\x90') # <fields>: 0-element array
    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8writer.A') # <klass_name>: utf-8 encoded "writer.A"
    correct.write('\x91') # <fields>: 1-element array
    correct.write('\x81') # <field>: 1-element map
    correct.write('\x00') # 0: NAME
    correct.write('\xa5value') # utf-8 encoded "value"
    correct.write('\x91') # <stores>: 1-element array
    correct.write('\x93') # <store>: 3-element array
    correct.write('\xa2as') # <store_name>: utf-8 encoded "as"
    correct.write('\x01') # <klass_id>: 1
    correct.write('\x04') # <store_nelem>: 4
    correct.write('\x01') # <instance_nbytes>: 1 byte after this for the document
    correct.write('\x80') # <instance>: 0-element map
    correct.write('\x10') # <instance_nbytes>: 16 byte after this for the "as" store
    correct.write('\x94') # <instance>: 4-element array
    correct.write('\x81\x00\xa5first') # {0: 'first'}
    correct.write('\x81\x00\x02')      # {0: 2}
    correct.write('\x80')              # {}
    correct.write('\x81\x00\xc3')      # {0: True}

    self.assertEqual(s, correct.getvalue())


class TestDocWithAYZ(unittest.TestCase):
  def test_empty(self):
    d = DocWithAYZ()
    s = serialise(d)

    correct = StringIO.StringIO()
    correct.write('\x94') # <klasses>: 4-element array
    correct.write('\x92') # <klass>: 2-element array

    correct.write('\xa8__meta__') # <klass_name>: utf-8 encoded "__meta__"
    correct.write('\x90') # <fields>: 0-element array

    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8writer.A') # <klass_name>: utf-8 encoded "writer.A"
    correct.write('\x91') # <fields>: 1-element array
    correct.write('\x81') # <field>: 1-element map
    correct.write('\x00') # 0: NAME
    correct.write('\xa5value') # utf-8 encoded "value"

    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8writer.Y') # <klass_name>: utf-8 encoded "writer.Y"
    correct.write('\x91') # <fields>: 1-element array
    correct.write('\x82') # <field>: 2-element map
    correct.write('\x00') # 0: NAME
    correct.write('\xa1p') # utf-8 encoded "p"
    correct.write('\x01') # 1: POINTER_TO
    correct.write('\x00') # <store_id>: 0

    correct.write('\x92') # <klass>: 2-element array
    correct.write('\xa8writer.Z') # <klass_name>: utf-8 encoded "writer.Z"
    correct.write('\x92') # <fields>: 2-element array
    correct.write('\x82') # <field>: 2-element map
    correct.write('\x00') # 0: NAME
    correct.write('\xa2zp') # utf-8 encoded "zp"
    correct.write('\x01') # 1: POINTER_TO
    correct.write('\x00') # <store_id>: 0
    correct.write('\x81') # <field>: 1-element map
    correct.write('\x00') # 0: NAME
    correct.write('\xa5value') # utf-8 encoded "value"

    correct.write('\x93') # <stores>: 3-element array

    correct.write('\x93') # <store>: 3-element array
    correct.write('\xa2as') # <store_name>: utf-8 encoded "as"
    correct.write('\x01') # <klass_id>: 1
    correct.write('\x00') # <store_nelem>: 0

    correct.write('\x93') # <store>: 3-element array
    correct.write('\xa2ys') # <store_name>: utf-8 encoded "ys"
    correct.write('\x02') # <klass_id>: 2
    correct.write('\x00') # <store_nelem>: 0

    correct.write('\x93') # <store>: 3-element array
    correct.write('\xa2zs') # <store_name>: utf-8 encoded "zs"
    correct.write('\x03') # <klass_id>: 3
    correct.write('\x00') # <store_nelem>: 0

    correct.write('\x01') # <instance_nbytes>: 1 byte after this for the document
    correct.write('\x80') # <instance>: 0-element map

    correct.write('\x01') # <instance_nbytes>: 1 byte after this for the "as" store
    correct.write('\x90') # <instance>: 0-element array

    correct.write('\x01') # <instance_nbytes>: 1 byte after this for the "ys" store
    correct.write('\x90') # <instance>: 0-element array

    correct.write('\x01') # <instance_nbytes>: 1 byte after this for the "zs" store
    correct.write('\x90') # <instance>: 0-element array

    self.assertEqual(s, correct.getvalue())

