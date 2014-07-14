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
  utf8_character_2 = 0xc0..0xdf 0x80..0xbf ;
  utf8_character_3 = 0xe0..0xef 0x80..0xbf 0x80..0xbf ;
  utf8_character_4 = 0xf0..0xf7 0x80..0xbf 0x80..0xbf 0x80..0xbf ;
  utf8_character = utf8_character_1 | utf8_character_2 | utf8_character_3 | utf8_character_4 ;
  text_character = utf8_character - [<] ;
  text = character_reference_named | character_reference_decimal | character_reference_hex | text_character ;

  # 4.1. http://www.w3.org/TR/html-markup/syntax.html#doctype-syntax and http://www.w3.org/QA/2002/04/valid-dtd-list.html
  html2_doctype_a                 = '-//' ( 'IETF'i | 'W3C'i ) '//DTD HTML 2.0//EN'i ;
  html3_doctype_a                 = '-//' ( 'IETF'i | 'W3C'i ) '//DTD HTML 3.'i ('0'..'2') ' FINAL'i? '//EN'i ;
  html4_doctype_strict_a          = '-//W3C//DTD HTML 4.0'i '1'? '//EN'i ;
  html4_doctype_transitional_a    = '-//W3C//DTD HTML 4.0'i '1'? ' Transitional//EN'i ;
  html4_doctype_frameset_a        = '-//W3C//DTD HTML 4.0'i '1'? ' Frameset//EN'i ;
  xhtml1_0_doctype_strict_a       = '-//W3C//DTD XHTML 1.0 Strict//EN'i ;
  xhtml1_0_doctype_transitional_a = '-//W3C//DTD XHTML 1.0 Transitional//EN'i ;
  xhtml1_0_doctype_frameset_a     = '-//W3C//DTD XHTML 1.0 Frameset//EN'i ;
  xhtml1_1_doctype_strict_a       = '-//W3C//DTD XHTML 1.1//EN'i ;
  xhtml1_1_doctype_basic_a        = '-//W3C//DTD XHTML Basic 1.1//EN'i ;
  html4_doctype_strict_b          = 'http://www.w3.org/TR/html4/strict.dtd'i ;
  html4_doctype_transitional_b    = 'http://www.w3.org/TR/html4/loose.dtd'i ;
  html4_doctype_frameset_b        = 'http://www.w3.org/TR/html4/frameset.dtd'i ;
  xhtml1_0_doctype_strict_b       = 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'i ;
  xhtml1_0_doctype_transitional_b = 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'i ;
  xhtml1_0_doctype_frameset_b     = 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd'i ;
  xhtml1_1_doctype_strict_b       = 'http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd'i ;
  xhtml1_1_doctype_basic_b        = 'http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd'i ;
  html4_doctype_id = 'PUBLIC'i space+ (
        ( '"' html4_doctype_strict_a '"' | "'" html4_doctype_strict_a "'" ) ( space+ ( '"' html4_doctype_strict_b '"' | "'" html4_doctype_strict_b "'" ) )?
      | ( '"' html4_doctype_transitional_a '"' | "'" html4_doctype_transitional_a "'" ) ( space+ ( '"' html4_doctype_transitional_b '"' | "'" html4_doctype_transitional_b "'" ) )?
      | ( '"' html4_doctype_frameset_a '"' | "'" html4_doctype_frameset_a "'" ) ( space+ ( '"' html4_doctype_frameset_b '"' | "'" html4_doctype_frameset_b "'" ) )?
      | ( '"' xhtml1_0_doctype_strict_a '"' | "'" xhtml1_0_doctype_strict_a "'" ) ( space+ ( '"' xhtml1_0_doctype_strict_b '"' | "'" xhtml1_0_doctype_strict_b "'" ) )?
      | ( '"' xhtml1_0_doctype_transitional_a '"' | "'" xhtml1_0_doctype_transitional_a "'" ) ( space+ ( '"' xhtml1_0_doctype_transitional_b '"' | "'" xhtml1_0_doctype_transitional_b "'" ) )?
      | ( '"' xhtml1_0_doctype_frameset_a '"' | "'" xhtml1_0_doctype_frameset_a "'" ) ( space+ ( '"' xhtml1_0_doctype_frameset_b '"' | "'" xhtml1_0_doctype_frameset_b "'" ) )?
      | ( '"' xhtml1_1_doctype_strict_a '"' | "'" xhtml1_1_doctype_strict_a "'" ) ( space+ ( '"' xhtml1_1_doctype_strict_b '"' | "'" xhtml1_1_doctype_strict_b "'" ) )?
      | ( '"' xhtml1_1_doctype_basic_a '"' | "'" xhtml1_1_doctype_basic_a "'" ) ( space+ ( '"' xhtml1_1_doctype_basic_b '"' | "'" xhtml1_1_doctype_basic_b "'" ) )?
      | ( '"' html3_doctype_a '"' | "'" html3_doctype_a "'" )
      | ( '"' html2_doctype_a '"' | "'" html2_doctype_a "'" )
    ) ;
  system_doctype_id = 'SYSTEM'i space+ ( '"about:legacy-compat"' | "'about:legacy-compat'" ) ;
  doctype = '<!DOCTYPE'i space+ 'HTML'i ( space+ ( html4_doctype_id | system_doctype_id ) )? space* '>' ;

  # 4.7. http://www.w3.org/TR/html-markup/syntax.html#comments
  comment = '<!--' (text | [<])* '-->' ;

  # 4.8. http://www.w3.org/TR/html-markup/syntax.html#cdata-sections
  cdata = '<![CDATA[' text* ']]>' ;

  # 4.4. http://www.w3.org/TR/html-markup/syntax.html#syntax-attributes
  attribute = [^\t\v\f\n\r "'>/=]+ ( space* '=' space* ( (text - [\t\v\f\n\r "'><`])+ | "'" (text - ['])* "'" | '"' (text - ["])* '"' ) )? ;

  # 4.3. http://www.w3.org/TR/html-markup/syntax.html#syntax-elements
  title_tag = '<title'i ( space+ attribute )* space* '>' ((text | [<])* -- '</title'i)  '</title'i space* '>' ;
  textarea_tag = '<textarea'i ( space+ attribute )* space* '>' ((text | [<])* -- '</textarea'i)  '</textarea'i space* '>' ;
  script_tag = '<script'i ( space+ attribute )* space* '>' ((text_character | [<])* -- '</script'i)  '</script'i space* '>' ;
  style_tag = '<style'i ( space+ attribute )* space* '>' ((text_character | [<])* -- '</style'i)  '</style'i space* '>' ;

  start_tag = '<' ( alnum | [:] )+ ( space+ attribute )* space* '/'? '>' ;
  end_tag = '</' ( alnum | [:] )+ space* '>' ;

  tag = title_tag | textarea_tag | script_tag | style_tag | start_tag | end_tag ;

}%%
