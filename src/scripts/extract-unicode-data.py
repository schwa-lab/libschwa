#!/usr/bin/env python3
"""
The contents of UnicodeData.txt is described in http://www.unicode.org/reports/tr44/tr44-6.html#UnicodeData.txt
"""
import collections
import math
import os
import re
import sys
import urllib.request


BASE_URL = 'http://www.unicode.org/Public/6.3.0/ucd/'
TMP_DIR = '/tmp'

MAX_CODE_POINT = 0x110000

FLAG_IS_ALPHA          = 1 << 0
FLAG_IS_CASED          = 1 << 1
FLAG_IS_CASE_IGNORABLE = 1 << 2
FLAG_IS_DECIMAL        = 1 << 3
FLAG_IS_DIGIT          = 1 << 4
FLAG_IS_LOWER          = 1 << 5
FLAG_IS_NUMERIC        = 1 << 6
FLAG_IS_PRINTABLE      = 1 << 7
FLAG_IS_SPACE          = 1 << 8
FLAG_IS_TITLE          = 1 << 9
FLAG_IS_UPPER          = 1 << 10
FLAG_IS_XID_CONTINUE   = 1 << 11
FLAG_IS_XID_START      = 1 << 12
FLAG_SPECIAL_CASING    = 1 << 13

RE_REMOVE_COMMENTS = re.compile(r'#.*$')

NIL_CTYPE = (0, 0, 0, 0, 0, 0, 0)
NIL_DATA = (0, 0, 0, 0, 0)


class CodePoint(object):
  __slots__ = ('code_point', 'name', 'gc', 'ccc', 'bidi_class', 'decimal', 'digit', 'numeric', 'mirrored', 'upper', 'lower', 'title', 'east_asian_width', '_flags', '_has_special_casing', '_properties', '_sc_lower', '_sc_title', '_sc_upper')

  def __init__(self, code_point, name, gc, ccc, bidi_class, decimal, digit, numeric, mirrored, upper, lower, title):
    self.code_point = code_point
    self.name = name
    self.gc = gc
    self.ccc = ccc
    self.bidi_class = bidi_class
    self.decimal = decimal
    self.digit = digit
    self.numeric = numeric
    self.mirrored = mirrored
    self.upper = 0 if upper is None else upper - code_point
    self.lower = 0 if lower is None else lower - code_point
    self.title = 0 if title is None else title - code_point
    self.east_asian_width = 0  # The default value is 'N'.
    self._flags = 0
    self._has_special_casing = False
    self._properties = set()
    self._sc_lower = self._sc_title = self._sc_upper = None

  def add_property(self, prop):
    self._properties.add(prop)

  def get_ctype(self):
    decimal = 0 if self.decimal is None else self.decimal
    digit = 0 if self.digit is None else self.digit
    return (self.upper, self.lower, self.title, decimal, digit, self.gc, self._flags)

  def get_data(self):
    mirrored = 1 if self.mirrored else 0
    sc = 0
    if self._has_special_casing:
      sc = (sc << 2) | self._sc_upper[1]
      sc = (sc << 2) | self._sc_lower[1]
      sc = (sc << 2) | self._sc_title[1]
      sc = (sc << 8) | self._sc_upper[0]
      sc = (sc << 8) | self._sc_lower[0]
      sc = (sc << 8) | self._sc_title[0]
    return (mirrored, self.bidi_class, self.ccc, self.east_asian_width, sc)

  def set_special_casing(self, attr, values, special_casings):
    if values in special_casings:
      index = special_casings[values]
    else:
      index = len(special_casings)
      special_casings[values] = index
    assert 0 <= index < (1 << 8)
    length = len(values)
    assert 0 < length < (1 << 8)
    setattr(self, attr, (index, length))

  def set_special_casings(self, lowers, titles, uppers, special_casings):
    self._has_special_casing = True
    self.set_special_casing('_sc_lower', lowers, special_casings)
    self.set_special_casing('_sc_title', titles, special_casings)
    self.set_special_casing('_sc_upper', uppers, special_casings)

  def update_flags(self, inv_categories):
    self._flags = 0
    if inv_categories[self.gc] in ('Lm', 'Lt', 'Lu', 'Ll', 'Lo'):
      self._flags |= FLAG_IS_ALPHA
    if 'Cased' in self._properties:
      self._flags |= FLAG_IS_CASED
    if 'Case_Ignorable' in self._properties:
      self._flags |= FLAG_IS_CASE_IGNORABLE
    if self.decimal is not None:
      self._flags |= FLAG_IS_DECIMAL
    if self.digit is not None:
      self._flags |= FLAG_IS_DIGIT
    if 'Lowercase' in self._properties:
      self._flags |= FLAG_IS_LOWER
    if self.numeric is not None:
      self._flags |= FLAG_IS_NUMERIC
    if self.code_point == ord(' ') or inv_categories[self.gc][0] not in ('C', 'Z'):
      self._flags |= FLAG_IS_PRINTABLE
    if inv_categories[self.gc] == 'Zs' or self.bidi_class in ('WS', 'B', 'S'):
      self._flags |= FLAG_IS_SPACE
    if inv_categories[self.gc] == 'Lt':
      self._flags |= FLAG_IS_TITLE
    if 'Uppercase' in self._properties:
      self._flags |= FLAG_IS_UPPER
    if 'XID_Continue' in self._properties:
      self._flags |= FLAG_IS_XID_CONTINUE
    if 'XID_Start' in self._properties:
      self._flags |= FLAG_IS_XID_START
    if self._has_special_casing:
      self._flags |= FLAG_SPECIAL_CASING


