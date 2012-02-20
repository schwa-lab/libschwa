import msgpack

from .constants import *
from .fields import Field
from .meta import Annotation
from .utils import pluralise


class Streamer(object):
  __slots__ = ('_types', '_read_types', '_reg_types', '_up', '_doc', '_doc_klass')

  def __init__(self, reg_types, doc_klass, istream):
    self._reg_types = reg_types
    self._up = msgpack.Unpacker(istream)
    self._doc = None
    self._doc_klass = doc_klass
    self._read_doc()

  def __iter__(self):
    return self

  def __nonzero__(self):
    return self._doc is not None

  def next(self):
    d = self._doc
    self._read_doc()
    return d

  def _read_doc(self):
    self._read_types = self._up.unpack()
    if self._read_types is None:
      self._doc = None
      return

    # do initial decoding of read types
    self._types = {}
    plurals = {}
    for name, mode, nelem, fields, ptrs in self._read_types:
      if name not in self._reg_types:
        new_fields = dict((name, Field(name)) for name in fields)
        klass = type(name, (Annotation, ), new_fields)
        self._types[name] = klass
      else:
        klass = self._reg_types[name]
        klass._update_fields(fields)
        self._types[name] = klass

      if mode != MODE_DOCUMENT:
        if hasattr(klass, '_docrep_plural'):
          plurals[name] = klass._docrep_plural
        else:
          plurals[name] = pluralise(klass._docrep_name)

    # decode the pointers
    for name, _, _, fields, ptrs in self._read_types:
      for k, v in ptrs.iteritems():
        field = self._types[name]._docrep_fields[fields[k]]
        field.pointer_to = self._types[self._read_types[v][0]]

    # instantiate a document
    if self._doc_klass is None:
      self._doc_klass = type('Document', (), dict((plurals[k], []) for k in self._types))
    self._doc = self._doc_klass()

    # read the instances
    for i in xrange(len(self._read_types)):
      type_id = self._up.unpack()
      nbytes = self._up.unpack()

      name, mode, nelem, fields, ptrs = self._read_types[type_id]
      klass = self._types[name]

      if mode == MODE_DOCUMENT:
        msg_obj = self._up.unpack()
        for idx, val in msg_obj.iteritems():
          field = fields[idx]
          if field in klass._docrep_fields:
            field = klass._docrep_fields[field].name
          setattr(self._doc, field, val)

      elif mode == MODE_TOKEN or mode == MODE_ANNOTATION:
        objs = [None]*nelem
        for i, msg_obj in enumerate(self._up.unpack()):
          if mode == MODE_TOKEN:
            if 'norm' in msg_obj and 'raw' not in msg_obj:
              msg_obj['raw'] = msg_obj['norm']
            if 'bytes_begin' in msg_obj and 'bytes_length' in msg_obj and 'bytes_end' not in msg_obj:
              msg_obj['bytes_end'] = msg_obj['bytes_begin'] + msg_obj['bytes_length']

          values = {}
          for idx, val in msg_obj.iteritems():
            field = klass._docrep_fields[fields[idx]]
            if field.pointer_to is not None:
              if val == -1:
                val = None
              else:
                to_type = [k for k, v in self._types.iteritems() if v == field.pointer_to][0]
                val = getattr(self._doc, plurals[to_type])[val]
            values[field.name] = val
          objs[i] = klass(**values)

        setattr(self._doc, plurals[name], objs)

      else:
        raise ValueError('Invalid "mode" value ' + str(mode))




class Reader(object):
  __slots__ = ('_reg_types', '_doc_klass')

  def __init__(self, doc_klass=None, *args, **kwargs):
    self._reg_types = {}
    self._doc_klass = doc_klass
    for v in args:
      self.reg(v)
    for k, v in kwargs.iteritems():
      self.reg(v, k)

  def reg(self, klass, name=None):
    if not issubclass(klass, Annotation):
      raise ValueError('You cannot register a class which does not subclass docrep.Annotation')
    if name is None:
      name = klass._docrep_name
    self._reg_types[name] = klass

  def stream(self, istream):
    return Streamer(self._reg_types, self._doc_klass, istream)

