// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine uri;
  alphtype unsigned char;

  # RFC1123
  #   Requirements for Internet Hosts -- Application and Support
  #   http://tools.ietf.org/html/rfc1123
  dec_octet = digit | '1'..'9' digit | '1' digit{2} | '2' '0'..'4' digit | '25' '0'..'5' ;
  ipv4address = dec_octet ( '.' dec_octet ){3} ;

  # RFC3513
  #   Internet Protocol Version 6 (IPv6) Addressing Architecture
  #   http://tools.ietf.org/html/rfc3986
  h16 = xdigit{1,4} ;
  ls32 = ( h16 ':' h16 ) | ipv4address ;
  ipv6address =
                                     ( h16 ':' ){6} ls32
    |                           '::' ( h16 ':' ){5} ls32
    | (                  h16 )? '::' ( h16 ':' ){4} ls32
    | ( ( h16 ':' ){0,1} h16 )? '::' ( h16 ':' ){3} ls32
    | ( ( h16 ':' ){0,2} h16 )? '::' ( h16 ':' ){2} ls32
    | ( ( h16 ':' ){0,3} h16 )? '::'   h16 ':'      ls32
    | ( ( h16 ':' ){0,4} h16 )? '::'                ls32
    | ( ( h16 ':' ){0,5} h16 )? '::'                h16
    | ( ( h16 ':' ){0,6} h16 )? '::'
    ;

  # RFC3986
  #   Uniform Resource Identifier (URI): Generic Syntax
  #   http://tools.ietf.org/html/rfc3986
  pct_encoded = '%' xdigit{2} ;

  gen_delims = [][:/?#@] ;
  sub_delims = [!$&'()*+,;=] ;
  reserved = gen_delims | sub_delims ;
  unreserved = alpha | digit | [\-._~] ;

  pchar = unreserved | pct_encoded | sub_delims | [:@] ;
  query = ( pchar | [/?] )* ;
  fragment = ( pchar | [/?] )* ;

  segment = pchar* ;
  segment_nz = pchar+ ;
  segment_nz_nc = ( unreserved | pct_encoded | sub_delims | [@] )+ ;

  path_abempty = ( '/' segment )* ;
  path_absolute = '/' ( segment_nz ( '/' segment )* )? ;
  path_rootless = segment_nz ( '/' segment )* ;

  ipvfuture = 'v' xdigit+ '.' ( unreserved | sub_delims | ':' )+ ;
  ip_literal = '[' ( ipv6address | ipvfuture ) ']' ;
  reg_name = ( unreserved | pct_encoded | sub_delims )* ;
  host = ip_literal | ipv4address | reg_name ;
  port = digit* ;

  userinfo = ( unreserved | pct_encoded | sub_delims | ':' )* ;
  authority = ( userinfo '@' )? host ( ':' port )? ;

  hier_part = ( '//' authority path_abempty ) | path_absolute | path_rootless ;
  scheme = alpha ( alpha | digit | [\-+.] )* ;

  uri          = scheme ':' hier_part? ( '?' query )? ( '#' fragment )? ;
  absolute_uri = scheme ':' hier_part? ( '?' query )? ;

}%%
