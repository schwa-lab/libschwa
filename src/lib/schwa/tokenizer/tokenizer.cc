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
Tokenizer::_token(Type type, Stream &dest, State &state, const char *norm) const {
  state.ensure_sentence(dest);
  state.add(type, dest, norm);
}


void
Tokenizer::_word(Type type, Stream &dest, State &state, const char *norm) const {
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
  if (state.in_single_quotes || state.te == eof || !std::isalnum(*state.te)) {
    _end(QUOTE, dest, state, "'");
    state.in_single_quotes = false;
    return;
  }

  _punct(QUOTE, dest, state, "`");
  state.in_single_quotes = true;
}

void
Tokenizer::_double_quote(Stream &dest, State &state, const char *) const {
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
  const long val = std::strtol(state.ts + 2 /* <h */, nullptr, 10);
  state.begin_heading(dest, static_cast<int>(val));
}

void
Tokenizer::_end_html_heading(Stream &dest, State &state) const {
  const long val = std::strtol(state.ts + 3 /* </h */, nullptr, 10);
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
Tokenizer::tokenize(Stream &dest, const char *data, OnError onerror) const {
  return tokenize(dest, data, std::strlen(data), onerror);
}

bool
Tokenizer::tokenize(Stream &dest, const std::string &data, OnError onerror) const {
  return tokenize(dest, data.data(), data.size(), onerror);
}

bool
Tokenizer::tokenize_stream(Stream &dest, std::istream &in, size_t buffer_size, OnError onerror) const {
  io::IStreamSource src(in);
  return tokenize(dest, src, buffer_size, onerror);
}

bool
Tokenizer::tokenize_mmap(Stream &dest, const std::string &filename, OnError onerror) const {
  io::MMappedSource src(filename.c_str());
  return tokenize(dest, src.data(), src.size(), onerror);
}

}  // namespace tokenizer
}  // namespace schwa