def maybe_int(value, base=10):
  return None if value is None else int(value, base)


def split_bins_get_size(data):
  # Return smallest possible integer size for the given array.
  max_data = max(data)
  if max_data < (1 << 8):
    return 1
  elif max_data < (1 << 16):
    return 2
  elif max_data < (1 << 32):
    return 4
  else:
    return 8


def split_bins(t, *, debug=False):
  """t, debug=False -> (t1, t2, shift).  Split a table to save space.

  t is a sequence of ints.  This function can be useful to save space if
  many of the ints are the same.  t1 and t2 are lists of ints, and shift
  is an int, chosen to minimize the combined size of t1 and t2 (in C
  code), and where for each i in range(len(t)),
      t[i] == t2[(t1[i >> shift] << shift) + (i & mask)]
  where mask is a bitmask isolating the last "shift" bits.

  This function is borrowed from the Python source code. Tools/unicode/makeunicodedata.py.
  """
  if debug:
    def dump(t1, t2, shift, nbytes):
      print('{}+{} bins at shift {}; {} bytes'.format(len(t1), len(t2), shift, nbytes))
      print('Size of original table:', len(t) * split_bins_get_size(t), 'bytes')

  n = len(t) - 1  # Last valid index.
  max_shift = 0  # The most we can shift n and still have something left.
  if n > 0:
    while n >> 1:
      n >>= 1
      max_shift += 1
  del n

  nbytes = sys.maxsize  # Smallest total size so far.
  t = tuple(t)  # So slices can be dict keys.
  for shift in range(max_shift + 1):
    t1 = []
    t2 = []
    size = 2 ** shift
    seq_cache = {}
    for i in range(0, len(t), size):
      seq = t[i:i + size]
      index = seq_cache.get(seq)
      if index is None:
        index = len(t2)
        seq_cache[seq] = index
        t2.extend(seq)
      t1.append(index >> shift)

    # Determine memory size.
    b = (len(t1) * split_bins_get_size(t1)) + (len(t2) * split_bins_get_size(t2))
    if debug:
      dump(t1, t2, shift, b)
    if b < nbytes:
      best = t1, t2, shift
      nbytes = b

  t1, t2, shift = best
  assert 0 <= min(t1) < (1 << 8)
  assert 0 <= max(t1) < (1 << 8)
  assert 0 <= min(t2) < (1 << 8)
  assert 0 <= max(t2) < (1 << 8)

  # Exhaustively verify that the decomposition is correct.
  mask = ~((~0) << shift)  # i.e., low-bit mask of shift bits.
  for i in range(len(t)):
    assert t[i] == t2[(t1[i >> shift] << shift) + (i & mask)]

  return best


