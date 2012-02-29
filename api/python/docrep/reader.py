import msgpack

from .constants import *
from .fields import *
from .io import *
from .meta import Annotation, Document

__all__ = ['Reader']


class Reader(object):
  __slots__ = ('_doc_klass', '_types', '_by_sname', '_unpacker', '_doc')

  def __init__(self, doc_klass=None):
    self._doc_klass = doc_klass
    self._types = {}    # { klass : Type }
    self._by_sname = {} # { str : Type }
    if doc_klass:
      if not issubclass(doc_klass, Document):
        raise ValueError('"doc_klass" must be a subclass of Document')
      self._types = types_from_doc(doc)
      for t in self._types.itervalues():
        self._by_sname[t.sname] = t
    print self._types, self._by_sname

  def __iter__(self):
    return self

  def next(self):
    self._read_doc()
    if self._doc is None:
      raise StopIteration()
    return self._doc

  def stream(self, istream):
    self._unpacker = msgpack.Unpacker(istream)
    return self

  def _unpack(self):
    obj = self._unpacker.unpack()
    #print '[unpack] {0!r}'.format(obj)
    return obj

  def _read_doc(self):
    # attempt to read the header
    header = self._unpack() # [ ( name, nelem, [ { field_key : field_val } ] ) ]
    if header is None:
      self._doc = None
      return
    print 'header', header

    # decode the header
    meta_num, meta_fields = -1, None
    by_num = []
    for klass_name, klass_nelem, klass_fields in header:
      # decode the fields
      fields = {}
      for klass_field in klass_fields:
        name = klass_field[FIELD_TYPE_NAME]
        ptr  = klass_field.get(FIELD_TYPE_POINTER_TO)
        rng  = klass_field.get(FIELD_TYPE_IS_RANGE)
        if rng:
          f = Range(sname=name)
          f._klass = ptr
        elif ptr:
          f = Pointer('', sname=name)
          f._klass = ptr
        else:
          f = Field(sname=name)
        fields[name] = f

      if klass_name == '__meta__':
        meta_num = len(by_num)
        meta_fields = fields
        if klass_name in self._by_sname:
          t = self._by_sname[klass_name]
        else:
          t = Type(None, klass_name, 0, is_meta=True)
      elif klass_name in self._by_sname:
        t = self._by_sname[klass_name]
        t.klass.load_read_fields(fields)
      else:
        klass = type(klass_name, (Annotation, ), fields)
        t = Type(klass, klass_name, 0)

      t.nelem  = klass_nelem
      t.number = len(by_num)
      if t.klass:
        self._types[t.klass] = t
      self._by_sname[t.name] = t
      by_num.append(t)

    if meta_num == -1:
      raise ValueError('Invalid stream: could not find a __meta__ annotation type')

    # read in the instantiations
    instantiations = [None] * len(by_num)
    for _ in xrange(len(by_num)):
      klass_num = self._unpack()
      nbytes    = self._unpack()
      msg_objs  = self._unpack()

      t = by_num[klass_num]
      print 'reading annotations:', klass_num, t
      if t.is_meta:
        instantiations[meta_num] = msg_objs
      elif isinstance(msg_objs, dict):
        t.is_singleton = True
        obj = self._unserialize(msg_objs, t)
        instantiations[t.number] = obj
      else:
        objs = [None] * t.nelem
        for i, msg_obj in enumerate(msg_objs):
          obj = self._unserialize(msg_obj, t)
          obj._dr_index = i
          objs[i] = obj
        instantiations[t.number] = objs

    # decode the pointers
    for t in by_num:
      if t.is_meta:
        fields = meta_fields
      else:
        fields = t.klass._dr_fields

      # update the _klass attributes on the Field instances
      for pyname, field in fields.iteritems():
        if isinstance(field, Pointer) or (isinstance(field, Range) and field._klass):
          klass = by_num[field._klass].klass
          field._klass_name = klass._dr_name
          field._klass = klass

        if isinstance(field, Pointer) and not field.is_collection:
          if t.is_singleton:
            objs = (instantiations[t.number], )
          else:
            objs = instantiations[t.number]
          for obj in objs:
            if isinstance(obj, (tuple, list)):
              fields[pyname] = Pointers(field._klass, via=field.via)

      # update the annotation instances
      if t.is_singleton or t.is_meta:
        objs = (instantiations[t.number], )
      else:
        objs = instantiations[t.number]

      for pyname, field in fields.iteritems():
        # convert pointer integer offsets into object references
        if isinstance(field, Pointer):
          pt = self._types[field._klass]
          items = instantiations[pt.number]
          for obj in objs:
            val = getattr(obj, pyname)
            if val is not None:
              if field.is_collection:
                val = map(lambda p: unswizzle_ptr(p, items), val)
              else:
                val = unswizzle_ptr(val, items)
              setattr(obj, pyname, val)

        # convert ranges into Python slice objects
        if isinstance(field, Range):
          for obj in objs:
            r = getattr(obj, pyname)
            if r is not None:
              if len(r) != 2:
                raise ValueError('Length of a Range instance is not 2! (is {0})'.format(len(r)))
              setattr(obj, pyname, slice(r[0], r[1]))


    # instantiate a document
    if self._doc_klass is None:
      doc_fields = {}
      # add the annotation types
      for t in self._types.itervalues():
        name = t.plural or t.klass._dr_plural
        if t.is_singleton:
          doc_fields[name] = Singleton(t.klass, sname=t.name)
        else:
          doc_fields[name] = Annotations(t.klass, sname=t.name)
      # add the metadata fields
      doc_fields.update(meta_fields)
      self._doc_klass = type('Document', (Document, ), doc_fields)
      print doc_fields
      print self._doc_klass
    self._doc = self._doc_klass()


  def _unserialize(self, obj, t):
    vals = {}
    for index, val in obj.iteritems():
      vals[t.index_to_pyname(index)] = val
    return t.klass(**vals)

