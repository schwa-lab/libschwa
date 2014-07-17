// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine html_charset_sniff_content_type;

  action content_type_param_key_start { _content_type_param_key_start(fpc); }
  action content_type_param_key_end   { _content_type_param_key_end(fpc); }
  action content_type_param_val_start { _content_type_param_val_start(fpc); }
  action content_type_param_val_end   { _content_type_param_val_end(fpc); }

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

  content_type_value =
    space*
    token '/' token
    (
      space* ';' space*
      token >content_type_param_key_start %content_type_param_key_end
      '='
      ( token | quoted_string ) >content_type_param_val_start %content_type_param_val_end
    )*
    space*
    ;

  main := content_type_value ;
}%%

#include <schwa/formats/html.h>
#include <iostream>

namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
HTMLCharsetSniffer::_run_content_type(const uint8_t *const input, const size_t nbytes) {
  (void)html_charset_sniff_content_type_en_main;  // Shoosh compiler warning about unused variable.
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  int cs = 0;

  %% write init;
  %% write exec;

  if (cs == %%{ write error; }%%) {
    std::cerr << "<<<<<<<<<<<<<<<<<<<" << std::endl;
    std::cerr.write(reinterpret_cast<const char *>(input), nbytes);
    std::cerr << "===================" << std::endl;
    std::cerr.write(reinterpret_cast<const char *>(p), pe - p);
    std::cerr << ">>>>>>>>>>>>>>>>>>>" << std::endl;
  }

  return cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa
