# vim: set ts=2 et:
import msgpack

from .constants import FIELD_TYPE_NAME, FIELD_TYPE_POINTER_TO, FIELD_TYPE_IS_SLICE
from .fields import Field, Pointer, Pointers, Singleton, Slice, Store
from .meta import Annotation, AnnotationMeta, Document

__all__ = ['Reader']


def serialised_instance_to_dict(obj, wire_type):
  """
  Converts a serialised instance obj to a dictionary which can be
  used as the kwargs for the constructor of the class represented
  by wire_type.
  """
  instance = {}  # { serial : val }
  for f in wire_type.fields:
    val = obj.get(f.number)
    if val is not None:
      if f.is_range:
        assert len(val) == 2
        val = slice(val[0], val[1])
      if f.is_pointer and isinstance(val, (list, tuple)):
        f.is_collection = True
      instance[f.name] = val
  return instance


def instantiate_instance(instance, klass):
  """
  Converts a dictionary returned by serialised_instance_to_dict into an actual
  Python object of type klass
  """
  s2p = klass._dr_s2p
  vals = dict((s2p[k], v) for k, v in instance.iteritems())
  return klass.from_wire(**vals)


class WireStore(object):
  __slots__ = ('name', 'nelem', 'wire_type', 'is_collection', '_instances', 'store')

  def __init__(self, name, nelem, wire_type):
    self.name = name
    self.nelem = nelem
    self.wire_type = wire_type
    self.is_collection = True
    self._instances = []
    self.store = None

  def add_instance(self, obj):
    instance = serialised_instance_to_dict(obj, self.wire_type)
    self._instances.append(instance)

  def instantiate_instances(self):
    klass = self.wire_type.klass()
    for i in self._instances:
      yield instantiate_instance(i, klass)


class WireField(object):
  __slots__ = ('number', 'name', 'pointer_to', 'is_range', 'is_collection', '_dr_field')

  def __init__(self, number, field):
    self.number = number
    self.name = field[FIELD_TYPE_NAME]
    self.pointer_to = field.get(FIELD_TYPE_POINTER_TO)
    self.is_range = FIELD_TYPE_IS_SLICE in field
    self.is_collection = False
    self._dr_field = None

  def __repr__(self):
    return 'WireField({0!r})'.format(self.name)

  def __str__(self):
    return self.name

  @property
  def is_pointer(self):
    return self.pointer_to is not None

  def dr_field(self):
    if self._dr_field is None:
      if self.is_range:
        if self.is_pointer:
          store = self.pointer_to.name
          klass = self.pointer_to.wire_type.name
          self._dr_field = Slice(klass, store=store, serial=self.name)
        else:
          self._dr_field = Slice(serial=self.name)
      elif self.is_pointer:
        store = self.pointer_to.name
        klass = self.pointer_to.wire_type.name
        if self.is_collection:
          self._dr_field = Pointers(klass, store=store, serial=self.name)
        else:
          self._dr_field = Pointer(klass, store=store, serial=self.name)
      else:
        self._dr_field = Field(serial=self.name)
    return self._dr_field


class WireType(object):
  __slots__ = ('number', 'name', 'fields', 'pointer_fields', 'is_meta', '_klass')

  by_number = {}

  def __init__(self, number, name, klass_fields):
    self.number = number
    self.name = name
    self.fields = [WireField(i, f) for i, f in enumerate(klass_fields)]
    self.pointer_fields = [f for f in self.fields if f.is_pointer and not f.is_range]
    self.is_meta = name == '__meta__'
    self._klass = None
    WireType.by_number[number] = self

  def __repr__(self):
    return 'WireType({0!r})'.format(self.name)

  def __str__(self):
    return self.name

  def create_klass(self, doc_klass=None):
    if self._klass is not None:
      return

    if self.is_meta:
      if doc_klass is None:
        klass_name = 'Document'
        klass = AnnotationMeta.cached(klass_name)
      else:
        klass = doc_klass
    else:
      klass_name = self.name
      klass = AnnotationMeta.cached(klass_name)

    dr_fields = dict((f.name, f.dr_field()) for f in self.fields)
    if klass is None:
      klass = type(klass_name, (Annotation, ), dr_fields)
    else:
      klass.update_attrs(dr_fields)
    self._klass = klass
    return self._klass

  def klass(self):
    return self._klass