def unicode_path(filename):
  path = os.path.join(TMP_DIR, filename)
  if not os.path.exists(path):
    data = urllib.request.urlopen(BASE_URL + filename).read()
    with open(path, 'wb') as f:
      f.write(data)
  return path


def load_unicode_data(code_points, categories, bidi_classes):
  start_code_point = end_code_point = None
  with open(unicode_path('UnicodeData.txt'), encoding='utf-8') as f:
    for line in f:
      parts = tuple(x or None for x in line.strip().split(';'))
      assert len(parts) == 15
      code_point, name, gc, ccc, bidi_class, decomp, decimal, digit, numeric, mirrored, _, _, upper, lower, title = parts

      code_point = int(code_point, 16)
      ccc = int(ccc)
      decimal = maybe_int(decimal)
      digit = maybe_int(digit)
      if numeric is not None:
        numeric = numeric.split('/')
        assert 1 <= len(numeric) <= 2
        if len(numeric) == 1:
          numeric = (int(numeric[0]), 1)
        else:
          numeric = (int(numeric[0]), int(numeric[1]))
      upper = maybe_int(upper, 16)
      lower = maybe_int(lower, 16)
      title = maybe_int(title, 16)
      mirrored = mirrored == 'Y'
      assert 0 <= code_point < MAX_CODE_POINT
      assert decimal is None or 0 <= decimal <= 9
      assert digit is None or 0 <= digit <= 9

      if name.endswith(', First>'):
        start_code_point = code_point
        continue
      elif name.endswith(', Last>'):
        end_code_point = code_point
      else:
        start_code_point = end_code_point = code_point

      if gc not in categories:
        categories[gc] = len(categories)
      gc = categories[gc]
      if bidi_class not in bidi_classes:
        bidi_classes[bidi_class] = len(bidi_classes)
      bidi_class = bidi_classes[bidi_class]

      for cp in range(start_code_point, end_code_point + 1):
        code_points[cp] = CodePoint(cp, name, gc, ccc, bidi_class, decimal, digit, numeric, mirrored, upper, lower, title)


def load_derived_core_properties(code_points):
  start_code_point = end_code_point = None
  with open(unicode_path('DerivedCoreProperties.txt'), encoding='utf-8') as f:
    for line in f:
      line = RE_REMOVE_COMMENTS.sub('', line).strip()
      if not line:
        continue
      parts = tuple(x.strip() for x in line.split(';'))
      assert len(parts) == 2
      cp_range, prop = parts

      if '..' in cp_range:
        start_code_point, end_code_point = (int(x, 16) for x in cp_range.split('..'))
      else:
        start_code_point = end_code_point = int(cp_range, 16)
      for cp in range(start_code_point, end_code_point + 1):
        if cp not in code_points and prop == 'Default_Ignorable_Code_Point':
          continue
        code_points[cp].add_property(prop)


def load_east_asian_widths(code_points, widths):
  with open(unicode_path('EastAsianWidth.txt'), encoding='utf-8') as f:
    for line in f:
      line = RE_REMOVE_COMMENTS.sub('', line).strip()
      if not line:
        continue
      parts = tuple(x.strip() for x in line.split(';'))
      assert len(parts) == 2
      cp_range, width = parts
      if '..' in cp_range:
        start_code_point, end_code_point = (int(x, 16) for x in cp_range.split('..'))
      else:
        start_code_point = end_code_point = int(cp_range, 16)
      if width not in widths:
        widths[width] = len(widths)
      width = widths[width]
      for cp in range(start_code_point, end_code_point + 1):
        if cp in code_points:
          code_points[cp].east_asian_width = width


