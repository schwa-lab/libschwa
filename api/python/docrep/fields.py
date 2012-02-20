
class Field(object):
  __slots__ = ('name', 'pointer_to')

  def __init__(self, name=None, pointer_to=None):
    self.name = name
    self.pointer_to = pointer_to


class AnnotationField(object):
  __slots__ = ('name', 'is_singleton')

  def __init__(self, name, singleton=False):
    self.name = name
    self.is_singleton = singleton