class Reader(object):
  __slots__ = ('_doc_klass', '_unpacker', '_doc')

  def __init__(self, doc_klass=None):
    self._doc_klass = doc_klass
    if doc_klass and not issubclass(doc_klass, Document):
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

  def _update_pointers(self, objs, pointer_fields):
    for obj in objs:
      for field in pointer_fields:
        old = getattr(obj, field.name)
        if old is None:
          continue
        store = getattr(self._doc, field.pointer_to.name)
        if field.is_collection:
          new = [store[i] for i in old]
        else:
          new = store[old]
        setattr(obj, field.name, new)

  def _read_doc(self):
    # attempt to read the header
    header = self._unpack()  # [ ( name, [ { field_key : field_val } ] ) ]
    if header is None:
      self._doc = None
      return

    # decode the klasses header
    wire_types, wire_meta = [], None
    for i, (klass_name, klass_fields) in enumerate(header):
      t = WireType(i, klass_name, klass_fields)
      wire_types.append(t)
      if t.is_meta:
        wire_meta = t
    assert wire_meta is not None

    # decode the stores header
    header = self._unpack()  # [ ( store_name, klass_id, store_nelem ) ]
    wire_stores = []
    for name, klass_id, nelem in header:
      wire_type = WireType.by_number[klass_id]
      wire_stores.append(WireStore(name, nelem, wire_type))

    # update the POINTER_TO values in the WireFields to point to their corresponding WireStore objects
    for t in wire_types:
      for f in t.fields:
        if f.is_pointer:
          f.pointer_to = wire_stores[f.pointer_to]

    # instantiate / create each of the required classes
    for t in wire_types:
      if not t.is_meta:
        t.create_klass()

    # decode the document instance
    self._unpack()  # nbytes (unused in the Python API)
    doc_blob = self._unpack()
    assert isinstance(doc_blob, dict)

    # decode each of the instances groups
    for s in wire_stores:
      self._unpack()  # nbytes (unused in the Python API)
      blob = self._unpack()

      if isinstance(blob, dict):
        s.is_collection = False
        s.add_instance(blob)
      else:
        assert isinstance(blob, (list, tuple))
        for obj in blob:
          s.add_instance(obj)

    # instantiate each of the Stores
    stores = {}
    for s in wire_stores:
      klass = s.wire_type.klass()
      storage = Store if s.is_collection else Singleton
      stores[s.name] = s.store = storage(klass)

    # create and update the Document type
    self._doc_klass = wire_meta.create_klass(self._doc_klass)
    self._doc_klass.update_attrs(stores)

    # instantiate the Document
    doc_vals = serialised_instance_to_dict(doc_blob, wire_meta)
    self._doc = instantiate_instance(doc_vals, self._doc_klass)

    # instantiate all of the instances
    for s in wire_stores:
      if s.is_collection:
        store = getattr(self._doc, s.name)
        store.clear()
        for obj in s.instantiate_instances():
          store.append(obj)
      else:
        objs = tuple(s.instantiate_instances())
        assert len(objs) == 1
        setattr(self._doc, s.name, objs[0])

    # update the pointers on the document
    pointer_fields = wire_meta.pointer_fields
    if pointer_fields:
      self._update_pointers((self._doc, ), pointer_fields)

    # update the pointers on the instances
    for s in wire_stores:
      pointer_fields = s.wire_type.pointer_fields
      if pointer_fields:
        objs = getattr(self._doc, s.name)
        if not s.is_collection:
          objs = (objs, )
        self._update_pointers(objs, pointer_fields)

    # call post-reading hook.
    self._doc.ready()
