
from unittest import TestCase

from schwa import dr

class Document(dr.Document):
  annots = dr.Store('MyAnnot')
  slices = dr.Store('SliceAnnot')

class MyAnnot(dr.Annotation):
  field = dr.Field()
  children = dr.Pointers('MyAnnot')
  child = dr.Pointer('MyAnnot')

  def __repr__(self):
    return '{}(field={}, children={}, child={})'.format(self.__class__.__name__, self.field, self.children, self.child)

class SliceAnnot(dr.Annotation):
  span = dr.Slice('MyAnnot')
  name = dr.Field()

  def __repr__(self):
    return '{}(span={}, name={})'.format(self.__class__.__name__, self.span, self.name)


class SliceDecoratorsTest(TestCase):
  def setUp(self):
    self.doc = Document()
    for val in '0123456':
      self.doc.annots.create(field=val)
    self.doc.slices.create(span=slice(1, 4), name='Long slice')
    self.doc.slices.create(span=slice(5, 6), name='Unit slice')

  def test_materialise_slices(self):
    decorate = dr.decorators.materialise_slices('slices', 'annots', 'span', 'annots')
    for sl in self.doc.slices:
      self.assertFalse(hasattr(sl, 'annots'))

    decorate(self.doc)

    self.assertEqual('123', ''.join(a.field for a in self.doc.slices[0].annots))
    self.assertEqual('5', ''.join(a.field for a in self.doc.slices[1].annots))

  def test_reverse_mutually_exclusive_slices(self, mark_outside=False):
    decorate = dr.decorators.reverse_slices('slices', 'annots', 'span', pointer_attr='slice_pointer', offset_attr='slice_offset', bilou_attr='slice_bilou', all_attr='slice_all', mutex=True, mark_outside=mark_outside)
    for a in self.doc.annots:
      self.assertFalse(hasattr(a, 'slice_pointer'))
      self.assertFalse(hasattr(a, 'slice_offset'))
      self.assertFalse(hasattr(a, 'slice_bilou'))
      self.assertFalse(hasattr(a, 'slice_all'))

    decorate(self.doc)
    annots = self.doc.annots
    slices = self.doc.slices

    EXPECTED = {
      1: (slices[0], 0, 'B'),
      2: (slices[0], 1, 'I'),
      3: (slices[0], 2, 'L'),
      5: (slices[1], 0, 'U'),
    }
    EXPECTED_OUTSIDE = (None, None, 'O')

    for i, a in enumerate(annots):
      if i in EXPECTED:
        self.assertEqual((a.slice_pointer, a.slice_offset, a.slice_bilou), EXPECTED[i])
        self.assertEqual(a.slice_all, EXPECTED[i])
      elif mark_outside:
        self.assertEqual((a.slice_pointer, a.slice_offset, a.slice_bilou), EXPECTED_OUTSIDE)
        self.assertEqual(a.slice_all, EXPECTED_OUTSIDE)
      else:
        self.assertFalse(hasattr(a, 'slice_pointer'))
        self.assertFalse(hasattr(a, 'slice_offset'))
        self.assertFalse(hasattr(a, 'slice_bilou'))
        self.assertFalse(hasattr(a, 'slice_all'))

  def test_reverse_mutually_exclusive_slices_with_o(self):
    self.test_reverse_mutually_exclusive_slices(mark_outside=True)

  def test_reverse_slices_no_attrs(self):
    # When no attributes are specified, nothing should not be set
    decorate = dr.decorators.reverse_slices('slices', 'annots', 'span')
    decorate(self.doc)
    # Nothing exploded? Great!

  def test_reverse_overlapping_slices(self, mark_outside=False):
    self.doc.slices.create(span=slice(0, 4), name='Overlapping')
    decorate = dr.decorators.reverse_slices('slices', 'annots', 'span', 'slice_pointer', 'slice_offset', 'slice_bilou', 'slice_all', mutex=False, mark_outside=mark_outside)

    for a in self.doc.annots:
      self.assertFalse(hasattr(a, 'slice_pointer'))
      self.assertFalse(hasattr(a, 'slice_offset'))
      self.assertFalse(hasattr(a, 'slice_bilou'))
      self.assertFalse(hasattr(a, 'slice_all'))

    decorate(self.doc)
    annots = self.doc.annots
    slices = self.doc.slices
    EXPECTED = {
      0: [(slices[2], 0, 'B')],
      1: [(slices[2], 1, 'I'), (slices[0], 0, 'B')],
      2: [(slices[2], 2, 'I'), (slices[0], 1, 'I')],
      3: [(slices[2], 3, 'L'), (slices[0], 2, 'L')],
      5: [(slices[1], 0, 'U')],
    }

    for i, a in enumerate(annots):
      if i in EXPECTED:
        self.assertEqual(set(a.slice_pointer), set(p for p, o, b in EXPECTED[i]))
        self.assertEqual(set(a.slice_offset), set(o for p, o, b in EXPECTED[i]))
        self.assertEqual(set(a.slice_bilou), set(b for p, o, b in EXPECTED[i]))
        self.assertEqual(set(a.slice_all), set(EXPECTED[i]))
      elif mark_outside:
        self.assertEqual(len(a.slice_pointer), 0)
        self.assertEqual(len(a.slice_offset), 0)
        self.assertEqual(len(a.slice_bilou), 0)
        self.assertEqual(len(a.slice_all), 0)
      else:
        self.assertFalse(hasattr(a, 'slice_pointer'))
        self.assertFalse(hasattr(a, 'slice_offset'))
        self.assertFalse(hasattr(a, 'slice_bilou'))
        self.assertFalse(hasattr(a, 'slice_all'))

  def test_reverse_overlapping_slices_with_o(self):
    self.test_reverse_overlapping_slices(mark_outside=True)


