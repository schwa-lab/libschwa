import msgpack

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

    self._types = {}
    plurals = {}
    for k in self._read_types:
      if k not in self._reg_types:
        fields = dict((name, Field(name)) for name in self._read_types[k]['fields'])
        klass = type(k, (Annotation, ), fields)
        self._types[k] = klass
        plurals[k] = pluralise(k)
      else:
        self._types[k] = self._reg_types[k]
        if hasattr(self._reg_types[k], '_docrep_plural'):
          plurals[k] = self._reg_types[k]._docrep_plural
        else:
          plurals[k] = pluralise(k)

    if self._doc_klass is None:
      self._doc_klass = type('Document', (), dict((plurals[k], []) for k in self._types))
    self._doc = self._doc_klass()

    # read the instances
    ngroups = self._up.unpack()
    for i in xrange(ngroups):
      name = self._up.unpack()
      klass = self._types[name]
      nbytes = self._up.unpack()
      objs = []
      for msg_obj in self._up.unpack():
        if name == 'Token':
          if 'norm' in msg_obj and 'raw' not in msg_obj:
            msg_obj['raw'] = msg_obj['norm']
          if 'begin' in msg_obj and '_length' in msg_obj and 'end' not in msg_obj:
            msg_obj['end'] = msg_obj['begin'] + msg_obj['_length']
        values = {}
        for idx, val in msg_obj.iteritems():
          key = self._read_types[name]['fields'][idx]
          if key in klass._docrep_fields:
            key = klass._docrep_fields[key].name
          values[key] = val
        objs.append(self._types[name](**values))
      setattr(self._doc, plurals[name], objs)



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
      name = klass.__name__.split('.')[-1]
    self._reg_types[name] = klass

  def stream(self, istream):
    return Streamer(self._reg_types, self._doc_klass, istream)

