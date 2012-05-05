"""
Tests ensuring that what is written out is identical to what is read in.
"""
from unittest import TestCase

from schwa import dr

from testutil import write_x_read_y, write_read


class DocWithoutFields(dr.Document):
  pass


class DocWithField(dr.Document):
  field = dr.Field()


class Annot(dr.Annotation):
  pass


class DocWithAnnotsAndPointer(dr.Document):
  annots = dr.Store('Annot')
  special_annot = dr.Pointer('Annot')


class SameModelTests(TestCase):
  def test_pointer(self):
    doc = DocWithAnnotsAndPointer()
    doc.annots.create()
    doc.special_annot = doc.annots[0]
    doc = write_read(doc)
    self.assertEquals(doc.special_annot, doc.annots[0])

  def test_null_pointer(self):
    doc = DocWithAnnotsAndPointer()
    doc.annots.create()
    doc.special_annot = None
    doc = write_read(doc)
    self.assertEquals(doc.special_annot, None)


class DifferentModelTests(TestCase):
  """Tests casting from one model to another via (de)serialisation"""
  def test_various(self):
    doc = DocWithField()
    doc = write_x_read_y(doc, DocWithField)
    doc.field = 'foo'
    doc = write_x_read_y(doc, DocWithoutFields)
    doc = write_x_read_y(doc, DocWithAnnotsAndPointer)
    doc.annots.create()
    doc.special_annot = doc.annots[-1]
    doc = write_x_read_y(doc, DocWithoutFields)
    doc = write_x_read_y(doc, DocWithField)
    self.assertEquals(doc.field, 'foo')
    self.assertEquals(len(doc.annots), 1)
    self.assertEquals(doc.special_annot, doc.annots[0])
