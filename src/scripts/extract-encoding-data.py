#!/usr/bin/env python3
import gzip
import hashlib
import os
import re
import sys
import urllib.request

MAX_CODE_POINT = 0x110000
SCRIPT_PATH = None
TMP_DIR = '/tmp'
UNICODE_CODE_PAGE_MAPPINGS = (
    ('KOI8_R', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MISC/KOI8-R.TXT'),
    ('KOI8_U', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MISC/KOI8-U.TXT'),
    ('LATIN1', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-1.TXT'),
    ('LATIN2', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-2.TXT'),
    ('LATIN3', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-3.TXT'),
    ('LATIN4', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-4.TXT'),
    ('LATIN5', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-5.TXT'),
    ('LATIN6', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-6.TXT'),
    ('LATIN7', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-7.TXT'),
    ('LATIN8', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-8.TXT'),
    ('LATIN9', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-9.TXT'),
    ('LATIN10', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-10.TXT'),
    ('LATIN11', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-11.TXT'),
    ('LATIN13', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-13.TXT'),
    ('LATIN14', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-14.TXT'),
    ('LATIN15', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-15.TXT'),
    ('LATIN16', 'http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-16.TXT'),
    ('WINDOWS_1250', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1250.TXT'),
    ('WINDOWS_1251', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1251.TXT'),
    ('WINDOWS_1252', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1252.TXT'),
    ('WINDOWS_1253', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1253.TXT'),
    ('WINDOWS_1254', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1254.TXT'),
    ('WINDOWS_1255', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1255.TXT'),
    ('WINDOWS_1256', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1256.TXT'),
    ('WINDOWS_1257', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1257.TXT'),
    ('WINDOWS_1258', 'http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1258.TXT'),
)
UNICODE_EASTASIAN_MAPPINGS = {
    'BIG5': 'http://www.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/OTHER/BIG5.TXT',
    'Shift_JIS': 'http://www.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/JIS/SHIFTJIS.TXT',
}

RE_REMOVE_COMMENTS = re.compile(r'#.*$')


def load_data(uri):
  print('[load_data] {0!r}'.format(uri))
  table = {}

  if uri.startswith('http'):
    md5 = hashlib.md5(uri.encode('utf-8')).hexdigest()
    path = os.path.join(TMP_DIR, md5)
    if os.path.exists(path):
      with open(path) as f:
        data = f.read()
    else:
      data = urllib.request.urlopen(uri).read().decode('utf-8')
      with open(path, 'w') as f:
        f.write(data)
  else:
    if uri.endswith('.gz'):
      with gzip.open(uri) as f:
        data = f.read().decode('utf-8')
    else:
      with open(uri) as f:
        data = f.read()

  for line in data.split('\n'):
    line = RE_REMOVE_COMMENTS.sub('', line.strip('\r')).strip()
    if not line:
      continue
    line = line.strip(',')

    if '\t' in line:
      columns = line.split('\t')
    else:
      columns = line.split('=>')
    if len(columns) == 1:
      columns.append('0x0')
    assert len(columns) == 2

    encoded, code_point = map(lambda n: int(n, 16), columns)
    assert encoded not in table
    assert code_point < MAX_CODE_POINT

    table[encoded] = code_point

  max_code_point = max(table.values())
  if max_code_point <= 0x007F:
    max_utf8_nbytes = 1
  elif max_code_point <= 0x07FF:
    max_utf8_nbytes = 2
  elif max_code_point <= 0xFFFF:
    max_utf8_nbytes = 3
  elif max_code_point <= 0x1FFFFF:
    max_utf8_nbytes = 4
  else:
    max_utf8_nbytes = -1

  if max_code_point < 2**8:
    max_nbytes = 8
  elif max_code_point < 2**16:
    max_nbytes = 16
  else:
    max_nbytes = 32

  return table, max_utf8_nbytes, max_nbytes


def process_unicode_mapping(file_h, prefix, uri):
  table, max_utf8_nbytes, max_nbytes = load_data(uri)

  min_encoded = min(table)
  max_encoded = max(table) + 1
  nencoded = max_encoded - min_encoded
  assert nencoded <= 2**8
  nencoded = 2**8

  if any(i not in table for i in range(nencoded)):
    max_utf8_nbytes = 4
    max_nbytes = 32

  print(file=file_h)
  print('// Data source: {0}.'.format(uri), file=file_h)
  print('static constexpr const uint8_t {0}_UTF8_NBYTES = 0x{1:02x};'.format(prefix, max_utf8_nbytes), file=file_h)
  print('static constexpr const uint8_t {0}_DELTA = 0x{1:02x};'.format(prefix, min_encoded), file=file_h)
  print('static constexpr const uint{0}_t {1}_TABLE[{2}] = {{'.format(max_nbytes, prefix, nencoded), file=file_h)
  for i in range(nencoded):
    if i % 16 == 0 and i != 0:
      print(file=file_h)
    if i % 16 == 0:
      print('  ', end='', file=file_h)
    else:
      print(' ', end='', file=file_h)
    print('0x{0:06x},'.format(table.get(min_encoded + i, MAX_CODE_POINT)), end='', file=file_h)
  print('\n};', file=file_h)


def process_gb2321(file_h, uri):
  table, max_utf8_nbytes, max_nbytes = load_data(uri)

  arrays = {}
  for encoded, code_point in sorted(table.items(), key=lambda t: (t[1], t[0])):
    e = chr(code_point).encode('GB2312')
    assert len(e) == 2
    assert int(e[0]) >= 128
    assert int(e[1]) >= 128
    n = ((int(e[0]) << 8) | int(e[1])) - 0x8080
    if n != encoded and code_point != 8741:
      encoded += 0x8080
      print('BAD', encoded, code_point, e, hex((encoded >> 8) & 0xff), hex(encoded & 0xff))
    a = int(e[0]) - 161
    if a not in arrays:
      arrays[a] = {}
    arrays[a][int(e[1]) - 161] = code_point

  assert len(arrays) <= 94
  for k, v in arrays.items():
    assert len(v) <= 94

  array_length = len(arrays)
  internal_array_length = max(map(len, arrays.values()))

  print(file=file_h)
  print('// Data source: {0}.'.format(uri), file=file_h)
  print('static constexpr const uint8_t GB2312_UTF8_NBYTES = 0x{0:02x};'.format(max_utf8_nbytes), file=file_h)
  print('static constexpr const uint8_t GB2312_DELTA = 0x{0:02x};'.format(161), file=file_h)
  print('static constexpr const uint{0}_t GB2312_TABLE[{1}][{2}] = {{'.format(max_nbytes, array_length, internal_array_length), file=file_h)
  for a in range(len(arrays)):
    array = arrays.get(a, {})
    print('  {{{0}}},'.format(', '.join('0x{0:06x}'.format(array.get(b, 0)) for b in range(internal_array_length))), file=file_h)
  print('};', file=file_h)


def process_big5(file_h, uri):
  table, max_utf8_nbytes, max_nbytes = load_data(uri)

  arrays = {}
  for encoded, code_point in sorted(table.items(), key=lambda t: (t[1], t[0])):
    # Remove '*** NO MAPPING ***' entries (\U+FFFD).
    if code_point == 0xfffd:
      continue

    e = chr(code_point).encode('big5')
    assert len(e) == 2
    a, b = map(int, e)
    assert 0xa1 <= a <= 0xf9
    assert 0x40 <= b <= 0x7e or 0xa1 <= b <= 0xfe
    n = (a << 8) | b
    assert n == encoded
    if a not in arrays:
      arrays[a] = {}
    arrays[a][b] = code_point

  outer_array_length = (0xf9 - 0xa1) + 1
  inner_array_length = 2**8 - 0x40

  print(file=file_h)
  print('// Data source: {0}.'.format(uri), file=file_h)
  print('static constexpr const uint8_t BIG5_UTF8_NBYTES = 0x{0:02x};'.format(max_utf8_nbytes), file=file_h)
  print('static constexpr const uint{0}_t BIG5_TABLE[{1}][{2}] = {{'.format(max_nbytes, outer_array_length, inner_array_length), file=file_h)
  for a in range(0xa1, 0xf9 + 1):
    array = arrays.get(a, {})
    items = [array.get(i, 0) for i in range(0x40, 2**8)]
    print('  {{{0}}},'.format(', '.join('0x{0:06x}'.format(item) for item in items)), file=file_h)
  print('};', file=file_h)


def process_shift_jis(file_h, uri):
  table, max_utf8_nbytes, max_nbytes = load_data(uri)

  EXCLUDED = {0x85, 0x86, 0x87, 0xeb, 0xec, 0xed, 0xee, 0xef}

  arrays = {}
  for encoded, code_point in sorted(table.items(), key=lambda t: (t[1], t[0])):
    e = chr(code_point).encode('Shift_JIS')
    assert 1 <= len(e) <= 2
    a = int(e[0])
    b = 0 if len(e) == 1 else int(e[1])
    if len(e) == 1:
      assert 0x00 <= a <= 0x80 or 0xa0 <= a <= 0xdf or 0xf0 <= a <= 0xff
      assert a not in EXCLUDED, hex(a)
      if 0xa1 <= a <= 0xdf:
        assert a == code_point - 0xfec0, (a, code_point - 0xfec0)
      elif a == 0x5c:
        # assert code_point == 0xa5, hex(code_point)
        pass
      elif a == 0x7e:
        assert code_point == 0x203e
      else:
        assert a == code_point
    else:
      assert 0x81 <= a <= 0x9f or 0xe0 <= a <= 0xef
      assert a not in EXCLUDED, hex(a)
      assert b >= 64
      n = (a << 8) | b
      assert n == encoded
      if a not in arrays:
        arrays[a] = {}
      arrays[a][b] = code_point

  for k, v in sorted(arrays.items()):
    assert min(v) >= 64

  outer_array_length = len(arrays)
  assert outer_array_length == 3*16 - 1 - len(EXCLUDED)
  inner_array_length = 2**8 - 64

  inner_array_min = min(map(min, arrays.values()))
  assert inner_array_min == 64

  first_index_deltas = []
  index_counter = 0
  for i in range(2**8):
    if i in EXCLUDED:
      delta = -2
    elif 0x81 <= i <= 0x9f or 0xe0 <= i <= 0xef:
      delta = index_counter
      index_counter += 1
    else:
      delta = -1
    first_index_deltas.append(delta)
  assert index_counter == outer_array_length

  print(file=file_h)
  print('// Data source: {0}.'.format(uri), file=file_h)
  print('static constexpr const uint8_t SHIFT_JIS_UTF8_NBYTES = 0x{0:02x};'.format(max_utf8_nbytes), file=file_h)
  print('static constexpr const int8_t SHIFT_JIS_INDICES[256] = {', file=file_h)
  for i in range(0, 2**8, 32):
    print('  {0},'.format(', '.join(map(lambda n: '{:2d}'.format(n), first_index_deltas[i:i+32]))), file=file_h)
  print('};', file=file_h)
  print('static constexpr const uint{0}_t SHIFT_JIS_TABLE[{1}][{2}] = {{'.format(max_nbytes, outer_array_length, inner_array_length), file=file_h)
  for a, array in sorted(arrays.items()):
    items = [array.get(i + 64, 0) for i in range(inner_array_length)]
    print('  {{{0}}},'.format(', '.join('0x{0:06x}'.format(item) for item in items)), file=file_h)
  print('};', file=file_h)


def main(file_h):
  print('// Generated by: {0}'.format(SCRIPT_PATH), file=file_h)
  print('#include <schwa/_base.h>', file=file_h)
  print(file=file_h)
  print('namespace schwa {', file=file_h)

  for prefix, uri in UNICODE_CODE_PAGE_MAPPINGS:
    process_unicode_mapping(file_h, prefix, uri)
  process_big5(file_h, UNICODE_EASTASIAN_MAPPINGS['BIG5'])
  process_gb2321(file_h, 'src/data/gb2312-80.txt.gz')
  process_shift_jis(file_h, UNICODE_EASTASIAN_MAPPINGS['Shift_JIS'])

  print(file=file_h)
  print('}  // namespace schwa', file=file_h)


if __name__ == '__main__':
  path = os.path.normpath(os.path.dirname(__file__))
  if path != 'src/scripts':
    print('Error: This script must be run from the top-level directory of libschwa.', file=sys.stderr)
    sys.exit(1)

  SCRIPT_PATH = os.path.join(path, os.path.basename(__file__))
  with open('src/lib/schwa/encoding_gen.h', 'w') as file_h:
    main(file_h)
