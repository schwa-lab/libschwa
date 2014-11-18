// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  letter =
      unicode_letter
    | unicode_digit
    | ( unicode_letter_uppercase '&' unicode_letter_uppercase )
    | ( unicode_letter ( single_quote | close_single_quote ) unicode_letter )
    | ( unicode_letter '.' unicode_letter )
    ;

  default = ( letter | hyphen letter | ( unicode -- symbols -- unicode_space -- unicode_line_space -- unicode_paragraph_space ) )+ ;

}%%
