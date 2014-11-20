// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  # From list on Wikipedia: http://en.wikipedia.org/wiki/List_of_emoticons
  # with additions from:
  # http://netforbeginners.about.com/cs/netiquette101/a/bl_emoticons101_2.htm
  # http://netforbeginners.about.com/cs/netiquette101/a/bl_emoticons101_3.htm
  # http://netforbeginners.about.com/cs/netiquette101/a/bl_emoticons101_4.htm
  # http://netforbeginners.about.com/cs/netiquette101/a/bl_emoticons101_5.htm
  emoticon_smile =
      ':)'
    | ':-)'
    | ':->'
    | ':-{)'
    | ':-{)}'
    | ':-}'
    | ':3'
    | ':>'
    | ':]'
    | ':^)'
    | ':c)'
    | ':o)'
    | ':}'
    | '=)'
    | '=]'
    | '>:]'
    ;
  emoticon_laugh =
      '8-D'
    | '8D'
    | ':))'
    | ':-D'
    | ':D'
    | ':^D'
    | '=))'
    | '=-3'
    | '=-D'
    | '=3'
    | '=D'
    | '=^D'
    | '>:D'
    | 'X-D'
    | 'XD'
    | '^^^'
    | 'x-D'
    | 'xD'
    ;
  emoticon_cry =
      ":'("
    | ":\'"
    | ":\'("
    | ":\'-("
    | ':(('
    | ':,('
    | ':.('
    | ':~-('
    | ';*('
    | 'T.T'
    | 'T_T'
    | 'Y.Y'
    | 'Y_Y'
    ;
  emoticon_frown =
      ':('
    | ':-('
    | ':-<'
    | ':-['
    | ':-c'
    | ':<'
    | ':['
    | ':^('
    | ':c'
    | ':{'
    | '<.<'
    | '>.<'
    | '>.>'
    | '>:['
    ;
  emoticon_sad =
      "D-':"
    | ':e'
    | '=('
    | 'D8'
    | 'D:'
    | 'D:<'
    | 'D;'
    | 'D='
    | 'DX'
    | 'v.v'
    ;
  emoticon_lick = ':-9' ;
  emoticon_wink =
      '*-)'
    | ';)'
    | ';-)'
    | ';-D'
    | ';-]'
    | ';;)'
    | ';D'
    | ';]'
    | '>;->'
    | '>;]'
    | '`:-)'
    ;
  emoticon_tongue =
      '8-p'i
    | ':-b'
    | ':-p'i
    | ':-r'
    | ':-Þ'
    | ':^P'i
    | ':b'
    | ':p'i
    | ':Þ'
    | ';p'i
    | '=p'i
    | '>:p'i
    | 'x-p'i
    | 'xp'i
    ;
  emoticon_surprise =
      '8-o'i
    | ':-o'i
    | ':C'
    | ':o'i
    | '=O'
    | '>:o'i
    | '°o°'i
    ;
  emoticon_skeptical =
      ':-.'
    | ':-/'
    | ':/'
    | ':S'
    | ':\\'
    | '=/'
    | '=\\'
    | '>:/'
    | '>:\\'
    ;
  emoticon_straight =
      ':-|'
    | ':|'
    ;
  emoticon_drool =
      ':)~'
    | ':-)>.' '.'+
    | ':p~~'
    | '=F'
    ;
  emoticon_sealed =
      ':#'
    | ':$'
    | ':-#'
    | ':-&'
    | ':-x'i
    | ':x'i
    | '=X'
    | '>:x'i
    ;
  emoticon_angel =
      '0:-3'
    | '0:3'
    | 'O:)'
    | 'O:-)'
    | 'O:-)'
    ;
  emoticon_evil =
      '>:)'
    | '>:-)'
    | '>;)'
    ;
  emoticon_shades =
      '8-)'
    | '>B]'
    | 'B-)'
    | 'B^P'
    ;
  emoticon_high5 =
      '^5'
    | 'o/\o'
    ;
  emoticon_rose = "@}-;-'---" ;
  emoticon_cap =
      'd:-)'
    | 'qB-)'
    ;
  emoticon_angry =
      ':-@[1]'
    | ':-Q'
    | ':-|'
    | ';('
    | '>:('
    | '>:-('
    | '>:-C'
    | '>:C'
    | '>:O'
    | 'D-:<'
    | 'D:<'
    | 'D<'
    | '`_´'
    | 'x('i
    | '~ :-('
    ;
  emoticon_kiss =
      ':*'
    | ':-*'
    | ':-><'
    | ':-{}'
    | '=^*'
    | '|-<>'
    ;
  emoticon_heart =
      '</3'
    | '<3'
    | '<3' '3'+
    | '=(('
    ;
  emoticon_wave =
      '*\o/*'
    | '\^_^/'
    | '\o'
    | '\o/'
    | 'o/'
    ;
  emoticon_brain =
      '%*}'
    | '%+\\'
    | '%-)'
    | '%-/'
    | '%-6'
    | '%-|'
    ;
  emoticon_confused =
      '%-('
    | '%-)'
    | ':-S'
    | ':-s'
    | ':>'
    | ':@'
    | ':~/'
    ;
  emoticon_sleep =
      '|-('
    | '|-O'
    | '|-|'
    | '|^o'
    ;
  emoticon_flame =
      '~:-('
    | '~='
    | '~=='
    | '~~:-('
    | '~~:['
    ;
  emoticon_misc =
      ":\'>"
    | '#:-S'
    | '*-)'
    | ':&'
    | ':-B'
    | ':-SS'
    | ':-V'
    | ':-Y'
    | ':3'
    | ':u'
    | '>.<'
    | '?('
    | '?-('
    | 'X_X'
    | ']:-)'
    | ']:->'
    | '~:o'
    ;

  # List of Unicode pages which contain emoji from https://en.wikipedia.org/wiki/Emoji

  # Miscellaneous Symbols Unicode page (U+2600 - U+26FF).
  unicode_miscellaneous_symbols_page = 0xe2 0x98..0x9b 0x80..0xbf ;

  # Miscellaneous Symbols and Pictographs Unicode page (U+1F300 - U+1F5FF)
  unicode_miscellaneous_symbols_pictographs_page = 0xf0 0x9f 0x8c..0x97 0x80..0xbf ;

  # Emoticons Unicode page (U+1F600 - U+1F64F)
  unicode_emoticons_page = ( 0xf0 0x9f 0x98 0x80..0xbf ) | 0xf0 0x9f 0x99 0x80..0x8f ;

  # Transport and Map Symbols Unicode page (U+1F680 - U+1F6ff)
  unicode_transport_and_map_symbols_page = 0xf0 0x9f 0x9a..0x9b 0x80..0xbf ;

  emoticon =
      emoticon_smile
    | emoticon_laugh
    | emoticon_cry
    | emoticon_frown
    | emoticon_sad
    | emoticon_lick
    | emoticon_wink
    | emoticon_tongue
    | emoticon_surprise
    | emoticon_skeptical
    | emoticon_straight
    | emoticon_drool
    | emoticon_sealed
    | emoticon_angel
    | emoticon_evil
    | emoticon_shades
    | emoticon_high5
    | emoticon_rose
    | emoticon_cap
    | emoticon_angry
    | emoticon_kiss
    | emoticon_heart
    | emoticon_wave
    | emoticon_brain
    | emoticon_confused
    | emoticon_sleep
    | emoticon_flame
    | emoticon_misc
    | unicode_miscellaneous_symbols_page
    | unicode_miscellaneous_symbols_pictographs_page
    | unicode_emoticons_page
    | unicode_transport_and_map_symbols_page
    ;

}%%
