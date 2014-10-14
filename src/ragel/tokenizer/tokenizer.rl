// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  access s.;
  alphtype unsigned char;

  # All of these rules assume the input byte stream is UTF-8.
  include 'actions.rl';

  include 'rules/unicode.rl';
  include 'rules/numbers.rl';
  include 'rules/punctuation.rl';
  include 'rules/quotes.rl';
  include 'rules/default.rl';

  include 'rules/contractions.rl';
  include 'rules/abbreviations.rl';
  include 'rules/date_time.rl';
  include 'rules/units.rl';
  include 'rules/web.rl';
  include 'rules/emoticons.rl';
  include 'rules/main.rl';
}%%


#include <cstring>
#include <memory>
#include <sstream>

#include <schwa/io/source.h>
#include <schwa/tokenizer.h>


namespace schwa {
namespace tokenizer {

%% write data noerror nofinal;

bool
Tokenizer::_tokenize(Stream &dest, State &s, const uint8_t *&n1, const uint8_t *&n2, const uint8_t *p, const uint8_t *pe, const uint8_t *eof, OnError onerror) const {
  static_cast<void>(eof);
  (void)onerror;

  %% write exec;

  if (s.cs == %%{ write error; }%%)
    return false;
  return true;
}


bool
Tokenizer::tokenize(Stream &dest, const uint8_t *data, const size_t len, OnError onerror) const {
  State s;

  %% write init;

  const uint8_t *p = data;
  const uint8_t *pe = data + len;
  const uint8_t *eof = pe;

  s.offset = p;
  s.begin_document(dest);
  bool finished = _tokenize(dest, s, s.n1, s.n2, p, pe, eof, onerror);
  if (finished)
    s.end_document(dest);
  return finished;
}


bool
Tokenizer::tokenize(Stream &dest, io::Source &src, const size_t buffer_size, const OnError onerror) const {
  // Code based on Ragel Guide, version 6.7, Figure 6.2.
  State s;
  (void)tokenizer_en_main;  // Shoosh compiler warning about unused variable.

  %% write init;

  std::unique_ptr<uint8_t[]> scoped_buffer(new uint8_t[buffer_size]);
  uint8_t *buffer = scoped_buffer.get();
  s.offset = buffer;
  s.begin_document(dest);

  bool done = false;
  size_t have = 0;
  while (!done) {
    const size_t space = buffer_size - have;
    if (space == 0) {
      std::ostringstream msg;
      msg << "current token (e.g. a HTML script element) is larger than buffer size of " << buffer_size;
      throw TokenError(msg.str());
    }

    uint8_t *p = buffer + have;
    size_t nread = src.read(reinterpret_cast<char *>(p), space);
    uint8_t *pe = p + nread;

    uint8_t *eof = nullptr;
    if (nread < space) {
      eof = pe;
      done = true;
    }

    if (!_tokenize(dest, s, s.n1, s.n2, p, pe, eof, onerror))
      return false;

    if (s.ts == nullptr)
      have = 0;
    else {
      have = pe - s.ts;
      std::memcpy(buffer, s.ts, have);
      s.te = buffer + (s.te - s.ts);
      s.ts = buffer;
    }

    s.offset -= (buffer_size - have);
  }

  s.end_document(dest);

  return true;
}

}  // namespace tokenizer
}  // namespace schwa
