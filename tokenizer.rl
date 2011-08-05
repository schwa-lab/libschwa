/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  access s.;

  include "actions.rl";
  include "rules/unicode.rl";
  include "rules/default.rl";
  include "rules/quotes.rl";
  include "rules/punctuation.rl";
  include "rules/contractions.rl";
  include "rules/abbreviations.rl";
  include "rules/numbers.rl";
  include "rules/units.rl";
  include "rules/web.rl";
  include "rules/html.rl";
  include "rules/main.rl";

}%%

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/scoped_array.hpp>

#include "tokens.h"
#include "stream.h"
#include "tokenizer.h"
#include "state.h"

using namespace std;
using namespace boost;

namespace schwa { namespace tokens {

%% write data nofinal;

void
Tokenizer::token_(Type type, Stream &dest, State &state, const char *norm) const {
  state.ensure_sentence(dest);
  state.add(type, dest, norm);
}

void
Tokenizer::word_(Type type, Stream &dest, State &state, const char *norm) const {
  if(state.seen_terminator){
    // need to make this work better for UTF8
    if(type == WORD && (isupper(*state.ts) || isdigit(*state.ts)))
      state.flush_sentence(dest);
    else
      state.seen_terminator = false;
  }
  token_(type, dest, state, norm);
}

void
Tokenizer::punct_(Type type, Stream &dest, State &state, const char *norm) const {
  state.flush_sentence(dest);
  token_(type, dest, state, norm);
}

void
Tokenizer::end_punct_(Type type, Stream &dest, State &state, const char *norm) const {
  token_(type, dest, state, norm);
}

void
Tokenizer::split_(Type type1, Type type2, Stream &dest, State &state,
                  const char *norm1, const char *norm2) const {
  state.flush_sentence(dest);
  state.ensure_sentence(dest);
  state.split(type1, type2, dest, norm1, norm2);
}

void
Tokenizer::terminator_(Stream &dest, State &state, const char *norm) const {
  state.seen_terminator = true;
  end_punct_(TERMINATOR, dest, state, norm);
}

void
Tokenizer::single_quote_(Stream &dest, State &state) const {
  if(state.in_single_quotes){
    end_punct_(QUOTE, dest, state, "'");
    state.in_single_quotes = false;
    return;
  }

  punct_(QUOTE, dest, state, "`");
  state.in_single_quotes = true;
}

void
Tokenizer::double_quote_(Stream &dest, State &state) const {
  if(state.in_double_quotes){
    end_punct_(QUOTE, dest, state, "''");
    state.in_double_quotes = false;
    return;
  }

  punct_(QUOTE, dest, state, "``");
  state.in_double_quotes = true;
}

void
Tokenizer::open_single_quote_(Stream &dest, State &state) const {
  punct_(QUOTE, dest, state, "`");
  state.in_single_quotes = true;
}

void
Tokenizer::close_single_quote_(Stream &dest, State &state) const {
  end_punct_(QUOTE, dest, state, "'");
  state.in_single_quotes = false;
}

void
Tokenizer::open_double_quote_(Stream &dest, State &state) const {
  punct_(QUOTE, dest, state, "``");
  state.in_double_quotes = true;
}

void
Tokenizer::close_double_quote_(Stream &dest, State &state) const {
  end_punct_(QUOTE, dest, state, "''");
  state.in_double_quotes = false;
}

void
Tokenizer::sep_text_paragraph_(Stream &dest, State &state) const {
  state.end_paragraph(dest);
}

void
Tokenizer::sep_html_paragraph_(Stream &dest, State &state) const {
  state.end_paragraph(dest);
}

void
Tokenizer::begin_html_paragraph_(Stream &dest, State &state) const {
  state.begin_paragraph(dest);
}

void
Tokenizer::end_html_paragraph_(Stream &dest, State &state) const {
  state.end_paragraph(dest);
}

void
Tokenizer::begin_html_list_(Stream &dest, State &state) const {
  state.begin_list(dest);
}


void
Tokenizer::end_html_list_(Stream &dest, State &state) const {
  state.end_list(dest);
}

void
Tokenizer::begin_html_item_(Stream &dest, State &state) const {
  state.begin_item(dest);
}

void
Tokenizer::end_html_item_(Stream &dest, State &state) const {
  state.end_item(dest);
}

void
Tokenizer::dash_or_item_(Stream &dest, State &state) const {
  if(state.in_sentence)
    punct_(DASH, dest, state, "--");
  else
    state.begin_item(dest);
}

bool
Tokenizer::tokenize_(Stream &dest, State &s, const char *&n1, const char *&n2,
                     const char *p, const char *pe, const char *eof) const {
  %% write exec;

  if(s.cs == tokenizer_error)
    return false;

  return true;
}

bool
Tokenizer::tokenize(Stream &dest, const char *data, offset_type len) const {
  State s;

  %% write init;

  const char *p = data;
  const char *pe = data + len;
  const char *eof = pe;

  s.begin_document(dest);
  bool result = tokenize_(dest, s, s.n1, s.n2, p, pe, eof);
  s.end_document(dest);
  return result;
}

bool
Tokenizer::tokenize(Stream &dest, std::istream &in, size_t buffer_size) const {
  // code based on Ragel Guide, version 6.7, Figure 6.2
  State s;

  %% write init;

  scoped_array<char> scoped_buffer(new char[buffer_size]);
  char *buffer = scoped_buffer.get();

  s.begin_document(dest);

  bool done = false;
  size_t have = 0;
  while(!done){
    size_t space = buffer_size - have;
    if(space == 0)
      return false;

    char *p = buffer + have;
    in.read(p, space);
    size_t nread = in.gcount();
    char *pe = p + nread;

    char *eof = 0;
    if(nread < space){
      eof = pe;
      done = true;
    }

    if(!tokenize_(dest, s, s.n1, s.n2, p, pe, eof))
      return false;

    if(s.ts == 0)
      have = 0;
    else {
      /* There is a prefix to preserve, shift it over. */
      have = pe - s.ts;
      memmove(buffer, s.ts, have);
      s.te = buffer + (s.te - s.ts);
      s.ts = buffer;
    }
  }

  s.end_document(dest);

  return 0;
}

bool
Tokenizer::tokenize(Stream &dest, const std::string &data) const {
  return tokenize(dest, data.data(), data.size());
}

bool
Tokenizer::tokenize(Stream &dest, const char *filename) const {
  iostreams::mapped_file file(filename);

  return tokenize(dest, file.data(), file.size());
}

} }
