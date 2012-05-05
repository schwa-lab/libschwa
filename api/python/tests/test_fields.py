from unittest import TestCase

from schwa import dr

from testutil import write_x_read_y, write_read


class DocWithoutFields(dr.Document):
  pass


class FieldWithDefault(dr.Field):
  def default(self):
    return 'default'


class DocWithDefaultField(dr.Document):
  field = FieldWithDefault()


class FieldTests(TestCase):
  def test_default_field(self):
    doc = DocWithDefaultField()
    self.assertEquals(doc.field, 'default')

  def test_default_field_from_wire(self):
    doc = DocWithoutFields()
    doc = write_x_read_y(doc, DocWithDefaultField)
    self.assertEquals(doc.field, 'default')

  def test_set_default_field_from_wire(self):
    doc = DocWithoutFields()
    doc = write_x_read_y(doc, DocWithDefaultField)
    self.assertEquals(doc.field, 'default')
    doc.field = 'value'
    doc = write_read(doc)
    self.assertEquals(doc.field, 'value')
