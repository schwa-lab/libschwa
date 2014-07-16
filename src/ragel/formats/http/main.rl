// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine http;
  alphtype unsigned char;

  include "http.rl";

  main := http_message ;
}%%

#include <schwa/formats/http.h>
#include <iostream>

namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
HTTPLexer::_run(const uint8_t *const input, const size_t nbytes) {
  (void)http_en_main;  // Shoosh compiler warning about unused variable.
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  int cs = 0;

  %% write init;
  %% write exec;

  std::cerr << "<<<<<" << std::endl;
  std::cerr.write(reinterpret_cast<const char *>(p), pe - p);
  std::cerr << ">>>>>" << std::endl;

  return cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa
