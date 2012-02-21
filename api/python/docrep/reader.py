import msgpack

from .constants import *
from .fields import Annotations, Field
from .meta import Annotation, Document

__all__ = ['Reader']


class Streamer(object):
  __slots__ = ('_read_types', '_reg_types', '_up', '_doc', '_doc_klass')

  def __init__(self, reg_types, doc_klass, istream):
    self._reg_types = reg_types
    self._up = msgpack.Unpacker(istream)
    self._doc = None
    self._doc_klass = doc_klass

  def __iter__(self):
    return self

  def next(self):
    self._read_doc()
    if self._doc is None:
      raise StopIteration()
    return self._doc

  def _read_doc(self):
    self._read_types = self._up.unpack()
    if self._read_types is None:
      self._doc = None
      return

    # do initial decoding of read types
    types = {} # { stream class name : klass }
    tok_klass = None
    for klass_name, mode, nelem, fields, ptrs in self._read_types:
      if klass_name in self._reg_types:
        klass = self._reg_types[klass_name]
        klass.update_osfields(fields)
      else:
        new_fields = dict((name, Field(name)) for name in fields)
        klass = type(klass_name, (Annotation, ), new_fields)
      types[klass_name] = klass
      if mode == MODE_TOKEN:
        if tok_klass is not None:
          raise ValueError('Two Token annotation types found ({0} and {1})'.format(tok_klass, klass))
        tok_klass = klass
    if tok_klass is None:
      raise ValueError('No Token annotation type found')

    # decode the pointers
    for klass_name, _, _, sfields, ptrs in self._read_types:
      for sidx, type_idx in ptrs.iteritems():
        klass = types[klass_name]
        field = klass._docrep_fields[klass.pyname_for_sidx(sidx)]
        field.pointer_to = types[self._read_types[type_idx][0]]

    # instantiate a document
    if self._doc_klass is None:
      attrs = dict((k._docrep_plural, Annotations(k)) for k in types.itervalues())
      self._doc_klass = type('Document', (Document, ), attrs)
    self._doc = self._doc_klass()

    # discover the field numbers for the four core Token attributes
    tok_begin = tok_klass.sidx_for_pyname('begin')
    tok_end   = tok_klass.sidx_for_pyname('end')
    tok_norm  = tok_klass.sidx_for_pyname('norm')
    tok_raw   = tok_klass.sidx_for_pyname('raw')

    # read the instances
    for i in xrange(len(self._read_types)):
      type_idx = self._up.unpack()
      nbytes   = self._up.unpack()

      klass_name, mode, nelem, sfields, ptrs = self._read_types[type_idx]
      klass = types[klass_name]

      if mode == MODE_DOCUMENT:
        msg_obj = self._up.unpack()
        for sidx, val in msg_obj.iteritems():
          pyname = klass.pyname_for_sidx(sidx)
          setattr(self._doc, pyname, val)

      elif mode == MODE_TOKEN or mode == MODE_ANNOTATION:
        objs = [None]*nelem
        for i, msg_obj in enumerate(self._up.unpack()):
          if mode == MODE_TOKEN:
            if tok_raw not in msg_obj and tok_norm in msg_obj:
              msg_obj[tok_raw] = msg_obj[tok_norm]
            if tok_begin in msg_obj and tok_end in msg_obj:
              msg_obj[tok_end] = msg_obj[tok_end] + msg_obj[tok_begin] - 1
          values = {}
          for sidx, val in msg_obj.iteritems():
            pyname = klass.pyname_for_sidx(sidx)
            if sidx in ptrs:
              if val == -1:
                val = None
              else:
                to_type = types[self._read_types[ptrs[sidx]][0]]
                val = getattr(self._doc, to_type._docrep_plural)[val]
            values[pyname] = val
          objs[i] = klass(**values)
        setattr(self._doc, klass._docrep_plural, objs)

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

