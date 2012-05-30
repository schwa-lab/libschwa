
#line 1 "src/ragel/tokenizer.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 24 "src/ragel/tokenizer.rl"


#include <schwa/base.h>
#include <schwa/io/source.h>
#include <schwa/io/sources/istream.h>
#include <schwa/tokenizer.h>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/scoped_array.hpp>

using namespace boost;

namespace schwa { namespace tokenizer {


#line 24 "src/lib/schwa/tokenizer/tokenizer.cc"
static const int tokenizer_start = 764;
static const int tokenizer_error = 0;

static const int tokenizer_en_main = 764;


#line 39 "src/ragel/tokenizer.rl"

void
Tokenizer::_token(Type type, Stream &dest, State &state, const char *norm) const {
  state.ensure_sentence(dest);
  state.add(type, dest, norm);
}

void
Tokenizer::_word(Type type, Stream &dest, State &state, const char *norm) const {
  if (state.seen_terminator) {
    // need to make this work better for UTF8
    if (type == WORD && (isupper(*state.ts) || isdigit(*state.ts)))
      state.flush_sentence(dest);
    else
      state.seen_terminator = false;
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
    if (type1 == WORD && (isupper(*state.ts) || isdigit(*state.ts)))
      state.flush_sentence(dest);
    else
      state.seen_terminator = false;
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
  if (state.in_sentence){
    _split(NUMBER, PUNCTUATION, dest, state);
    state.seen_terminator = true;
  }else
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

  
#line 216 "src/lib/schwa/tokenizer/tokenizer.cc"
	{
	short _widec;
	if ( p == pe )
		goto _test_eof;
	switch (  s.cs )
	{
tr0:
#line 1 "NONE"
	{	switch(  s.act ) {
	case 3:
	{{p = (( s.te))-1;} _open_single_quote(dest, s); }
	break;
	case 4:
	{{p = (( s.te))-1;} _close_single_quote(dest, s); }
	break;
	case 5:
	{{p = (( s.te))-1;} _open_double_quote(dest, s); }
	break;
	case 6:
	{{p = (( s.te))-1;} _close_double_quote(dest, s); }
	break;
	case 7:
	{{p = (( s.te))-1;} _terminator(dest, s, "."); }
	break;
	case 10:
	{{p = (( s.te))-1;} _terminator(dest, s, "..."); }
	break;
	case 11:
	{{p = (( s.te))-1;} _dash_or_item(dest, s); }
	break;
	case 12:
	{{p = (( s.te))-1;}}
	break;
	case 13:
	{{p = (( s.te))-1;} _sep_text_paragraph(dest, s); }
	break;
	case 24:
	{{p = (( s.te))-1;} _split(WORD, CONTRACTION, dest, s); }
	break;
	case 25:
	{{p = (( s.te))-1;} _split(WORD, CONTRACTION, dest, s); }
	break;
	case 26:
	{{p = (( s.te))-1;} _split(WORD, CONTRACTION, dest, s); }
	break;
	case 27:
	{{p = (( s.te))-1;} _split(WORD, POSSESSIVE, dest, s); }
	break;
	case 29:
	{{p = (( s.te))-1;} _split(NUMBER, UNIT, dest, s); }
	break;
	case 30:
	{{p = (( s.te))-1;} _split(NUMBER, UNIT, dest, s); }
	break;
	case 31:
	{{p = (( s.te))-1;} _word(WORD, dest, s); }
	break;
	case 32:
	{{p = (( s.te))-1;} _word(WORD, dest, s); }
	break;
	case 34:
	{{p = (( s.te))-1;} _word(WORD, dest, s); }
	break;
	case 35:
	{{p = (( s.te))-1;} _punct(PUNCTUATION, dest, s); }
	break;
	case 37:
	{{p = (( s.te))-1;} _word(WORD, dest, s); }
	break;
	case 38:
	{{p = (( s.te))-1;} _word(WORD, dest, s); }
	break;
	case 39:
	{{p = (( s.te))-1;} _word(WORD, dest, s); }
	break;
	case 40:
	{{p = (( s.te))-1;} _word(WORD, dest, s); }
	break;
	case 41:
	{{p = (( s.te))-1;}
    switch(errors){
      case ERROR_SKIP:
        break;
      case ERROR_CALL:
        _error(dest, s);
        break;
      case ERROR_THROW:
        _die(msg << "stuck on character " << (int)*p << " at offset " << (p - s.offset));
        break;
      default:
        _die(msg << "illegal value " << errors << " for bad byte error handling");
    }
  }
	break;
	}
	}
	goto st764;
tr10:
#line 8 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _word(WORD, dest, s); }}
	goto st764;
tr18:
#line 7 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}}
	goto st764;
tr25:
#line 8 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _word(WORD, dest, s); }}
	goto st764;
tr26:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr28:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr30:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 70 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr39:
#line 11 "src/ragel/actions.rl"
	{ s.suffix += 6; }
#line 70 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr51:
#line 70 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr67:
#line 10 "src/ragel/actions.rl"
	{ s.suffix += 5; }
#line 70 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr74:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 70 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr82:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 70 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr91:
#line 12 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{
    switch(errors){
      case ERROR_SKIP:
        break;
      case ERROR_CALL:
        _error(dest, s);
        break;
      case ERROR_THROW:
        _die(msg << "stuck on character " << (int)*p << " at offset " << (p - s.offset));
        break;
      default:
        _die(msg << "illegal value " << errors << " for bad byte error handling");
    }
  }}
	goto st764;
tr94:
#line 28 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _single_quote(dest, s, eof); }}
	goto st764;
tr96:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(WORD, dest, s); }}
	goto st764;
tr98:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(WORD, dest, s); }}
	goto st764;
tr123:
#line 8 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _word(WORD, dest, s); }}
	goto st764;
tr198:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 67 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, POSSESSIVE, dest, s); }}
	goto st764;
tr199:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 12 "src/ragel/rules/contractions.rl"
	{n2="'D";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr201:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 13 "src/ragel/rules/contractions.rl"
	{n2="'M";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr204:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 9 "src/ragel/rules/contractions.rl"
	{n2="'d";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr206:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 10 "src/ragel/rules/contractions.rl"
	{n2="'m";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr209:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 19 "src/ragel/rules/contractions.rl"
	{n2="'LL";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr210:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 20 "src/ragel/rules/contractions.rl"
	{n2="'RE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr211:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 21 "src/ragel/rules/contractions.rl"
	{n2="'VE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr212:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 15 "src/ragel/rules/contractions.rl"
	{n2="'ll";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr213:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 16 "src/ragel/rules/contractions.rl"
	{n2="'re";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr214:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 17 "src/ragel/rules/contractions.rl"
	{n2="'ve";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr217:
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 12 "src/ragel/rules/contractions.rl"
	{n2="'D";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr219:
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 13 "src/ragel/rules/contractions.rl"
	{n2="'M";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr221:
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 67 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, POSSESSIVE, dest, s); }}
	goto st764;
tr223:
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 9 "src/ragel/rules/contractions.rl"
	{n2="'d";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr225:
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 10 "src/ragel/rules/contractions.rl"
	{n2="'m";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr228:
#line 10 "src/ragel/actions.rl"
	{ s.suffix += 5; }
#line 19 "src/ragel/rules/contractions.rl"
	{n2="'LL";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr229:
#line 10 "src/ragel/actions.rl"
	{ s.suffix += 5; }
#line 20 "src/ragel/rules/contractions.rl"
	{n2="'RE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr230:
#line 10 "src/ragel/actions.rl"
	{ s.suffix += 5; }
#line 21 "src/ragel/rules/contractions.rl"
	{n2="'VE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr231:
#line 10 "src/ragel/actions.rl"
	{ s.suffix += 5; }
#line 15 "src/ragel/rules/contractions.rl"
	{n2="'ll";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr232:
#line 10 "src/ragel/actions.rl"
	{ s.suffix += 5; }
#line 16 "src/ragel/rules/contractions.rl"
	{n2="'re";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr233:
#line 10 "src/ragel/actions.rl"
	{ s.suffix += 5; }
#line 17 "src/ragel/rules/contractions.rl"
	{n2="'ve";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr234:
#line 42 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _dash_or_item(dest, s); }}
	goto st764;
tr248:
#line 11 "src/ragel/actions.rl"
	{ s.suffix += 6; }
#line 12 "src/ragel/rules/contractions.rl"
	{n2="'D";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr250:
#line 11 "src/ragel/actions.rl"
	{ s.suffix += 6; }
#line 13 "src/ragel/rules/contractions.rl"
	{n2="'M";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr252:
#line 11 "src/ragel/actions.rl"
	{ s.suffix += 6; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 67 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, POSSESSIVE, dest, s); }}
	goto st764;
tr254:
#line 11 "src/ragel/actions.rl"
	{ s.suffix += 6; }
#line 9 "src/ragel/rules/contractions.rl"
	{n2="'d";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr256:
#line 11 "src/ragel/actions.rl"
	{ s.suffix += 6; }
#line 10 "src/ragel/rules/contractions.rl"
	{n2="'m";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr259:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 19 "src/ragel/rules/contractions.rl"
	{n2="'LL";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr260:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 20 "src/ragel/rules/contractions.rl"
	{n2="'RE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr261:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 21 "src/ragel/rules/contractions.rl"
	{n2="'VE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr262:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 15 "src/ragel/rules/contractions.rl"
	{n2="'ll";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr263:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 16 "src/ragel/rules/contractions.rl"
	{n2="'re";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr264:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 17 "src/ragel/rules/contractions.rl"
	{n2="'ve";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr269:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 12 "src/ragel/rules/contractions.rl"
	{n2="'D";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr271:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 13 "src/ragel/rules/contractions.rl"
	{n2="'M";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr273:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 67 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, POSSESSIVE, dest, s); }}
	goto st764;
tr275:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 9 "src/ragel/rules/contractions.rl"
	{n2="'d";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr277:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 10 "src/ragel/rules/contractions.rl"
	{n2="'m";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr280:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 19 "src/ragel/rules/contractions.rl"
	{n2="'LL";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr281:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 20 "src/ragel/rules/contractions.rl"
	{n2="'RE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr282:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 21 "src/ragel/rules/contractions.rl"
	{n2="'VE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr283:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 15 "src/ragel/rules/contractions.rl"
	{n2="'ll";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr284:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 16 "src/ragel/rules/contractions.rl"
	{n2="'re";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr285:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 17 "src/ragel/rules/contractions.rl"
	{n2="'ve";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr291:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 12 "src/ragel/rules/contractions.rl"
	{n2="'D";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr293:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 13 "src/ragel/rules/contractions.rl"
	{n2="'M";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr295:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 67 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, POSSESSIVE, dest, s); }}
	goto st764;
tr297:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 9 "src/ragel/rules/contractions.rl"
	{n2="'d";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr299:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 10 "src/ragel/rules/contractions.rl"
	{n2="'m";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr302:
#line 15 "src/ragel/actions.rl"
	{ s.suffix += 10; }
#line 19 "src/ragel/rules/contractions.rl"
	{n2="'LL";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr303:
#line 15 "src/ragel/actions.rl"
	{ s.suffix += 10; }
#line 20 "src/ragel/rules/contractions.rl"
	{n2="'RE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr304:
#line 15 "src/ragel/actions.rl"
	{ s.suffix += 10; }
#line 21 "src/ragel/rules/contractions.rl"
	{n2="'VE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr305:
#line 15 "src/ragel/actions.rl"
	{ s.suffix += 10; }
#line 15 "src/ragel/rules/contractions.rl"
	{n2="'ll";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr306:
#line 15 "src/ragel/actions.rl"
	{ s.suffix += 10; }
#line 16 "src/ragel/rules/contractions.rl"
	{n2="'re";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr307:
#line 15 "src/ragel/actions.rl"
	{ s.suffix += 10; }
#line 17 "src/ragel/rules/contractions.rl"
	{n2="'ve";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr318:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 19 "src/ragel/rules/contractions.rl"
	{n2="'LL";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr319:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 20 "src/ragel/rules/contractions.rl"
	{n2="'RE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr320:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 21 "src/ragel/rules/contractions.rl"
	{n2="'VE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr321:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 15 "src/ragel/rules/contractions.rl"
	{n2="'ll";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr322:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 16 "src/ragel/rules/contractions.rl"
	{n2="'re";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr323:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 17 "src/ragel/rules/contractions.rl"
	{n2="'ve";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr358:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 67 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, POSSESSIVE, dest, s); }}
	goto st764;
tr379:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 12 "src/ragel/rules/contractions.rl"
	{n2="'D";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr381:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 13 "src/ragel/rules/contractions.rl"
	{n2="'M";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr384:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 9 "src/ragel/rules/contractions.rl"
	{n2="'d";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr386:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 10 "src/ragel/rules/contractions.rl"
	{n2="'m";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr389:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 19 "src/ragel/rules/contractions.rl"
	{n2="'LL";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr390:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 20 "src/ragel/rules/contractions.rl"
	{n2="'RE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr391:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 21 "src/ragel/rules/contractions.rl"
	{n2="'VE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr392:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 15 "src/ragel/rules/contractions.rl"
	{n2="'ll";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr393:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 16 "src/ragel/rules/contractions.rl"
	{n2="'re";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr394:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 17 "src/ragel/rules/contractions.rl"
	{n2="'ve";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr435:
#line 37 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _terminator(dest, s, "."); }}
	goto st764;
tr438:
#line 40 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _terminator(dest, s, "..."); }}
	goto st764;
tr492:
#line 8 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _word(WORD, dest, s); }}
	goto st764;
tr494:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 58 "src/ragel/rules/date_time.rl"
	{n2="am";}
#line 71 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr496:
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 58 "src/ragel/rules/date_time.rl"
	{n2="am";}
#line 71 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr498:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 59 "src/ragel/rules/date_time.rl"
	{n2="pm";}
#line 71 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr500:
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 59 "src/ragel/rules/date_time.rl"
	{n2="pm";}
#line 71 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr527:
#line 9 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _punct(PUNCTUATION, dest, s); }}
	goto st764;
tr529:
#line 7 "src/ragel/rules/main.rl"
	{ s.te = p+1;}
	goto st764;
tr535:
#line 7 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}}
	goto st764;
tr544:
#line 53 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _end_html_heading(dest, s); }}
	goto st764;
tr546:
#line 59 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _end_html_item(dest, s); }}
	goto st764;
tr548:
#line 56 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _end_html_list(dest, s); }}
	goto st764;
tr549:
#line 49 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _end_html_paragraph(dest, s); }}
	goto st764;
tr579:
#line 52 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _begin_html_heading(dest, s); }}
	goto st764;
tr581:
#line 58 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _begin_html_item(dest, s); }}
	goto st764;
tr583:
#line 55 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _begin_html_list(dest, s); }}
	goto st764;
tr587:
#line 48 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _begin_html_paragraph(dest, s); }}
	goto st764;
tr592:
#line 8 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _word(WORD, dest, s); }}
	goto st764;
tr652:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 34 "src/ragel/rules/contractions.rl"
	{n2="N'T";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr655:
#line 10 "src/ragel/actions.rl"
	{ s.suffix += 5; }
#line 34 "src/ragel/rules/contractions.rl"
	{n2="N'T";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr664:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 34 "src/ragel/rules/contractions.rl"
	{n2="N'T";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr669:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 34 "src/ragel/rules/contractions.rl"
	{n2="N'T";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr675:
#line 15 "src/ragel/actions.rl"
	{ s.suffix += 10; }
#line 34 "src/ragel/rules/contractions.rl"
	{n2="N'T";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr680:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 34 "src/ragel/rules/contractions.rl"
	{n2="N'T";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr685:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 43 "src/ragel/rules/contractions.rl"
	{n2="N'T";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr688:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(WORD, dest, s); }}
	goto st764;
tr698:
#line 15 "src/ragel/actions.rl"
	{ s.suffix += 10; }
#line 33 "src/ragel/rules/contractions.rl"
	{n2="n't";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr699:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 33 "src/ragel/rules/contractions.rl"
	{n2="n't";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr702:
#line 10 "src/ragel/actions.rl"
	{ s.suffix += 5; }
#line 33 "src/ragel/rules/contractions.rl"
	{n2="n't";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr710:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 33 "src/ragel/rules/contractions.rl"
	{n2="n't";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr715:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 33 "src/ragel/rules/contractions.rl"
	{n2="n't";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr720:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 33 "src/ragel/rules/contractions.rl"
	{n2="n't";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr725:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 42 "src/ragel/rules/contractions.rl"
	{n2="n't";}
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr738:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(WORD, dest, s); }}
	goto st764;
tr780:
#line 9 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _punct(PUNCTUATION, dest, s); }}
	goto st764;
tr781:
#line 42 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _dash_or_item(dest, s); }}
	goto st764;
tr782:
#line 31 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _open_single_quote(dest, s); }}
	goto st764;
tr784:
#line 34 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _open_double_quote(dest, s); }}
	goto st764;
tr785:
#line 35 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _close_double_quote(dest, s); }}
	goto st764;
tr787:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(WORD, dest, s); }}
	goto st764;
tr806:
#line 32 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _close_single_quote(dest, s); }}
	goto st764;
tr809:
#line 29 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _double_quote(dest, s, eof); }}
	goto st764;
tr858:
#line 31 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _open_single_quote(dest, s); }}
	goto st764;
tr873:
#line 32 "src/ragel/rules/main.rl"
	{{p = (( s.te))-1;}{ _close_single_quote(dest, s); }}
	goto st764;
tr911:
#line 12 "src/ragel/rules/main.rl"
	{ s.te = p+1;{
    switch(errors){
      case ERROR_SKIP:
        break;
      case ERROR_CALL:
        _error(dest, s);
        break;
      case ERROR_THROW:
        _die(msg << "stuck on character " << (int)*p << " at offset " << (p - s.offset));
        break;
      default:
        _die(msg << "illegal value " << errors << " for bad byte error handling");
    }
  }}
	goto st764;
tr920:
#line 39 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _terminator(dest, s, "!"); }}
	goto st764;
tr924:
#line 10 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _end(PUNCTUATION, dest, s); }}
	goto st764;
tr937:
#line 38 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _terminator(dest, s, "?"); }}
	goto st764;
tr995:
#line 12 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{
    switch(errors){
      case ERROR_SKIP:
        break;
      case ERROR_CALL:
        _error(dest, s);
        break;
      case ERROR_THROW:
        _die(msg << "stuck on character " << (int)*p << " at offset " << (p - s.offset));
        break;
      default:
        _die(msg << "illegal value " << errors << " for bad byte error handling");
    }
  }}
	goto st764;
tr997:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _word(WORD, dest, s); }}
	goto st764;
tr1000:
#line 7 "src/ragel/rules/main.rl"
	{ s.te = p;p--;}
	goto st764;
tr1005:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _word(WORD, dest, s); }}
	goto st764;
tr1036:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _word(WORD, dest, s); }}
	goto st764;
tr1037:
#line 70 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr1156:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _word(WORD, dest, s); }}
	goto st764;
tr1158:
#line 9 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _punct(PUNCTUATION, dest, s); }}
	goto st764;
tr1231:
#line 28 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _single_quote(dest, s, eof); }}
	goto st764;
tr1233:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 68 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(POSSESSIVE, dest, s); }}
	goto st764;
tr1235:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _word(WORD, dest, s); }}
	goto st764;
tr1239:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(WORD, dest, s); }}
	goto st764;
tr1297:
#line 42 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _dash_or_item(dest, s); }}
	goto st764;
tr1313:
#line 11 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _split(WORD, CONTRACTION, dest, s); }}
	goto st764;
tr1405:
#line 37 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _terminator(dest, s, "."); }}
	goto st764;
tr1475:
#line 71 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _split(NUMBER, UNIT, dest, s); }}
	goto st764;
tr1536:
#line 7 "src/ragel/rules/main.rl"
	{ s.te = p;p--;}
	goto st764;
tr1568:
#line 8 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _word(WORD, dest, s); }}
	goto st764;
tr1992:
#line 31 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _open_single_quote(dest, s); }}
	goto st764;
tr2129:
#line 32 "src/ragel/rules/main.rl"
	{ s.te = p;p--;{ _close_single_quote(dest, s); }}
	goto st764;
tr2130:
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 68 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(POSSESSIVE, dest, s); }}
	goto st764;
tr2137:
#line 11 "src/ragel/actions.rl"
	{ s.suffix += 6; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 68 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(POSSESSIVE, dest, s); }}
	goto st764;
tr2138:
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 68 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(POSSESSIVE, dest, s); }}
	goto st764;
tr2139:
#line 14 "src/ragel/actions.rl"
	{ s.suffix += 9; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 68 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(POSSESSIVE, dest, s); }}
	goto st764;
tr2140:
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 68 "src/ragel/rules/main.rl"
	{ s.te = p+1;{ _word(POSSESSIVE, dest, s); }}
	goto st764;
st764:
#line 1 "NONE"
	{ s.ts = 0;}
	if ( ++p == pe )
		goto _test_eof764;
case 764:
#line 1 "NONE"
	{ s.ts = p;}
#line 1411 "src/lib/schwa/tokenizer/tokenizer.cc"
	_widec = (*p);
	if ( (*p) < 19 ) {
		if ( (*p) < -110 ) {
			if ( -111 <= (*p) && (*p) <= -111 ) {
				_widec = (short)(128 + ((*p) - -128));
				if ( 
#line 8 "src/ragel/tokenizer.rl"
 true  ) _widec += 256;
			}
		} else if ( (*p) > -110 ) {
			if ( -30 <= (*p) && (*p) <= -30 ) {
				_widec = (short)(128 + ((*p) - -128));
				if ( 
#line 8 "src/ragel/tokenizer.rl"
 true  ) _widec += 256;
			}
		} else {
			_widec = (short)(128 + ((*p) - -128));
			if ( 
#line 8 "src/ragel/tokenizer.rl"
 true  ) _widec += 256;
		}
	} else if ( (*p) > 19 ) {
		if ( (*p) < 29 ) {
			if ( 28 <= (*p) && (*p) <= 28 ) {
				_widec = (short)(128 + ((*p) - -128));
				if ( 
#line 8 "src/ragel/tokenizer.rl"
 true  ) _widec += 256;
			}
		} else if ( (*p) > 29 ) {
			if ( 38 <= (*p) && (*p) <= 38 ) {
				_widec = (short)(128 + ((*p) - -128));
				if ( 
#line 8 "src/ragel/tokenizer.rl"
 true  ) _widec += 256;
			}
		} else {
			_widec = (short)(128 + ((*p) - -128));
			if ( 
#line 8 "src/ragel/tokenizer.rl"
 true  ) _widec += 256;
		}
	} else {
		_widec = (short)(128 + ((*p) - -128));
		if ( 
#line 8 "src/ragel/tokenizer.rl"
 true  ) _widec += 256;
	}
	switch( _widec ) {
		case -123: goto tr438;
		case -109: goto tr784;
		case -108: goto tr785;
		case -62: goto st765;
		case -17: goto tr916;
		case 10: goto tr918;
		case 13: goto tr919;
		case 32: goto tr19;
		case 33: goto tr920;
		case 34: goto tr809;
		case 35: goto st877;
		case 36: goto tr922;
		case 37: goto tr780;
		case 39: goto tr923;
		case 41: goto tr924;
		case 42: goto tr925;
		case 43: goto tr926;
		case 44: goto tr924;
		case 45: goto tr927;
		case 46: goto tr928;
		case 48: goto tr929;
		case 49: goto tr930;
		case 50: goto tr931;
		case 51: goto tr932;
		case 53: goto tr934;
		case 57: goto tr933;
		case 60: goto tr935;
		case 61: goto tr936;
		case 63: goto tr937;
		case 64: goto st1207;
		case 65: goto tr939;
		case 66: goto tr940;
		case 67: goto tr941;
		case 68: goto tr942;
		case 69: goto tr943;
		case 70: goto tr944;
		case 71: goto tr945;
		case 72: goto tr946;
		case 73: goto tr947;
		case 74: goto tr948;
		case 75: goto tr949;
		case 76: goto tr950;
		case 77: goto tr951;
		case 78: goto tr952;
		case 79: goto tr953;
		case 80: goto tr954;
		case 81: goto tr955;
		case 82: goto tr956;
		case 83: goto tr957;
		case 84: goto tr958;
		case 85: goto tr959;
		case 86: goto tr960;
		case 87: goto tr961;
		case 88: goto tr962;
		case 89: goto tr963;
		case 90: goto tr962;
		case 93: goto tr924;
		case 95: goto tr964;
		case 96: goto st1698;
		case 97: goto tr966;
		case 99: goto tr968;
		case 100: goto tr969;
		case 101: goto tr970;
		case 102: goto tr971;
		case 103: goto tr972;
		case 104: goto tr973;
		case 105: goto tr974;
		case 108: goto tr975;
		case 109: goto tr976;
		case 110: goto tr977;
		case 111: goto tr978;
		case 112: goto tr979;
		case 113: goto tr980;
		case 114: goto tr981;
		case 115: goto tr982;
		case 116: goto tr983;
		case 117: goto tr984;
		case 118: goto tr985;
		case 119: goto tr986;
		case 121: goto tr987;
		case 125: goto tr924;
		case 127: goto tr911;
		case 145: goto st1840;
		case 146: goto tr806;
		case 226: goto tr989;
		case 275: goto tr911;
		case 294: goto tr990;
		case 401: goto tr991;
		case 402: goto tr992;
		case 482: goto tr993;
		case 531: goto tr781;
		case 540: goto tr784;
		case 541: goto tr785;
		case 550: goto tr994;
	}
	if ( _widec < 20 ) {
		if ( _widec < -29 ) {
			if ( _widec < -107 ) {
				if ( _widec <= -112 )
					goto tr911;
			} else if ( _widec > -63 ) {
				if ( _widec > -33 ) {
					if ( -32 <= _widec && _widec <= -31 )
						goto tr915;
				} else if ( _widec >= -61 )
					goto st867;
			} else
				goto tr911;
		} else if ( _widec > -18 ) {
			if ( _widec < -11 ) {
				if ( -16 <= _widec && _widec <= -12 )
					goto tr917;
			} else if ( _widec > 8 ) {
				if ( _widec > 12 ) {
					if ( 14 <= _widec && _widec <= 18 )
						goto tr911;
				} else if ( _widec >= 9 )
					goto tr19;
			} else
				goto tr911;
		} else
			goto tr915;
	} else if ( _widec > 27 ) {
		if ( _widec < 58 ) {
			if ( _widec < 40 ) {
				if ( 30 <= _widec && _widec <= 31 )
					goto tr911;
			} else if ( _widec > 47 ) {
				if ( _widec > 54 ) {
					if ( 55 <= _widec && _widec <= 56 )
						goto tr934;
				} else if ( _widec >= 52 )
					goto tr933;
			} else
				goto tr780;
		} else if ( _widec > 59 ) {
			if ( _widec < 98 ) {
				if ( 62 <= _widec && _widec <= 94 )
					goto tr780;
			} else if ( _widec > 122 ) {
				if ( _widec > 126 ) {
					if ( 284 <= _widec && _widec <= 285 )
						goto tr911;
				} else if ( _widec >= 123 )
					goto tr780;
			} else
				goto tr967;
		} else
			goto tr924;
	} else
		goto tr911;
	goto st0;
st0:
 s.cs = 0;
	goto _out;
st765:
	if ( ++p == pe )
		goto _test_eof765;
case 765:
	switch( (*p) ) {
		case -96: goto tr19;
		case -92: goto tr1;
		case -79: goto tr788;
	}
	if ( (*p) < -94 ) {
		if ( (*p) <= -95 )
			goto tr1;
	} else if ( (*p) > -91 ) {
		if ( -90 <= (*p) && (*p) <= -65 )
			goto tr1;
	} else
		goto tr787;
	goto tr995;
tr1:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st766;
tr1126:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st766;
tr1114:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st766;
tr786:
#line 1 "NONE"
	{ s.te = p+1;}
#line 40 "src/ragel/rules/main.rl"
	{ s.act = 10;}
	goto st766;
tr789:
#line 1 "NONE"
	{ s.te = p+1;}
#line 31 "src/ragel/rules/main.rl"
	{ s.act = 3;}
	goto st766;
tr790:
#line 1 "NONE"
	{ s.te = p+1;}
#line 32 "src/ragel/rules/main.rl"
	{ s.act = 4;}
	goto st766;
tr791:
#line 1 "NONE"
	{ s.te = p+1;}
#line 34 "src/ragel/rules/main.rl"
	{ s.act = 5;}
	goto st766;
tr792:
#line 1 "NONE"
	{ s.te = p+1;}
#line 35 "src/ragel/rules/main.rl"
	{ s.act = 6;}
	goto st766;
st766:
	if ( ++p == pe )
		goto _test_eof766;
case 766:
#line 1691 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto tr996;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr0;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case -95: goto tr1;
		case -92: goto tr1;
	}
	if ( (*p) > -97 ) {
		if ( -90 <= (*p) && (*p) <= -65 )
			goto tr1;
	} else
		goto tr1;
	goto tr0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) <= -65 )
		goto tr1;
	goto tr0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) <= -65 )
		goto st2;
	goto tr0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case -128: goto st5;
		case -126: goto st6;
		case -120: goto st7;
	}
	if ( -127 <= (*p) && (*p) <= -65 )
		goto st2;
	goto tr0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case -102: goto tr1;
		case -98: goto tr1;
	}
	if ( (*p) < -106 ) {
		if ( (*p) <= -119 )
			goto tr1;
	} else if ( (*p) > -105 ) {
		if ( -96 <= (*p) && (*p) <= -65 )
			goto tr1;
	} else
		goto tr1;
	goto tr0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) < -91 ) {
		if ( (*p) <= -94 )
			goto tr1;
	} else if ( (*p) > -85 ) {
		if ( -83 <= (*p) && (*p) <= -65 )
			goto tr1;
	} else
		goto tr1;
	goto tr0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) > -111 ) {
		if ( -109 <= (*p) && (*p) <= -65 )
			goto tr1;
	} else
		goto tr1;
	goto tr0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) <= -65 )
		goto st3;
	goto tr0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 97 )
		goto st10;
	goto tr0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 109 )
		goto st11;
	goto tr0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 112 )
		goto st12;
	goto tr0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 59 )
		goto tr1;
	goto tr0;
tr996:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st767;
st767:
	if ( ++p == pe )
		goto _test_eof767;
case 767:
#line 1841 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto st13;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto st13;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr10;
tr16:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st768;
tr1116:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st768;
tr1108:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st768;
tr1066:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st768;
st768:
	if ( ++p == pe )
		goto _test_eof768;
case 768:
#line 1928 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 97 )
		goto st10;
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr1;
	goto tr0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr1;
	} else if ( (*p) >= 65 )
		goto tr1;
	goto tr0;
tr17:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st769;
tr1103:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st769;
tr1093:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st769;
tr1111:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st769;
tr1137:
#line 1 "NONE"
	{ s.te = p+1;}
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st769;
tr1146:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st769;
st769:
	if ( ++p == pe )
		goto _test_eof769;
case 769:
#line 2024 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr0;
tr19:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/rules/main.rl"
	{ s.act = 12;}
	goto st770;
st770:
	if ( ++p == pe )
		goto _test_eof770;
case 770:
#line 2061 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st16;
		case 9: goto tr19;
		case 32: goto tr19;
		case 38: goto st17;
	}
	if ( 11 <= (*p) && (*p) <= 13 )
		goto tr19;
	goto tr1000;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == -96 )
		goto tr19;
	goto tr18;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 110 )
		goto st18;
	goto tr18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 98 )
		goto st19;
	goto tr0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 115 )
		goto st20;
	goto tr0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 112 )
		goto st21;
	goto tr0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 59 )
		goto tr19;
	goto tr0;
tr788:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st771;
st771:
	if ( ++p == pe )
		goto _test_eof771;
case 771:
#line 2123 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto tr996;
		case 46: goto st22;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1004;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr24;
	goto tr0;
tr24:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st772;
st772:
	if ( ++p == pe )
		goto _test_eof772;
case 772:
#line 2166 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 45: goto st42;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr24;
	goto tr1005;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case -94: goto tr26;
		case -80: goto tr27;
	}
	goto tr25;
tr27:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st24;
tr40:
#line 10 "src/ragel/actions.rl"
	{ s.suffix += 5; }
	goto st24;
tr43:
#line 11 "src/ragel/actions.rl"
	{ s.suffix += 6; }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 2230 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 67: goto tr28;
		case 70: goto tr28;
	}
	goto tr0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == -68 )
		goto st26;
	goto tr25;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 115 )
		goto tr30;
	goto tr25;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 35: goto st28;
		case 99: goto st36;
		case 100: goto st39;
	}
	goto tr25;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 49: goto st29;
		case 88: goto st33;
		case 120: goto st33;
	}
	goto tr25;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 54: goto st30;
		case 55: goto st32;
	}
	goto tr0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 50 )
		goto st31;
	goto tr0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 59 )
		goto tr39;
	goto tr0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 54 )
		goto tr40;
	goto tr0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 65: goto st30;
		case 66: goto st34;
		case 97: goto st30;
		case 98: goto st34;
	}
	goto tr25;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 48 )
		goto st35;
	goto tr0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 59 )
		goto tr43;
	goto tr0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 101 )
		goto st37;
	goto tr0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 110 )
		goto st38;
	goto tr0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 116 )
		goto st31;
	goto tr0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 101 )
		goto st40;
	goto tr0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 103 )
		goto st41;
	goto tr0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 59 )
		goto tr40;
	goto tr0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr48;
	} else if ( (*p) >= 65 )
		goto tr48;
	goto tr0;
tr48:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 32;}
	goto st773;
st773:
	if ( ++p == pe )
		goto _test_eof773;
case 773:
#line 2387 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 45 )
		goto st42;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr48;
	} else if ( (*p) >= 65 )
		goto tr48;
	goto tr1036;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 66: goto tr49;
		case 72: goto st46;
		case 98: goto tr49;
	}
	goto tr25;
tr49:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st774;
tr1041:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st774;
st774:
	if ( ++p == pe )
		goto _test_eof774;
case 774:
#line 2422 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st44;
		case 105: goto st45;
	}
	goto tr1037;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 115 )
		goto tr26;
	goto tr51;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 116 )
		goto tr52;
	goto tr51;
tr52:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st775;
tr1040:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st775;
st775:
	if ( ++p == pe )
		goto _test_eof775;
case 775:
#line 2458 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 47 )
		goto st44;
	goto tr1037;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 122 )
		goto tr30;
	goto tr25;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 122 )
		goto tr26;
	goto tr0;
tr1012:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st776;
st776:
	if ( ++p == pe )
		goto _test_eof776;
case 776:
#line 2488 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 66: goto tr1040;
		case 72: goto st47;
	}
	goto tr1037;
tr1013:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st777;
st777:
	if ( ++p == pe )
		goto _test_eof777;
case 777:
#line 2506 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 66: goto tr1041;
		case 72: goto st47;
		case 98: goto tr1041;
	}
	goto tr1037;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) == 97 )
		goto tr26;
	goto tr0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 80 )
		goto st50;
	goto tr25;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 77 )
		goto tr30;
	goto tr25;
tr1016:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st778;
st778:
	if ( ++p == pe )
		goto _test_eof778;
case 778:
#line 2542 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 66: goto tr1040;
		case 98: goto tr1040;
	}
	goto tr1037;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 112 )
		goto st26;
	goto tr25;
tr1018:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st779;
st779:
	if ( ++p == pe )
		goto _test_eof779;
case 779:
#line 2567 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 99: goto tr28;
		case 109: goto tr1042;
		case 117: goto st53;
		case 119: goto st60;
	}
	goto tr1037;
tr64:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st780;
tr1042:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st780;
st780:
	if ( ++p == pe )
		goto _test_eof780;
case 780:
#line 2591 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 94 )
		goto st52;
	if ( 50 <= (*p) && (*p) <= 51 )
		goto tr28;
	goto tr1037;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( 50 <= (*p) && (*p) <= 51 )
		goto tr26;
	goto tr51;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 32: goto tr54;
		case 46: goto st59;
	}
	goto tr51;
tr84:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st54;
tr54:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
	goto st54;
tr61:
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 2629 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 99: goto st55;
		case 102: goto st56;
		case 105: goto st57;
		case 107: goto st55;
		case 109: goto tr59;
		case 121: goto st58;
	}
	goto tr0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 109 )
		goto tr28;
	goto tr0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 116 )
		goto tr26;
	goto tr0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 110 )
		goto tr26;
	goto tr0;
tr59:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st781;
st781:
	if ( ++p == pe )
		goto _test_eof781;
case 781:
#line 2668 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 105: goto tr28;
		case 109: goto tr28;
	}
	goto tr1037;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 100 )
		goto tr26;
	goto tr0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 32 )
		goto tr61;
	goto tr0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 116 )
		goto tr26;
	goto tr51;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 66: goto tr26;
		case 98: goto tr26;
		case 112: goto st62;
		case 114: goto tr28;
	}
	goto tr25;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 105 )
		goto tr30;
	goto tr25;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 108: goto st64;
		case 116: goto tr64;
	}
	goto tr25;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 32 )
		goto st65;
	goto tr25;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 111 )
		goto st66;
	goto tr0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 122 )
		goto tr67;
	goto tr0;
tr1021:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st782;
st782:
	if ( ++p == pe )
		goto _test_eof782;
case 782:
#line 2753 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 97: goto st67;
		case 98: goto tr1041;
	}
	goto tr1037;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 108 )
		goto tr26;
	goto tr51;
tr1022:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st783;
st783:
	if ( ++p == pe )
		goto _test_eof783;
case 783:
#line 2778 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 80: goto st48;
		case 114: goto tr1047;
	}
	goto tr1037;
tr69:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st784;
tr76:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
	goto st784;
tr1047:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st784;
st784:
	if ( ++p == pe )
		goto _test_eof784;
case 784:
#line 2800 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 115 )
		goto tr28;
	goto tr1037;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 110 )
		goto tr64;
	goto tr25;
tr1024:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st785;
st785:
	if ( ++p == pe )
		goto _test_eof785;
case 785:
#line 2823 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 66: goto tr1040;
		case 72: goto st47;
		case 80: goto st48;
		case 98: goto tr1041;
		case 103: goto tr28;
		case 109: goto tr1048;
	}
	goto tr1037;
tr1048:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st786;
st786:
	if ( ++p == pe )
		goto _test_eof786;
case 786:
#line 2843 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st69;
		case 94: goto st52;
		case 104: goto tr1050;
		case 112: goto st71;
	}
	if ( 50 <= (*p) && (*p) <= 51 )
		goto tr28;
	goto tr1037;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 104 )
		goto tr68;
	goto tr51;
tr68:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st787;
st787:
	if ( ++p == pe )
		goto _test_eof787;
case 787:
#line 2868 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 114 )
		goto tr28;
	goto tr1037;
tr1050:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st788;
st788:
	if ( ++p == pe )
		goto _test_eof788;
case 788:
#line 2882 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 45 )
		goto st70;
	goto tr1037;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 49 )
		goto tr26;
	goto tr51;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( (*p) == 104 )
		goto tr26;
	goto tr51;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 98 )
		goto tr69;
	goto tr25;
tr1026:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st789;
st789:
	if ( ++p == pe )
		goto _test_eof789;
case 789:
#line 2917 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st44;
		case 76: goto tr28;
		case 94: goto st52;
		case 98: goto tr1041;
		case 103: goto tr28;
		case 105: goto tr1053;
		case 109: goto tr1042;
		case 110: goto st79;
		case 112: goto st71;
		case 115: goto tr1050;
		case 116: goto st81;
	}
	if ( 50 <= (*p) && (*p) <= 51 )
		goto tr28;
	goto tr1037;
tr1053:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st790;
st790:
	if ( ++p == pe )
		goto _test_eof790;
case 790:
#line 2944 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st69;
		case 94: goto st52;
		case 99: goto st73;
		case 104: goto tr1050;
		case 108: goto tr1057;
		case 110: goto tr1047;
	}
	if ( 50 <= (*p) && (*p) <= 51 )
		goto tr28;
	goto tr1037;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) == 114 )
		goto st74;
	goto tr51;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	if ( (*p) == 111 )
		goto st75;
	goto tr51;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 115 )
		goto st76;
	goto tr51;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	if ( (*p) == 101 )
		goto st77;
	goto tr51;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( (*p) == 99 )
		goto tr74;
	goto tr51;
tr1057:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st791;
st791:
	if ( ++p == pe )
		goto _test_eof791;
case 791:
#line 3001 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 108 )
		goto st78;
	goto tr1037;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 105 )
		goto st75;
	goto tr51;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 116 )
		goto st80;
	goto tr51;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	if ( (*p) == 104 )
		goto tr76;
	goto tr51;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	if ( (*p) == 104 )
		goto tr69;
	goto tr51;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 97: goto st83;
		case 115: goto tr26;
	}
	goto tr25;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( (*p) == 110 )
		goto st84;
	goto tr25;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 111 )
		goto st85;
	goto tr25;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 115 )
		goto st86;
	goto tr25;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) == 101 )
		goto st87;
	goto tr25;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	if ( (*p) == 99 )
		goto tr82;
	goto tr25;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( (*p) == 122 )
		goto tr26;
	goto tr25;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 115: goto st62;
		case 116: goto tr26;
	}
	goto tr25;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	if ( (*p) == 116 )
		goto tr26;
	goto tr25;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	if ( (*p) == 112 )
		goto st92;
	goto tr25;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	if ( (*p) == 109 )
		goto tr30;
	goto tr25;
tr1032:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st792;
st792:
	if ( ++p == pe )
		goto _test_eof792;
case 792:
#line 3126 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 101: goto st93;
		case 113: goto st94;
	}
	goto tr1037;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) == 99 )
		goto tr69;
	goto tr51;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	switch( (*p) ) {
		case 32: goto tr84;
		case 46: goto st95;
	}
	goto tr51;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( (*p) == 32 )
		goto tr54;
	goto tr0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	if ( (*p) == 111 )
		goto st97;
	goto tr25;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	if ( (*p) == 110 )
		goto tr30;
	goto tr25;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	if ( (*p) == 115 )
		goto tr26;
	goto tr25;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	switch( (*p) ) {
		case 100: goto tr64;
		case 114: goto tr69;
	}
	goto tr25;
tr1004:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st793;
st793:
	if ( ++p == pe )
		goto _test_eof793;
case 793:
#line 3195 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st4;
		case 38: goto st102;
		case 44: goto st103;
		case 45: goto tr1065;
		case 46: goto st22;
		case 47: goto st104;
		case 67: goto tr1066;
		case 70: goto tr1066;
		case 71: goto tr1067;
		case 72: goto tr1068;
		case 75: goto tr1069;
		case 76: goto tr1066;
		case 77: goto tr1070;
		case 80: goto tr1071;
		case 82: goto tr1072;
		case 84: goto tr1073;
		case 98: goto tr1074;
		case 99: goto tr1075;
		case 100: goto tr1076;
		case 102: goto tr1077;
		case 103: goto tr1078;
		case 104: goto tr1079;
		case 105: goto tr1080;
		case 107: goto tr1081;
		case 108: goto tr1082;
		case 109: goto tr1083;
		case 110: goto tr1084;
		case 111: goto tr1085;
		case 112: goto tr1086;
		case 113: goto tr1087;
		case 114: goto tr1088;
		case 115: goto tr1089;
		case 116: goto tr1090;
		case 117: goto tr1091;
		case 121: goto tr1092;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1004;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1005;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	switch( (*p) ) {
		case -95: goto tr1;
		case -94: goto tr26;
		case -92: goto tr1;
		case -80: goto tr87;
	}
	if ( (*p) > -97 ) {
		if ( -90 <= (*p) && (*p) <= -65 )
			goto tr1;
	} else
		goto tr1;
	goto tr0;
tr87:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st794;
st794:
	if ( ++p == pe )
		goto _test_eof794;
case 794:
#line 3281 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto tr996;
		case 67: goto tr1066;
		case 70: goto tr1066;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( (*p) == -68 )
		goto tr88;
	if ( (*p) <= -65 )
		goto tr1;
	goto tr0;
tr88:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st795;
st795:
	if ( ++p == pe )
		goto _test_eof795;
case 795:
#line 3327 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto tr996;
		case 115: goto tr1093;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	switch( (*p) ) {
		case 35: goto st28;
		case 97: goto st10;
		case 99: goto st36;
		case 100: goto st39;
	}
	goto tr25;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr89;
	goto tr0;
tr89:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st796;
st796:
	if ( ++p == pe )
		goto _test_eof796;
case 796:
#line 3381 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 44: goto st103;
		case 45: goto st42;
		case 46: goto st22;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr89;
	goto tr1005;
tr1065:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st797;
st797:
	if ( ++p == pe )
		goto _test_eof797;
case 797:
#line 3432 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto st13;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1094;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1096;
		} else
			goto tr1095;
	} else
		goto st8;
	goto tr997;
tr1094:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st798;
st798:
	if ( ++p == pe )
		goto _test_eof798;
case 798:
#line 3467 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto tr996;
		case 47: goto st104;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1094;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto tr0;
tr90:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st799;
st799:
	if ( ++p == pe )
		goto _test_eof799;
case 799:
#line 3510 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto tr1005;
tr1095:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 32;}
	goto st800;
st800:
	if ( ++p == pe )
		goto _test_eof800;
case 800:
#line 3558 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr1097;
		case 46: goto st15;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1096;
		} else
			goto tr1095;
	} else
		goto st8;
	goto tr1036;
tr1097:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st801;
st801:
	if ( ++p == pe )
		goto _test_eof801;
case 801:
#line 3595 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto st13;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1096;
		} else
			goto tr1095;
	} else
		goto st8;
	goto tr997;
tr1096:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 32;}
	goto st802;
st802:
	if ( ++p == pe )
		goto _test_eof802;
case 802:
#line 3630 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr1097;
		case 46: goto st15;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1096;
		} else
			goto tr1095;
	} else
		goto st8;
	goto tr1036;
tr1067:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st803;
st803:
	if ( ++p == pe )
		goto _test_eof803;
case 803:
#line 3667 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 66: goto tr1098;
		case 72: goto tr1099;
		case 98: goto tr1100;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1098:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st804;
tr1105:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st804;
st804:
	if ( ++p == pe )
		goto _test_eof804;
case 804:
#line 3717 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 47: goto st44;
		case 105: goto tr1101;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1101:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st805;
st805:
	if ( ++p == pe )
		goto _test_eof805;
case 805:
#line 3756 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 116: goto tr1102;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1102:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st806;
tr1109:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st806;
st806:
	if ( ++p == pe )
		goto _test_eof806;
case 806:
#line 3804 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 47: goto st44;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1099:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st807;
st807:
	if ( ++p == pe )
		goto _test_eof807;
case 807:
#line 3842 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 122: goto tr1093;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1100:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st808;
tr1106:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st808;
st808:
	if ( ++p == pe )
		goto _test_eof808;
case 808:
#line 3890 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 47: goto st44;
		case 105: goto tr1101;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1068:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st809;
st809:
	if ( ++p == pe )
		goto _test_eof809;
case 809:
#line 3929 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 122: goto tr1103;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1069:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st810;
st810:
	if ( ++p == pe )
		goto _test_eof810;
case 810:
#line 3969 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 66: goto tr1104;
		case 72: goto tr1068;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1104:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st811;
st811:
	if ( ++p == pe )
		goto _test_eof811;
case 811:
#line 4010 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 47: goto st44;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1070:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st812;
st812:
	if ( ++p == pe )
		goto _test_eof812;
case 812:
#line 4050 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 66: goto tr1105;
		case 72: goto tr1068;
		case 98: goto tr1106;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1071:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st813;
st813:
	if ( ++p == pe )
		goto _test_eof813;
case 813:
#line 4090 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 97: goto tr1103;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1072:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st814;
st814:
	if ( ++p == pe )
		goto _test_eof814;
case 814:
#line 4128 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 80: goto tr1107;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1107:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st815;
st815:
	if ( ++p == pe )
		goto _test_eof815;
case 815:
#line 4166 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 77: goto tr1108;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1073:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st816;
st816:
	if ( ++p == pe )
		goto _test_eof816;
case 816:
#line 4206 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 66: goto tr1104;
		case 98: goto tr1109;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1074:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st817;
st817:
	if ( ++p == pe )
		goto _test_eof817;
case 817:
#line 4245 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 112: goto tr1110;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1110:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st818;
st818:
	if ( ++p == pe )
		goto _test_eof818;
case 818:
#line 4283 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 115: goto tr1093;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1075:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st819;
st819:
	if ( ++p == pe )
		goto _test_eof819;
case 819:
#line 4323 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 99: goto tr1111;
		case 109: goto tr1112;
		case 117: goto tr1113;
		case 119: goto tr1087;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1119:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st820;
tr1112:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st820;
st820:
	if ( ++p == pe )
		goto _test_eof820;
case 820:
#line 4374 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 94: goto st52;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1114;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr17;
			} else if ( (*p) >= 65 )
				goto tr16;
		} else
			goto tr1;
	} else
		goto tr1;
	goto tr1037;
tr1113:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st821;
st821:
	if ( ++p == pe )
		goto _test_eof821;
case 821:
#line 4418 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 32: goto tr54;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st105;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	if ( (*p) == 32 )
		goto tr61;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr1;
	} else if ( (*p) >= 65 )
		goto tr1;
	goto tr10;
tr1087:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st822;
st822:
	if ( ++p == pe )
		goto _test_eof822;
case 822:
#line 4468 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 116: goto tr1103;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1076:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st823;
st823:
	if ( ++p == pe )
		goto _test_eof823;
case 823:
#line 4506 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 66: goto tr1116;
		case 98: goto tr1103;
		case 112: goto tr1117;
		case 114: goto tr1111;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1117:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st824;
st824:
	if ( ++p == pe )
		goto _test_eof824;
case 824:
#line 4547 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 105: goto tr1093;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1077:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st825;
st825:
	if ( ++p == pe )
		goto _test_eof825;
case 825:
#line 4585 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 108: goto tr1118;
		case 116: goto tr1119;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1118:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st826;
st826:
	if ( ++p == pe )
		goto _test_eof826;
case 826:
#line 4624 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 32: goto st65;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1078:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st827;
st827:
	if ( ++p == pe )
		goto _test_eof827;
case 827:
#line 4664 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 97: goto tr1120;
		case 98: goto tr1106;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 99 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1120:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st828;
st828:
	if ( ++p == pe )
		goto _test_eof828;
case 828:
#line 4703 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 108: goto tr1103;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1079:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st829;
st829:
	if ( ++p == pe )
		goto _test_eof829;
case 829:
#line 4743 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 80: goto tr1071;
		case 114: goto tr1121;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1127:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st830;
tr1140:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st830;
tr1121:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st830;
st830:
	if ( ++p == pe )
		goto _test_eof830;
case 830:
#line 4800 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 115: goto tr1111;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1080:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st831;
st831:
	if ( ++p == pe )
		goto _test_eof831;
case 831:
#line 4838 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 110: goto tr1119;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1081:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st832;
st832:
	if ( ++p == pe )
		goto _test_eof832;
case 832:
#line 4878 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 66: goto tr1104;
		case 72: goto tr1068;
		case 80: goto tr1071;
		case 98: goto tr1106;
		case 103: goto tr1111;
		case 109: goto tr1122;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1122:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st833;
st833:
	if ( ++p == pe )
		goto _test_eof833;
case 833:
#line 4923 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 47: goto st69;
		case 94: goto st52;
		case 104: goto tr1123;
		case 112: goto tr1124;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1114;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr17;
			} else if ( (*p) >= 65 )
				goto tr16;
		} else
			goto tr1;
	} else
		goto tr1;
	goto tr1037;
tr1123:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st834;
st834:
	if ( ++p == pe )
		goto _test_eof834;
case 834:
#line 4972 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr1125;
		case 46: goto st15;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1125:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st835;
st835:
	if ( ++p == pe )
		goto _test_eof835;
case 835:
#line 5009 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto st13;
		case 49: goto tr1126;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1124:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st836;
st836:
	if ( ++p == pe )
		goto _test_eof836;
case 836:
#line 5045 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 104: goto tr1103;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1082:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st837;
st837:
	if ( ++p == pe )
		goto _test_eof837;
case 837:
#line 5083 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 98: goto tr1127;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1083:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st838;
st838:
	if ( ++p == pe )
		goto _test_eof838;
case 838:
#line 5123 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 47: goto st44;
		case 76: goto tr1066;
		case 94: goto st52;
		case 98: goto tr1106;
		case 103: goto tr1111;
		case 105: goto tr1128;
		case 109: goto tr1112;
		case 110: goto tr1129;
		case 112: goto tr1124;
		case 115: goto tr1123;
		case 116: goto tr1130;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1114;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr17;
			} else if ( (*p) >= 65 )
				goto tr16;
		} else
			goto tr1;
	} else
		goto tr1;
	goto tr1037;
tr1128:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st839;
st839:
	if ( ++p == pe )
		goto _test_eof839;
case 839:
#line 5179 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 47: goto st69;
		case 94: goto st52;
		case 99: goto tr1131;
		case 104: goto tr1123;
		case 108: goto tr1132;
		case 110: goto tr1121;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1114;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr17;
			} else if ( (*p) >= 65 )
				goto tr16;
		} else
			goto tr1;
	} else
		goto tr1;
	goto tr1037;
tr1131:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st840;
st840:
	if ( ++p == pe )
		goto _test_eof840;
case 840:
#line 5228 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 114: goto tr1133;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1133:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st841;
st841:
	if ( ++p == pe )
		goto _test_eof841;
case 841:
#line 5266 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 111: goto tr1134;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1134:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st842;
st842:
	if ( ++p == pe )
		goto _test_eof842;
case 842:
#line 5304 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 115: goto tr1135;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1135:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st843;
st843:
	if ( ++p == pe )
		goto _test_eof843;
case 843:
#line 5342 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 101: goto tr1136;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1136:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st844;
st844:
	if ( ++p == pe )
		goto _test_eof844;
case 844:
#line 5380 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 99: goto tr1137;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1132:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st845;
st845:
	if ( ++p == pe )
		goto _test_eof845;
case 845:
#line 5420 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 108: goto tr1138;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1138:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st846;
st846:
	if ( ++p == pe )
		goto _test_eof846;
case 846:
#line 5458 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 105: goto tr1134;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1129:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st847;
st847:
	if ( ++p == pe )
		goto _test_eof847;
case 847:
#line 5496 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 116: goto tr1139;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1139:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st848;
st848:
	if ( ++p == pe )
		goto _test_eof848;
case 848:
#line 5534 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 104: goto tr1140;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1130:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st849;
st849:
	if ( ++p == pe )
		goto _test_eof849;
case 849:
#line 5572 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 104: goto tr1127;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1084:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st850;
st850:
	if ( ++p == pe )
		goto _test_eof850;
case 850:
#line 5610 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 97: goto tr1141;
		case 115: goto tr1103;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1141:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st851;
st851:
	if ( ++p == pe )
		goto _test_eof851;
case 851:
#line 5649 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 110: goto tr1142;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1142:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st852;
st852:
	if ( ++p == pe )
		goto _test_eof852;
case 852:
#line 5687 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 111: goto tr1143;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1143:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st853;
st853:
	if ( ++p == pe )
		goto _test_eof853;
case 853:
#line 5725 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 115: goto tr1144;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1144:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st854;
st854:
	if ( ++p == pe )
		goto _test_eof854;
case 854:
#line 5763 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 101: goto tr1145;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1145:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st855;
st855:
	if ( ++p == pe )
		goto _test_eof855;
case 855:
#line 5801 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 99: goto tr1146;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1085:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st856;
st856:
	if ( ++p == pe )
		goto _test_eof856;
case 856:
#line 5839 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 122: goto tr1103;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1086:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st857;
st857:
	if ( ++p == pe )
		goto _test_eof857;
case 857:
#line 5877 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 115: goto tr1117;
		case 116: goto tr1103;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1088:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st858;
st858:
	if ( ++p == pe )
		goto _test_eof858;
case 858:
#line 5916 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 112: goto tr1147;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1147:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st859;
st859:
	if ( ++p == pe )
		goto _test_eof859;
case 859:
#line 5954 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 109: goto tr1093;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1089:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st860;
st860:
	if ( ++p == pe )
		goto _test_eof860;
case 860:
#line 5994 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 101: goto tr1148;
		case 113: goto tr1149;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr1037;
tr1148:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st861;
st861:
	if ( ++p == pe )
		goto _test_eof861;
case 861:
#line 6033 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 99: goto tr1127;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1149:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st862;
st862:
	if ( ++p == pe )
		goto _test_eof862;
case 862:
#line 6071 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 32: goto tr84;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st106;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	if ( (*p) == 32 )
		goto tr54;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr1;
	} else if ( (*p) >= 65 )
		goto tr1;
	goto tr10;
tr1090:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st863;
st863:
	if ( ++p == pe )
		goto _test_eof863;
case 863:
#line 6121 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 111: goto tr1151;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1151:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st864;
st864:
	if ( ++p == pe )
		goto _test_eof864;
case 864:
#line 6159 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 110: goto tr1093;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1091:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st865;
st865:
	if ( ++p == pe )
		goto _test_eof865;
case 865:
#line 6197 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 115: goto tr1103;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
tr1092:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st866;
st866:
	if ( ++p == pe )
		goto _test_eof866;
case 866:
#line 6235 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 100: goto tr1119;
		case 114: goto tr1127;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr17;
		} else
			goto tr16;
	} else
		goto st8;
	goto tr997;
st867:
	if ( ++p == pe )
		goto _test_eof867;
case 867:
	if ( (*p) <= -65 )
		goto tr1;
	goto tr995;
tr915:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "src/ragel/rules/main.rl"
	{ s.act = 41;}
	goto st868;
st868:
	if ( ++p == pe )
		goto _test_eof868;
case 868:
#line 6281 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) <= -65 )
		goto st2;
	goto tr995;
tr916:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "src/ragel/rules/main.rl"
	{ s.act = 41;}
	goto st869;
st869:
	if ( ++p == pe )
		goto _test_eof869;
case 869:
#line 6295 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == -68 )
		goto st107;
	if ( (*p) <= -65 )
		goto st2;
	goto tr995;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	if ( (*p) == -125 )
		goto tr92;
	if ( (*p) <= -65 )
		goto tr1;
	goto tr91;
tr92:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st870;
tr1153:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st870;
st870:
	if ( ++p == pe )
		goto _test_eof870;
case 870:
#line 6326 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 45: goto tr996;
		case 95: goto tr816;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1153;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1155;
		} else
			goto tr1154;
	} else
		goto st8;
	goto tr0;
tr1154:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st871;
st871:
	if ( ++p == pe )
		goto _test_eof871;
case 871:
#line 6362 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st14;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 95: goto tr816;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1153;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1155;
		} else
			goto tr1154;
	} else
		goto st8;
	goto tr1156;
tr1191:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st872;
tr1190:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st872;
tr1160:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st872;
tr1216:
#line 1 "NONE"
	{ s.te = p+1;}
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st872;
tr1225:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st872;
tr816:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st872;
st872:
	if ( ++p == pe )
		goto _test_eof872;
case 872:
#line 6440 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 95 )
		goto tr816;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr0;
tr1155:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st873;
st873:
	if ( ++p == pe )
		goto _test_eof873;
case 873:
#line 6462 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st4;
		case 38: goto st9;
		case 39: goto st15;
		case 45: goto tr996;
		case 46: goto st15;
		case 95: goto tr816;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1153;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1155;
		} else
			goto tr1154;
	} else
		goto st8;
	goto tr1156;
tr917:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "src/ragel/rules/main.rl"
	{ s.act = 41;}
	goto st874;
st874:
	if ( ++p == pe )
		goto _test_eof874;
case 874:
#line 6500 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) <= -65 )
		goto st3;
	goto tr995;
tr918:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/rules/main.rl"
	{ s.act = 12;}
	goto st875;
tr93:
#line 1 "NONE"
	{ s.te = p+1;}
#line 46 "src/ragel/rules/main.rl"
	{ s.act = 13;}
	goto st875;
st875:
	if ( ++p == pe )
		goto _test_eof875;
case 875:
#line 6520 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 10: goto tr93;
		case 13: goto st108;
	}
	goto tr0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	if ( (*p) == 10 )
		goto tr93;
	goto tr0;
tr919:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/rules/main.rl"
	{ s.act = 12;}
	goto st876;
st876:
	if ( ++p == pe )
		goto _test_eof876;
case 876:
#line 6543 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st16;
		case 10: goto tr918;
		case 32: goto tr19;
		case 38: goto st17;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr19;
	goto tr1000;
st877:
	if ( ++p == pe )
		goto _test_eof877;
case 877:
	if ( (*p) == 95 )
		goto tr816;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1159;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1158;
tr1159:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st878;
st878:
	if ( ++p == pe )
		goto _test_eof878;
case 878:
#line 6578 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 67: goto tr1160;
		case 70: goto tr1160;
		case 71: goto st879;
		case 72: goto st884;
		case 75: goto tr1163;
		case 76: goto tr1160;
		case 77: goto tr1164;
		case 80: goto st887;
		case 82: goto st888;
		case 84: goto tr1167;
		case 95: goto tr816;
		case 98: goto st891;
		case 99: goto tr1169;
		case 100: goto st897;
		case 102: goto st899;
		case 103: goto tr1172;
		case 104: goto tr1173;
		case 105: goto st905;
		case 107: goto tr1175;
		case 108: goto st910;
		case 109: goto tr1177;
		case 110: goto st923;
		case 111: goto st884;
		case 112: goto st929;
		case 113: goto st896;
		case 114: goto st930;
		case 115: goto tr1182;
		case 116: goto st935;
		case 117: goto st937;
		case 121: goto st938;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1159;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1005;
st879:
	if ( ++p == pe )
		goto _test_eof879;
case 879:
	switch( (*p) ) {
		case 66: goto tr1186;
		case 72: goto st883;
		case 95: goto tr816;
		case 98: goto tr1186;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1186:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st880;
tr1193:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st880;
st880:
	if ( ++p == pe )
		goto _test_eof880;
case 880:
#line 6658 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st44;
		case 95: goto tr816;
		case 105: goto st881;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
st881:
	if ( ++p == pe )
		goto _test_eof881;
case 881:
	switch( (*p) ) {
		case 95: goto tr816;
		case 116: goto tr1189;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1189:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st882;
tr1192:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st882;
st882:
	if ( ++p == pe )
		goto _test_eof882;
case 882:
#line 6706 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st44;
		case 95: goto tr816;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
st883:
	if ( ++p == pe )
		goto _test_eof883;
case 883:
	switch( (*p) ) {
		case 95: goto tr816;
		case 122: goto tr1190;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 121 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st884:
	if ( ++p == pe )
		goto _test_eof884;
case 884:
	switch( (*p) ) {
		case 95: goto tr816;
		case 122: goto tr1191;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 121 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1163:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st885;
st885:
	if ( ++p == pe )
		goto _test_eof885;
case 885:
#line 6762 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 66: goto tr1192;
		case 72: goto st884;
		case 95: goto tr816;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
tr1164:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st886;
st886:
	if ( ++p == pe )
		goto _test_eof886;
case 886:
#line 6785 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 66: goto tr1193;
		case 72: goto st884;
		case 95: goto tr816;
		case 98: goto tr1193;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
st887:
	if ( ++p == pe )
		goto _test_eof887;
case 887:
	switch( (*p) ) {
		case 95: goto tr816;
		case 97: goto tr1191;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st888:
	if ( ++p == pe )
		goto _test_eof888;
case 888:
	switch( (*p) ) {
		case 80: goto st889;
		case 95: goto tr816;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st889:
	if ( ++p == pe )
		goto _test_eof889;
case 889:
	switch( (*p) ) {
		case 77: goto tr1190;
		case 95: goto tr816;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1167:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st890;
st890:
	if ( ++p == pe )
		goto _test_eof890;
case 890:
#line 6860 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 66: goto tr1192;
		case 95: goto tr816;
		case 98: goto tr1192;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
st891:
	if ( ++p == pe )
		goto _test_eof891;
case 891:
	switch( (*p) ) {
		case 95: goto tr816;
		case 112: goto st892;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st892:
	if ( ++p == pe )
		goto _test_eof892;
case 892:
	switch( (*p) ) {
		case 95: goto tr816;
		case 115: goto tr1190;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1169:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st893;
st893:
	if ( ++p == pe )
		goto _test_eof893;
case 893:
#line 6917 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 95: goto tr816;
		case 99: goto tr1160;
		case 109: goto tr1196;
		case 117: goto tr1197;
		case 119: goto st896;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
tr1200:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st894;
tr1196:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st894;
st894:
	if ( ++p == pe )
		goto _test_eof894;
case 894:
#line 6950 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 94: goto st52;
		case 95: goto tr816;
	}
	if ( (*p) < 52 ) {
		if ( (*p) > 49 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1160;
		} else if ( (*p) >= 48 )
			goto tr816;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr816;
		} else if ( (*p) >= 65 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
tr1197:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st895;
st895:
	if ( ++p == pe )
		goto _test_eof895;
case 895:
#line 6980 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 32: goto tr54;
		case 46: goto st59;
		case 95: goto tr816;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st896:
	if ( ++p == pe )
		goto _test_eof896;
case 896:
	switch( (*p) ) {
		case 95: goto tr816;
		case 116: goto tr1191;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st897:
	if ( ++p == pe )
		goto _test_eof897;
case 897:
	switch( (*p) ) {
		case 66: goto tr1191;
		case 95: goto tr816;
		case 98: goto tr1191;
		case 112: goto st898;
		case 114: goto tr1160;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st898:
	if ( ++p == pe )
		goto _test_eof898;
case 898:
	switch( (*p) ) {
		case 95: goto tr816;
		case 105: goto tr1190;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st899:
	if ( ++p == pe )
		goto _test_eof899;
case 899:
	switch( (*p) ) {
		case 95: goto tr816;
		case 108: goto tr1199;
		case 116: goto tr1200;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1199:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st900;
st900:
	if ( ++p == pe )
		goto _test_eof900;
case 900:
#line 7077 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 32: goto st65;
		case 95: goto tr816;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1172:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st901;
st901:
	if ( ++p == pe )
		goto _test_eof901;
case 901:
#line 7099 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 95: goto tr816;
		case 97: goto st902;
		case 98: goto tr1193;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 99 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
st902:
	if ( ++p == pe )
		goto _test_eof902;
case 902:
	switch( (*p) ) {
		case 95: goto tr816;
		case 108: goto tr1191;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1173:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st903;
st903:
	if ( ++p == pe )
		goto _test_eof903;
case 903:
#line 7139 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 80: goto st887;
		case 95: goto tr816;
		case 114: goto tr1202;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
tr1206:
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
	goto st904;
tr1219:
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
	goto st904;
tr1202:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st904;
st904:
	if ( ++p == pe )
		goto _test_eof904;
case 904:
#line 7170 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 95: goto tr816;
		case 115: goto tr1160;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
st905:
	if ( ++p == pe )
		goto _test_eof905;
case 905:
	switch( (*p) ) {
		case 95: goto tr816;
		case 110: goto tr1200;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1175:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st906;
st906:
	if ( ++p == pe )
		goto _test_eof906;
case 906:
#line 7209 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 66: goto tr1192;
		case 72: goto st884;
		case 80: goto st887;
		case 95: goto tr816;
		case 98: goto tr1193;
		case 103: goto tr1160;
		case 109: goto tr1203;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
tr1203:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st907;
st907:
	if ( ++p == pe )
		goto _test_eof907;
case 907:
#line 7238 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st69;
		case 94: goto st52;
		case 95: goto tr816;
		case 104: goto tr1204;
		case 112: goto st909;
	}
	if ( (*p) < 52 ) {
		if ( (*p) > 49 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1160;
		} else if ( (*p) >= 48 )
			goto tr816;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr816;
		} else if ( (*p) >= 65 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
tr1204:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st908;
st908:
	if ( ++p == pe )
		goto _test_eof908;
case 908:
#line 7271 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st70;
		case 95: goto tr816;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
st909:
	if ( ++p == pe )
		goto _test_eof909;
case 909:
	switch( (*p) ) {
		case 95: goto tr816;
		case 104: goto tr1191;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st910:
	if ( ++p == pe )
		goto _test_eof910;
case 910:
	switch( (*p) ) {
		case 95: goto tr816;
		case 98: goto tr1206;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1177:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st911;
st911:
	if ( ++p == pe )
		goto _test_eof911;
case 911:
#line 7329 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st44;
		case 76: goto tr1160;
		case 94: goto st52;
		case 95: goto tr816;
		case 98: goto tr1193;
		case 103: goto tr1160;
		case 105: goto tr1207;
		case 109: goto tr1196;
		case 110: goto st920;
		case 112: goto st909;
		case 115: goto tr1204;
		case 116: goto st922;
	}
	if ( (*p) < 52 ) {
		if ( (*p) > 49 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1160;
		} else if ( (*p) >= 48 )
			goto tr816;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr816;
		} else if ( (*p) >= 65 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
tr1207:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st912;
st912:
	if ( ++p == pe )
		goto _test_eof912;
case 912:
#line 7369 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st69;
		case 94: goto st52;
		case 95: goto tr816;
		case 99: goto st913;
		case 104: goto tr1204;
		case 108: goto tr1211;
		case 110: goto tr1202;
	}
	if ( (*p) < 52 ) {
		if ( (*p) > 49 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1160;
		} else if ( (*p) >= 48 )
			goto tr816;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr816;
		} else if ( (*p) >= 65 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
st913:
	if ( ++p == pe )
		goto _test_eof913;
case 913:
	switch( (*p) ) {
		case 95: goto tr816;
		case 114: goto st914;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st914:
	if ( ++p == pe )
		goto _test_eof914;
case 914:
	switch( (*p) ) {
		case 95: goto tr816;
		case 111: goto st915;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st915:
	if ( ++p == pe )
		goto _test_eof915;
case 915:
	switch( (*p) ) {
		case 95: goto tr816;
		case 115: goto st916;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st916:
	if ( ++p == pe )
		goto _test_eof916;
case 916:
	switch( (*p) ) {
		case 95: goto tr816;
		case 101: goto st917;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st917:
	if ( ++p == pe )
		goto _test_eof917;
case 917:
	switch( (*p) ) {
		case 95: goto tr816;
		case 99: goto tr1216;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1211:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st918;
st918:
	if ( ++p == pe )
		goto _test_eof918;
case 918:
#line 7487 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 95: goto tr816;
		case 108: goto st919;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
st919:
	if ( ++p == pe )
		goto _test_eof919;
case 919:
	switch( (*p) ) {
		case 95: goto tr816;
		case 105: goto st915;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st920:
	if ( ++p == pe )
		goto _test_eof920;
case 920:
	switch( (*p) ) {
		case 95: goto tr816;
		case 116: goto st921;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st921:
	if ( ++p == pe )
		goto _test_eof921;
case 921:
	switch( (*p) ) {
		case 95: goto tr816;
		case 104: goto tr1219;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st922:
	if ( ++p == pe )
		goto _test_eof922;
case 922:
	switch( (*p) ) {
		case 95: goto tr816;
		case 104: goto tr1206;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st923:
	if ( ++p == pe )
		goto _test_eof923;
case 923:
	switch( (*p) ) {
		case 95: goto tr816;
		case 97: goto st924;
		case 115: goto tr1191;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st924:
	if ( ++p == pe )
		goto _test_eof924;
case 924:
	switch( (*p) ) {
		case 95: goto tr816;
		case 110: goto st925;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st925:
	if ( ++p == pe )
		goto _test_eof925;
case 925:
	switch( (*p) ) {
		case 95: goto tr816;
		case 111: goto st926;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st926:
	if ( ++p == pe )
		goto _test_eof926;
case 926:
	switch( (*p) ) {
		case 95: goto tr816;
		case 115: goto st927;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st927:
	if ( ++p == pe )
		goto _test_eof927;
case 927:
	switch( (*p) ) {
		case 95: goto tr816;
		case 101: goto st928;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st928:
	if ( ++p == pe )
		goto _test_eof928;
case 928:
	switch( (*p) ) {
		case 95: goto tr816;
		case 99: goto tr1225;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st929:
	if ( ++p == pe )
		goto _test_eof929;
case 929:
	switch( (*p) ) {
		case 95: goto tr816;
		case 115: goto st898;
		case 116: goto tr1191;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st930:
	if ( ++p == pe )
		goto _test_eof930;
case 930:
	switch( (*p) ) {
		case 95: goto tr816;
		case 112: goto st931;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st931:
	if ( ++p == pe )
		goto _test_eof931;
case 931:
	switch( (*p) ) {
		case 95: goto tr816;
		case 109: goto tr1190;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1182:
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
	goto st932;
st932:
	if ( ++p == pe )
		goto _test_eof932;
case 932:
#line 7732 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 95: goto tr816;
		case 101: goto st933;
		case 113: goto tr1228;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1037;
st933:
	if ( ++p == pe )
		goto _test_eof933;
case 933:
	switch( (*p) ) {
		case 95: goto tr816;
		case 99: goto tr1206;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1228:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st934;
st934:
	if ( ++p == pe )
		goto _test_eof934;
case 934:
#line 7774 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 32: goto tr84;
		case 46: goto st95;
		case 95: goto tr816;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st935:
	if ( ++p == pe )
		goto _test_eof935;
case 935:
	switch( (*p) ) {
		case 95: goto tr816;
		case 111: goto st936;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st936:
	if ( ++p == pe )
		goto _test_eof936;
case 936:
	switch( (*p) ) {
		case 95: goto tr816;
		case 110: goto tr1190;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st937:
	if ( ++p == pe )
		goto _test_eof937;
case 937:
	switch( (*p) ) {
		case 95: goto tr816;
		case 115: goto tr1191;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
st938:
	if ( ++p == pe )
		goto _test_eof938;
case 938:
	switch( (*p) ) {
		case 95: goto tr816;
		case 100: goto tr1200;
		case 114: goto tr1206;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr816;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr816;
	} else
		goto tr816;
	goto tr1156;
tr1230:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st939;
tr922:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "src/ragel/rules/main.rl"
	{ s.act = 35;}
	goto st939;
st939:
	if ( ++p == pe )
		goto _test_eof939;
case 939:
#line 7874 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr1230;
	goto tr0;
tr923:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st940;
st940:
	if ( ++p == pe )
		goto _test_eof940;
case 940:
#line 7886 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 39: goto tr809;
		case 83: goto tr1233;
		case 115: goto tr1233;
		case 116: goto st110;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st109;
	goto tr1231;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	if ( (*p) == 48 )
		goto st941;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto tr94;
st941:
	if ( ++p == pe )
		goto _test_eof941;
case 941:
	if ( (*p) == 115 )
		goto tr96;
	goto tr1235;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) == 105 )
		goto st111;
	goto tr94;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	if ( (*p) == 115 )
		goto tr98;
	goto tr94;
tr1236:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st942;
tr925:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "src/ragel/rules/main.rl"
	{ s.act = 35;}
	goto st942;
st942:
	if ( ++p == pe )
		goto _test_eof942;
case 942:
#line 7942 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 42 )
		goto tr1236;
	goto tr0;
tr926:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "src/ragel/rules/main.rl"
	{ s.act = 35;}
	goto st943;
st943:
	if ( ++p == pe )
		goto _test_eof943;
case 943:
#line 7956 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st162;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1238;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1158;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr0;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	if ( (*p) == 50 )
		goto st130;
	if ( (*p) < 51 ) {
		if ( 48 <= (*p) && (*p) <= 49 )
			goto st114;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr104;
		} else if ( (*p) >= 65 )
			goto tr104;
	} else
		goto st128;
	goto tr0;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	if ( (*p) == 46 )
		goto st115;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st128;
	goto tr0;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	if ( (*p) == 50 )
		goto st126;
	if ( (*p) > 49 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st124;
	} else if ( (*p) >= 48 )
		goto st116;
	goto tr0;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	if ( (*p) == 46 )
		goto st117;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st124;
	goto tr0;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	if ( (*p) == 50 )
		goto st122;
	if ( (*p) > 49 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st120;
	} else if ( (*p) >= 48 )
		goto st118;
	goto tr0;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	if ( (*p) == 46 )
		goto st119;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st120;
	goto tr0;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	if ( (*p) == 50 )
		goto st946;
	if ( (*p) > 49 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st945;
	} else if ( (*p) >= 48 )
		goto st944;
	goto tr0;
st944:
	if ( ++p == pe )
		goto _test_eof944;
case 944:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st945;
	goto tr1156;
st945:
	if ( ++p == pe )
		goto _test_eof945;
case 945:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr1239;
	goto tr1156;
st946:
	if ( ++p == pe )
		goto _test_eof946;
case 946:
	if ( (*p) == 53 )
		goto st947;
	if ( (*p) > 52 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr1239;
	} else if ( (*p) >= 48 )
		goto st945;
	goto tr1156;
st947:
	if ( ++p == pe )
		goto _test_eof947;
case 947:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr1239;
	goto tr1156;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	if ( (*p) == 46 )
		goto st119;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st121;
	goto tr0;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	if ( (*p) == 46 )
		goto st119;
	goto tr0;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
	switch( (*p) ) {
		case 46: goto st119;
		case 53: goto st123;
	}
	if ( (*p) > 52 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto st121;
	} else if ( (*p) >= 48 )
		goto st120;
	goto tr0;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	if ( (*p) == 46 )
		goto st119;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st121;
	goto tr0;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	if ( (*p) == 46 )
		goto st117;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st125;
	goto tr0;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	if ( (*p) == 46 )
		goto st117;
	goto tr0;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	switch( (*p) ) {
		case 46: goto st117;
		case 53: goto st127;
	}
	if ( (*p) > 52 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto st125;
	} else if ( (*p) >= 48 )
		goto st124;
	goto tr0;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	if ( (*p) == 46 )
		goto st117;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st125;
	goto tr0;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	if ( (*p) == 46 )
		goto st115;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st129;
	goto tr0;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	if ( (*p) == 46 )
		goto st115;
	goto tr0;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
	switch( (*p) ) {
		case 46: goto st115;
		case 53: goto st131;
	}
	if ( (*p) > 52 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto st129;
	} else if ( (*p) >= 48 )
		goto st128;
	goto tr0;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	if ( (*p) == 46 )
		goto st115;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st129;
	goto tr0;
tr104:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st948;
st948:
	if ( ++p == pe )
		goto _test_eof948;
case 948:
#line 8233 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st132;
		case 46: goto st133;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr104;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr104;
	} else
		goto tr104;
	goto tr1156;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	if ( (*p) == 45 )
		goto st132;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr104;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr104;
	} else
		goto tr104;
	goto tr123;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
	switch( (*p) ) {
		case 65: goto st134;
		case 66: goto st139;
		case 67: goto st140;
		case 69: goto st143;
		case 71: goto st144;
		case 73: goto st145;
		case 74: goto st146;
		case 77: goto st148;
		case 78: goto st153;
		case 79: goto st155;
		case 80: goto st156;
		case 84: goto st157;
		case 88: goto st161;
		case 97: goto st134;
		case 98: goto st139;
		case 99: goto st140;
		case 101: goto st143;
		case 103: goto st144;
		case 105: goto st145;
		case 106: goto st146;
		case 109: goto st148;
		case 110: goto st153;
		case 111: goto st155;
		case 112: goto st156;
		case 116: goto st157;
		case 120: goto st161;
	}
	if ( (*p) > 90 ) {
		if ( 100 <= (*p) && (*p) <= 122 )
			goto st142;
	} else if ( (*p) >= 68 )
		goto st142;
	goto tr123;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 69: goto tr143;
		case 82: goto tr144;
		case 83: goto tr145;
		case 101: goto tr143;
		case 114: goto tr144;
		case 115: goto tr145;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	if ( (*p) == 45 )
		goto st135;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
tr142:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st949;
st949:
	if ( ++p == pe )
		goto _test_eof949;
case 949:
#line 8363 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
tr143:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st950;
st950:
	if ( ++p == pe )
		goto _test_eof950;
case 950:
#line 8385 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 82: goto st137;
		case 114: goto st137;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 79: goto tr142;
		case 111: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
tr144:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st951;
st951:
	if ( ++p == pe )
		goto _test_eof951;
case 951:
#line 8428 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 80: goto st138;
		case 112: goto st138;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 65: goto tr142;
		case 97: goto tr142;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
tr145:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st952;
st952:
	if ( ++p == pe )
		goto _test_eof952;
case 952:
#line 8471 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 73: goto st138;
		case 105: goto st138;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 73: goto tr146;
		case 105: goto tr146;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr146:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st953;
st953:
	if ( ++p == pe )
		goto _test_eof953;
case 953:
#line 8514 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 90: goto tr142;
		case 122: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 89 ) {
		if ( 97 <= (*p) && (*p) <= 121 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 65: goto tr147;
		case 79: goto tr148;
		case 97: goto tr147;
		case 111: goto tr148;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr147:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st954;
st954:
	if ( ++p == pe )
		goto _test_eof954;
case 954:
#line 8559 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 84: goto tr142;
		case 116: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
tr148:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st955;
st955:
	if ( ++p == pe )
		goto _test_eof955;
case 955:
#line 8583 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 77: goto tr142;
		case 79: goto st141;
		case 109: goto tr142;
		case 111: goto st141;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 80: goto tr142;
		case 112: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 68: goto tr149;
		case 100: goto tr149;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr149:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st956;
st956:
	if ( ++p == pe )
		goto _test_eof956;
case 956:
#line 8664 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 85: goto tr142;
		case 117: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 79: goto tr150;
		case 111: goto tr150;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr150:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st957;
st957:
	if ( ++p == pe )
		goto _test_eof957;
case 957:
#line 8707 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 86: goto tr142;
		case 118: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 78: goto tr151;
		case 110: goto tr151;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr151:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st958;
st958:
	if ( ++p == pe )
		goto _test_eof958;
case 958:
#line 8750 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 70: goto st137;
		case 84: goto tr142;
		case 102: goto st137;
		case 116: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 79: goto tr152;
		case 111: goto tr152;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr152:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st959;
st959:
	if ( ++p == pe )
		goto _test_eof959;
case 959:
#line 8795 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 66: goto st147;
		case 98: goto st147;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 83: goto tr142;
		case 115: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 73: goto tr153;
		case 79: goto tr154;
		case 85: goto tr155;
		case 105: goto tr153;
		case 111: goto tr154;
		case 117: goto tr155;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr153:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st960;
st960:
	if ( ++p == pe )
		goto _test_eof960;
case 960:
#line 8861 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 76: goto tr142;
		case 108: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
tr154:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st961;
st961:
	if ( ++p == pe )
		goto _test_eof961;
case 961:
#line 8885 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 66: goto st149;
		case 98: goto st149;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 73: goto tr142;
		case 105: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
tr155:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st962;
st962:
	if ( ++p == pe )
		goto _test_eof962;
case 962:
#line 8928 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 83: goto st150;
		case 115: goto st150;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 69: goto st151;
		case 101: goto st151;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 85: goto st152;
		case 117: goto st152;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 77: goto tr142;
		case 109: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 65: goto tr158;
		case 69: goto tr147;
		case 97: goto tr158;
		case 101: goto tr147;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr158:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st963;
st963:
	if ( ++p == pe )
		goto _test_eof963;
case 963:
#line 9030 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 77: goto st154;
		case 109: goto st154;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 69: goto tr142;
		case 101: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 82: goto tr159;
		case 114: goto tr159;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr159:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st964;
st964:
	if ( ++p == pe )
		goto _test_eof964;
case 964:
#line 9092 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 71: goto tr142;
		case 103: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 82: goto tr160;
		case 114: goto tr160;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr160:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st965;
st965:
	if ( ++p == pe )
		goto _test_eof965;
case 965:
#line 9135 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 79: goto tr142;
		case 111: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 69: goto tr153;
		case 82: goto tr161;
		case 101: goto tr153;
		case 114: goto tr161;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr161:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st966;
st966:
	if ( ++p == pe )
		goto _test_eof966;
case 966:
#line 9180 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 65: goto st158;
		case 97: goto st158;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 86: goto st159;
		case 118: goto st159;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 69: goto st160;
		case 101: goto st160;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 76: goto tr142;
		case 108: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr123;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 88: goto tr164;
		case 120: goto tr164;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr142;
	} else
		goto tr142;
	goto tr123;
tr164:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st967;
st967:
	if ( ++p == pe )
		goto _test_eof967;
case 967:
#line 9280 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st135;
		case 46: goto st133;
		case 88: goto tr142;
		case 120: goto tr142;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st136;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st136;
	} else
		goto st136;
	goto tr1156;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr165;
	goto tr0;
tr165:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st968;
st968:
	if ( ++p == pe )
		goto _test_eof968;
case 968:
#line 9327 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st112;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr165;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr166;
		} else if ( (*p) >= 65 )
			goto tr166;
	} else
		goto st112;
	goto tr0;
tr166:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 32;}
	goto st969;
st969:
	if ( ++p == pe )
		goto _test_eof969;
case 969:
#line 9407 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr166;
	} else
		goto tr166;
	goto tr1036;
tr171:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st970;
tr170:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st970;
tr176:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st970;
tr184:
#line 1 "NONE"
	{ s.te = p+1;}
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st970;
tr192:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st970;
tr1415:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st970;
tr468:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 58 "src/ragel/rules/date_time.rl"
	{n2="am";}
#line 71 "src/ragel/rules/main.rl"
	{ s.act = 30;}
	goto st970;
tr470:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 58 "src/ragel/rules/date_time.rl"
	{n2="am";}
#line 71 "src/ragel/rules/main.rl"
	{ s.act = 30;}
	goto st970;
tr472:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 59 "src/ragel/rules/date_time.rl"
	{n2="pm";}
#line 71 "src/ragel/rules/main.rl"
	{ s.act = 30;}
	goto st970;
tr474:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 59 "src/ragel/rules/date_time.rl"
	{n2="pm";}
#line 71 "src/ragel/rules/main.rl"
	{ s.act = 30;}
	goto st970;
tr481:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st970;
tr689:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st970;
tr964:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "src/ragel/rules/main.rl"
	{ s.act = 35;}
	goto st970;
st970:
	if ( ++p == pe )
		goto _test_eof970;
case 970:
#line 9532 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr0;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 66: goto tr167;
		case 72: goto st166;
		case 95: goto st112;
		case 98: goto tr167;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
tr167:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st971;
tr1278:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st971;
st971:
	if ( ++p == pe )
		goto _test_eof971;
case 971:
#line 9594 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 47: goto st44;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto st165;
	}
	if ( (*p) < 65 ) {
		if ( 45 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr169;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
tr169:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st972;
tr1277:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st972;
st972:
	if ( ++p == pe )
		goto _test_eof972;
case 972:
#line 9653 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 47: goto st44;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 45 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 122: goto tr170;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 122: goto tr171;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr0;
tr1252:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st973;
st973:
	if ( ++p == pe )
		goto _test_eof973;
case 973:
#line 9725 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 66: goto tr1277;
		case 72: goto st167;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
tr1253:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st974;
st974:
	if ( ++p == pe )
		goto _test_eof974;
case 974:
#line 9757 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 66: goto tr1278;
		case 72: goto st167;
		case 95: goto st112;
		case 98: goto tr1278;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr171;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr0;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 80: goto st170;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 77: goto tr170;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
tr1256:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st975;
st975:
	if ( ++p == pe )
		goto _test_eof975;
case 975:
#line 9856 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 66: goto tr1277;
		case 95: goto st112;
		case 98: goto tr1277;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 112: goto st172;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr170;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
tr1258:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st976;
st976:
	if ( ++p == pe )
		goto _test_eof976;
case 976:
#line 9932 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr176;
		case 109: goto tr1279;
		case 117: goto st173;
		case 119: goto st175;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
tr178:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st977;
tr1279:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st977;
st977:
	if ( ++p == pe )
		goto _test_eof977;
case 977:
#line 9974 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 94: goto st52;
		case 95: goto st112;
	}
	if ( (*p) < 50 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto st112;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 51 ) {
		if ( (*p) < 65 ) {
			if ( 52 <= (*p) && (*p) <= 57 )
				goto st112;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else
			goto st112;
	} else
		goto tr176;
	goto tr1037;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
	switch( (*p) ) {
		case 32: goto tr54;
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st174;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
	switch( (*p) ) {
		case 32: goto tr61;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr171;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 66: goto tr171;
		case 95: goto st112;
		case 98: goto tr171;
		case 112: goto st177;
		case 114: goto tr176;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto tr170;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto st179;
		case 116: goto tr178;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
	switch( (*p) ) {
		case 32: goto st65;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
tr1261:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st978;
st978:
	if ( ++p == pe )
		goto _test_eof978;
case 978:
#line 10168 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto st180;
		case 98: goto tr1278;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 99 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
st180:
	if ( ++p == pe )
		goto _test_eof180;
case 180:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr171;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
tr1262:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st979;
st979:
	if ( ++p == pe )
		goto _test_eof979;
case 979:
#line 10222 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 80: goto st168;
		case 95: goto st112;
		case 114: goto tr1283;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
tr179:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st980;
tr186:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st980;
tr1283:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st980;
st980:
	if ( ++p == pe )
		goto _test_eof980;
case 980:
#line 10270 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr176;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
st181:
	if ( ++p == pe )
		goto _test_eof181;
case 181:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr178;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
tr1264:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st981;
st981:
	if ( ++p == pe )
		goto _test_eof981;
case 981:
#line 10323 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 66: goto tr1277;
		case 72: goto st167;
		case 80: goto st168;
		case 95: goto st112;
		case 98: goto tr1278;
		case 103: goto tr176;
		case 109: goto tr1284;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
tr1284:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st982;
st982:
	if ( ++p == pe )
		goto _test_eof982;
case 982:
#line 10359 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 47: goto st69;
		case 64: goto st113;
		case 94: goto st52;
		case 95: goto st112;
		case 104: goto tr1285;
		case 112: goto st183;
	}
	if ( (*p) < 52 ) {
		if ( (*p) > 49 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr176;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
tr1285:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st983;
st983:
	if ( ++p == pe )
		goto _test_eof983;
case 983:
#line 10396 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st182;
		case 46: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
st182:
	if ( ++p == pe )
		goto _test_eof182;
case 182:
	switch( (*p) ) {
		case 43: goto st112;
		case 49: goto tr171;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st183:
	if ( ++p == pe )
		goto _test_eof183;
case 183:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 104: goto tr171;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st184:
	if ( ++p == pe )
		goto _test_eof184;
case 184:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 98: goto tr179;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
tr1266:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st984;
st984:
	if ( ++p == pe )
		goto _test_eof984;
case 984:
#line 10491 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 47: goto st44;
		case 64: goto st113;
		case 76: goto tr176;
		case 94: goto st52;
		case 95: goto st112;
		case 98: goto tr1278;
		case 103: goto tr176;
		case 105: goto tr1288;
		case 109: goto tr1279;
		case 110: goto st191;
		case 112: goto st183;
		case 115: goto tr1285;
		case 116: goto st193;
	}
	if ( (*p) < 52 ) {
		if ( (*p) > 49 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr176;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
tr1288:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st985;
st985:
	if ( ++p == pe )
		goto _test_eof985;
case 985:
#line 10535 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 47: goto st69;
		case 64: goto st113;
		case 94: goto st52;
		case 95: goto st112;
		case 99: goto st185;
		case 104: goto tr1285;
		case 108: goto tr1292;
		case 110: goto tr1283;
	}
	if ( (*p) < 52 ) {
		if ( (*p) > 49 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr176;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
st185:
	if ( ++p == pe )
		goto _test_eof185;
case 185:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 114: goto st186;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st186:
	if ( ++p == pe )
		goto _test_eof186;
case 186:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 111: goto st187;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st187:
	if ( ++p == pe )
		goto _test_eof187;
case 187:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto st188;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st188:
	if ( ++p == pe )
		goto _test_eof188;
case 188:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto st189;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st189:
	if ( ++p == pe )
		goto _test_eof189;
case 189:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr184;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
tr1292:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st986;
st986:
	if ( ++p == pe )
		goto _test_eof986;
case 986:
#line 10684 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto st190;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
st190:
	if ( ++p == pe )
		goto _test_eof190;
case 190:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto st187;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st191:
	if ( ++p == pe )
		goto _test_eof191;
case 191:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto st192;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st192:
	if ( ++p == pe )
		goto _test_eof192;
case 192:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 104: goto tr186;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st193:
	if ( ++p == pe )
		goto _test_eof193;
case 193:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 104: goto tr179;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st194:
	if ( ++p == pe )
		goto _test_eof194;
case 194:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto st195;
		case 115: goto tr171;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st195:
	if ( ++p == pe )
		goto _test_eof195;
case 195:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto st196;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st196:
	if ( ++p == pe )
		goto _test_eof196;
case 196:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 111: goto st197;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st197:
	if ( ++p == pe )
		goto _test_eof197;
case 197:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto st198;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st198:
	if ( ++p == pe )
		goto _test_eof198;
case 198:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto st199;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st199:
	if ( ++p == pe )
		goto _test_eof199;
case 199:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr192;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st200:
	if ( ++p == pe )
		goto _test_eof200;
case 200:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 122: goto tr171;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st201:
	if ( ++p == pe )
		goto _test_eof201;
case 201:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto st177;
		case 116: goto tr171;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st202:
	if ( ++p == pe )
		goto _test_eof202;
case 202:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr171;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st203:
	if ( ++p == pe )
		goto _test_eof203;
case 203:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 112: goto st204;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st204:
	if ( ++p == pe )
		goto _test_eof204;
case 204:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto tr170;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
tr1272:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st987;
st987:
	if ( ++p == pe )
		goto _test_eof987;
case 987:
#line 11047 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto st205;
		case 113: goto st206;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1037;
st205:
	if ( ++p == pe )
		goto _test_eof205;
case 205:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr179;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st206:
	if ( ++p == pe )
		goto _test_eof206;
case 206:
	switch( (*p) ) {
		case 32: goto tr84;
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st207;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st207:
	if ( ++p == pe )
		goto _test_eof207;
case 207:
	switch( (*p) ) {
		case 32: goto tr54;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr51;
st208:
	if ( ++p == pe )
		goto _test_eof208;
case 208:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 111: goto st209;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st209:
	if ( ++p == pe )
		goto _test_eof209;
case 209:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr170;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st210:
	if ( ++p == pe )
		goto _test_eof210;
case 210:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr171;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st211:
	if ( ++p == pe )
		goto _test_eof211;
case 211:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 100: goto tr178;
		case 114: goto tr179;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
tr1238:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st988;
st988:
	if ( ++p == pe )
		goto _test_eof988;
case 988:
#line 11231 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto st212;
		case 46: goto st162;
		case 47: goto st104;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1238;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
st212:
	if ( ++p == pe )
		goto _test_eof212;
case 212:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr166;
		} else if ( (*p) >= 65 )
			goto tr166;
	} else
		goto st213;
	goto tr25;
st213:
	if ( ++p == pe )
		goto _test_eof213;
case 213:
	switch( (*p) ) {
		case 43: goto st112;
		case 47: goto st104;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st213;
	goto tr25;
tr927:
#line 1 "NONE"
	{ s.te = p+1;}
#line 42 "src/ragel/rules/main.rl"
	{ s.act = 11;}
	goto st989;
st989:
	if ( ++p == pe )
		goto _test_eof989;
case 989:
#line 11335 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 32: goto st231;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1299;
		case 46: goto st350;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1297;
st214:
	if ( ++p == pe )
		goto _test_eof214;
case 214:
	switch( (*p) ) {
		case 83: goto tr198;
		case 115: goto tr198;
	}
	goto tr0;
st215:
	if ( ++p == pe )
		goto _test_eof215;
case 215:
	switch( (*p) ) {
		case 68: goto tr199;
		case 76: goto st216;
		case 77: goto tr201;
		case 82: goto st217;
		case 86: goto st218;
		case 100: goto tr204;
		case 108: goto st219;
		case 109: goto tr206;
		case 114: goto st220;
		case 118: goto st221;
	}
	goto tr0;
st216:
	if ( ++p == pe )
		goto _test_eof216;
case 216:
	if ( (*p) == 76 )
		goto tr209;
	goto tr0;
st217:
	if ( ++p == pe )
		goto _test_eof217;
case 217:
	if ( (*p) == 69 )
		goto tr210;
	goto tr0;
st218:
	if ( ++p == pe )
		goto _test_eof218;
case 218:
	if ( (*p) == 69 )
		goto tr211;
	goto tr0;
st219:
	if ( ++p == pe )
		goto _test_eof219;
case 219:
	if ( (*p) == 108 )
		goto tr212;
	goto tr0;
st220:
	if ( ++p == pe )
		goto _test_eof220;
case 220:
	if ( (*p) == 101 )
		goto tr213;
	goto tr0;
st221:
	if ( ++p == pe )
		goto _test_eof221;
case 221:
	if ( (*p) == 101 )
		goto tr214;
	goto tr0;
st222:
	if ( ++p == pe )
		goto _test_eof222;
case 222:
	switch( (*p) ) {
		case -128: goto st223;
		case -126: goto st6;
		case -120: goto st7;
	}
	if ( -127 <= (*p) && (*p) <= -65 )
		goto st2;
	goto tr0;
st223:
	if ( ++p == pe )
		goto _test_eof223;
case 223:
	switch( (*p) ) {
		case -103: goto st224;
		case -102: goto tr1;
		case -98: goto tr1;
	}
	if ( (*p) < -106 ) {
		if ( (*p) <= -119 )
			goto tr1;
	} else if ( (*p) > -105 ) {
		if ( -96 <= (*p) && (*p) <= -65 )
			goto tr1;
	} else
		goto tr1;
	goto tr0;
st224:
	if ( ++p == pe )
		goto _test_eof224;
case 224:
	switch( (*p) ) {
		case 68: goto tr217;
		case 76: goto st225;
		case 77: goto tr219;
		case 82: goto st226;
		case 83: goto tr221;
		case 86: goto st227;
		case 100: goto tr223;
		case 108: goto st228;
		case 109: goto tr225;
		case 114: goto st229;
		case 115: goto tr221;
		case 118: goto st230;
	}
	goto tr0;
st225:
	if ( ++p == pe )
		goto _test_eof225;
case 225:
	if ( (*p) == 76 )
		goto tr228;
	goto tr0;
st226:
	if ( ++p == pe )
		goto _test_eof226;
case 226:
	if ( (*p) == 69 )
		goto tr229;
	goto tr0;
st227:
	if ( ++p == pe )
		goto _test_eof227;
case 227:
	if ( (*p) == 69 )
		goto tr230;
	goto tr0;
st228:
	if ( ++p == pe )
		goto _test_eof228;
case 228:
	if ( (*p) == 108 )
		goto tr231;
	goto tr0;
st229:
	if ( ++p == pe )
		goto _test_eof229;
case 229:
	if ( (*p) == 101 )
		goto tr232;
	goto tr0;
st230:
	if ( ++p == pe )
		goto _test_eof230;
case 230:
	if ( (*p) == 101 )
		goto tr233;
	goto tr0;
st231:
	if ( ++p == pe )
		goto _test_eof231;
case 231:
	if ( (*p) == 45 )
		goto st232;
	goto tr234;
st232:
	if ( ++p == pe )
		goto _test_eof232;
case 232:
	if ( (*p) == 32 )
		goto st233;
	goto tr234;
st233:
	if ( ++p == pe )
		goto _test_eof233;
case 233:
	if ( (*p) == 45 )
		goto st234;
	goto tr234;
st234:
	if ( ++p == pe )
		goto _test_eof234;
case 234:
	if ( (*p) == 32 )
		goto st235;
	goto tr234;
st235:
	if ( ++p == pe )
		goto _test_eof235;
case 235:
	if ( (*p) == 45 )
		goto tr239;
	goto tr0;
tr239:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st990;
st990:
	if ( ++p == pe )
		goto _test_eof990;
case 990:
#line 11572 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 32 )
		goto st235;
	goto tr1005;
st236:
	if ( ++p == pe )
		goto _test_eof236;
case 236:
	switch( (*p) ) {
		case 35: goto st237;
		case 97: goto st270;
		case 114: goto st311;
	}
	goto tr0;
st237:
	if ( ++p == pe )
		goto _test_eof237;
case 237:
	switch( (*p) ) {
		case 51: goto st238;
		case 56: goto st247;
		case 88: goto st258;
		case 120: goto st258;
	}
	goto tr0;
st238:
	if ( ++p == pe )
		goto _test_eof238;
case 238:
	if ( (*p) == 57 )
		goto st239;
	goto tr0;
st239:
	if ( ++p == pe )
		goto _test_eof239;
case 239:
	if ( (*p) == 59 )
		goto st240;
	goto tr0;
st240:
	if ( ++p == pe )
		goto _test_eof240;
case 240:
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto st241;
		case 77: goto tr250;
		case 82: goto st242;
		case 83: goto tr252;
		case 86: goto st243;
		case 100: goto tr254;
		case 108: goto st244;
		case 109: goto tr256;
		case 114: goto st245;
		case 115: goto tr252;
		case 118: goto st246;
	}
	goto tr0;
st241:
	if ( ++p == pe )
		goto _test_eof241;
case 241:
	if ( (*p) == 76 )
		goto tr259;
	goto tr0;
st242:
	if ( ++p == pe )
		goto _test_eof242;
case 242:
	if ( (*p) == 69 )
		goto tr260;
	goto tr0;
st243:
	if ( ++p == pe )
		goto _test_eof243;
case 243:
	if ( (*p) == 69 )
		goto tr261;
	goto tr0;
st244:
	if ( ++p == pe )
		goto _test_eof244;
case 244:
	if ( (*p) == 108 )
		goto tr262;
	goto tr0;
st245:
	if ( ++p == pe )
		goto _test_eof245;
case 245:
	if ( (*p) == 101 )
		goto tr263;
	goto tr0;
st246:
	if ( ++p == pe )
		goto _test_eof246;
case 246:
	if ( (*p) == 101 )
		goto tr264;
	goto tr0;
st247:
	if ( ++p == pe )
		goto _test_eof247;
case 247:
	if ( (*p) == 50 )
		goto st248;
	goto tr0;
st248:
	if ( ++p == pe )
		goto _test_eof248;
case 248:
	if ( (*p) == 49 )
		goto st249;
	goto tr0;
st249:
	if ( ++p == pe )
		goto _test_eof249;
case 249:
	if ( (*p) == 55 )
		goto st250;
	goto tr0;
st250:
	if ( ++p == pe )
		goto _test_eof250;
case 250:
	if ( (*p) == 59 )
		goto st251;
	goto tr0;
st251:
	if ( ++p == pe )
		goto _test_eof251;
case 251:
	switch( (*p) ) {
		case 68: goto tr269;
		case 76: goto st252;
		case 77: goto tr271;
		case 82: goto st253;
		case 83: goto tr273;
		case 86: goto st254;
		case 100: goto tr275;
		case 108: goto st255;
		case 109: goto tr277;
		case 114: goto st256;
		case 115: goto tr273;
		case 118: goto st257;
	}
	goto tr0;
st252:
	if ( ++p == pe )
		goto _test_eof252;
case 252:
	if ( (*p) == 76 )
		goto tr280;
	goto tr0;
st253:
	if ( ++p == pe )
		goto _test_eof253;
case 253:
	if ( (*p) == 69 )
		goto tr281;
	goto tr0;
st254:
	if ( ++p == pe )
		goto _test_eof254;
case 254:
	if ( (*p) == 69 )
		goto tr282;
	goto tr0;
st255:
	if ( ++p == pe )
		goto _test_eof255;
case 255:
	if ( (*p) == 108 )
		goto tr283;
	goto tr0;
st256:
	if ( ++p == pe )
		goto _test_eof256;
case 256:
	if ( (*p) == 101 )
		goto tr284;
	goto tr0;
st257:
	if ( ++p == pe )
		goto _test_eof257;
case 257:
	if ( (*p) == 101 )
		goto tr285;
	goto tr0;
st258:
	if ( ++p == pe )
		goto _test_eof258;
case 258:
	if ( (*p) == 50 )
		goto st259;
	goto tr0;
st259:
	if ( ++p == pe )
		goto _test_eof259;
case 259:
	if ( (*p) == 48 )
		goto st260;
	goto tr0;
st260:
	if ( ++p == pe )
		goto _test_eof260;
case 260:
	if ( (*p) == 49 )
		goto st261;
	goto tr0;
st261:
	if ( ++p == pe )
		goto _test_eof261;
case 261:
	if ( (*p) == 57 )
		goto st262;
	goto tr0;
st262:
	if ( ++p == pe )
		goto _test_eof262;
case 262:
	if ( (*p) == 59 )
		goto st263;
	goto tr0;
st263:
	if ( ++p == pe )
		goto _test_eof263;
case 263:
	switch( (*p) ) {
		case 68: goto tr291;
		case 76: goto st264;
		case 77: goto tr293;
		case 82: goto st265;
		case 83: goto tr295;
		case 86: goto st266;
		case 100: goto tr297;
		case 108: goto st267;
		case 109: goto tr299;
		case 114: goto st268;
		case 115: goto tr295;
		case 118: goto st269;
	}
	goto tr0;
st264:
	if ( ++p == pe )
		goto _test_eof264;
case 264:
	if ( (*p) == 76 )
		goto tr302;
	goto tr0;
st265:
	if ( ++p == pe )
		goto _test_eof265;
case 265:
	if ( (*p) == 69 )
		goto tr303;
	goto tr0;
st266:
	if ( ++p == pe )
		goto _test_eof266;
case 266:
	if ( (*p) == 69 )
		goto tr304;
	goto tr0;
st267:
	if ( ++p == pe )
		goto _test_eof267;
case 267:
	if ( (*p) == 108 )
		goto tr305;
	goto tr0;
st268:
	if ( ++p == pe )
		goto _test_eof268;
case 268:
	if ( (*p) == 101 )
		goto tr306;
	goto tr0;
st269:
	if ( ++p == pe )
		goto _test_eof269;
case 269:
	if ( (*p) == 101 )
		goto tr307;
	goto tr0;
st270:
	if ( ++p == pe )
		goto _test_eof270;
case 270:
	switch( (*p) ) {
		case 109: goto st271;
		case 112: goto st317;
	}
	goto tr0;
st271:
	if ( ++p == pe )
		goto _test_eof271;
case 271:
	if ( (*p) == 112 )
		goto st272;
	goto tr0;
st272:
	if ( ++p == pe )
		goto _test_eof272;
case 272:
	if ( (*p) == 59 )
		goto tr311;
	goto tr0;
tr311:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st991;
tr365:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 12 "src/ragel/rules/contractions.rl"
	{n2="'D";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st991;
tr367:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 13 "src/ragel/rules/contractions.rl"
	{n2="'M";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st991;
tr369:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 67 "src/ragel/rules/main.rl"
	{ s.act = 27;}
	goto st991;
tr371:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 9 "src/ragel/rules/contractions.rl"
	{n2="'d";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st991;
tr373:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 10 "src/ragel/rules/contractions.rl"
	{n2="'m";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st991;
tr729:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st991;
tr684:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 34 "src/ragel/rules/contractions.rl"
	{n2="N'T";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 24;}
	goto st991;
tr724:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 33 "src/ragel/rules/contractions.rl"
	{n2="n't";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 24;}
	goto st991;
tr765:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 9 "src/ragel/rules/contractions.rl"
	{n2="'d";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
#line 6 "src/ragel/rules/contractions.rl"
	{n1="continued";}
	goto st991;
tr767:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/rules/contractions.rl"
	{n1="government";}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st991;
tr774:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 9 "src/ragel/rules/contractions.rl"
	{n2="'d";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
#line 6 "src/ragel/rules/contractions.rl"
	{n1="required";}
	goto st991;
st991:
	if ( ++p == pe )
		goto _test_eof991;
case 991:
#line 11998 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr0;
st273:
	if ( ++p == pe )
		goto _test_eof273;
case 273:
	switch( (*p) ) {
		case 68: goto tr199;
		case 76: goto st274;
		case 77: goto tr201;
		case 82: goto st275;
		case 83: goto tr198;
		case 86: goto st276;
		case 100: goto tr204;
		case 108: goto st277;
		case 109: goto tr206;
		case 114: goto st278;
		case 115: goto tr198;
		case 118: goto st279;
	}
	goto tr0;
st274:
	if ( ++p == pe )
		goto _test_eof274;
case 274:
	if ( (*p) == 76 )
		goto tr318;
	goto tr0;
st275:
	if ( ++p == pe )
		goto _test_eof275;
case 275:
	if ( (*p) == 69 )
		goto tr319;
	goto tr0;
st276:
	if ( ++p == pe )
		goto _test_eof276;
case 276:
	if ( (*p) == 69 )
		goto tr320;
	goto tr0;
st277:
	if ( ++p == pe )
		goto _test_eof277;
case 277:
	if ( (*p) == 108 )
		goto tr321;
	goto tr0;
st278:
	if ( ++p == pe )
		goto _test_eof278;
case 278:
	if ( (*p) == 101 )
		goto tr322;
	goto tr0;
st279:
	if ( ++p == pe )
		goto _test_eof279;
case 279:
	if ( (*p) == 101 )
		goto tr323;
	goto tr0;
tr1302:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st992;
st992:
	if ( ++p == pe )
		goto _test_eof992;
case 992:
#line 12098 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto st280;
		case 46: goto st281;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
st280:
	if ( ++p == pe )
		goto _test_eof280;
case 280:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto st280;
		case 46: goto st281;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr10;
st281:
	if ( ++p == pe )
		goto _test_eof281;
case 281:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
	}
	goto tr0;
st282:
	if ( ++p == pe )
		goto _test_eof282;
case 282:
	if ( (*p) == -128 )
		goto st283;
	goto tr0;
st283:
	if ( ++p == pe )
		goto _test_eof283;
case 283:
	if ( (*p) == -103 )
		goto st284;
	goto tr0;
st284:
	if ( ++p == pe )
		goto _test_eof284;
case 284:
	switch( (*p) ) {
		case 83: goto tr221;
		case 115: goto tr221;
	}
	goto tr0;
st285:
	if ( ++p == pe )
		goto _test_eof285;
case 285:
	switch( (*p) ) {
		case 35: goto st286;
		case 97: goto st301;
		case 114: goto st306;
	}
	goto tr0;
st286:
	if ( ++p == pe )
		goto _test_eof286;
case 286:
	switch( (*p) ) {
		case 51: goto st287;
		case 56: goto st290;
		case 88: goto st295;
		case 120: goto st295;
	}
	goto tr0;
st287:
	if ( ++p == pe )
		goto _test_eof287;
case 287:
	if ( (*p) == 57 )
		goto st288;
	goto tr0;
st288:
	if ( ++p == pe )
		goto _test_eof288;
case 288:
	if ( (*p) == 59 )
		goto st289;
	goto tr0;
st289:
	if ( ++p == pe )
		goto _test_eof289;
case 289:
	switch( (*p) ) {
		case 83: goto tr252;
		case 115: goto tr252;
	}
	goto tr0;
st290:
	if ( ++p == pe )
		goto _test_eof290;
case 290:
	if ( (*p) == 50 )
		goto st291;
	goto tr0;
st291:
	if ( ++p == pe )
		goto _test_eof291;
case 291:
	if ( (*p) == 49 )
		goto st292;
	goto tr0;
st292:
	if ( ++p == pe )
		goto _test_eof292;
case 292:
	if ( (*p) == 55 )
		goto st293;
	goto tr0;
st293:
	if ( ++p == pe )
		goto _test_eof293;
case 293:
	if ( (*p) == 59 )
		goto st294;
	goto tr0;
st294:
	if ( ++p == pe )
		goto _test_eof294;
case 294:
	switch( (*p) ) {
		case 83: goto tr273;
		case 115: goto tr273;
	}
	goto tr0;
st295:
	if ( ++p == pe )
		goto _test_eof295;
case 295:
	if ( (*p) == 50 )
		goto st296;
	goto tr0;
st296:
	if ( ++p == pe )
		goto _test_eof296;
case 296:
	if ( (*p) == 48 )
		goto st297;
	goto tr0;
st297:
	if ( ++p == pe )
		goto _test_eof297;
case 297:
	if ( (*p) == 49 )
		goto st298;
	goto tr0;
st298:
	if ( ++p == pe )
		goto _test_eof298;
case 298:
	if ( (*p) == 57 )
		goto st299;
	goto tr0;
st299:
	if ( ++p == pe )
		goto _test_eof299;
case 299:
	if ( (*p) == 59 )
		goto st300;
	goto tr0;
st300:
	if ( ++p == pe )
		goto _test_eof300;
case 300:
	switch( (*p) ) {
		case 83: goto tr295;
		case 115: goto tr295;
	}
	goto tr0;
st301:
	if ( ++p == pe )
		goto _test_eof301;
case 301:
	if ( (*p) == 112 )
		goto st302;
	goto tr0;
st302:
	if ( ++p == pe )
		goto _test_eof302;
case 302:
	if ( (*p) == 111 )
		goto st303;
	goto tr0;
st303:
	if ( ++p == pe )
		goto _test_eof303;
case 303:
	if ( (*p) == 115 )
		goto st304;
	goto tr0;
st304:
	if ( ++p == pe )
		goto _test_eof304;
case 304:
	if ( (*p) == 59 )
		goto st305;
	goto tr0;
st305:
	if ( ++p == pe )
		goto _test_eof305;
case 305:
	switch( (*p) ) {
		case 83: goto tr358;
		case 115: goto tr358;
	}
	goto tr0;
st306:
	if ( ++p == pe )
		goto _test_eof306;
case 306:
	if ( (*p) == 115 )
		goto st307;
	goto tr0;
st307:
	if ( ++p == pe )
		goto _test_eof307;
case 307:
	if ( (*p) == 113 )
		goto st308;
	goto tr0;
st308:
	if ( ++p == pe )
		goto _test_eof308;
case 308:
	if ( (*p) == 117 )
		goto st309;
	goto tr0;
st309:
	if ( ++p == pe )
		goto _test_eof309;
case 309:
	if ( (*p) == 111 )
		goto st293;
	goto tr0;
tr331:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st993;
tr1306:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 19 "src/ragel/rules/contractions.rl"
	{n2="'LL";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st993;
tr1307:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 20 "src/ragel/rules/contractions.rl"
	{n2="'RE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st993;
tr1308:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 21 "src/ragel/rules/contractions.rl"
	{n2="'VE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st993;
st993:
	if ( ++p == pe )
		goto _test_eof993;
case 993:
#line 12423 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr0;
st310:
	if ( ++p == pe )
		goto _test_eof310;
case 310:
	switch( (*p) ) {
		case 35: goto st237;
		case 97: goto st270;
		case 114: goto st311;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr311;
	goto tr0;
st311:
	if ( ++p == pe )
		goto _test_eof311;
case 311:
	if ( (*p) == 115 )
		goto st312;
	goto tr0;
st312:
	if ( ++p == pe )
		goto _test_eof312;
case 312:
	if ( (*p) == 113 )
		goto st313;
	goto tr0;
st313:
	if ( ++p == pe )
		goto _test_eof313;
case 313:
	if ( (*p) == 117 )
		goto st314;
	goto tr0;
st314:
	if ( ++p == pe )
		goto _test_eof314;
case 314:
	if ( (*p) == 111 )
		goto st250;
	goto tr0;
st315:
	if ( ++p == pe )
		goto _test_eof315;
case 315:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 100: goto tr371;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr0;
tr366:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st994;
st994:
	if ( ++p == pe )
		goto _test_eof994;
case 994:
#line 12526 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 76: goto tr1306;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr332:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st995;
tr1309:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 15 "src/ragel/rules/contractions.rl"
	{n2="'ll";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st995;
tr1310:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 16 "src/ragel/rules/contractions.rl"
	{n2="'re";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st995;
tr1311:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 17 "src/ragel/rules/contractions.rl"
	{n2="'ve";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st995;
tr1713:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st995;
tr2071:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/rules/contractions.rl"
	{n1="madame";}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st995;
tr2078:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/rules/contractions.rl"
	{n1="over";}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st995;
tr2124:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/rules/contractions.rl"
	{n1="weekend";}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st995;
st995:
	if ( ++p == pe )
		goto _test_eof995;
case 995:
#line 12626 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr0;
st316:
	if ( ++p == pe )
		goto _test_eof316;
case 316:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr0;
tr368:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st996;
st996:
	if ( ++p == pe )
		goto _test_eof996;
case 996:
#line 12681 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 69: goto tr1307;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr370:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st997;
st997:
	if ( ++p == pe )
		goto _test_eof997;
case 997:
#line 12721 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 69: goto tr1308;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr372:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st998;
tr690:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st998;
tr769:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/rules/contractions.rl"
	{n1="international";}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st998;
tr771:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/rules/contractions.rl"
	{n1="national";}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st998;
st998:
	if ( ++p == pe )
		goto _test_eof998;
case 998:
#line 12783 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 108: goto tr1309;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr0;
tr374:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st999;
st999:
	if ( ++p == pe )
		goto _test_eof999;
case 999:
#line 12823 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 101: goto tr1310;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr375:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1000;
st1000:
	if ( ++p == pe )
		goto _test_eof1000;
case 1000:
#line 12863 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 101: goto tr1311;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
st317:
	if ( ++p == pe )
		goto _test_eof317;
case 317:
	if ( (*p) == 111 )
		goto st318;
	goto tr0;
st318:
	if ( ++p == pe )
		goto _test_eof318;
case 318:
	if ( (*p) == 115 )
		goto st319;
	goto tr0;
st319:
	if ( ++p == pe )
		goto _test_eof319;
case 319:
	if ( (*p) == 59 )
		goto st320;
	goto tr0;
st320:
	if ( ++p == pe )
		goto _test_eof320;
case 320:
	switch( (*p) ) {
		case 68: goto tr379;
		case 76: goto st321;
		case 77: goto tr381;
		case 82: goto st322;
		case 83: goto tr358;
		case 86: goto st323;
		case 100: goto tr384;
		case 108: goto st324;
		case 109: goto tr386;
		case 114: goto st325;
		case 115: goto tr358;
		case 118: goto st326;
	}
	goto tr0;
st321:
	if ( ++p == pe )
		goto _test_eof321;
case 321:
	if ( (*p) == 76 )
		goto tr389;
	goto tr0;
st322:
	if ( ++p == pe )
		goto _test_eof322;
case 322:
	if ( (*p) == 69 )
		goto tr390;
	goto tr0;
st323:
	if ( ++p == pe )
		goto _test_eof323;
case 323:
	if ( (*p) == 69 )
		goto tr391;
	goto tr0;
st324:
	if ( ++p == pe )
		goto _test_eof324;
case 324:
	if ( (*p) == 108 )
		goto tr392;
	goto tr0;
st325:
	if ( ++p == pe )
		goto _test_eof325;
case 325:
	if ( (*p) == 101 )
		goto tr393;
	goto tr0;
st326:
	if ( ++p == pe )
		goto _test_eof326;
case 326:
	if ( (*p) == 101 )
		goto tr394;
	goto tr0;
tr1299:
#line 1 "NONE"
	{ s.te = p+1;}
#line 42 "src/ragel/rules/main.rl"
	{ s.act = 11;}
	goto st1001;
st1001:
	if ( ++p == pe )
		goto _test_eof1001;
case 1001:
#line 12985 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1312;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st346;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st349;
	} else
		goto st347;
	goto tr1297;
st327:
	if ( ++p == pe )
		goto _test_eof327;
case 327:
	if ( (*p) == -128 )
		goto st328;
	goto tr0;
st328:
	if ( ++p == pe )
		goto _test_eof328;
case 328:
	if ( (*p) == -103 )
		goto st224;
	goto tr0;
st329:
	if ( ++p == pe )
		goto _test_eof329;
case 329:
	switch( (*p) ) {
		case 35: goto st237;
		case 97: goto st330;
		case 114: goto st311;
	}
	goto tr0;
st330:
	if ( ++p == pe )
		goto _test_eof330;
case 330:
	switch( (*p) ) {
		case 109: goto st331;
		case 112: goto st317;
	}
	goto tr0;
st331:
	if ( ++p == pe )
		goto _test_eof331;
case 331:
	if ( (*p) == 112 )
		goto st332;
	goto tr0;
st332:
	if ( ++p == pe )
		goto _test_eof332;
case 332:
	if ( (*p) == 59 )
		goto st333;
	goto tr0;
st333:
	if ( ++p == pe )
		goto _test_eof333;
case 333:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 45: goto st333;
		case 46: goto st281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr0;
st334:
	if ( ++p == pe )
		goto _test_eof334;
case 334:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st335;
		case 39: goto st336;
		case 45: goto st333;
		case 46: goto st338;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr0;
st335:
	if ( ++p == pe )
		goto _test_eof335;
case 335:
	switch( (*p) ) {
		case 35: goto st237;
		case 97: goto st330;
		case 114: goto st311;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto st333;
	goto tr0;
st336:
	if ( ++p == pe )
		goto _test_eof336;
case 336:
	switch( (*p) ) {
		case 68: goto tr407;
		case 76: goto st339;
		case 77: goto tr409;
		case 82: goto st340;
		case 83: goto tr411;
		case 86: goto st341;
		case 100: goto tr413;
		case 108: goto st342;
		case 109: goto tr415;
		case 114: goto st343;
		case 115: goto tr411;
		case 118: goto st344;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st333;
	} else if ( (*p) >= 65 )
		goto st333;
	goto tr0;
tr407:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 12 "src/ragel/rules/contractions.rl"
	{n2="'D";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1002;
tr409:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 13 "src/ragel/rules/contractions.rl"
	{n2="'M";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1002;
tr411:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 30 "src/ragel/rules/quotes.rl"
	{n2="'s";}
#line 67 "src/ragel/rules/main.rl"
	{ s.act = 27;}
	goto st1002;
tr413:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 9 "src/ragel/rules/contractions.rl"
	{n2="'d";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1002;
tr415:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 10 "src/ragel/rules/contractions.rl"
	{n2="'m";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1002;
st1002:
	if ( ++p == pe )
		goto _test_eof1002;
case 1002:
#line 13188 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 45: goto st333;
		case 46: goto st281;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr0;
st337:
	if ( ++p == pe )
		goto _test_eof337;
case 337:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st336;
		case 45: goto st333;
		case 46: goto st338;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr0;
st338:
	if ( ++p == pe )
		goto _test_eof338;
case 338:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st333;
	} else if ( (*p) >= 65 )
		goto st333;
	goto tr0;
st339:
	if ( ++p == pe )
		goto _test_eof339;
case 339:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 45: goto st333;
		case 46: goto st281;
		case 76: goto tr418;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr0;
tr418:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 19 "src/ragel/rules/contractions.rl"
	{n2="'LL";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1003;
tr419:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 20 "src/ragel/rules/contractions.rl"
	{n2="'RE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1003;
tr420:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 21 "src/ragel/rules/contractions.rl"
	{n2="'VE";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1003;
st1003:
	if ( ++p == pe )
		goto _test_eof1003;
case 1003:
#line 13302 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st335;
		case 39: goto st336;
		case 45: goto st333;
		case 46: goto st338;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr1313;
st340:
	if ( ++p == pe )
		goto _test_eof340;
case 340:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 45: goto st333;
		case 46: goto st281;
		case 69: goto tr419;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr0;
st341:
	if ( ++p == pe )
		goto _test_eof341;
case 341:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 45: goto st333;
		case 46: goto st281;
		case 69: goto tr420;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr0;
st342:
	if ( ++p == pe )
		goto _test_eof342;
case 342:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 45: goto st333;
		case 46: goto st281;
		case 108: goto tr421;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr0;
tr421:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 15 "src/ragel/rules/contractions.rl"
	{n2="'ll";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1004;
tr422:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 16 "src/ragel/rules/contractions.rl"
	{n2="'re";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1004;
tr423:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 17 "src/ragel/rules/contractions.rl"
	{n2="'ve";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1004;
st1004:
	if ( ++p == pe )
		goto _test_eof1004;
case 1004:
#line 13424 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st336;
		case 45: goto st333;
		case 46: goto st338;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr1313;
st343:
	if ( ++p == pe )
		goto _test_eof343;
case 343:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 45: goto st333;
		case 46: goto st281;
		case 101: goto tr422;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr0;
st344:
	if ( ++p == pe )
		goto _test_eof344;
case 344:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 45: goto st333;
		case 46: goto st281;
		case 101: goto tr423;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st333;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st337;
	} else
		goto st334;
	goto tr0;
tr1314:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1005;
tr1312:
#line 1 "NONE"
	{ s.te = p+1;}
#line 42 "src/ragel/rules/main.rl"
	{ s.act = 11;}
	goto st1005;
st1005:
	if ( ++p == pe )
		goto _test_eof1005;
case 1005:
#line 13505 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1314;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st346;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st349;
	} else
		goto st347;
	goto tr0;
st345:
	if ( ++p == pe )
		goto _test_eof345;
case 345:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr0;
st346:
	if ( ++p == pe )
		goto _test_eof346;
case 346:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st346;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st346;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st349;
	} else
		goto st347;
	goto tr0;
st347:
	if ( ++p == pe )
		goto _test_eof347;
case 347:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st335;
		case 39: goto st336;
		case 43: goto st112;
		case 45: goto st346;
		case 46: goto st348;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st346;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st349;
	} else
		goto st347;
	goto tr0;
st348:
	if ( ++p == pe )
		goto _test_eof348;
case 348:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st346;
		} else if ( (*p) >= 65 )
			goto st346;
	} else
		goto st112;
	goto tr0;
st349:
	if ( ++p == pe )
		goto _test_eof349;
case 349:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -30: goto st327;
		case 38: goto st329;
		case 39: goto st336;
		case 43: goto st112;
		case 45: goto st346;
		case 46: goto st348;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st346;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st349;
	} else
		goto st347;
	goto tr0;
st350:
	if ( ++p == pe )
		goto _test_eof350;
case 350:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr165;
	goto tr0;
tr1301:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1006;
st1006:
	if ( ++p == pe )
		goto _test_eof1006;
case 1006:
#line 13687 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st350;
		case 47: goto st104;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
st351:
	if ( ++p == pe )
		goto _test_eof351;
case 351:
	switch( (*p) ) {
		case 35: goto st352;
		case 97: goto st270;
		case 99: goto st36;
		case 100: goto st39;
		case 114: goto st311;
	}
	goto tr0;
st352:
	if ( ++p == pe )
		goto _test_eof352;
case 352:
	switch( (*p) ) {
		case 49: goto st29;
		case 51: goto st238;
		case 56: goto st247;
		case 88: goto st353;
		case 120: goto st353;
	}
	goto tr0;
st353:
	if ( ++p == pe )
		goto _test_eof353;
case 353:
	switch( (*p) ) {
		case 50: goto st259;
		case 65: goto st30;
		case 66: goto st34;
		case 97: goto st30;
		case 98: goto st34;
	}
	goto tr0;
tr1316:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1007;
st1007:
	if ( ++p == pe )
		goto _test_eof1007;
case 1007:
#line 13797 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto st8;
	goto tr997;
st354:
	if ( ++p == pe )
		goto _test_eof354;
case 354:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr10;
tr432:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1008;
tr1379:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1008;
tr1367:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1008;
tr1435:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1008;
st1008:
	if ( ++p == pe )
		goto _test_eof1008;
case 1008:
#line 13896 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr0;
tr1347:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1009;
st1009:
	if ( ++p == pe )
		goto _test_eof1009;
case 1009:
#line 13938 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr433:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1010;
tr1369:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1010;
tr1361:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1010;
tr1317:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1010;
tr1473:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 58 "src/ragel/rules/date_time.rl"
	{n2="am";}
#line 71 "src/ragel/rules/main.rl"
	{ s.act = 30;}
	goto st1010;
tr1477:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 59 "src/ragel/rules/date_time.rl"
	{n2="pm";}
#line 71 "src/ragel/rules/main.rl"
	{ s.act = 30;}
	goto st1010;
st1010:
	if ( ++p == pe )
		goto _test_eof1010;
case 1010:
#line 14024 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr0;
st355:
	if ( ++p == pe )
		goto _test_eof355;
case 355:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr432;
		} else if ( (*p) >= 65 )
			goto tr432;
	} else
		goto st112;
	goto tr0;
tr434:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1011;
tr1356:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1011;
tr1355:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1011;
tr1364:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1011;
tr1390:
#line 1 "NONE"
	{ s.te = p+1;}
#line 13 "src/ragel/actions.rl"
	{ s.suffix += 8; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1011;
tr1399:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "src/ragel/actions.rl"
	{ s.suffix += 7; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1011;
tr1480:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 58 "src/ragel/rules/date_time.rl"
	{n2="am";}
#line 71 "src/ragel/rules/main.rl"
	{ s.act = 30;}
	goto st1011;
tr1482:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 59 "src/ragel/rules/date_time.rl"
	{n2="pm";}
#line 71 "src/ragel/rules/main.rl"
	{ s.act = 30;}
	goto st1011;
st1011:
	if ( ++p == pe )
		goto _test_eof1011;
case 1011:
#line 14151 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr0;
tr1344:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1012;
st1012:
	if ( ++p == pe )
		goto _test_eof1012;
case 1012:
#line 14193 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 47: goto st104;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1345:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 32;}
	goto st1013;
st1013:
	if ( ++p == pe )
		goto _test_eof1013;
case 1013:
#line 14236 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1349;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto st8;
	goto tr1036;
tr1349:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1014;
st1014:
	if ( ++p == pe )
		goto _test_eof1014;
case 1014:
#line 14278 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto st8;
	goto tr997;
tr1346:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 32;}
	goto st1015;
st1015:
	if ( ++p == pe )
		goto _test_eof1015;
case 1015:
#line 14320 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1349;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto st8;
	goto tr1036;
tr1318:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1016;
st1016:
	if ( ++p == pe )
		goto _test_eof1016;
case 1016:
#line 14362 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 66: goto tr1350;
		case 72: goto tr1351;
		case 95: goto st112;
		case 98: goto tr1352;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1350:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1017;
tr1358:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1017;
st1017:
	if ( ++p == pe )
		goto _test_eof1017;
case 1017:
#line 14417 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 47: goto st44;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto tr1353;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1353:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1018;
st1018:
	if ( ++p == pe )
		goto _test_eof1018;
case 1018:
#line 14461 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1354;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1354:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1019;
tr1362:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1019;
st1019:
	if ( ++p == pe )
		goto _test_eof1019;
case 1019:
#line 14514 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 47: goto st44;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1351:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1020;
st1020:
	if ( ++p == pe )
		goto _test_eof1020;
case 1020:
#line 14557 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 122: goto tr1355;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1352:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1021;
tr1359:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1021;
st1021:
	if ( ++p == pe )
		goto _test_eof1021;
case 1021:
#line 14610 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 47: goto st44;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto tr1353;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1319:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1022;
st1022:
	if ( ++p == pe )
		goto _test_eof1022;
case 1022:
#line 14654 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 122: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1320:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1023;
st1023:
	if ( ++p == pe )
		goto _test_eof1023;
case 1023:
#line 14699 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 66: goto tr1357;
		case 72: goto tr1319;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1357:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1024;
st1024:
	if ( ++p == pe )
		goto _test_eof1024;
case 1024:
#line 14745 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 47: goto st44;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1321:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1025;
st1025:
	if ( ++p == pe )
		goto _test_eof1025;
case 1025:
#line 14790 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 66: goto tr1358;
		case 72: goto tr1319;
		case 95: goto st112;
		case 98: goto tr1359;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1322:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1026;
st1026:
	if ( ++p == pe )
		goto _test_eof1026;
case 1026:
#line 14835 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1323:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1027;
st1027:
	if ( ++p == pe )
		goto _test_eof1027;
case 1027:
#line 14878 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 80: goto tr1360;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1360:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1028;
st1028:
	if ( ++p == pe )
		goto _test_eof1028;
case 1028:
#line 14921 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 77: goto tr1361;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1324:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1029;
st1029:
	if ( ++p == pe )
		goto _test_eof1029;
case 1029:
#line 14966 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 66: goto tr1357;
		case 95: goto st112;
		case 98: goto tr1362;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1325:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1030;
st1030:
	if ( ++p == pe )
		goto _test_eof1030;
case 1030:
#line 15010 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 112: goto tr1363;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1363:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1031;
st1031:
	if ( ++p == pe )
		goto _test_eof1031;
case 1031:
#line 15053 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1355;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1326:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1032;
st1032:
	if ( ++p == pe )
		goto _test_eof1032;
case 1032:
#line 15098 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1364;
		case 109: goto tr1365;
		case 117: goto tr1366;
		case 119: goto tr1338;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1372:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1033;
tr1365:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1033;
st1033:
	if ( ++p == pe )
		goto _test_eof1033;
case 1033:
#line 15154 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 94: goto st52;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1367;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr434;
			} else if ( (*p) >= 65 )
				goto tr433;
		} else
			goto tr432;
	} else
		goto tr432;
	goto tr1037;
tr1366:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1034;
st1034:
	if ( ++p == pe )
		goto _test_eof1034;
case 1034:
#line 15203 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 32: goto tr54;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st356;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
st356:
	if ( ++p == pe )
		goto _test_eof356;
case 356:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 32: goto tr61;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr432;
		} else if ( (*p) >= 65 )
			goto tr432;
	} else
		goto st112;
	goto tr10;
tr1338:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1035;
st1035:
	if ( ++p == pe )
		goto _test_eof1035;
case 1035:
#line 15272 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1327:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1036;
st1036:
	if ( ++p == pe )
		goto _test_eof1036;
case 1036:
#line 15315 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 66: goto tr1369;
		case 95: goto st112;
		case 98: goto tr1356;
		case 112: goto tr1370;
		case 114: goto tr1364;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1370:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1037;
st1037:
	if ( ++p == pe )
		goto _test_eof1037;
case 1037:
#line 15361 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto tr1355;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1328:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1038;
st1038:
	if ( ++p == pe )
		goto _test_eof1038;
case 1038:
#line 15404 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1371;
		case 116: goto tr1372;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1371:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1039;
st1039:
	if ( ++p == pe )
		goto _test_eof1039;
case 1039:
#line 15448 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 32: goto st65;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1329:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1040;
st1040:
	if ( ++p == pe )
		goto _test_eof1040;
case 1040:
#line 15493 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1373;
		case 98: goto tr1359;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 99 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1373:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1041;
st1041:
	if ( ++p == pe )
		goto _test_eof1041;
case 1041:
#line 15537 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1330:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1042;
st1042:
	if ( ++p == pe )
		goto _test_eof1042;
case 1042:
#line 15582 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 80: goto tr1322;
		case 95: goto st112;
		case 114: goto tr1374;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1380:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1043;
tr1393:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/actions.rl"
	{ s.suffix += 3; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1043;
tr1374:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1043;
st1043:
	if ( ++p == pe )
		goto _test_eof1043;
case 1043:
#line 15644 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1364;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1331:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1044;
st1044:
	if ( ++p == pe )
		goto _test_eof1044;
case 1044:
#line 15687 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1372;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1332:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1045;
st1045:
	if ( ++p == pe )
		goto _test_eof1045;
case 1045:
#line 15732 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 66: goto tr1357;
		case 72: goto tr1319;
		case 80: goto tr1322;
		case 95: goto st112;
		case 98: goto tr1359;
		case 103: goto tr1364;
		case 109: goto tr1375;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1375:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1046;
st1046:
	if ( ++p == pe )
		goto _test_eof1046;
case 1046:
#line 15782 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 47: goto st69;
		case 64: goto st113;
		case 94: goto st52;
		case 95: goto st112;
		case 104: goto tr1376;
		case 112: goto tr1377;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1367;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr434;
			} else if ( (*p) >= 65 )
				goto tr433;
		} else
			goto tr432;
	} else
		goto tr432;
	goto tr1037;
tr1376:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1047;
st1047:
	if ( ++p == pe )
		goto _test_eof1047;
case 1047:
#line 15836 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1378;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1378:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1048;
st1048:
	if ( ++p == pe )
		goto _test_eof1048;
case 1048:
#line 15878 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 49: goto tr1379;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1377:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1049;
st1049:
	if ( ++p == pe )
		goto _test_eof1049;
case 1049:
#line 15921 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 104: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1333:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1050;
st1050:
	if ( ++p == pe )
		goto _test_eof1050;
case 1050:
#line 15964 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 98: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1334:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1051;
st1051:
	if ( ++p == pe )
		goto _test_eof1051;
case 1051:
#line 16009 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 47: goto st44;
		case 64: goto st113;
		case 76: goto tr1317;
		case 94: goto st52;
		case 95: goto st112;
		case 98: goto tr1359;
		case 103: goto tr1364;
		case 105: goto tr1381;
		case 109: goto tr1365;
		case 110: goto tr1382;
		case 112: goto tr1377;
		case 115: goto tr1376;
		case 116: goto tr1383;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1367;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr434;
			} else if ( (*p) >= 65 )
				goto tr433;
		} else
			goto tr432;
	} else
		goto tr432;
	goto tr1037;
tr1381:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1052;
st1052:
	if ( ++p == pe )
		goto _test_eof1052;
case 1052:
#line 16070 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 47: goto st69;
		case 64: goto st113;
		case 94: goto st52;
		case 95: goto st112;
		case 99: goto tr1384;
		case 104: goto tr1376;
		case 108: goto tr1385;
		case 110: goto tr1374;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1367;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr434;
			} else if ( (*p) >= 65 )
				goto tr433;
		} else
			goto tr432;
	} else
		goto tr432;
	goto tr1037;
tr1384:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1053;
st1053:
	if ( ++p == pe )
		goto _test_eof1053;
case 1053:
#line 16124 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 114: goto tr1386;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1386:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1054;
st1054:
	if ( ++p == pe )
		goto _test_eof1054;
case 1054:
#line 16167 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 111: goto tr1387;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1387:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1055;
st1055:
	if ( ++p == pe )
		goto _test_eof1055;
case 1055:
#line 16210 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1388;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1388:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1056;
st1056:
	if ( ++p == pe )
		goto _test_eof1056;
case 1056:
#line 16253 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1389;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1389:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1057;
st1057:
	if ( ++p == pe )
		goto _test_eof1057;
case 1057:
#line 16296 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1390;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1385:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1058;
st1058:
	if ( ++p == pe )
		goto _test_eof1058;
case 1058:
#line 16341 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1391;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1037;
tr1391:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1059;
st1059:
	if ( ++p == pe )
		goto _test_eof1059;
case 1059:
#line 16384 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto tr1387;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1382:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1060;
st1060:
	if ( ++p == pe )
		goto _test_eof1060;
case 1060:
#line 16427 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1392;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1392:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1061;
st1061:
	if ( ++p == pe )
		goto _test_eof1061;
case 1061:
#line 16470 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 104: goto tr1393;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1383:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1062;
st1062:
	if ( ++p == pe )
		goto _test_eof1062;
case 1062:
#line 16513 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 104: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1335:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1063;
st1063:
	if ( ++p == pe )
		goto _test_eof1063;
case 1063:
#line 16556 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1394;
		case 115: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1394:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1064;
st1064:
	if ( ++p == pe )
		goto _test_eof1064;
case 1064:
#line 16600 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1395;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1395:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1065;
st1065:
	if ( ++p == pe )
		goto _test_eof1065;
case 1065:
#line 16643 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 111: goto tr1396;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1396:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1066;
st1066:
	if ( ++p == pe )
		goto _test_eof1066;
case 1066:
#line 16686 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1397:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1067;
st1067:
	if ( ++p == pe )
		goto _test_eof1067;
case 1067:
#line 16729 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1398;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1398:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1068;
st1068:
	if ( ++p == pe )
		goto _test_eof1068;
case 1068:
#line 16772 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1399;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1336:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1069;
st1069:
	if ( ++p == pe )
		goto _test_eof1069;
case 1069:
#line 16815 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 122: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1337:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1070;
st1070:
	if ( ++p == pe )
		goto _test_eof1070;
case 1070:
#line 16858 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1370;
		case 116: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1339:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1071;
st1071:
	if ( ++p == pe )
		goto _test_eof1071;
case 1071:
#line 16902 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 112: goto tr1400;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1400:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1072;
st1072:
	if ( ++p == pe )
		goto _test_eof1072;
case 1072:
#line 16945 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto tr1355;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1340:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 70 "src/ragel/rules/main.rl"
	{ s.act = 29;}
	goto st1073;
tr1471:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "src/ragel/actions.rl"
	{ s.suffix++; }
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1073;
st1073:
	if ( ++p == pe )
		goto _test_eof1073;
case 1073:
#line 16998 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1401;
		case 113: goto tr1402;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr0;
tr1401:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1074;
st1074:
	if ( ++p == pe )
		goto _test_eof1074;
case 1074:
#line 17042 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1402:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1075;
st1075:
	if ( ++p == pe )
		goto _test_eof1075;
case 1075:
#line 17085 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 32: goto tr84;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st357;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
st357:
	if ( ++p == pe )
		goto _test_eof357;
case 357:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 32: goto tr54;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr432;
		} else if ( (*p) >= 65 )
			goto tr432;
	} else
		goto st112;
	goto tr10;
tr1341:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1076;
st1076:
	if ( ++p == pe )
		goto _test_eof1076;
case 1076:
#line 17154 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 111: goto tr1404;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1404:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1077;
st1077:
	if ( ++p == pe )
		goto _test_eof1077;
case 1077:
#line 17197 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1355;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1342:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1078;
st1078:
	if ( ++p == pe )
		goto _test_eof1078;
case 1078:
#line 17240 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1343:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1079;
st1079:
	if ( ++p == pe )
		goto _test_eof1079;
case 1079:
#line 17283 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st355;
		case 64: goto st113;
		case 95: goto st112;
		case 100: goto tr1372;
		case 114: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr928:
#line 1 "NONE"
	{ s.te = p+1;}
#line 37 "src/ragel/rules/main.rl"
	{ s.act = 7;}
	goto st1080;
st1080:
	if ( ++p == pe )
		goto _test_eof1080;
case 1080:
#line 17327 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 32: goto st358;
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st361;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr165;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1405;
st358:
	if ( ++p == pe )
		goto _test_eof358;
case 358:
	if ( (*p) == 46 )
		goto st359;
	goto tr435;
st359:
	if ( ++p == pe )
		goto _test_eof359;
case 359:
	if ( (*p) == 32 )
		goto st360;
	goto tr435;
st360:
	if ( ++p == pe )
		goto _test_eof360;
case 360:
	if ( (*p) == 46 )
		goto tr438;
	goto tr435;
st361:
	if ( ++p == pe )
		goto _test_eof361;
case 361:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto tr439;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr435;
tr1408:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1081;
tr439:
#line 1 "NONE"
	{ s.te = p+1;}
#line 40 "src/ragel/rules/main.rl"
	{ s.act = 10;}
	goto st1081;
st1081:
	if ( ++p == pe )
		goto _test_eof1081;
case 1081:
#line 17402 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto tr1408;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr0;
tr929:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1082;
st1082:
	if ( ++p == pe )
		goto _test_eof1082;
case 1082:
#line 17429 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st362;
		case 47: goto st104;
		case 48: goto tr1410;
		case 50: goto tr1412;
		case 52: goto tr1413;
		case 54: goto tr1413;
		case 57: goto tr1413;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 56 )
				goto tr1411;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
st362:
	if ( ++p == pe )
		goto _test_eof362;
case 362:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 50: goto tr441;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 51 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto tr440;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr442;
	goto tr25;
tr440:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1083;
st1083:
	if ( ++p == pe )
		goto _test_eof1083;
case 1083:
#line 17537 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr442;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
st363:
	if ( ++p == pe )
		goto _test_eof363;
case 363:
	switch( (*p) ) {
		case 43: goto st112;
		case 50: goto st368;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 51 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto st364;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st366;
	goto tr25;
st364:
	if ( ++p == pe )
		goto _test_eof364;
case 364:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st365;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st366;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st365:
	if ( ++p == pe )
		goto _test_eof365;
case 365:
	switch( (*p) ) {
		case 43: goto st112;
		case 50: goto tr448;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 51 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto tr447;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr449;
	goto tr25;
tr447:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1084;
st1084:
	if ( ++p == pe )
		goto _test_eof1084;
case 1084:
#line 17666 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr449;
	goto tr1156;
tr449:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1085;
st1085:
	if ( ++p == pe )
		goto _test_eof1085;
case 1085:
#line 17694 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr1415;
	goto tr1156;
tr448:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1086;
st1086:
	if ( ++p == pe )
		goto _test_eof1086;
case 1086:
#line 17722 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 53: goto tr1416;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 52 )
				goto tr449;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr1415;
	goto tr1156;
tr1416:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1087;
st1087:
	if ( ++p == pe )
		goto _test_eof1087;
case 1087:
#line 17754 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto tr1415;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1156;
st366:
	if ( ++p == pe )
		goto _test_eof366;
case 366:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st365;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st367;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st367:
	if ( ++p == pe )
		goto _test_eof367;
case 367:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st365;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st368:
	if ( ++p == pe )
		goto _test_eof368;
case 368:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st365;
		case 53: goto st369;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 52 )
			goto st366;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st367;
	goto tr25;
st369:
	if ( ++p == pe )
		goto _test_eof369;
case 369:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st365;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st367;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
tr442:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1088;
st1088:
	if ( ++p == pe )
		goto _test_eof1088;
case 1088:
#line 17872 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1417;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
tr1417:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1089;
st1089:
	if ( ++p == pe )
		goto _test_eof1089;
case 1089:
#line 17931 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr165;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
tr441:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1090;
st1090:
	if ( ++p == pe )
		goto _test_eof1090;
case 1090:
#line 17990 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 53: goto tr1418;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 52 )
			goto tr442;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr1417;
	goto tr1005;
tr1418:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1091;
st1091:
	if ( ++p == pe )
		goto _test_eof1091;
case 1091:
#line 18053 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto tr1417;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr165;
	goto tr1005;
tr1410:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1092;
st1092:
	if ( ++p == pe )
		goto _test_eof1092;
case 1092:
#line 18115 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st370;
		case 39: goto st380;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st362;
		case 47: goto st104;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1421;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
st370:
	if ( ++p == pe )
		goto _test_eof370;
case 370:
	switch( (*p) ) {
		case 35: goto st371;
		case 97: goto st375;
		case 99: goto st36;
		case 100: goto st39;
		case 114: goto st311;
	}
	goto tr0;
st371:
	if ( ++p == pe )
		goto _test_eof371;
case 371:
	switch( (*p) ) {
		case 49: goto st29;
		case 51: goto st372;
		case 56: goto st247;
		case 88: goto st353;
		case 120: goto st353;
	}
	goto tr0;
st372:
	if ( ++p == pe )
		goto _test_eof372;
case 372:
	if ( (*p) == 57 )
		goto st373;
	goto tr0;
st373:
	if ( ++p == pe )
		goto _test_eof373;
case 373:
	if ( (*p) == 59 )
		goto st374;
	goto tr0;
st374:
	if ( ++p == pe )
		goto _test_eof374;
case 374:
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto st241;
		case 77: goto tr250;
		case 82: goto st242;
		case 83: goto tr252;
		case 86: goto st243;
		case 100: goto tr254;
		case 108: goto st244;
		case 109: goto tr256;
		case 114: goto st245;
		case 115: goto tr96;
		case 118: goto st246;
	}
	goto tr0;
st375:
	if ( ++p == pe )
		goto _test_eof375;
case 375:
	switch( (*p) ) {
		case 109: goto st271;
		case 112: goto st376;
	}
	goto tr0;
st376:
	if ( ++p == pe )
		goto _test_eof376;
case 376:
	if ( (*p) == 111 )
		goto st377;
	goto tr0;
st377:
	if ( ++p == pe )
		goto _test_eof377;
case 377:
	if ( (*p) == 115 )
		goto st378;
	goto tr0;
st378:
	if ( ++p == pe )
		goto _test_eof378;
case 378:
	if ( (*p) == 59 )
		goto st379;
	goto tr0;
st379:
	if ( ++p == pe )
		goto _test_eof379;
case 379:
	switch( (*p) ) {
		case 68: goto tr379;
		case 76: goto st321;
		case 77: goto tr381;
		case 82: goto st322;
		case 83: goto tr358;
		case 86: goto st323;
		case 100: goto tr384;
		case 108: goto st324;
		case 109: goto tr386;
		case 114: goto st325;
		case 115: goto tr96;
		case 118: goto st326;
	}
	goto tr0;
st380:
	if ( ++p == pe )
		goto _test_eof380;
case 380:
	switch( (*p) ) {
		case 68: goto tr199;
		case 76: goto st274;
		case 77: goto tr201;
		case 82: goto st275;
		case 83: goto tr198;
		case 86: goto st276;
		case 100: goto tr204;
		case 108: goto st277;
		case 109: goto tr206;
		case 114: goto st278;
		case 115: goto tr96;
		case 118: goto st279;
	}
	goto tr0;
tr1421:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1093;
st1093:
	if ( ++p == pe )
		goto _test_eof1093;
case 1093:
#line 18314 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st362;
		case 47: goto st104;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1411:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1094;
st1094:
	if ( ++p == pe )
		goto _test_eof1094;
case 1094:
#line 18388 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1422;
		case 46: goto st381;
		case 47: goto st397;
		case 58: goto st403;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1421;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1422:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1095;
st1095:
	if ( ++p == pe )
		goto _test_eof1095;
case 1095:
#line 18463 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 48: goto tr1426;
		case 51: goto tr1428;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 49 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 50 ) {
		if ( (*p) < 65 ) {
			if ( 52 <= (*p) && (*p) <= 57 )
				goto tr1429;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto tr1427;
	goto tr997;
tr1426:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1096;
st1096:
	if ( ++p == pe )
		goto _test_eof1096;
case 1096:
#line 18510 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 47: goto st104;
		case 48: goto tr1344;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1429;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1429:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1097;
st1097:
	if ( ++p == pe )
		goto _test_eof1097;
case 1097:
#line 18554 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1430;
		case 46: goto st345;
		case 47: goto st104;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1430:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1098;
st1098:
	if ( ++p == pe )
		goto _test_eof1098;
case 1098:
#line 18597 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 49: goto tr1431;
		case 50: goto tr1432;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1431:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1099;
st1099:
	if ( ++p == pe )
		goto _test_eof1099;
case 1099:
#line 18641 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 54 ) {
		if ( (*p) < 65 ) {
			if ( 55 <= (*p) && (*p) <= 57 )
				goto tr1433;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr432;
	goto tr997;
tr1433:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1100;
st1100:
	if ( ++p == pe )
		goto _test_eof1100;
case 1100:
#line 18686 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1434;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1434:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1101;
st1101:
	if ( ++p == pe )
		goto _test_eof1101;
case 1101:
#line 18728 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1435;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1432:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1102;
st1102:
	if ( ++p == pe )
		goto _test_eof1102;
case 1102:
#line 18770 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 65 ) {
			if ( 50 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1433;
	goto tr997;
tr1427:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1103;
st1103:
	if ( ++p == pe )
		goto _test_eof1103;
case 1103:
#line 18815 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1430;
		case 46: goto st345;
		case 47: goto st104;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1429;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1428:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1104;
st1104:
	if ( ++p == pe )
		goto _test_eof1104;
case 1104:
#line 18858 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1430;
		case 46: goto st345;
		case 47: goto st104;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 65 ) {
			if ( 50 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1429;
	goto tr997;
st381:
	if ( ++p == pe )
		goto _test_eof381;
case 381:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 48: goto tr461;
		case 49: goto tr462;
		case 50: goto tr463;
		case 51: goto tr464;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 46 ) {
			if ( 52 <= (*p) && (*p) <= 53 )
				goto tr465;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr466;
	goto tr0;
tr461:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1105;
st1105:
	if ( ++p == pe )
		goto _test_eof1105;
case 1105:
#line 18936 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 48: goto tr1436;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr1437;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
tr1436:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1106;
st1106:
	if ( ++p == pe )
		goto _test_eof1106;
case 1106:
#line 18996 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 64: goto st113;
		case 65: goto st382;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st385;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 97: goto st388;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st390;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1417;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
st382:
	if ( ++p == pe )
		goto _test_eof382;
case 382:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st383;
		case 64: goto st113;
		case 77: goto tr468;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st383:
	if ( ++p == pe )
		goto _test_eof383;
case 383:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 77: goto st384;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st384:
	if ( ++p == pe )
		goto _test_eof384;
case 384:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto tr470;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st385:
	if ( ++p == pe )
		goto _test_eof385;
case 385:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st386;
		case 64: goto st113;
		case 77: goto tr472;
		case 95: goto st112;
		case 97: goto tr171;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st386:
	if ( ++p == pe )
		goto _test_eof386;
case 386:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 77: goto st387;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st387:
	if ( ++p == pe )
		goto _test_eof387;
case 387:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto tr474;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st388:
	if ( ++p == pe )
		goto _test_eof388;
case 388:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st389;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto tr468;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st389:
	if ( ++p == pe )
		goto _test_eof389;
case 389:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto st384;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st390:
	if ( ++p == pe )
		goto _test_eof390;
case 390:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st391;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto tr472;
		case 115: goto st177;
		case 116: goto tr171;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st391:
	if ( ++p == pe )
		goto _test_eof391;
case 391:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto st387;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr25;
tr1437:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1107;
st1107:
	if ( ++p == pe )
		goto _test_eof1107;
case 1107:
#line 19272 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 64: goto st113;
		case 65: goto st382;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st385;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 97: goto st388;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st390;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1417;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
st392:
	if ( ++p == pe )
		goto _test_eof392;
case 392:
	switch( (*p) ) {
		case 43: goto st112;
		case 48: goto st364;
		case 49: goto st393;
		case 50: goto st396;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 51 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st366;
	goto tr25;
st393:
	if ( ++p == pe )
		goto _test_eof393;
case 393:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st365;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 55 ) {
		if ( 48 <= (*p) && (*p) <= 54 )
			goto st366;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st394;
	goto tr25;
st394:
	if ( ++p == pe )
		goto _test_eof394;
case 394:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st365;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st395;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st395:
	if ( ++p == pe )
		goto _test_eof395;
case 395:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st365;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr481;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr25;
st396:
	if ( ++p == pe )
		goto _test_eof396;
case 396:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto st365;
		case 53: goto st369;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 49 ) {
			if ( 50 <= (*p) && (*p) <= 52 )
				goto st366;
		} else if ( (*p) >= 48 )
			goto st394;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st367;
	goto tr25;
tr462:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1108;
st1108:
	if ( ++p == pe )
		goto _test_eof1108;
case 1108:
#line 19447 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1437;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
tr463:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1109;
st1109:
	if ( ++p == pe )
		goto _test_eof1109;
case 1109:
#line 19506 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 53: goto tr1443;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 52 )
			goto tr1437;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr1444;
	goto tr1005;
tr1443:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1110;
st1110:
	if ( ++p == pe )
		goto _test_eof1110;
case 1110:
#line 19569 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 64: goto st113;
		case 65: goto st382;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st385;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 97: goto st388;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st390;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto tr1417;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 66 )
			goto st112;
	} else
		goto tr165;
	goto tr1005;
tr1444:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1111;
st1111:
	if ( ++p == pe )
		goto _test_eof1111;
case 1111:
#line 19633 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 64: goto st113;
		case 65: goto st382;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st385;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 97: goto st388;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st390;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr165;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
tr464:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1112;
st1112:
	if ( ++p == pe )
		goto _test_eof1112;
case 1112:
#line 19694 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 50 ) {
		if ( 48 <= (*p) && (*p) <= 49 )
			goto tr1444;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr1445;
	goto tr1005;
tr1445:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1113;
st1113:
	if ( ++p == pe )
		goto _test_eof1113;
case 1113:
#line 19756 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 64: goto st113;
		case 65: goto st382;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st385;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 97: goto st388;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st390;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr165;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
tr465:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1114;
st1114:
	if ( ++p == pe )
		goto _test_eof1114;
case 1114:
#line 19817 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1445;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
tr466:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1115;
st1115:
	if ( ++p == pe )
		goto _test_eof1115;
case 1115:
#line 19876 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1417;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
st397:
	if ( ++p == pe )
		goto _test_eof397;
case 397:
	switch( (*p) ) {
		case 48: goto tr482;
		case 51: goto tr484;
	}
	if ( (*p) > 50 ) {
		if ( 52 <= (*p) && (*p) <= 57 )
			goto tr485;
	} else if ( (*p) >= 49 )
		goto tr483;
	goto tr0;
tr482:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1116;
st1116:
	if ( ++p == pe )
		goto _test_eof1116;
case 1116:
#line 19949 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 48: goto tr90;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr485;
	goto tr1005;
tr485:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1117;
st1117:
	if ( ++p == pe )
		goto _test_eof1117;
case 1117:
#line 19998 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 47: goto st398;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto tr1005;
st398:
	if ( ++p == pe )
		goto _test_eof398;
case 398:
	switch( (*p) ) {
		case 49: goto st399;
		case 50: goto st402;
	}
	goto tr25;
st399:
	if ( ++p == pe )
		goto _test_eof399;
case 399:
	if ( 55 <= (*p) && (*p) <= 57 )
		goto st400;
	goto tr25;
st400:
	if ( ++p == pe )
		goto _test_eof400;
case 400:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st401;
	goto tr25;
st401:
	if ( ++p == pe )
		goto _test_eof401;
case 401:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto tr25;
st402:
	if ( ++p == pe )
		goto _test_eof402;
case 402:
	if ( 48 <= (*p) && (*p) <= 49 )
		goto st400;
	goto tr25;
tr483:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1118;
st1118:
	if ( ++p == pe )
		goto _test_eof1118;
case 1118:
#line 20084 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 47: goto st398;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr485;
	goto tr1005;
tr484:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1119;
st1119:
	if ( ++p == pe )
		goto _test_eof1119;
case 1119:
#line 20133 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 47: goto st398;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( (*p) > 49 ) {
		if ( 50 <= (*p) && (*p) <= 57 )
			goto tr90;
	} else if ( (*p) >= 48 )
		goto tr485;
	goto tr1005;
st403:
	if ( ++p == pe )
		goto _test_eof403;
case 403:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st404;
	goto tr0;
st404:
	if ( ++p == pe )
		goto _test_eof404;
case 404:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr491;
	goto tr0;
tr491:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st1120;
st1120:
	if ( ++p == pe )
		goto _test_eof1120;
case 1120:
#line 20197 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 65: goto st405;
		case 80: goto st408;
		case 97: goto st411;
		case 112: goto st413;
	}
	goto tr1235;
st405:
	if ( ++p == pe )
		goto _test_eof405;
case 405:
	switch( (*p) ) {
		case 46: goto st406;
		case 77: goto tr494;
	}
	goto tr492;
st406:
	if ( ++p == pe )
		goto _test_eof406;
case 406:
	if ( (*p) == 77 )
		goto st407;
	goto tr492;
st407:
	if ( ++p == pe )
		goto _test_eof407;
case 407:
	if ( (*p) == 46 )
		goto tr496;
	goto tr492;
st408:
	if ( ++p == pe )
		goto _test_eof408;
case 408:
	switch( (*p) ) {
		case 46: goto st409;
		case 77: goto tr498;
	}
	goto tr492;
st409:
	if ( ++p == pe )
		goto _test_eof409;
case 409:
	if ( (*p) == 77 )
		goto st410;
	goto tr492;
st410:
	if ( ++p == pe )
		goto _test_eof410;
case 410:
	if ( (*p) == 46 )
		goto tr500;
	goto tr492;
st411:
	if ( ++p == pe )
		goto _test_eof411;
case 411:
	switch( (*p) ) {
		case 46: goto st412;
		case 109: goto tr494;
	}
	goto tr492;
st412:
	if ( ++p == pe )
		goto _test_eof412;
case 412:
	if ( (*p) == 109 )
		goto st407;
	goto tr492;
st413:
	if ( ++p == pe )
		goto _test_eof413;
case 413:
	switch( (*p) ) {
		case 46: goto st414;
		case 109: goto tr498;
	}
	goto tr492;
st414:
	if ( ++p == pe )
		goto _test_eof414;
case 414:
	if ( (*p) == 109 )
		goto st410;
	goto tr492;
tr1412:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1121;
st1121:
	if ( ++p == pe )
		goto _test_eof1121;
case 1121:
#line 20293 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1451;
		case 46: goto st415;
		case 47: goto st416;
		case 58: goto st403;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1421;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1451:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1122;
st1122:
	if ( ++p == pe )
		goto _test_eof1122;
case 1122:
#line 20368 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 48: goto tr1426;
		case 51: goto tr1454;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 49 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 50 ) {
		if ( (*p) < 65 ) {
			if ( 52 <= (*p) && (*p) <= 57 )
				goto tr1429;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto tr1427;
	goto tr997;
tr1454:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1123;
st1123:
	if ( ++p == pe )
		goto _test_eof1123;
case 1123:
#line 20415 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1430;
		case 46: goto st345;
		case 47: goto st104;
		case 49: goto tr1429;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
st415:
	if ( ++p == pe )
		goto _test_eof415;
case 415:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 48: goto tr461;
		case 49: goto tr462;
		case 50: goto tr463;
		case 51: goto tr503;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 46 ) {
			if ( 52 <= (*p) && (*p) <= 53 )
				goto tr465;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr466;
	goto tr0;
tr503:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1124;
st1124:
	if ( ++p == pe )
		goto _test_eof1124;
case 1124:
#line 20491 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 49: goto tr1444;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1445;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
st416:
	if ( ++p == pe )
		goto _test_eof416;
case 416:
	switch( (*p) ) {
		case 48: goto tr482;
		case 51: goto tr504;
	}
	if ( (*p) > 50 ) {
		if ( 52 <= (*p) && (*p) <= 57 )
			goto tr485;
	} else if ( (*p) >= 49 )
		goto tr483;
	goto tr0;
tr504:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1125;
st1125:
	if ( ++p == pe )
		goto _test_eof1125;
case 1125:
#line 20565 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 47: goto st398;
		case 49: goto tr485;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto tr1005;
tr1413:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1126;
st1126:
	if ( ++p == pe )
		goto _test_eof1126;
case 1126:
#line 20615 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1455;
		case 46: goto st417;
		case 47: goto st418;
		case 58: goto st403;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1421;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1455:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1127;
st1127:
	if ( ++p == pe )
		goto _test_eof1127;
case 1127:
#line 20690 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 48: goto tr1426;
		case 51: goto tr1458;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 49 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 50 ) {
		if ( (*p) < 65 ) {
			if ( 52 <= (*p) && (*p) <= 57 )
				goto tr1429;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto tr1427;
	goto tr997;
tr1458:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1128;
st1128:
	if ( ++p == pe )
		goto _test_eof1128;
case 1128:
#line 20737 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1430;
		case 46: goto st345;
		case 47: goto st104;
		case 48: goto tr1429;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
st417:
	if ( ++p == pe )
		goto _test_eof417;
case 417:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 48: goto tr461;
		case 49: goto tr462;
		case 50: goto tr463;
		case 51: goto tr505;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 46 ) {
			if ( 52 <= (*p) && (*p) <= 53 )
				goto tr465;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr466;
	goto tr0;
tr505:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1129;
st1129:
	if ( ++p == pe )
		goto _test_eof1129;
case 1129:
#line 20813 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 48: goto tr1444;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr1445;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
st418:
	if ( ++p == pe )
		goto _test_eof418;
case 418:
	switch( (*p) ) {
		case 48: goto tr482;
		case 51: goto tr506;
	}
	if ( (*p) > 50 ) {
		if ( 52 <= (*p) && (*p) <= 57 )
			goto tr485;
	} else if ( (*p) >= 49 )
		goto tr483;
	goto tr0;
tr506:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1130;
st1130:
	if ( ++p == pe )
		goto _test_eof1130;
case 1130:
#line 20887 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 47: goto st398;
		case 48: goto tr485;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto tr1005;
tr930:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1131;
st1131:
	if ( ++p == pe )
		goto _test_eof1131;
case 1131:
#line 20937 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1422;
		case 46: goto st381;
		case 47: goto st397;
		case 48: goto tr1459;
		case 49: goto tr1460;
		case 50: goto tr1461;
		case 58: goto st403;
		case 64: goto st113;
		case 65: goto tr1464;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1465;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 97: goto tr1466;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1467;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < 51 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 54 ) {
		if ( (*p) < 66 ) {
			if ( 55 <= (*p) && (*p) <= 57 )
				goto tr1463;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1462;
	goto tr1235;
tr1459:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1132;
st1132:
	if ( ++p == pe )
		goto _test_eof1132;
case 1132:
#line 21020 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st370;
		case 39: goto st380;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1422;
		case 46: goto st381;
		case 47: goto st397;
		case 48: goto tr1468;
		case 58: goto st403;
		case 64: goto st113;
		case 65: goto tr1464;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1465;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 97: goto tr1466;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1467;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1469;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1235;
tr1468:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1133;
st1133:
	if ( ++p == pe )
		goto _test_eof1133;
case 1133:
#line 21098 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st370;
		case 39: goto st380;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st362;
		case 47: goto st104;
		case 48: goto tr1470;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1471;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1470:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1134;
st1134:
	if ( ++p == pe )
		goto _test_eof1134;
case 1134:
#line 21173 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st370;
		case 39: goto st380;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st350;
		case 47: goto st104;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1471;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1469:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1135;
st1135:
	if ( ++p == pe )
		goto _test_eof1135;
case 1135:
#line 21247 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st362;
		case 47: goto st104;
		case 48: goto tr1470;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1464:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1136;
st1136:
	if ( ++p == pe )
		goto _test_eof1136;
case 1136:
#line 21322 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st419;
		case 64: goto st113;
		case 77: goto tr1473;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
st419:
	if ( ++p == pe )
		goto _test_eof419;
case 419:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 77: goto tr507;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr432;
		} else if ( (*p) >= 65 )
			goto tr432;
	} else
		goto st112;
	goto tr10;
tr507:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1137;
st1137:
	if ( ++p == pe )
		goto _test_eof1137;
case 1137:
#line 21391 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto tr1474;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1474:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 58 "src/ragel/rules/date_time.rl"
	{n2="am";}
#line 71 "src/ragel/rules/main.rl"
	{ s.act = 30;}
	goto st1138;
tr1478:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "src/ragel/actions.rl"
	{ s.suffix += 4; }
#line 59 "src/ragel/rules/date_time.rl"
	{n2="pm";}
#line 71 "src/ragel/rules/main.rl"
	{ s.act = 30;}
	goto st1138;
st1138:
	if ( ++p == pe )
		goto _test_eof1138;
case 1138:
#line 21447 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto tr1475;
tr1465:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1139;
st1139:
	if ( ++p == pe )
		goto _test_eof1139;
case 1139:
#line 21479 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st420;
		case 64: goto st113;
		case 77: goto tr1477;
		case 95: goto st112;
		case 97: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
st420:
	if ( ++p == pe )
		goto _test_eof420;
case 420:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 77: goto tr508;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr432;
		} else if ( (*p) >= 65 )
			goto tr432;
	} else
		goto st112;
	goto tr10;
tr508:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1140;
st1140:
	if ( ++p == pe )
		goto _test_eof1140;
case 1140:
#line 21549 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto tr1478;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1466:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1141;
st1141:
	if ( ++p == pe )
		goto _test_eof1141;
case 1141:
#line 21591 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st421;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto tr1480;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
st421:
	if ( ++p == pe )
		goto _test_eof421;
case 421:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto tr507;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr432;
		} else if ( (*p) >= 65 )
			goto tr432;
	} else
		goto st112;
	goto tr10;
tr1467:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1142;
st1142:
	if ( ++p == pe )
		goto _test_eof1142;
case 1142:
#line 21660 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st422;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto tr1482;
		case 115: goto tr1370;
		case 116: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
st422:
	if ( ++p == pe )
		goto _test_eof422;
case 422:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto tr508;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr432;
		} else if ( (*p) >= 65 )
			goto tr432;
	} else
		goto st112;
	goto tr10;
tr1460:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1143;
st1143:
	if ( ++p == pe )
		goto _test_eof1143;
case 1143:
#line 21731 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1455;
		case 46: goto st417;
		case 47: goto st418;
		case 48: goto tr1468;
		case 58: goto st403;
		case 64: goto st113;
		case 65: goto tr1464;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1465;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 97: goto tr1466;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1467;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1469;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1235;
tr1461:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1144;
st1144:
	if ( ++p == pe )
		goto _test_eof1144;
case 1144:
#line 21809 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1422;
		case 46: goto st381;
		case 47: goto st397;
		case 48: goto tr1468;
		case 58: goto st403;
		case 64: goto st113;
		case 65: goto tr1464;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1465;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 97: goto tr1466;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1467;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1469;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1235;
tr1462:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1145;
st1145:
	if ( ++p == pe )
		goto _test_eof1145;
case 1145:
#line 21887 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1483;
		case 46: goto st423;
		case 47: goto st424;
		case 48: goto tr1468;
		case 58: goto st403;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1469;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1483:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1146;
st1146:
	if ( ++p == pe )
		goto _test_eof1146;
case 1146:
#line 21963 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 48: goto tr1426;
		case 49: goto tr1486;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 50 <= (*p) && (*p) <= 57 )
				goto tr1429;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto st8;
	goto tr997;
tr1486:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1147;
st1147:
	if ( ++p == pe )
		goto _test_eof1147;
case 1147:
#line 22007 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1430;
		case 46: goto st345;
		case 47: goto st104;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 50 ) {
		if ( (*p) < 65 ) {
			if ( 51 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1429;
	goto tr997;
st423:
	if ( ++p == pe )
		goto _test_eof423;
case 423:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 48: goto tr461;
		case 49: goto tr509;
		case 50: goto tr510;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 46 ) {
			if ( 51 <= (*p) && (*p) <= 53 )
				goto tr465;
		} else if ( (*p) >= 45 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr466;
	goto tr25;
tr509:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1148;
st1148:
	if ( ++p == pe )
		goto _test_eof1148;
case 1148:
#line 22084 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 51 ) {
		if ( 48 <= (*p) && (*p) <= 50 )
			goto tr1437;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr1436;
	goto tr1005;
tr510:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1149;
st1149:
	if ( ++p == pe )
		goto _test_eof1149;
case 1149:
#line 22146 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 53: goto tr1487;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 52 )
			goto tr1436;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr1445;
	goto tr1005;
tr1487:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1150;
st1150:
	if ( ++p == pe )
		goto _test_eof1150;
case 1150:
#line 22209 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 64: goto st113;
		case 65: goto st382;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st385;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 97: goto st388;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st390;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto tr1417;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 66 )
			goto st112;
	} else
		goto tr165;
	goto tr1005;
st424:
	if ( ++p == pe )
		goto _test_eof424;
case 424:
	switch( (*p) ) {
		case 48: goto tr482;
		case 49: goto tr511;
	}
	if ( 50 <= (*p) && (*p) <= 57 )
		goto tr485;
	goto tr25;
tr511:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1151;
st1151:
	if ( ++p == pe )
		goto _test_eof1151;
case 1151:
#line 22284 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 47: goto st398;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( (*p) > 50 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto tr90;
	} else if ( (*p) >= 48 )
		goto tr485;
	goto tr1005;
tr1463:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1152;
st1152:
	if ( ++p == pe )
		goto _test_eof1152;
case 1152:
#line 22336 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1483;
		case 46: goto st423;
		case 47: goto st424;
		case 48: goto tr1488;
		case 58: goto st403;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1489;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1488:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1153;
st1153:
	if ( ++p == pe )
		goto _test_eof1153;
case 1153:
#line 22412 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st370;
		case 39: goto st380;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st362;
		case 47: goto st104;
		case 48: goto tr1490;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1471;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1491;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1490:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1154;
st1154:
	if ( ++p == pe )
		goto _test_eof1154;
case 1154:
#line 22487 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st370;
		case 39: goto st380;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1492;
		case 46: goto st350;
		case 47: goto st425;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1471;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1492:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1155;
st1155:
	if ( ++p == pe )
		goto _test_eof1155;
case 1155:
#line 22561 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 48: goto tr1494;
		case 49: goto tr1495;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 50 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto st8;
	goto tr997;
tr1494:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1156;
st1156:
	if ( ++p == pe )
		goto _test_eof1156;
case 1156:
#line 22605 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 47: goto st104;
		case 48: goto tr1344;
		case 50: goto tr1497;
		case 52: goto tr1498;
		case 54: goto tr1498;
		case 57: goto tr1498;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 56 )
				goto tr1496;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1496:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1157;
st1157:
	if ( ++p == pe )
		goto _test_eof1157;
case 1157:
#line 22653 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1499;
		case 46: goto st345;
		case 47: goto st104;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1499:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1158;
st1158:
	if ( ++p == pe )
		goto _test_eof1158;
case 1158:
#line 22696 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 48: goto tr1500;
		case 51: goto tr1501;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 49 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 50 ) {
		if ( (*p) < 65 ) {
			if ( 52 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1434;
	goto tr997;
tr1500:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1159;
st1159:
	if ( ++p == pe )
		goto _test_eof1159;
case 1159:
#line 22743 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 48: goto tr432;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1435;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1501:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1160;
st1160:
	if ( ++p == pe )
		goto _test_eof1160;
case 1160:
#line 22786 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 65 ) {
			if ( 50 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1435;
	goto tr997;
tr1497:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1161;
st1161:
	if ( ++p == pe )
		goto _test_eof1161;
case 1161:
#line 22831 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1502;
		case 46: goto st345;
		case 47: goto st104;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1502:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1162;
st1162:
	if ( ++p == pe )
		goto _test_eof1162;
case 1162:
#line 22874 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 48: goto tr1500;
		case 51: goto tr1503;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 49 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 50 ) {
		if ( (*p) < 65 ) {
			if ( 52 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1434;
	goto tr997;
tr1503:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1163;
st1163:
	if ( ++p == pe )
		goto _test_eof1163;
case 1163:
#line 22921 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 49: goto tr1435;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1498:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1164;
st1164:
	if ( ++p == pe )
		goto _test_eof1164;
case 1164:
#line 22964 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1504;
		case 46: goto st345;
		case 47: goto st104;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1504:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1165;
st1165:
	if ( ++p == pe )
		goto _test_eof1165;
case 1165:
#line 23007 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 48: goto tr1500;
		case 51: goto tr1505;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 49 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 50 ) {
		if ( (*p) < 65 ) {
			if ( 52 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1434;
	goto tr997;
tr1505:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1166;
st1166:
	if ( ++p == pe )
		goto _test_eof1166;
case 1166:
#line 23054 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 48: goto tr1435;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr432;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1495:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1167;
st1167:
	if ( ++p == pe )
		goto _test_eof1167;
case 1167:
#line 23097 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 47: goto st104;
		case 49: goto tr1498;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 50 ) {
		if ( (*p) < 65 ) {
			if ( 51 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1496;
	goto tr997;
st425:
	if ( ++p == pe )
		goto _test_eof425;
case 425:
	switch( (*p) ) {
		case 48: goto tr512;
		case 49: goto tr513;
	}
	if ( 50 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto tr25;
tr512:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1168;
st1168:
	if ( ++p == pe )
		goto _test_eof1168;
case 1168:
#line 23155 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 48: goto tr90;
		case 50: goto tr1507;
		case 52: goto tr1508;
		case 54: goto tr1508;
		case 57: goto tr1508;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 49 <= (*p) && (*p) <= 56 )
		goto tr1506;
	goto tr1005;
tr1506:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1169;
st1169:
	if ( ++p == pe )
		goto _test_eof1169;
case 1169:
#line 23208 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 47: goto st426;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto tr1005;
st426:
	if ( ++p == pe )
		goto _test_eof426;
case 426:
	switch( (*p) ) {
		case 48: goto st427;
		case 51: goto st428;
	}
	if ( 49 <= (*p) && (*p) <= 50 )
		goto st401;
	goto tr25;
st427:
	if ( ++p == pe )
		goto _test_eof427;
case 427:
	if ( 49 <= (*p) && (*p) <= 57 )
		goto tr96;
	goto tr25;
st428:
	if ( ++p == pe )
		goto _test_eof428;
case 428:
	if ( 48 <= (*p) && (*p) <= 49 )
		goto tr96;
	goto tr25;
tr1507:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1170;
st1170:
	if ( ++p == pe )
		goto _test_eof1170;
case 1170:
#line 23282 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 47: goto st429;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto tr1005;
st429:
	if ( ++p == pe )
		goto _test_eof429;
case 429:
	switch( (*p) ) {
		case 48: goto st427;
		case 51: goto st430;
	}
	if ( 49 <= (*p) && (*p) <= 50 )
		goto st401;
	goto tr25;
st430:
	if ( ++p == pe )
		goto _test_eof430;
case 430:
	if ( (*p) == 49 )
		goto tr96;
	goto tr25;
tr1508:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1171;
st1171:
	if ( ++p == pe )
		goto _test_eof1171;
case 1171:
#line 23349 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 47: goto st431;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr90;
	goto tr1005;
st431:
	if ( ++p == pe )
		goto _test_eof431;
case 431:
	switch( (*p) ) {
		case 48: goto st427;
		case 51: goto st432;
	}
	if ( 49 <= (*p) && (*p) <= 50 )
		goto st401;
	goto tr25;
st432:
	if ( ++p == pe )
		goto _test_eof432;
case 432:
	if ( (*p) == 48 )
		goto tr96;
	goto tr25;
tr513:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1172;
st1172:
	if ( ++p == pe )
		goto _test_eof1172;
case 1172:
#line 23416 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 49: goto tr1508;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( (*p) > 50 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto tr90;
	} else if ( (*p) >= 48 )
		goto tr1506;
	goto tr1005;
tr1491:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1173;
st1173:
	if ( ++p == pe )
		goto _test_eof1173;
case 1173:
#line 23468 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1492;
		case 46: goto st350;
		case 47: goto st425;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1489:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1174;
st1174:
	if ( ++p == pe )
		goto _test_eof1174;
case 1174:
#line 23542 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st362;
		case 47: goto st104;
		case 48: goto tr1490;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1491;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr931:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1175;
st1175:
	if ( ++p == pe )
		goto _test_eof1175;
case 1175:
#line 23617 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1451;
		case 46: goto st415;
		case 47: goto st416;
		case 48: goto tr1512;
		case 49: goto tr1463;
		case 53: goto tr1514;
		case 58: goto st403;
		case 64: goto st113;
		case 65: goto tr1464;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1465;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 97: goto tr1466;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1467;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < 50 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 52 ) {
		if ( (*p) < 66 ) {
			if ( 54 <= (*p) && (*p) <= 57 )
				goto tr1515;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1513;
	goto tr1235;
tr1512:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1176;
st1176:
	if ( ++p == pe )
		goto _test_eof1176;
case 1176:
#line 23700 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st370;
		case 39: goto st380;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1483;
		case 46: goto st423;
		case 47: goto st424;
		case 48: goto tr1488;
		case 58: goto st403;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1489;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1513:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1177;
st1177:
	if ( ++p == pe )
		goto _test_eof1177;
case 1177:
#line 23776 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1483;
		case 46: goto st423;
		case 47: goto st424;
		case 48: goto tr1516;
		case 58: goto st403;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1421;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1516:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1178;
st1178:
	if ( ++p == pe )
		goto _test_eof1178;
case 1178:
#line 23852 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st370;
		case 39: goto st380;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st362;
		case 47: goto st104;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1471;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1514:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1179;
st1179:
	if ( ++p == pe )
		goto _test_eof1179;
case 1179:
#line 23926 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1483;
		case 46: goto st433;
		case 47: goto st424;
		case 48: goto tr1516;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < 49 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 53 ) {
		if ( (*p) < 65 ) {
			if ( 54 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1421;
	goto tr1005;
st433:
	if ( ++p == pe )
		goto _test_eof433;
case 433:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 48: goto tr518;
		case 49: goto tr519;
		case 50: goto tr520;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 51 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr466;
	goto tr25;
tr518:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1180;
st1180:
	if ( ++p == pe )
		goto _test_eof1180;
case 1180:
#line 24032 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 48: goto tr442;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 57 )
			goto tr466;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
tr519:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1181;
st1181:
	if ( ++p == pe )
		goto _test_eof1181;
case 1181:
#line 24092 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 51 ) {
		if ( 48 <= (*p) && (*p) <= 50 )
			goto tr466;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr442;
	goto tr1005;
tr520:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1182;
st1182:
	if ( ++p == pe )
		goto _test_eof1182;
case 1182:
#line 24154 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 53: goto tr1418;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 52 )
			goto tr442;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr1417;
	goto tr1005;
tr1515:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1183;
st1183:
	if ( ++p == pe )
		goto _test_eof1183;
case 1183:
#line 24217 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1483;
		case 46: goto st433;
		case 47: goto st424;
		case 48: goto tr1470;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr932:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1184;
st1184:
	if ( ++p == pe )
		goto _test_eof1184;
case 1184:
#line 24292 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1422;
		case 46: goto st381;
		case 47: goto st397;
		case 48: goto tr1518;
		case 49: goto tr1519;
		case 58: goto st403;
		case 64: goto st113;
		case 65: goto tr1464;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1465;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 97: goto tr1466;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1467;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 50 <= (*p) && (*p) <= 57 )
				goto tr1469;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1235;
tr1518:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1185;
st1185:
	if ( ++p == pe )
		goto _test_eof1185;
case 1185:
#line 24371 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st370;
		case 39: goto st380;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1520;
		case 46: goto st434;
		case 47: goto st435;
		case 48: goto tr1470;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1520:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1186;
st1186:
	if ( ++p == pe )
		goto _test_eof1186;
case 1186:
#line 24446 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 48: goto tr1523;
		case 49: goto tr1486;
		case 50: goto tr1344;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 51 <= (*p) && (*p) <= 57 )
				goto tr1429;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto st8;
	goto tr997;
tr1523:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1187;
st1187:
	if ( ++p == pe )
		goto _test_eof1187;
case 1187:
#line 24491 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 47: goto st104;
		case 49: goto tr1429;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 50 ) {
		if ( (*p) < 65 ) {
			if ( 51 <= (*p) && (*p) <= 57 )
				goto tr1429;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1344;
	goto tr997;
st434:
	if ( ++p == pe )
		goto _test_eof434;
case 434:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 48: goto tr521;
		case 49: goto tr519;
		case 50: goto tr441;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 51 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr466;
	goto tr25;
tr521:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1188;
st1188:
	if ( ++p == pe )
		goto _test_eof1188;
case 1188:
#line 24566 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 49: goto tr466;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 51 ) {
		if ( 48 <= (*p) && (*p) <= 50 )
			goto tr442;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr466;
	goto tr1005;
st435:
	if ( ++p == pe )
		goto _test_eof435;
case 435:
	switch( (*p) ) {
		case 48: goto tr522;
		case 49: goto tr511;
		case 50: goto tr90;
	}
	if ( 51 <= (*p) && (*p) <= 57 )
		goto tr485;
	goto tr25;
tr522:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1189;
st1189:
	if ( ++p == pe )
		goto _test_eof1189;
case 1189:
#line 24641 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 49: goto tr485;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( (*p) > 50 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto tr485;
	} else if ( (*p) >= 48 )
		goto tr90;
	goto tr1005;
tr1519:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1190;
st1190:
	if ( ++p == pe )
		goto _test_eof1190;
case 1190:
#line 24693 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1524;
		case 46: goto st436;
		case 47: goto st437;
		case 48: goto tr1470;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr1524:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1191;
st1191:
	if ( ++p == pe )
		goto _test_eof1191;
case 1191:
#line 24768 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st354;
		case 46: goto st345;
		case 48: goto tr1527;
		case 49: goto tr1528;
		case 52: goto tr1344;
		case 54: goto tr1344;
		case 57: goto tr1344;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 50 <= (*p) && (*p) <= 56 )
				goto tr1429;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1346;
		} else
			goto tr1345;
	} else
		goto st8;
	goto tr997;
tr1527:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1192;
st1192:
	if ( ++p == pe )
		goto _test_eof1192;
case 1192:
#line 24815 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1347;
		case 46: goto st345;
		case 47: goto st104;
		case 48: goto tr1344;
		case 52: goto tr1344;
		case 54: goto tr1344;
		case 57: goto tr1344;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 56 )
				goto tr1429;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr997;
tr1528:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1193;
st1193:
	if ( ++p == pe )
		goto _test_eof1193;
case 1193:
#line 24862 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1430;
		case 46: goto st345;
		case 47: goto st104;
		case 49: goto tr1344;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 50 ) {
		if ( (*p) < 65 ) {
			if ( 51 <= (*p) && (*p) <= 57 )
				goto tr1344;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto tr1429;
	goto tr997;
st436:
	if ( ++p == pe )
		goto _test_eof436;
case 436:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 48: goto tr523;
		case 49: goto tr524;
		case 50: goto tr520;
		case 52: goto tr442;
		case 54: goto tr442;
		case 57: goto tr442;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 51 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 56 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr466;
	goto tr25;
tr523:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1194;
st1194:
	if ( ++p == pe )
		goto _test_eof1194;
case 1194:
#line 24940 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st363;
		case 48: goto tr442;
		case 52: goto tr442;
		case 54: goto tr442;
		case 57: goto tr442;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 65 ) {
		if ( 49 <= (*p) && (*p) <= 56 )
			goto tr466;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr1005;
tr524:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1195;
st1195:
	if ( ++p == pe )
		goto _test_eof1195;
case 1195:
#line 25003 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 43: goto st112;
		case 45: goto st163;
		case 46: goto st392;
		case 49: goto tr442;
		case 64: goto st113;
		case 67: goto tr176;
		case 70: goto tr176;
		case 71: goto st164;
		case 72: goto st167;
		case 75: goto tr1252;
		case 76: goto tr176;
		case 77: goto tr1253;
		case 80: goto st168;
		case 82: goto st169;
		case 84: goto tr1256;
		case 95: goto st112;
		case 98: goto st171;
		case 99: goto tr1258;
		case 100: goto st176;
		case 102: goto st178;
		case 103: goto tr1261;
		case 104: goto tr1262;
		case 105: goto st181;
		case 107: goto tr1264;
		case 108: goto st184;
		case 109: goto tr1266;
		case 110: goto st194;
		case 111: goto st200;
		case 112: goto st201;
		case 113: goto st202;
		case 114: goto st203;
		case 115: goto tr1272;
		case 116: goto st208;
		case 117: goto st210;
		case 121: goto st211;
	}
	if ( (*p) < 51 ) {
		if ( 48 <= (*p) && (*p) <= 50 )
			goto tr466;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto tr442;
	goto tr1005;
st437:
	if ( ++p == pe )
		goto _test_eof437;
case 437:
	switch( (*p) ) {
		case 48: goto tr525;
		case 49: goto tr526;
		case 52: goto tr90;
		case 54: goto tr90;
		case 57: goto tr90;
	}
	if ( 50 <= (*p) && (*p) <= 56 )
		goto tr485;
	goto tr25;
tr525:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1196;
st1196:
	if ( ++p == pe )
		goto _test_eof1196;
case 1196:
#line 25080 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 48: goto tr90;
		case 52: goto tr90;
		case 54: goto tr90;
		case 57: goto tr90;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( 49 <= (*p) && (*p) <= 56 )
		goto tr485;
	goto tr1005;
tr526:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1197;
st1197:
	if ( ++p == pe )
		goto _test_eof1197;
case 1197:
#line 25132 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st23;
		case -50: goto st25;
		case 38: goto st27;
		case 47: goto st398;
		case 49: goto tr90;
		case 67: goto tr28;
		case 70: goto tr28;
		case 71: goto st43;
		case 72: goto st47;
		case 75: goto tr1012;
		case 76: goto tr28;
		case 77: goto tr1013;
		case 80: goto st48;
		case 82: goto st49;
		case 84: goto tr1016;
		case 98: goto st51;
		case 99: goto tr1018;
		case 100: goto st61;
		case 102: goto st63;
		case 103: goto tr1021;
		case 104: goto tr1022;
		case 105: goto st68;
		case 107: goto tr1024;
		case 108: goto st72;
		case 109: goto tr1026;
		case 110: goto st82;
		case 111: goto st88;
		case 112: goto st89;
		case 113: goto st90;
		case 114: goto st91;
		case 115: goto tr1032;
		case 116: goto st96;
		case 117: goto st98;
		case 121: goto st99;
	}
	if ( (*p) > 50 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto tr90;
	} else if ( (*p) >= 48 )
		goto tr485;
	goto tr1005;
tr933:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1198;
st1198:
	if ( ++p == pe )
		goto _test_eof1198;
case 1198:
#line 25185 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1455;
		case 46: goto st417;
		case 47: goto st418;
		case 48: goto tr1529;
		case 58: goto st403;
		case 64: goto st113;
		case 65: goto tr1464;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1465;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 97: goto tr1466;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1467;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1469;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1235;
tr1529:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1199;
st1199:
	if ( ++p == pe )
		goto _test_eof1199;
case 1199:
#line 25263 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st370;
		case 39: goto st380;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1316;
		case 46: goto st362;
		case 47: goto st104;
		case 48: goto tr1470;
		case 64: goto st113;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1322;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1337;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1301;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1005;
tr934:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1200;
st1200:
	if ( ++p == pe )
		goto _test_eof1200;
case 1200:
#line 25338 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st100;
		case -50: goto st101;
		case -30: goto st222;
		case 38: goto st351;
		case 39: goto st273;
		case 43: goto st112;
		case 44: goto st103;
		case 45: goto tr1422;
		case 46: goto st381;
		case 47: goto st397;
		case 48: goto tr1529;
		case 58: goto st403;
		case 64: goto st113;
		case 65: goto tr1464;
		case 67: goto tr1317;
		case 70: goto tr1317;
		case 71: goto tr1318;
		case 72: goto tr1319;
		case 75: goto tr1320;
		case 76: goto tr1317;
		case 77: goto tr1321;
		case 80: goto tr1465;
		case 82: goto tr1323;
		case 84: goto tr1324;
		case 95: goto st112;
		case 97: goto tr1466;
		case 98: goto tr1325;
		case 99: goto tr1326;
		case 100: goto tr1327;
		case 102: goto tr1328;
		case 103: goto tr1329;
		case 104: goto tr1330;
		case 105: goto tr1331;
		case 107: goto tr1332;
		case 108: goto tr1333;
		case 109: goto tr1334;
		case 110: goto tr1335;
		case 111: goto tr1336;
		case 112: goto tr1467;
		case 113: goto tr1338;
		case 114: goto tr1339;
		case 115: goto tr1340;
		case 116: goto tr1341;
		case 117: goto tr1342;
		case 121: goto tr1343;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1469;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr434;
		} else
			goto tr433;
	} else
		goto st8;
	goto tr1235;
tr935:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st1201;
st1201:
	if ( ++p == pe )
		goto _test_eof1201;
case 1201:
#line 25414 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 33: goto st439;
		case 47: goto st447;
		case 62: goto tr529;
		case 83: goto st455;
		case 104: goto st482;
		case 108: goto st484;
		case 111: goto st486;
		case 112: goto st488;
		case 115: goto st455;
		case 117: goto st486;
	}
	goto st438;
st438:
	if ( ++p == pe )
		goto _test_eof438;
case 438:
	if ( (*p) == 62 )
		goto tr529;
	goto st438;
st439:
	if ( ++p == pe )
		goto _test_eof439;
case 439:
	switch( (*p) ) {
		case 45: goto st440;
		case 62: goto tr529;
	}
	goto st438;
st440:
	if ( ++p == pe )
		goto _test_eof440;
case 440:
	switch( (*p) ) {
		case 45: goto st441;
		case 62: goto tr529;
	}
	goto st438;
st441:
	if ( ++p == pe )
		goto _test_eof441;
case 441:
	switch( (*p) ) {
		case 45: goto st442;
		case 62: goto tr533;
	}
	goto st441;
st442:
	if ( ++p == pe )
		goto _test_eof442;
case 442:
	switch( (*p) ) {
		case 45: goto st443;
		case 62: goto tr533;
	}
	goto st441;
st443:
	if ( ++p == pe )
		goto _test_eof443;
case 443:
	switch( (*p) ) {
		case 45: goto st443;
		case 62: goto tr529;
	}
	goto st441;
tr533:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st1202;
st1202:
	if ( ++p == pe )
		goto _test_eof1202;
case 1202:
#line 25488 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 45 )
		goto st445;
	goto st444;
st444:
	if ( ++p == pe )
		goto _test_eof444;
case 444:
	if ( (*p) == 45 )
		goto st445;
	goto st444;
st445:
	if ( ++p == pe )
		goto _test_eof445;
case 445:
	if ( (*p) == 45 )
		goto st446;
	goto st444;
st446:
	if ( ++p == pe )
		goto _test_eof446;
case 446:
	switch( (*p) ) {
		case 45: goto st446;
		case 62: goto tr529;
	}
	goto st444;
st447:
	if ( ++p == pe )
		goto _test_eof447;
case 447:
	switch( (*p) ) {
		case 62: goto tr529;
		case 104: goto st448;
		case 108: goto st450;
		case 111: goto st452;
		case 112: goto st454;
		case 117: goto st452;
	}
	goto st438;
st448:
	if ( ++p == pe )
		goto _test_eof448;
case 448:
	if ( (*p) == 62 )
		goto tr529;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st449;
	goto st438;
st449:
	if ( ++p == pe )
		goto _test_eof449;
case 449:
	if ( (*p) == 62 )
		goto tr544;
	goto st449;
st450:
	if ( ++p == pe )
		goto _test_eof450;
case 450:
	switch( (*p) ) {
		case 62: goto tr529;
		case 105: goto st451;
	}
	goto st438;
st451:
	if ( ++p == pe )
		goto _test_eof451;
case 451:
	if ( (*p) == 62 )
		goto tr546;
	goto st451;
st452:
	if ( ++p == pe )
		goto _test_eof452;
case 452:
	switch( (*p) ) {
		case 62: goto tr529;
		case 108: goto st453;
	}
	goto st438;
st453:
	if ( ++p == pe )
		goto _test_eof453;
case 453:
	if ( (*p) == 62 )
		goto tr548;
	goto st453;
st454:
	if ( ++p == pe )
		goto _test_eof454;
case 454:
	if ( (*p) == 62 )
		goto tr549;
	goto st454;
st455:
	if ( ++p == pe )
		goto _test_eof455;
case 455:
	switch( (*p) ) {
		case 62: goto tr529;
		case 67: goto st456;
		case 84: goto st470;
		case 99: goto st456;
		case 116: goto st470;
	}
	goto st438;
st456:
	if ( ++p == pe )
		goto _test_eof456;
case 456:
	switch( (*p) ) {
		case 62: goto tr529;
		case 82: goto st457;
		case 114: goto st457;
	}
	goto st438;
st457:
	if ( ++p == pe )
		goto _test_eof457;
case 457:
	switch( (*p) ) {
		case 62: goto tr529;
		case 73: goto st458;
		case 105: goto st458;
	}
	goto st438;
st458:
	if ( ++p == pe )
		goto _test_eof458;
case 458:
	switch( (*p) ) {
		case 62: goto tr529;
		case 80: goto st459;
		case 112: goto st459;
	}
	goto st438;
st459:
	if ( ++p == pe )
		goto _test_eof459;
case 459:
	switch( (*p) ) {
		case 62: goto tr529;
		case 84: goto st460;
		case 116: goto st460;
	}
	goto st438;
st460:
	if ( ++p == pe )
		goto _test_eof460;
case 460:
	if ( (*p) == 62 )
		goto tr556;
	goto st460;
tr556:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st1203;
st1203:
	if ( ++p == pe )
		goto _test_eof1203;
case 1203:
#line 25650 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 60 )
		goto st462;
	goto st461;
st461:
	if ( ++p == pe )
		goto _test_eof461;
case 461:
	if ( (*p) == 60 )
		goto st462;
	goto st461;
st462:
	if ( ++p == pe )
		goto _test_eof462;
case 462:
	switch( (*p) ) {
		case 47: goto st463;
		case 60: goto st462;
	}
	goto st461;
st463:
	if ( ++p == pe )
		goto _test_eof463;
case 463:
	switch( (*p) ) {
		case 60: goto st462;
		case 83: goto st464;
		case 115: goto st464;
	}
	goto st461;
st464:
	if ( ++p == pe )
		goto _test_eof464;
case 464:
	switch( (*p) ) {
		case 60: goto st462;
		case 67: goto st465;
		case 99: goto st465;
	}
	goto st461;
st465:
	if ( ++p == pe )
		goto _test_eof465;
case 465:
	switch( (*p) ) {
		case 60: goto st462;
		case 82: goto st466;
		case 114: goto st466;
	}
	goto st461;
st466:
	if ( ++p == pe )
		goto _test_eof466;
case 466:
	switch( (*p) ) {
		case 60: goto st462;
		case 73: goto st467;
		case 105: goto st467;
	}
	goto st461;
st467:
	if ( ++p == pe )
		goto _test_eof467;
case 467:
	switch( (*p) ) {
		case 60: goto st462;
		case 80: goto st468;
		case 112: goto st468;
	}
	goto st461;
st468:
	if ( ++p == pe )
		goto _test_eof468;
case 468:
	switch( (*p) ) {
		case 60: goto st462;
		case 84: goto st469;
		case 116: goto st469;
	}
	goto st461;
st469:
	if ( ++p == pe )
		goto _test_eof469;
case 469:
	switch( (*p) ) {
		case 60: goto st462;
		case 62: goto tr529;
	}
	goto st461;
st470:
	if ( ++p == pe )
		goto _test_eof470;
case 470:
	switch( (*p) ) {
		case 62: goto tr529;
		case 89: goto st471;
		case 121: goto st471;
	}
	goto st438;
st471:
	if ( ++p == pe )
		goto _test_eof471;
case 471:
	switch( (*p) ) {
		case 62: goto tr529;
		case 76: goto st472;
		case 108: goto st472;
	}
	goto st438;
st472:
	if ( ++p == pe )
		goto _test_eof472;
case 472:
	switch( (*p) ) {
		case 62: goto tr529;
		case 69: goto st473;
		case 101: goto st473;
	}
	goto st438;
st473:
	if ( ++p == pe )
		goto _test_eof473;
case 473:
	if ( (*p) == 62 )
		goto tr569;
	goto st473;
tr569:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st1204;
st1204:
	if ( ++p == pe )
		goto _test_eof1204;
case 1204:
#line 25784 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 60 )
		goto st475;
	goto st474;
st474:
	if ( ++p == pe )
		goto _test_eof474;
case 474:
	if ( (*p) == 60 )
		goto st475;
	goto st474;
st475:
	if ( ++p == pe )
		goto _test_eof475;
case 475:
	switch( (*p) ) {
		case 47: goto st476;
		case 60: goto st475;
	}
	goto st474;
st476:
	if ( ++p == pe )
		goto _test_eof476;
case 476:
	switch( (*p) ) {
		case 60: goto st475;
		case 83: goto st477;
		case 115: goto st477;
	}
	goto st474;
st477:
	if ( ++p == pe )
		goto _test_eof477;
case 477:
	switch( (*p) ) {
		case 60: goto st475;
		case 84: goto st478;
		case 116: goto st478;
	}
	goto st474;
st478:
	if ( ++p == pe )
		goto _test_eof478;
case 478:
	switch( (*p) ) {
		case 60: goto st475;
		case 89: goto st479;
		case 121: goto st479;
	}
	goto st474;
st479:
	if ( ++p == pe )
		goto _test_eof479;
case 479:
	switch( (*p) ) {
		case 60: goto st475;
		case 76: goto st480;
		case 108: goto st480;
	}
	goto st474;
st480:
	if ( ++p == pe )
		goto _test_eof480;
case 480:
	switch( (*p) ) {
		case 60: goto st475;
		case 69: goto st481;
		case 101: goto st481;
	}
	goto st474;
st481:
	if ( ++p == pe )
		goto _test_eof481;
case 481:
	switch( (*p) ) {
		case 60: goto st475;
		case 62: goto tr529;
	}
	goto st474;
st482:
	if ( ++p == pe )
		goto _test_eof482;
case 482:
	if ( (*p) == 62 )
		goto tr529;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st483;
	goto st438;
st483:
	if ( ++p == pe )
		goto _test_eof483;
case 483:
	if ( (*p) == 62 )
		goto tr579;
	goto st483;
st484:
	if ( ++p == pe )
		goto _test_eof484;
case 484:
	switch( (*p) ) {
		case 62: goto tr529;
		case 105: goto st485;
	}
	goto st438;
st485:
	if ( ++p == pe )
		goto _test_eof485;
case 485:
	if ( (*p) == 62 )
		goto tr581;
	goto st485;
st486:
	if ( ++p == pe )
		goto _test_eof486;
case 486:
	switch( (*p) ) {
		case 62: goto tr529;
		case 108: goto st487;
	}
	goto st438;
st487:
	if ( ++p == pe )
		goto _test_eof487;
case 487:
	if ( (*p) == 62 )
		goto tr583;
	goto st487;
st488:
	if ( ++p == pe )
		goto _test_eof488;
case 488:
	switch( (*p) ) {
		case 32: goto st488;
		case 47: goto st490;
		case 62: goto tr587;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st488;
	goto st489;
st489:
	if ( ++p == pe )
		goto _test_eof489;
case 489:
	if ( (*p) == 62 )
		goto tr587;
	goto st489;
st490:
	if ( ++p == pe )
		goto _test_eof490;
case 490:
	if ( (*p) == 62 )
		goto tr587;
	goto st489;
tr936:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st1205;
st1205:
	if ( ++p == pe )
		goto _test_eof1205;
case 1205:
#line 25945 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 61 )
		goto st491;
	goto tr1158;
st491:
	if ( ++p == pe )
		goto _test_eof491;
case 491:
	if ( (*p) == 61 )
		goto st1206;
	goto tr527;
st1206:
	if ( ++p == pe )
		goto _test_eof1206;
case 1206:
	if ( (*p) == 61 )
		goto st1206;
	goto tr1005;
st1207:
	if ( ++p == pe )
		goto _test_eof1207;
case 1207:
	if ( (*p) == 95 )
		goto st1208;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1208;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1208;
	} else
		goto st1208;
	goto tr1158;
st1208:
	if ( ++p == pe )
		goto _test_eof1208;
case 1208:
	if ( (*p) == 95 )
		goto st1209;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1209;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1209;
	} else
		goto st1209;
	goto tr1156;
st1209:
	if ( ++p == pe )
		goto _test_eof1209;
case 1209:
	if ( (*p) == 95 )
		goto st1210;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1210;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1210;
	} else
		goto st1210;
	goto tr1156;
st1210:
	if ( ++p == pe )
		goto _test_eof1210;
case 1210:
	if ( (*p) == 95 )
		goto st1211;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1211;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1211;
	} else
		goto st1211;
	goto tr1156;
st1211:
	if ( ++p == pe )
		goto _test_eof1211;
case 1211:
	if ( (*p) == 95 )
		goto st1212;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1212;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1212;
	} else
		goto st1212;
	goto tr1156;
st1212:
	if ( ++p == pe )
		goto _test_eof1212;
case 1212:
	if ( (*p) == 95 )
		goto st1213;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1213;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1213;
	} else
		goto st1213;
	goto tr1156;
st1213:
	if ( ++p == pe )
		goto _test_eof1213;
case 1213:
	if ( (*p) == 95 )
		goto st1214;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1214;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1214;
	} else
		goto st1214;
	goto tr1156;
st1214:
	if ( ++p == pe )
		goto _test_eof1214;
case 1214:
	if ( (*p) == 95 )
		goto st1215;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1215;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1215;
	} else
		goto st1215;
	goto tr1156;
st1215:
	if ( ++p == pe )
		goto _test_eof1215;
case 1215:
	if ( (*p) == 95 )
		goto st1216;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1216;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1216;
	} else
		goto st1216;
	goto tr1156;
st1216:
	if ( ++p == pe )
		goto _test_eof1216;
case 1216:
	if ( (*p) == 95 )
		goto st1217;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1217;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1217;
	} else
		goto st1217;
	goto tr1156;
st1217:
	if ( ++p == pe )
		goto _test_eof1217;
case 1217:
	if ( (*p) == 95 )
		goto st1218;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1218;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1218;
	} else
		goto st1218;
	goto tr1156;
st1218:
	if ( ++p == pe )
		goto _test_eof1218;
case 1218:
	if ( (*p) == 95 )
		goto st1219;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1219;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1219;
	} else
		goto st1219;
	goto tr1156;
st1219:
	if ( ++p == pe )
		goto _test_eof1219;
case 1219:
	if ( (*p) == 95 )
		goto st1220;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1220;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1220;
	} else
		goto st1220;
	goto tr1156;
st1220:
	if ( ++p == pe )
		goto _test_eof1220;
case 1220:
	if ( (*p) == 95 )
		goto st1221;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st1221;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1221;
	} else
		goto st1221;
	goto tr1156;
st1221:
	if ( ++p == pe )
		goto _test_eof1221;
case 1221:
	if ( (*p) == 95 )
		goto tr1239;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1239;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr1239;
	} else
		goto tr1239;
	goto tr1156;
tr939:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1222;
st1222:
	if ( ++p == pe )
		goto _test_eof1222;
case 1222:
#line 26198 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1554;
		case 64: goto st113;
		case 73: goto tr1556;
		case 78: goto tr1557;
		case 82: goto tr1558;
		case 95: goto st112;
		case 97: goto tr596;
		case 100: goto tr1560;
		case 101: goto tr596;
		case 105: goto tr1561;
		case 108: goto tr1562;
		case 111: goto tr596;
		case 112: goto tr1563;
		case 114: goto tr1564;
		case 115: goto tr1565;
		case 117: goto tr1566;
		case 118: goto tr1567;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
st492:
	if ( ++p == pe )
		goto _test_eof492;
case 492:
	switch( (*p) ) {
		case 35: goto st237;
		case 38: goto st493;
		case 97: goto st270;
		case 114: goto st311;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr590;
	goto tr0;
st493:
	if ( ++p == pe )
		goto _test_eof493;
case 493:
	if ( (*p) == 38 )
		goto st493;
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr591;
	goto tr0;
tr591:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1223;
st1223:
	if ( ++p == pe )
		goto _test_eof1223;
case 1223:
#line 26279 "src/lib/schwa/tokenizer/tokenizer.cc"
	if ( (*p) == 38 )
		goto st493;
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr591;
	goto tr1568;
tr590:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1224;
st1224:
	if ( ++p == pe )
		goto _test_eof1224;
case 1224:
#line 26295 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st494;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr1570;
	} else
		goto st8;
	goto tr1568;
st494:
	if ( ++p == pe )
		goto _test_eof494;
case 494:
	switch( (*p) ) {
		case 35: goto st237;
		case 38: goto st493;
		case 97: goto st270;
		case 114: goto st311;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr591;
	goto tr592;
tr1570:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1225;
st1225:
	if ( ++p == pe )
		goto _test_eof1225;
case 1225:
#line 26347 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st492;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr1570;
	} else
		goto st8;
	goto tr1568;
tr1553:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1226;
st1226:
	if ( ++p == pe )
		goto _test_eof1226;
case 1226:
#line 26386 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st495;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st495:
	if ( ++p == pe )
		goto _test_eof495;
case 495:
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st495;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr10;
tr594:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1227;
st1227:
	if ( ++p == pe )
		goto _test_eof1227;
case 1227:
#line 26463 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st496:
	if ( ++p == pe )
		goto _test_eof496;
case 496:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto st497;
		case 66: goto st501;
		case 67: goto st502;
		case 69: goto st505;
		case 71: goto st506;
		case 73: goto st507;
		case 74: goto st509;
		case 77: goto st511;
		case 78: goto st516;
		case 79: goto st518;
		case 80: goto st519;
		case 84: goto st520;
		case 88: goto st524;
		case 95: goto st112;
		case 97: goto st497;
		case 98: goto st501;
		case 99: goto st502;
		case 101: goto st505;
		case 103: goto st506;
		case 105: goto st507;
		case 106: goto st509;
		case 109: goto st511;
		case 110: goto st516;
		case 111: goto st518;
		case 112: goto st519;
		case 116: goto st520;
		case 120: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto st504;
		} else if ( (*p) >= 68 )
			goto st504;
	} else
		goto st112;
	goto tr10;
st497:
	if ( ++p == pe )
		goto _test_eof497;
case 497:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 69: goto tr615;
		case 82: goto tr616;
		case 83: goto tr617;
		case 95: goto st112;
		case 101: goto tr615;
		case 114: goto tr616;
		case 115: goto tr617;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
st498:
	if ( ++p == pe )
		goto _test_eof498;
case 498:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st112;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr0;
st499:
	if ( ++p == pe )
		goto _test_eof499;
case 499:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr0;
st500:
	if ( ++p == pe )
		goto _test_eof500;
case 500:
	switch( (*p) ) {
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto st497;
		case 66: goto st501;
		case 67: goto st502;
		case 69: goto st505;
		case 71: goto st506;
		case 73: goto st507;
		case 74: goto st509;
		case 77: goto st511;
		case 78: goto st516;
		case 79: goto st518;
		case 80: goto st519;
		case 84: goto st520;
		case 88: goto st524;
		case 95: goto st112;
		case 97: goto st497;
		case 98: goto st501;
		case 99: goto st502;
		case 101: goto st505;
		case 103: goto st506;
		case 105: goto st507;
		case 106: goto st509;
		case 109: goto st511;
		case 110: goto st516;
		case 111: goto st518;
		case 112: goto st519;
		case 116: goto st520;
		case 120: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto st504;
		} else if ( (*p) >= 68 )
			goto st504;
	} else
		goto st112;
	goto tr0;
st501:
	if ( ++p == pe )
		goto _test_eof501;
case 501:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 73: goto tr618;
		case 95: goto st112;
		case 105: goto tr618;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr614:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1228;
st1228:
	if ( ++p == pe )
		goto _test_eof1228;
case 1228:
#line 26691 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
tr618:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1229;
st1229:
	if ( ++p == pe )
		goto _test_eof1229;
case 1229:
#line 26718 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 90: goto tr614;
		case 95: goto st112;
		case 122: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 89 ) {
		if ( 97 <= (*p) && (*p) <= 121 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st502:
	if ( ++p == pe )
		goto _test_eof502;
case 502:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 65: goto tr619;
		case 79: goto tr620;
		case 95: goto st112;
		case 97: goto tr619;
		case 111: goto tr620;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr619:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1230;
st1230:
	if ( ++p == pe )
		goto _test_eof1230;
case 1230:
#line 26771 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 84: goto tr614;
		case 95: goto st112;
		case 116: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
tr620:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1231;
st1231:
	if ( ++p == pe )
		goto _test_eof1231;
case 1231:
#line 26800 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 77: goto tr614;
		case 79: goto st503;
		case 95: goto st112;
		case 109: goto tr614;
		case 111: goto st503;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st503:
	if ( ++p == pe )
		goto _test_eof503;
case 503:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 80: goto tr614;
		case 95: goto st112;
		case 112: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
st504:
	if ( ++p == pe )
		goto _test_eof504;
case 504:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
st505:
	if ( ++p == pe )
		goto _test_eof505;
case 505:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 68: goto tr621;
		case 95: goto st112;
		case 100: goto tr621;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr621:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1232;
st1232:
	if ( ++p == pe )
		goto _test_eof1232;
case 1232:
#line 26895 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 85: goto tr614;
		case 95: goto st112;
		case 117: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st506:
	if ( ++p == pe )
		goto _test_eof506;
case 506:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 79: goto tr622;
		case 95: goto st112;
		case 111: goto tr622;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr622:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1233;
st1233:
	if ( ++p == pe )
		goto _test_eof1233;
case 1233:
#line 26946 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 86: goto tr614;
		case 95: goto st112;
		case 118: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st507:
	if ( ++p == pe )
		goto _test_eof507;
case 507:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 78: goto tr623;
		case 95: goto st112;
		case 110: goto tr623;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr623:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1234;
st1234:
	if ( ++p == pe )
		goto _test_eof1234;
case 1234:
#line 26997 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 70: goto st508;
		case 84: goto tr614;
		case 95: goto st112;
		case 102: goto st508;
		case 116: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st508:
	if ( ++p == pe )
		goto _test_eof508;
case 508:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 79: goto tr614;
		case 95: goto st112;
		case 111: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
st509:
	if ( ++p == pe )
		goto _test_eof509;
case 509:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 79: goto tr624;
		case 95: goto st112;
		case 111: goto tr624;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr624:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1235;
st1235:
	if ( ++p == pe )
		goto _test_eof1235;
case 1235:
#line 27072 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 66: goto st510;
		case 95: goto st112;
		case 98: goto st510;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st510:
	if ( ++p == pe )
		goto _test_eof510;
case 510:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 83: goto tr614;
		case 95: goto st112;
		case 115: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
st511:
	if ( ++p == pe )
		goto _test_eof511;
case 511:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 73: goto tr625;
		case 79: goto tr626;
		case 85: goto tr627;
		case 95: goto st112;
		case 105: goto tr625;
		case 111: goto tr626;
		case 117: goto tr627;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr625:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1236;
st1236:
	if ( ++p == pe )
		goto _test_eof1236;
case 1236:
#line 27149 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 76: goto tr614;
		case 95: goto st112;
		case 108: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
tr626:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1237;
st1237:
	if ( ++p == pe )
		goto _test_eof1237;
case 1237:
#line 27178 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 66: goto st512;
		case 95: goto st112;
		case 98: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st512:
	if ( ++p == pe )
		goto _test_eof512;
case 512:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 73: goto tr614;
		case 95: goto st112;
		case 105: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
tr627:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1238;
st1238:
	if ( ++p == pe )
		goto _test_eof1238;
case 1238:
#line 27229 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 83: goto st513;
		case 95: goto st112;
		case 115: goto st513;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st513:
	if ( ++p == pe )
		goto _test_eof513;
case 513:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 69: goto st514;
		case 95: goto st112;
		case 101: goto st514;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
st514:
	if ( ++p == pe )
		goto _test_eof514;
case 514:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 85: goto st515;
		case 95: goto st112;
		case 117: goto st515;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
st515:
	if ( ++p == pe )
		goto _test_eof515;
case 515:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 77: goto tr614;
		case 95: goto st112;
		case 109: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
st516:
	if ( ++p == pe )
		goto _test_eof516;
case 516:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 65: goto tr630;
		case 69: goto tr619;
		case 95: goto st112;
		case 97: goto tr630;
		case 101: goto tr619;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr630:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1239;
st1239:
	if ( ++p == pe )
		goto _test_eof1239;
case 1239:
#line 27348 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 77: goto st517;
		case 95: goto st112;
		case 109: goto st517;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st517:
	if ( ++p == pe )
		goto _test_eof517;
case 517:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 69: goto tr614;
		case 95: goto st112;
		case 101: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
st518:
	if ( ++p == pe )
		goto _test_eof518;
case 518:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 82: goto tr631;
		case 95: goto st112;
		case 114: goto tr631;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr631:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1240;
st1240:
	if ( ++p == pe )
		goto _test_eof1240;
case 1240:
#line 27421 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 71: goto tr614;
		case 95: goto st112;
		case 103: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st519:
	if ( ++p == pe )
		goto _test_eof519;
case 519:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 82: goto tr632;
		case 95: goto st112;
		case 114: goto tr632;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr632:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1241;
st1241:
	if ( ++p == pe )
		goto _test_eof1241;
case 1241:
#line 27472 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 79: goto tr614;
		case 95: goto st112;
		case 111: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st520:
	if ( ++p == pe )
		goto _test_eof520;
case 520:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 69: goto tr625;
		case 82: goto tr633;
		case 95: goto st112;
		case 101: goto tr625;
		case 114: goto tr633;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr633:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1242;
st1242:
	if ( ++p == pe )
		goto _test_eof1242;
case 1242:
#line 27525 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 65: goto st521;
		case 95: goto st112;
		case 97: goto st521;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st521:
	if ( ++p == pe )
		goto _test_eof521;
case 521:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 86: goto st522;
		case 95: goto st112;
		case 118: goto st522;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
st522:
	if ( ++p == pe )
		goto _test_eof522;
case 522:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 69: goto st523;
		case 95: goto st112;
		case 101: goto st523;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
st523:
	if ( ++p == pe )
		goto _test_eof523;
case 523:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 76: goto tr614;
		case 95: goto st112;
		case 108: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
st524:
	if ( ++p == pe )
		goto _test_eof524;
case 524:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 88: goto tr636;
		case 95: goto st112;
		case 120: goto tr636;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr614;
	} else
		goto tr614;
	goto tr0;
tr636:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1243;
st1243:
	if ( ++p == pe )
		goto _test_eof1243;
case 1243:
#line 27642 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 88: goto tr614;
		case 95: goto st112;
		case 120: goto tr614;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
tr615:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1244;
st1244:
	if ( ++p == pe )
		goto _test_eof1244;
case 1244:
#line 27671 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 82: goto st508;
		case 95: goto st112;
		case 114: goto st508;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
tr616:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1245;
st1245:
	if ( ++p == pe )
		goto _test_eof1245;
case 1245:
#line 27700 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 80: goto st525;
		case 95: goto st112;
		case 112: goto st525;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
st525:
	if ( ++p == pe )
		goto _test_eof525;
case 525:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 65: goto tr614;
		case 95: goto st112;
		case 97: goto tr614;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr123;
tr617:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1246;
st1246:
	if ( ++p == pe )
		goto _test_eof1246;
case 1246:
#line 27751 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st498;
		case 46: goto st500;
		case 64: goto st113;
		case 73: goto st525;
		case 95: goto st112;
		case 105: goto st525;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st499;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st499;
	} else
		goto st499;
	goto tr1156;
tr595:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1247;
tr1581:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1247;
st1247:
	if ( ++p == pe )
		goto _test_eof1247;
case 1247:
#line 27786 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr0;
st526:
	if ( ++p == pe )
		goto _test_eof526;
case 526:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto tr637;
		case 66: goto tr638;
		case 67: goto tr639;
		case 69: goto tr641;
		case 71: goto tr642;
		case 73: goto tr643;
		case 74: goto tr644;
		case 77: goto tr645;
		case 78: goto tr646;
		case 79: goto tr647;
		case 80: goto tr648;
		case 84: goto tr649;
		case 88: goto tr650;
		case 95: goto st112;
		case 97: goto tr637;
		case 98: goto tr638;
		case 99: goto tr639;
		case 101: goto tr641;
		case 103: goto tr642;
		case 105: goto tr643;
		case 106: goto tr644;
		case 109: goto tr645;
		case 110: goto tr646;
		case 111: goto tr647;
		case 112: goto tr648;
		case 116: goto tr649;
		case 120: goto tr650;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto tr640;
		} else if ( (*p) >= 68 )
			goto tr640;
	} else
		goto st112;
	goto tr0;
tr637:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1248;
st1248:
	if ( ++p == pe )
		goto _test_eof1248;
case 1248:
#line 27879 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 69: goto tr1582;
		case 82: goto tr1583;
		case 83: goto tr1584;
		case 95: goto st112;
		case 101: goto tr1586;
		case 114: goto tr1587;
		case 115: goto tr1588;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1582:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1249;
st1249:
	if ( ++p == pe )
		goto _test_eof1249;
case 1249:
#line 27927 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 82: goto tr1589;
		case 95: goto st112;
		case 114: goto tr1590;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1589:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1250;
tr1631:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1250;
st1250:
	if ( ++p == pe )
		goto _test_eof1250;
case 1250:
#line 27977 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 79: goto tr1581;
		case 95: goto st112;
		case 111: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr0;
tr596:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1251;
tr1585:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1251;
tr1676:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 42 "src/ragel/rules/contractions.rl"
	{n2="n't";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 25;}
	goto st1251;
st1251:
	if ( ++p == pe )
		goto _test_eof1251;
case 1251:
#line 28037 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr0;
tr1590:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1252;
tr1632:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1252;
st1252:
	if ( ++p == pe )
		goto _test_eof1252;
case 1252:
#line 28085 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 79: goto tr1581;
		case 95: goto st112;
		case 111: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr0;
tr1583:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1253;
st1253:
	if ( ++p == pe )
		goto _test_eof1253;
case 1253:
#line 28129 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1591;
		case 95: goto st112;
		case 112: goto tr1592;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1591:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1254;
st1254:
	if ( ++p == pe )
		goto _test_eof1254;
case 1254:
#line 28173 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1581;
		case 95: goto st112;
		case 97: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1592:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1255;
st1255:
	if ( ++p == pe )
		goto _test_eof1255;
case 1255:
#line 28217 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1581;
		case 95: goto st112;
		case 97: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1584:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1256;
st1256:
	if ( ++p == pe )
		goto _test_eof1256;
case 1256:
#line 28261 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 73: goto tr1591;
		case 95: goto st112;
		case 105: goto tr1592;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1586:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1257;
st1257:
	if ( ++p == pe )
		goto _test_eof1257;
case 1257:
#line 28305 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 82: goto tr1589;
		case 95: goto st112;
		case 114: goto tr1590;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1587:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1258;
st1258:
	if ( ++p == pe )
		goto _test_eof1258;
case 1258:
#line 28349 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1591;
		case 95: goto st112;
		case 112: goto tr1592;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1588:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1259;
st1259:
	if ( ++p == pe )
		goto _test_eof1259;
case 1259:
#line 28393 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 73: goto tr1591;
		case 95: goto st112;
		case 105: goto tr1592;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr638:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1260;
st1260:
	if ( ++p == pe )
		goto _test_eof1260;
case 1260:
#line 28437 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 73: goto tr1593;
		case 95: goto st112;
		case 105: goto tr1594;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1593:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1261;
st1261:
	if ( ++p == pe )
		goto _test_eof1261;
case 1261:
#line 28481 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 90: goto tr1581;
		case 95: goto st112;
		case 122: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 89 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1594:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1262;
st1262:
	if ( ++p == pe )
		goto _test_eof1262;
case 1262:
#line 28525 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 90: goto tr1581;
		case 95: goto st112;
		case 122: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 89 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr639:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1263;
st1263:
	if ( ++p == pe )
		goto _test_eof1263;
case 1263:
#line 28569 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 65: goto tr1595;
		case 79: goto tr1596;
		case 95: goto st112;
		case 97: goto tr1597;
		case 111: goto tr1598;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1595:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1264;
st1264:
	if ( ++p == pe )
		goto _test_eof1264;
case 1264:
#line 28615 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1581;
		case 95: goto st112;
		case 116: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1596:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1265;
st1265:
	if ( ++p == pe )
		goto _test_eof1265;
case 1265:
#line 28659 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1581;
		case 79: goto tr1599;
		case 95: goto st112;
		case 109: goto tr1585;
		case 111: goto tr1600;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1599:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1266;
st1266:
	if ( ++p == pe )
		goto _test_eof1266;
case 1266:
#line 28705 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1581;
		case 95: goto st112;
		case 112: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1600:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1267;
st1267:
	if ( ++p == pe )
		goto _test_eof1267;
case 1267:
#line 28749 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1581;
		case 95: goto st112;
		case 112: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1597:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1268;
st1268:
	if ( ++p == pe )
		goto _test_eof1268;
case 1268:
#line 28793 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1581;
		case 95: goto st112;
		case 116: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1598:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1269;
st1269:
	if ( ++p == pe )
		goto _test_eof1269;
case 1269:
#line 28837 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1581;
		case 79: goto tr1599;
		case 95: goto st112;
		case 109: goto tr1585;
		case 111: goto tr1600;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr640:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1270;
st1270:
	if ( ++p == pe )
		goto _test_eof1270;
case 1270:
#line 28883 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr641:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1271;
st1271:
	if ( ++p == pe )
		goto _test_eof1271;
case 1271:
#line 28925 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 68: goto tr1601;
		case 95: goto st112;
		case 100: goto tr1602;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1601:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1272;
st1272:
	if ( ++p == pe )
		goto _test_eof1272;
case 1272:
#line 28969 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 85: goto tr1581;
		case 95: goto st112;
		case 117: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1602:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1273;
st1273:
	if ( ++p == pe )
		goto _test_eof1273;
case 1273:
#line 29013 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 85: goto tr1581;
		case 95: goto st112;
		case 117: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr642:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1274;
st1274:
	if ( ++p == pe )
		goto _test_eof1274;
case 1274:
#line 29057 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 79: goto tr1603;
		case 95: goto st112;
		case 111: goto tr1604;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1603:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1275;
st1275:
	if ( ++p == pe )
		goto _test_eof1275;
case 1275:
#line 29101 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 86: goto tr1581;
		case 95: goto st112;
		case 118: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1604:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1276;
st1276:
	if ( ++p == pe )
		goto _test_eof1276;
case 1276:
#line 29145 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 86: goto tr1581;
		case 95: goto st112;
		case 118: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr643:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1277;
st1277:
	if ( ++p == pe )
		goto _test_eof1277;
case 1277:
#line 29189 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 78: goto tr1605;
		case 95: goto st112;
		case 110: goto tr1606;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1605:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1278;
st1278:
	if ( ++p == pe )
		goto _test_eof1278;
case 1278:
#line 29233 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 70: goto tr1589;
		case 84: goto tr1581;
		case 95: goto st112;
		case 102: goto tr1590;
		case 116: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1606:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1279;
st1279:
	if ( ++p == pe )
		goto _test_eof1279;
case 1279:
#line 29279 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 70: goto tr1589;
		case 84: goto tr1581;
		case 95: goto st112;
		case 102: goto tr1590;
		case 116: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr644:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1280;
st1280:
	if ( ++p == pe )
		goto _test_eof1280;
case 1280:
#line 29325 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 79: goto tr1607;
		case 95: goto st112;
		case 111: goto tr1608;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1607:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1281;
st1281:
	if ( ++p == pe )
		goto _test_eof1281;
case 1281:
#line 29369 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 66: goto tr1609;
		case 95: goto st112;
		case 98: goto tr1610;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1609:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1282;
st1282:
	if ( ++p == pe )
		goto _test_eof1282;
case 1282:
#line 29413 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1581;
		case 95: goto st112;
		case 115: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1610:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1283;
st1283:
	if ( ++p == pe )
		goto _test_eof1283;
case 1283:
#line 29457 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1581;
		case 95: goto st112;
		case 115: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1608:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1284;
st1284:
	if ( ++p == pe )
		goto _test_eof1284;
case 1284:
#line 29501 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 66: goto tr1609;
		case 95: goto st112;
		case 98: goto tr1610;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr645:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1285;
st1285:
	if ( ++p == pe )
		goto _test_eof1285;
case 1285:
#line 29545 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 73: goto tr1611;
		case 79: goto tr1612;
		case 85: goto tr1613;
		case 95: goto st112;
		case 105: goto tr1614;
		case 111: goto tr1615;
		case 117: goto tr1616;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1639:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1286;
tr1611:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1286;
st1286:
	if ( ++p == pe )
		goto _test_eof1286;
case 1286:
#line 29599 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1581;
		case 95: goto st112;
		case 108: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr0;
tr1612:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1287;
st1287:
	if ( ++p == pe )
		goto _test_eof1287;
case 1287:
#line 29643 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 66: goto tr1617;
		case 95: goto st112;
		case 98: goto tr1618;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1617:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1288;
st1288:
	if ( ++p == pe )
		goto _test_eof1288;
case 1288:
#line 29687 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 73: goto tr1581;
		case 95: goto st112;
		case 105: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1618:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1289;
st1289:
	if ( ++p == pe )
		goto _test_eof1289;
case 1289:
#line 29731 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 73: goto tr1581;
		case 95: goto st112;
		case 105: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1613:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1290;
st1290:
	if ( ++p == pe )
		goto _test_eof1290;
case 1290:
#line 29775 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1619;
		case 95: goto st112;
		case 115: goto tr1620;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1619:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1291;
st1291:
	if ( ++p == pe )
		goto _test_eof1291;
case 1291:
#line 29819 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1621;
		case 95: goto st112;
		case 101: goto tr1622;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1621:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1292;
st1292:
	if ( ++p == pe )
		goto _test_eof1292;
case 1292:
#line 29863 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 85: goto tr1623;
		case 95: goto st112;
		case 117: goto tr1624;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1623:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1293;
st1293:
	if ( ++p == pe )
		goto _test_eof1293;
case 1293:
#line 29907 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1581;
		case 95: goto st112;
		case 109: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1624:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1294;
st1294:
	if ( ++p == pe )
		goto _test_eof1294;
case 1294:
#line 29951 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1581;
		case 95: goto st112;
		case 109: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1622:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1295;
st1295:
	if ( ++p == pe )
		goto _test_eof1295;
case 1295:
#line 29995 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 85: goto tr1623;
		case 95: goto st112;
		case 117: goto tr1624;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1620:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1296;
st1296:
	if ( ++p == pe )
		goto _test_eof1296;
case 1296:
#line 30039 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1621;
		case 95: goto st112;
		case 101: goto tr1622;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1640:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1297;
tr1614:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1297;
st1297:
	if ( ++p == pe )
		goto _test_eof1297;
case 1297:
#line 30089 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1581;
		case 95: goto st112;
		case 108: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr0;
tr1615:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1298;
st1298:
	if ( ++p == pe )
		goto _test_eof1298;
case 1298:
#line 30133 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 66: goto tr1617;
		case 95: goto st112;
		case 98: goto tr1618;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1616:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1299;
st1299:
	if ( ++p == pe )
		goto _test_eof1299;
case 1299:
#line 30177 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1619;
		case 95: goto st112;
		case 115: goto tr1620;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr646:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1300;
st1300:
	if ( ++p == pe )
		goto _test_eof1300;
case 1300:
#line 30221 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 65: goto tr1625;
		case 69: goto tr1595;
		case 95: goto st112;
		case 97: goto tr1626;
		case 101: goto tr1597;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1625:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1301;
st1301:
	if ( ++p == pe )
		goto _test_eof1301;
case 1301:
#line 30267 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1627;
		case 95: goto st112;
		case 109: goto tr1628;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1627:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1302;
st1302:
	if ( ++p == pe )
		goto _test_eof1302;
case 1302:
#line 30311 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1581;
		case 95: goto st112;
		case 101: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1628:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1303;
st1303:
	if ( ++p == pe )
		goto _test_eof1303;
case 1303:
#line 30355 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1581;
		case 95: goto st112;
		case 101: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1626:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1304;
st1304:
	if ( ++p == pe )
		goto _test_eof1304;
case 1304:
#line 30399 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1627;
		case 95: goto st112;
		case 109: goto tr1628;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr647:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1305;
st1305:
	if ( ++p == pe )
		goto _test_eof1305;
case 1305:
#line 30443 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 82: goto tr1629;
		case 95: goto st112;
		case 114: goto tr1630;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1629:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1306;
st1306:
	if ( ++p == pe )
		goto _test_eof1306;
case 1306:
#line 30487 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 71: goto tr1581;
		case 95: goto st112;
		case 103: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1630:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1307;
st1307:
	if ( ++p == pe )
		goto _test_eof1307;
case 1307:
#line 30531 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 71: goto tr1581;
		case 95: goto st112;
		case 103: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr648:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1308;
st1308:
	if ( ++p == pe )
		goto _test_eof1308;
case 1308:
#line 30575 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 82: goto tr1631;
		case 95: goto st112;
		case 114: goto tr1632;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr649:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1309;
st1309:
	if ( ++p == pe )
		goto _test_eof1309;
case 1309:
#line 30619 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 69: goto tr1611;
		case 82: goto tr1633;
		case 95: goto st112;
		case 101: goto tr1614;
		case 114: goto tr1634;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1633:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1310;
st1310:
	if ( ++p == pe )
		goto _test_eof1310;
case 1310:
#line 30665 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1635;
		case 95: goto st112;
		case 97: goto tr1636;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1635:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1311;
st1311:
	if ( ++p == pe )
		goto _test_eof1311;
case 1311:
#line 30709 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 86: goto tr1637;
		case 95: goto st112;
		case 118: goto tr1638;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1637:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1312;
st1312:
	if ( ++p == pe )
		goto _test_eof1312;
case 1312:
#line 30753 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1639;
		case 95: goto st112;
		case 101: goto tr1640;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1638:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1313;
st1313:
	if ( ++p == pe )
		goto _test_eof1313;
case 1313:
#line 30797 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1639;
		case 95: goto st112;
		case 101: goto tr1640;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1636:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1314;
st1314:
	if ( ++p == pe )
		goto _test_eof1314;
case 1314:
#line 30841 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 86: goto tr1637;
		case 95: goto st112;
		case 118: goto tr1638;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1634:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1315;
st1315:
	if ( ++p == pe )
		goto _test_eof1315;
case 1315:
#line 30885 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1635;
		case 95: goto st112;
		case 97: goto tr1636;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr650:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1316;
st1316:
	if ( ++p == pe )
		goto _test_eof1316;
case 1316:
#line 30929 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st496;
		case 64: goto st113;
		case 88: goto tr1641;
		case 95: goto st112;
		case 120: goto tr1642;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1641:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1317;
st1317:
	if ( ++p == pe )
		goto _test_eof1317;
case 1317:
#line 30973 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 88: goto tr1581;
		case 95: goto st112;
		case 120: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1642:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 39;}
	goto st1318;
st1318:
	if ( ++p == pe )
		goto _test_eof1318;
case 1318:
#line 31017 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 88: goto tr1581;
		case 95: goto st112;
		case 120: goto tr1585;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr1156;
tr1554:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1319;
st1319:
	if ( ++p == pe )
		goto _test_eof1319;
case 1319:
#line 31061 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto tr749;
		case 66: goto tr750;
		case 67: goto tr751;
		case 69: goto tr753;
		case 71: goto tr754;
		case 73: goto tr755;
		case 74: goto tr756;
		case 77: goto tr763;
		case 78: goto tr758;
		case 79: goto tr759;
		case 80: goto tr760;
		case 84: goto tr761;
		case 88: goto tr762;
		case 95: goto st112;
		case 97: goto tr749;
		case 98: goto tr750;
		case 99: goto tr751;
		case 101: goto tr753;
		case 103: goto tr754;
		case 105: goto tr755;
		case 106: goto tr756;
		case 109: goto tr757;
		case 110: goto tr758;
		case 111: goto tr759;
		case 112: goto tr760;
		case 116: goto tr761;
		case 120: goto tr762;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto tr752;
		} else if ( (*p) >= 68 )
			goto tr752;
	} else
		goto st112;
	goto tr1568;
tr749:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1320;
st1320:
	if ( ++p == pe )
		goto _test_eof1320;
case 1320:
#line 31119 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 69: goto tr1582;
		case 82: goto tr1583;
		case 83: goto tr1584;
		case 95: goto st112;
		case 101: goto tr1586;
		case 114: goto tr1587;
		case 115: goto tr1588;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1643:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1321;
tr1644:
#line 1 "NONE"
	{ s.te = p+1;}
#line 61 "src/ragel/rules/date_time.rl"
	{n1="am";}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1321;
tr1898:
#line 1 "NONE"
	{ s.te = p+1;}
#line 61 "src/ragel/rules/date_time.rl"
	{n1="pm";}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 31;}
	goto st1321;
st1321:
	if ( ++p == pe )
		goto _test_eof1321;
case 1321:
#line 31183 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto st497;
		case 66: goto st501;
		case 67: goto st502;
		case 69: goto st505;
		case 71: goto st506;
		case 73: goto st507;
		case 74: goto st509;
		case 77: goto st511;
		case 78: goto st516;
		case 79: goto st518;
		case 80: goto st519;
		case 84: goto st520;
		case 88: goto st524;
		case 95: goto st112;
		case 97: goto st497;
		case 98: goto st501;
		case 99: goto st502;
		case 101: goto st505;
		case 103: goto st506;
		case 105: goto st507;
		case 106: goto st509;
		case 109: goto st511;
		case 110: goto st516;
		case 111: goto st518;
		case 112: goto st519;
		case 116: goto st520;
		case 120: goto st524;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto st504;
		} else if ( (*p) >= 68 )
			goto st504;
	} else
		goto st112;
	goto tr0;
tr750:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1322;
st1322:
	if ( ++p == pe )
		goto _test_eof1322;
case 1322:
#line 31241 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 73: goto tr1593;
		case 95: goto st112;
		case 105: goto tr1594;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr751:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1323;
st1323:
	if ( ++p == pe )
		goto _test_eof1323;
case 1323:
#line 31285 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 65: goto tr1595;
		case 79: goto tr1596;
		case 95: goto st112;
		case 97: goto tr1597;
		case 111: goto tr1598;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr752:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1324;
st1324:
	if ( ++p == pe )
		goto _test_eof1324;
case 1324:
#line 31331 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr753:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1325;
st1325:
	if ( ++p == pe )
		goto _test_eof1325;
case 1325:
#line 31373 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 68: goto tr1601;
		case 95: goto st112;
		case 100: goto tr1602;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr754:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1326;
st1326:
	if ( ++p == pe )
		goto _test_eof1326;
case 1326:
#line 31417 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 79: goto tr1603;
		case 95: goto st112;
		case 111: goto tr1604;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr755:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1327;
st1327:
	if ( ++p == pe )
		goto _test_eof1327;
case 1327:
#line 31461 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 78: goto tr1605;
		case 95: goto st112;
		case 110: goto tr1606;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr756:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1328;
st1328:
	if ( ++p == pe )
		goto _test_eof1328;
case 1328:
#line 31505 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 79: goto tr1607;
		case 95: goto st112;
		case 111: goto tr1608;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr763:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1329;
st1329:
	if ( ++p == pe )
		goto _test_eof1329;
case 1329:
#line 31549 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1644;
		case 64: goto st113;
		case 73: goto tr1611;
		case 79: goto tr1612;
		case 85: goto tr1613;
		case 95: goto st112;
		case 105: goto tr1614;
		case 111: goto tr1615;
		case 117: goto tr1616;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr758:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1330;
st1330:
	if ( ++p == pe )
		goto _test_eof1330;
case 1330:
#line 31597 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 65: goto tr1625;
		case 69: goto tr1595;
		case 95: goto st112;
		case 97: goto tr1626;
		case 101: goto tr1597;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr759:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1331;
st1331:
	if ( ++p == pe )
		goto _test_eof1331;
case 1331:
#line 31643 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 82: goto tr1629;
		case 95: goto st112;
		case 114: goto tr1630;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr760:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1332;
st1332:
	if ( ++p == pe )
		goto _test_eof1332;
case 1332:
#line 31687 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 82: goto tr1631;
		case 95: goto st112;
		case 114: goto tr1632;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr761:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1333;
st1333:
	if ( ++p == pe )
		goto _test_eof1333;
case 1333:
#line 31731 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 69: goto tr1611;
		case 82: goto tr1633;
		case 95: goto st112;
		case 101: goto tr1614;
		case 114: goto tr1634;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr762:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1334;
st1334:
	if ( ++p == pe )
		goto _test_eof1334;
case 1334:
#line 31777 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 88: goto tr1641;
		case 95: goto st112;
		case 120: goto tr1642;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr757:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1335;
st1335:
	if ( ++p == pe )
		goto _test_eof1335;
case 1335:
#line 31821 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1643;
		case 64: goto st113;
		case 73: goto tr1611;
		case 79: goto tr1612;
		case 85: goto tr1613;
		case 95: goto st112;
		case 105: goto tr1614;
		case 111: goto tr1615;
		case 117: goto tr1616;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1555:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1336;
tr1654:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 43 "src/ragel/rules/contractions.rl"
	{n2="N'T";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 25;}
	goto st1336;
st1336:
	if ( ++p == pe )
		goto _test_eof1336;
case 1336:
#line 31879 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr0;
tr1556:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1337;
st1337:
	if ( ++p == pe )
		goto _test_eof1337;
case 1337:
#line 31922 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1645;
		case 78: goto tr1646;
		case 95: goto st112;
		case 109: goto tr1647;
		case 110: goto tr1648;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1645:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1338;
st1338:
	if ( ++p == pe )
		goto _test_eof1338;
case 1338:
#line 31969 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st527;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
st527:
	if ( ++p == pe )
		goto _test_eof527;
case 527:
	switch( (*p) ) {
		case 35: goto st1339;
		case 93: goto st1339;
		case 95: goto st1339;
		case 126: goto st1339;
	}
	if ( (*p) < 63 ) {
		if ( 45 <= (*p) && (*p) <= 58 )
			goto st1339;
	} else if ( (*p) > 91 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1339;
	} else
		goto st1339;
	goto tr0;
st1339:
	if ( ++p == pe )
		goto _test_eof1339;
case 1339:
	switch( (*p) ) {
		case 35: goto st1339;
		case 93: goto st1339;
		case 95: goto st1339;
		case 126: goto st1339;
	}
	if ( (*p) < 63 ) {
		if ( 45 <= (*p) && (*p) <= 58 )
			goto st1339;
	} else if ( (*p) > 91 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1339;
	} else
		goto st1339;
	goto tr1156;
tr1646:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1340;
st1340:
	if ( ++p == pe )
		goto _test_eof1340;
case 1340:
#line 32051 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st528;
		case -62: goto st1;
		case -30: goto st529;
		case 36: goto tr787;
		case 38: goto st532;
		case 39: goto st557;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 63: goto st558;
		case 64: goto st113;
		case 84: goto tr1654;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
st528:
	if ( ++p == pe )
		goto _test_eof528;
case 528:
	switch( (*p) ) {
		case 68: goto tr199;
		case 76: goto st216;
		case 77: goto tr201;
		case 82: goto st217;
		case 84: goto tr652;
		case 86: goto st218;
		case 100: goto tr204;
		case 108: goto st219;
		case 109: goto tr206;
		case 114: goto st220;
		case 118: goto st221;
	}
	goto tr592;
st529:
	if ( ++p == pe )
		goto _test_eof529;
case 529:
	switch( (*p) ) {
		case -128: goto st530;
		case -126: goto st6;
		case -120: goto st7;
	}
	if ( -127 <= (*p) && (*p) <= -65 )
		goto st2;
	goto tr592;
st530:
	if ( ++p == pe )
		goto _test_eof530;
case 530:
	switch( (*p) ) {
		case -103: goto st531;
		case -102: goto tr1;
		case -98: goto tr1;
	}
	if ( (*p) < -106 ) {
		if ( (*p) <= -119 )
			goto tr1;
	} else if ( (*p) > -105 ) {
		if ( -96 <= (*p) && (*p) <= -65 )
			goto tr1;
	} else
		goto tr1;
	goto tr592;
st531:
	if ( ++p == pe )
		goto _test_eof531;
case 531:
	switch( (*p) ) {
		case 68: goto tr217;
		case 76: goto st225;
		case 77: goto tr219;
		case 82: goto st226;
		case 83: goto tr221;
		case 84: goto tr655;
		case 86: goto st227;
		case 100: goto tr223;
		case 108: goto st228;
		case 109: goto tr225;
		case 114: goto st229;
		case 115: goto tr221;
		case 118: goto st230;
	}
	goto tr592;
st532:
	if ( ++p == pe )
		goto _test_eof532;
case 532:
	switch( (*p) ) {
		case 35: goto st533;
		case 38: goto st493;
		case 97: goto st548;
		case 114: goto st553;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr590;
	goto tr592;
st533:
	if ( ++p == pe )
		goto _test_eof533;
case 533:
	switch( (*p) ) {
		case 51: goto st534;
		case 56: goto st537;
		case 88: goto st542;
		case 120: goto st542;
	}
	goto tr592;
st534:
	if ( ++p == pe )
		goto _test_eof534;
case 534:
	if ( (*p) == 57 )
		goto st535;
	goto tr592;
st535:
	if ( ++p == pe )
		goto _test_eof535;
case 535:
	if ( (*p) == 59 )
		goto st536;
	goto tr592;
st536:
	if ( ++p == pe )
		goto _test_eof536;
case 536:
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto st241;
		case 77: goto tr250;
		case 82: goto st242;
		case 83: goto tr252;
		case 84: goto tr664;
		case 86: goto st243;
		case 100: goto tr254;
		case 108: goto st244;
		case 109: goto tr256;
		case 114: goto st245;
		case 115: goto tr252;
		case 118: goto st246;
	}
	goto tr592;
st537:
	if ( ++p == pe )
		goto _test_eof537;
case 537:
	if ( (*p) == 50 )
		goto st538;
	goto tr592;
st538:
	if ( ++p == pe )
		goto _test_eof538;
case 538:
	if ( (*p) == 49 )
		goto st539;
	goto tr592;
st539:
	if ( ++p == pe )
		goto _test_eof539;
case 539:
	if ( (*p) == 55 )
		goto st540;
	goto tr592;
st540:
	if ( ++p == pe )
		goto _test_eof540;
case 540:
	if ( (*p) == 59 )
		goto st541;
	goto tr592;
st541:
	if ( ++p == pe )
		goto _test_eof541;
case 541:
	switch( (*p) ) {
		case 68: goto tr269;
		case 76: goto st252;
		case 77: goto tr271;
		case 82: goto st253;
		case 83: goto tr273;
		case 84: goto tr669;
		case 86: goto st254;
		case 100: goto tr275;
		case 108: goto st255;
		case 109: goto tr277;
		case 114: goto st256;
		case 115: goto tr273;
		case 118: goto st257;
	}
	goto tr592;
st542:
	if ( ++p == pe )
		goto _test_eof542;
case 542:
	if ( (*p) == 50 )
		goto st543;
	goto tr0;
st543:
	if ( ++p == pe )
		goto _test_eof543;
case 543:
	if ( (*p) == 48 )
		goto st544;
	goto tr0;
st544:
	if ( ++p == pe )
		goto _test_eof544;
case 544:
	if ( (*p) == 49 )
		goto st545;
	goto tr0;
st545:
	if ( ++p == pe )
		goto _test_eof545;
case 545:
	if ( (*p) == 57 )
		goto st546;
	goto tr0;
st546:
	if ( ++p == pe )
		goto _test_eof546;
case 546:
	if ( (*p) == 59 )
		goto st547;
	goto tr0;
st547:
	if ( ++p == pe )
		goto _test_eof547;
case 547:
	switch( (*p) ) {
		case 68: goto tr291;
		case 76: goto st264;
		case 77: goto tr293;
		case 82: goto st265;
		case 83: goto tr295;
		case 84: goto tr675;
		case 86: goto st266;
		case 100: goto tr297;
		case 108: goto st267;
		case 109: goto tr299;
		case 114: goto st268;
		case 115: goto tr295;
		case 118: goto st269;
	}
	goto tr0;
st548:
	if ( ++p == pe )
		goto _test_eof548;
case 548:
	switch( (*p) ) {
		case 109: goto st271;
		case 112: goto st549;
	}
	goto tr592;
st549:
	if ( ++p == pe )
		goto _test_eof549;
case 549:
	if ( (*p) == 111 )
		goto st550;
	goto tr592;
st550:
	if ( ++p == pe )
		goto _test_eof550;
case 550:
	if ( (*p) == 115 )
		goto st551;
	goto tr592;
st551:
	if ( ++p == pe )
		goto _test_eof551;
case 551:
	if ( (*p) == 59 )
		goto st552;
	goto tr592;
st552:
	if ( ++p == pe )
		goto _test_eof552;
case 552:
	switch( (*p) ) {
		case 68: goto tr379;
		case 76: goto st321;
		case 77: goto tr381;
		case 82: goto st322;
		case 83: goto tr358;
		case 84: goto tr680;
		case 86: goto st323;
		case 100: goto tr384;
		case 108: goto st324;
		case 109: goto tr386;
		case 114: goto st325;
		case 115: goto tr358;
		case 118: goto st326;
	}
	goto tr592;
st553:
	if ( ++p == pe )
		goto _test_eof553;
case 553:
	if ( (*p) == 115 )
		goto st554;
	goto tr592;
st554:
	if ( ++p == pe )
		goto _test_eof554;
case 554:
	if ( (*p) == 113 )
		goto st555;
	goto tr592;
st555:
	if ( ++p == pe )
		goto _test_eof555;
case 555:
	if ( (*p) == 117 )
		goto st556;
	goto tr592;
st556:
	if ( ++p == pe )
		goto _test_eof556;
case 556:
	if ( (*p) == 111 )
		goto st540;
	goto tr592;
st557:
	if ( ++p == pe )
		goto _test_eof557;
case 557:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 84: goto tr684;
		case 86: goto tr370;
		case 100: goto tr371;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr592;
st558:
	if ( ++p == pe )
		goto _test_eof558;
case 558:
	if ( (*p) == 84 )
		goto tr685;
	goto tr592;
tr1647:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1341;
st1341:
	if ( ++p == pe )
		goto _test_eof1341;
case 1341:
#line 32435 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st527;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1648:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1342;
st1342:
	if ( ++p == pe )
		goto _test_eof1342;
case 1342:
#line 32478 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st559;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st559:
	if ( ++p == pe )
		goto _test_eof559;
case 559:
	switch( (*p) ) {
		case 35: goto st560;
		case 97: goto st270;
		case 114: goto st311;
	}
	goto tr10;
st560:
	if ( ++p == pe )
		goto _test_eof560;
case 560:
	switch( (*p) ) {
		case 51: goto st238;
		case 56: goto st247;
		case 88: goto st542;
		case 120: goto st542;
	}
	goto tr10;
tr1557:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1343;
st1343:
	if ( ++p == pe )
		goto _test_eof1343;
case 1343:
#line 32541 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 68: goto tr1656;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1656:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1344;
st1344:
	if ( ++p == pe )
		goto _test_eof1344;
case 1344:
#line 32585 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 47: goto st561;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
st561:
	if ( ++p == pe )
		goto _test_eof561;
case 561:
	if ( (*p) == 79 )
		goto st562;
	goto tr592;
st562:
	if ( ++p == pe )
		goto _test_eof562;
case 562:
	if ( (*p) == 82 )
		goto tr688;
	goto tr592;
tr1558:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1345;
st1345:
	if ( ++p == pe )
		goto _test_eof1345;
case 1345:
#line 32643 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1658;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1658:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1346;
st1346:
	if ( ++p == pe )
		goto _test_eof1346;
case 1346:
#line 32687 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1646;
		case 95: goto st112;
		case 110: goto tr1648;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1559:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1347;
st1347:
	if ( ++p == pe )
		goto _test_eof1347;
case 1347:
#line 32732 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st563:
	if ( ++p == pe )
		goto _test_eof563;
case 563:
	switch( (*p) ) {
		case 43: goto st112;
		case 45: goto st112;
		case 46: goto tr689;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st112;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st112;
	} else
		goto st112;
	goto tr10;
tr1560:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1348;
st1348:
	if ( ++p == pe )
		goto _test_eof1348;
case 1348:
#line 32794 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 100: goto tr1660;
		case 109: goto tr1661;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1660:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1349;
st1349:
	if ( ++p == pe )
		goto _test_eof1349;
case 1349:
#line 32838 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st564;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st564:
	if ( ++p == pe )
		goto _test_eof564;
case 564:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 100: goto tr371;
		case 108: goto tr690;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
tr1661:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1350;
st1350:
	if ( ++p == pe )
		goto _test_eof1350;
case 1350:
#line 32904 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto tr1664;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1666:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 38;}
	goto st1351;
tr1663:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1351;
tr1678:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 37;}
	goto st1351;
st1351:
	if ( ++p == pe )
		goto _test_eof1351;
case 1351:
#line 32959 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto tr637;
		case 66: goto tr638;
		case 67: goto tr639;
		case 69: goto tr641;
		case 71: goto tr642;
		case 73: goto tr643;
		case 74: goto tr644;
		case 77: goto tr645;
		case 78: goto tr646;
		case 79: goto tr647;
		case 80: goto tr648;
		case 84: goto tr649;
		case 88: goto tr650;
		case 95: goto st112;
		case 97: goto tr637;
		case 98: goto tr638;
		case 99: goto tr639;
		case 101: goto tr641;
		case 103: goto tr642;
		case 105: goto tr643;
		case 106: goto tr644;
		case 109: goto tr645;
		case 110: goto tr646;
		case 111: goto tr647;
		case 112: goto tr648;
		case 116: goto tr649;
		case 120: goto tr650;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto tr640;
		} else if ( (*p) >= 68 )
			goto tr640;
	} else
		goto st112;
	goto tr0;
tr1664:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1352;
st1352:
	if ( ++p == pe )
		goto _test_eof1352;
case 1352:
#line 33017 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1665:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1353;
st1353:
	if ( ++p == pe )
		goto _test_eof1353;
case 1353:
#line 33060 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1666;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1561:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1354;
st1354:
	if ( ++p == pe )
		goto _test_eof1354;
case 1354:
#line 33102 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1667;
		case 78: goto tr1668;
		case 95: goto st112;
		case 109: goto tr1647;
		case 110: goto tr1669;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1667:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1355;
st1355:
	if ( ++p == pe )
		goto _test_eof1355;
case 1355:
#line 33148 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st527;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1668:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1356;
st1356:
	if ( ++p == pe )
		goto _test_eof1356;
case 1356:
#line 33191 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st565;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st565:
	if ( ++p == pe )
		goto _test_eof565;
case 565:
	switch( (*p) ) {
		case 35: goto st566;
		case 97: goto st270;
		case 114: goto st311;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr311;
	goto tr10;
st566:
	if ( ++p == pe )
		goto _test_eof566;
case 566:
	switch( (*p) ) {
		case 51: goto st238;
		case 56: goto st247;
		case 88: goto st567;
		case 120: goto st567;
	}
	goto tr10;
st567:
	if ( ++p == pe )
		goto _test_eof567;
case 567:
	if ( (*p) == 50 )
		goto st568;
	goto tr10;
st568:
	if ( ++p == pe )
		goto _test_eof568;
case 568:
	if ( (*p) == 48 )
		goto st569;
	goto tr10;
st569:
	if ( ++p == pe )
		goto _test_eof569;
case 569:
	if ( (*p) == 49 )
		goto st570;
	goto tr10;
st570:
	if ( ++p == pe )
		goto _test_eof570;
case 570:
	if ( (*p) == 57 )
		goto st571;
	goto tr10;
st571:
	if ( ++p == pe )
		goto _test_eof571;
case 571:
	if ( (*p) == 59 )
		goto st572;
	goto tr10;
st572:
	if ( ++p == pe )
		goto _test_eof572;
case 572:
	switch( (*p) ) {
		case 68: goto tr291;
		case 76: goto st264;
		case 77: goto tr293;
		case 82: goto st265;
		case 83: goto tr295;
		case 86: goto st266;
		case 100: goto tr297;
		case 108: goto st267;
		case 109: goto tr299;
		case 114: goto st268;
		case 115: goto tr295;
		case 116: goto tr698;
		case 118: goto st269;
	}
	goto tr10;
tr1669:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1357;
st1357:
	if ( ++p == pe )
		goto _test_eof1357;
case 1357:
#line 33311 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st573;
		case -62: goto st1;
		case -30: goto st574;
		case 38: goto st577;
		case 39: goto st596;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 63: goto st597;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1676;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st573:
	if ( ++p == pe )
		goto _test_eof573;
case 573:
	switch( (*p) ) {
		case 68: goto tr199;
		case 76: goto st216;
		case 77: goto tr201;
		case 82: goto st217;
		case 86: goto st218;
		case 100: goto tr204;
		case 108: goto st219;
		case 109: goto tr206;
		case 114: goto st220;
		case 116: goto tr699;
		case 118: goto st221;
	}
	goto tr10;
st574:
	if ( ++p == pe )
		goto _test_eof574;
case 574:
	switch( (*p) ) {
		case -128: goto st575;
		case -126: goto st6;
		case -120: goto st7;
	}
	if ( -127 <= (*p) && (*p) <= -65 )
		goto st2;
	goto tr10;
st575:
	if ( ++p == pe )
		goto _test_eof575;
case 575:
	switch( (*p) ) {
		case -103: goto st576;
		case -102: goto tr1;
		case -98: goto tr1;
	}
	if ( (*p) < -106 ) {
		if ( (*p) <= -119 )
			goto tr1;
	} else if ( (*p) > -105 ) {
		if ( -96 <= (*p) && (*p) <= -65 )
			goto tr1;
	} else
		goto tr1;
	goto tr10;
st576:
	if ( ++p == pe )
		goto _test_eof576;
case 576:
	switch( (*p) ) {
		case 68: goto tr217;
		case 76: goto st225;
		case 77: goto tr219;
		case 82: goto st226;
		case 83: goto tr221;
		case 86: goto st227;
		case 100: goto tr223;
		case 108: goto st228;
		case 109: goto tr225;
		case 114: goto st229;
		case 115: goto tr221;
		case 116: goto tr702;
		case 118: goto st230;
	}
	goto tr10;
st577:
	if ( ++p == pe )
		goto _test_eof577;
case 577:
	switch( (*p) ) {
		case 35: goto st578;
		case 97: goto st587;
		case 114: goto st592;
	}
	goto tr10;
st578:
	if ( ++p == pe )
		goto _test_eof578;
case 578:
	switch( (*p) ) {
		case 51: goto st579;
		case 56: goto st582;
		case 88: goto st567;
		case 120: goto st567;
	}
	goto tr10;
st579:
	if ( ++p == pe )
		goto _test_eof579;
case 579:
	if ( (*p) == 57 )
		goto st580;
	goto tr10;
st580:
	if ( ++p == pe )
		goto _test_eof580;
case 580:
	if ( (*p) == 59 )
		goto st581;
	goto tr10;
st581:
	if ( ++p == pe )
		goto _test_eof581;
case 581:
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto st241;
		case 77: goto tr250;
		case 82: goto st242;
		case 83: goto tr252;
		case 86: goto st243;
		case 100: goto tr254;
		case 108: goto st244;
		case 109: goto tr256;
		case 114: goto st245;
		case 115: goto tr252;
		case 116: goto tr710;
		case 118: goto st246;
	}
	goto tr10;
st582:
	if ( ++p == pe )
		goto _test_eof582;
case 582:
	if ( (*p) == 50 )
		goto st583;
	goto tr10;
st583:
	if ( ++p == pe )
		goto _test_eof583;
case 583:
	if ( (*p) == 49 )
		goto st584;
	goto tr10;
st584:
	if ( ++p == pe )
		goto _test_eof584;
case 584:
	if ( (*p) == 55 )
		goto st585;
	goto tr10;
st585:
	if ( ++p == pe )
		goto _test_eof585;
case 585:
	if ( (*p) == 59 )
		goto st586;
	goto tr10;
st586:
	if ( ++p == pe )
		goto _test_eof586;
case 586:
	switch( (*p) ) {
		case 68: goto tr269;
		case 76: goto st252;
		case 77: goto tr271;
		case 82: goto st253;
		case 83: goto tr273;
		case 86: goto st254;
		case 100: goto tr275;
		case 108: goto st255;
		case 109: goto tr277;
		case 114: goto st256;
		case 115: goto tr273;
		case 116: goto tr715;
		case 118: goto st257;
	}
	goto tr10;
st587:
	if ( ++p == pe )
		goto _test_eof587;
case 587:
	switch( (*p) ) {
		case 109: goto st271;
		case 112: goto st588;
	}
	goto tr10;
st588:
	if ( ++p == pe )
		goto _test_eof588;
case 588:
	if ( (*p) == 111 )
		goto st589;
	goto tr10;
st589:
	if ( ++p == pe )
		goto _test_eof589;
case 589:
	if ( (*p) == 115 )
		goto st590;
	goto tr10;
st590:
	if ( ++p == pe )
		goto _test_eof590;
case 590:
	if ( (*p) == 59 )
		goto st591;
	goto tr10;
st591:
	if ( ++p == pe )
		goto _test_eof591;
case 591:
	switch( (*p) ) {
		case 68: goto tr379;
		case 76: goto st321;
		case 77: goto tr381;
		case 82: goto st322;
		case 83: goto tr358;
		case 86: goto st323;
		case 100: goto tr384;
		case 108: goto st324;
		case 109: goto tr386;
		case 114: goto st325;
		case 115: goto tr358;
		case 116: goto tr720;
		case 118: goto st326;
	}
	goto tr10;
st592:
	if ( ++p == pe )
		goto _test_eof592;
case 592:
	if ( (*p) == 115 )
		goto st593;
	goto tr10;
st593:
	if ( ++p == pe )
		goto _test_eof593;
case 593:
	if ( (*p) == 113 )
		goto st594;
	goto tr10;
st594:
	if ( ++p == pe )
		goto _test_eof594;
case 594:
	if ( (*p) == 117 )
		goto st595;
	goto tr10;
st595:
	if ( ++p == pe )
		goto _test_eof595;
case 595:
	if ( (*p) == 111 )
		goto st585;
	goto tr10;
st596:
	if ( ++p == pe )
		goto _test_eof596;
case 596:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 100: goto tr371;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 116: goto tr724;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
st597:
	if ( ++p == pe )
		goto _test_eof597;
case 597:
	if ( (*p) == 116 )
		goto tr725;
	goto tr10;
tr1562:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1358;
st1358:
	if ( ++p == pe )
		goto _test_eof1358;
case 1358:
#line 33636 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1677;
		case 121: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1677:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1359;
st1359:
	if ( ++p == pe )
		goto _test_eof1359;
case 1359:
#line 33680 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1563:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1360;
st1360:
	if ( ++p == pe )
		goto _test_eof1360;
case 1360:
#line 33722 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 114: goto tr1677;
		case 116: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1564:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1361;
st1361:
	if ( ++p == pe )
		goto _test_eof1361;
case 1361:
#line 33766 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1679;
		case 105: goto tr1680;
		case 107: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1679:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1362;
st1362:
	if ( ++p == pe )
		goto _test_eof1362;
case 1362:
#line 33811 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1668;
		case 95: goto st112;
		case 110: goto tr1669;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1680:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1363;
st1363:
	if ( ++p == pe )
		goto _test_eof1363;
case 1363:
#line 33855 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 122: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1565:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1364;
st1364:
	if ( ++p == pe )
		goto _test_eof1364;
case 1364:
#line 33898 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1681;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1681:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1365;
st1365:
	if ( ++p == pe )
		goto _test_eof1365;
case 1365:
#line 33941 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1665;
		case 111: goto tr1682;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1682:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1366;
st1366:
	if ( ++p == pe )
		goto _test_eof1366;
case 1366:
#line 33985 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1683;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1683:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1367;
st1367:
	if ( ++p == pe )
		goto _test_eof1367;
case 1367:
#line 34028 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1666;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1566:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1368;
st1368:
	if ( ++p == pe )
		goto _test_eof1368;
case 1368:
#line 34071 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 103: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1567:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1369;
st1369:
	if ( ++p == pe )
		goto _test_eof1369;
case 1369:
#line 34114 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr940:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1370;
st1370:
	if ( ++p == pe )
		goto _test_eof1370;
case 1370:
#line 34157 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 95: goto st112;
		case 98: goto tr1559;
		case 99: goto tr1685;
		case 100: goto tr1559;
		case 105: goto tr596;
		case 108: goto tr1686;
		case 111: goto tr1687;
		case 114: goto tr1688;
		case 115: goto tr1689;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr1555;
		} else if ( (*p) > 101 ) {
			if ( 102 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr596;
	} else
		goto tr594;
	goto tr997;
tr1684:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1371;
st1371:
	if ( ++p == pe )
		goto _test_eof1371;
case 1371:
#line 34214 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto tr749;
		case 66: goto tr750;
		case 67: goto tr751;
		case 69: goto tr753;
		case 71: goto tr754;
		case 73: goto tr755;
		case 74: goto tr756;
		case 77: goto tr757;
		case 78: goto tr758;
		case 79: goto tr759;
		case 80: goto tr760;
		case 84: goto tr761;
		case 88: goto tr762;
		case 95: goto st112;
		case 97: goto tr749;
		case 98: goto tr750;
		case 99: goto tr751;
		case 101: goto tr753;
		case 103: goto tr754;
		case 105: goto tr755;
		case 106: goto tr756;
		case 109: goto tr757;
		case 110: goto tr758;
		case 111: goto tr759;
		case 112: goto tr760;
		case 116: goto tr761;
		case 120: goto tr762;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto tr752;
		} else if ( (*p) >= 68 )
			goto tr752;
	} else
		goto st112;
	goto tr1568;
tr1685:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1372;
st1372:
	if ( ++p == pe )
		goto _test_eof1372;
case 1372:
#line 34272 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 104: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1686:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1373;
st1373:
	if ( ++p == pe )
		goto _test_eof1373;
case 1373:
#line 34315 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 100: goto tr1566;
		case 118: goto tr1690;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1690:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1374;
st1374:
	if ( ++p == pe )
		goto _test_eof1374;
case 1374:
#line 34359 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 100: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1687:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1375;
st1375:
	if ( ++p == pe )
		goto _test_eof1375;
case 1375:
#line 34402 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 117: goto tr1691;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1691:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1376;
st1376:
	if ( ++p == pe )
		goto _test_eof1376;
case 1376:
#line 34445 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1688:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1377;
st1377:
	if ( ++p == pe )
		goto _test_eof1377;
case 1377:
#line 34488 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto tr1692;
		case 111: goto tr1693;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1692:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1378;
st1378:
	if ( ++p == pe )
		goto _test_eof1378;
case 1378:
#line 34532 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 103: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1694:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1379;
st1379:
	if ( ++p == pe )
		goto _test_eof1379;
case 1379:
#line 34575 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1693:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1380;
st1380:
	if ( ++p == pe )
		goto _test_eof1380;
case 1380:
#line 34617 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1689:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1381;
st1381:
	if ( ++p == pe )
		goto _test_eof1381;
case 1381:
#line 34660 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 109: goto tr1695;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1695:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1382;
st1382:
	if ( ++p == pe )
		goto _test_eof1382;
case 1382:
#line 34703 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr941:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1383;
st1383:
	if ( ++p == pe )
		goto _test_eof1383;
case 1383:
#line 34746 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st598;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 65: goto tr1697;
		case 72: goto tr1698;
		case 73: goto tr1699;
		case 79: goto tr1700;
		case 86: goto tr1701;
		case 95: goto st112;
		case 97: goto tr1702;
		case 99: goto tr1703;
		case 100: goto tr1704;
		case 101: goto tr596;
		case 104: goto tr1705;
		case 105: goto tr1706;
		case 109: goto tr1707;
		case 111: goto tr1708;
		case 114: goto tr1709;
		case 116: goto tr1710;
		case 117: goto tr596;
		case 118: goto tr1711;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
st598:
	if ( ++p == pe )
		goto _test_eof598;
case 598:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 100: goto tr371;
		case 108: goto tr372;
		case 109: goto tr726;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
tr726:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "src/ragel/actions.rl"
	{ s.suffix += 2; }
#line 10 "src/ragel/rules/contractions.rl"
	{n2="'m";}
#line 11 "src/ragel/rules/main.rl"
	{ s.act = 26;}
	goto st1384;
st1384:
	if ( ++p == pe )
		goto _test_eof1384;
case 1384:
#line 34836 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 111: goto tr1712;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr1313;
tr1712:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1385;
st1385:
	if ( ++p == pe )
		goto _test_eof1385;
case 1385:
#line 34876 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
		case 110: goto tr1713;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1697:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1386;
st1386:
	if ( ++p == pe )
		goto _test_eof1386;
case 1386:
#line 34916 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1714;
		case 78: goto tr1646;
		case 95: goto st112;
		case 108: goto tr1715;
		case 110: goto tr1648;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1714:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1387;
st1387:
	if ( ++p == pe )
		goto _test_eof1387;
case 1387:
#line 34963 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1716;
		case 95: goto st112;
		case 108: goto tr1717;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1716:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1388;
st1388:
	if ( ++p == pe )
		goto _test_eof1388;
case 1388:
#line 35008 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1718;
		case 95: goto st112;
		case 116: goto tr1719;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1718:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1389;
st1389:
	if ( ++p == pe )
		goto _test_eof1389;
case 1389:
#line 35053 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 79: goto tr1645;
		case 95: goto st112;
		case 111: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1719:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1390;
st1390:
	if ( ++p == pe )
		goto _test_eof1390;
case 1390:
#line 35098 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 79: goto tr1667;
		case 95: goto st112;
		case 111: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1717:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1391;
st1391:
	if ( ++p == pe )
		goto _test_eof1391;
case 1391:
#line 35142 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1720;
		case 95: goto st112;
		case 116: goto tr1719;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1720:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1392;
st1392:
	if ( ++p == pe )
		goto _test_eof1392;
case 1392:
#line 35186 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 79: goto tr1667;
		case 95: goto st112;
		case 111: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1715:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1393;
st1393:
	if ( ++p == pe )
		goto _test_eof1393;
case 1393:
#line 35230 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1721;
		case 95: goto st112;
		case 108: goto tr1717;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1721:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1394;
st1394:
	if ( ++p == pe )
		goto _test_eof1394;
case 1394:
#line 35274 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1720;
		case 95: goto st112;
		case 116: goto tr1719;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1698:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1395;
st1395:
	if ( ++p == pe )
		goto _test_eof1395;
case 1395:
#line 35318 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 82: goto tr1722;
		case 95: goto st112;
		case 114: goto tr1723;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1722:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1396;
st1396:
	if ( ++p == pe )
		goto _test_eof1396;
case 1396:
#line 35363 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 79: goto tr1724;
		case 95: goto st112;
		case 111: goto tr1725;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1724:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1397;
st1397:
	if ( ++p == pe )
		goto _test_eof1397;
case 1397:
#line 35408 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1726;
		case 95: goto st112;
		case 109: goto tr1727;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1726:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1398;
st1398:
	if ( ++p == pe )
		goto _test_eof1398;
case 1398:
#line 35453 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1728;
		case 95: goto st112;
		case 101: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1728:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1399;
st1399:
	if ( ++p == pe )
		goto _test_eof1399;
case 1399:
#line 35498 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st599;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
st599:
	if ( ++p == pe )
		goto _test_eof599;
case 599:
	if ( (*p) == 47 )
		goto st600;
	goto tr0;
st600:
	if ( ++p == pe )
		goto _test_eof600;
case 600:
	if ( (*p) == 47 )
		goto st527;
	goto tr0;
tr1729:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1400;
st1400:
	if ( ++p == pe )
		goto _test_eof1400;
case 1400:
#line 35556 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st599;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1727:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1401;
st1401:
	if ( ++p == pe )
		goto _test_eof1401;
case 1401:
#line 35599 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1731;
		case 95: goto st112;
		case 101: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1731:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1402;
st1402:
	if ( ++p == pe )
		goto _test_eof1402;
case 1402:
#line 35643 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st599;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1725:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1403;
st1403:
	if ( ++p == pe )
		goto _test_eof1403;
case 1403:
#line 35686 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1732;
		case 95: goto st112;
		case 109: goto tr1727;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1732:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1404;
st1404:
	if ( ++p == pe )
		goto _test_eof1404;
case 1404:
#line 35730 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1731;
		case 95: goto st112;
		case 101: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1723:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1405;
st1405:
	if ( ++p == pe )
		goto _test_eof1405;
case 1405:
#line 35774 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 79: goto tr1733;
		case 95: goto st112;
		case 111: goto tr1725;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1733:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1406;
st1406:
	if ( ++p == pe )
		goto _test_eof1406;
case 1406:
#line 35818 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1732;
		case 95: goto st112;
		case 109: goto tr1727;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1699:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1407;
st1407:
	if ( ++p == pe )
		goto _test_eof1407;
case 1407:
#line 35862 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 68: goto tr1645;
		case 95: goto st112;
		case 100: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1700:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1408;
st1408:
	if ( ++p == pe )
		goto _test_eof1408;
case 1408:
#line 35907 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 85: goto tr1734;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1734:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1409;
st1409:
	if ( ++p == pe )
		goto _test_eof1409;
case 1409:
#line 35951 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1735;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1735:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1410;
st1410:
	if ( ++p == pe )
		goto _test_eof1410;
case 1410:
#line 35995 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 68: goto tr1658;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1701:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1411;
st1411:
	if ( ++p == pe )
		goto _test_eof1411;
case 1411:
#line 36039 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1728;
		case 95: goto st112;
		case 115: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1702:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1412;
st1412:
	if ( ++p == pe )
		goto _test_eof1412;
case 1412:
#line 36084 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 76: goto tr1736;
		case 78: goto tr1668;
		case 95: goto st112;
		case 108: goto tr1737;
		case 110: goto tr1669;
		case 112: goto tr1738;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1736:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1413;
st1413:
	if ( ++p == pe )
		goto _test_eof1413;
case 1413:
#line 36131 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1721;
		case 95: goto st112;
		case 108: goto tr1717;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1737:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1414;
st1414:
	if ( ++p == pe )
		goto _test_eof1414;
case 1414:
#line 36175 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1721;
		case 95: goto st112;
		case 105: goto tr1739;
		case 108: goto tr1717;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1739:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1415;
st1415:
	if ( ++p == pe )
		goto _test_eof1415;
case 1415:
#line 36220 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 102: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1738:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1416;
st1416:
	if ( ++p == pe )
		goto _test_eof1416;
case 1416:
#line 36263 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1703:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1417;
st1417:
	if ( ++p == pe )
		goto _test_eof1417;
case 1417:
#line 36306 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1704:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1418;
st1418:
	if ( ++p == pe )
		goto _test_eof1418;
case 1418:
#line 36349 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 114: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1705:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1419;
st1419:
	if ( ++p == pe )
		goto _test_eof1419;
case 1419:
#line 36392 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 82: goto tr1740;
		case 95: goto st112;
		case 114: goto tr1723;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1740:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1420;
st1420:
	if ( ++p == pe )
		goto _test_eof1420;
case 1420:
#line 36436 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 79: goto tr1733;
		case 95: goto st112;
		case 111: goto tr1725;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1706:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1421;
st1421:
	if ( ++p == pe )
		goto _test_eof1421;
case 1421:
#line 36480 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 68: goto tr1667;
		case 95: goto st112;
		case 100: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1707:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1422;
st1422:
	if ( ++p == pe )
		goto _test_eof1422;
case 1422:
#line 36524 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 100: goto tr1741;
		case 110: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1741:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1423;
st1423:
	if ( ++p == pe )
		goto _test_eof1423;
case 1423:
#line 36568 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 114: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1708:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1424;
st1424:
	if ( ++p == pe )
		goto _test_eof1424;
case 1424:
#line 36611 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1742;
		case 46: goto tr1666;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1743;
		case 110: goto tr1744;
		case 111: goto tr1745;
		case 114: goto tr1745;
		case 117: goto tr1746;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1742:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1425;
st1425:
	if ( ++p == pe )
		goto _test_eof1425;
case 1425:
#line 36658 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto st495;
		case 46: goto st345;
		case 64: goto st113;
		case 95: goto st112;
		case 111: goto tr1745;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1745:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1426;
st1426:
	if ( ++p == pe )
		goto _test_eof1426;
case 1426:
#line 36701 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 112: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1743:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1427;
st1427:
	if ( ++p == pe )
		goto _test_eof1427;
case 1427:
#line 36744 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 111: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1744:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1428;
st1428:
	if ( ++p == pe )
		goto _test_eof1428;
case 1428:
#line 36787 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1677;
		case 116: goto tr1747;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1747:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1429;
st1429:
	if ( ++p == pe )
		goto _test_eof1429;
case 1429:
#line 36831 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st601;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st601:
	if ( ++p == pe )
		goto _test_eof601;
case 601:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 100: goto tr371;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
tr1746:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1430;
st1430:
	if ( ++p == pe )
		goto _test_eof1430;
case 1430:
#line 36897 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1749;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1749:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1431;
st1431:
	if ( ++p == pe )
		goto _test_eof1431;
case 1431:
#line 36940 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 100: goto tr1679;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1709:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1432;
st1432:
	if ( ++p == pe )
		goto _test_eof1432;
case 1432:
#line 36983 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1750;
		case 107: goto tr1677;
		case 112: goto tr1751;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1750:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1433;
st1433:
	if ( ++p == pe )
		goto _test_eof1433;
case 1433:
#line 37028 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1751:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1434;
st1434:
	if ( ++p == pe )
		goto _test_eof1434;
case 1434:
#line 37071 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1710:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1435;
st1435:
	if ( ++p == pe )
		goto _test_eof1435;
case 1435:
#line 37114 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1711:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1436;
st1436:
	if ( ++p == pe )
		goto _test_eof1436;
case 1436:
#line 37157 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1731;
		case 95: goto st112;
		case 115: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr942:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1437;
st1437:
	if ( ++p == pe )
		goto _test_eof1437;
case 1437:
#line 37201 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 65: goto tr1752;
		case 73: goto tr1753;
		case 78: goto tr1701;
		case 79: goto tr1754;
		case 95: goto st112;
		case 97: goto tr1755;
		case 101: goto tr1756;
		case 105: goto tr1757;
		case 110: goto tr1711;
		case 111: goto tr1758;
		case 114: goto tr1759;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1752:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1438;
st1438:
	if ( ++p == pe )
		goto _test_eof1438;
case 1438:
#line 37257 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1760;
		case 86: goto tr1728;
		case 95: goto st112;
		case 116: goto tr1761;
		case 118: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1760:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1439;
st1439:
	if ( ++p == pe )
		goto _test_eof1439;
case 1439:
#line 37304 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1645;
		case 95: goto st112;
		case 97: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1761:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1440;
st1440:
	if ( ++p == pe )
		goto _test_eof1440;
case 1440:
#line 37349 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1667;
		case 95: goto st112;
		case 97: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1753:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1441;
st1441:
	if ( ++p == pe )
		goto _test_eof1441;
case 1441:
#line 37393 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 67: goto tr1762;
		case 68: goto tr1658;
		case 95: goto st112;
		case 99: goto tr1763;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1762:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1442;
st1442:
	if ( ++p == pe )
		goto _test_eof1442;
case 1442:
#line 37439 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1728;
		case 95: goto st112;
		case 116: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1763:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1443;
st1443:
	if ( ++p == pe )
		goto _test_eof1443;
case 1443:
#line 37484 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1731;
		case 95: goto st112;
		case 116: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1754:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1444;
st1444:
	if ( ++p == pe )
		goto _test_eof1444;
case 1444:
#line 37528 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1764;
		case 73: goto tr1645;
		case 78: goto tr1646;
		case 95: goto st112;
		case 105: goto tr1647;
		case 110: goto tr1648;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1764:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1445;
st1445:
	if ( ++p == pe )
		goto _test_eof1445;
case 1445:
#line 37576 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1658;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1755:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1446;
st1446:
	if ( ++p == pe )
		goto _test_eof1446;
case 1446:
#line 37620 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1765;
		case 86: goto tr1731;
		case 95: goto st112;
		case 116: goto tr1761;
		case 118: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1765:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1447;
st1447:
	if ( ++p == pe )
		goto _test_eof1447;
case 1447:
#line 37666 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1667;
		case 95: goto st112;
		case 97: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1756:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1448;
st1448:
	if ( ++p == pe )
		goto _test_eof1448;
case 1448:
#line 37710 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1677;
		case 108: goto tr1677;
		case 112: goto tr1766;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1766:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1449;
st1449:
	if ( ++p == pe )
		goto _test_eof1449;
case 1449:
#line 37755 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st602;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st602:
	if ( ++p == pe )
		goto _test_eof602;
case 602:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 100: goto tr371;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 116: goto tr729;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
tr1757:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1450;
st1450:
	if ( ++p == pe )
		goto _test_eof1450;
case 1450:
#line 37823 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 67: goto tr1768;
		case 95: goto st112;
		case 99: goto tr1763;
		case 100: goto tr1679;
		case 115: goto tr1769;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1768:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1451;
st1451:
	if ( ++p == pe )
		goto _test_eof1451;
case 1451:
#line 37869 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1731;
		case 95: goto st112;
		case 116: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1769:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1452;
st1452:
	if ( ++p == pe )
		goto _test_eof1452;
case 1452:
#line 37913 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1758:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1453;
st1453:
	if ( ++p == pe )
		goto _test_eof1453;
case 1453:
#line 37956 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 73: goto tr1667;
		case 78: goto tr1668;
		case 95: goto st112;
		case 101: goto tr1770;
		case 105: goto tr1647;
		case 110: goto tr1669;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1770:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1454;
st1454:
	if ( ++p == pe )
		goto _test_eof1454;
case 1454:
#line 38003 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1679;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1759:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1455;
st1455:
	if ( ++p == pe )
		goto _test_eof1455;
case 1455:
#line 38046 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1694;
		case 118: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr943:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1456;
st1456:
	if ( ++p == pe )
		goto _test_eof1456;
case 1456:
#line 38090 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr596;
		case 100: goto tr1771;
		case 101: goto tr596;
		case 105: goto tr596;
		case 111: goto tr596;
		case 115: goto tr1772;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1771:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1457;
st1457:
	if ( ++p == pe )
		goto _test_eof1457;
case 1457:
#line 38142 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 117: goto tr1773;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1773:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1458;
st1458:
	if ( ++p == pe )
		goto _test_eof1458;
case 1458:
#line 38185 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1772:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1459;
st1459:
	if ( ++p == pe )
		goto _test_eof1459;
case 1459:
#line 38228 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 113: goto tr1694;
		case 116: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr944:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1460;
st1460:
	if ( ++p == pe )
		goto _test_eof1460;
case 1460:
#line 38272 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 69: goto tr1774;
		case 73: goto tr1775;
		case 84: goto tr1776;
		case 95: goto st112;
		case 97: goto tr596;
		case 101: goto tr1777;
		case 105: goto tr1778;
		case 108: goto tr1779;
		case 111: goto tr596;
		case 114: goto tr1780;
		case 116: goto tr1781;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1774:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1461;
st1461:
	if ( ++p == pe )
		goto _test_eof1461;
case 1461:
#line 38328 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1699;
		case 95: goto st112;
		case 101: goto tr1706;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1775:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1462;
st1462:
	if ( ++p == pe )
		goto _test_eof1462;
case 1462:
#line 38373 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1726;
		case 95: goto st112;
		case 108: goto tr1727;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1776:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1463;
st1463:
	if ( ++p == pe )
		goto _test_eof1463;
case 1463:
#line 38418 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1728;
		case 95: goto st112;
		case 112: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1777:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1464;
st1464:
	if ( ++p == pe )
		goto _test_eof1464;
case 1464:
#line 38463 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1782;
		case 95: goto st112;
		case 98: goto tr1677;
		case 101: goto tr1706;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1782:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1465;
st1465:
	if ( ++p == pe )
		goto _test_eof1465;
case 1465:
#line 38508 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 68: goto tr1667;
		case 95: goto st112;
		case 100: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1778:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1466;
st1466:
	if ( ++p == pe )
		goto _test_eof1466;
case 1466:
#line 38552 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1732;
		case 95: goto st112;
		case 108: goto tr1727;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1779:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1467;
st1467:
	if ( ++p == pe )
		goto _test_eof1467;
case 1467:
#line 38596 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1780:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1468;
st1468:
	if ( ++p == pe )
		goto _test_eof1468;
case 1468:
#line 38639 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1781:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1469;
st1469:
	if ( ++p == pe )
		goto _test_eof1469;
case 1469:
#line 38682 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1731;
		case 95: goto st112;
		case 112: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr945:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1470;
st1470:
	if ( ++p == pe )
		goto _test_eof1470;
case 1470:
#line 38726 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 69: goto tr1718;
		case 73: goto tr1762;
		case 79: goto tr1783;
		case 84: goto tr1784;
		case 95: goto st112;
		case 97: goto tr1677;
		case 101: goto tr1785;
		case 105: goto tr1763;
		case 111: goto tr1786;
		case 116: goto tr1787;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1783:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1471;
st1471:
	if ( ++p == pe )
		goto _test_eof1471;
case 1471:
#line 38781 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st599;
		case 64: goto st113;
		case 80: goto tr1788;
		case 95: goto st112;
		case 112: goto tr1789;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1788:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1472;
st1472:
	if ( ++p == pe )
		goto _test_eof1472;
case 1472:
#line 38827 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 72: goto tr1790;
		case 95: goto st112;
		case 104: goto tr1791;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1790:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1473;
st1473:
	if ( ++p == pe )
		goto _test_eof1473;
case 1473:
#line 38872 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1792;
		case 95: goto st112;
		case 101: goto tr1793;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1792:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1474;
st1474:
	if ( ++p == pe )
		goto _test_eof1474;
case 1474:
#line 38917 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 82: goto tr1728;
		case 95: goto st112;
		case 114: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1793:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1475;
st1475:
	if ( ++p == pe )
		goto _test_eof1475;
case 1475:
#line 38962 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 82: goto tr1731;
		case 95: goto st112;
		case 114: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1791:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1476;
st1476:
	if ( ++p == pe )
		goto _test_eof1476;
case 1476:
#line 39006 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1794;
		case 95: goto st112;
		case 101: goto tr1793;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1794:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1477;
st1477:
	if ( ++p == pe )
		goto _test_eof1477;
case 1477:
#line 39050 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 82: goto tr1731;
		case 95: goto st112;
		case 114: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1789:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1478;
st1478:
	if ( ++p == pe )
		goto _test_eof1478;
case 1478:
#line 39094 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 72: goto tr1795;
		case 95: goto st112;
		case 104: goto tr1791;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1795:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1479;
st1479:
	if ( ++p == pe )
		goto _test_eof1479;
case 1479:
#line 39138 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1794;
		case 95: goto st112;
		case 101: goto tr1793;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1784:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1480;
st1480:
	if ( ++p == pe )
		goto _test_eof1480;
case 1480:
#line 39182 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1796;
		case 95: goto st112;
		case 97: goto tr1797;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1796:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1481;
st1481:
	if ( ++p == pe )
		goto _test_eof1481;
case 1481:
#line 39227 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1798;
		case 95: goto st112;
		case 108: goto tr1799;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1798:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1482;
st1482:
	if ( ++p == pe )
		goto _test_eof1482;
case 1482:
#line 39272 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 75: goto tr1645;
		case 95: goto st112;
		case 107: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1799:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1483;
st1483:
	if ( ++p == pe )
		goto _test_eof1483;
case 1483:
#line 39317 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 75: goto tr1667;
		case 95: goto st112;
		case 107: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1797:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1484;
st1484:
	if ( ++p == pe )
		goto _test_eof1484;
case 1484:
#line 39361 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1800;
		case 95: goto st112;
		case 108: goto tr1799;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1800:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1485;
st1485:
	if ( ++p == pe )
		goto _test_eof1485;
case 1485:
#line 39405 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 75: goto tr1667;
		case 95: goto st112;
		case 107: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1785:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1486;
st1486:
	if ( ++p == pe )
		goto _test_eof1486;
case 1486:
#line 39449 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 79: goto tr1667;
		case 95: goto st112;
		case 110: goto tr1694;
		case 111: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1786:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1487;
st1487:
	if ( ++p == pe )
		goto _test_eof1487;
case 1487:
#line 39494 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st599;
		case 64: goto st113;
		case 80: goto tr1801;
		case 95: goto st112;
		case 112: goto tr1789;
		case 118: goto tr1802;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1801:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1488;
st1488:
	if ( ++p == pe )
		goto _test_eof1488;
case 1488:
#line 39540 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 72: goto tr1795;
		case 95: goto st112;
		case 104: goto tr1791;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1802:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1489;
st1489:
	if ( ++p == pe )
		goto _test_eof1489;
case 1489:
#line 39584 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st602;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1787:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1490;
st1490:
	if ( ++p == pe )
		goto _test_eof1490;
case 1490:
#line 39627 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1803;
		case 95: goto st112;
		case 97: goto tr1797;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1803:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1491;
st1491:
	if ( ++p == pe )
		goto _test_eof1491;
case 1491:
#line 39671 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1800;
		case 95: goto st112;
		case 108: goto tr1799;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr946:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1492;
st1492:
	if ( ++p == pe )
		goto _test_eof1492;
case 1492:
#line 39715 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 65: goto tr1804;
		case 84: goto tr1805;
		case 95: goto st112;
		case 97: goto tr1806;
		case 98: goto tr1807;
		case 101: goto tr596;
		case 103: goto tr1808;
		case 105: goto tr596;
		case 111: goto tr1809;
		case 116: goto tr1810;
		case 117: goto tr596;
		case 119: goto tr1811;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 99 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1804:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1493;
st1493:
	if ( ++p == pe )
		goto _test_eof1493;
case 1493:
#line 39770 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 68: goto tr1658;
		case 83: goto tr1658;
		case 86: goto tr1558;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1805:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1494;
st1494:
	if ( ++p == pe )
		goto _test_eof1494;
case 1494:
#line 39816 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1812;
		case 95: goto st112;
		case 116: goto tr1813;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1812:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1495;
st1495:
	if ( ++p == pe )
		goto _test_eof1495;
case 1495:
#line 39861 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1814;
		case 95: goto st112;
		case 112: goto tr1815;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1814:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1496;
st1496:
	if ( ++p == pe )
		goto _test_eof1496;
case 1496:
#line 39906 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st599;
		case 64: goto st113;
		case 83: goto tr1728;
		case 95: goto st112;
		case 115: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1815:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1497;
st1497:
	if ( ++p == pe )
		goto _test_eof1497;
case 1497:
#line 39952 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st599;
		case 64: goto st113;
		case 83: goto tr1731;
		case 95: goto st112;
		case 115: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1813:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1498;
st1498:
	if ( ++p == pe )
		goto _test_eof1498;
case 1498:
#line 39997 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1816;
		case 95: goto st112;
		case 112: goto tr1815;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1816:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1499;
st1499:
	if ( ++p == pe )
		goto _test_eof1499;
case 1499:
#line 40041 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st599;
		case 64: goto st113;
		case 83: goto tr1731;
		case 95: goto st112;
		case 115: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1806:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1500;
st1500:
	if ( ++p == pe )
		goto _test_eof1500;
case 1500:
#line 40086 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 100: goto tr1679;
		case 115: goto tr1679;
		case 118: goto tr1817;
		case 119: goto tr1818;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1817:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1501;
st1501:
	if ( ++p == pe )
		goto _test_eof1501;
case 1501:
#line 40132 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1679;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1818:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1502;
st1502:
	if ( ++p == pe )
		goto _test_eof1502;
case 1502:
#line 40175 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1819;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1819:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1503;
st1503:
	if ( ++p == pe )
		goto _test_eof1503;
case 1503:
#line 40218 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto tr1820;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1820:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1504;
st1504:
	if ( ++p == pe )
		goto _test_eof1504;
case 1504:
#line 40261 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st603;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st603:
	if ( ++p == pe )
		goto _test_eof603;
case 603:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 100: goto tr371;
		case 105: goto tr729;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
tr1807:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1505;
st1505:
	if ( ++p == pe )
		goto _test_eof1505;
case 1505:
#line 40328 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 114: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1808:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1506;
st1506:
	if ( ++p == pe )
		goto _test_eof1506;
case 1506:
#line 40371 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1750;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1809:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1507;
st1507:
	if ( ++p == pe )
		goto _test_eof1507;
case 1507:
#line 40414 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1810:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1508;
st1508:
	if ( ++p == pe )
		goto _test_eof1508;
case 1508:
#line 40457 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1822;
		case 95: goto st112;
		case 115: goto tr1677;
		case 116: goto tr1813;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1822:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1509;
st1509:
	if ( ++p == pe )
		goto _test_eof1509;
case 1509:
#line 40502 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1816;
		case 95: goto st112;
		case 112: goto tr1815;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1811:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1510;
st1510:
	if ( ++p == pe )
		goto _test_eof1510;
case 1510:
#line 40546 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 121: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr947:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1511;
st1511:
	if ( ++p == pe )
		goto _test_eof1511;
case 1511:
#line 40589 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 77: goto tr1823;
		case 82: goto tr1824;
		case 83: goto tr1658;
		case 95: goto st112;
		case 97: goto tr596;
		case 101: goto tr596;
		case 105: goto tr596;
		case 108: goto tr1825;
		case 109: goto tr1826;
		case 110: goto tr1827;
		case 111: goto tr596;
		case 114: goto tr1828;
		case 115: goto tr1829;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1823:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1512;
st1512:
	if ( ++p == pe )
		goto _test_eof1512;
case 1512:
#line 40647 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st599;
		case 64: goto st113;
		case 65: goto tr1776;
		case 95: goto st112;
		case 97: goto tr1830;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1830:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1513;
st1513:
	if ( ++p == pe )
		goto _test_eof1513;
case 1513:
#line 40693 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1731;
		case 95: goto st112;
		case 112: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1824:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1514;
st1514:
	if ( ++p == pe )
		goto _test_eof1514;
case 1514:
#line 40737 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 67: goto tr1814;
		case 95: goto st112;
		case 99: goto tr1815;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1825:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1515;
st1515:
	if ( ++p == pe )
		goto _test_eof1515;
case 1515:
#line 40782 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1826:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1516;
st1516:
	if ( ++p == pe )
		goto _test_eof1516;
case 1516:
#line 40825 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st599;
		case 64: goto st113;
		case 65: goto tr1831;
		case 95: goto st112;
		case 97: goto tr1830;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1831:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1517;
st1517:
	if ( ++p == pe )
		goto _test_eof1517;
case 1517:
#line 40870 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1731;
		case 95: goto st112;
		case 112: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1827:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1518;
st1518:
	if ( ++p == pe )
		goto _test_eof1518;
case 1518:
#line 40914 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1665;
		case 100: goto tr1832;
		case 116: goto tr1833;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1832:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1519;
st1519:
	if ( ++p == pe )
		goto _test_eof1519;
case 1519:
#line 40959 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1833:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1520;
st1520:
	if ( ++p == pe )
		goto _test_eof1520;
case 1520:
#line 41001 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st564;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1666;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1828:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1521;
st1521:
	if ( ++p == pe )
		goto _test_eof1521;
case 1521:
#line 41044 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 67: goto tr1816;
		case 95: goto st112;
		case 99: goto tr1815;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1829:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1522;
st1522:
	if ( ++p == pe )
		goto _test_eof1522;
case 1522:
#line 41088 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 78: goto tr1668;
		case 95: goto st112;
		case 110: goto tr1669;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr948:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1523;
st1523:
	if ( ++p == pe )
		goto _test_eof1523;
case 1523:
#line 41132 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1834;
		case 99: goto tr1835;
		case 101: goto tr596;
		case 105: goto tr596;
		case 110: goto tr1704;
		case 111: goto tr596;
		case 114: goto tr1836;
		case 117: goto tr1837;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1834:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1524;
st1524:
	if ( ++p == pe )
		goto _test_eof1524;
case 1524:
#line 41185 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1835:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1525;
st1525:
	if ( ++p == pe )
		goto _test_eof1525;
case 1525:
#line 41228 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1834;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1836:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1526;
st1526:
	if ( ++p == pe )
		goto _test_eof1526;
case 1526:
#line 41271 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1837:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1527;
st1527:
	if ( ++p == pe )
		goto _test_eof1527;
case 1527:
#line 41313 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1677;
		case 110: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr949:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1528;
st1528:
	if ( ++p == pe )
		goto _test_eof1528;
case 1528:
#line 41357 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1834;
		case 101: goto tr596;
		case 105: goto tr596;
		case 111: goto tr596;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr950:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1529;
st1529:
	if ( ++p == pe )
		goto _test_eof1529;
case 1529:
#line 41407 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 65: goto tr1838;
		case 68: goto tr1839;
		case 95: goto st112;
		case 97: goto tr1840;
		case 98: goto tr1841;
		case 100: goto tr1842;
		case 101: goto tr596;
		case 105: goto tr596;
		case 107: goto tr1843;
		case 110: goto tr1844;
		case 111: goto tr596;
		case 116: goto tr1845;
		case 117: goto tr596;
		case 118: goto tr1825;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 99 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1838:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1530;
st1530:
	if ( ++p == pe )
		goto _test_eof1530;
case 1530:
#line 41465 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1846;
		case 95: goto st112;
		case 115: goto tr1847;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1846:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1531;
st1531:
	if ( ++p == pe )
		goto _test_eof1531;
case 1531:
#line 41510 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1848;
		case 95: goto st112;
		case 116: goto tr1849;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1848:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1532;
st1532:
	if ( ++p == pe )
		goto _test_eof1532;
case 1532:
#line 41555 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 70: goto tr1850;
		case 95: goto st112;
		case 102: goto tr1851;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1850:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1533;
st1533:
	if ( ++p == pe )
		goto _test_eof1533;
case 1533:
#line 41600 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1728;
		case 95: goto st112;
		case 109: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1851:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1534;
st1534:
	if ( ++p == pe )
		goto _test_eof1534;
case 1534:
#line 41645 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1731;
		case 95: goto st112;
		case 109: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1849:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1535;
st1535:
	if ( ++p == pe )
		goto _test_eof1535;
case 1535:
#line 41689 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 70: goto tr1852;
		case 95: goto st112;
		case 102: goto tr1851;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1852:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1536;
st1536:
	if ( ++p == pe )
		goto _test_eof1536;
case 1536:
#line 41733 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1731;
		case 95: goto st112;
		case 109: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1847:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1537;
st1537:
	if ( ++p == pe )
		goto _test_eof1537;
case 1537:
#line 41777 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1853;
		case 95: goto st112;
		case 116: goto tr1849;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1853:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1538;
st1538:
	if ( ++p == pe )
		goto _test_eof1538;
case 1538:
#line 41821 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 70: goto tr1852;
		case 95: goto st112;
		case 102: goto tr1851;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1839:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1539;
st1539:
	if ( ++p == pe )
		goto _test_eof1539;
case 1539:
#line 41865 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1812;
		case 95: goto st112;
		case 97: goto tr1813;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1840:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1540;
st1540:
	if ( ++p == pe )
		goto _test_eof1540;
case 1540:
#line 41910 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 83: goto tr1854;
		case 95: goto st112;
		case 115: goto tr1847;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1854:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1541;
st1541:
	if ( ++p == pe )
		goto _test_eof1541;
case 1541:
#line 41954 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1853;
		case 95: goto st112;
		case 116: goto tr1849;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1841:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1542;
st1542:
	if ( ++p == pe )
		goto _test_eof1542;
case 1542:
#line 41998 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 98: goto tr1811;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1842:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1543;
st1543:
	if ( ++p == pe )
		goto _test_eof1543;
case 1543:
#line 42041 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1822;
		case 95: goto st112;
		case 97: goto tr1813;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1843:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1544;
st1544:
	if ( ++p == pe )
		goto _test_eof1544;
case 1544:
#line 42085 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1844:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1545;
st1545:
	if ( ++p == pe )
		goto _test_eof1545;
case 1545:
#line 42128 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 100: goto tr1566;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1845:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1546;
st1546:
	if ( ++p == pe )
		goto _test_eof1546;
case 1546:
#line 42171 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 100: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr951:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1547;
st1547:
	if ( ++p == pe )
		goto _test_eof1547;
case 1547:
#line 42214 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 65: goto tr1855;
		case 77: goto tr1701;
		case 95: goto st112;
		case 97: goto tr1856;
		case 100: goto tr1857;
		case 101: goto tr596;
		case 105: goto tr1858;
		case 107: goto tr1859;
		case 108: goto tr1843;
		case 109: goto tr1711;
		case 110: goto tr1860;
		case 111: goto tr1861;
		case 114: goto tr1862;
		case 115: goto tr1863;
		case 116: goto tr1864;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1855:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1548;
st1548:
	if ( ++p == pe )
		goto _test_eof1548;
case 1548:
#line 42274 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 73: goto tr1714;
		case 95: goto st112;
		case 105: goto tr1715;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1856:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1549;
st1549:
	if ( ++p == pe )
		goto _test_eof1549;
case 1549:
#line 42319 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st604;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 73: goto tr1736;
		case 95: goto st112;
		case 105: goto tr1715;
		case 106: goto tr1694;
		case 110: goto tr1866;
		case 114: goto tr1677;
		case 115: goto tr1750;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st604:
	if ( ++p == pe )
		goto _test_eof604;
case 604:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 97: goto tr730;
		case 100: goto tr371;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
tr730:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1550;
st1550:
	if ( ++p == pe )
		goto _test_eof1550;
case 1550:
#line 42392 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 109: goto tr1713;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1866:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1551;
st1551:
	if ( ++p == pe )
		goto _test_eof1551;
case 1551:
#line 42432 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 105: goto tr1695;
		case 117: goto tr1867;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1867:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1552;
st1552:
	if ( ++p == pe )
		goto _test_eof1552;
case 1552:
#line 42476 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 102: goto tr1868;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1868:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1553;
st1553:
	if ( ++p == pe )
		goto _test_eof1553;
case 1553:
#line 42519 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1773;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1857:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1554;
st1554:
	if ( ++p == pe )
		goto _test_eof1554;
case 1554:
#line 42562 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 119: goto tr1869;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1869:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1555;
st1555:
	if ( ++p == pe )
		goto _test_eof1555;
case 1555:
#line 42605 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1858:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1556;
st1556:
	if ( ++p == pe )
		goto _test_eof1556;
case 1556:
#line 42648 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1870;
		case 110: goto tr1834;
		case 115: goto tr1750;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1870:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1557;
st1557:
	if ( ++p == pe )
		goto _test_eof1557;
case 1557:
#line 42693 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 104: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1859:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1558;
st1558:
	if ( ++p == pe )
		goto _test_eof1558;
case 1558:
#line 42736 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1871;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1871:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1559;
st1559:
	if ( ++p == pe )
		goto _test_eof1559;
case 1559:
#line 42779 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1860:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1560;
st1560:
	if ( ++p == pe )
		goto _test_eof1560;
case 1560:
#line 42822 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 114: goto tr1677;
		case 116: goto tr1872;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1872:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1561;
st1561:
	if ( ++p == pe )
		goto _test_eof1561;
case 1561:
#line 42866 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1869;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1861:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1562;
st1562:
	if ( ++p == pe )
		goto _test_eof1562;
case 1562:
#line 42909 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1873;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1873:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1563;
st1563:
	if ( ++p == pe )
		goto _test_eof1563;
case 1563:
#line 42952 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1862:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1564;
st1564:
	if ( ++p == pe )
		goto _test_eof1564;
case 1564:
#line 42995 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1863:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1565;
st1565:
	if ( ++p == pe )
		goto _test_eof1565;
case 1565:
#line 43038 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1874;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1874:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1566;
st1566:
	if ( ++p == pe )
		goto _test_eof1566;
case 1566:
#line 43081 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 114: goto tr1875;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1875:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1567;
st1567:
	if ( ++p == pe )
		goto _test_eof1567;
case 1567:
#line 43124 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1864:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1568;
st1568:
	if ( ++p == pe )
		goto _test_eof1568;
case 1568:
#line 43167 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr952:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1569;
st1569:
	if ( ++p == pe )
		goto _test_eof1569;
case 1569:
#line 43210 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1876;
		case 64: goto st113;
		case 69: goto tr1877;
		case 95: goto st112;
		case 97: goto tr1878;
		case 101: goto tr1879;
		case 105: goto tr596;
		case 111: goto tr1880;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1876:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1570;
st1570:
	if ( ++p == pe )
		goto _test_eof1570;
case 1570:
#line 43261 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 32: goto st605;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto tr749;
		case 66: goto tr750;
		case 67: goto tr751;
		case 69: goto tr753;
		case 71: goto tr754;
		case 73: goto tr755;
		case 74: goto tr756;
		case 77: goto tr757;
		case 78: goto tr758;
		case 79: goto tr759;
		case 80: goto tr760;
		case 84: goto tr761;
		case 88: goto tr762;
		case 95: goto st112;
		case 97: goto tr749;
		case 98: goto tr750;
		case 99: goto tr751;
		case 101: goto tr753;
		case 103: goto tr754;
		case 105: goto tr755;
		case 106: goto tr756;
		case 109: goto tr757;
		case 110: goto tr758;
		case 111: goto tr759;
		case 112: goto tr760;
		case 116: goto tr761;
		case 120: goto tr762;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st112;
		} else if ( (*p) >= 9 )
			goto st605;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto tr752;
		} else if ( (*p) >= 68 )
			goto tr752;
	} else
		goto st112;
	goto tr1568;
st605:
	if ( ++p == pe )
		goto _test_eof605;
case 605:
	switch( (*p) ) {
		case 32: goto st605;
		case 67: goto st606;
		case 68: goto st611;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st605;
	goto tr0;
st606:
	if ( ++p == pe )
		goto _test_eof606;
case 606:
	if ( (*p) == 97 )
		goto st607;
	goto tr0;
st607:
	if ( ++p == pe )
		goto _test_eof607;
case 607:
	if ( (*p) == 114 )
		goto st608;
	goto tr0;
st608:
	if ( ++p == pe )
		goto _test_eof608;
case 608:
	if ( (*p) == 111 )
		goto st609;
	goto tr0;
st609:
	if ( ++p == pe )
		goto _test_eof609;
case 609:
	if ( (*p) == 108 )
		goto st610;
	goto tr0;
st610:
	if ( ++p == pe )
		goto _test_eof610;
case 610:
	if ( (*p) == 46 )
		goto tr738;
	goto tr0;
st611:
	if ( ++p == pe )
		goto _test_eof611;
case 611:
	if ( (*p) == 97 )
		goto st612;
	goto tr0;
st612:
	if ( ++p == pe )
		goto _test_eof612;
case 612:
	if ( (*p) == 107 )
		goto st610;
	goto tr0;
tr1877:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1571;
st1571:
	if ( ++p == pe )
		goto _test_eof1571;
case 1571:
#line 43384 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 87: goto tr1881;
		case 95: goto st112;
		case 119: goto tr1882;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1881:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1572;
st1572:
	if ( ++p == pe )
		goto _test_eof1572;
case 1572:
#line 43429 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1645;
		case 95: goto st112;
		case 115: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1882:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1573;
st1573:
	if ( ++p == pe )
		goto _test_eof1573;
case 1573:
#line 43474 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1667;
		case 95: goto st112;
		case 115: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1878:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1574;
st1574:
	if ( ++p == pe )
		goto _test_eof1574;
case 1574:
#line 43518 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1660;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1879:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1575;
st1575:
	if ( ++p == pe )
		goto _test_eof1575;
case 1575:
#line 43561 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 87: goto tr1883;
		case 95: goto st112;
		case 98: goto tr1677;
		case 118: goto tr1677;
		case 119: goto tr1882;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1883:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1576;
st1576:
	if ( ++p == pe )
		goto _test_eof1576;
case 1576:
#line 43607 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 83: goto tr1667;
		case 95: goto st112;
		case 115: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1880:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1577;
st1577:
	if ( ++p == pe )
		goto _test_eof1577;
case 1577:
#line 43651 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st613;
		case 64: goto st113;
		case 95: goto st112;
		case 118: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st613:
	if ( ++p == pe )
		goto _test_eof613;
case 613:
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 32: goto st605;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto tr637;
		case 66: goto tr638;
		case 67: goto tr639;
		case 69: goto tr641;
		case 71: goto tr642;
		case 73: goto tr643;
		case 74: goto tr644;
		case 77: goto tr645;
		case 78: goto tr646;
		case 79: goto tr647;
		case 80: goto tr648;
		case 84: goto tr649;
		case 88: goto tr650;
		case 95: goto st112;
		case 97: goto tr637;
		case 98: goto tr638;
		case 99: goto tr639;
		case 101: goto tr641;
		case 103: goto tr642;
		case 105: goto tr643;
		case 106: goto tr644;
		case 109: goto tr645;
		case 110: goto tr646;
		case 111: goto tr647;
		case 112: goto tr648;
		case 116: goto tr649;
		case 120: goto tr650;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st112;
		} else if ( (*p) >= 9 )
			goto st605;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto tr640;
		} else if ( (*p) >= 68 )
			goto tr640;
	} else
		goto st112;
	goto tr10;
tr953:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1578;
st1578:
	if ( ++p == pe )
		goto _test_eof1578;
case 1578:
#line 43749 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st614;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 95: goto st112;
		case 98: goto tr1559;
		case 99: goto tr1703;
		case 100: goto tr1559;
		case 105: goto tr596;
		case 107: goto tr1886;
		case 110: goto tr1703;
		case 111: goto tr596;
		case 114: goto tr1887;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr1555;
		} else if ( (*p) > 101 ) {
			if ( 102 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr596;
	} else
		goto tr594;
	goto tr997;
st614:
	if ( ++p == pe )
		goto _test_eof614;
case 614:
	switch( (*p) ) {
		case 67: goto tr740;
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 99: goto tr740;
		case 100: goto tr371;
		case 101: goto tr741;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
tr740:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1579;
st1579:
	if ( ++p == pe )
		goto _test_eof1579;
case 1579:
#line 43833 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 108: goto tr1888;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1888:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1580;
st1580:
	if ( ++p == pe )
		goto _test_eof1580;
case 1580:
#line 43873 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
		case 111: goto tr1889;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1889:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1581;
st1581:
	if ( ++p == pe )
		goto _test_eof1581;
case 1581:
#line 43913 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
		case 99: goto tr1890;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1890:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1582;
st1582:
	if ( ++p == pe )
		goto _test_eof1582;
case 1582:
#line 43953 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
		case 107: goto tr1713;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr741:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1583;
st1583:
	if ( ++p == pe )
		goto _test_eof1583;
case 1583:
#line 43993 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 114: goto tr1713;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1886:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1584;
st1584:
	if ( ++p == pe )
		goto _test_eof1584;
case 1584:
#line 44033 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 108: goto tr1891;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1891:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1585;
st1585:
	if ( ++p == pe )
		goto _test_eof1585;
case 1585:
#line 44076 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1887:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1586;
st1586:
	if ( ++p == pe )
		goto _test_eof1586;
case 1586:
#line 44119 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr954:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1587;
st1587:
	if ( ++p == pe )
		goto _test_eof1587;
case 1587:
#line 44162 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1892;
		case 64: goto st113;
		case 79: goto tr1776;
		case 95: goto st112;
		case 97: goto tr1677;
		case 101: goto tr1893;
		case 105: goto tr596;
		case 107: goto tr1894;
		case 108: goto tr1895;
		case 111: goto tr1830;
		case 114: goto tr1896;
		case 116: goto tr1897;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1892:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1588;
st1588:
	if ( ++p == pe )
		goto _test_eof1588;
case 1588:
#line 44217 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto tr749;
		case 66: goto tr750;
		case 67: goto tr751;
		case 69: goto tr753;
		case 71: goto tr754;
		case 73: goto tr755;
		case 74: goto tr756;
		case 77: goto tr773;
		case 78: goto tr758;
		case 79: goto tr759;
		case 80: goto tr760;
		case 84: goto tr761;
		case 88: goto tr762;
		case 95: goto st112;
		case 97: goto tr749;
		case 98: goto tr750;
		case 99: goto tr751;
		case 101: goto tr753;
		case 103: goto tr754;
		case 105: goto tr755;
		case 106: goto tr756;
		case 109: goto tr757;
		case 110: goto tr758;
		case 111: goto tr759;
		case 112: goto tr760;
		case 116: goto tr761;
		case 120: goto tr762;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st112;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto tr752;
		} else if ( (*p) >= 68 )
			goto tr752;
	} else
		goto st112;
	goto tr1568;
tr773:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1589;
st1589:
	if ( ++p == pe )
		goto _test_eof1589;
case 1589:
#line 44275 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1898;
		case 64: goto st113;
		case 73: goto tr1611;
		case 79: goto tr1612;
		case 85: goto tr1613;
		case 95: goto st112;
		case 105: goto tr1614;
		case 111: goto tr1615;
		case 117: goto tr1616;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1585;
		} else
			goto tr1581;
	} else
		goto st8;
	goto tr997;
tr1893:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1590;
st1590:
	if ( ++p == pe )
		goto _test_eof1590;
case 1590:
#line 44323 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1899;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1899:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1591;
st1591:
	if ( ++p == pe )
		goto _test_eof1591;
case 1591:
#line 44366 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 110: goto tr1900;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1900:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1592;
st1592:
	if ( ++p == pe )
		goto _test_eof1592;
case 1592:
#line 44409 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1894:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1593;
st1593:
	if ( ++p == pe )
		goto _test_eof1593;
case 1593:
#line 44452 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 119: goto tr1811;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1895:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1594;
st1594:
	if ( ++p == pe )
		goto _test_eof1594;
case 1594:
#line 44495 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1665;
		case 110: goto tr1869;
		case 122: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1896:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1595;
st1595:
	if ( ++p == pe )
		goto _test_eof1595;
case 1595:
#line 44540 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1693;
		case 111: goto tr1901;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1901:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1596;
st1596:
	if ( ++p == pe )
		goto _test_eof1596;
case 1596:
#line 44584 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 102: goto tr1694;
		case 118: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1897:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1597;
st1597:
	if ( ++p == pe )
		goto _test_eof1597;
case 1597:
#line 44628 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 121: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr955:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1598;
st1598:
	if ( ++p == pe )
		goto _test_eof1598;
case 1598:
#line 44671 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr596;
		case 101: goto tr596;
		case 105: goto tr596;
		case 111: goto tr596;
		case 117: goto tr1902;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1902:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1599;
st1599:
	if ( ++p == pe )
		goto _test_eof1599;
case 1599:
#line 44721 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1677;
		case 114: goto tr1903;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1903:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1600;
st1600:
	if ( ++p == pe )
		goto _test_eof1600;
case 1600:
#line 44765 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st615;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
st615:
	if ( ++p == pe )
		goto _test_eof615;
case 615:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 97: goto tr742;
		case 100: goto tr371;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
tr742:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1601;
st1601:
	if ( ++p == pe )
		goto _test_eof1601;
case 1601:
#line 44832 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 110: goto tr1713;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr956:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1602;
st1602:
	if ( ++p == pe )
		goto _test_eof1602;
case 1602:
#line 44872 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 77: goto tr1905;
		case 83: goto tr1906;
		case 84: goto tr1907;
		case 95: goto st112;
		case 97: goto tr596;
		case 100: goto tr1710;
		case 101: goto tr1908;
		case 105: goto tr596;
		case 109: goto tr1909;
		case 111: goto tr596;
		case 115: goto tr1910;
		case 116: goto tr1911;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1905:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1603;
st1603:
	if ( ++p == pe )
		goto _test_eof1603;
case 1603:
#line 44929 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 73: goto tr1728;
		case 95: goto st112;
		case 105: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1906:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1604;
st1604:
	if ( ++p == pe )
		goto _test_eof1604;
case 1604:
#line 44974 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 89: goto tr1912;
		case 95: goto st112;
		case 121: goto tr1913;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1912:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1605;
st1605:
	if ( ++p == pe )
		goto _test_eof1605;
case 1605:
#line 45019 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1914;
		case 95: goto st112;
		case 110: goto tr1915;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1914:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1606;
st1606:
	if ( ++p == pe )
		goto _test_eof1606;
case 1606:
#line 45064 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 67: goto tr1728;
		case 95: goto st112;
		case 99: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1915:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1607;
st1607:
	if ( ++p == pe )
		goto _test_eof1607;
case 1607:
#line 45109 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 67: goto tr1731;
		case 95: goto st112;
		case 99: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1913:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1608;
st1608:
	if ( ++p == pe )
		goto _test_eof1608;
case 1608:
#line 45153 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1916;
		case 95: goto st112;
		case 110: goto tr1915;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1916:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1609;
st1609:
	if ( ++p == pe )
		goto _test_eof1609;
case 1609:
#line 45197 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 67: goto tr1731;
		case 95: goto st112;
		case 99: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1907:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1610;
st1610:
	if ( ++p == pe )
		goto _test_eof1610;
case 1610:
#line 45241 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 77: goto tr1776;
		case 95: goto st112;
		case 109: goto tr1830;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1908:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1611;
st1611:
	if ( ++p == pe )
		goto _test_eof1611;
case 1611:
#line 45286 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 112: goto tr1694;
		case 113: goto tr1747;
		case 118: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1909:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1612;
st1612:
	if ( ++p == pe )
		goto _test_eof1612;
case 1612:
#line 45331 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 73: goto tr1731;
		case 95: goto st112;
		case 105: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1910:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1613;
st1613:
	if ( ++p == pe )
		goto _test_eof1613;
case 1613:
#line 45375 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 89: goto tr1917;
		case 95: goto st112;
		case 121: goto tr1913;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1917:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1614;
st1614:
	if ( ++p == pe )
		goto _test_eof1614;
case 1614:
#line 45419 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1916;
		case 95: goto st112;
		case 110: goto tr1915;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1911:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1615;
st1615:
	if ( ++p == pe )
		goto _test_eof1615;
case 1615:
#line 45463 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1663;
		case 64: goto st113;
		case 77: goto tr1831;
		case 95: goto st112;
		case 100: goto tr1694;
		case 109: goto tr1830;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr957:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1616;
st1616:
	if ( ++p == pe )
		goto _test_eof1616;
case 1616:
#line 45508 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1876;
		case 64: goto st113;
		case 70: goto tr1918;
		case 72: goto tr1919;
		case 75: goto tr1920;
		case 77: goto tr1921;
		case 79: goto tr1922;
		case 83: goto tr1923;
		case 95: goto st112;
		case 97: goto tr1695;
		case 101: goto tr1924;
		case 102: goto tr1925;
		case 103: goto tr1926;
		case 104: goto tr1927;
		case 105: goto tr596;
		case 107: goto tr1928;
		case 109: goto tr1929;
		case 110: goto tr1704;
		case 111: goto tr1930;
		case 114: goto tr1836;
		case 115: goto tr1931;
		case 116: goto tr1932;
		case 117: goto tr1834;
		case 118: goto tr1933;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1918:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1617;
st1617:
	if ( ++p == pe )
		goto _test_eof1617;
case 1617:
#line 45574 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1776;
		case 95: goto st112;
		case 116: goto tr1830;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1919:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1618;
st1618:
	if ( ++p == pe )
		goto _test_eof1618;
case 1618:
#line 45619 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 79: goto tr1700;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1920:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1619;
st1619:
	if ( ++p == pe )
		goto _test_eof1619;
case 1619:
#line 45663 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 89: goto tr1934;
		case 95: goto st112;
		case 121: goto tr1935;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1934:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1620;
st1620:
	if ( ++p == pe )
		goto _test_eof1620;
case 1620:
#line 45708 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1936;
		case 95: goto st112;
		case 112: goto tr1937;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1936:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1621;
st1621:
	if ( ++p == pe )
		goto _test_eof1621;
case 1621:
#line 45753 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1645;
		case 95: goto st112;
		case 101: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1937:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1622;
st1622:
	if ( ++p == pe )
		goto _test_eof1622;
case 1622:
#line 45798 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1667;
		case 95: goto st112;
		case 101: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1935:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1623;
st1623:
	if ( ++p == pe )
		goto _test_eof1623;
case 1623:
#line 45842 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1938;
		case 95: goto st112;
		case 112: goto tr1937;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1938:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1624;
st1624:
	if ( ++p == pe )
		goto _test_eof1624;
case 1624:
#line 45886 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1667;
		case 95: goto st112;
		case 101: goto tr1647;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1921:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1625;
st1625:
	if ( ++p == pe )
		goto _test_eof1625;
case 1625:
#line 45930 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 66: goto tr1728;
		case 83: goto tr1645;
		case 84: goto tr1776;
		case 95: goto st112;
		case 98: goto tr1729;
		case 115: goto tr1647;
		case 116: goto tr1830;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1922:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1626;
st1626:
	if ( ++p == pe )
		goto _test_eof1626;
case 1626:
#line 45979 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1776;
		case 95: goto st112;
		case 97: goto tr1830;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1923:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1627;
st1627:
	if ( ++p == pe )
		goto _test_eof1627;
case 1627:
#line 46024 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 72: goto tr1728;
		case 95: goto st112;
		case 104: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1924:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1628;
st1628:
	if ( ++p == pe )
		goto _test_eof1628;
case 1628:
#line 46069 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1694;
		case 110: goto tr1694;
		case 112: goto tr1873;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1925:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1629;
st1629:
	if ( ++p == pe )
		goto _test_eof1629;
case 1629:
#line 46114 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 84: goto tr1831;
		case 95: goto st112;
		case 116: goto tr1830;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1926:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1630;
st1630:
	if ( ++p == pe )
		goto _test_eof1630;
case 1630:
#line 46158 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 116: goto tr1694;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1927:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1631;
st1631:
	if ( ++p == pe )
		goto _test_eof1631;
case 1631:
#line 46201 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 111: goto tr1939;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1939:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1632;
st1632:
	if ( ++p == pe )
		goto _test_eof1632;
case 1632:
#line 46244 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 117: goto tr1746;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1928:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1633;
st1633:
	if ( ++p == pe )
		goto _test_eof1633;
case 1633:
#line 46287 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 89: goto tr1940;
		case 95: goto st112;
		case 121: goto tr1935;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1940:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1634;
st1634:
	if ( ++p == pe )
		goto _test_eof1634;
case 1634:
#line 46331 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 80: goto tr1938;
		case 95: goto st112;
		case 112: goto tr1937;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1929:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1635;
st1635:
	if ( ++p == pe )
		goto _test_eof1635;
case 1635:
#line 46375 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 66: goto tr1731;
		case 83: goto tr1667;
		case 84: goto tr1831;
		case 95: goto st112;
		case 98: goto tr1729;
		case 115: goto tr1647;
		case 116: goto tr1830;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1930:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1636;
st1636:
	if ( ++p == pe )
		goto _test_eof1636;
case 1636:
#line 46423 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st613;
		case 64: goto st113;
		case 65: goto tr1831;
		case 95: goto st112;
		case 97: goto tr1830;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1931:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1637;
st1637:
	if ( ++p == pe )
		goto _test_eof1637;
case 1637:
#line 46467 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 72: goto tr1731;
		case 95: goto st112;
		case 104: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1932:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1638;
st1638:
	if ( ++p == pe )
		goto _test_eof1638;
case 1638:
#line 46511 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1677;
		case 110: goto tr1677;
		case 114: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1933:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1639;
st1639:
	if ( ++p == pe )
		goto _test_eof1639;
case 1639:
#line 46556 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1871;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr958:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1640;
st1640:
	if ( ++p == pe )
		goto _test_eof1640;
case 1640:
#line 46599 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 69: goto tr1941;
		case 95: goto st112;
		case 97: goto tr596;
		case 101: goto tr1942;
		case 104: goto tr1943;
		case 105: goto tr596;
		case 111: goto tr596;
		case 112: goto tr1944;
		case 114: goto tr1945;
		case 117: goto tr1946;
		case 119: goto tr1745;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1941:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1641;
st1641:
	if ( ++p == pe )
		goto _test_eof1641;
case 1641:
#line 46653 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1947;
		case 95: goto st112;
		case 108: goto tr1948;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1947:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1642;
st1642:
	if ( ++p == pe )
		goto _test_eof1642;
case 1642:
#line 46698 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1949;
		case 95: goto st112;
		case 110: goto tr1950;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1949:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1643;
st1643:
	if ( ++p == pe )
		goto _test_eof1643;
case 1643:
#line 46743 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1762;
		case 95: goto st112;
		case 101: goto tr1763;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1950:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1644;
st1644:
	if ( ++p == pe )
		goto _test_eof1644;
case 1644:
#line 46788 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1768;
		case 95: goto st112;
		case 101: goto tr1763;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1948:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1645;
st1645:
	if ( ++p == pe )
		goto _test_eof1645;
case 1645:
#line 46832 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1951;
		case 95: goto st112;
		case 110: goto tr1950;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1951:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1646;
st1646:
	if ( ++p == pe )
		goto _test_eof1646;
case 1646:
#line 46876 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1768;
		case 95: goto st112;
		case 101: goto tr1763;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1942:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1647;
st1647:
	if ( ++p == pe )
		goto _test_eof1647;
case 1647:
#line 46920 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 76: goto tr1952;
		case 95: goto st112;
		case 108: goto tr1948;
		case 110: goto tr1834;
		case 120: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1952:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1648;
st1648:
	if ( ++p == pe )
		goto _test_eof1648;
case 1648:
#line 46966 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1951;
		case 95: goto st112;
		case 110: goto tr1950;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1943:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1649;
st1649:
	if ( ++p == pe )
		goto _test_eof1649;
case 1649:
#line 47010 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 117: goto tr1953;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1953:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1650;
st1650:
	if ( ++p == pe )
		goto _test_eof1650;
case 1650:
#line 47053 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 114: goto tr1750;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1944:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1651;
st1651:
	if ( ++p == pe )
		goto _test_eof1651;
case 1651:
#line 47096 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 107: goto tr1954;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1954:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1652;
st1652:
	if ( ++p == pe )
		goto _test_eof1652;
case 1652:
#line 47139 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1945:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1653;
st1653:
	if ( ++p == pe )
		goto _test_eof1653;
case 1653:
#line 47182 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 112: goto tr1955;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st2;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st8;
		} else
			goto st3;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr595;
		} else if ( (*p) > 106 ) {
			if ( (*p) > 108 ) {
				if ( 109 <= (*p) && (*p) <= 122 )
					goto tr596;
			} else if ( (*p) >= 107 )
				goto tr1677;
		} else
			goto tr596;
	} else
		goto tr594;
	goto tr997;
tr1955:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1654;
st1654:
	if ( ++p == pe )
		goto _test_eof1654;
case 1654:
#line 47231 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 107: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1946:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1655;
st1655:
	if ( ++p == pe )
		goto _test_eof1655;
case 1655:
#line 47274 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 101: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr959:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1656;
st1656:
	if ( ++p == pe )
		goto _test_eof1656;
case 1656:
#line 47317 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 78: goto tr1956;
		case 95: goto st112;
		case 97: goto tr596;
		case 101: goto tr596;
		case 105: goto tr596;
		case 110: goto tr1957;
		case 111: goto tr596;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1956:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1657;
st1657:
	if ( ++p == pe )
		goto _test_eof1657;
case 1657:
#line 47369 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 82: goto tr1958;
		case 95: goto st112;
		case 114: goto tr1959;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1958:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1658;
st1658:
	if ( ++p == pe )
		goto _test_eof1658;
case 1658:
#line 47414 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1960;
		case 95: goto st112;
		case 101: goto tr1961;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1960:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1659;
st1659:
	if ( ++p == pe )
		goto _test_eof1659;
case 1659:
#line 47459 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1962;
		case 95: goto st112;
		case 97: goto tr1963;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1962:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1660;
st1660:
	if ( ++p == pe )
		goto _test_eof1660;
case 1660:
#line 47504 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1728;
		case 95: goto st112;
		case 108: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1963:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1661;
st1661:
	if ( ++p == pe )
		goto _test_eof1661;
case 1661:
#line 47549 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1731;
		case 95: goto st112;
		case 108: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1961:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1662;
st1662:
	if ( ++p == pe )
		goto _test_eof1662;
case 1662:
#line 47593 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1964;
		case 95: goto st112;
		case 97: goto tr1963;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1964:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1663;
st1663:
	if ( ++p == pe )
		goto _test_eof1663;
case 1663:
#line 47637 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 76: goto tr1731;
		case 95: goto st112;
		case 108: goto tr1729;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1959:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1664;
st1664:
	if ( ++p == pe )
		goto _test_eof1664;
case 1664:
#line 47681 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1965;
		case 95: goto st112;
		case 101: goto tr1961;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1965:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1665;
st1665:
	if ( ++p == pe )
		goto _test_eof1665;
case 1665:
#line 47725 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 65: goto tr1964;
		case 95: goto st112;
		case 97: goto tr1963;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1957:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1666;
st1666:
	if ( ++p == pe )
		goto _test_eof1666;
case 1666:
#line 47769 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 82: goto tr1966;
		case 95: goto st112;
		case 105: goto tr1967;
		case 114: goto tr1959;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1966:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1667;
st1667:
	if ( ++p == pe )
		goto _test_eof1667;
case 1667:
#line 47814 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 69: goto tr1965;
		case 95: goto st112;
		case 101: goto tr1961;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1967:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1668;
st1668:
	if ( ++p == pe )
		goto _test_eof1668;
case 1668:
#line 47858 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1666;
		case 64: goto st113;
		case 95: goto st112;
		case 118: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr960:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1669;
st1669:
	if ( ++p == pe )
		goto _test_eof1669;
case 1669:
#line 47901 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1677;
		case 100: goto tr1968;
		case 101: goto tr596;
		case 105: goto tr1891;
		case 108: goto tr1969;
		case 111: goto tr596;
		case 116: goto tr1970;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr1968:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1670;
st1670:
	if ( ++p == pe )
		goto _test_eof1670;
case 1670:
#line 47954 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1695;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1969:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1671;
st1671:
	if ( ++p == pe )
		goto _test_eof1671;
case 1671:
#line 47997 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 103: goto tr1677;
		case 121: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1970:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1672;
st1672:
	if ( ++p == pe )
		goto _test_eof1672;
case 1672:
#line 48041 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr961:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1673;
st1673:
	if ( ++p == pe )
		goto _test_eof1673;
case 1673:
#line 48083 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st616;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1972;
		case 64: goto st113;
		case 65: goto tr1764;
		case 69: goto tr1973;
		case 79: goto tr1974;
		case 83: goto tr1645;
		case 95: goto st112;
		case 97: goto tr1975;
		case 101: goto tr1976;
		case 105: goto tr1977;
		case 108: goto tr1710;
		case 111: goto tr1978;
		case 115: goto tr1979;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr1743;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
st616:
	if ( ++p == pe )
		goto _test_eof616;
case 616:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 100: goto tr371;
		case 101: goto tr743;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
tr743:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1674;
st1674:
	if ( ++p == pe )
		goto _test_eof1674;
case 1674:
#line 48164 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 110: goto tr1980;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1980:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1675;
st1675:
	if ( ++p == pe )
		goto _test_eof1675;
case 1675:
#line 48204 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
		case 100: goto tr1713;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1972:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1676;
st1676:
	if ( ++p == pe )
		goto _test_eof1676;
case 1676:
#line 48244 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -30: goto st282;
		case 32: goto st617;
		case 38: goto st285;
		case 39: goto st214;
		case 43: goto st112;
		case 64: goto st113;
		case 65: goto tr749;
		case 66: goto tr750;
		case 67: goto tr751;
		case 69: goto tr753;
		case 71: goto tr754;
		case 73: goto tr755;
		case 74: goto tr756;
		case 77: goto tr757;
		case 78: goto tr758;
		case 79: goto tr759;
		case 80: goto tr760;
		case 84: goto tr761;
		case 88: goto tr762;
		case 95: goto st112;
		case 97: goto tr749;
		case 98: goto tr750;
		case 99: goto tr751;
		case 101: goto tr753;
		case 103: goto tr754;
		case 105: goto tr755;
		case 106: goto tr756;
		case 109: goto tr757;
		case 110: goto tr758;
		case 111: goto tr759;
		case 112: goto tr760;
		case 116: goto tr761;
		case 120: goto tr762;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st112;
		} else if ( (*p) >= 9 )
			goto st617;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto tr752;
		} else if ( (*p) >= 68 )
			goto tr752;
	} else
		goto st112;
	goto tr1568;
st617:
	if ( ++p == pe )
		goto _test_eof617;
case 617:
	switch( (*p) ) {
		case 32: goto st617;
		case 86: goto st618;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st617;
	goto tr592;
st618:
	if ( ++p == pe )
		goto _test_eof618;
case 618:
	if ( (*p) == 97 )
		goto st610;
	goto tr592;
tr1973:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1677;
st1677:
	if ( ++p == pe )
		goto _test_eof1677;
case 1677:
#line 48324 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 66: goto tr1981;
		case 82: goto tr1558;
		case 95: goto st112;
		case 98: goto tr1982;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1981:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1678;
st1678:
	if ( ++p == pe )
		goto _test_eof1678;
case 1678:
#line 48370 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 67: goto tr1960;
		case 95: goto st112;
		case 99: goto tr1961;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1982:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1679;
st1679:
	if ( ++p == pe )
		goto _test_eof1679;
case 1679:
#line 48415 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 67: goto tr1965;
		case 95: goto st112;
		case 99: goto tr1961;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1974:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 34;}
	goto st1680;
st1680:
	if ( ++p == pe )
		goto _test_eof1680;
case 1680:
#line 48459 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1646;
		case 85: goto tr1734;
		case 95: goto st112;
		case 110: goto tr1648;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr1568;
tr1975:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1681;
st1681:
	if ( ++p == pe )
		goto _test_eof1681;
case 1681:
#line 48505 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1983;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1983:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1682;
st1682:
	if ( ++p == pe )
		goto _test_eof1682;
case 1682:
#line 48548 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1668;
		case 95: goto st112;
		case 104: goto tr1677;
		case 110: goto tr1669;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1976:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1683;
st1683:
	if ( ++p == pe )
		goto _test_eof1683;
case 1683:
#line 48593 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 66: goto tr1984;
		case 95: goto st112;
		case 98: goto tr1982;
		case 100: goto tr1677;
		case 114: goto tr1817;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1984:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1684;
st1684:
	if ( ++p == pe )
		goto _test_eof1684;
case 1684:
#line 48639 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st310;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 67: goto tr1965;
		case 95: goto st112;
		case 99: goto tr1961;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1977:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1685;
st1685:
	if ( ++p == pe )
		goto _test_eof1685;
case 1685:
#line 48683 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 95: goto st112;
		case 115: goto tr1985;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1985:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1686;
st1686:
	if ( ++p == pe )
		goto _test_eof1686;
case 1686:
#line 48726 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1678;
		case 64: goto st113;
		case 95: goto st112;
		case 99: goto tr1677;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1978:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1687;
st1687:
	if ( ++p == pe )
		goto _test_eof1687;
case 1687:
#line 48769 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto st526;
		case 64: goto st113;
		case 78: goto tr1668;
		case 95: goto st112;
		case 110: goto tr1669;
		case 117: goto tr1746;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr1979:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1688;
st1688:
	if ( ++p == pe )
		goto _test_eof1688;
case 1688:
#line 48814 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 43: goto st563;
		case 45: goto tr1553;
		case 46: goto st526;
		case 58: goto st527;
		case 64: goto st113;
		case 95: goto st112;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr596;
		} else
			goto tr595;
	} else
		goto st8;
	goto tr997;
tr962:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1689;
st1689:
	if ( ++p == pe )
		goto _test_eof1689;
case 1689:
#line 48857 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st315;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr596;
		case 101: goto tr596;
		case 105: goto tr596;
		case 111: goto tr596;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
tr963:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1690;
st1690:
	if ( ++p == pe )
		goto _test_eof1690;
case 1690:
#line 48907 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 36: goto tr787;
		case 38: goto st492;
		case 39: goto st619;
		case 43: goto st112;
		case 45: goto tr1553;
		case 46: goto tr1684;
		case 64: goto st113;
		case 95: goto st112;
		case 97: goto tr1987;
		case 101: goto tr596;
		case 105: goto tr596;
		case 111: goto tr596;
		case 117: goto tr596;
		case 119: goto tr596;
		case 121: goto tr596;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr594;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1559;
		} else
			goto tr1555;
	} else
		goto st8;
	goto tr997;
st619:
	if ( ++p == pe )
		goto _test_eof619;
case 619:
	switch( (*p) ) {
		case 68: goto tr365;
		case 76: goto tr366;
		case 77: goto tr367;
		case 82: goto tr368;
		case 83: goto tr369;
		case 86: goto tr370;
		case 97: goto tr746;
		case 100: goto tr371;
		case 107: goto tr747;
		case 108: goto tr372;
		case 109: goto tr373;
		case 114: goto tr374;
		case 115: goto tr369;
		case 118: goto tr375;
	}
	if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr311;
	} else if ( (*p) >= 65 )
		goto tr311;
	goto tr10;
tr746:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1691;
st1691:
	if ( ++p == pe )
		goto _test_eof1691;
case 1691:
#line 48983 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 108: goto tr1988;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1988:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1692;
st1692:
	if ( ++p == pe )
		goto _test_eof1692;
case 1692:
#line 49023 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
		case 108: goto tr1713;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr747:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1693;
st1693:
	if ( ++p == pe )
		goto _test_eof1693;
case 1693:
#line 49063 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st273;
		case 45: goto tr1302;
		case 46: goto st281;
		case 110: goto tr1989;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1989:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1694;
st1694:
	if ( ++p == pe )
		goto _test_eof1694;
case 1694:
#line 49103 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
		case 111: goto tr1990;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st3;
		} else if ( (*p) >= -61 )
			goto st2;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr311;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr332;
		} else
			goto tr331;
	} else
		goto st8;
	goto tr997;
tr1990:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "src/ragel/rules/main.rl"
	{ s.act = 40;}
	goto st1695;
st1695:
	if ( ++p == pe )
		goto _test_eof1695;
case 1695:
#line 49143 "src/lib/schwa/tokenizer/tokenizer.cc"
	switch( (*p) ) {
		case -111: goto st214;
		case -110: goto st215;
		case -62: goto st1;
		case -30: goto st222;
		case 38: goto st236;
		case 39: goto st315;
		case 45: goto tr1302;
		case 46: goto st316;
		case 119: goto tr1713;
	}
	if ( (*p) < -16 ) {
				goto st3;
	}
