/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  sign = "+" | "-" | 0xc2 0xb1 | 0xe2 0x88 0x93;
  integer = sign? digit+ ("," digit+)*;
  float = integer "." digit+ | sign? "." digit+;
  fraction = sign? (digit+ "-")? digit+ "/" digit+;
  ordinal = "#" digit+;

  day = digit{1,2};
  month = digit{1,2};
  year = digit{2} digit{2}?;

  abbrev_year = "'" digit{2} | digit{2}? digit "0's";

  date0 = day "/" month "/" year | day "-" month "-" year | day "." month "." year;
  date = date0 | abbrev_year;

  time = digit{1,2} ":" digit{2} | digit{1,2} "." digit{2};

  numbers = integer | float | ordinal | fraction | date | time;

}%%
