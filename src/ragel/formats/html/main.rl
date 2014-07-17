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
  (void)html_en_main;  // Shoosh compiler warning about unused variable.
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  const uint8_t *te = nullptr, *ts = nullptr;
  int cs = 0, act = 0;

  _begin_document();

  %% write init;
  %% write exec;

  _end_document();
  return cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa