# vim: set ts=2 et:
"""
Unit test for #1423
http://schwa.org/issues/1423

Overwriting a previously existing attr of a different type should raise an
exception. An AttributeException will be raised if this occurs.
"""
import unittest

from schwa import dr

from utils import write_x_read_y


class DocWithFieldFoo(dr.Document):
  foo = dr.Field()

  class Meta:
    name = 'issue_1423.DocWithFieldFoo'


class DocWithFieldSerialFoo(dr.Document):
  bar = dr.Field(serial='foo')

  class Meta:
    name = 'issue_1423.DocWithFieldSerialFoo'


class DocWithConstAttrFoo(dr.Document):
  foo = 5

  class Meta:
    name = 'issue_1423.DocWithConstAttrFoo'


class Issue1423Test(unittest.TestCase):
  OVERWRITE_MSG = r'Cannot overwrite attr .* of class .* of different type (.*)'

  def test_fieldfoo_fieldfoo(self):
    d = DocWithFieldFoo()
    d.foo = 'hello'
    d = write_x_read_y(d, DocWithFieldFoo)
    self.assertEqual(d.foo, 'hello')

  def test_fieldfoo_serialfoo(self):
    d = DocWithFieldFoo()
    d.foo = 'hello'
    d = write_x_read_y(d, DocWithFieldSerialFoo)
    self.assertFalse(hasattr(d, 'foo'))
    self.assertTrue(hasattr(d, 'bar'))
    self.assertEqual(d.bar, 'hello')

  def test_fieldfoo_constfoo(self):
    d = DocWithFieldFoo()
    d.foo = 'hello'
    with self.assertRaisesRegexp(AttributeError, Issue1423Test.OVERWRITE_MSG):
      write_x_read_y(d, DocWithConstAttrFoo)

  def test_serialfoo_fieldfoo(self):
    d = DocWithFieldSerialFoo()
    d.bar = 'hello'
    d = write_x_read_y(d, DocWithFieldFoo)
    self.assertFalse(hasattr(d, 'bar'))
    self.assertEqual(d.foo, 'hello')

  def test_serialfoo_serialfoo(self):
    d = DocWithFieldSerialFoo()
    d.bar = 'hello'
    d = write_x_read_y(d, DocWithFieldSerialFoo)
    self.assertFalse(hasattr(d, 'foo'))
    self.assertEqual(d.bar, 'hello')

  def test_fieldfoo_constfoo(self):
    d = DocWithFieldSerialFoo()
    d.bar = 'hello'
    with self.assertRaisesRegexp(AttributeError, Issue1423Test.OVERWRITE_MSG):
      write_x_read_y(d, DocWithConstAttrFoo)
