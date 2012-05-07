# vim: set ts=2 et:
import StringIO

from .exceptions import DependencyException
from .fields import BaseAttr, BaseField, BaseStore, Field, Slice
from .utils import pluralise

__all__ = ['AnnotationMeta', 'Annotation', 'Document', 'Token']


class DocrepMeta(type):
  def __new__(mklass, klass_name, bases, attrs):
    meta = attrs.get('Meta', None)

    # construct the class
    klass = super(DocrepMeta, mklass).__new__(mklass, klass_name, bases, attrs)

    # discover the BaseField and BaseStore instances
    stores, fields, s2p = {}, {}, {}
    for base in bases:
      stores.update(getattr(base, '_dr_stores', {}))
      fields.update(getattr(base, '_dr_fields', {}))
      s2p.update(getattr(base, '_dr_s2p', {}))
    for name, attr in attrs.iteritems():
      if isinstance(attr, BaseAttr):
        if isinstance(attr, BaseStore):
          if attr.serial is None:
            attr.serial = attr.klass_name
          stores[name] = attr
        elif isinstance(attr, BaseField):
          if attr.serial is None:
            attr.serial = name
          fields[name] = attr
          s2p[attr.serial] = name

    # adds the Field and Store information appropriately
    klass._dr_fields = fields  # { pyname : Field }
    klass._dr_stores = stores  # { pyname : Store }
    klass._dr_s2p = s2p        # { serial : pyname }

    # add the name
    if hasattr(meta, 'name'):
      klass._dr_name = meta.name
    else:
      klass._dr_name = klass_name.split('.')[-1]

    # add the plural name
    if hasattr(meta, 'plural'):
      klass._dr_plural = meta.plural
    else:
      klass._dr_plural = pluralise(klass._dr_name)

    # add the dependency requirements fulfilled flag
    klass._dr_fulfilled = False

    # construct the docstring for the class
    doc = StringIO.StringIO()
    write_doc = False
    if klass.__doc__:
      doc.write(klass.__doc__ + '\n')
      write_doc = True
    doc.write('Docrep members for this class:\n')
    for name, field in sorted(klass._dr_fields.items()):
      doc.write('* ')
      doc.write(name)
      if field.help:
        doc.write(': ')
        doc.write(field.help)
        write_doc = True
      doc.write('\n')
    if write_doc:
      klass.__doc__ = doc.getvalue()

    return klass


class AnnotationMeta(DocrepMeta):
  reg     = {}  # { name : (sorted(attrs), klass) }
  unbound = {}  # { name : [ (Field, klass) ] }

  def __new__(mklass, klass_name, bases, attrs):
    klass = super(AnnotationMeta, mklass).__new__(mklass, klass_name, bases, attrs)
    klass = AnnotationMeta.register(klass)
    return klass

  @staticmethod
  def register(klass):
    fields = tuple(sorted(klass._dr_fields.keys() + klass._dr_stores.keys()))
    name = klass._dr_name

    # check if we have cached this class
    if name in AnnotationMeta.reg:
      f, k = AnnotationMeta.reg[name]
      if fields != f:
        raise ValueError('Cannot register two Annotation types {0!r} with the same name but with different fields ({1} != {2})'.format(name, f, fields))
      return k

    # register the class
    AnnotationMeta.reg[name] = (fields, klass)

    # update the dependency fulfilled information for the class
    for field_set in ('_dr_fields', '_dr_stores'):
      for field in getattr(klass, field_set).itervalues():
        if not field.is_fulfilled():
          dep = field.get_dependency()
          if dep in AnnotationMeta.reg:
            _, k = AnnotationMeta.reg[dep]
            field.set_dependency(k)
          else:
            if dep not in AnnotationMeta.unbound:
              AnnotationMeta.unbound[dep] = []
            AnnotationMeta.unbound[dep].append((field, klass))
    klass.update_fulfilled()

    # update fields which depend on this newly created class
    if name in AnnotationMeta.unbound:
      for f, k in AnnotationMeta.unbound[name]:
        f.set_dependency(klass)
        k.update_fulfilled()
      del AnnotationMeta.unbound[name]

    return klass

  @staticmethod
  def cached(klass_name):
    x = AnnotationMeta.reg.get(klass_name)
    return x and x[1]


class Base(object):
  __metaclass__ = AnnotationMeta

  def __init__(self, **kwargs):
    if not self._dr_fulfilled:
      raise DependencyException('Cannot instantiate class {0!r} with unfilled dependencies {1!r}'.format(self._dr_name, list(self.find_unfulfilled())))

    for name, field in self._dr_fields.iteritems():
      self.__dict__[name] = field.default()
    for name, store in self._dr_stores.iteritems():
      self.__dict__[name] = store.default()

    for k, v in kwargs.iteritems():
      setattr(self, k, v)

  @classmethod
  def from_wire(klass, **kwargs):
    for k, v in kwargs.iteritems():
      f = klass._dr_fields.get(k, None)
      if hasattr(f, 'from_wire'):
        kwargs[k] = f.from_wire(v)
    return klass(**kwargs)

  FIELD_MSG_TEMPLATE = 'Field {0!r} which refers to class name {1!r}'

  @classmethod
  def find_unfulfilled(klass):
    """
    Returns an unfulfilled field where available, otherwise None
    """
    for name, field in klass._dr_fields.iteritems():
      if not field.is_fulfilled():
        if hasattr(field, 'klass_name'):
          yield klass.FIELD_MSG_TEMPLATE.format(name, field.klass_name)
        else:
          yield name
    for name, store in klass._dr_stores.iteritems():
      if not store.is_fulfilled():
        if hasattr(store, 'klass_name'):
          yield klass.FIELD_MSG_TEMPLATE.format(name, store.klass_name)
        else:
          yield name

  @classmethod
  def update_fulfilled(klass):
    klass._dr_fulfilled = not any(klass.find_unfulfilled())

  @classmethod
  def update_fields(klass, fields):
    a = frozenset(klass._dr_s2p)
    b = frozenset(fields)
    for name in b - a:
      field = fields[name]
      if isinstance(field, BaseStore):
        klass._dr_stores[name] = field
      else:
        klass._dr_fields[name] = field
      klass._dr_s2p[name] = name


class Annotation(Base):
  class Meta:
    name = 'schwa.dr.Annotation'


class Document(Base):
  class Meta:
    name = 'schwa.dr.Document'

  def ready(self):
    """
    Hook called after a Document and all its Stores are loaded.
    """
    pass


class Token(Annotation):
  span = Slice()
  raw  = Field()
  norm = Field()

  class Meta:
    name = 'schwa.dr.Token'

  def __repr__(self):
    return 'Token({0!r})'.format(self.norm)

  def __str__(self):
    return self.norm