class PointerDecoratorTest(TestCase):
  def setUp(self):
    self.doc = Document()
    self.doc.annots.create(field='0')
    self.doc.annots.create(field='1', child=self.doc.annots[0])
    self.doc.annots.create(field='2', children=[self.doc.annots[0], self.doc.annots[1]])

  def test_reverse_mutually_exclusive_pointer(self, mark_outside=False):
    """One child, one parent"""
    decorate = dr.decorators.reverse_pointers('annots', 'annots', 'child', 'parent', mutex=True, mark_outside=mark_outside)

    for a in self.doc.annots:
      self.assertFalse(hasattr(a, 'parent'))

    decorate(self.doc)

    EXPECTED = {
      0: self.doc.annots[1],
    }
    for i, a in enumerate(self.doc.annots):
      if i in EXPECTED:
        self.assertEqual(a.parent, EXPECTED[i])
      elif mark_outside:
        self.assertIsNone(a.parent)
      else:
        self.assertFalse(hasattr(a, 'parent'))

  def test_reverse_mutually_exclusive_pointer_with_o(self):
    self.test_reverse_mutually_exclusive_pointer(True)

  def test_reverse_mutually_exclusive_pointers(self, mark_outside=False):
    """Multiple children, one parent"""
    decorate = dr.decorators.reverse_pointers('annots', 'annots', 'children', 'parent', mutex=True, mark_outside=mark_outside)

    for a in self.doc.annots:
      self.assertFalse(hasattr(a, 'parent'))

    decorate(self.doc)

    EXPECTED = {
      0: self.doc.annots[2],
      1: self.doc.annots[2],
    }
    for i, a in enumerate(self.doc.annots):
      if i in EXPECTED:
        self.assertEqual(a.parent, EXPECTED[i])
      elif mark_outside:
        self.assertIsNone(a.parent)
      else:
        self.assertFalse(hasattr(a, 'parent'))

  def test_reverse_mutually_exclusive_pointers_with_o(self):
    self.test_reverse_mutually_exclusive_pointers(True)

  def test_reverse_overlapping_pointer(self, mark_outside=False):
    """One child, multiple parents"""
    self.doc.annots[0].child = self.doc.annots[0]
    decorate = dr.decorators.reverse_pointers('annots', 'annots', 'child', 'parents', mutex=False, mark_outside=mark_outside)

    for a in self.doc.annots:
      self.assertFalse(hasattr(a, 'parents'))

    decorate(self.doc)

    EXPECTED = {
      0: [self.doc.annots[0], self.doc.annots[1]],
    }
    for i, a in enumerate(self.doc.annots):
      if i in EXPECTED:
        self.assertEqual(set(a.parents), set(EXPECTED[i]))
      elif mark_outside:
        self.assertEqual(len(a.parents), 0)
      else:
        self.assertFalse(hasattr(a, 'parents'))

  def test_reverse_overlapping_pointer_with_o(self):
    self.test_reverse_overlapping_pointer(True)

  def test_reverse_overlapping_pointers(self, mark_outside=False):
    """Multiple children, multiple parents"""
    self.doc.annots[0].children = [self.doc.annots[0]]
    decorate = dr.decorators.reverse_pointers('annots', 'annots', 'children', 'parents', mutex=False, mark_outside=mark_outside)

    for a in self.doc.annots:
      self.assertFalse(hasattr(a, 'parents'))

    decorate(self.doc)

    EXPECTED = {
      0: [self.doc.annots[0], self.doc.annots[2]],
      1: [self.doc.annots[2]],
    }
    for i, a in enumerate(self.doc.annots):
      if i in EXPECTED:
        self.assertEqual(set(a.parents), set(EXPECTED[i]))
      elif mark_outside:
        self.assertEqual(len(a.parents), 0)
      else:
        self.assertFalse(hasattr(a, 'parents'))

  def test_reverse_overlapping_pointers_with_o(self):
    self.test_reverse_overlapping_pointers(True)


class PrevNextIndexTest(TestCase):
  def setUp(self):
    self.doc = Document()
    for val in '012':
      self.doc.annots.create(field=val)
    self.doc.slices.create(span=slice(0, 1), name='Unit slice')

  def test_prev_next(self):
    decorate = dr.decorators.add_prev_next('annots', 'prev', 'next')

    for a in self.doc.annots:
      self.assertFalse(hasattr(a, 'prev'))
      self.assertFalse(hasattr(a, 'next'))

    decorate(self.doc)

    annots = self.doc.annots
    self.assertEqual(annots[0].next, annots[1])
    self.assertEqual(annots[1].next, annots[2])
    self.assertIsNone(annots[2].next)

    self.assertIsNone(annots[0].prev)
    self.assertEqual(annots[1].prev, annots[0])
    self.assertEqual(annots[2].prev, annots[1])

    for a in self.doc.annots:
      self.assertFalse(hasattr(a, 'index'))

  def test_prev_next_single_item(self):
    decorate = dr.decorators.add_prev_next('slices', 'prev', 'next')

    for s in self.doc.slices:
      self.assertFalse(hasattr(s, 'prev'))
      self.assertFalse(hasattr(s, 'next'))

    decorate(self.doc)

    self.assertIsNone(self.doc.slices[0].prev)
    self.assertIsNone(self.doc.slices[0].next)

  def test_index(self):
    decorate = dr.decorators.add_prev_next('annots', index_attr='index')
    for a in self.doc.annots:
      self.assertFalse(hasattr(a, 'index'))

    decorate(self.doc)
    for i, a in enumerate(self.doc.annots):
      self.assertEqual(i, a.index)
