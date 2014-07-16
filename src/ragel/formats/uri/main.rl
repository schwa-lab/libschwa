// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine uri;
  alphtype unsigned char;

  include "uri.rl";

  main := uri ;
}%%

#include <schwa/formats/uri.h>


namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
URILexer::_run(const uint8_t *const input, const size_t nbytes) {
  (void)uri_en_main;  // Shoosh compiler warning about unused variable.
  const uint8_t *p = input, *pe = p + nbytes;
  int cs = 0;

  %% write init;
  %% write exec;

  return cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa
