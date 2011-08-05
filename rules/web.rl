/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

	# gTLDs from http://www.iana.org/domains/root/db

  generic_tld0 = ".com"i|".edu"i|".gov"i|".mil"i|".net"i|".org"i;
  generic_tld1 = ".aero"i|".arpa"i|".asia"i|".biz"i|".cat"i|".coop"i|".info"i|".int"i;
  generic_tld2 = ".jobs"i|".mobi"i|".museum"i|".name"i|".pro"i|".tel"i|".travel"i|".xxx"i;
	generic_tld = generic_tld0 | generic_tld1 | generic_tld2;

  # ccTLDs from http://www.iana.org/domains/root/db
	# but we're just going to accept all 2 letter codes

  country_code_tld = "." alpha{2};

  top_level_domain = generic_tld | country_code_tld;

  # email username can contain any of: ! # $ % & ' * + - / = ? ^ _ ` { | } ~
  # and non-adjacent periods (that also cannot begin or end the username)
	# according to RFC 5322 (http://tools.ietf.org/html/rfc5322#section-3.2)
	# but you only really see: _ - + and .

  email_username = (alnum | "_" | "-" | "+" | ".")+;

	# domain name labels must start with a letter and cannot have
	# hyphens at the beginning or end according to RFC 1035
	# (http://tools.ietf.org/html/rfc1035 page 8)

  domain_name_label = alpha+ ( (alnum | "-")* alnum )?;
  domain_name = domain_name_label ("." domain_name_label)+ top_level_domain;

	# IP address
	ip_byte = "0"? [0-9]{1,2} | "1" [0-9]{1,2} | "2" [0-4][0-9] | "25" [0-5];
	ip_address = ip_byte "." ip_byte "." ip_byte "." ip_byte;

	host_name = domain_name | ip_address;

  email_address = email_username "@" host_name;

	# URI schemes: http://en.wikipedia.org/wiki/URI_scheme
	# URI schemes: http://www.iana.org/assignments/uri-schemes.html

	uri_official0 = "cid:"i|"data:"i|"dav://"i|"dict://"i|"dns://"i|"fax:"|"file://"i;
	uri_official1 = "ftp://"i|"geo:"i|"go://"i|"gopher://"i|"http://"i|"https://"i;
	uri_official2 = "im://"i|"imap://"i|"ldap://"i|"mailto:"i|"news:"i|"pop://"i;
	uri_official3 = "rsync://"i|"smtp://"i|"sms:"i|"soap://"i|"telnet://"i;

	uri_official = uri_official0 | uri_official1 | uri_official2 | uri_official3;

	uri_extra0 = "aim:"i|"callto:"i|"chrome://"i|"cvs://"i|"doi:"i|"feed:"i;
	uri_extra1 = "finger://"|"git://"i|"gtalk:"i|"irc://"i|"ircs://"i;
	uri_extra2 = "lastfm://"i|"ldaps://"i|"mms://"i|"rmi://"i|"rtmp://"i;
	uri_extra3 = "skype:"i|"ssh://"i|"sftp://"i|"smb://"i|"unreal://"i;
	uri_extra4 = "ws:"i|"webcal://"i;

	uri_extra = uri_extra0 | uri_extra1 | uri_extra2 | uri_extra3 | uri_extra4;

	uri_scheme = uri_official | uri_extra;

	# URI generic syntax from: http://tools.ietf.org/html/rfc3986
  # rare URI characters that are common to regular English punctuation
	# are ignored: ( ) ' and others cannot end URI

	uri_general = (alnum | "-" | "." | "_" | "~");
	uri_gen_delim = (":" | "/" | "?" | "#" | "[" | "]" | "@");
	uri_sub_delim = ("!" | "$" | "&" | "*" | "+" | "," | ";" | "=");
	uri_body = uri_general | uri_gen_delim | uri_sub_delim;

	uri = (uri_scheme uri_body+) -- (any* uri_sub_delim);

}%%
