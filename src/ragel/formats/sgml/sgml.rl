// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine sgml;
  access _state.;
  alphtype unsigned char;
  variable eof _state.eof;
  variable p _state.p;
  variable pe _state.pe;

  action attribute_name_start { _attr_name_start(fpc); }
  action attribute_name_end { _attr_name_end(fpc); }
  action character_start { _character_start(fpc); }
  action character_end { _character_end(fpc); }
  action character_reference_decimal_end { _character_reference_decimal(fpc); }
  action character_reference_hex_end { _character_reference_hex(fpc); }
  action character_reference_named_end { _character_reference_named(fpc); }
  action tag_name_start { _tag_name_start(fpc); }
  action tag_name_end { _tag_name_end(fpc); }

  action create_attribute { _create_attr(); }

  utf8_character_1 = 0x01..0x7f ;
  utf8_character_2 = 0xc0..0xdf 0x80..0xbf ;
  utf8_character_3 = 0xe0..0xef 0x80..0xbf 0x80..0xbf ;
  utf8_character_4 = 0xf0..0xf7 0x80..0xbf 0x80..0xbf 0x80..0xbf ;
  utf8_character = utf8_character_1 | utf8_character_2 | utf8_character_3 | utf8_character_4 ;

  ws_character = 0x09 | 0x0a | 0x0d | 0x20 ;

  character_reference_decimal = ( '&#' digit+ ';' ) >character_start %character_reference_decimal_end ;
  character_reference_hex = ( '&#x'i xdigit+ ';' ) >character_start %character_reference_hex_end ;
  character_reference_named = ( '&' ('amp'i | 'apos'i | 'gt'i | 'lt'i | 'quot'i) ';' ) >character_start %character_reference_named_end ;

  character_text        = ( utf8_character -- [<&] )  >character_start %character_end ;
  character_text_dquote = ( utf8_character -- [<&"] ) >character_start %character_end ;
  character_text_squote = ( utf8_character -- [<&'] ) >character_start %character_end ;
  unescaped_ampersand = ( '&' [ \n] ) >character_start %character_end ;  # Stupid broken real world.

  text = character_reference_decimal | character_reference_hex | character_reference_named | character_text | unescaped_ampersand ;
  name = ( utf8_character -- [<&!?/=>] -- ws_character )+ ;
  ws = ws_character ;

  attribute_name = name >attribute_name_start %attribute_name_end ;
  attribute_value = ( ["] ( character_reference_decimal | character_reference_hex | character_reference_named | character_text_dquote )* ["] )
                  | ( ['] ( character_reference_decimal | character_reference_hex | character_reference_named | character_text_squote )* ['] ) ;
  attribute = attribute_name ws* '=' ws* attribute_value %create_attribute;

  tag_name = name >tag_name_start %tag_name_end ;
  start_tag = '<' tag_name ( ws+ attribute )* ws* '>' ;
  empty_tag = '<' tag_name ( ws+ attribute )* ws* '/>' ;
  end_tag = '</' tag_name ws* '>' ;

  cdata = '<![CDATA[' utf8_character* :>> ']]>' ;

  comment = '<!--' utf8_character* :>> '-->' ;

  xml_decl = '<?xml'i ( ws+ attribute )* ws* '?>' ;

  main := |*
    start_tag => { _create_start_tag_node(); fbreak; } ;
    empty_tag => { _create_empty_tag_node(); fbreak; } ;
    end_tag => { _create_end_tag_node(); fbreak; } ;
    text+ => { _create_text_node(); fbreak; } ;
    cdata => { _create_cdata_node(); fbreak; } ;
    comment => { _create_comment_node(); fbreak; } ;
    xml_decl => { _create_xml_decl_node(); fbreak; } ;
  *|;
}%%


#include <schwa/formats/sgml.h>


namespace schwa {
namespace formats {

%% write data noerror nofinal;

void
SGMLishLexer::_init(void) {
  (void)sgml_en_main;
  %% write init;
}


SGMLishNode *
SGMLishLexer::lex(Pool &node_pool) {
  _node_pool = &node_pool;

  // Don't attempt to lex if we're at EOF.
  if (_state.at_eof())
    return nullptr;
  const auto p_before = _state.p;

  %% write exec;

  // Fail if we ended up in an error state or we didn't consume any input.
  if (_state.cs == %%{ write error; }%%)
    return nullptr;
  else if (_state.p == p_before)
    return nullptr;
  else
    return _node;
}

}  // namespace formats
}  // namespace schwa
