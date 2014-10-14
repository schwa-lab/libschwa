// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  include uri '../../formats/uri/uri.rl';

  # RFC5322
  #   Internet Message Format
  #   https://tools.ietf.org/html/rfc5322#section-3.4.1
  rfc5322_atext = [a-zA-Z0-9!#$%&'*+\-/=?^_`{|}~] ;
  rfc5322_dot_atom_text = rfc5322_atext+ ( '.' rfc5322_atext+ )* ;
  rfc5322_dot_atom = rfc5322_dot_atom_text ;
  email_address = rfc5322_dot_atom '@' rfc5322_dot_atom ;

  # Twitter usernames and hash tags.
  twitter_username = '@' [A-Za-z0-9_]{1,15} ;
  hash_tag = '#' [A-Za-z0-9_]+ ;

}%%
