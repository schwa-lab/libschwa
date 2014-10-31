// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  # https://en.wikipedia.org/wiki/Quotation_mark_glyphs
  # https://en.wikipedia.org/wiki/International_variation_in_quotation_marks

  # Straight single quote characters are ambiguous.
  unicode_ff07 = 0xef 0xbc 0x87 ;  # U+ff07 fullwidth apostrophe (＇)
  single_quote = "'" | unicode_ff07 ;

  # Straight double quote characters are ambiguous.
  unicode_ff02 = 0xef 0xbc 0x82 ;  # U+ff02 fullwidth quotation mark (＂)
  double_quote = '"' | "''" | unicode_ff02 ;

  # Open single quotation marks.
  unicode_2018 = 0xe2 0x80 0x98 ;  # U+2018 left single quotation mark (‘)
  unicode_201a = 0xe2 0x80 0x9a ;  # U+201a single low-9 quotation mark (‚)
  unicode_201b = 0xe2 0x80 0x9b ;  # U+201b single high-reversed-9 quotation mark (‛)
  unicode_2039 = 0xe2 0x80 0xb9 ;  # U+2939 single left-pointing angle quotation mark (‹)
  unicode_275b = 0xe2 0x9d 0x9b ;  # U+275b heavy single turned comma quotation mark ornament (❛)
  unicode_300e = 0xe3 0x80 0x8e ;  # U+300e left white corner bracket (『)
  open_single_quote = "`" | unicode_2018 | unicode_201a | unicode_201b | unicode_2039 | unicode_275b | unicode_300e ;

  # Close single quotation marks.
  unicode_2019 = 0xe2 0x80 0x99 ;  # U+2019 right single quotation mark (’)
  unicode_203a = 0xe2 0x80 0xba ;  # U+293a single right-pointing angle quotation mark (›)
  unicode_275c = 0xe2 0x9d 0x9c ;  # U+275c heavy single comma quotation mark ornament (❜)
  unicode_300f = 0xe3 0x80 0x8f ;  # U+300f right white corner bracket (』)
  close_single_quote = unicode_2019 | unicode_203a | unicode_275c | unicode_300f ;

  # Open double quotation marks.
  unicode_00ab = 0xc2 0xab ;       # U+00ab left-pointing double angle quotation mark («)
  unicode_201c = 0xe2 0x80 0x9c ;  # U+201c left double quotation mark (“)
  unicode_201e = 0xe2 0x80 0x9e ;  # U+201e double low-9 quotation mark („)
  unicode_201f = 0xe2 0x80 0x9f ;  # U+201f double high-reversed-9 quotation mark (‟)
  unicode_275d = 0xe2 0x9d 0x9d ;  # U+275d heavy double turned comma quotation mark ornament (❝)
  unicode_300c = 0xe3 0x80 0x8c ;  # U+300c left corner bracket (「)
  open_double_quote = "``" | unicode_00ab | unicode_201c | unicode_201e | unicode_201f | unicode_275d | unicode_300c;

  # Close double quotation marks.
  unicode_00bb = 0xc2 0xbb ;       # U+00ab right-pointing double angle quotation mark (»)
  unicode_201d = 0xe2 0x80 0x9d ;  # U+201d right double quotation mark (”)
  unicode_275e = 0xe2 0x9d 0x9e ;  # U+275e heavy double comma quotation mark ornament (❞)
  unicode_300d = 0xe3 0x80 0x8d ;  # U+300d right corner bracket (」)
  close_double_quote = unicode_00bb | unicode_201d | unicode_275e | unicode_300d ;

  # Possessive 's.
  possessive =
      ( close_single_quote | single_quote ) 's' @s2 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'s"); }
    | ( close_single_quote | single_quote ) 'S' @s2 @{ _state.n2 = reinterpret_cast<const uint8_t *>(u8"'S"); }
    ;

}%%
