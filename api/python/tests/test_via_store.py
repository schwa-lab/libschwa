# vim: set ts=2 et:
import unittest

from schwa import dr

from utils import write_read, write_x_read_y


class X(dr.Annotation):
  class Meta:
    name = 'via_store.X'

  value = dr.Field()


class Y(dr.Annotation):
  class Meta:
    name = 'via_store.Y'

  p = dr.Pointer(X, store='xs')


class Z(dr.Annotation):
  class Meta:
    name = 'via_store.Z'

  p = dr.Pointer(X, store='foo_xs')


class Doc(dr.Document):
  class Meta:
    name = 'via_store.Doc'

  xs = dr.Store(X)
  ys = dr.Store(Y)
  zs = dr.Store(Z)
  foo_xs = dr.Store(X)



class ViaStoreTest(unittest.TestCase):
  def test_simple(self):
    NXS = 10
    NFOO_XS = 7
    d1 = Doc()

    for i in xrange(NXS):
      d1.xs.create(value=i)
    for i in xrange(NFOO_XS):
      d1.foo_xs.create(value=100 + i)

    for i in xrange(20):
      d1.ys.create(p=d1.xs[i % NXS])
      d1.zs.create(p=d1.foo_xs[i % NFOO_XS])
    d1.ys.create()
    d1.zs.create()

    self.assertEqual(len(d1.ys), 21)
    self.assertIsNone(d1.ys[-1].p)
    self.assertEqual(len(d1.zs), 21)
    self.assertIsNone(d1.zs[-1].p)

    d2 = write_read(d1)
    print d2
