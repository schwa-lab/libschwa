#!/usr/bin/env python3
import sys

import msgpack


def main(in_file, out_file):
  gaz = {}  # { token0 : [ [ token ] ] }
  for line in in_file:
    line = line.strip()
    if not line or line[0] == '#':
      continue

    tokens = line.split()
    if tokens[0] not in gaz:
      gaz[tokens[0]] = []
    gaz[tokens[0]].append(tokens)

  # Order the matches lexicographically.
  for results in gaz.values():
    results.sort()

  # Pack the list of lists to the output stream.
  msgpack.pack(gaz, out_file)


if __name__ == '__main__':
  main(sys.stdin, sys.stdout.buffer)