def load_special_casing(code_points, special_casings):
  with open(unicode_path('SpecialCasing.txt'), encoding='utf-8') as f:
    for line in f:
      line = RE_REMOVE_COMMENTS.sub('', line).strip()
      if not line:
        continue
      parts = tuple(x.strip() for x in line.split(';'))
      assert len(parts) in (5, 6)
      if len(parts) == 6:
        continue  # Throw away conditional special casing for the moment as it's too hard.

      code_point, lowers, titles, uppers, conditions = parts
      assert not conditions
      code_point = int(code_point, 16)
      lowers = tuple(int(x, 16) for x in lowers.split())
      titles = tuple(int(x, 16) for x in titles.split())
      uppers = tuple(int(x, 16) for x in uppers.split())
      code_points[code_point].set_special_casings(lowers, titles, uppers, special_casings)


def output_names_and_enum(name, inv_mapping, comment):
  assert 0 < len(inv_mapping) < (1 << 8)
  print(r'''
// {0}
static constexpr const char *{1}_NAMES[{2}] = {{'''.format(comment, name.upper().replace(' ', '_'), len(inv_mapping)))
  for i, c in sorted(inv_mapping.items()):
    print('    "{0}",'.format(c))
  print(r'''}};

enum class Unicode{0}Names : uint8_t {{'''.format(name.title().replace(' ', '')))
  for c, i in sorted((v, k) for k, v in inv_mapping.items()):
    print('    {0} = {1},'.format(c, i))
  print(r'''};
''')


def output_table(code_points, cc_type, cc_prefix, nil_obj, obj_gettr):
  objs = {}  # { obj : index }
  objs[nil_obj] = 0
  indices = []  # [ index ]
  for cp in range(MAX_CODE_POINT):
    obj = nil_obj
    if cp in code_points:
      obj = obj_gettr(code_points[cp])
    if obj not in objs:
      objs[obj] = len(objs)
    indices.append(objs[obj])

  print(r'static constexpr const {0} {1}_DATA[{2}] = {{'.format(cc_type, cc_prefix, len(objs)))
  for i, obj in sorted((v, k) for k, v in objs.items()):
    print('    {{{0}}},'.format(', '.join(map(str, obj))))
  print(r'''}};
static_assert(sizeof({0}_DATA) <= 4096, "{1}_DATA should fit inside a page.");
'''.format(cc_prefix, cc_prefix))

  t1, t2, shift = split_bins(indices)
  print(r'static constexpr const size_t {0}_INDEX_SHIFT = {1};'.format(cc_prefix, shift))
  print(r'static constexpr const uint8_t {0}_INDEX1[{1}] = {{'.format(cc_prefix, len(t1)))
  for i in range(0, len(t1), 32):
    print('    {0},'.format(', '.join(map(str, t1[i:i + 32]))))
  print(r'''}};

static constexpr const uint8_t {0}_INDEX2[{1}] = {{'''.format(cc_prefix, len(t2)))
  for i in range(0, len(t2), 32):
    print('    {0},'.format(', '.join(map(str, t2[i:i + 32]))))
  print(r'''}};

static const {cc_type} &
get_{cc_type}(const unicode_t code_point) {{
  if (code_point >= MAX_CODE_POINT)
    UnicodeException::throw_invalid_code_point(code_point);
  size_t index = {cc_prefix}_INDEX1[(code_point >> {cc_prefix}_INDEX_SHIFT)];
  index = {cc_prefix}_INDEX2[(index << {cc_prefix}_INDEX_SHIFT) + (code_point & ((1 << {cc_prefix}_INDEX_SHIFT) - 1))];
  return {cc_prefix}_DATA[index];
}}
'''.format(cc_prefix=cc_prefix, cc_type=cc_type))


