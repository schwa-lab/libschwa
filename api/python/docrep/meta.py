from .fields import AnnotationField, Field

class DocrepMetaclass(type):
  def __new__(mklass, name, bases, attrs, field_klass):
    fields = {}
    for base in bases:
      if hasattr(base, '_docrep_fields'):
        fields.update(base._docrep_fields)
    for k, v in attrs.iteritems():
      if isinstance(v, field_klass):
        fields[k] = v
        if v.name is None:
          v.name = k

    for k in fields:
      attrs[k] = None
    klass = super(DocrepMetaclass, mklass).__new__(mklass, name, bases, attrs)
    klass._docrep_name = name.split('.')[-1]
    klass._docrep_fields = fields
    klass._update_fields(())
    return klass


class AnnotationMetaclass(DocrepMetaclass):
  def __new__(mklass, name, bases, attrs):
    return super(AnnotationMetaclass, mklass).__new__(mklass, name, bases, attrs, Field)


class DocumentMetaclass(DocrepMetaclass):
  def __new__(mklass, name, bases, attrs):
    return super(DocumentMetaclass, mklass).__new__(mklass, name, bases, attrs, AnnotationField)


class Base(object):
  def __init__(self, **kwargs):
    self._docrep_set = [False] * len(self._docrep_fields)
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


class Token(Annotation):
  begin = Field()
  end   = Field()
  raw   = Field()
  norm  = Field()

  def __repr__(self):
    return self.norm

  def __str__(self):
    return self.norm


class Document(Base):
  __metaclass__ = DocumentMetaclass


