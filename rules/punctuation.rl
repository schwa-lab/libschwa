/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

	full_stop = ".";
	question_mark = "?";
	exclamation_mark = "!";

  # unicode U+2026 horizontal ellipsis
	unicode_2026 = 0xe2 0x80 0xa6 | "&#8230" | "&#x2026;" | "&hellip;";
  ellipsis = "..." | ". . ." | unicode_2026;

  hyphen = 0xe2 0x80 (0x90..0x91) | 0xe2 0x88 0x92;
  dash = "-"{1,3} | 0xe2 0x80 (0x92..0x95);

  symbols = punct | hyphen | dash;

  lines =	"-"{4,} | "="{3,} | "*"{2,} | "."{4,} | "- "{3,} "-";
}%%
