# vim: set ts=2 et:
# encoding: utf-8
import datetime
import unittest

from schwa import dr

from utils import write_read


class Event(dr.Annotation):
  name = dr.Field()
  date = dr.DateTimeField()

  class Meta:
    name = 'test_field_types.Event'


class String(dr.Annotation):
  utf8  = dr.EncodedStringField()
  utf32 = dr.EncodedStringField('utf-32')

  class Meta:
    name = 'test_field_types.String'


class Doc(dr.Document):
  events  = dr.Store(Event)
  strings = dr.Store(String)

  class Meta:
    name = 'test_field_types.Doc'


class TestDateTimeField(unittest.TestCase):
  def test_none_and_notnone(self):
    doc1 = Doc()
    doc1.events.create(name='Some event', date=datetime.datetime(year=1990, month=12, day=2))
    doc1.events.create(name='Another event')
    self.assertIsNotNone(doc1.events[0].date)
    self.assertIsNone(doc1.events[1].date)

    doc2 = write_read(doc1)
    self.assertIsNotNone(doc2.events[0].date)
    self.assertIsNone(doc2.events[1].date)
    self.assertEqual(doc1.events[0].date, doc2.events[0].date)


class TestEncodedStringField(unittest.TestCase):
  def test_none_and_notnone(self):
    U = u'⁆foo'
    doc = Doc()
    doc.strings.create(utf8=U, utf32=U)
    doc.strings.create()
    self.assertTrue(isinstance(doc.strings[0].utf8, unicode))
    self.assertTrue(isinstance(doc.strings[0].utf32, unicode))

    doc = write_read(doc)
    self.assertEqual(len(doc.strings), 2)
    self.assertTrue(isinstance(doc.strings[0].utf8, unicode))
    self.assertTrue(isinstance(doc.strings[0].utf32, unicode))
    self.assertEqual(doc.strings[0].utf8, U)
    self.assertEqual(doc.strings[0].utf32, U)
