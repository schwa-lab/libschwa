/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  sign = "+" | "-" | 0xc2 0xb1 | 0xe2 0x88 0x93;
  integer = sign? digit+ ("," digit+)*;
  float = integer "." digit+ | sign? "." digit+;
  fraction = sign? (digit+ "-")? digit+ "/" digit+;
  ordinal = "#" digit+;

  numbers = integer | float | ordinal | fraction;

}%%
