import cStringIO

import msgpack

from .constants import *
from .fields import Annotations, Field
from .meta import Document, Token

__all__ = ['Writer']

class Type(object):
  __slots__ = ('number', 'klass', 'name', 'mode', 'fields', 'pointers', 'plural', 'nelem')
  def __init__(self, number, klass, plural=None, nelem=0):
    self.number = number
    self.klass = klass
    self.name = klass._docrep_name
    self.plural = plural
    self.nelem = nelem
    if issubclass(klass, Document):
      self.mode = MODE_DOCUMENT
    elif issubclass(klass, Token):
      self.mode = MODE_TOKEN
    else:
      self.mode = MODE_ANNOTATION
    self.fields = klass._docrep_ofields
    self.pointers = {}
    for name, field in klass._docrep_fields.iteritems():
      if isinstance(field, Field) and field.pointer_to is not None:
        self.pointers[name] = field.pointer_to


class Writer(object):
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
      types[annotations.klass] = Type(len(types), annotations.klass, name, len(getattr(doc, name)))

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
    for k, v in types.iteritems():
      t = [None, None, None, None, {}]
      t[0] = self._klass_names.get(k, k._docrep_name)
      t[1] = v.mode
      t[2] = 0 if v.mode == MODE_DOCUMENT else v.nelem
      t[3] = v.fields
      for field, to_type in v.pointers.iteritems():
        t[4][k._docrep_ofields_inv[field]] = types[to_type].number
      header.append(t)
    self._pack(header)

    # discover the field numbers for the four core Token attributes
    tok_begin = tok_klass._docrep_ofields_inv['begin']
    tok_end   = tok_klass._docrep_ofields_inv['end']
    tok_norm  = tok_klass._docrep_ofields_inv['norm']
    tok_raw   = tok_klass._docrep_ofields_inv['raw']

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
    for i, name in enumerate(obj._docrep_ofields):
      if not obj._docrep_set[i]:
        continue
      ptr_to = obj._docrep_fields[name].pointer_to
      val = getattr(obj, name)
      if ptr_to is not None:
        if val is None:
          val = -1
        else:
          val = getattr(doc, types[ptr_to].plural).index(val)
      elif isinstance(val, unicode):
        val = val.encode('utf-8')
      msg_obj[i] = val
    return msg_obj

