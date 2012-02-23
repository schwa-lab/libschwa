
__all__ = ['Field', 'Pointer', 'Pointers', 'Range', 'Annotations', 'Singleton']


class BaseField(object):
  __slots__ = ('sname', )

  def __init__(self, **kwargs):
    self.sname = kwargs.get('name')

  def default(self):
    raise NotImplementedError

  def is_fulfilled(self):
    raise NotImplementedError

  def get_dependency(self):
    raise NotImplementedError

  def set_dependency(self, klass):
    raise NotImplementedError



class Field(BaseField):
  def default(self):
    return None

  def is_fulfilled(self):
    return True


class Pointer(BaseField):
  __slots__ = ('klass_name', 'via', 'is_collection', '_klass')

  def __init__(self, klass_name, **kwargs):
    super(Pointer, self).__init__(**kwargs)
    self.klass_name = klass_name.encode('utf-8')
    self.via = kwargs.get('via')
    self.is_collection = kwargs.get('is_collection', False)
    self._klass = None

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
    return []


class Range(BaseField):
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

class BaseAnnotations(object):
  __slots__ = ('sname', )

  def __init__(self, **kwargs):
    self.sname = kwargs.get('name')

  def default(self):
    raise NotImplementedError

  def is_fulfilled(self):
    raise NotImplementedError


class Annotations(BaseAnnotations):
  __slots__ = ('klass_name', '_klass')

  def __init__(self, klass_name, **kwargs):
    super(Annotations, self).__init__(**kwargs)
    self.klass_name = klass_name

  def default(self):
    return []

  def is_fulfilled(self):
    return self._klass is not None


class Singleton(Annotations):
  def default(self):
    return None

