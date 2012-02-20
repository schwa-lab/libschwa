
class Field(object):
  __slots__ = ('name', 'pointer_to')
  def __init__(self, name=None, pointer_to=None):
    self.name = name
    self.pointer_to = pointer_to


class Pointer(Field):
  def __init__(self, pointer_to, name=None):
    super(Pointer, self).__init__(name, pointer_to)


class Annotations(object):
  __slots__ = ('klass',)
  def __init__(self, klass):
    self.klass = klass
