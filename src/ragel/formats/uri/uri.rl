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
  sub_delims = [!$&'*+,;=] ;
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

  # Manually whitelist these so we don't overmatch this very generic rule (e.g. "listing:)" matches URI).
  # https://en.wikipedia.org/wiki/URI_scheme
  ## scheme = alpha ( alpha | digit | [\-+.] )* ;
  scheme =
      'aaa'
    | 'about'
    | 'acap'
    | 'acct'
    | 'adiumxtra'
    | 'afp'
    | 'afs'
    | 'aim'
    | 'app'
    | 'apt'
    | 'attachment'
    | 'aw'
    | 'barion'
    | 'beshare'
    | 'bitcoin'
    | 'bolo'
    | 'callto'
    | 'cap'
    | 'chrome'
    | 'chrome'
    | 'chrome-extension'
    | 'cid'
    | 'coap'
    | 'com-eventbrite-attendee'
    | 'content'
    | 'crid'
    | 'cvs'
    | 'dav'
    | 'dict'
    | 'dlna-playsingle'
    | 'dns'
    | 'doi'
    | 'dtn'
    | 'dvb'
    | 'ed2k'
    | 'facetime'
    | 'fax'
    | 'feed'
    | 'file'
    | 'finger'
    | 'fish'
    | 'ftp'
    | 'geo'
    | 'gg'
    | 'git'
    | 'gizmoproject'
    | 'go'
    | 'gopher'
    | 'gtalk'
    | 'h323'
    | 'hcp'
    | 'http'
    | 'https'
    | 'iax'
    | 'icap'
    | 'icon'
    | 'im'
    | 'imap'
    | 'info'
    | 'ipn'
    | 'ipp'
    | 'irc'
    | 'irc6'
    | 'ircs'
    | 'iris'
    | 'itms'
    | 'jabber'
    | 'jar'
    | 'javascript'
    | 'jdbc'
    | 'jms'
    | 'keyparc'
    | 'lastfm'
    | 'ldap'
    | 'ldaps'
    | 'magnet'
    | 'mailserver'
    | 'mailto'
    | 'maps'
    | 'market'
    | 'message'
    | 'mid'
    | 'mms'
    | 'modem'
    | 'ms-help'
    | 'ms-settings-power'
    | 'msnim'
    | 'msrp'
    | 'mtqp'
    | 'mumble'
    | 'mupdate'
    | 'mvn'
    | 'news'
    | 'nfs'
    | 'ni'
    | 'nntp'
    | 'notes'
    | 'nsfw'
    | 'oid'
    | 'opaquelocktoken'
    | 'pack'
    | 'palm'
    | 'paparazzi'
    | 'pkcs11'
    | 'platform'
    | 'pop'
    | 'pres'
    | 'prospero'
    | 'proxy'
    | 'psyc'
    | 'query'
    | 'reload'
    | 'res'
    | 'resource'
    | 'rmi'
    | 'rsync'
    | 'rtmfp'
    | 'rtmp'
    | 'rtsp'
    | 'secondlife'
    | 'service'
    | 'session'
    | 'sftp'
    | 'sgn'
    | 'shttp'
    | 'sieve'
    | 'sip'
    | 'sips'
    | 'skype'
    | 'smb'
    | 'sms'
    | 'snews'
    | 'snmp'
    | 'soap.beep'
    | 'soldat'
    | 'spotify'
    | 'ssh'
    | 'steam'
    | 'stratum'
    | 'stun'
    | 'svn'
    | 'tag'
    | 'teamspeak'
    | 'tel'
    | 'telnet'
    | 'tftp'
    | 'things'
    | 'thismessage'
    | 'tip'
    | 'tn3270'
    | 'turn'
    | 'tv'
    | 'udp'
    | 'udp'
    | 'unreal'
    | 'urn'
    | 'ut2004'
    | 'vemmi'
    | 'ventrilo'
    | 'videotex'
    | 'view-source'
    | 'wais'
    | 'webcal'
    | 'ws'
    | 'wtai'
    | 'wyciwyg'
    | 'xcon'
    | 'xfire'
    | 'xmlrpc.beep'
    | 'xmpp'
    | 'xri'
    | 'ymsgr'
    | 'z39.50'
    | 'z39.50r'
    | 'z39.50s'
  ;

  uri          = scheme ':' hier_part ( '?' query )? ( '#' fragment )? ;
  absolute_uri = scheme ':' hier_part ( '?' query )? ;

}%%
