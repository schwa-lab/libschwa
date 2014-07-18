// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine http;
  alphtype unsigned char;

  include uri "../uri/uri.rl";

  action content_length_start   { _content_length_start(fpc); }
  action content_length_consume { _content_length_consume(fc); }
  action content_length_end     { _content_length_end(fpc); }

  action content_type_type_start { _content_type_type_start(fpc); }
  action content_type_type_end   { _content_type_type_end(fpc); }
  action content_type_subtype_start { _content_type_subtype_start(fpc); }
  action content_type_subtype_end   { _content_type_subtype_end(fpc); }
  action content_type_param_key_start { _content_type_param_key_start(fpc); }
  action content_type_param_key_end   { _content_type_param_key_end(fpc); }
  action content_type_param_val_start { _content_type_param_val_start(fpc); }
  action content_type_param_val_end   { _content_type_param_val_end(fpc); }

  action message_body_start   { _message_body_start(fpc); }
  action message_body_consume { _message_body_consume(fc); }
  action message_body_test    { _message_body_test() }
  action message_body_end     { _message_body_end(fpc); }

  action status_code_consume { _status_code_consume(fc); }

  action header_field_error {
    fhold;
    fgoto consume_bad_header_field;
  }
  action consume_bad_header_field_end {
    fhold;
    fgoto header_field_recover;
  }

  # RFC5234
  #   Augmented BNF for Syntax Specifications: ABNF
  #   http://tools.ietf.org/html/rfc5234#appendix-B.1
  char = ascii ;
  crlf = '\r\n' | '\n' ;
  ctl = cntrl | 0x7f ;
  octet = any ;
  sp = ' ' ;
  vchar = 0x21..0x7e ;
  wsp = [ \t] ;
  lwsp = ( wsp | crlf wsp )* ;

  # RFC7230
  #  Hypertext Transfer Protocol (HTTP/1.1): Message Syntax and Routing
  #  http://tools.ietf.org/html/rfc7230
  #
  # https://www.mnot.net/blog/2014/06/07/rfc2616_is_dead
  http_name = 'HTTP' ;
  http_version = http_name '/' digit '.' digit ;

  tchar = [!#$%&'*+\-.^_`|~0-9a-zA-Z ] ;
  token = tchar+ ;

  obs_text = 0x80..0xff ;
  quoted_pair = '\\' ( wsp | vchar | obs_text ) ;
  qdtext = wsp | 0x21 | 0x23..0x5b | 0x5d..0x7e | obs_text ;
  quoted_string = '"' ( qdtext | quoted_pair  )* '"' ;

  ctext = wsp | 0x21..0x27 | 0x2a..0x5b | 0x5d..0x7e | obs_text ;
  comment = '(' ( ctext | quoted_pair )* ')' ;

  method = 'GET' | 'HEAD' | 'POST' | 'PUT' | 'DELETE' | 'CONNECT' | 'OPTIONS' | 'TRACE' ;
  status_code = digit{3} ;
  reason_phrase = ( wsp | vchar | obs_text )* ;

  obs_fold = crlf wsp+ ;
  field_vchar = vchar | obs_text ;
  field_content = field_vchar | wsp+ field_vchar ;
  field_value = ( field_content | obs_fold )** ;
  field_name = token ;
  header_field =
      'content-length'i
      wsp* ':' wsp*
      digit+ >content_length_start $content_length_consume %content_length_end
      wsp*
    | 'content-type'i
      wsp* ':' wsp*
      token >content_type_type_start %content_type_type_end
      '/'
      token >content_type_subtype_start %content_type_subtype_end
      (
        wsp* ';' wsp*
        token >content_type_param_key_start %content_type_param_key_end
        '='
        ( token | quoted_string ) >content_type_param_val_start %content_type_param_val_end
      )* wsp*
    | field_name wsp* ':' wsp* field_value wsp*
    ;
  consume_bad_header_field := [^\r\n]* [\r\n] $consume_bad_header_field_end ;

  absolute_path = ( '/' segment )+ ;

  asterisk_form = '*' ;
  authority_form = authority ;
  absolute_form = absolute_uri ;
  origin_form = absolute_path ( '?' query )? ;
  request_target = origin_form | absolute_form | authority_form | asterisk_form ;

  request_line = method sp request_target sp http_version crlf ;
  status_line = http_version sp status_code $status_code_consume ( sp reason_phrase )? crlf ;
  start_line = request_line | status_line ;

  message_body = ( octet when message_body_test $message_body_consume )* ;
  http_message =
    start_line
    ( header_field $!header_field_error header_field_recover: crlf )* crlf
    message_body >message_body_start %message_body_end
    space*
    ;
}%%
