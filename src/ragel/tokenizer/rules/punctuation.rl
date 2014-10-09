// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

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
  unicode_203d = 0xe2 0x80 0xbd ;  # U+203d interrobang (‽)
  unicode_2047 = 0xe2 0x81 0x87 ;  # U+2047 double question mark (⁇)
  unicode_2048 = 0xe2 0x81 0x88 ;  # U+2048 question exclamation mark (⁈)
  unicode_2049 = 0xe2 0x81 0x89 ;  # U+2049 exclamation question mark (⁉)
  unicode_2cfa = 0xe2 0xb3 0xba ;  # U+2cfa coptic old nubian direct question mark (⳺)
  unicode_2cfb = 0xe2 0xb3 0xbb ;  # U+2cfb coptic old nubian indirect question mark (⳻)
  unicode_2e2e = 0xe2 0xb8 0xae ;  # U+2e2e reversed question mark (⸮)
  unicode_a60f = 0xea 0x98 0x8f ;  # U+a60f vai question mark (꘏)
  unicode_a6f7 = 0xea 0x9b 0xb7 ;  # U+a6f7 bamum question mark (꛷)
  unicode_fe16 = 0xef 0xb8 0x96 ;  # U+fe16 presentation form for vertical question mark (︖)
  unicode_fe56 = 0xef 0xb9 0x96 ;  # U+fe56 small question mark (﹖)
  unicode_ff1f = 0xef 0xbc 0x9f ;  # U+ff1f fullwidth question mark (？)
  question_mark = '?' | '?!' | '!?' | unicode_037e | unicode_055e | unicode_061f | unicode_1367 | unicode_1945 | unicode_203d | unicode_2047 | unicode_2048 | unicode_2049 | unicode_2cfa | unicode_2cfb | unicode_a60f | unicode_a6f7 | unicode_fe16 | unicode_fe56 | unicode_ff1f ;

  # Inverted question marks.
  unicode_00a1 = 0xc2 0xa1 ;       # U+00a1 inverted exclamation mark (¡)
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

  # Hyphens.
  unicode_002d = 0x2d ;            # U+002d hyphen-minus (-)
  unicode_00ad = 0xc2 0xad ;       # U+00ad soft hyphen (­)
  unicode_00b7 = 0xc2 0xb7 ;       # U+00b7 middle dot (·)
  unicode_058a = 0xd6 0x8a ;       # U+058a armenian hyphen (֊)
  unicode_1400 = 0xe1 0x90 0x80 ;  # U+1400 canadian syllabics hyphen (᐀)
  unicode_1806 = 0xe1 0xa0 0x86 ;  # U+1806 mongolian todo soft hyphen (᠆)
  unicode_2010 = 0xe2 0x80 0x90 ;  # U+2010 hyphen (‐)
  unicode_2011 = 0xe2 0x80 0x91 ;  # U+2011 non-breaking hyphen (‑)
  unicode_2027 = 0xe2 0x80 0xa7 ;  # U+2027 hyphenation point (‧)
  unicode_2043 = 0xe2 0x81 0x83 ;  # U+2043 hyphen bullet (⁃)
  unicode_2212 = 0xe2 0x88 0x92 ;  # U+2212 minus sign (−)
  hyphen = unicode_002d | unicode_00ad | unicode_00b7 | unicode_058a | unicode_1400 | unicode_1806 | unicode_2010 | unicode_2011 | unicode_2027 | unicode_2043 | unicode_2212 ;

  # Dashes.
  unicode_2012 = 0xe2 0x80 0x92 ;  # U+2012 figure dash (‒)
  unicode_2013 = 0xe2 0x80 0x93 ;  # U+2013 en dash (–)
  unicode_2014 = 0xe2 0x80 0x94 ;  # U+2014 em dash (—)
  unicode_2015 = 0xe2 0x80 0x95 ;  # U+2015 horizontal bar (―)
  unicode_2053 = 0xe2 0x81 0x93 ;  # U+2053 swung dash (⁓)
  unicode_2e3a = 0xe2 0xb8 0xba ;  # U+2e3a two-em dash (⸺)
  unicode_2e3b = 0xe2 0xb8 0xbb ;  # U+2e3b three-em dash (⸻)
  dash = '-'{2,3} | unicode_2012 | unicode_2013 | unicode_2014 | unicode_2015 | unicode_2053 | unicode_2e3a | unicode_2e3b ;

  # Commas.
  unicode_002c = 0x2c ;  # U+002c comma (,)
  unicode_02bb = 0xca 0xbb ;  # U+02bb modifier letter turned comma (ʻ)
  unicode_02bd = 0xca 0xbd ;  # U+02bd modifier letter reversed comma (ʽ)
  unicode_0312 = 0xcc 0x92 ;  # U+0312 combining turned comma above (̒)
  unicode_0313 = 0xcc 0x93 ;  # U+0313 combining comma above (̓)
  unicode_0314 = 0xcc 0x94 ;  # U+0314 combining reversed comma above (̔)
  unicode_0315 = 0xcc 0x95 ;  # U+0315 combining comma above right (̕)
  unicode_0326 = 0xcc 0xa6 ;  # U+0326 combining comma below (̦)
  unicode_055d = 0xd5 0x9d ;  # U+055d armenian comma (՝)
  unicode_060c = 0xd8 0x8c ;  # U+060c arabic comma (،)
  unicode_07f8 = 0xdf 0xb8 ;  # U+07f8 nko comma (߸)
  unicode_1363 = 0xe1 0x8d 0xa3 ;  # U+1363 ethiopic comma (፣)
  unicode_1802 = 0xe1 0xa0 0x82 ;  # U+1802 mongolian comma (᠂)
  unicode_1808 = 0xe1 0xa0 0x88 ;  # U+1808 mongolian manchu comma (᠈)
  unicode_3001 = 0xe3 0x80 0x81 ;  # U+3001 ideographic comma (、)
  unicode_a4fe = 0xea 0x93 0xbe ;  # U+a4fe lisu punctuation comma (꓾)
  unicode_a60d = 0xea 0x98 0x8d ;  # U+a60d vai comma (꘍)
  unicode_a6f5 = 0xea 0x9b 0xb5 ;  # U+a6f5 bamum comma (꛵)
  unicode_fe10 = 0xef 0xb8 0x90 ;  # U+fe10 presentation form for vertical comma (︐)
  unicode_fe11 = 0xef 0xb8 0x91 ;  # U+fe11 presentation form for vertical ideographic comma (︑)
  unicode_fe50 = 0xef 0xb9 0x90 ;  # U+fe50 small comma (﹐)
  unicode_fe51 = 0xef 0xb9 0x91 ;  # U+fe51 small ideographic comma (﹑)
  unicode_ff0c = 0xef 0xbc 0x8c ;  # U+ff0c fullwidth comma (，)
  unicode_ff64 = 0xef 0xbd 0xa4 ;  # U+ff64 halfwidth ideographic comma (､)
  comma = unicode_002c | unicode_02bb | unicode_02bd | unicode_0312 | unicode_0313 | unicode_0314 | unicode_0315 | unicode_0326 | unicode_055d | unicode_060c | unicode_07f8 | unicode_1363 | unicode_1802 | unicode_1808 | unicode_3001 | unicode_a4fe | unicode_a60d | unicode_a6f5 | unicode_fe10 | unicode_fe11 | unicode_fe50 | unicode_fe51 | unicode_ff0c | unicode_ff64 ;

  # Colons.
  unicode_003a = 0x3a ;  # U+003a colon (:)
  unicode_02d0 = 0xcb 0x90 ;  # U+02d0 modifier letter triangular colon (ː)
  unicode_ff1a = 0xef 0xbc 0x9a ;  # U+ff1a fullwidth colon (：)
  colon = unicode_003a | unicode_02d0 | unicode_ff1a ;

  # Semicolons.
  unicode_003b = 0x3b ;            # U+003b semicolon (;)
  unicode_061b = 0xd8 0x9b ;       # U+061b arabic semicolon (؛)
  unicode_1364 = 0xe1 0x8d 0xa4 ;  # U+1364 ethiopic semicolon (፤)
  unicode_204f = 0xe2 0x81 0x8f ;  # U+204f reversed semicolon (⁏)
  unicode_236e = 0xe2 0x8d 0xae ;  # U+236e apl functional symbol semicolon underbar (⍮)
  unicode_2e35 = 0xe2 0xb8 0xb5 ;  # U+2e35 turned semicolon (⸵)
  unicode_a6f6 = 0xea 0x9b 0xb6 ;  # U+a6f6 bamum semicolon (꛶)
  unicode_fe14 = 0xef 0xb8 0x94 ;  # U+fe14 presentation form for vertical semicolon (︔)
  unicode_fe54 = 0xef 0xb9 0x94 ;  # U+fe54 small semicolon (﹔)
  unicode_ff1b = 0xef 0xbc 0x9b ;  # U+ff1b fullwidth semicolon (；)
  unicode_e003b = 0xf3 0xa0 0x80 0xbb ;  # U+e003b tag semicolon (󠀻)
  semicolon = unicode_003b | unicode_061b | unicode_1364 | unicode_204f | unicode_236e | unicode_2e35 | unicode_a6f6 | unicode_fe14 | unicode_fe54 | unicode_ff1b | unicode_e003b ;

  # Punctuation symbols that end a token.
  end_punct = unicode_punct_close | comma | colon | semicolon ;

  # All other punctuation symbols.
  symbols = end_punct | hyphen | dash | unicode_punct ;

  # ASCII art lines.
  lines = '-'{4,} | '='{3,} | '*'{2,} | '.'{4,} | '- '{3,} '-';

}%%
