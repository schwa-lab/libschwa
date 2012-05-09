# vim: set ts=2 et:
import cStringIO

import msgpack

from .constants import FIELD_TYPE_NAME, FIELD_TYPE_POINTER_TO, FIELD_TYPE_IS_SLICE
from .exceptions import StoreException
from .fields import Pointer, Slice
from .meta import Document

__all__ = ['Writer']


class Type(object):
  __slots__ = ('klass', 'number', 'is_meta', 'fields', '_p2i', '_i2p')

  def __init__(self, klass, number, is_meta=False):
    self.klass = klass
    self.number = number
    self.is_meta = is_meta
    self.fields = []
    self._p2i = {}  # { index : pyname }
    self._i2p = {}  # { pyname : index }
    for i, (pyname, field) in enumerate(klass._dr_fields.iteritems()):
      self._p2i[pyname] = i
      self._i2p[i] = pyname
      f = {}
      f[FIELD_TYPE_NAME] = field.serial or pyname
      if isinstance(field, Pointer):
        f[FIELD_TYPE_POINTER_TO] = field  # placeholder
      elif isinstance(field, Slice):
        f[FIELD_TYPE_IS_SLICE] = True
        if field._klass is not None:
          f[FIELD_TYPE_POINTER_TO] = field  # placeholder
      self.fields.append(f)

  def __repr__(self):
    return 'Type({0!r})'.format(self.name())

  def __str__(self):
    return self.name()

  def name(self):
    if self.is_meta:
      return '__meta__'
    return self.klass._dr_name

  def pyname_to_index(self, pyname):
    return self._p2i[pyname]

  def index_to_pyname(self, index):
    return self._i2p[index]


def types_from_doc(doc):
  """
  Find all of the types used by a document. Returns a dictionary, mapping from
  the Python classes to their corresponding Type instance.
  """
  types = {}  # { klass : Type }
  types[doc.__class__] = Type(doc.__class__, len(types), is_meta=True)
  for name, store in doc._dr_stores.iteritems():
    if store._klass not in types:
      types[store._klass] = Type(store._klass, len(types))
  return types


def swizzle_ptr(ptr):
  """
  Swizzles a Pointer instance. The ptr argument is assumed not to be None.
  """
  if not hasattr(ptr, '_dr_index'):
    raise ValueError('Cannot serialize a pointer to an object which is not managed by a Store')
  return ptr._dr_index


class Writer(object):
  __slots__ = ('_ostream', '_packer')

  def __init__(self, ostream):
    if not hasattr(ostream, 'write'):
        raise TypeError('"ostream" must have a write attr')
    self._ostream = ostream
    self._packer = msgpack.Packer()

  def write_doc(self, doc):
    if not isinstance(doc, Document):
      raise ValueError('You can only stream instances of Document')

    # run along each of the Stores and update the _dr_index attributes
    for name, store in doc._dr_stores.iteritems():
      val = getattr(doc, name)
      if store.is_collection():
        for i, obj in enumerate(val):
          obj._dr_index = i
      elif val:
        val._dr_index = 0

    # find all of the types defined
    types = types_from_doc(doc)  # { klass : Type }
    doc_type = types[doc.__class__]

    # create a mapping from Store names and classes to Store objects
    stores = {}  # { klass : store_id } \cup { pyname : store_id }
    for i, (pyname, store) in enumerate(doc._dr_stores.iteritems()):
      stores[pyname] = i
      if store._klass in stores:
        stores[store._klass].append(i)
      else:
        stores[store._klass] = [i]

    # construct the klasses header
    header = [None] * len(types)
    for klass, t in types.iteritems():
      fields = []
      for field in t.fields:
        f = field.copy()
        pointer_to = f.get(FIELD_TYPE_POINTER_TO)  # is a dr.Field instance
        if pointer_to is not None:
          if pointer_to.store:
            if pointer_to.store not in stores:
              raise StoreException('Store name {0!r} does not exist for field {1!r} of class {2}'.format(pointer_to.store, f[FIELD_TYPE_NAME], pointer_to._klass))
            store_id = stores[pointer_to.store]
          else:
            store_ids = stores.get(pointer_to._klass, [])
            if len(store_ids) == 1:
              store_id = store_ids[0]
            elif len(store_ids) == 0:
              raise StoreException('No Store was found for class {0} (for field {1!r})'.format(pointer_to._klass, f[FIELD_TYPE_NAME]))
            else:
              raise StoreException('The field {0!r} of class {1} needs to specify which Store it points to, as there are {2} potential Stores'.format(f[FIELD_TYPE_NAME], pointer_to._klass, len(store_ids)))
          f[FIELD_TYPE_POINTER_TO] = store_id
        fields.append(f)
      header[t.number] = (t.name(), fields)
    self._pack(header)

    # construct the stores header
    header = []
    for pyname, store in doc._dr_stores.iteritems():
      name = store.serial or pyname
      klass_id = types[store._klass].number
      nelem = 0 if not store.is_collection() else len(getattr(doc, pyname))
      header.append((name, klass_id, nelem))
    self._pack(header)

    # write out the document itself
    tmp = cStringIO.StringIO()
    self._pack(self._serialize(doc, doc_type), tmp)
    self._pack(len(tmp.getvalue()))
    self._ostream.write(tmp.getvalue())

    # write out each of the annotation sets
    for pyname, store in doc._dr_stores.iteritems():
      t = types[store._klass]
      tmp = cStringIO.StringIO()

      if store.is_collection():
        msg_objs = []
        for obj in getattr(doc, pyname):
          msg_objs.append(self._serialize(obj, t))
        self._pack(msg_objs, tmp)
      else:
        msg_obj = self._serialize(getattr(doc, pyname), t)
        self._pack(msg_obj, tmp)

      self._pack(len(tmp.getvalue()))
      self._ostream.write(tmp.getvalue())

  def _pack(self, obj, out=None):
    if out is None:
      out = self._ostream
    out.write(self._packer.pack(obj))

  def _serialize(self, obj, t):
    """
    Returns a Python object which represents the serialised form of object obj,
    which is of type t. The return value can be passed directly to the msgpack
    serialisation process -- it contains only values which msgpack can directly
    serialise.
    """
    msg_obj = {}
    for pyname, field in obj._dr_fields.iteritems():
      val = getattr(obj, pyname)
      if hasattr(field, 'to_wire'):
        val = field.to_wire(val)
      if val is None:
        continue
      if isinstance(field, Pointer):
        if field.is_collection:
          val = map(swizzle_ptr, val)
        else:
          val = swizzle_ptr(val)
      elif isinstance(field, Slice):
        val = (val.start, val.stop)
      elif isinstance(val, unicode):
        val = val.encode('utf-8')
      msg_obj[t.pyname_to_index(pyname)] = val
    return msg_obj
