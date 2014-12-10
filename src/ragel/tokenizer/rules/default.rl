// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  letter =
      unicode_letter
    | numbers
    | ( unicode_letter_uppercase '&' unicode_letter_uppercase )
    | ( unicode_letter ( single_quote | close_single_quote ) unicode_letter )
    | ( unicode_letter ('.' unicode_letter)+ '.'? )
    | ( (unicode_letter '.')+ hyphen )
    ;

  default = ( letter | hyphen letter | ( unicode -- symbols -- unicode_space -- unicode_line_space -- unicode_paragraph_space ) )+ ;
  default_title = ( unicode_letter_uppercase | numbers ) ( letter | hyphen letter | ( unicode -- symbols -- unicode_space -- unicode_line_space -- unicode_paragraph_space ) )+ ;

}%%
