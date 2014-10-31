// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine plain_text;
  access _state.;
  alphtype unsigned char;
  variable eof _state.eof;
  variable p _state.p;
  variable pe _state.pe;

  # Single UTF-8 units (single Unicode code points).
  unicode_1 = 0x00..0x7f ;
  unicode_2 = 0xc0..0xdf 0x80..0xbf ;
  unicode_3 = 0xe0..0xef 0x80..0xbf 0x80..0xbf ;
  unicode_4 = 0xf0..0xf7 0x80..0xbf 0x80..0xbf 0x80..0xbf ;
  unicode = unicode_1 | unicode_2 | unicode_3 | unicode_4 ;

  # Unicode spaces.
  unicode_0020 = 0x20 ;            # U+0020 space ( )
  unicode_00a0 = 0xc2 0xa0 ;       # U+00a0 no-break space ( )
  unicode_1680 = 0xe1 0x9a 0x80 ;  # U+1680 ogham space mark ( )
  unicode_2000 = 0xe2 0x80 0x80 ;  # U+2000 en quad ( )
  unicode_2001 = 0xe2 0x80 0x81 ;  # U+2001 em quad ( )
  unicode_2002 = 0xe2 0x80 0x82 ;  # U+2002 en space ( )
  unicode_2003 = 0xe2 0x80 0x83 ;  # U+2003 em space ( )
  unicode_2004 = 0xe2 0x80 0x84 ;  # U+2004 three-per-em space ( )
  unicode_2005 = 0xe2 0x80 0x85 ;  # U+2005 four-per-em space ( )
  unicode_2006 = 0xe2 0x80 0x86 ;  # U+2006 six-per-em space ( )
  unicode_2007 = 0xe2 0x80 0x87 ;  # U+2007 figure space ( )
  unicode_2008 = 0xe2 0x80 0x88 ;  # U+2008 punctuation space ( )
  unicode_2009 = 0xe2 0x80 0x89 ;  # U+2009 thin space ( )
  unicode_200a = 0xe2 0x80 0x8a ;  # U+200a hair space ( )
  unicode_202f = 0xe2 0x80 0xaf ;  # U+202f narrow no-break space ( )
  unicode_205f = 0xe2 0x81 0x9f ;  # U+205f medium mathematical space ( )
  unicode_3000 = 0xe3 0x80 0x80 ;  # U+3000 ideographic space (　)
  unicode_space = unicode_0020 | unicode_00a0 | unicode_1680 | unicode_2000 | unicode_2001 | unicode_2002 | unicode_2003 | unicode_2004 | unicode_2005 | unicode_2006 | unicode_2007 | unicode_2008 | unicode_2009 | unicode_200a | unicode_202f | unicode_205f | unicode_3000 ;

  # Unicode line separators.
  unicode_2028 = 0xe2 0x80 0xa8 ;  # U+2028 line separator
  unicode_line_space = '\r\n' | '\r' | '\n' | unicode_2028 ;

  # Unicode paragraph separators.
  unicode_2029 = 0xe2 0x80 0xa9 ;  # U+2029 paragraph separator ( )
  unicode_paragraph_space = unicode_2029 ;

  main := |*
    unicode_paragraph_space+ => { _create_paragraph(); } ;
    ( unicode_line_space unicode_space* ){2,} => { _create_paragraph(); } ;
    unicode;
  *|;
}%%


#include <schwa/formats/plain-text.h>


namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
PlainTextLexer::_lex(void) {
  (void)plain_text_en_main;
  %% write init;
  %% write exec;
  return _state.cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa
