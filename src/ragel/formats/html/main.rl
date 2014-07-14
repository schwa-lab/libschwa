// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine html;

  include "html.rl";

  main := |*
    comment => { debug("comment", ts, te); } ;
    cdata => { debug("cdata", ts, te); } ;
    doctype => { debug("doctype", ts, te); } ;

    tag => { debug("tag", ts, te); } ;

    text+ => { debug("text", ts, te); } ;
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

void
HTMLLexer::debug(const char *const rule, const uint8_t *const ts, const uint8_t *const te) {
  if (_debug) {
    std::cout << rule << " " << (te - ts) << ": '";
    std::cout.write(reinterpret_cast<const char *>(ts), te - ts);
    std::cout << "'" << std::endl;
  }
}

bool
HTMLLexer::run(std::istream &input, const size_t buffer_size) {
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

}  // namespace formats
}  // namespace schwa