def main():
  code_points = {}  # { code_point : CodePoint }
  bidi_classes = {}  # { str : index }
  categories = {'Cn': 0}  # { str : index }
  east_asian_widths = {'N': 0}  # { str : index }
  special_casings = {}  # { (code_point) : index }

  load_unicode_data(code_points, categories, bidi_classes)
  load_derived_core_properties(code_points)
  load_east_asian_widths(code_points, east_asian_widths)
  load_special_casing(code_points, special_casings)

  assert len(bidi_classes) == 23, len(bidi_classes)
  assert len(categories) == 30, len(categories)
  assert len(east_asian_widths) == 6, len(east_asian_widths)
  assert 0 < len(special_casings) < (1 << 8)
  assert 0 < max(map(len, special_casings)) < 4
  inv_categories = {v: k for k, v in categories.items()}
  inv_bidi_classes = {v: k for k, v in bidi_classes.items()}
  inv_east_asian_widths = {v: k for k, v in east_asian_widths.items()}

  for cp in code_points.values():
    cp.update_flags(inv_categories)

  print(r'''
//#include <schwa/_base.h>  // FIXME swap these back
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <sstream>
#include <string>


namespace schwa {{

// FIXME remove these once the header is included.
using unicode_t = uint32_t;
class UnicodeException : std::exception {{
private:
  std::string _msg;
  unicode_t _code_point;

public:
  UnicodeException(const std::string &msg, const unicode_t code_point) noexcept : _code_point(code_point) {{
    std::ostringstream ss;
    ss << msg << "(0x" << std::hex << code_point << ")";
    _msg = ss.str();
  }}
  virtual ~UnicodeException(void) noexcept {{ }}

  inline unicode_t code_point(void) const {{ return _code_point; }}
  virtual const char *what(void) const noexcept {{ return _msg.c_str(); }}

  static void
  throw_invalid_code_point(const unicode_t code_point) {{
    throw UnicodeException("Invalid code point", code_point);
  }}
}};


static constexpr const unicode_t MAX_CODE_POINT = 0x{0:X};

struct unicode_ctype {{
  int32_t upper;
  int32_t lower;
  int32_t title;
  uint8_t decimal : 4;
  uint8_t digit : 4;
  uint8_t category;
  uint16_t flags;
}};
static_assert(sizeof(unicode_ctype) == 16, "Unexpected struct size.");


struct unicode_data {{
  uint8_t mirrored;
  uint8_t bidi_class;
  uint8_t combining;  // http://www.unicode.org/reports/tr44/tr44-6.html#Canonical_Combining_Class_Values
  uint8_t east_asian_width;  // http://www.unicode.org/reports/tr11/
  uint32_t special_casing;
}};
static_assert(sizeof(unicode_data) == 8, "Unexpected struct size.");
'''.format(MAX_CODE_POINT))

  output_names_and_enum('Category', inv_categories, 'Category names: http://www.unicode.org/reports/tr44/tr44-6.html#General_Category_Values')
  output_names_and_enum('Bidi Class', inv_bidi_classes, 'Bidi class names: http://www.unicode.org/reports/tr44/tr44-6.html#Bidi_Class_Values')
  output_names_and_enum('East Asian Width', inv_east_asian_widths, 'East asian widths: http://www.unicode.org/reports/tr11/')

  for flag_name, flag in sorted(globals().items()):
    if flag_name.startswith('FLAG_'):
      shift = int(math.log(flag, 2))
      print('static constexpr const uint16_t UNICODE_CTYPE_{0} = 1u << {1};'.format(flag_name, shift))

  output_table(code_points, 'unicode_ctype', 'UNICODE_CTYPE', NIL_CTYPE, CodePoint.get_ctype)
  output_table(code_points, 'unicode_data', 'UNICODE_DATA', NIL_DATA, CodePoint.get_data)

  numerics = collections.defaultdict(list)  # { (int, int) : [CodePoint] }
  for cp in code_points.values():
    if cp.numeric is not None:
      numerics[cp.numeric].append(cp)
  assert len(numerics) != 0
  print(r'''
double
code_point_to_numeric(const unicode_t code_point) {
  switch (code_point) {''')
  for key, cps in sorted(numerics.items()):
    for cp in cps:
      print('  case 0x{0:X}:'.format(cp.code_point))
    numerator, denominator = key
    numerator = float(numerator)
    if denominator == 1:
      print('    return {0};'.format(numerator))
    else:
      print('    return {0}/{1};'.format(numerator, denominator))
  print(r'''  default:
    throw UnicodeException("Code point is not numeric", code_point);
  }
}
''')

  print('}  // namespace schwa')


if __name__ == '__main__':
  main()
