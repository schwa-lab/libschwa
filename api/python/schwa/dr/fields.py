# vim: set ts=2 et:
from .collections import StoreList

__all__ = ['BaseField', 'BaseAnnotationField', 'Field', 'Pointer', 'Pointers', 'Range', 'BaseStore', 'Store', 'Singleton']


class BaseField(object):
  __slots__ = ('serial', )

  def __init__(self, **kwargs):
    self.serial = kwargs.get('serial')

  def default(self):
    """
    Returns the default value for this type when it is instantiated.
    """
    raise NotImplementedError

  def is_fulfilled(self):
    raise NotImplementedError

  def get_dependency(self):
    raise NotImplementedError

  def set_dependency(self, klass):
    raise NotImplementedError


# =============================================================================
# =============================================================================
class BaseAnnotationField(BaseField):
  pass


class Field(BaseAnnotationField):
  def default(self):
    return None

  def is_fulfilled(self):
    return True


class Pointer(BaseAnnotationField):
  __slots__ = ('klass_name', 'via', 'is_collection', '_klass')

  def __init__(self, klass_name, **kwargs):
    super(Pointer, self).__init__(**kwargs)
    if isinstance(klass_name, (str, unicode)):
      self.klass_name = klass_name.encode('utf-8')
      self._klass = None
    else:
      self.klass_name = klass_name._dr_name
      self._klass = klass_name
    self.via = kwargs.get('via')
    self.is_collection = kwargs.get('is_collection', False)

  def default(self):
    return None

  def is_fulfilled(self):
    return self._klass is not None

  def get_dependency(self):
    return self.klass_name

  def set_dependency(self, klass):
    self._klass = klass


class Pointers(Pointer):
  def __init__(self, klass_name, **kwargs):
    kwargs['is_collection'] = True
    super(Pointers, self).__init__(klass_name, **kwargs)

  def default(self):
    assert self._klass is not None
    return StoreList(self._klass)


class Range(BaseAnnotationField):
  __slots__ = ('klass_name', 'via', '_klass')

  def __init__(self, klass_name=None, **kwargs):
    super(Range, self).__init__(**kwargs)
    self.klass_name = klass_name and klass_name.encode('utf-8')
    self.via = kwargs.get('via')
    self._klass = None

  def default(self):
    return None

  def is_pointer(self):
    return self.klass_name is not None

  def is_fulfilled(self):
    if self.is_pointer():
      return self._klass is not None
    return True

  def get_dependency(self):
    return self.klass_name

  def set_dependency(self, klass):
    self._klass = klass


# =============================================================================
# =============================================================================
class BaseStore(BaseField):
  def is_collection(self):
    return True


class Store(BaseStore):
  """
  A Store houses BaseAnnotationField instances. For a BaseAnnotationField to be
  serialised, it needs to be placed into a Store.
  """
  __slots__ = ('klass_name', '_klass')

  def __init__(self, klass_name, **kwargs):
    super(Store, self).__init__(**kwargs)
    if isinstance(klass_name, (str, unicode)):
      self.klass_name = klass_name.encode('utf-8')
      self._klass = None
    else:
      self.klass_name = klass_name._dr_name
      self._klass = klass_name

  def default(self):
    assert self._klass is not None
    return StoreList(self._klass)

  def is_fulfilled(self):
    return self._klass is not None

  def get_dependency(self):
    return self.klass_name

  def set_dependency(self, klass):
    self._klass = klass


class Singleton(Store):
  def default(self):
    return None

  def is_collection(self):
    return False
