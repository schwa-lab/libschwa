# vim: set ts=2 et:
from StringIO import StringIO

from schwa import dr


def write_x_read_y(doc, class2=None):
  """
  typecast doc as class2
  """
  print 'Writing {0}'.format(doc.__class__.__name__)
  f = StringIO()
  dr.Writer(f).write_doc(doc)
  f.seek(0)
  print 'Reading {0}'.format(class2.__name__)
  return dr.Reader(class2).stream(f).next()


def write_read(doc):
  return write_x_read_y(doc, doc.__class__)
