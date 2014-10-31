/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tokenizer.h>

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <schwa/io/istream_source.h>
#include <schwa/io/mmapped_source.h>


namespace schwa {
namespace tokenizer {

void
Tokenizer::_token(Type type, Stream &dest, State &state, const uint8_t *norm) const {
  state.ensure_sentence(dest);
  state.add(type, dest, norm);
}


void
Tokenizer::_word(Type type, Stream &dest, State &state, const uint8_t *norm) const {
  if (state.seen_terminator) {
    // XXX need to make this work better for UTF-8.
    if (type == WORD && (std::isupper(*state.ts) || std::isdigit(*state.ts))) {
      state.flush_sentence(dest);
    }
    else {
      state.seen_terminator = false;
    }
  }
  _token(type, dest, state, norm);
}


void
Tokenizer::_punct(Type type, Stream &dest, State &state, const uint8_t *norm) const {
  state.flush_sentence(dest);
  _token(type, dest, state, norm);
}


void
Tokenizer::_end(Type type, Stream &dest, State &state, const uint8_t *norm) const {
  _token(type, dest, state, norm);
}


void
Tokenizer::_split(Type type1, Type type2, Stream &dest, State &state, const uint8_t *norm1, const uint8_t *norm2) const {
  if (state.seen_terminator) {
    // need to make this work better for UTF-8
    if (type1 == WORD && (std::isupper(*state.ts) || std::isdigit(*state.ts))) {
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
Tokenizer::_terminator(Stream &dest, State &state, const uint8_t *norm) const {
  state.seen_terminator = true;
  _end(TERMINATOR, dest, state, norm);
}

void
Tokenizer::_error(Stream &dest, State &state) const {
  state.error(dest);
}

void
Tokenizer::_single_quote(Stream &dest, State &state, const uint8_t *eof) const {
  if (state.in_single_quotes || state.te == eof || !std::isalnum(*state.te)) {
    _end(QUOTE, dest, state, reinterpret_cast<const uint8_t *>(u8"'"));
    state.in_single_quotes = false;
    return;
  }

  _punct(QUOTE, dest, state, reinterpret_cast<const uint8_t *>(u8"`"));
  state.in_single_quotes = true;
}

void
Tokenizer::_double_quote(Stream &dest, State &state, const uint8_t *) const {
  if (state.in_double_quotes) {
    _end(QUOTE, dest, state, reinterpret_cast<const uint8_t *>(u8"''"));
    state.in_double_quotes = false;
    return;
  }

  _punct(QUOTE, dest, state, reinterpret_cast<const uint8_t *>(u8"``"));
  state.in_double_quotes = true;
}

void
Tokenizer::_open_single_quote(Stream &dest, State &state) const {
  _punct(QUOTE, dest, state, reinterpret_cast<const uint8_t *>(u8"`"));
  state.in_single_quotes = true;
}

void
Tokenizer::_close_single_quote(Stream &dest, State &state) const {
  _end(QUOTE, dest, state, reinterpret_cast<const uint8_t *>(u8"'"));
  state.in_single_quotes = false;
}

void
Tokenizer::_open_double_quote(Stream &dest, State &state) const {
  _punct(QUOTE, dest, state, reinterpret_cast<const uint8_t *>(u8"``"));
  state.in_double_quotes = true;
}

void
Tokenizer::_close_double_quote(Stream &dest, State &state) const {
  _end(QUOTE, dest, state, reinterpret_cast<const uint8_t *>(u8"''"));
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
  const long val = std::strtol(reinterpret_cast<const char *>(state.ts) + 2 /* <h */, nullptr, 10);
  state.begin_heading(dest, static_cast<int>(val));
}

void
Tokenizer::_end_html_heading(Stream &dest, State &state) const {
  const long val = std::strtol(reinterpret_cast<const char *>(state.ts) + 3 /* </h */, nullptr, 10);
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
    _punct(DASH, dest, state, reinterpret_cast<const uint8_t *>(u8"--"));
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
Tokenizer::tokenize(Stream &dest, const uint8_t *data, OnError onerror) const {
  return tokenize(dest, data, std::strlen(reinterpret_cast<const char *>(data)), onerror);
}

bool
Tokenizer::tokenize(Stream &dest, const std::string &data, OnError onerror) const {
  return tokenize(dest, reinterpret_cast<const uint8_t *>(data.data()), data.size(), onerror);
}

bool
Tokenizer::tokenize_stream(Stream &dest, std::istream &in, size_t buffer_size, OnError onerror) const {
  io::IStreamSource src(in);
  return tokenize(dest, src, buffer_size, onerror);
}

bool
Tokenizer::tokenize_mmap(Stream &dest, const std::string &filename, OnError onerror) const {
  io::MMappedSource src(filename.c_str());
  return tokenize(dest, reinterpret_cast<const uint8_t *>(src.data()), src.size(), onerror);
}



bool
Tokenizer::_tokenize(Stream &, State &, const uint8_t *&, const uint8_t *&, const uint8_t *, const uint8_t *, const uint8_t *, OnError) const {
  return false;
}

bool
Tokenizer::tokenize(Stream &, const uint8_t *, const size_t, OnError) const {
  return false;
}

bool
Tokenizer::tokenize(Stream &, io::Source &, const size_t, const OnError) const {
  return false;
}

}  // namespace tokenizer
}  // namespace schwa
