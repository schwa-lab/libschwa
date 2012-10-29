// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  access s.;
  action FAIRFAX { true }

  include "actions.rl";
  include "rules/unicode.rl";
  include "rules/quotes.rl";
  include "rules/punctuation.rl";
  include "rules/contractions.rl";
  include "rules/abbreviations.rl";
  include "rules/numbers.rl";
  include "rules/date_time.rl";
  include "rules/units.rl";
  include "rules/web.rl";
  include "rules/html.rl";
  include "rules/emoticons.rl";
  include "rules/default.rl";
  include "rules/main.rl";
}%%

#include <cctype>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <schwa/_base.h>
#include <schwa/io/istream_source.h>
#include <schwa/io/mmapped_source.h>
#include <schwa/io/source.h>
#include <schwa/tokenizer.h>

namespace schwa {
namespace tokenizer {

%% write data nofinal;
void
Tokenizer::_token(Type type, Stream &dest, State &state, const char *norm) const {
  state.ensure_sentence(dest);
  state.add(type, dest, norm);
}

void
Tokenizer::_word(Type type, Stream &dest, State &state, const char *norm) const {
  if (state.seen_terminator) {
    // need to make this work better for UTF8
    if (type == WORD && (isupper(*state.ts) || isdigit(*state.ts))) {
      state.flush_sentence(dest);
    }
    else {
      state.seen_terminator = false;
    }
  }
  _token(type, dest, state, norm);
}

void
Tokenizer::_punct(Type type, Stream &dest, State &state, const char *norm) const {
  state.flush_sentence(dest);
  _token(type, dest, state, norm);
}

void
Tokenizer::_end(Type type, Stream &dest, State &state, const char *norm) const {
  _token(type, dest, state, norm);
}

void
Tokenizer::_split(Type type1, Type type2, Stream &dest, State &state, const char *norm1, const char *norm2) const {
  if (state.seen_terminator) {
    // need to make this work better for UTF8
    if (type1 == WORD && (isupper(*state.ts) || isdigit(*state.ts))) {
      state.flush_sentence(dest);
    }
    else {
      state.seen_terminator = false;
    }
  }
  state.ensure_sentence(dest);
  state.split(type1, type2, dest, norm1, norm2);
}

void
Tokenizer::_terminator(Stream &dest, State &state, const char *norm) const {
  state.seen_terminator = true;
  _end(TERMINATOR, dest, state, norm);
}

void
Tokenizer::_error(Stream &dest, State &state) const {
  state.error(dest);
}

void
Tokenizer::_single_quote(Stream &dest, State &state, const char *eof) const {
  if (state.in_single_quotes || state.te == eof || !isalnum(*state.te)) {
    _end(QUOTE, dest, state, "'");
    state.in_single_quotes = false;
    return;
  }

  _punct(QUOTE, dest, state, "`");
  state.in_single_quotes = true;
}

void
Tokenizer::_double_quote(Stream &dest, State &state, const char *eof) const {
  static_cast<void>(eof);
  if (state.in_double_quotes) {
    _end(QUOTE, dest, state, "''");
    state.in_double_quotes = false;
    return;
  }

  _punct(QUOTE, dest, state, "``");
  state.in_double_quotes = true;
}

void
Tokenizer::_open_single_quote(Stream &dest, State &state) const {
  _punct(QUOTE, dest, state, "`");
  state.in_single_quotes = true;
}

void
Tokenizer::_close_single_quote(Stream &dest, State &state) const {
  _end(QUOTE, dest, state, "'");
  state.in_single_quotes = false;
}

void
Tokenizer::_open_double_quote(Stream &dest, State &state) const {
  _punct(QUOTE, dest, state, "``");
  state.in_double_quotes = true;
}

void
Tokenizer::_close_double_quote(Stream &dest, State &state) const {
  _end(QUOTE, dest, state, "''");
  state.in_double_quotes = false;
}

void
Tokenizer::_sep_text_paragraph(Stream &dest, State &state) const {
  state.end_paragraph(dest);
}

void
Tokenizer::_sep_html_paragraph(Stream &dest, State &state) const {
  state.end_paragraph(dest);
}

void
Tokenizer::_begin_html_paragraph(Stream &dest, State &state) const {
  state.begin_paragraph(dest);
}

void
Tokenizer::_end_html_paragraph(Stream &dest, State &state) const {
  state.end_paragraph(dest);
}

void
Tokenizer::_begin_html_heading(Stream &dest, State &state) const {
  const long val = strtol(state.ts + 2 /* <h */, NULL, 10);
  state.begin_heading(dest, static_cast<int>(val));
}

void
Tokenizer::_end_html_heading(Stream &dest, State &state) const {
  const long val = strtol(state.ts + 3 /* </h */, NULL, 10);
  state.end_heading(dest, static_cast<int>(val));
}

void
Tokenizer::_begin_html_list(Stream &dest, State &state) const {
  state.begin_list(dest);
}

void
Tokenizer::_end_html_list(Stream &dest, State &state) const {
  state.end_list(dest);
}

void
Tokenizer::_begin_html_item(Stream &dest, State &state) const {
  state.begin_item(dest);
}

void
Tokenizer::_end_html_item(Stream &dest, State &state) const {
  state.end_item(dest);
}

void
Tokenizer::_dash_or_item(Stream &dest, State &state) const {
  if (state.in_sentence)
    _punct(DASH, dest, state, "--");
  else
    state.begin_item(dest);
}

void
Tokenizer::_number_or_item(Stream &dest, State &state) const {
  if (state.in_sentence) {
    _split(NUMBER, PUNCTUATION, dest, state);
    state.seen_terminator = true;
  }
  else
    state.begin_item(dest);
}

bool
Tokenizer::_die(std::ostream &msg) const {
  throw TokenError(dynamic_cast<std::ostringstream &>(msg).str());
  return false;
}

bool
Tokenizer::_tokenize(Stream &dest, State &s, const char *&n1, const char *&n2, const char *p, const char *pe, const char *eof, int errors) const {
  static_cast<void>(eof);
  std::ostringstream msg;

  %% write exec;

  if (s.cs == tokenizer_error)
    return false;

  return true;
}

bool
Tokenizer::tokenize(Stream &dest, const char *data, offset_type len, int errors) const {
  State s;

  %% write init;

  const char *p = data;
  const char *pe = data + len;
  const char *eof = pe;

  s.offset = p;
  s.begin_document(dest);
  bool finished = _tokenize(dest, s, s.n1, s.n2, p, pe, eof, errors);
  if (finished)
    s.end_document(dest);
  return finished;
}

bool
Tokenizer::tokenize(Stream &dest, io::Source &src, size_t buffer_size, int errors) const {
  std::ostringstream msg;
  // code based on Ragel Guide, version 6.7, Figure 6.2
  State s;

  %% write init;

  std::unique_ptr<char[]> scoped_buffer(new char[buffer_size]);
  char *buffer = scoped_buffer.get();
  if (!buffer)
    return _die(msg << "could not allocate a buffer of size " << buffer_size);

  s.offset = buffer;
  s.begin_document(dest);

  bool done = false;
  size_t have = 0;
  while (!done) {
    size_t space = buffer_size - have;
    if (space == 0)
      return _die(msg << "current token (e.g. a HTML script element) is larger than buffer size of " << buffer_size);

    char *p = buffer + have;
    size_t nread = src.read(p, space);
    char *pe = p + nread;

    char *eof = 0;
    if (nread < space) {
      eof = pe;
      done = true;
    }

    if (!_tokenize(dest, s, s.n1, s.n2, p, pe, eof, errors))
      return false;

    if (s.ts == 0)
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

  return 0;
}

bool
Tokenizer::tokenize(Stream &dest, const char *data, int errors) const {
  return tokenize(dest, data, std::strlen(data), errors);
}

bool
Tokenizer::tokenize(Stream &dest, const std::string &data, int errors) const {
  return tokenize(dest, data.data(), data.size(), errors);
}

bool
Tokenizer::tokenize_stream(Stream &dest, std::istream &in, size_t buffer_size, int errors) const {
  io::IStreamSource src(in);
  return tokenize(dest, src, buffer_size, errors);
}

bool
Tokenizer::tokenize_mmap(Stream &dest, const std::string &filename, int errors) const {
  io::MMappedSource src(filename.c_str());
  return tokenize(dest, src.data(), src.size(), errors);
}

}  // namespace tokenizer
}  // namespace schwa
