#!/usr/bin/env python3
from __future__ import print_function
import sys

import msgpack


def main(in_file, out_file):
  embeddings = {}  # { token : [ float ] }
  dimensions = None
  for l, line in enumerate(in_file):
    line = line.strip().split()

    # Sanity check the dimensions.
    if dimensions is None:
      dimensions = len(line) - 1
    elif len(line) - 1 != dimensions:
      raise ValueError('Line #{} has {} dimensions when it should have {}'.format(l, len(line) - 1, dimensions))

    # Convert the values in each dimension to floats.
    it = iter(line)
    word = next(it)
    embeddings[word] = tuple(float(v) for v in it)

  # Pack the map to the output stream, using only floating point precision.
  msgpack.pack(embeddings, out_file)

  print('# dimensions:', dimensions, file=sys.stderr)
  print('# words     :', len(embeddings), file=sys.stderr)


if __name__ == '__main__':
  main(sys.stdin, sys.stdout.buffer)
