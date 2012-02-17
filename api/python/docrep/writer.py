import msgpack

from .meta import Document


class Writer(object):
  def __init__(self, ostream):
    self._packer = msgpack.Packer()

  def stream(self, doc):
    if not isinstance(doc, Document):
      raise ValueError('You can only stream instances of docrep.Document')

  def _write_header(self, doc):
    header = {}
    for name, field in doc._docrep_fields.iteritems():
      item = {}


