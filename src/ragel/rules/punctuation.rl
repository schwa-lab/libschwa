/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  include "unicode_punctuation.rl";

  full_stop = ".";
  question_mark = "?";
  exclamation_mark = "!";

  end_punct = ")" | "]" | "}" | "," | ";" | ":";

  # unicode U+2026 horizontal ellipsis
  # windows 1250-1258 code pages 0x85 -> unicode U+2026
  unicode_2026 = 0xe2 0x80 0xa6 | "&#8230" | "&#x2026;" | "&hellip;" | 0x85;
  ellipsis = "..." | ". . ." | unicode_2026;

  hyphen = 0xe2 0x80 (0x90..0x91) | 0xe2 0x88 0x92;

  # Fairfax appears to use 0x13 as a hyphen (may be an db/encoding error)
  # in the online rendering of these are translated 0x80 0x93 (almost UTF w/o 0xe2)
  fairfax_dash = 0x13;

  dash = "-"{1,3} | 0xe2 0x80 (0x92..0x95) | fairfax_dash when FAIRFAX;

  symbols = (punct - end_punct) | hyphen | dash;
  unicode_symbols = unicode_punct;

  lines = "-"{4,} | "="{3,} | "*"{2,} | "."{4,} | "- "{3,} "-";
}%%
