// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine html;

  include "html.rl";

  main := |*
    comment => { _debug("comment", ts, te); } ;
    cdata => { _debug("cdata", ts, te); } ;
    doctype => { _debug("doctype", ts, te); } ;

    tag => { _debug("tag", ts, te); } ;
    text+ => { _debug("text", ts, te); } ;
  *|;

}%%

#include <schwa/formats/html.h>


namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
HTMLLexer::_run(const uint8_t *const input, const size_t nbytes) {
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  int cs = 0, act = 0;
  const uint8_t *te = nullptr, *ts = nullptr;
  (void)html_en_main;  // Shoosh compiler warning about unused variable.
  std::cerr << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

  %% write init;
  %% write exec;

  std::cerr << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
  std::exit(0);
  return cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa
