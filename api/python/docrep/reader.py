import msgpack

from .constants import *
from .fields import *
from .io import *
from .meta import AnnotationMeta, Annotation, Document
from .utils import *

__all__ = ['Reader']


class WireField(object):
  __slots__ = ('_number', '_name', '_pointer_num', '_is_range', '_is_collection', '_dr_field')

  def __init__(self, number, field):
    self._number = number
    self._name = field[FIELD_TYPE_NAME]
    self._pointer_num = field.get(FIELD_TYPE_POINTER_TO)
    self._is_range = FIELD_TYPE_IS_RANGE in field
    self._is_collection = False
    self._dr_field = None

  def __repr__(self):
    return 'WireField({0!r})'.format(self._name)

  def __str__(self):
    return repr(self)

  def number(self):
    return self._number

  def name(self):
    return self._name

  def is_collection(self):
    return self.is_collection

  def is_pointer(self):
    return self._pointer_num is not None

  def is_range(self):
    return self._is_range

  def pointer_num(self):
    return self._pointer_num

  def set_is_collection(self, val):
    self._is_collection = val

  def dr_field(self):
    if self._dr_field is None:
      if self.is_range():
        if self.is_pointer():
          klass_name = WireType.by_number[self._pointer_num].name()
          self._dr_field = Range(klass_name, sname=self._name)
        else:
          self._dr_field = Range(sname=self._name)
      elif self.is_pointer():
        klass_name = WireType.by_number[self._pointer_num].name()
        if self.is_collection():
          self._dr_field = Pointers(klass_name, sname=self._name)
        else:
          self._dr_field = Pointer(klass_name, sname=self._name)
      else:
        self._dr_field = Field(sname=self._name)
    return self._dr_field


class WireType(object):
  by_number = {}
  by_name   = {}

  def __init__(self, number, name, nelem):
    self._number = number
    self._name = name
    self._nelem = nelem
    self._fields = []
    self._instances = []
    self._klass = None
    self.is_meta = name == '__meta__'
    self.is_singleton = False
    WireType.by_number[number] = self
    WireType.by_name[name] = self

  def __repr__(self):
    return 'WireType({0!r})'.format(self._name)

  def __str__(self):
    return repr(self)

  def number(self):
    return self._number

  def name(self):
    return self._name

  def nelem(self):
    return self._nelem

  def fields(self):
    return self._fields

  def get_instance(self):
    assert len(self._instances) == 1
    return self._instances[0]

  def add_field(self, field):
    self._fields.append(field)

  def add_instance(self, obj):
    instance = {} # { sname : val }
    for f in self._fields:
      val = obj.get(f.number())
      if val is not None:
        if f.is_range():
          assert len(val) == 2
          val = slice(val[0], val[1])
        if f.is_pointer() and isinstance(val, (list, tuple)):
          f.set_is_collection(True)
        instance[f.name()] = val
    self._instances.append(instance)

  def klass(self):
    if self._klass is None:
      dr_fields = dict((f.name(), f.dr_field()) for f in self._fields)
      klass = AnnotationMeta.cached(self._name)
      if klass is None:
        klass = type(self._name, (Annotation, ), dr_fields)
      else:
        klass.update_fields(dr_fields)
      self._klass = klass
    return self._klass

  def collection_name(self):
    if self.is_singleton:
      return to_lower(self._name)
    return self._klass._dr_plural


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
    wire_types, wire_meta = [], None
    for i, (klass_name, klass_nelem, klass_fields) in enumerate(header):
      t = WireType(i, klass_name, klass_nelem)
      for j, f in enumerate(klass_fields):
        t.add_field(WireField(j, f))
      wire_types.append(t)
      if t.is_meta:
        wire_meta = t

    print '<'*10
    for t in wire_types:
      print t.name()
      for f in t.fields():
        print '  ', f.name(), f.is_range(), f.is_pointer(), f.pointer_num()
    print '>'*10

    # decode each of the annotation sets
    for _ in xrange(len(wire_types)):
      klass_num = self._unpack()
      nbytes    = self._unpack()
      blob      = self._unpack()
      t = wire_types[klass_num]

      if isinstance(blob, dict):
        if not t.is_meta:
          t.is_singleton = True
        t.add_instance(blob)
      else:
        assert isinstance(blob, (list, tuple))
        for obj in blob:
          t.add_instance(obj)

    annotations = {}
    for t in wire_types:
      if t.is_meta:
        continue
      annotations[t.collection_name()] = t.klass()
    print 'annotations', annotations

    doc_fields = annotations.copy()
    if wire_meta:
      for f in wire_meta.fields():
        doc_fields[f.name()] = f.dr_field()
    print 'doc_fields', doc_fields

    # create or update the Document class
    if self._doc_klass is None:
      klass = AnnotationMeta.cached('Document')
      if klass is None:
        klass = type('Document', (Document, ), doc_fields)
      else:
        klass.update_fields(doc_fields)
      self._doc_klass = klass
    else:
      self._doc_klass.update_fields(doc_fields)

    # instantiate the document
    doc_vals = {}
    if wire_meta:
      doc_vals = wire_meta.get_instance()
    self._doc = self._doc_klass(**doc_vals)
    print 'self._doc', self._doc

    import sys
    sys.exit(1)

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

