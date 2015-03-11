#!/usr/bin/env python3
import sys

import msgpack


def main(in_file, out_file):
  seen_two = False
  words = {}  # { word: (class, prob) }
  for l, line in enumerate(in_file):
    line = line.strip().rsplit(' ', 2)
    if len(line) == 2 and not seen_two:
      seen_two = True
      continue
    if len(line) != 3:
      raise ValueError('Line #{} has {} columns instead of 3: {}'.format(l, len(line), line))

    word, cluster, prob = line
    cluster = int(cluster)
    prob = float(prob)

    assert word not in words
    words[word] = (cluster, prob)

  # Pack the map to the output stream.
  msgpack.pack(words, out_file)


if __name__ == '__main__':
  main(sys.stdin, sys.stdout.buffer)
