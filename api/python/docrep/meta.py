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
    klass._docrep_fields = fields
    return klass


class AnnotationMetaclass(DocrepMetaclass):
  def __new__(mklass, name, bases, attrs):
    return super(AnnotationMetaclass, mklass).__new__(mklass, name, bases, attrs, Field)


class DocumentMetaclass(DocrepMetaclass):
  def __new__(mklass, name, bases, attrs):
    return super(DocumentMetaclass, mklass).__new__(mklass, name, bases, attrs, AnnotationField)


class Annotation(object):
  __metaclass__ = AnnotationMetaclass

  def __init__(self, **kwargs):
    for k, v in kwargs.iteritems():
      if k in self._docrep_fields:
        setattr(self, k, v)


class Document(object):
  __metaclass__ = DocumentMetaclass


