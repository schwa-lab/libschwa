// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  # Plus/minus signs.
  unicode_00b1 = 0xc2 0xb1 ;       # U+00b1 plus-minus sign (±)
  unicode_2213 = 0xe2 0x88 0x93 ;  # U+2213 minus-or-plus sign (∓)
  sign = '+' | '-' | unicode_00b1 | unicode_2213 ;

  integer = sign? unicode_digit+ ( ',' unicode_digit+ )* ;
  float = integer '.' unicode_digit+ | sign? '.' unicode_digit+ ;
  fraction = sign? ( unicode_digit+ ( '-' | unicode_space ) )? unicode_digit+ '/' unicode_digit+ ;
  ordinal = '#' unicode_digit+ ;

  numbers = integer | float | ordinal | fraction;

}%%
