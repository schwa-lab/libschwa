"""
Utilities for managing document decoration by marking the document with the set of decorations that have been applied to it.
"""

from functools import wraps, partial


def decorator(key=None):
  """
  Wraps a docrep decorator, ensuring it is only executed once per document.
  Duplication is checked using the given key or the function object.
  """
  def dec(fn):
    @wraps(fn)
    def wrapper(doc):
      try:
        if key in doc._decorated_by:
            return
      except AttributeError:
        doc._decorated_by = set()
      doc._decorated_by.add(key)
      fn(doc)
    return wrapper
  if callable(key):
    return dec(key)
  return dec


class Decorator(object):
  """
  An abstract document decorator, which wraps its decorate method to ensure it is only executed once per document.
  """

  def __init__(self, key):
    # NOTE: wrapping __call__ like this didn't seem to work
    self.decorate = decorator(key)(self.decorate)

  @classmethod
  def _build_key(cls, *args):
    return '{}-{}'.format(cls.__name__, '-'.join(repr(arg) for arg in args))

  def __call__(self, doc):
    self.decorate(doc)

  def decorate(self, doc):
    raise NotImplementedError()


def requires_decoration(*decorators, **kwargs):
  """
  Marks the document decoration dependencies for a function, where the
  document is found in the doc_arg positional argument (default 0) or
  doc_kwarg keyword argument (default 'doc').
  """
  doc_arg = kwargs.pop('doc_arg', 0)
  doc_kwarg = kwargs.pop('doc_kwarg', 'doc')
  if kwargs:
    raise ValueError("Got unexpected keyword arguments: {}".format(kwargs.keys()))
  def dec(fn):
    @wraps(fn)
    def wrapper(*args, **kwargs):
      try:
        doc = args[doc_arg]
      except IndexError:
        doc = kwargs[doc_kwarg]

      for decorate in decorators:
        decorate(doc)
      return fn(*args, **kwargs)
    return wrapper
  return dec


method_requires_decoration = partial(requires_decoration, doc_arg=1)
