// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  letter =
      unicode_alpha
    | unicode_digit
    | ( unicode_upper '&' unicode_upper )
    | hyphen
    | ( unicode_alpha ( single_quote | close_single_quote ) unicode_alpha )
    | ( unicode_alpha '.' unicode_alpha )
    ;

  # default = (letter | (unicode - unicode_punct))+ - (any* '--' | '--' any*);
  default = ( letter | (unicode - unicode_punct) )+ ;

}%%
