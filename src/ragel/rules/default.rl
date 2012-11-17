/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  newline = "\r\n"|"\n";

  # unicode U+00A0: no-break space
  unicode_00a0 = 0xc2 0xa0 | "&nbsp;";
  other_ws = unicode_00a0;

  letter = alpha | digit | "&amp;" | (upper "&" upper) | "-" | (alpha (single_quote | close_single_quote) alpha) | (alpha "." alpha);

  default = (letter | (unicode - unicode_punct))+ - (any* "--" | "--" any*);

}%%
