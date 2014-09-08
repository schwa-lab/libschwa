// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine html;

  include "html.rl";

  action do_try_recover {
    fhold; fgoto try_recover;
  }

  try_recover := [^>]* '>' @{ fgoto main; } ;
  main := ( ( comment | cdata | doctype | tag | text+ ) $err(do_try_recover) )** ;
}%%

#include <schwa/formats/html.h>
#include <iostream>

namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
HTMLLexer::_run(const uint8_t *const input, const size_t nbytes) {
  (void)html_en_main;  // Shoosh compiler warning about unused variable.
  (void)html_en_try_recover;  // Shoosh compiler warning about unused variable.
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  int cs;

  %% write init;

  _begin_document();
  %% write exec;
  _end_document();

  return cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa
