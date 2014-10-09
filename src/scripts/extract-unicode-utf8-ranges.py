#!/usr/bin/env python3
CATEGORIES = {'Ll'}


def output_leaves(node, keys, path):
  ranges = []
  start = prev = keys[0]
  for i in range(1, len(keys)):
    val = keys[i]
    if val == prev + 1:
      prev = val
    else:
      ranges.append((start, prev))
      start = prev = val
  ranges.append((start, prev))

  out = ['|']
  out.extend(['0x{:02x}'.format(b) for b in path])
  if len(ranges) != 1:
    out.append('(')

  wrote = 0
  for i, (first, last) in enumerate(ranges):
    if i != 0:
      out.append('|')
    if first == last:
      out.append('0x{:02x}'.format(first))
      wrote += 1
    else:
      out.append('0x{:02x}..0x{:02x}'.format(first, last))
      wrote += last - first + 1

  if len(ranges) != 1:
    out.append(')')
  print(' '.join(out))

  return wrote


def output(node, path):
  subtree_keys, leaf_keys = [], []
  for k, v in sorted(node.items()):
    if isinstance(v, bool):
      leaf_keys.append(k)
    else:
      subtree_keys.append(k)

  wrote = 0
  if leaf_keys:
    wrote += output_leaves(node, leaf_keys, path)
  if subtree_keys:
    for k in subtree_keys:
      path.append(k)
      wrote += output(node[k], path)
      path.pop()
  return wrote


if __name__ == '__main__':
  code_points = []
  with open('/tmp/UnicodeData.txt') as f:
    for line in f:
      parts = line.strip().split(';')
      if parts[2] in CATEGORIES:
        code_points.append(int(parts[0], 16))

  tree = {}
  for cp in code_points:
    utf8 = chr(cp).encode('utf-8')
    parent = tree
    for i, b in enumerate(utf8):
      if i == len(utf8) - 1:
        parent[b] = True
      else:
        if b not in parent:
          parent[b] = {}
        else:
          assert isinstance(parent[b], dict)
        parent = parent[b]

  print('# Unicode categories: {} ({} code points)'.format(', '.join(sorted(CATEGORIES)), len(code_points)))
  wrote = output(tree, [])
  assert len(code_points) == wrote
