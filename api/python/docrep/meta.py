from .fields import Annotations, Field

__all__ = ['Annotation', 'Document', 'Token']


class DocrepMetaclass(type):
  def __new__(mklass, klass_name, bases, attrs, add_annotations):
    annotations = {}
    fields = {}
    for base in bases:
      if hasattr(base, '_docrep_annotations'):
        annotations.update(base._docrep_annotations)
      if hasattr(base, '_docrep_fields'):
        fields.update(base._docrep_fields)
    for name, field in attrs.iteritems():
      if isinstance(field, Annotations):
        annotations[name] = field
      elif isinstance(field, Field):
        if field.name is None:
          field.name = name
        fields[field.name] = field

    klass = super(DocrepMetaclass, mklass).__new__(mklass, klass_name, bases, attrs)
    klass._docrep_name = klass_name.split('.')[-1]
    klass._docrep_fields = fields
    if add_annotations:
      klass._docrep_annotations = annotations
    klass._update_fields(())
    return klass


class AnnotationMetaclass(DocrepMetaclass):
  def __new__(mklass, klass_name, bases, attrs):
    return super(AnnotationMetaclass, mklass).__new__(mklass, klass_name, bases, attrs, False)


class DocumentMetaclass(DocrepMetaclass):
  def __new__(mklass, klass_name, bases, attrs):
    return super(DocumentMetaclass, mklass).__new__(mklass, klass_name, bases, attrs, True)


class Base(object):
  def __init__(self, **kwargs):
    self._docrep_set = [False] * len(self._docrep_fields)
    for name in self._docrep_fields:
      self.__dict__[name] = None
    for k, v in kwargs.iteritems():
      if k in self._docrep_fields:
        setattr(self, k, v)

  def __setattr__(self, key, val):
    if key in self._docrep_fields:
      idx = self._docrep_ofields_inv[key]
      self._docrep_set[idx] = True
    self.__dict__[key] = val

  @classmethod
  def _update_fields(klass, fields):
    old_fields = set(klass._docrep_fields)
    new_fields = set(fields)
    for field in new_fields - old_fields:
      klass._docrep_fields[field] = Field(field)

    klass._docrep_ofields = list(klass._docrep_fields)
    klass._docrep_ofields_inv = dict((v, i) for i, v in enumerate(klass._docrep_ofields))


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

