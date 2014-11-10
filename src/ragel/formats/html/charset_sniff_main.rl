// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine html_charset_sniff;
  include "charset_sniff.rl";

  main := |*
    comment ;
    cdata ;
    meta_tag ;
    any ;
  *|;
}%%

#include <schwa/formats/html.h>


namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
HTMLCharsetSniffer::_run_charset_sniff(const uint8_t *const input, const size_t nbytes) {
  (void)html_charset_sniff_en_main;  // Shoosh compiler warning about unused variable.
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  const uint8_t *te = nullptr, *ts = nullptr;
  int cs = 0, act = 0;
  (void)te;
  (void)ts;
  (void)act;

  %% write init;
  %% write exec;

  return cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa
