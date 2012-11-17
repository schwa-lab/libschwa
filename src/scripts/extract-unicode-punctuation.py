#!/usr/bin/env python2
import gzip
import os
import sys

DATA_FILE = 'src/data/UnicodeData.txt.gz'
PREFIX = '  unicode_punct '

if not os.path.exists(DATA_FILE):
  print >> sys.stderr, 'Could not find file {0!r}'.format(DATA_FILE)
  sys.exit(1)

punct = []
with gzip.open(DATA_FILE) as f:
  for line in f:
    line = line.split(';')
    assert len(line) == 15
    codepoint, name, category = line[:3]
    if category == 'Po':
      char = unichr(int(codepoint, 16))
      punct.append((codepoint, name, char))

print '''
%%{
  machine tokenizer;
'''
for i, (codepoint, name, char) in enumerate(punct):
  if i == 0:
    prefix = PREFIX + '='
  else:
    prefix = ' ' * len(PREFIX) + '|'
  utf8_char = char.encode('utf-8')
  utf8_bytes = ' '.join(str(hex(ord(c))) for c in utf8_char)
  print '{} {} # U{}, {} ({})'.format(prefix, utf8_bytes, codepoint, name, utf8_char)
print ' ' * len(PREFIX) + ';'
print '''
}%%
'''
