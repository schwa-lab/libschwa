
class Field(object):
  __slots__ = ('name', )

  def __init__(self, name=None):
    self.name = name


class AnnotationField(object):
  __slots__ = ('name', 'is_singleton')

  def __init__(self, name, singleton=False):
    self.name = name
    self.is_singleton = singleton

