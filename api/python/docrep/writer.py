import cStringIO

import msgpack

from .constants import *
from .fields import Annotations, Field
from .meta import Document, Token

__all__ = ['Writer']


class Type(object):
  __slots__ = ('number', 'klass', 'name', 'mode', 'sfields', 'pointers', 'plural', 'nelem')

  def __init__(self, number, klass, nelem=0):
    self.number = number
    self.klass  = klass
    self.name   = klass._docrep_name
    self.plural = klass._docrep_plural
    self.nelem  = nelem
    if issubclass(klass, Document):
      self.mode = MODE_DOCUMENT
    elif issubclass(klass, Token):
      self.mode = MODE_TOKEN
    else:
      self.mode = MODE_ANNOTATION
    self.sfields = klass._docrep_osfields
    self.pointers = {}
    for py_name, field in klass._docrep_fields.iteritems():
      if field.pointer_to is not None:
        self.pointers[py_name] = field.pointer_to


class Writer(object):
  __slots__ = ('_ostream', '_packer', '_klass_names')

  def __init__(self, ostream):
    self._ostream = ostream
    self._packer = msgpack.Packer()
    self._klass_names = {}

  def reg(self, klass, name):
    self._klass_names[klass] = name

  def write_doc(self, doc):
    if not isinstance(doc, Document):
      raise ValueError('You can only stream instances of docrep.Document')

    # find all of the types defined
    types = {}
    types[doc.__class__] = Type(len(types), doc.__class__)
    for name, annotations in doc._docrep_annotations.iteritems():
      types[annotations.klass] = Type(len(types), annotations.klass, len(getattr(doc, name)))

    # ensure we have one Document and one Token type
    doc_klass = tok_klass = None
    for t in types.itervalues():
      if t.mode == MODE_DOCUMENT:
        if doc_klass is not None:
          raise ValueError('Two subclasses of Document found ({0} and {1})'.format(doc_klass, klass))
        doc_klass = t.klass
      elif t.mode == MODE_TOKEN:
        if tok_klass is not None:
          raise ValueError('Two subclasses of Token found ({0} and {1})'.format(tok_klass, klass))
        tok_klass = t.klass
    if doc_klass is None:
      raise ValueError('No subclass of Document found')
    if tok_klass is None:
      raise ValueError('No subclass of Token found')

    # construct the header
    header = []
    for klass, t in types.iteritems():
      x = [None, None, None, None, {}]
      x[0] = self._klass_names.get(klass, klass._docrep_name)
      x[1] = t.mode
      x[2] = 0 if t.mode == MODE_DOCUMENT else t.nelem
      x[3] = t.sfields
      for py_name, to_type in t.pointers.iteritems():
        x[4][klass.sidx_for_pyname(py_name)] = types[to_type].number
      header.append(x)
    self._pack(header)

    # discover the field numbers for the four core Token attributes
    tok_begin = tok_klass.sidx_for_pyname('begin')
    tok_end   = tok_klass.sidx_for_pyname('end')
    tok_norm  = tok_klass.sidx_for_pyname('norm')
    tok_raw   = tok_klass.sidx_for_pyname('raw')

    # write out each of the annotation sets
    for t in types.itervalues():
      tmp = cStringIO.StringIO()

      if t.mode == MODE_DOCUMENT:
        self._pack(self._msgpack_annotation(doc, doc, types), tmp)

      elif t.mode == MODE_TOKEN or t.mode == MODE_ANNOTATION:
        msg_objs = []
        for obj in getattr(doc, t.plural):
          msg_obj = self._msgpack_annotation(obj, doc, types)
          if t.mode == MODE_TOKEN:
            if tok_norm in msg_obj and tok_raw in msg_obj and msg_obj[tok_norm] == msg_obj[tok_raw]:
              del msg_obj[tok_raw]
            if tok_begin in msg_obj and tok_end in msg_obj:
              msg_obj[tok_end] = msg_obj[tok_end] - msg_obj[tok_begin] + 1
          msg_objs.append(msg_obj)
        self._pack(msg_objs, tmp)

      else:
        raise ValueError('Invalid "mode" value ' + str(mode))

      self._pack(types[t.klass].number)
      self._pack(len(tmp.getvalue()))
      self._ostream.write(tmp.getvalue())

  def _pack(self, obj, out=None):
    if out is None:
      out = self._ostream
    out.write(self._packer.pack(obj))

  def _msgpack_annotation(self, obj, doc, types):
    msg_obj = {}
    for py_name, field in obj._docrep_fields.iteritems():
      if py_name not in obj._docrep_is_set:
        continue
      val = getattr(obj, py_name)
      if field.pointer_to is not None:
        if val is None:
          val = -1
        else:
          val = getattr(doc, types[field.pointer_to].plural).index(val)
      elif isinstance(val, unicode):
        val = val.encode('utf-8')
      msg_obj[obj.sidx_for_pyname(py_name)] = val
    return msg_obj

