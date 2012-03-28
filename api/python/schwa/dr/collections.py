
class AnnotationsList(object):
  __slots__ = ('_items', '_klass')

  def __init__(self, klass):
    self._items = []
    self._klass = klass

  def __iadd__(self, other):
    for obj in other:
      self.append(obj)
    return self

  def __getitem__(self, index):
    return self._items[index]

  def __iter__(self):
    return iter(self._items)

  def __len__(self):
    return len(self._items)

  def __repr__(self):
    return 'AnnotationsList({0!r})'.format(self._items)

  def __str__(self):
    return str(self._items)

  def append(self, item):
    item._dr_index = len(self._items)
    self._items.append(item)

  def clear(self):
    del self._items
    self._items = []

  def index(self, item):
    return self._items.index(item)

  def create(self, **kwargs):
    obj = self._klass(**kwargs)
    self.append(obj)
    return obj

