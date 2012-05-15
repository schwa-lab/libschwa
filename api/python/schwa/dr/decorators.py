from .decoration import Decorator

def _attrsetter(attr):
  if attr is None:
    return lambda obj, val: None
  def fn(obj, val):
    setattr(obj, attr, val)
  # Set a default value like any other value
  fn.default = fn
  return fn


def _attrappender(attr):
  if attr is None:
    return lambda obj, val: None
  def fn(obj, val):
    try:
      getattr(obj, attr).append(val)
    except AttributeError:
      setattr(obj, attr, [val])

  # Do not set a default value, just initialise the list
  def default_fn(obj, val):
    setattr(obj, attr, [])
  fn.default = default_fn 
  return fn


class add_prev_next(Decorator):
  """Adds prev and next pointers or None where N/A. Also may add index."""
  def __init__(self, store, prev_attr='prev', next_attr='next', index_attr=None):
    super(add_prev_next, self).__init__(self._build_key(store, prev_attr, next_attr, index_attr))
    self.store = store
    self.set_prev = _attrsetter(prev_attr)
    self.set_next = _attrsetter(next_attr)
    self.set_index = _attrsetter(index_attr)

  def decorate(self, doc):
    prev = None
    for i, item in enumerate(getattr(doc, self.store)):
      self.set_prev(item, prev)
      self.set_index(item, i)
      if prev:
        self.set_next(prev, item)
      prev = item
    if prev:
      self.set_next(prev, None)


class materialise_slices(Decorator):
  """
  Decorates entries in the source_store with deref_attr, the list of elements
  in target_store corresponding to the slice_attr value.
  """
  def __init__(self, source_store, target_store, slice_attr, deref_attr):
    super(materialise_slices, self).__init__(self._build_key(source_store, target_store, slice_attr, deref_attr))
    self.source_store = source_store
    self.target_store = target_store
    self.slice_attr = slice_attr
    self.deref_attr = deref_attr

  def decorate(self, doc):
    store = getattr(doc, self.target_store)
    for obj in getattr(doc, self.source_store):
      span = getattr(obj, self.slice_attr)
      if span is not None:
        setattr(obj, self.deref_attr, store[span])


class reverse_slices(Decorator):
  """
  Where objects in source_store point (through slice_attr) to slices over
  objects in target_store, this decorates the target_store objects with any or
  all of: a pointer to a target_store object, its offset within the slice
  range, the appropriate BILOU label, a tuple of all the above.

  If slices are not mutually exclusive, each attribute will be a list whose
  items correspond to source annotations.
  """

  def __init__(self, source_store, target_store, slice_attr, pointer_attr=None, offset_attr=None, bilou_attr=None, all_attr=None, mutex=True, mark_outside=False):
    super(reverse_slices, self).__init__(self._build_key(source_store, target_store, slice_attr, pointer_attr, offset_attr, bilou_attr, all_attr, mutex, mark_outside))
    self.source_store = source_store
    self.target_store = target_store
    self.slice_attr = slice_attr
    if mutex:
      setter = _attrsetter
    else:
      setter = _attrappender
    self.set_pointer = setter(pointer_attr)
    self.set_offset = setter(offset_attr)
    self.set_bilou = setter(bilou_attr)
    self.set_all = setter(all_attr)
    self.mark_outside = mark_outside

  def decorate(self, doc):
    target_items = getattr(doc, self.target_store)

    if self.mark_outside:
      for target in target_items:
        self.set_pointer.default(target, None)
        self.set_offset.default(target, None)
        self.set_bilou.default(target, 'O')
        self.set_all.default(target, (None, None, 'O'))

    for source in getattr(doc, self.source_store):
      span = getattr(source, self.slice_attr)
      if not span:
        continue
      n = span.stop - span.start
      for i, target in enumerate(target_items[span]):
        self.set_pointer(target, source)
        self.set_offset(target, i)
        if n == 1:
          bilou = 'U'
        elif i == n - 1:
          bilou = 'L'
        elif i == 0:
          bilou = 'B'
        else:
          bilou = 'I'
        self.set_bilou(target, bilou)
        self.set_all(target, (source, i, bilou))


class reverse_pointers(Decorator):
  """
  Where objects in source_store point (through slice_attr) to objects in
  target_store, this decorates the target objects with a rev_attr attribute
  pointing back to the source object.
  """

  def __init__(self, source_store, target_store, pointer_attr, rev_attr, mutex=True, mark_outside=False):
    super(reverse_pointers, self).__init__(self._build_key(source_store, target_store, pointer_attr, rev_attr, mutex, mark_outside))
    self.source_store = source_store
    self.target_store = target_store
    self.pointer_attr = pointer_attr
    if mutex:
      setter = _attrsetter
    else:
      setter = _attrappender
    self.set_rev = setter(rev_attr)
    self.mark_outside = mark_outside

  def decorate(self, doc):
    if self.mark_outside:
      for target in getattr(doc, self.target_store):
        self.set_rev.default(target, None)

    for source in getattr(doc, self.source_store):
      target = getattr(source, self.pointer_attr)
      if not target:
        continue
      if isinstance(target, list):
        for target_item in target:
          self.set_rev(target_item, source)
      else:
        self.set_rev(target, source)

