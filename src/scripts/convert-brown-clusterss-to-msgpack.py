#!/usr/bin/env python3
import sys

import msgpack


def main(in_file, out_file):
  paths = {}
  for l, line in enumerate(in_file):
    line = line.strip().split('\t')
    if len(line) != 3:
      raise ValueError('Line #{} has {} columns instead of 3: {}'.format(l, len(line), line))

    path, word, freq = line
    if path not in paths:
      paths[path] = {}
    paths[path][word] = int(freq)

  # Pack the map to the output stream.
  msgpack.pack(paths, out_file)


if __name__ == '__main__':
  main(sys.stdin, sys.stdout.buffer)
