# vim: set ts=2 et:
from .fields import Field

__all__ = ['DateTimeField', 'EncodedStringField']


class DateTimeField(Field):
  def to_wire(self, date):
    if date is not None:
      date = date.isoformat()
    return date

  def from_wire(self, data):
    import dateutil.parser
    if data is None:
      return None
    return dateutil.parser.parse(data)


class EncodedStringField(Field):
  __slots__ = ('encoding', )

  def __init__(self, encoding='utf-8', **kwargs):
    super(EncodedStringField, self).__init__(**kwargs)
    self.encoding = encoding

  def to_wire(self, string):
    if string is not None:
      string = string.encode(self.encoding)
    return string

  def from_wire(self, data):
    if data is None:
      return None
    return data.decode(self.encoding)
