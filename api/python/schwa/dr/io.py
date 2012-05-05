# vim: set ts=2 et:
from .constants import FIELD_TYPE_NAME, FIELD_TYPE_POINTER_TO, FIELD_TYPE_IS_RANGE
from .fields import Pointer, Range

__all__ = ['Type', 'swizzle_ptr', 'types_from_doc']


class Type(object):
  __slots__ = ('klass', 'number', 'nelem', 'plural', 'name', 'fields', 'pyname2index', 'index2pyname', 'is_meta', 'is_singleton')

  def __init__(self, klass, serial, number, nelem=0, plural=None, is_meta=False, is_singleton=False):
    self.klass  = klass
    self.number = number
    self.nelem  = nelem
    self.name   = serial or klass._dr_name
    self.plural = plural
    self.fields = []
    self.pyname2index = {}
    self.index2pyname = {}
    self.is_meta = is_meta
    self.is_singleton = is_singleton
    if klass:
      for pyname, field in klass._dr_fields.iteritems():
        index = len(self.fields)
        self.pyname2index[pyname] = index
        self.index2pyname[index] = pyname
        f = {}
        f[FIELD_TYPE_NAME] = field.serial or pyname
        if isinstance(field, Pointer):
          f[FIELD_TYPE_POINTER_TO] = field._klass
        elif isinstance(field, Range):
          f[FIELD_TYPE_IS_RANGE] = True
          if field._klass is not None:
            f[FIELD_TYPE_POINTER_TO] = field._klass
        self.fields.append(f)

  def __repr__(self):
    return 'Type({0!r})'.format(self.name)

  def __str__(self):
    return self.name

  def pyname_to_index(self, pyname):
    return self.pyname2index[pyname]

  def index_to_pyname(self, index):
    return self.index2pyname[index]


def types_from_doc(doc):
  # find all of the types defined
  types = {}  # { klass : Type }
  types[doc.__class__] = Type(doc.__class__, '__meta__', len(types), is_meta=True)
  for name, store in doc._dr_stores.iteritems():
    t = Type(store._klass, store.serial, len(types), 0, name)
    if store.is_collection():
      t.nelem = len(getattr(doc, name))
    else:
      t.is_singleton = True
    types[store._klass] = t
  return types


def swizzle_ptr(ptr):
  if not hasattr(ptr, '_dr_index'):
    raise ValueError('Cannot serialize a pointer to an object which is not managed by a Store')
  return ptr._dr_index
