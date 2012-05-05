# vim: set ts=2 et:
import datetime
import unittest

from schwa import dr

from testutil import write_read


class Event(dr.Annotation):
  name = dr.Field()
  date = dr.DateTimeField()


class Doc(dr.Document):
  events = dr.Store('Event')


class DateTimeTest(unittest.TestCase):
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
