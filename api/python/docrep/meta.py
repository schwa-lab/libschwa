from .exceptions import DependencyException
from .fields import Annotations, BaseField, Field
from .utils import pluralise

__all__ = ['Annotation', 'Document', 'Token']


class DocrepMeta(type):
  def __new__(mklass, klass_name, bases, attrs, add_annotations):
    meta = attrs.get('Meta', None)

    # construct the class
    klass = super(DocrepMeta, mklass).__new__(mklass, klass_name, bases, attrs)
    klass._dr_name = klass_name.split('.')[-1]

    # discover the BaseField and Annotations instances
    annotations, fields = {}, {}
    for base in bases:
      if hasattr(base, '_dr_annotations'):
        annotations.update(base._dr_annotations)
      if hasattr(base, '_dr_fields'):
        fields.update(base._dr_fields)
    for name, field in attrs.iteritems():
      if isinstance(field, Annotations):
        annotations[name] = field
      elif isinstance(field, BaseField):
        if field.sname is None:
          field.sname = name
        fields[name] = field

    # adds the Field and Annotation information appropriately
    klass._dr_fields = fields
    if add_annotations:
      klass._dr_annotations = annotations

    # add the plural name
    if hasattr(meta, 'plural'):
      klass._dr_plural = meta.plural
    else:
      klass._dr_plural = pluralise(klass._dr_name)

    # set the dependency requirements flag
    klass._dr_fulfilled = False

    return klass


class AnnotationMeta(DocrepMeta):
  reg     = {} # { name : ( sorted(attrs), klass) }
  unbound = {} # { name : [ (Field, klass) ] }

  def __new__(mklass, name, bases, attrs):
    # create and register the class
    klass = super(AnnotationMeta, mklass).__new__(mklass, name, bases, attrs, False)
    fields = tuple(sorted(klass._dr_fields))

    # check if we have cached this class
    if name in mklass.reg:
      f, k = mklass.reg[name]
      if fields != f:
        raise ValueError('Cannot register two Annotation types {0!r} with the same name but with different fields ({1} != {2})'.format(name, f, fields))
      print 'returning cached class {0}: {1}'.format(name, k)
      del klass
      return k

    # register the class
    mklass.reg[name] = (fields, klass)
    print 'registering', name

    # update the dependency fulfilled information for the class
    for field in klass._dr_fields.itervalues():
      if not field.is_fulfilled():
        dep = field.get_dependency()
        if dep in mklass.reg:
          _, k = mklass.reg[dep]
          field.set_dependency(k)
        else:
          if dep not in mklass.unbound:
            mklass.unbound[dep] = []
          mklass.unbound[dep].append( (field, klass) )
    klass.update_fulfilled()

    # update fields which depend on this newly created class
    if name in mklass.unbound:
      for field, k in mklass.unbound[name]:
        field.set_dependency(klass)
        k.update_fulfilled()
      del mklass.unbound[name]

    return klass


class DocumentMeta(DocrepMeta):
  def __new__(mklass, name, bases, attrs):
    return super(DocumentMeta, mklass).__new__(mklass, name, bases, attrs, True)


class Base(object):
  def __init__(self, **kwargs):
    if not self._dr_fulfilled:
      raise DependencyException('Cannot instantiate a class with unfilled dependencies')

    for name, field in self._dr_fields.iteritems():
      self.__dict__[name] = field.default()
    for k, v in kwargs.iteritems():
      if k in self._dr_fields:
        setattr(self, k, v)

  @classmethod
  def update_fulfilled(klass):
    for field in klass._dr_fields.itervalues():
      if not field.is_fulfilled():
        klass._dr_fulfilled = False
        break
    else:
      klass._dr_fulfilled = True


class Annotation(Base):
  __metaclass__ = AnnotationMeta



class Document(Base):
  __metaclass__ = DocumentMeta

  def __init__(self, **kwargs):
    for name, annotations in self._dr_annotations.iteritems():
      self.__dict__[name] = annotations.default()
    super(Document, self).__init__(**kwargs)

