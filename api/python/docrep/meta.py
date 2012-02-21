from .fields import Annotations, Field
from .utils import pluralise

__all__ = ['Annotation', 'Document', 'Token']


class DocrepMetaclass(type):
  def __new__(mklass, klass_name, bases, attrs, add_annotations):
    meta = attrs.get('Meta', None)

    # construct the class
    klass = super(DocrepMetaclass, mklass).__new__(mklass, klass_name, bases, attrs)
    klass._docrep_name = klass_name.split('.')[-1]

    # discover the Field and Annotations instances
    annotations, fields, fields_s2py = {}, {}, {}
    for base in bases:
      if hasattr(base, '_docrep_annotations'):
        annotations.update(base._docrep_annotations)
      if hasattr(base, '_docrep_fields'):
        fields.update(base._docrep_fields)
        fields_s2py.update(base._docrep_fields_s2py)
    for name, field in attrs.iteritems():
      if isinstance(field, Annotations):
        annotations[name] = field
      elif isinstance(field, Field):
        if field.name is None:
          field.name = name
        fields[name] = field
        fields_s2py[field.name] = name

    # adds the Field and Annotation information appropriately
    klass._docrep_fields = fields
    klass._docrep_fields_s2py = fields_s2py
    if add_annotations:
      klass._docrep_annotations = annotations

    # add the plural name
    if hasattr(meta, 'plural'):
      klass._docrep_plural = meta.plural
    else:
      klass._docrep_plural = pluralise(klass._docrep_name)

    # ordered stream field attributes
    klass._docrep_osfields = list(sorted(klass._docrep_fields_s2py))
    klass._docrep_osfields_inv = dict((v, i) for i, v in enumerate(klass._docrep_osfields))
    return klass


class AnnotationMetaclass(DocrepMetaclass):
  def __new__(mklass, klass_name, bases, attrs):
    return super(AnnotationMetaclass, mklass).__new__(mklass, klass_name, bases, attrs, False)


class DocumentMetaclass(DocrepMetaclass):
  def __new__(mklass, klass_name, bases, attrs):
    return super(DocumentMetaclass, mklass).__new__(mklass, klass_name, bases, attrs, True)


class Base(object):
  def __init__(self, **kwargs):
    self._docrep_is_set = set()
    for name in self._docrep_fields:
      self.__dict__[name] = None
    for k, v in kwargs.iteritems():
      if k in self._docrep_fields:
        setattr(self, k, v)

  def __setattr__(self, key, val):
    if key in self._docrep_fields:
      self._docrep_is_set.add(key)
    self.__dict__[key] = val

  @classmethod
  def sidx_for_pyname(klass, py_name):
    return klass._docrep_osfields_inv[klass._docrep_fields[py_name].name]

  @classmethod
  def pyname_for_sidx(klass, sidx):
    return klass._docrep_fields_s2py[klass._docrep_osfields[sidx]]

  @classmethod
  def update_osfields(klass, osfields):
    osfields = list(osfields)
    old_osfields = set(klass._docrep_osfields)
    new_osfields = set(osfields)

    for field in new_osfields - old_osfields:
      klass._docrep_fields[field] = Field(field)
      klass._docrep_fields_s2py[field] = field
    for field in old_osfields - new_osfields:
      osfields.append(field)

    klass._docrep_osfields = osfields
    klass._docrep_osfields_inv = dict((v, i) for i, v in enumerate(osfields))


class Annotation(Base):
  __metaclass__ = AnnotationMetaclass


class Document(Base):
  __metaclass__ = DocumentMetaclass

  def __init__(self, **kwargs):
    for name in self._docrep_annotations:
      self.__dict__[name] = []
    super(Document, self).__init__(**kwargs)


class Token(Annotation):
  begin = Field()
  end   = Field()
  raw   = Field()
  norm  = Field()

  def __repr__(self):
    return 'Token({0!r})'.format(self.norm)

  def __str__(self):
    return self.norm

