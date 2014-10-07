// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  include 'unicode_punctuation.rl';

  # Full stops.
  unicode_2024 = 0xe2 0x80 0xa4 ;  # U+2024 one dot leader (․)
  unicode_fe52 = 0xef 0xb9 0x92 ;  # U+fe52 small full stop (﹒)
  unicode_ff0e = 0xef 0xbc 0x8e ;  # U+ff0e fullwidth full stop (．)
  full_stop = '.' | unicode_2024 | unicode_fe52 | unicode_ff0e ;

  # Question marks.
  unicode_037e = 0xcd 0xbe ;       # U+037e greek question mark (;)
  unicode_055e = 0xd5 0x9e ;       # U+055e armenian question mark (՞)
  unicode_061f = 0xd8 0x9f ;       # U+061f arabic question mark (؟)
  unicode_1367 = 0xe1 0x8d 0xa7 ;  # U+1367 ethiopic question mark (፧)
  unicode_1945 = 0xe1 0xa5 0x85 ;  # U+1945 limbu question mark (᥅)
  unicode_2047 = 0xe2 0x81 0x87 ;  # U+2047 double question mark (⁇)
  unicode_2048 = 0xe2 0x81 0x88 ;  # U+2048 question exclamation mark (⁈)
  unicode_2049 = 0xe2 0x81 0x89 ;  # U+2049 interrobang (‽)
  unicode_2cfa = 0xe2 0xb3 0xba ;  # U+2cfa coptic old nubian direct question mark (⳺)
  unicode_2cfb = 0xe2 0xb3 0xbb ;  # U+2cfb coptic old nubian indirect question mark (⳻)
  unicode_2e2e = 0xe2 0xb8 0xae ;  # U+2e2e reversed question mark (⸮)
  unicode_a60f = 0xea 0x98 0x8f ;  # U+a60f vai question mark (꘏)
  unicode_a6f7 = 0xea 0x9b 0xb7 ;  # U+a6f7 bamum question mark (꛷)
  unicode_fe16 = 0xef 0xb8 0x96 ;  # U+fe16 presentation form for vertical question mark (︖)
  unicode_fe56 = 0xef 0xb9 0x96 ;  # U+fe56 small question mark (﹖)
  unicode_ff1f = 0xef 0xbc 0x9f ;  # U+ff1f fullwidth question mark (？)
  question_mark = '?' | '?!' | '!?' | unicode_037e | unicode_055e | unicode_061f | unicode_1367 | unicode_1945 | unicode_2047 | unicode_2048 | unicode_2049 | unicode_2cfa | unicode_2cfb | unicode_a60f | unicode_a6f7 | unicode_fe16 | unicode_fe56 | unicode_ff1f ;

  # Inverted question marks.
  unicode_00a1 = 0xc2 0xa1 ;  # U+00a1 inverted exclamation mark (¡)
  unicode_00bf = 0xc2 0xbf ;       # U+00bf inverted question mark (¿)
  unicode_2e18 = 0xe2 0xb8 0x98 ;  # U+2e18 inverted interrobang (⸘)
  inverted_question_mark = unicode_00bf | unicode_2e18 | unicode_00bf unicode_00a1 | unicode_00a1 unicode_00bf ;

  # Exclamation marks.
  unicode_055c = 0xd5 0x9c ;  # U+055c armenian exclamation mark (՜)
  unicode_07f9 = 0xdf 0xb9 ;  # U+07f9 nko exclamation mark (߹)
  unicode_1944 = 0xe1 0xa5 0x84 ;  # U+1944 limbu exclamation mark (᥄)
  unicode_203c = 0xe2 0x80 0xbc ;  # U+203c double exclamation mark (‼)
  unicode_fe15 = 0xef 0xb8 0x95 ;  # U+fe15 presentation form for vertical exclamation mark (︕)
  unicode_fe57 = 0xef 0xb9 0x97 ;  # U+fe57 small exclamation mark (﹗)
  unicode_ff01 = 0xef 0xbc 0x81 ;  # U+ff01 fullwidth exclamation mark (！)
  exclamation_mark = '!' | unicode_055c | unicode_07f9 | unicode_1944 | unicode_203c | unicode_fe15 | unicode_fe57 | unicode_ff01 ;

  # Inverted exclamation marks.
  inverted_exclamation_mark = unicode_00a1 ;

  # Ellipsis.
  unicode_0e2f = 0xe0 0xb8 0xaf ;  # U+0e2f thai character paiyannoi (ฯ)
  unicode_0eaf = 0xe0 0xba 0xaf ;  # U+0eaf lao ellipsis (ຯ)
  unicode_1801 = 0xe1 0xa0 0x81 ;  # U+1801 mongolian ellipsis (᠁)
  unicode_2026 = 0xe2 0x80 0xa6 ;  # U+2026 horizontal ellipsis (…)
  ellipsis = '...' | '. . .' | unicode_2026{1,} | unicode_0e2f | unicode_0eaf | unicode_1801 ;


  end_punct = ')' | ']' | '}' | ',' | ';' | ':';

  hyphen = 0xe2 0x80 (0x90..0x91) | 0xe2 0x88 0x92;

  dash = '-'{1,3} | 0xe2 0x80 (0x92..0x95) ;

  symbols = (punct - end_punct) | hyphen | dash;
  unicode_symbols = unicode_punct;

  lines = '-'{4,} | '='{3,} | '*'{2,} | '.'{4,} | '- '{3,} '-';

}%%
