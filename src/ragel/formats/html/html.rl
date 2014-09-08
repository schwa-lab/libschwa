// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine html;
  alphtype unsigned char;

  action decimal_character_reference_start { _decimal_character_reference_start(fpc); }
  action decimal_character_reference_end   { _decimal_character_reference_end(fpc); }
  action hex_character_reference_start     { _hex_character_reference_start(fpc); }
  action hex_character_reference_end       { _hex_character_reference_end(fpc); }
  action named_character_reference_start   { _named_character_reference_start(fpc); }
  action named_character_reference_end     { _named_character_reference_end(fpc); }
  action text_character_start              { _text_character_start(fpc); }
  action text_character_end                { _text_character_end(fpc); }

  action cdata_start   { _cdata_start(fpc); }
  action cdata_end     { _cdata_end(fpc); }
  action comment_start { _comment_start(fpc); }
  action comment_end   { _comment_end(fpc); }
  action doctype_start { _doctype_start(fpc); }
  action doctype_end   { _doctype_end(fpc); }

  action open_tag_start        { _open_tag_start(fpc); }
  action open_tag_end          { _open_tag_end(fpc); }
  action open_tag_name_start   { _open_tag_name_start(fpc); }
  action open_tag_name_end     { _open_tag_name_end(fpc); }
  action open_tag_self_closing { _open_tag_self_closing(); }
  action close_tag_start       { _close_tag_start(fpc); }
  action close_tag_end         { _close_tag_end(fpc); }
  action close_tag_name_start  { _close_tag_name_start(fpc); }
  action close_tag_name_end    { _close_tag_name_end(fpc); }

  # 4.6. http://www.w3.org/TR/html-markup/syntax.html#character-references
  character_reference_named = '&' alnum+ >named_character_reference_start %named_character_reference_end ';' ;
  character_reference_hex = '&#x'i xdigit+ >hex_character_reference_start %hex_character_reference_end ';' ;
  character_reference_decimal = '&#' digit+ >decimal_character_reference_start %decimal_character_reference_end ';' ;

  # 4.5. http://www.w3.org/TR/html-markup/syntax.html#text-syntax
  utf8_character_1 = 0x01..0x7f ;
  utf8_character_2 = 0xc0..0xdf 0x80..0xbf ;
  utf8_character_3 = 0xe0..0xef 0x80..0xbf 0x80..0xbf ;
  utf8_character_4 = 0xf0..0xf7 0x80..0xbf 0x80..0xbf 0x80..0xbf ;
  utf8_character = utf8_character_1 | utf8_character_2 | utf8_character_3 | utf8_character_4 ;
  text_character = ( utf8_character - [<] ) >text_character_start %text_character_end ;
  text_character_lt = [<] >text_character_start %text_character_end ;
  text = character_reference_named | character_reference_hex | character_reference_decimal | text_character ;

  # 4.1. http://www.w3.org/TR/html-markup/syntax.html#doctype-syntax and http://www.w3.org/QA/2002/04/valid-dtd-list.html
