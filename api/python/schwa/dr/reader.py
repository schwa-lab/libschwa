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
    return self._is_collection

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
        klass = AnnotationMeta.cached(klass_name)
        v = klass_name if klass is None else klass
        if self.is_collection():
          self._dr_field = Pointers(v, sname=self._name)
        else:
          self._dr_field = Pointer(v, sname=self._name)
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

  def instantiate_instances(self):
    klass = self.klass()
    s2p = klass._dr_s2p
    for i in self._instances:
      vals = dict((s2p[k], v) for k, v in i.iteritems())
      yield klass(**vals)

  def get_pointer_fields(self):
    return [f for f in self._fields if f.is_pointer() and not f.is_range()]


class Reader(object):
  __slots__ = ('_doc_klass', '_unpacker', '_doc')

  def __init__(self, doc_klass=None):
    self._doc_klass = doc_klass
    if doc_klass:
      if not issubclass(doc_klass, Document):
        raise ValueError('"doc_klass" must be a subclass of Document')

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
    return obj

  def _read_doc(self):
    # attempt to read the header
    header = self._unpack() # [ ( name, nelem, [ { field_key : field_val } ] ) ]
    if header is None:
      self._doc = None
      return

    # decode the header
    wire_types, wire_meta = [], None
    for i, (klass_name, klass_nelem, klass_fields) in enumerate(header):
      t = WireType(i, klass_name, klass_nelem)
      for j, f in enumerate(klass_fields):
        t.add_field(WireField(j, f))
      wire_types.append(t)
      if t.is_meta:
        wire_meta = t

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

    # instantiate / create each of the required classes
    for t in wire_types:
      if not t.is_meta:
        t.klass()
    annotations = {}
    for t in wire_types:
      if not t.is_meta:
        klass = Singleton if t.is_singleton else Annotations
        annotations[t.collection_name()] = klass(t.klass())

    # add the Document fields
    doc_fields = annotations.copy()
    if wire_meta:
      for f in wire_meta.fields():
        doc_fields[f.name()] = f.dr_field()

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

    # instantiate the Document
    doc_vals = {}
    if wire_meta:
      doc_vals = wire_meta.get_instance()
    self._doc = self._doc_klass(**doc_vals)

    # instantiate all of the objects
    klass2collection_name = {}
    for t in wire_types:
      if t.is_meta:
        continue
      klass2collection_name[t.klass()] = t.collection_name()

      if t.is_singleton:
        objs = list(t.instantiate_instances())
        assert len(objs) == 1
        setattr(self._doc, t.collection_name(), objs[0])
      else:
        collection = getattr(self._doc, t.collection_name())
        collection.clear()
        objs = t.instantiate_instances()
        for obj in objs:
          collection.append(obj)

    # update the pointers
    for t in wire_types:
      if t.is_meta:
        continue
      objs = getattr(self._doc, t.collection_name())
      if t.is_singleton:
        objs = (objs, )

      for obj in objs:
        for f in t.get_pointer_fields():
          klass = f.dr_field()._klass
          collection = getattr(self._doc, klass2collection_name[klass])
          old = getattr(obj, f.name())
          if f.is_collection():
            new = [collection[i] for i in old]
          else:
            new = collection[old]
          setattr(obj, f.name(), new)

