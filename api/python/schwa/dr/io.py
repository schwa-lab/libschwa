# vim: set ts=2 et:
from .constants import FIELD_TYPE_NAME, FIELD_TYPE_POINTER_TO, FIELD_TYPE_IS_SLICE
from .fields import Pointer, Slice

__all__ = ['Type', 'swizzle_ptr', 'types_from_doc']


class Type(object):
  __slots__ = ('klass', 'number', 'is_meta', 'fields', '_pyname2index', '_index2pyname')

  def __init__(self, klass, number, is_meta=False):
    self.klass = klass
    self.number = number
    self.is_meta = is_meta
    self.fields = []
    self._pyname2index = {}
    self._index2pyname = {}
    for i, (pyname, field) in enumerate(klass._dr_fields.iteritems()):
      self._pyname2index[pyname] = i
      self._index2pyname[i] = pyname
      f = {}
      f[FIELD_TYPE_NAME] = field.serial or pyname
      if isinstance(field, Pointer):
        f[FIELD_TYPE_POINTER_TO] = field._klass
      elif isinstance(field, Slice):
        f[FIELD_TYPE_IS_SLICE] = True
        if field._klass is not None:
          f[FIELD_TYPE_POINTER_TO] = field._klass
      self.fields.append(f)

  def __repr__(self):
    return 'Type({0!r})'.format(self.name())

  def __str__(self):
    return self.name()

  def name(self):
    return self.klass._dr_name

  def pyname_to_index(self, pyname):
    return self._pyname2index[pyname]

  def index_to_pyname(self, index):
    return self._index2pyname[index]


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