#  html2_doctype_a                 = '-//' ( 'IETF'i | 'W3C'i ) '//DTD HTML 2.0//EN'i ;
#  html3_doctype_a                 = '-//' ( 'IETF'i | 'W3C'i ) '//DTD HTML 3.'i ('0'..'2') ' FINAL'i? '//EN'i ;
#  html4_doctype_strict_a          = '-//W3C//DTD HTML 4.0'i '1'? '//EN'i ;
#  html4_doctype_transitional_a    = '-//W3C//DTD HTML 4.0'i '1'? ' Transitional//EN'i ;
#  html4_doctype_frameset_a        = '-//W3C//DTD HTML 4.0'i '1'? ' Frameset//EN'i ;
#  xhtml1_0_doctype_strict_a       = '-//W3C//DTD XHTML 1.0 Strict//EN'i ;
#  xhtml1_0_doctype_transitional_a = '-//W3C//DTD XHTML 1.0 Transitional//EN'i ;
#  xhtml1_0_doctype_frameset_a     = '-//W3C//DTD XHTML 1.0 Frameset//EN'i ;
#  xhtml1_1_doctype_strict_a       = '-//W3C//DTD XHTML 1.1//EN'i ;
#  xhtml1_1_doctype_basic_a        = '-//W3C//DTD XHTML Basic 1.1//EN'i ;
#  html4_doctype_strict_b          = 'http://www.w3.org/TR/html4/strict.dtd'i ;
#  html4_doctype_transitional_b    = 'http://www.w3.org/TR/html4/loose.dtd'i ;
#  html4_doctype_frameset_b        = 'http://www.w3.org/TR/html4/frameset.dtd'i ;
#  xhtml1_0_doctype_strict_b       = 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'i ;
#  xhtml1_0_doctype_transitional_b = 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'i ;
#  xhtml1_0_doctype_frameset_b     = 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd'i ;
#  xhtml1_1_doctype_strict_b       = 'http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd'i ;
#  xhtml1_1_doctype_basic_b        = 'http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd'i ;
#  html4_doctype_id = 'PUBLIC'i space+ (
#        ( '"' html4_doctype_strict_a '"' | "'" html4_doctype_strict_a "'" ) ( space+ ( '"' html4_doctype_strict_b '"' | "'" html4_doctype_strict_b "'" ) )?
#      | ( '"' html4_doctype_transitional_a '"' | "'" html4_doctype_transitional_a "'" ) ( space+ ( '"' html4_doctype_transitional_b '"' | "'" html4_doctype_transitional_b "'" ) )?
#      | ( '"' html4_doctype_frameset_a '"' | "'" html4_doctype_frameset_a "'" ) ( space+ ( '"' html4_doctype_frameset_b '"' | "'" html4_doctype_frameset_b "'" ) )?
#      | ( '"' xhtml1_0_doctype_strict_a '"' | "'" xhtml1_0_doctype_strict_a "'" ) ( space+ ( '"' xhtml1_0_doctype_strict_b '"' | "'" xhtml1_0_doctype_strict_b "'" ) )?
#      | ( '"' xhtml1_0_doctype_transitional_a '"' | "'" xhtml1_0_doctype_transitional_a "'" ) ( space+ ( '"' xhtml1_0_doctype_transitional_b '"' | "'" xhtml1_0_doctype_transitional_b "'" ) )?
#      | ( '"' xhtml1_0_doctype_frameset_a '"' | "'" xhtml1_0_doctype_frameset_a "'" ) ( space+ ( '"' xhtml1_0_doctype_frameset_b '"' | "'" xhtml1_0_doctype_frameset_b "'" ) )?
#      | ( '"' xhtml1_1_doctype_strict_a '"' | "'" xhtml1_1_doctype_strict_a "'" ) ( space+ ( '"' xhtml1_1_doctype_strict_b '"' | "'" xhtml1_1_doctype_strict_b "'" ) )?
#      | ( '"' xhtml1_1_doctype_basic_a '"' | "'" xhtml1_1_doctype_basic_a "'" ) ( space+ ( '"' xhtml1_1_doctype_basic_b '"' | "'" xhtml1_1_doctype_basic_b "'" ) )?
#      | ( '"' html3_doctype_a '"' | "'" html3_doctype_a "'" )
#      | ( '"' html2_doctype_a '"' | "'" html2_doctype_a "'" )
#    ) ;
#  system_doctype_id = 'SYSTEM'i space+ ( '"about:legacy-compat"' | "'about:legacy-compat'" ) ;
#  doctype = '<!DOCTYPE'i >doctype_start space+ 'HTML'i ( space+ ( html4_doctype_id | system_doctype_id ) )? space* '>' %doctype_end ;
  doctype = '<!DOCTYPE'i >doctype_start [^>]* '>' %doctype_end ;

  # 4.7. http://www.w3.org/TR/html-markup/syntax.html#comments
  comment = '<!--' >comment_start ( text | text_character_lt )* :>> '-->' %comment_end ;

  # 4.8. http://www.w3.org/TR/html-markup/syntax.html#cdata-sections
  cdata = '<![CDATA[' >cdata_start text* :>> ']]>' %cdata_end ;

  # 4.4. http://www.w3.org/TR/html-markup/syntax.html#syntax-attributes
  attribute = [^\t\v\f\n\r "'>/=]+ ( space* '=' space* ( (text - [\t\v\f\n\r "'><`])+ | "'" (text - ['])* "'" | '"' (text - ["])* '"' ) )? ;
  tag_attributes = ( space+ attribute )* space* ;

  # 4.3. http://www.w3.org/TR/html-markup/syntax.html#syntax-elements
  title_tag = '<title'i tag_attributes '>' ((text | text_character_lt)* -- '</title'i) '</title'i space* '>' ;
  textarea_tag = '<textarea'i tag_attributes '>' ((text | text_character_lt)* -- '</textarea'i) '</textarea'i space* '>' ;
  script_tag = '<script'i tag_attributes '>' (text_character | text_character_lt)* :>> '</script'i space* '>' ;
  style_tag = '<style'i tag_attributes '>' ((text_character | text_character_lt)* -- '</style'i) '</style'i space* '>' ;

  start_tag = '<' >open_tag_start ( alnum | [:] )+ >open_tag_name_start %open_tag_name_end tag_attributes ( '/' %open_tag_self_closing )? '>' %open_tag_end ;
  end_tag = '</' >close_tag_start ( alnum | [:] )+ >close_tag_name_start %close_tag_name_end space* '>' %close_tag_end ;
  template_language_tag = '<?' any* :>> '?>' | '<%' any* :>> '%>' ;

  tag = end_tag | title_tag | textarea_tag | script_tag | style_tag | start_tag | template_language_tag ;
}%%
