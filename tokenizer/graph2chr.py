#!/usr/bin/env python

import sys
import re
import string

LABEL = re.compile('".*?["/]')
NUMBER = re.compile("[0-9]+")

ESCAPE = {'"': '\\"', '\n': '\\n', '\r': '\\r', '\t':'\\t', ' ': 'space', '\0': '\\0', '\v': '\\v'}

def convert_char(match):
    try:
        char = chr(int(match.group(0)))
        if char in ESCAPE:
            return ESCAPE[char]
        if char in string.printable:
            return char
    except ValueError:
        pass
    return match.group(0)

def convert_label(match):
    return NUMBER.sub(convert_char, match.group(0))

for line in sys.stdin:
    print LABEL.sub(convert_label, line),
