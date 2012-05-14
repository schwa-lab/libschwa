from .decoration import Decorator

def materialise_slices(source_store, target_store, slice_attr, deref_attr):
  """
  Decorates entries in the source_store with deref_attr, the list of elements
  in target_store corresponding to the slice_attr value.
  """
  @dr.decorator('materialise_slices-{}-{}-{}-{}'.format(source_store, target_store, slice_attr, deref_attr))
  def decorate(doc):
    store = getattr(doc, target_store)
    for obj in getattr(doc, source_store):
      span = getattr(obj, slice_attr)
      if span is not None:
        setattr(obj, deref_attr, store[span])
  return decorate


class materialise_slices(Decorator):
  def __init__(self, source_store, target_store, slice_attr, deref_attr):
    super(materialise_slices, self).__init__('materialise_slices-{}-{}-{}-{}'.format(source_store, target_store, slice_attr, deref_attr))
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

