// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine html;
  alphtype unsigned char;

  include "named_character_references.rl";

  # 4.6. http://www.w3.org/TR/html-markup/syntax.html#character-references
  character_reference_named = '&' named_character_references ';' ;
  character_reference_decimal = '&#' digit+ ';' ;
  character_reference_hex = '&#x'i xdigit+ ';' ;

  # 4.5. http://www.w3.org/TR/html-markup/syntax.html#text-syntax
  utf8_character_1 = 0x01..0x7f ;
  utf8_character_2 = 0xc0..0xdf 0x80..0xbf;
  utf8_character_3 = 0xe0..0xef 0x80..0xbf 0x80..0xbf;
  utf8_character_4 = 0xf0..0xf7 0x80..0xbf 0x80..0xbf 0x80..0xbf;
  utf8_character = utf8_character_1 | utf8_character_2 | utf8_character_3 | utf8_character_4 ;
  text_character = utf8_character - [<] ;
  text = character_reference_named | character_reference_decimal | character_reference_hex | text_character;

  # 4.1. http://www.w3.org/TR/html-markup/syntax.html#doctype-syntax
  html4_doctype_id = 'PUBLIC'i space+ (
        ( '"-//W3C//DTD HTML 4.0//EN"' | "'-//W3C//DTD HTML 4.01//EN'" ) ( space+ ( '"http://www.w3.org/TR/REC-html40/strict.dtd"' | "'http://www.w3.org/TR/REC-html40/strict.dtd'" ) )?
      | ( '"-//W3C//DTD HTML 4.01//EN"' | "'-//W3C//DTD HTML 4.01//EN'" ) ( space+ ( '"http://www.w3.org/TR/html4/strict.dtd"' | "'http://www.w3.org/TR/html4/strict.dtd'" ) )?
      | ( '"-//W3C//DTD XHTML 1.0 Strict//EN"' | "'-//W3C//DTD XHTML 1.0 Strict//EN'" ) space+ ( '"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"' | "'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'" )
      | ( '"-//W3C//DTD XHTML 1.1//EN"' | "'-//W3C//DTD XHTML 1.1//EN'" ) space+ ( '"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd"' | "'http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd'" )
    ) ;
  system_doctype_id = 'SYSTEM'i space+ ( '"about:legacy-compat"' | "'about:legacy-compat'" ) ;
  doctype = '<!DOCTYPE'i space+ 'HTML'i ( space+ ( html4_doctype_id | system_doctype_id ) )? space* '>' ;

  # 4.7. http://www.w3.org/TR/html-markup/syntax.html#comments
  comment = '<!--' text* '-->' ;

  # 4.8. http://www.w3.org/TR/html-markup/syntax.html#cdata-sections
  cdata = '<![CDATA[' text* ']]>' ;

  # 4.4. http://www.w3.org/TR/html-markup/syntax.html#syntax-attributes
  attribute = [^\t\v\f\n\r "'>/=]+ ( space* '=' space* ( (text - [\t\v\f\n\r "'=><`])+ | "'" (text - ['])* "'" | '"' (text - ["])* '"' ) )? ;

  # 4.3. http://www.w3.org/TR/html-markup/syntax.html#syntax-elements
  void_tag_names = 'area'i | 'base'i | 'br'i | 'col'i | 'command'i | 'embed'i | 'hr'i | 'img'i | 'input'i | 'keygen'i | 'link'i | 'meta'i | 'param'i | 'source'i | 'track'i | 'wbr'i ;
  void_tag = '<' void_tag_names ( space+ attribute )* space* '/'? '>' ;
  start_tag = '<' alnum+ ( space+ attribute )* space* '>' ;
  end_tag = '</' alnum+ space* '>' ;

}%%
