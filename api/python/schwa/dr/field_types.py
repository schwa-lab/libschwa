# vim: set ts=2 et:
from .fields import *

__all__ = ['DateTimeField']


class DateTimeField(Field):
  def to_wire(self, date):
    if date is not None:
      date = date.isoformat()
    return date

  def from_wire(self, value):
    import dateutil.parser
    return dateutil.parser.parse(value)


