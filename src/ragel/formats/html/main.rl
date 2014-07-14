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

#include <cstring>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <schwa/formats/html.h>


namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
HTMLLexer::_run(std::istream &input, const size_t buffer_size) {
  int cs = 0, act = 0;
  uint8_t *te = nullptr, *ts = nullptr;
  (void)html_en_main;  // Shoosh compiler warning about unused variable.

  %% write init;

  std::unique_ptr<uint8_t[]> scoped_buffer(new uint8_t[buffer_size]);
  uint8_t *buffer = scoped_buffer.get();

  bool done = false;
  size_t have = 0;
  while (!done) {
    const size_t space = buffer_size - have;
    if (space == 0) {
      std::ostringstream msg;
      msg << "current token (e.g. a HTML script element) is larger than buffer size of " << buffer_size;
      throw std::runtime_error(msg.str());
    }

    uint8_t *p = buffer + have;
    input.read(reinterpret_cast<char *>(p), space);
    size_t nread = input.gcount();
    uint8_t *pe = p + nread;

    uint8_t *eof = nullptr;
    if (nread < space) {
      eof = pe;
      done = true;
    }

    %% write exec;

    if (cs == %%{ write error; }%%)
      return false;

    if (ts == nullptr)
      have = 0;
    else {
      have = pe - ts;
      std::memcpy(buffer, ts, have);
      te = buffer + (te - ts);
      ts = buffer;
    }
  }

  return true;
}


bool
HTMLLexer::_run(const uint8_t *const input, const size_t nbytes) {
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  int cs = 0, act = 0;
  const uint8_t *te = nullptr, *ts = nullptr;

  %% write init;
  %% write exec;

  return cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa
