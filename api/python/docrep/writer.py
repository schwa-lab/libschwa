import cStringIO

import msgpack

from .constants import *
from .fields import *
from .meta import Document

__all__ = ['Writer']


class Type(object):
  __slots__ = ('klass', 'number', 'nelem', 'plural', 'name', 'fields', 'pyname2index', 'is_meta', 'is_singleton')

  def __init__(self, klass, sname, number, nelem, plural, is_meta=False, is_singleton=False):
    self.klass  = klass
    self.number = number
    self.nelem  = nelem
    self.name   = sname or self.klass._dr_name
    self.plural = plural
    self.fields = []
    self.pyname2index = {}
    self.is_meta = is_meta
    self.is_singleton = is_singleton

    for pyname, field in self.klass._dr_fields.iteritems():
      self.pyname2index[pyname] = len(self.fields)
      f = {}
      f[FIELD_TYPE_NAME] = field.sname or pyname
      if isinstance(field, Pointer):
        f[FIELD_TYPE_POINTER_TO] = field._klass
      elif isinstance(field, Range):
        f[FIELD_TYPE_IS_RANGE] = True
        if field._klass is not None:
          f[FIELD_TYPE_POINTER_TO] = field._klass
      self.fields.append(f)

  def pyname_to_index(self, pyname):
    return self.pyname2index[pyname]



class Writer(object):
  __slots__ = ('_ostream', '_packer')

  def __init__(self, ostream):
    self._ostream = ostream
    self._packer  = msgpack.Packer()

  def write_doc(self, doc):
    if not isinstance(doc, Document):
      raise ValueError('You can only stream instances of docrep.Document')

    # find all of the types defined
    types = {} # { klass : Type }
    types[doc.__class__] = Type(doc.__class__, '__meta__', len(types), 0, None, is_meta=True)
    for name, annotations in doc._dr_annotations.iteritems():
      t = Type(annotations._klass, annotations.sname, len(types), 0, name)
      if isinstance(annotations, Singleton):
        t.is_singleton = True
      else:
        t.nelem = len(getattr(doc, name))
      types[annotations._klass] = t

    # run along each of the Annotations and update the _dr_index attributes
    for name, annotations in doc._dr_annotations.iteritems():
      val = getattr(doc, name)
      if isinstance(annotations, Singleton):
        if val:
          val._dr_index = 0
      else:
        for i, obj in enumerate(val):
          obj._dr_index = i

    # construct the header
    header = []
    for klass, t in types.iteritems():
      x = [t.name, t.nelem, []]
      for field in t.fields:
        f = field.copy()
        ptr_klass = f.get(FIELD_TYPE_POINTER_TO)
        if ptr_klass is not None:
          f[FIELD_TYPE_POINTER_TO] = types[ptr_klass].number
        x[2].append(f)
      header.append(x)
    self._pack(header)

    # write out each of the annotation sets
    for t in types.itervalues():
      tmp = cStringIO.StringIO()

      if t.is_meta:
        self._pack(self._serialize(doc, t), tmp)
      elif t.is_singleton:
        self._pack(self._serialize(getattr(doc, t.plural), t), tmp)
      else:
        msg_objs = []
        for obj in getattr(doc, t.plural):
          msg_objs.append(self._serialize(obj, t))
        self._pack(msg_objs, tmp)

      self._pack(t.number)
      self._pack(len(tmp.getvalue()))
      self._ostream.write(tmp.getvalue())

  def _pack(self, obj, out=None):
    if out is None:
      out = self._ostream
    out.write(self._packer.pack(obj))

  def _serialize(self, obj, t):
    def swizzle(obj):
      if not hasattr(obj, '_dr_index'):
        raise ValueError('Cannot serialize a pointer to an object which is not managed by an Annotations')
      return obj._dr_index

    msg_obj = {}
    for pyname, field in obj._dr_fields.iteritems():
      val = getattr(obj, pyname)
      if val is None:
        continue
      if isinstance(field, Pointer):
        val = swizzle(val)
      elif isinstance(field, Range) and field._klass:
        val = map(swizzle, val)
      elif isinstance(val, unicode):
        val = val.encode('utf-8')
      msg_obj[t.pyname_to_index(pyname)] = val
    return msg_obj

