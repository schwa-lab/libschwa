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

    ofields = list(fields)
    ofields_inv = dict((v, i) for i, v in enumerate(ofields))

    for k in fields:
      attrs[k] = None
    klass = super(DocrepMetaclass, mklass).__new__(mklass, name, bases, attrs)
    klass._docrep_fields = fields
    klass._docrep_ofields = ofields
    klass._docrep_ofields_inv = ofields_inv
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
    print '_update_ofields', klass, fields


class Document(object):
  __metaclass__ = DocumentMetaclass


