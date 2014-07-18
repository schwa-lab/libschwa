// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine http;
  alphtype unsigned char;

  include "http.rl";

  main := http_message ;
}%%

#include <schwa/formats/http.h>
#include <iomanip>
#include <iostream>

namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
HTTPParser::_run(const uint8_t *const input, const size_t nbytes) {
  (void)http_en_main;  // Shoosh compiler warning about unused variable.
  (void)http_en_consume_bad_header_field;
  (void)http_en_main_http_message_header_field_recover;
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  int cs = 0;

  %% write init;
  %% write exec;

  return cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa
