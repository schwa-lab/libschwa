
#line 1 "tokenizer.rl"
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 21 "tokenizer.rl"


#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/scoped_array.hpp>

#include "tokens.h"
#include "stream.h"
#include "tokenizer.h"
#include "state.h"

using namespace std;
using namespace boost;

namespace schwa { namespace tokens {


#line 24 "tokenizer.cc"
static const int tokenizer_start = 624;
static const int tokenizer_error = 0;

static const int tokenizer_en_main = 624;


#line 37 "tokenizer.rl"

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
  
#line 177 "tokenizer.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch (  s.cs )
	{
tr2:
#line 7 "rules/main.rl"
	{ s.te = p+1;{ word_(WORD, dest, s); }}
	goto st624;
tr5:
#line 1 "NONE"
	{	switch(  s.act ) {
	case 0:
	{{goto st0;}}
	break;
	case 1:
	{{p = (( s.te))-1;} single_quote_(dest, s); }
	break;
	case 3:
	{{p = (( s.te))-1;} open_single_quote_(dest, s); }
	break;
	case 4:
	{{p = (( s.te))-1;} close_single_quote_(dest, s); }
	break;
	case 5:
	{{p = (( s.te))-1;} open_double_quote_(dest, s); }
	break;
	case 6:
	{{p = (( s.te))-1;} close_double_quote_(dest, s); }
	break;
	case 7:
	{{p = (( s.te))-1;} terminator_(dest, s, "."); }
	break;
	case 10:
	{{p = (( s.te))-1;} terminator_(dest, s, "..."); }
	break;
	case 11:
	{{p = (( s.te))-1;} dash_or_item_(dest, s); }
	break;
	case 12:
	{{p = (( s.te))-1;}}
	break;
	case 22:
	{{p = (( s.te))-1;} split_(WORD, CONTRACTION, dest, s); }
	break;
	case 23:
	{{p = (( s.te))-1;} split_(WORD, CONTRACTION, dest, s); }
	break;
	case 24:
	{{p = (( s.te))-1;} split_(WORD, CONTRACTION, dest, s); }
	break;
	case 25:
	{{p = (( s.te))-1;} split_(WORD, CONTRACTION, dest, s); }
	break;
	case 26:
	{{p = (( s.te))-1;} split_(WORD, CONTRACTION, dest, s); }
	break;
	case 27:
	{{p = (( s.te))-1;} split_(WORD, POSSESSIVE, dest, s); }
	break;
	case 28:
	{{p = (( s.te))-1;} split_(NUMBER, UNIT, dest, s); }
	break;
	case 30:
	{{p = (( s.te))-1;} word_(WORD, dest, s); }
	break;
	case 31:
	{{p = (( s.te))-1;} word_(WORD, dest, s); }
	break;
	case 33:
	{{p = (( s.te))-1;} word_(WORD, dest, s); }
	break;
	case 34:
	{{p = (( s.te))-1;} word_(WORD, dest, s); }
	break;
	case 35:
	{{p = (( s.te))-1;} punct_(PUNCTUATION, dest, s); }
	break;
	case 36:
	{{p = (( s.te))-1;} word_(WORD, dest, s); }
	break;
	case 37:
	{{p = (( s.te))-1;} word_(WORD, dest, s); }
	break;
	case 38:
	{{p = (( s.te))-1;} word_(WORD, dest, s); }
	break;
	case 39:
	{{p = (( s.te))-1;} word_(WORD, dest, s); }
	break;
	}
	}
	goto st624;
tr14:
#line 7 "rules/main.rl"
	{{p = (( s.te))-1;}{ word_(WORD, dest, s); }}
	goto st624;
tr22:
#line 6 "rules/main.rl"
	{{p = (( s.te))-1;}}
	goto st624;
tr28:
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr35:
#line 11 "actions.rl"
	{ s.suffix += 6; }
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr40:
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/units.rl"
	{n2="am";}
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr42:
#line 9 "actions.rl"
	{ s.suffix += 4; }
#line 47 "rules/units.rl"
	{n2="am";}
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr44:
#line 51 "rules/main.rl"
	{{p = (( s.te))-1;}{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr46:
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 48 "rules/units.rl"
	{n2="pm";}
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr48:
#line 9 "actions.rl"
	{ s.suffix += 4; }
#line 48 "rules/units.rl"
	{n2="pm";}
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr51:
#line 8 "actions.rl"
	{ s.suffix += 3; }
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr59:
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr66:
#line 10 "actions.rl"
	{ s.suffix += 5; }
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr68:
#line 9 "actions.rl"
	{ s.suffix += 4; }
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr82:
#line 7 "rules/main.rl"
	{{p = (( s.te))-1;}{ word_(WORD, dest, s); }}
	goto st624;
tr91:
#line 8 "rules/main.rl"
	{ s.te = p+1;{ punct_(PUNCTUATION, dest, s); }}
	goto st624;
tr92:
#line 25 "rules/main.rl"
	{ s.te = p+1;{ dash_or_item_(dest, s); }}
	goto st624;
tr95:
#line 14 "rules/main.rl"
	{ s.te = p+1;{ open_single_quote_(dest, s); }}
	goto st624;
tr96:
#line 17 "rules/main.rl"
	{ s.te = p+1;{ open_double_quote_(dest, s); }}
	goto st624;
tr97:
#line 18 "rules/main.rl"
	{ s.te = p+1;{ close_double_quote_(dest, s); }}
	goto st624;
tr99:
#line 14 "rules/main.rl"
	{{p = (( s.te))-1;}{ open_single_quote_(dest, s); }}
	goto st624;
tr114:
#line 15 "rules/main.rl"
	{{p = (( s.te))-1;}{ close_single_quote_(dest, s); }}
	goto st624;
tr133:
#line 8 "rules/main.rl"
	{{p = (( s.te))-1;}{ punct_(PUNCTUATION, dest, s); }}
	goto st624;
tr144:
#line 15 "rules/main.rl"
	{ s.te = p+1;{ close_single_quote_(dest, s); }}
	goto st624;
tr147:
#line 12 "rules/main.rl"
	{ s.te = p+1;{ double_quote_(dest, s); }}
	goto st624;
tr148:
#line 11 "rules/main.rl"
	{ s.te = p+1;{ single_quote_(dest, s); }}
	goto st624;
tr155:
#line 23 "rules/main.rl"
	{ s.te = p+1;{ terminator_(dest, s, "..."); }}
	goto st624;
tr171:
#line 10 "rules/contractions.rl"
	{n2="'D";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr173:
#line 10 "rules/contractions.rl"
	{n2="'M";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr175:
#line 6 "actions.rl"
	{ s.suffix++; }
#line 49 "rules/main.rl"
	{n2="'s";}
#line 49 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, POSSESSIVE, dest, s); }}
	goto st624;
tr177:
#line 9 "rules/contractions.rl"
	{n2="'d";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr179:
#line 9 "rules/contractions.rl"
	{n2="'m";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr182:
#line 13 "rules/contractions.rl"
	{n2="'LL";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr183:
#line 13 "rules/contractions.rl"
	{n2="'RE";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr184:
#line 13 "rules/contractions.rl"
	{n2="'VE";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr185:
#line 12 "rules/contractions.rl"
	{n2="'ll";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr186:
#line 12 "rules/contractions.rl"
	{n2="'re";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr187:
#line 12 "rules/contractions.rl"
	{n2="'ve";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr273:
#line 11 "rules/main.rl"
	{{p = (( s.te))-1;}{ single_quote_(dest, s); }}
	goto st624;
tr275:
#line 7 "rules/main.rl"
	{ s.te = p+1;{ word_(WORD, dest, s); }}
	goto st624;
tr323:
#line 7 "rules/main.rl"
	{{p = (( s.te))-1;}{ word_(WORD, dest, s); }}
	goto st624;
tr379:
#line 25 "rules/main.rl"
	{{p = (( s.te))-1;}{ dash_or_item_(dest, s); }}
	goto st624;
tr427:
#line 20 "rules/main.rl"
	{{p = (( s.te))-1;}{ terminator_(dest, s, "."); }}
	goto st624;
tr472:
#line 6 "rules/main.rl"
	{ s.te = p+1;}
	goto st624;
tr478:
#line 6 "rules/main.rl"
	{{p = (( s.te))-1;}}
	goto st624;
tr486:
#line 38 "rules/main.rl"
	{ s.te = p+1;{ end_html_item_(dest, s); }}
	goto st624;
tr488:
#line 35 "rules/main.rl"
	{ s.te = p+1;{ end_html_list_(dest, s); }}
	goto st624;
tr489:
#line 31 "rules/main.rl"
	{ s.te = p+1;{ end_html_paragraph_(dest, s); }}
	goto st624;
tr519:
#line 37 "rules/main.rl"
	{ s.te = p+1;{ begin_html_item_(dest, s); }}
	goto st624;
tr521:
#line 34 "rules/main.rl"
	{ s.te = p+1;{ begin_html_list_(dest, s); }}
	goto st624;
tr525:
#line 30 "rules/main.rl"
	{ s.te = p+1;{ begin_html_paragraph_(dest, s); }}
	goto st624;
tr530:
#line 7 "rules/main.rl"
	{{p = (( s.te))-1;}{ word_(WORD, dest, s); }}
	goto st624;
tr596:
#line 22 "rules/contractions.rl"
	{n2="N'T";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 42 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr623:
#line 7 "rules/main.rl"
	{ s.te = p+1;{ word_(WORD, dest, s); }}
	goto st624;
tr628:
#line 21 "rules/contractions.rl"
	{n2="n't";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 42 "rules/main.rl"
	{ s.te = p+1;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr666:
#line 7 "rules/main.rl"
	{ s.te = p+1;{ word_(WORD, dest, s); }}
	goto st624;
tr698:
#line 22 "rules/main.rl"
	{ s.te = p+1;{ terminator_(dest, s, "!"); }}
	goto st624;
tr712:
#line 21 "rules/main.rl"
	{ s.te = p+1;{ terminator_(dest, s, "?"); }}
	goto st624;
tr763:
#line 7 "rules/main.rl"
	{ s.te = p;p--;{ word_(WORD, dest, s); }}
	goto st624;
tr766:
#line 6 "rules/main.rl"
	{ s.te = p;p--;}
	goto st624;
tr771:
#line 7 "rules/main.rl"
	{ s.te = p;p--;{ word_(WORD, dest, s); }}
	goto st624;
tr797:
#line 7 "rules/main.rl"
	{ s.te = p;p--;{ word_(WORD, dest, s); }}
	goto st624;
tr798:
#line 51 "rules/main.rl"
	{ s.te = p;p--;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr804:
#line 51 "rules/main.rl"
	{ s.te = p+1;{ split_(NUMBER, UNIT, dest, s); }}
	goto st624;
tr896:
#line 14 "rules/main.rl"
	{ s.te = p;p--;{ open_single_quote_(dest, s); }}
	goto st624;
tr899:
#line 15 "rules/main.rl"
	{ s.te = p;p--;{ close_single_quote_(dest, s); }}
	goto st624;
tr903:
#line 8 "rules/main.rl"
	{ s.te = p;p--;{ punct_(PUNCTUATION, dest, s); }}
	goto st624;
tr921:
#line 11 "rules/main.rl"
	{ s.te = p;p--;{ single_quote_(dest, s); }}
	goto st624;
tr927:
#line 7 "rules/main.rl"
	{ s.te = p;p--;{ word_(WORD, dest, s); }}
	goto st624;
tr928:
#line 7 "rules/main.rl"
	{ s.te = p+1;{ word_(WORD, dest, s); }}
	goto st624;
tr979:
#line 25 "rules/main.rl"
	{ s.te = p;p--;{ dash_or_item_(dest, s); }}
	goto st624;
tr985:
#line 47 "rules/main.rl"
	{ s.te = p;p--;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr1069:
#line 20 "rules/main.rl"
	{ s.te = p;p--;{ terminator_(dest, s, "."); }}
	goto st624;
tr1120:
#line 6 "rules/main.rl"
	{ s.te = p;p--;}
	goto st624;
tr1139:
#line 7 "rules/main.rl"
	{ s.te = p;p--;{ word_(WORD, dest, s); }}
	goto st624;
tr1278:
#line 46 "rules/main.rl"
	{ s.te = p;p--;{ split_(WORD, CONTRACTION, dest, s); }}
	goto st624;
tr1662:
#line 7 "rules/main.rl"
	{ s.te = p;p--;{ word_(WORD, dest, s); }}
	goto st624;
st624:
#line 1 "NONE"
	{ s.ts = 0;}
#line 1 "NONE"
	{ s.act = 0;}
	if ( ++p == pe )
		goto _test_eof624;
case 624:
#line 1 "NONE"
	{ s.ts = p;}
#line 665 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st1;
		case -30: goto st100;
		case 10: goto tr696;
		case 13: goto tr697;
		case 32: goto tr1;
		case 33: goto tr698;
		case 34: goto tr147;
		case 35: goto st714;
		case 36: goto tr700;
		case 38: goto tr701;
		case 39: goto tr702;
		case 42: goto tr703;
		case 43: goto tr704;
		case 45: goto tr705;
		case 46: goto tr706;
		case 50: goto tr708;
		case 60: goto tr710;
		case 61: goto tr711;
		case 63: goto tr712;
		case 65: goto tr713;
		case 66: goto tr714;
		case 67: goto tr715;
		case 68: goto tr716;
		case 69: goto tr717;
		case 70: goto tr718;
		case 71: goto tr719;
		case 72: goto tr720;
		case 73: goto tr721;
		case 74: goto tr722;
		case 75: goto tr723;
		case 76: goto tr724;
		case 77: goto tr725;
		case 78: goto tr726;
		case 79: goto tr727;
		case 80: goto tr728;
		case 81: goto tr729;
		case 82: goto tr730;
		case 83: goto tr731;
		case 84: goto tr732;
		case 85: goto tr733;
		case 86: goto tr734;
		case 87: goto tr735;
		case 88: goto tr736;
		case 89: goto tr737;
		case 90: goto tr736;
		case 95: goto tr738;
		case 96: goto st1358;
		case 97: goto tr740;
		case 99: goto tr742;
		case 100: goto tr743;
		case 101: goto tr744;
		case 102: goto tr745;
		case 103: goto tr746;
		case 104: goto tr747;
		case 105: goto tr748;
		case 108: goto tr749;
		case 109: goto tr750;
		case 110: goto tr751;
		case 111: goto tr752;
		case 112: goto tr753;
		case 113: goto tr754;
		case 114: goto tr755;
		case 115: goto tr756;
		case 116: goto tr757;
		case 117: goto tr758;
		case 118: goto tr759;
		case 119: goto tr760;
		case 121: goto tr761;
	}
	if ( (*p) < 37 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( (*p) > -12 ) {
				if ( 9 <= (*p) && (*p) <= 12 )
					goto tr1;
			} else if ( (*p) >= -16 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 47 ) {
		if ( (*p) < 58 ) {
			if ( (*p) > 49 ) {
				if ( 51 <= (*p) && (*p) <= 57 )
					goto tr709;
			} else if ( (*p) >= 48 )
				goto tr707;
		} else if ( (*p) > 94 ) {
			if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto tr91;
			} else if ( (*p) >= 98 )
				goto tr741;
		} else
			goto tr91;
	} else
		goto tr91;
	goto st0;
st0:
 s.cs = 0;
	goto _out;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case -96: goto tr1;
		case -92: goto tr0;
		case -79: goto tr3;
	}
	if ( (*p) < -94 ) {
		if ( (*p) <= -95 )
			goto tr0;
	} else if ( (*p) > -91 ) {
		if ( -90 <= (*p) && (*p) <= -65 )
			goto tr0;
	} else
		goto tr2;
	goto st0;
tr0:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st625;
tr129:
#line 1 "NONE"
	{ s.te = p+1;}
#line 14 "rules/main.rl"
	{ s.act = 3;}
	goto st625;
tr130:
#line 1 "NONE"
	{ s.te = p+1;}
#line 15 "rules/main.rl"
	{ s.act = 4;}
	goto st625;
tr98:
#line 1 "NONE"
	{ s.te = p+1;}
#line 23 "rules/main.rl"
	{ s.act = 10;}
	goto st625;
tr131:
#line 1 "NONE"
	{ s.te = p+1;}
#line 17 "rules/main.rl"
	{ s.act = 5;}
	goto st625;
tr132:
#line 1 "NONE"
	{ s.te = p+1;}
#line 18 "rules/main.rl"
	{ s.act = 6;}
	goto st625;
tr861:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st625;
st625:
	if ( ++p == pe )
		goto _test_eof625;
case 625:
#line 835 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 45: goto tr762;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr5;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case -95: goto tr0;
		case -92: goto tr0;
	}
	if ( (*p) > -97 ) {
		if ( -90 <= (*p) && (*p) <= -65 )
			goto tr0;
	} else
		goto tr0;
	goto tr5;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) <= -65 )
		goto tr0;
	goto tr5;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) <= -65 )
		goto st3;
	goto tr5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case -128: goto st6;
		case -126: goto st7;
		case -120: goto st8;
	}
	if ( -127 <= (*p) && (*p) <= -65 )
		goto st3;
	goto tr5;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case -102: goto tr0;
		case -98: goto tr0;
	}
	if ( (*p) < -106 ) {
		if ( (*p) <= -119 )
			goto tr0;
	} else if ( (*p) > -105 ) {
		if ( -96 <= (*p) && (*p) <= -65 )
			goto tr0;
	} else
		goto tr0;
	goto tr5;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) < -91 ) {
		if ( (*p) <= -94 )
			goto tr0;
	} else if ( (*p) > -85 ) {
		if ( -83 <= (*p) && (*p) <= -65 )
			goto tr0;
	} else
		goto tr0;
	goto tr5;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) > -111 ) {
		if ( -109 <= (*p) && (*p) <= -65 )
			goto tr0;
	} else
		goto tr0;
	goto tr5;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) <= -65 )
		goto st4;
	goto tr5;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 97 )
		goto st11;
	goto tr5;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 109 )
		goto st12;
	goto tr5;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 112 )
		goto st13;
	goto tr5;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 59 )
		goto tr0;
	goto tr5;
tr762:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st626;
st626:
	if ( ++p == pe )
		goto _test_eof626;
case 626:
#line 985 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 45: goto st14;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 45: goto st14;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr14;
tr20:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st627;
tr849:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/units.rl"
	{n2="am";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st627;
tr863:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st627;
tr854:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 48 "rules/units.rl"
	{n2="pm";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st627;
tr822:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st627;
st627:
	if ( ++p == pe )
		goto _test_eof627;
case 627:
#line 1084 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st15;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr5;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 97 )
		goto st11;
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr0;
	goto tr5;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else if ( (*p) >= 65 )
		goto tr0;
	goto tr5;
tr21:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st628;
tr856:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/units.rl"
	{n2="am";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st628;
tr852:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st628;
tr893:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 48 "rules/units.rl"
	{n2="pm";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st628;
tr858:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "actions.rl"
	{ s.suffix += 3; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st628;
tr870:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "actions.rl"
	{ s.suffix += 4; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st628;
tr872:
#line 1 "NONE"
	{ s.te = p+1;}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st628;
tr877:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st628;
st628:
	if ( ++p == pe )
		goto _test_eof628;
case 628:
#line 1198 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr5;
tr1:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "rules/main.rl"
	{ s.act = 12;}
	goto st629;
st629:
	if ( ++p == pe )
		goto _test_eof629;
case 629:
#line 1235 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st17;
		case 32: goto tr1;
		case 38: goto st18;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr1;
	goto tr766;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == -96 )
		goto tr1;
	goto tr22;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 110 )
		goto st19;
	goto tr22;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 98 )
		goto st20;
	goto tr5;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 115 )
		goto st21;
	goto tr5;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 112 )
		goto st22;
	goto tr5;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 59 )
		goto tr1;
	goto tr5;
tr3:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st630;
st630:
	if ( ++p == pe )
		goto _test_eof630;
case 630:
#line 1296 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 45: goto tr762;
		case 46: goto st23;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr770;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr27;
	goto tr5;
tr27:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st631;
st631:
	if ( ++p == pe )
		goto _test_eof631;
case 631:
#line 1339 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 45: goto st34;
		case 65: goto st35;
		case 71: goto st38;
		case 76: goto tr59;
		case 77: goto tr777;
		case 80: goto st40;
		case 97: goto st43;
		case 98: goto st45;
		case 99: goto tr781;
		case 100: goto st55;
		case 102: goto st57;
		case 103: goto tr784;
		case 104: goto st64;
		case 105: goto st65;
		case 107: goto tr787;
		case 108: goto st69;
		case 109: goto tr789;
		case 110: goto st80;
		case 111: goto st82;
		case 112: goto st83;
		case 113: goto st51;
		case 114: goto st85;
		case 115: goto st87;
		case 116: goto st88;
		case 121: goto st90;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr27;
	goto tr771;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == -94 )
		goto tr28;
	goto tr5;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 35: goto st26;
		case 99: goto st31;
	}
	goto tr5;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 49: goto st27;
		case 88: goto st30;
		case 120: goto st30;
	}
	goto tr5;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 54 )
		goto st28;
	goto tr5;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 50 )
		goto st29;
	goto tr5;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 59 )
		goto tr35;
	goto tr5;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 65: goto st28;
		case 97: goto st28;
	}
	goto tr5;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 101 )
		goto st32;
	goto tr5;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 110 )
		goto st33;
	goto tr5;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 116 )
		goto st29;
	goto tr5;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr38;
	} else if ( (*p) >= 65 )
		goto tr38;
	goto tr5;
tr38:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 31;}
	goto st632;
st632:
	if ( ++p == pe )
		goto _test_eof632;
case 632:
#line 1469 "tokenizer.cc"
	if ( (*p) == 45 )
		goto st34;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr38;
	} else if ( (*p) >= 65 )
		goto tr38;
	goto tr797;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 46: goto st36;
		case 77: goto tr40;
	}
	goto tr5;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 77 )
		goto st37;
	goto tr5;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 46 )
		goto tr42;
	goto tr5;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	switch( (*p) ) {
		case 66: goto tr43;
		case 98: goto tr43;
	}
	goto tr5;
tr43:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
	goto st633;
st633:
	if ( ++p == pe )
		goto _test_eof633;
case 633:
#line 1520 "tokenizer.cc"
	if ( (*p) == 105 )
		goto st39;
	goto tr798;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 116 )
		goto tr28;
	goto tr44;
tr777:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st634;
st634:
	if ( ++p == pe )
		goto _test_eof634;
case 634:
#line 1539 "tokenizer.cc"
	switch( (*p) ) {
		case 66: goto tr43;
		case 98: goto tr43;
	}
	goto tr798;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	switch( (*p) ) {
		case 46: goto st41;
		case 77: goto tr46;
	}
	goto tr5;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 77 )
		goto st42;
	goto tr5;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 46 )
		goto tr48;
	goto tr5;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 46: goto st44;
		case 109: goto tr40;
	}
	goto tr5;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 109 )
		goto st37;
	goto tr5;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	if ( (*p) == 112 )
		goto st46;
	goto tr5;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 115 )
		goto tr51;
	goto tr5;
tr781:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st635;
st635:
	if ( ++p == pe )
		goto _test_eof635;
case 635:
#line 1610 "tokenizer.cc"
	switch( (*p) ) {
		case 109: goto tr800;
		case 117: goto st48;
		case 119: goto st39;
	}
	goto tr798;
tr63:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
	goto st636;
tr800:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st636;
st636:
	if ( ++p == pe )
		goto _test_eof636;
case 636:
#line 1633 "tokenizer.cc"
	if ( (*p) == 94 )
		goto st47;
	if ( 50 <= (*p) && (*p) <= 51 )
		goto tr59;
	goto tr798;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( 50 <= (*p) && (*p) <= 51 )
		goto tr28;
	goto tr44;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 32: goto tr52;
		case 46: goto st54;
	}
	goto tr5;
tr52:
#line 7 "actions.rl"
	{ s.suffix += 2; }
	goto st49;
tr60:
#line 8 "actions.rl"
	{ s.suffix += 3; }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 1667 "tokenizer.cc"
	switch( (*p) ) {
		case 99: goto st50;
		case 102: goto st51;
		case 105: goto st52;
		case 107: goto st50;
		case 109: goto tr57;
		case 121: goto st53;
	}
	goto tr5;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 109 )
		goto tr59;
	goto tr5;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 116 )
		goto tr28;
	goto tr5;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 110 )
		goto tr28;
	goto tr5;
tr57:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st637;
st637:
	if ( ++p == pe )
		goto _test_eof637;
case 637:
#line 1706 "tokenizer.cc"
	switch( (*p) ) {
		case 105: goto tr59;
		case 109: goto tr59;
	}
	goto tr798;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 100 )
		goto tr28;
	goto tr5;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 32 )
		goto tr60;
	goto tr5;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 66: goto tr28;
		case 112: goto st56;
		case 114: goto tr28;
	}
	goto tr5;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 105 )
		goto tr51;
	goto tr5;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 108: goto st58;
		case 116: goto tr63;
	}
	goto tr5;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 32 )
		goto st59;
	goto tr5;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 111 )
		goto st60;
	goto tr5;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 122 )
		goto tr66;
	goto tr5;
tr784:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st638;
st638:
	if ( ++p == pe )
		goto _test_eof638;
case 638:
#line 1783 "tokenizer.cc"
	switch( (*p) ) {
		case 97: goto st61;
		case 98: goto st62;
	}
	goto tr798;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 108 )
		goto tr51;
	goto tr44;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 105 )
		goto st63;
	goto tr44;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 116 )
		goto tr68;
	goto tr44;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 114 )
		goto st639;
	goto tr5;
st639:
	if ( ++p == pe )
		goto _test_eof639;
case 639:
	if ( (*p) == 115 )
		goto tr804;
	goto tr798;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 110 )
		goto tr63;
	goto tr5;
tr787:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st640;
st640:
	if ( ++p == pe )
		goto _test_eof640;
case 640:
#line 1841 "tokenizer.cc"
	switch( (*p) ) {
		case 66: goto tr28;
		case 98: goto tr805;
		case 103: goto tr28;
		case 109: goto tr806;
		case 112: goto st68;
	}
	goto tr798;
tr805:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
	goto st641;
st641:
	if ( ++p == pe )
		goto _test_eof641;
case 641:
#line 1860 "tokenizer.cc"
	if ( (*p) == 105 )
		goto st66;
	goto tr798;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 116 )
		goto tr51;
	goto tr44;
tr806:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st642;
st642:
	if ( ++p == pe )
		goto _test_eof642;
case 642:
#line 1881 "tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st67;
		case 94: goto st47;
		case 104: goto tr59;
	}
	if ( 50 <= (*p) && (*p) <= 51 )
		goto tr59;
	goto tr798;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 104 )
		goto tr28;
	goto tr44;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 104 )
		goto tr51;
	goto tr44;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 98 )
		goto tr70;
	goto tr5;
tr70:
#line 7 "actions.rl"
	{ s.suffix += 2; }
	goto st643;
st643:
	if ( ++p == pe )
		goto _test_eof643;
case 643:
#line 1919 "tokenizer.cc"
	if ( (*p) == 115 )
		goto tr59;
	goto tr798;
tr789:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st644;
st644:
	if ( ++p == pe )
		goto _test_eof644;
case 644:
#line 1933 "tokenizer.cc"
	switch( (*p) ) {
		case 47: goto st70;
		case 76: goto tr59;
		case 94: goto st47;
		case 98: goto st71;
		case 103: goto tr59;
		case 105: goto tr811;
		case 109: goto tr800;
		case 110: goto st78;
		case 111: goto st639;
		case 112: goto st67;
		case 116: goto st79;
	}
	if ( 50 <= (*p) && (*p) <= 51 )
		goto tr59;
	goto tr798;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 115 )
		goto tr28;
	goto tr44;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	if ( (*p) == 105 )
		goto st66;
	goto tr44;
tr811:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st645;
st645:
	if ( ++p == pe )
		goto _test_eof645;
case 645:
#line 1976 "tokenizer.cc"
	switch( (*p) ) {
		case 94: goto st47;
		case 99: goto st72;
		case 108: goto tr814;
		case 110: goto st639;
	}
	if ( 50 <= (*p) && (*p) <= 51 )
		goto tr59;
	goto tr798;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 114 )
		goto st73;
	goto tr44;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) == 111 )
		goto st74;
	goto tr5;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	if ( (*p) == 115 )
		goto st75;
	goto tr5;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 101 )
		goto st76;
	goto tr5;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	if ( (*p) == 99 )
		goto st639;
	goto tr5;
tr814:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st646;
st646:
	if ( ++p == pe )
		goto _test_eof646;
case 646:
#line 2033 "tokenizer.cc"
	if ( (*p) == 108 )
		goto st77;
	goto tr798;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	if ( (*p) == 105 )
		goto st74;
	goto tr44;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 116 )
		goto st79;
	goto tr44;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 104 )
		goto st639;
	goto tr44;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 97: goto st81;
		case 111: goto tr28;
	}
	goto tr5;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	if ( (*p) == 110 )
		goto st73;
	goto tr5;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	if ( (*p) == 122 )
		goto tr28;
	goto tr5;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 46: goto st84;
		case 109: goto tr46;
		case 116: goto tr28;
	}
	goto tr5;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 109 )
		goto st42;
	goto tr5;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 112 )
		goto st86;
	goto tr5;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) == 109 )
		goto tr51;
	goto tr5;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 101: goto st76;
		case 113: goto st48;
	}
	goto tr5;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( (*p) == 111 )
		goto st89;
	goto tr5;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	if ( (*p) == 110 )
		goto tr51;
	goto tr5;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 100: goto tr63;
		case 114: goto st639;
	}
	goto tr5;
tr770:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st647;
st647:
	if ( ++p == pe )
		goto _test_eof647;
case 647:
#line 2154 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st5;
		case 38: goto st92;
		case 44: goto st93;
		case 45: goto tr819;
		case 46: goto st23;
		case 47: goto st94;
		case 65: goto tr820;
		case 71: goto tr821;
		case 76: goto tr822;
		case 77: goto tr823;
		case 80: goto tr824;
		case 97: goto tr825;
		case 98: goto tr826;
		case 99: goto tr827;
		case 100: goto tr828;
		case 102: goto tr829;
		case 103: goto tr830;
		case 104: goto tr831;
		case 105: goto tr832;
		case 107: goto tr833;
		case 108: goto tr834;
		case 109: goto tr835;
		case 110: goto tr836;
		case 111: goto tr837;
		case 112: goto tr838;
		case 113: goto tr839;
		case 114: goto tr840;
		case 115: goto tr841;
		case 116: goto tr842;
		case 121: goto tr843;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr770;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr771;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case -95: goto tr0;
		case -94: goto tr28;
		case -92: goto tr0;
	}
	if ( (*p) > -97 ) {
		if ( -90 <= (*p) && (*p) <= -65 )
			goto tr0;
	} else
		goto tr0;
	goto tr82;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	switch( (*p) ) {
		case 35: goto st26;
		case 97: goto st11;
		case 99: goto st31;
	}
	goto tr82;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr83;
	goto tr82;
tr83:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st648;
st648:
	if ( ++p == pe )
		goto _test_eof648;
case 648:
#line 2248 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 44: goto st93;
		case 45: goto st34;
		case 46: goto st23;
		case 65: goto st35;
		case 71: goto st38;
		case 76: goto tr59;
		case 77: goto tr777;
		case 80: goto st40;
		case 97: goto st43;
		case 98: goto st45;
		case 99: goto tr781;
		case 100: goto st55;
		case 102: goto st57;
		case 103: goto tr784;
		case 104: goto st64;
		case 105: goto st65;
		case 107: goto tr787;
		case 108: goto st69;
		case 109: goto tr789;
		case 110: goto st80;
		case 111: goto st82;
		case 112: goto st83;
		case 113: goto st51;
		case 114: goto st85;
		case 115: goto st87;
		case 116: goto st88;
		case 121: goto st90;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr83;
	goto tr771;
tr819:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st649;
st649:
	if ( ++p == pe )
		goto _test_eof649;
case 649:
#line 2293 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 45: goto st14;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr844;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr846;
		} else
			goto tr845;
	} else
		goto st9;
	goto tr763;
tr844:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st650;
st650:
	if ( ++p == pe )
		goto _test_eof650;
case 650:
#line 2328 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 45: goto tr762;
		case 47: goto st94;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr844;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr84;
	goto tr5;
tr84:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st651;
st651:
	if ( ++p == pe )
		goto _test_eof651;
case 651:
#line 2371 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 65: goto st35;
		case 71: goto st38;
		case 76: goto tr59;
		case 77: goto tr777;
		case 80: goto st40;
		case 97: goto st43;
		case 98: goto st45;
		case 99: goto tr781;
		case 100: goto st55;
		case 102: goto st57;
		case 103: goto tr784;
		case 104: goto st64;
		case 105: goto st65;
		case 107: goto tr787;
		case 108: goto st69;
		case 109: goto tr789;
		case 110: goto st80;
		case 111: goto st82;
		case 112: goto st83;
		case 113: goto st51;
		case 114: goto st85;
		case 115: goto st87;
		case 116: goto st88;
		case 121: goto st90;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr84;
	goto tr771;
tr845:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 31;}
	goto st652;
st652:
	if ( ++p == pe )
		goto _test_eof652;
case 652:
#line 2413 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st15;
		case 39: goto st16;
		case 45: goto tr847;
		case 46: goto st16;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr846;
		} else
			goto tr845;
	} else
		goto st9;
	goto tr797;
tr847:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st653;
st653:
	if ( ++p == pe )
		goto _test_eof653;
case 653:
#line 2450 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 45: goto st14;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr846;
		} else
			goto tr845;
	} else
		goto st9;
	goto tr763;
tr846:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 31;}
	goto st654;
st654:
	if ( ++p == pe )
		goto _test_eof654;
case 654:
#line 2485 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr847;
		case 46: goto st16;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr846;
		} else
			goto tr845;
	} else
		goto st9;
	goto tr797;
tr820:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st655;
st655:
	if ( ++p == pe )
		goto _test_eof655;
case 655:
#line 2522 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st15;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st95;
		case 77: goto tr849;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	if ( (*p) == 77 )
		goto tr85;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else if ( (*p) >= 65 )
		goto tr0;
	goto tr14;
tr85:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st656;
st656:
	if ( ++p == pe )
		goto _test_eof656;
case 656:
#line 2572 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 45: goto tr762;
		case 46: goto tr42;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr821:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st657;
tr823:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st657;
st657:
	if ( ++p == pe )
		goto _test_eof657;
case 657:
#line 2616 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st15;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 66: goto tr850;
		case 98: goto tr851;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr5;
tr850:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st658;
st658:
	if ( ++p == pe )
		goto _test_eof658;
case 658:
#line 2657 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st15;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 105: goto tr839;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr798;
tr839:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st659;
st659:
	if ( ++p == pe )
		goto _test_eof659;
case 659:
#line 2695 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 116: goto tr852;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr851:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st660;
st660:
	if ( ++p == pe )
		goto _test_eof660;
case 660:
#line 2735 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 105: goto tr839;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr798;
tr824:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st661;
st661:
	if ( ++p == pe )
		goto _test_eof661;
case 661:
#line 2773 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st15;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st96;
		case 77: goto tr854;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	if ( (*p) == 77 )
		goto tr86;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else if ( (*p) >= 65 )
		goto tr0;
	goto tr14;
tr86:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st662;
st662:
	if ( ++p == pe )
		goto _test_eof662;
case 662:
#line 2823 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 45: goto tr762;
		case 46: goto tr48;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr825:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st663;
st663:
	if ( ++p == pe )
		goto _test_eof663;
case 663:
#line 2859 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st97;
		case 109: goto tr856;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	if ( (*p) == 109 )
		goto tr85;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else if ( (*p) >= 65 )
		goto tr0;
	goto tr14;
tr826:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st664;
st664:
	if ( ++p == pe )
		goto _test_eof664;
case 664:
#line 2909 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 112: goto tr857;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr857:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st665;
st665:
	if ( ++p == pe )
		goto _test_eof665;
case 665:
#line 2947 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 115: goto tr858;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr827:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st666;
st666:
	if ( ++p == pe )
		goto _test_eof666;
case 666:
#line 2987 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 109: goto tr859;
		case 117: goto tr860;
		case 119: goto tr839;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr798;
tr866:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st667;
tr859:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st667;
st667:
	if ( ++p == pe )
		goto _test_eof667;
case 667:
#line 3037 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 94: goto st47;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr861;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr21;
			} else if ( (*p) >= 65 )
				goto tr20;
		} else
			goto tr0;
	} else
		goto tr0;
	goto tr798;
tr860:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st668;
st668:
	if ( ++p == pe )
		goto _test_eof668;
case 668:
#line 3081 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 32: goto tr52;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st98;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	if ( (*p) == 32 )
		goto tr60;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else if ( (*p) >= 65 )
		goto tr0;
	goto tr14;
tr828:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st669;
st669:
	if ( ++p == pe )
		goto _test_eof669;
case 669:
#line 3131 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 66: goto tr863;
		case 112: goto tr864;
		case 114: goto tr852;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr864:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st670;
st670:
	if ( ++p == pe )
		goto _test_eof670;
case 670:
#line 3171 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 105: goto tr858;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr829:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st671;
st671:
	if ( ++p == pe )
		goto _test_eof671;
case 671:
#line 3209 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 108: goto tr865;
		case 116: goto tr866;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr865:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st672;
st672:
	if ( ++p == pe )
		goto _test_eof672;
case 672:
#line 3248 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 32: goto st59;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr830:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st673;
st673:
	if ( ++p == pe )
		goto _test_eof673;
case 673:
#line 3288 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 97: goto tr867;
		case 98: goto tr868;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 99 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr798;
tr867:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st674;
st674:
	if ( ++p == pe )
		goto _test_eof674;
case 674:
#line 3327 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 108: goto tr858;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr868:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st675;
st675:
	if ( ++p == pe )
		goto _test_eof675;
case 675:
#line 3365 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 105: goto tr869;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr869:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st676;
st676:
	if ( ++p == pe )
		goto _test_eof676;
case 676:
#line 3403 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 116: goto tr870;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr831:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st677;
st677:
	if ( ++p == pe )
		goto _test_eof677;
case 677:
#line 3441 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 114: goto tr871;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr871:
#line 1 "NONE"
	{ s.te = p+1;}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st678;
st678:
	if ( ++p == pe )
		goto _test_eof678;
case 678:
#line 3479 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 115: goto tr872;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr798;
tr832:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st679;
st679:
	if ( ++p == pe )
		goto _test_eof679;
case 679:
#line 3517 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 110: goto tr866;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr833:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st680;
st680:
	if ( ++p == pe )
		goto _test_eof680;
case 680:
#line 3557 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 66: goto tr863;
		case 98: goto tr873;
		case 103: goto tr852;
		case 109: goto tr874;
		case 112: goto tr875;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr798;
tr879:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st681;
tr873:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st681;
st681:
	if ( ++p == pe )
		goto _test_eof681;
case 681:
#line 3607 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 105: goto tr876;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr5;
tr876:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st682;
st682:
	if ( ++p == pe )
		goto _test_eof682;
case 682:
#line 3645 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 116: goto tr858;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr874:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st683;
st683:
	if ( ++p == pe )
		goto _test_eof683;
case 683:
#line 3685 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 47: goto st67;
		case 94: goto st47;
		case 104: goto tr877;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr861;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr21;
			} else if ( (*p) >= 65 )
				goto tr20;
		} else
			goto tr0;
	} else
		goto tr0;
	goto tr798;
tr875:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st684;
st684:
	if ( ++p == pe )
		goto _test_eof684;
case 684:
#line 3731 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 104: goto tr858;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr834:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st685;
st685:
	if ( ++p == pe )
		goto _test_eof685;
case 685:
#line 3769 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 98: goto tr878;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr878:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st686;
st686:
	if ( ++p == pe )
		goto _test_eof686;
case 686:
#line 3809 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 115: goto tr877;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr798;
tr835:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st687;
st687:
	if ( ++p == pe )
		goto _test_eof687;
case 687:
#line 3849 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 47: goto st70;
		case 76: goto tr822;
		case 94: goto st47;
		case 98: goto tr879;
		case 103: goto tr877;
		case 105: goto tr880;
		case 109: goto tr859;
		case 110: goto tr881;
		case 111: goto tr871;
		case 112: goto tr882;
		case 116: goto tr883;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr861;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr21;
			} else if ( (*p) >= 65 )
				goto tr20;
		} else
			goto tr0;
	} else
		goto tr0;
	goto tr798;
tr880:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st688;
st688:
	if ( ++p == pe )
		goto _test_eof688;
case 688:
#line 3905 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 94: goto st47;
		case 99: goto tr884;
		case 108: goto tr885;
		case 110: goto tr871;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr861;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr21;
			} else if ( (*p) >= 65 )
				goto tr20;
		} else
			goto tr0;
	} else
		goto tr0;
	goto tr798;
tr884:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st689;
st689:
	if ( ++p == pe )
		goto _test_eof689;
case 689:
#line 3952 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 114: goto tr886;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr886:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st690;
st690:
	if ( ++p == pe )
		goto _test_eof690;
case 690:
#line 3990 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 111: goto tr887;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr887:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st691;
st691:
	if ( ++p == pe )
		goto _test_eof691;
case 691:
#line 4028 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 115: goto tr888;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr888:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st692;
st692:
	if ( ++p == pe )
		goto _test_eof692;
case 692:
#line 4066 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 101: goto tr889;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr889:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st693;
st693:
	if ( ++p == pe )
		goto _test_eof693;
case 693:
#line 4104 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 99: goto tr871;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr885:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st694;
st694:
	if ( ++p == pe )
		goto _test_eof694;
case 694:
#line 4144 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 108: goto tr890;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr798;
tr890:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st695;
st695:
	if ( ++p == pe )
		goto _test_eof695;
case 695:
#line 4182 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 105: goto tr887;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr881:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st696;
st696:
	if ( ++p == pe )
		goto _test_eof696;
case 696:
#line 4220 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 116: goto tr883;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr883:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st697;
st697:
	if ( ++p == pe )
		goto _test_eof697;
case 697:
#line 4258 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 104: goto tr871;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr882:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st698;
st698:
	if ( ++p == pe )
		goto _test_eof698;
case 698:
#line 4296 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 104: goto tr852;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr836:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st699;
st699:
	if ( ++p == pe )
		goto _test_eof699;
case 699:
#line 4334 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 97: goto tr891;
		case 111: goto tr852;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr891:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st700;
st700:
	if ( ++p == pe )
		goto _test_eof700;
case 700:
#line 4373 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 110: goto tr886;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr837:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st701;
st701:
	if ( ++p == pe )
		goto _test_eof701;
case 701:
#line 4411 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 122: goto tr852;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr838:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st702;
st702:
	if ( ++p == pe )
		goto _test_eof702;
case 702:
#line 4449 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st99;
		case 109: goto tr893;
		case 116: goto tr852;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	if ( (*p) == 109 )
		goto tr86;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr0;
	} else if ( (*p) >= 65 )
		goto tr0;
	goto tr14;
tr840:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st703;
st703:
	if ( ++p == pe )
		goto _test_eof703;
case 703:
#line 4500 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 112: goto tr894;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr894:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st704;
st704:
	if ( ++p == pe )
		goto _test_eof704;
case 704:
#line 4538 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 109: goto tr858;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr841:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st705;
st705:
	if ( ++p == pe )
		goto _test_eof705;
case 705:
#line 4576 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 101: goto tr889;
		case 113: goto tr860;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr842:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st706;
st706:
	if ( ++p == pe )
		goto _test_eof706;
case 706:
#line 4615 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 111: goto tr895;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr895:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st707;
st707:
	if ( ++p == pe )
		goto _test_eof707;
case 707:
#line 4653 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 110: goto tr858;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
tr843:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st708;
st708:
	if ( ++p == pe )
		goto _test_eof708;
case 708:
#line 4691 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st5;
		case 38: goto st10;
		case 39: goto st16;
		case 45: goto tr762;
		case 46: goto st16;
		case 100: goto tr866;
		case 114: goto tr871;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr0;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr21;
		} else
			goto tr20;
	} else
		goto st9;
	goto tr763;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	switch( (*p) ) {
		case -128: goto st101;
		case -126: goto st134;
		case -120: goto st135;
		case -99: goto st136;
	}
	if ( -127 <= (*p) && (*p) <= -65 )
		goto st3;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	switch( (*p) ) {
		case -104: goto tr93;
		case -103: goto tr94;
		case -101: goto tr95;
		case -100: goto tr96;
		case -99: goto tr97;
		case -97: goto tr96;
		case -90: goto tr98;
	}
	if ( (*p) < -112 ) {
		if ( (*p) <= -119 )
			goto tr0;
	} else if ( (*p) > -111 ) {
		if ( (*p) > -107 ) {
			if ( -106 <= (*p) && (*p) <= -65 )
				goto tr0;
		} else if ( (*p) >= -110 )
			goto tr92;
	} else
		goto tr91;
	goto st0;
tr93:
#line 1 "NONE"
	{ s.te = p+1;}
#line 14 "rules/main.rl"
	{ s.act = 3;}
	goto st709;
st709:
	if ( ++p == pe )
		goto _test_eof709;
case 709:
#line 4768 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st102;
		case 38: goto st104;
	}
	goto tr896;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	if ( (*p) == -128 )
		goto st103;
	goto tr99;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( (*p) == -104 )
		goto tr96;
	goto tr99;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	switch( (*p) ) {
		case 35: goto st105;
		case 108: goto st114;
	}
	goto tr99;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	switch( (*p) ) {
		case 56: goto st106;
		case 88: goto st110;
		case 120: goto st110;
	}
	goto tr99;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	if ( (*p) == 50 )
		goto st107;
	goto tr99;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	if ( (*p) == 49 )
		goto st108;
	goto tr99;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	if ( (*p) == 54 )
		goto st109;
	goto tr99;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	if ( (*p) == 59 )
		goto tr96;
	goto tr5;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) == 50 )
		goto st111;
	goto tr99;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	if ( (*p) == 48 )
		goto st112;
	goto tr99;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	if ( (*p) == 49 )
		goto st113;
	goto tr99;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	if ( (*p) == 56 )
		goto st109;
	goto tr99;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	if ( (*p) == 115 )
		goto st115;
	goto tr99;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	if ( (*p) == 113 )
		goto st116;
	goto tr5;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	if ( (*p) == 117 )
		goto st117;
	goto tr5;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	if ( (*p) == 111 )
		goto st109;
	goto tr5;
tr94:
#line 1 "NONE"
	{ s.te = p+1;}
#line 15 "rules/main.rl"
	{ s.act = 4;}
	goto st710;
st710:
	if ( ++p == pe )
		goto _test_eof710;
case 710:
#line 4901 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st118;
		case 38: goto st120;
	}
	goto tr899;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	if ( (*p) == -128 )
		goto st119;
	goto tr114;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	if ( (*p) == -103 )
		goto tr97;
	goto tr114;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	switch( (*p) ) {
		case 35: goto st121;
		case 114: goto st130;
	}
	goto tr114;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	switch( (*p) ) {
		case 56: goto st122;
		case 88: goto st126;
		case 120: goto st126;
	}
	goto tr114;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
	if ( (*p) == 50 )
		goto st123;
	goto tr114;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	if ( (*p) == 49 )
		goto st124;
	goto tr114;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	if ( (*p) == 55 )
		goto st125;
	goto tr114;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	if ( (*p) == 59 )
		goto tr97;
	goto tr5;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	if ( (*p) == 50 )
		goto st127;
	goto tr114;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	if ( (*p) == 48 )
		goto st128;
	goto tr114;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	if ( (*p) == 49 )
		goto st129;
	goto tr114;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	if ( (*p) == 57 )
		goto st125;
	goto tr114;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
	if ( (*p) == 115 )
		goto st131;
	goto tr114;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	if ( (*p) == 113 )
		goto st132;
	goto tr5;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	if ( (*p) == 117 )
		goto st133;
	goto tr5;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
	if ( (*p) == 111 )
		goto st125;
	goto tr5;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
	if ( (*p) == -84 )
		goto tr2;
	if ( (*p) < -93 ) {
		if ( (*p) <= -94 )
			goto tr0;
	} else if ( (*p) > -92 ) {
		if ( -91 <= (*p) && (*p) <= -65 )
			goto tr0;
	} else
		goto tr2;
	goto st0;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	switch( (*p) ) {
		case -110: goto tr91;
		case -109: goto tr3;
	}
	if ( (*p) <= -65 )
		goto tr0;
	goto st0;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	switch( (*p) ) {
		case -101: goto tr129;
		case -100: goto tr130;
		case -99: goto tr131;
		case -98: goto tr132;
	}
	if ( (*p) <= -65 )
		goto tr0;
	goto st0;
tr696:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "rules/main.rl"
	{ s.act = 12;}
	goto st711;
st711:
	if ( ++p == pe )
		goto _test_eof711;
case 711:
#line 5073 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st17;
		case 10: goto tr696;
		case 13: goto tr902;
		case 32: goto tr1;
		case 38: goto st18;
	}
	if ( 9 <= (*p) && (*p) <= 12 )
		goto tr1;
	goto tr766;
tr902:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "rules/main.rl"
	{ s.act = 12;}
	goto st712;
st712:
	if ( ++p == pe )
		goto _test_eof712;
case 712:
#line 5094 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st17;
		case 10: goto tr696;
		case 32: goto tr1;
		case 38: goto st18;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr1;
	goto tr766;
tr697:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "rules/main.rl"
	{ s.act = 12;}
	goto st713;
st713:
	if ( ++p == pe )
		goto _test_eof713;
case 713:
#line 5114 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st17;
		case 10: goto tr696;
		case 32: goto tr1;
		case 38: goto st18;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr1;
	goto tr766;
st714:
	if ( ++p == pe )
		goto _test_eof714;
case 714:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr84;
	goto tr903;
tr700:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st715;
st715:
	if ( ++p == pe )
		goto _test_eof715;
case 715:
#line 5139 "tokenizer.cc"
	if ( 65 <= (*p) && (*p) <= 90 )
		goto st137;
	goto tr903;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	if ( (*p) == 43 )
		goto tr2;
	goto tr133;
tr701:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "rules/main.rl"
	{ s.act = 35;}
	goto st716;
st716:
	if ( ++p == pe )
		goto _test_eof716;
case 716:
#line 5160 "tokenizer.cc"
	switch( (*p) ) {
		case 35: goto st138;
		case 97: goto st166;
		case 104: goto st230;
		case 108: goto st235;
		case 110: goto st19;
		case 113: goto st239;
		case 114: goto st242;
	}
	goto tr903;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	switch( (*p) ) {
		case 49: goto st139;
		case 51: goto st145;
		case 56: goto st148;
		case 88: goto st155;
		case 120: goto st161;
	}
	goto tr133;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	if ( (*p) == 48 )
		goto st140;
	goto tr133;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
	if ( (*p) == 48 )
		goto st141;
	goto tr133;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	if ( (*p) == 55 )
		goto st142;
	goto tr133;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	switch( (*p) ) {
		case 53: goto st143;
		case 54: goto st144;
		case 55: goto st109;
		case 56: goto st125;
	}
	goto tr133;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
	if ( (*p) == 59 )
		goto tr95;
	goto tr133;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	if ( (*p) == 59 )
		goto tr144;
	goto tr133;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
	switch( (*p) ) {
		case 52: goto st146;
		case 57: goto st147;
	}
	goto tr133;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	if ( (*p) == 59 )
		goto tr147;
	goto tr133;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	if ( (*p) == 59 )
		goto tr148;
	goto tr133;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	if ( (*p) == 50 )
		goto st149;
	goto tr133;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
	switch( (*p) ) {
		case 49: goto st150;
		case 50: goto st153;
		case 51: goto st154;
	}
	goto tr133;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
	switch( (*p) ) {
		case 54: goto st151;
		case 55: goto st152;
		case 57: goto st143;
	}
	goto tr133;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
	if ( (*p) == 59 )
		goto tr93;
	goto tr133;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	if ( (*p) == 59 )
		goto tr94;
	goto tr133;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	switch( (*p) ) {
		case 48: goto st109;
		case 49: goto st125;
		case 51: goto st109;
	}
	goto tr133;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
	if ( (*p) == 48 )
		goto tr155;
	goto tr133;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	if ( (*p) == 50 )
		goto st156;
	goto tr133;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	switch( (*p) ) {
		case 48: goto st157;
		case 55: goto st159;
	}
	goto tr133;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	if ( (*p) == 49 )
		goto st158;
	goto tr133;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
	switch( (*p) ) {
		case 56: goto st151;
		case 57: goto st152;
		case 66: goto st143;
		case 67: goto st109;
		case 68: goto st125;
		case 70: goto tr96;
		case 98: goto st143;
		case 99: goto st109;
		case 100: goto st125;
		case 102: goto tr96;
	}
	goto tr133;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
	if ( (*p) == 53 )
		goto st160;
	goto tr133;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	switch( (*p) ) {
		case 66: goto st143;
		case 67: goto st144;
		case 68: goto st109;
		case 69: goto st125;
		case 98: goto st143;
		case 99: goto st144;
		case 100: goto st109;
		case 101: goto st125;
	}
	goto tr133;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	if ( (*p) == 50 )
		goto st162;
	goto tr133;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	switch( (*p) ) {
		case 48: goto st163;
		case 55: goto st159;
	}
	goto tr133;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
	switch( (*p) ) {
		case 49: goto st158;
		case 50: goto st164;
	}
	goto tr133;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
	if ( (*p) == 54 )
		goto st165;
	goto tr133;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
	if ( (*p) == 59 )
		goto tr155;
	goto tr133;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
	switch( (*p) ) {
		case 109: goto st167;
		case 112: goto st228;
	}
	goto tr133;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
	if ( (*p) == 112 )
		goto st168;
	goto tr5;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
	if ( (*p) == 59 )
		goto tr168;
	goto tr5;
tr168:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st717;
tr248:
#line 1 "NONE"
	{ s.te = p+1;}
#line 10 "rules/contractions.rl"
	{n2="'D";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
	goto st717;
tr250:
#line 1 "NONE"
	{ s.te = p+1;}
#line 10 "rules/contractions.rl"
	{n2="'M";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
	goto st717;
tr252:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 49 "rules/main.rl"
	{n2="'s";}
#line 49 "rules/main.rl"
	{ s.act = 27;}
	goto st717;
tr254:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "rules/contractions.rl"
	{n2="'d";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
	goto st717;
tr256:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "rules/contractions.rl"
	{n2="'m";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
	goto st717;
tr621:
#line 1 "NONE"
	{ s.te = p+1;}
#line 22 "rules/contractions.rl"
	{n2="N'T";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 42 "rules/main.rl"
	{ s.act = 22;}
	goto st717;
tr657:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 33;}
	goto st717;
tr653:
#line 1 "NONE"
	{ s.te = p+1;}
#line 21 "rules/contractions.rl"
	{n2="n't";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 42 "rules/main.rl"
	{ s.act = 22;}
	goto st717;
tr680:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "rules/contractions.rl"
	{n2="'d";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
#line 6 "rules/contractions.rl"
	{n1="continued";}
	goto st717;
tr682:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "rules/contractions.rl"
	{n1="government";}
#line 7 "rules/main.rl"
	{ s.act = 33;}
	goto st717;
tr688:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "rules/contractions.rl"
	{n2="'d";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
#line 6 "rules/contractions.rl"
	{n1="required";}
	goto st717;
st717:
	if ( ++p == pe )
		goto _test_eof717;
case 717:
#line 5552 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr5;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
	switch( (*p) ) {
		case -128: goto st170;
		case -126: goto st7;
		case -120: goto st8;
	}
	if ( -127 <= (*p) && (*p) <= -65 )
		goto st3;
	goto tr5;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
	switch( (*p) ) {
		case -103: goto tr170;
		case -102: goto tr0;
		case -98: goto tr0;
	}
	if ( (*p) < -106 ) {
		if ( (*p) <= -119 )
			goto tr0;
	} else if ( (*p) > -105 ) {
		if ( -96 <= (*p) && (*p) <= -65 )
			goto tr0;
	} else
		goto tr0;
	goto tr5;
tr214:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st171;
tr170:
#line 8 "actions.rl"
	{ s.suffix += 3; }
	goto st171;
tr195:
#line 10 "actions.rl"
	{ s.suffix += 5; }
	goto st171;
tr199:
#line 12 "actions.rl"
	{ s.suffix += 7; }
	goto st171;
tr204:
#line 13 "actions.rl"
	{ s.suffix += 8; }
	goto st171;
tr208:
#line 11 "actions.rl"
	{ s.suffix += 6; }
	goto st171;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
#line 5637 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr171;
		case 76: goto st172;
		case 77: goto tr173;
		case 82: goto st173;
		case 83: goto tr175;
		case 86: goto st174;
		case 100: goto tr177;
		case 108: goto st175;
		case 109: goto tr179;
		case 114: goto st176;
		case 115: goto tr175;
		case 118: goto st177;
	}
	goto tr5;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
	if ( (*p) == 76 )
		goto tr182;
	goto tr5;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
	if ( (*p) == 69 )
		goto tr183;
	goto tr5;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
	if ( (*p) == 69 )
		goto tr184;
	goto tr5;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	if ( (*p) == 108 )
		goto tr185;
	goto tr5;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
	if ( (*p) == 101 )
		goto tr186;
	goto tr5;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
	if ( (*p) == 101 )
		goto tr187;
	goto tr5;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
	switch( (*p) ) {
		case 35: goto st179;
		case 97: goto st191;
		case 114: goto st195;
	}
	goto tr5;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
	switch( (*p) ) {
		case 51: goto st180;
		case 56: goto st182;
		case 88: goto st186;
		case 120: goto st186;
	}
	goto tr5;
st180:
	if ( ++p == pe )
		goto _test_eof180;
case 180:
	if ( (*p) == 57 )
		goto st181;
	goto tr5;
st181:
	if ( ++p == pe )
		goto _test_eof181;
case 181:
	if ( (*p) == 59 )
		goto tr195;
	goto tr5;
st182:
	if ( ++p == pe )
		goto _test_eof182;
case 182:
	if ( (*p) == 50 )
		goto st183;
	goto tr5;
st183:
	if ( ++p == pe )
		goto _test_eof183;
case 183:
	if ( (*p) == 49 )
		goto st184;
	goto tr5;
st184:
	if ( ++p == pe )
		goto _test_eof184;
case 184:
	if ( (*p) == 55 )
		goto st185;
	goto tr5;
st185:
	if ( ++p == pe )
		goto _test_eof185;
case 185:
	if ( (*p) == 59 )
		goto tr199;
	goto tr5;
st186:
	if ( ++p == pe )
		goto _test_eof186;
case 186:
	if ( (*p) == 50 )
		goto st187;
	goto tr5;
st187:
	if ( ++p == pe )
		goto _test_eof187;
case 187:
	if ( (*p) == 48 )
		goto st188;
	goto tr5;
st188:
	if ( ++p == pe )
		goto _test_eof188;
case 188:
	if ( (*p) == 49 )
		goto st189;
	goto tr5;
st189:
	if ( ++p == pe )
		goto _test_eof189;
case 189:
	if ( (*p) == 57 )
		goto st190;
	goto tr5;
st190:
	if ( ++p == pe )
		goto _test_eof190;
case 190:
	if ( (*p) == 59 )
		goto tr204;
	goto tr5;
st191:
	if ( ++p == pe )
		goto _test_eof191;
case 191:
	switch( (*p) ) {
		case 109: goto st167;
		case 112: goto st192;
	}
	goto tr5;
st192:
	if ( ++p == pe )
		goto _test_eof192;
case 192:
	if ( (*p) == 111 )
		goto st193;
	goto tr5;
st193:
	if ( ++p == pe )
		goto _test_eof193;
case 193:
	if ( (*p) == 115 )
		goto st194;
	goto tr5;
st194:
	if ( ++p == pe )
		goto _test_eof194;
case 194:
	if ( (*p) == 59 )
		goto tr208;
	goto tr5;
st195:
	if ( ++p == pe )
		goto _test_eof195;
case 195:
	if ( (*p) == 115 )
		goto st196;
	goto tr5;
st196:
	if ( ++p == pe )
		goto _test_eof196;
case 196:
	if ( (*p) == 113 )
		goto st197;
	goto tr5;
st197:
	if ( ++p == pe )
		goto _test_eof197;
case 197:
	if ( (*p) == 117 )
		goto st198;
	goto tr5;
st198:
	if ( ++p == pe )
		goto _test_eof198;
case 198:
	if ( (*p) == 111 )
		goto st185;
	goto tr5;
tr911:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st718;
st718:
	if ( ++p == pe )
		goto _test_eof718;
case 718:
#line 5861 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto st199;
		case 46: goto st200;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
st199:
	if ( ++p == pe )
		goto _test_eof199;
case 199:
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto st199;
		case 46: goto st200;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr14;
st200:
	if ( ++p == pe )
		goto _test_eof200;
case 200:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
	}
	goto tr5;
st201:
	if ( ++p == pe )
		goto _test_eof201;
case 201:
	if ( (*p) == -128 )
		goto st202;
	goto tr5;
st202:
	if ( ++p == pe )
		goto _test_eof202;
case 202:
	if ( (*p) == -103 )
		goto tr223;
	goto tr5;
tr221:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st203;
tr223:
#line 8 "actions.rl"
	{ s.suffix += 3; }
	goto st203;
tr231:
#line 10 "actions.rl"
	{ s.suffix += 5; }
	goto st203;
tr235:
#line 12 "actions.rl"
	{ s.suffix += 7; }
	goto st203;
tr240:
#line 13 "actions.rl"
	{ s.suffix += 8; }
	goto st203;
tr244:
#line 11 "actions.rl"
	{ s.suffix += 6; }
	goto st203;
st203:
	if ( ++p == pe )
		goto _test_eof203;
case 203:
#line 5970 "tokenizer.cc"
	switch( (*p) ) {
		case 83: goto tr175;
		case 115: goto tr175;
	}
	goto tr5;
st204:
	if ( ++p == pe )
		goto _test_eof204;
case 204:
	switch( (*p) ) {
		case 35: goto st205;
		case 97: goto st217;
		case 114: goto st221;
	}
	goto tr5;
st205:
	if ( ++p == pe )
		goto _test_eof205;
case 205:
	switch( (*p) ) {
		case 51: goto st206;
		case 56: goto st208;
		case 88: goto st212;
		case 120: goto st212;
	}
	goto tr5;
st206:
	if ( ++p == pe )
		goto _test_eof206;
case 206:
	if ( (*p) == 57 )
		goto st207;
	goto tr5;
st207:
	if ( ++p == pe )
		goto _test_eof207;
case 207:
	if ( (*p) == 59 )
		goto tr231;
	goto tr5;
st208:
	if ( ++p == pe )
		goto _test_eof208;
case 208:
	if ( (*p) == 50 )
		goto st209;
	goto tr5;
st209:
	if ( ++p == pe )
		goto _test_eof209;
case 209:
	if ( (*p) == 49 )
		goto st210;
	goto tr5;
st210:
	if ( ++p == pe )
		goto _test_eof210;
case 210:
	if ( (*p) == 55 )
		goto st211;
	goto tr5;
st211:
	if ( ++p == pe )
		goto _test_eof211;
case 211:
	if ( (*p) == 59 )
		goto tr235;
	goto tr5;
st212:
	if ( ++p == pe )
		goto _test_eof212;
case 212:
	if ( (*p) == 50 )
		goto st213;
	goto tr5;
st213:
	if ( ++p == pe )
		goto _test_eof213;
case 213:
	if ( (*p) == 48 )
		goto st214;
	goto tr5;
st214:
	if ( ++p == pe )
		goto _test_eof214;
case 214:
	if ( (*p) == 49 )
		goto st215;
	goto tr5;
st215:
	if ( ++p == pe )
		goto _test_eof215;
case 215:
	if ( (*p) == 57 )
		goto st216;
	goto tr5;
st216:
	if ( ++p == pe )
		goto _test_eof216;
case 216:
	if ( (*p) == 59 )
		goto tr240;
	goto tr5;
st217:
	if ( ++p == pe )
		goto _test_eof217;
case 217:
	if ( (*p) == 112 )
		goto st218;
	goto tr5;
st218:
	if ( ++p == pe )
		goto _test_eof218;
case 218:
	if ( (*p) == 111 )
		goto st219;
	goto tr5;
st219:
	if ( ++p == pe )
		goto _test_eof219;
case 219:
	if ( (*p) == 115 )
		goto st220;
	goto tr5;
st220:
	if ( ++p == pe )
		goto _test_eof220;
case 220:
	if ( (*p) == 59 )
		goto tr244;
	goto tr5;
st221:
	if ( ++p == pe )
		goto _test_eof221;
case 221:
	if ( (*p) == 115 )
		goto st222;
	goto tr5;
st222:
	if ( ++p == pe )
		goto _test_eof222;
case 222:
	if ( (*p) == 113 )
		goto st223;
	goto tr5;
st223:
	if ( ++p == pe )
		goto _test_eof223;
case 223:
	if ( (*p) == 117 )
		goto st224;
	goto tr5;
st224:
	if ( ++p == pe )
		goto _test_eof224;
case 224:
	if ( (*p) == 111 )
		goto st211;
	goto tr5;
tr217:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st719;
tr915:
#line 1 "NONE"
	{ s.te = p+1;}
#line 13 "rules/contractions.rl"
	{n2="'LL";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st719;
tr916:
#line 1 "NONE"
	{ s.te = p+1;}
#line 13 "rules/contractions.rl"
	{n2="'RE";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st719;
tr917:
#line 1 "NONE"
	{ s.te = p+1;}
#line 13 "rules/contractions.rl"
	{n2="'VE";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st719;
st719:
	if ( ++p == pe )
		goto _test_eof719;
case 719:
#line 6170 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr5;
st225:
	if ( ++p == pe )
		goto _test_eof225;
case 225:
	switch( (*p) ) {
		case 35: goto st179;
		case 97: goto st191;
		case 114: goto st195;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr168;
	goto tr5;
tr913:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st226;
st226:
	if ( ++p == pe )
		goto _test_eof226;
case 226:
#line 6217 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr5;
tr249:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st720;
st720:
	if ( ++p == pe )
		goto _test_eof720;
case 720:
#line 6248 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 76: goto tr915;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr218:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st721;
tr918:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "rules/contractions.rl"
	{n2="'ll";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st721;
tr919:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "rules/contractions.rl"
	{n2="'re";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st721;
tr920:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "rules/contractions.rl"
	{n2="'ve";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st721;
tr1280:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 33;}
	goto st721;
tr1637:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "rules/contractions.rl"
	{n1="madame";}
#line 7 "rules/main.rl"
	{ s.act = 33;}
	goto st721;
tr1644:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "rules/contractions.rl"
	{n1="over";}
#line 7 "rules/main.rl"
	{ s.act = 33;}
	goto st721;
tr1691:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "rules/contractions.rl"
	{n1="weekend";}
#line 7 "rules/main.rl"
	{ s.act = 33;}
	goto st721;
st721:
	if ( ++p == pe )
		goto _test_eof721;
case 721:
#line 6346 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr5;
st227:
	if ( ++p == pe )
		goto _test_eof227;
case 227:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr5;
tr251:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st722;
st722:
	if ( ++p == pe )
		goto _test_eof722;
case 722:
#line 6398 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 69: goto tr916;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr253:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st723;
st723:
	if ( ++p == pe )
		goto _test_eof723;
case 723:
#line 6436 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 69: goto tr917;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr255:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st724;
tr625:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 33;}
	goto st724;
tr683:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "rules/contractions.rl"
	{n1="international";}
#line 7 "rules/main.rl"
	{ s.act = 33;}
	goto st724;
tr685:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "rules/contractions.rl"
	{n1="national";}
#line 7 "rules/main.rl"
	{ s.act = 33;}
	goto st724;
st724:
	if ( ++p == pe )
		goto _test_eof724;
case 724:
#line 6496 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 108: goto tr918;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr5;
tr257:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st725;
st725:
	if ( ++p == pe )
		goto _test_eof725;
case 725:
#line 6534 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 101: goto tr919;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr258:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st726;
st726:
	if ( ++p == pe )
		goto _test_eof726;
case 726:
#line 6572 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 101: goto tr920;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
st228:
	if ( ++p == pe )
		goto _test_eof228;
case 228:
	if ( (*p) == 111 )
		goto st229;
	goto tr133;
st229:
	if ( ++p == pe )
		goto _test_eof229;
case 229:
	if ( (*p) == 115 )
		goto st147;
	goto tr133;
st230:
	if ( ++p == pe )
		goto _test_eof230;
case 230:
	if ( (*p) == 101 )
		goto st231;
	goto tr133;
st231:
	if ( ++p == pe )
		goto _test_eof231;
case 231:
	if ( (*p) == 108 )
		goto st232;
	goto tr133;
st232:
	if ( ++p == pe )
		goto _test_eof232;
case 232:
	if ( (*p) == 108 )
		goto st233;
	goto tr133;
st233:
	if ( ++p == pe )
		goto _test_eof233;
case 233:
	if ( (*p) == 105 )
		goto st234;
	goto tr133;
st234:
	if ( ++p == pe )
		goto _test_eof234;
case 234:
	if ( (*p) == 112 )
		goto st165;
	goto tr133;
st235:
	if ( ++p == pe )
		goto _test_eof235;
case 235:
	switch( (*p) ) {
		case 100: goto st115;
		case 115: goto st236;
	}
	goto tr133;
st236:
	if ( ++p == pe )
		goto _test_eof236;
case 236:
	if ( (*p) == 113 )
		goto st237;
	goto tr133;
st237:
	if ( ++p == pe )
		goto _test_eof237;
case 237:
	if ( (*p) == 117 )
		goto st238;
	goto tr133;
st238:
	if ( ++p == pe )
		goto _test_eof238;
case 238:
	if ( (*p) == 111 )
		goto st151;
	goto tr133;
st239:
	if ( ++p == pe )
		goto _test_eof239;
case 239:
	if ( (*p) == 117 )
		goto st240;
	goto tr133;
st240:
	if ( ++p == pe )
		goto _test_eof240;
case 240:
	if ( (*p) == 111 )
		goto st241;
	goto tr133;
st241:
	if ( ++p == pe )
		goto _test_eof241;
case 241:
	if ( (*p) == 116 )
		goto st146;
	goto tr133;
st242:
	if ( ++p == pe )
		goto _test_eof242;
case 242:
	switch( (*p) ) {
		case 100: goto st131;
		case 115: goto st243;
	}
	goto tr133;
st243:
	if ( ++p == pe )
		goto _test_eof243;
case 243:
	if ( (*p) == 113 )
		goto st244;
	goto tr133;
st244:
	if ( ++p == pe )
		goto _test_eof244;
case 244:
	if ( (*p) == 117 )
		goto st245;
	goto tr133;
st245:
	if ( ++p == pe )
		goto _test_eof245;
case 245:
	if ( (*p) == 111 )
		goto st152;
	goto tr133;
tr702:
#line 1 "NONE"
	{ s.te = p+1;}
#line 11 "rules/main.rl"
	{ s.act = 1;}
	goto st727;
st727:
	if ( ++p == pe )
		goto _test_eof727;
case 727:
#line 6740 "tokenizer.cc"
	switch( (*p) ) {
		case 39: goto tr147;
		case 116: goto st247;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st246;
	goto tr921;
st246:
	if ( ++p == pe )
		goto _test_eof246;
case 246:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr272;
	goto tr5;
tr272:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st728;
tr459:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 49 "rules/main.rl"
	{n2="'s";}
#line 49 "rules/main.rl"
	{ s.act = 27;}
	goto st728;
st728:
	if ( ++p == pe )
		goto _test_eof728;
case 728:
#line 6775 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 65: goto st35;
		case 71: goto st38;
		case 76: goto tr59;
		case 77: goto tr777;
		case 80: goto st40;
		case 97: goto st43;
		case 98: goto st45;
		case 99: goto tr781;
		case 100: goto st55;
		case 102: goto st57;
		case 103: goto tr784;
		case 104: goto st64;
		case 105: goto st65;
		case 107: goto tr787;
		case 108: goto st69;
		case 109: goto tr789;
		case 110: goto st80;
		case 111: goto st82;
		case 112: goto st83;
		case 113: goto st51;
		case 114: goto st85;
		case 115: goto st87;
		case 116: goto st88;
		case 121: goto st90;
	}
	goto tr5;
st247:
	if ( ++p == pe )
		goto _test_eof247;
case 247:
	if ( (*p) == 105 )
		goto st248;
	goto tr273;
st248:
	if ( ++p == pe )
		goto _test_eof248;
case 248:
	if ( (*p) == 115 )
		goto tr275;
	goto tr273;
tr924:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st729;
tr703:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "rules/main.rl"
	{ s.act = 35;}
	goto st729;
st729:
	if ( ++p == pe )
		goto _test_eof729;
case 729:
#line 6835 "tokenizer.cc"
	if ( (*p) == 42 )
		goto tr924;
	goto tr5;
tr704:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "rules/main.rl"
	{ s.act = 35;}
	goto st730;
st730:
	if ( ++p == pe )
		goto _test_eof730;
case 730:
#line 6849 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st301;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr926;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr903;
st249:
	if ( ++p == pe )
		goto _test_eof249;
case 249:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr5;
st250:
	if ( ++p == pe )
		goto _test_eof250;
case 250:
	if ( (*p) == 50 )
		goto st267;
	if ( (*p) < 51 ) {
		if ( 48 <= (*p) && (*p) <= 49 )
			goto st251;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st269;
		} else if ( (*p) >= 65 )
			goto st269;
	} else
		goto st265;
	goto tr5;
st251:
	if ( ++p == pe )
		goto _test_eof251;
case 251:
	if ( (*p) == 46 )
		goto st252;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st265;
	goto tr5;
st252:
	if ( ++p == pe )
		goto _test_eof252;
case 252:
	if ( (*p) == 50 )
		goto st263;
	if ( (*p) > 49 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st261;
	} else if ( (*p) >= 48 )
		goto st253;
	goto tr5;
st253:
	if ( ++p == pe )
		goto _test_eof253;
case 253:
	if ( (*p) == 46 )
		goto st254;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st261;
	goto tr5;
st254:
	if ( ++p == pe )
		goto _test_eof254;
case 254:
	if ( (*p) == 50 )
		goto st259;
	if ( (*p) > 49 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st257;
	} else if ( (*p) >= 48 )
		goto st255;
	goto tr5;
st255:
	if ( ++p == pe )
		goto _test_eof255;
case 255:
	if ( (*p) == 46 )
		goto st256;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st257;
	goto tr5;
st256:
	if ( ++p == pe )
		goto _test_eof256;
case 256:
	if ( (*p) == 50 )
		goto st733;
	if ( (*p) > 49 ) {
		if ( 51 <= (*p) && (*p) <= 57 )
			goto st732;
	} else if ( (*p) >= 48 )
		goto st731;
	goto tr5;
st731:
	if ( ++p == pe )
		goto _test_eof731;
case 731:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st732;
	goto tr927;
st732:
	if ( ++p == pe )
		goto _test_eof732;
case 732:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr928;
	goto tr927;
st733:
	if ( ++p == pe )
		goto _test_eof733;
case 733:
	if ( (*p) == 53 )
		goto st734;
	if ( (*p) > 52 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto tr928;
	} else if ( (*p) >= 48 )
		goto st732;
	goto tr927;
st734:
	if ( ++p == pe )
		goto _test_eof734;
case 734:
	if ( 48 <= (*p) && (*p) <= 53 )
		goto tr928;
	goto tr927;
st257:
	if ( ++p == pe )
		goto _test_eof257;
case 257:
	if ( (*p) == 46 )
		goto st256;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st258;
	goto tr5;
st258:
	if ( ++p == pe )
		goto _test_eof258;
case 258:
	if ( (*p) == 46 )
		goto st256;
	goto tr5;
st259:
	if ( ++p == pe )
		goto _test_eof259;
case 259:
	switch( (*p) ) {
		case 46: goto st256;
		case 53: goto st260;
	}
	if ( (*p) > 52 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto st258;
	} else if ( (*p) >= 48 )
		goto st257;
	goto tr5;
st260:
	if ( ++p == pe )
		goto _test_eof260;
case 260:
	if ( (*p) == 46 )
		goto st256;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st258;
	goto tr5;
st261:
	if ( ++p == pe )
		goto _test_eof261;
case 261:
	if ( (*p) == 46 )
		goto st254;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st262;
	goto tr5;
st262:
	if ( ++p == pe )
		goto _test_eof262;
case 262:
	if ( (*p) == 46 )
		goto st254;
	goto tr5;
st263:
	if ( ++p == pe )
		goto _test_eof263;
case 263:
	switch( (*p) ) {
		case 46: goto st254;
		case 53: goto st264;
	}
	if ( (*p) > 52 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto st262;
	} else if ( (*p) >= 48 )
		goto st261;
	goto tr5;
st264:
	if ( ++p == pe )
		goto _test_eof264;
case 264:
	if ( (*p) == 46 )
		goto st254;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st262;
	goto tr5;
st265:
	if ( ++p == pe )
		goto _test_eof265;
case 265:
	if ( (*p) == 46 )
		goto st252;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st266;
	goto tr5;
st266:
	if ( ++p == pe )
		goto _test_eof266;
case 266:
	if ( (*p) == 46 )
		goto st252;
	goto tr5;
st267:
	if ( ++p == pe )
		goto _test_eof267;
case 267:
	switch( (*p) ) {
		case 46: goto st252;
		case 53: goto st268;
	}
	if ( (*p) > 52 ) {
		if ( 54 <= (*p) && (*p) <= 57 )
			goto st266;
	} else if ( (*p) >= 48 )
		goto st265;
	goto tr5;
st268:
	if ( ++p == pe )
		goto _test_eof268;
case 268:
	if ( (*p) == 46 )
		goto st252;
	if ( 48 <= (*p) && (*p) <= 53 )
		goto st266;
	goto tr5;
st269:
	if ( ++p == pe )
		goto _test_eof269;
case 269:
	switch( (*p) ) {
		case 45: goto st270;
		case 46: goto st271;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st269;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st269;
	} else
		goto st269;
	goto tr5;
st270:
	if ( ++p == pe )
		goto _test_eof270;
case 270:
	if ( (*p) == 45 )
		goto st270;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st269;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st269;
	} else
		goto st269;
	goto tr5;
st271:
	if ( ++p == pe )
		goto _test_eof271;
case 271:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else if ( (*p) >= 65 )
		goto st272;
	goto tr5;
st272:
	if ( ++p == pe )
		goto _test_eof272;
case 272:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr5;
st273:
	if ( ++p == pe )
		goto _test_eof273;
case 273:
	if ( (*p) == 45 )
		goto st273;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr5;
st274:
	if ( ++p == pe )
		goto _test_eof274;
case 274:
	switch( (*p) ) {
		case 65: goto st275;
		case 66: goto st278;
		case 67: goto st279;
		case 69: goto st282;
		case 71: goto st283;
		case 73: goto st284;
		case 74: goto st285;
		case 77: goto st287;
		case 78: goto st292;
		case 79: goto st294;
		case 80: goto st295;
		case 84: goto st296;
		case 88: goto st300;
		case 97: goto st275;
		case 98: goto st278;
		case 99: goto st279;
		case 101: goto st282;
		case 103: goto st283;
		case 105: goto st284;
		case 106: goto st285;
		case 109: goto st287;
		case 110: goto st292;
		case 111: goto st294;
		case 112: goto st295;
		case 116: goto st296;
		case 120: goto st300;
	}
	if ( (*p) > 90 ) {
		if ( 100 <= (*p) && (*p) <= 122 )
			goto st281;
	} else if ( (*p) >= 68 )
		goto st281;
	goto tr5;
st275:
	if ( ++p == pe )
		goto _test_eof275;
case 275:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 69: goto tr320;
		case 82: goto tr321;
		case 83: goto tr322;
		case 101: goto tr320;
		case 114: goto tr321;
		case 115: goto tr322;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr319:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st735;
st735:
	if ( ++p == pe )
		goto _test_eof735;
case 735:
#line 7263 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
tr320:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st736;
st736:
	if ( ++p == pe )
		goto _test_eof736;
case 736:
#line 7287 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 82: goto st276;
		case 114: goto st276;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st276:
	if ( ++p == pe )
		goto _test_eof276;
case 276:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 79: goto tr319;
		case 111: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
tr321:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st737;
st737:
	if ( ++p == pe )
		goto _test_eof737;
case 737:
#line 7332 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 80: goto st277;
		case 112: goto st277;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st277:
	if ( ++p == pe )
		goto _test_eof277;
case 277:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 65: goto tr319;
		case 97: goto tr319;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
tr322:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st738;
st738:
	if ( ++p == pe )
		goto _test_eof738;
case 738:
#line 7377 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 73: goto st277;
		case 105: goto st277;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st278:
	if ( ++p == pe )
		goto _test_eof278;
case 278:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 73: goto tr324;
		case 105: goto tr324;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr324:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st739;
st739:
	if ( ++p == pe )
		goto _test_eof739;
case 739:
#line 7422 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 90: goto tr319;
		case 122: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 89 ) {
		if ( 97 <= (*p) && (*p) <= 121 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st279:
	if ( ++p == pe )
		goto _test_eof279;
case 279:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 65: goto tr325;
		case 79: goto tr326;
		case 97: goto tr325;
		case 111: goto tr326;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr325:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st740;
st740:
	if ( ++p == pe )
		goto _test_eof740;
case 740:
#line 7469 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 84: goto tr319;
		case 116: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
tr326:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st741;
st741:
	if ( ++p == pe )
		goto _test_eof741;
case 741:
#line 7495 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 77: goto tr319;
		case 79: goto st280;
		case 109: goto tr319;
		case 111: goto st280;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st280:
	if ( ++p == pe )
		goto _test_eof280;
case 280:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 80: goto tr319;
		case 112: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
st281:
	if ( ++p == pe )
		goto _test_eof281;
case 281:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
st282:
	if ( ++p == pe )
		goto _test_eof282;
case 282:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 68: goto tr327;
		case 100: goto tr327;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr327:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st742;
st742:
	if ( ++p == pe )
		goto _test_eof742;
case 742:
#line 7578 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 85: goto tr319;
		case 117: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st283:
	if ( ++p == pe )
		goto _test_eof283;
case 283:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 79: goto tr328;
		case 111: goto tr328;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr328:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st743;
st743:
	if ( ++p == pe )
		goto _test_eof743;
case 743:
#line 7623 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 86: goto tr319;
		case 118: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st284:
	if ( ++p == pe )
		goto _test_eof284;
case 284:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 78: goto tr329;
		case 110: goto tr329;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr329:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st744;
st744:
	if ( ++p == pe )
		goto _test_eof744;
case 744:
#line 7668 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 70: goto st276;
		case 84: goto tr319;
		case 102: goto st276;
		case 116: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st285:
	if ( ++p == pe )
		goto _test_eof285;
case 285:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 79: goto tr330;
		case 111: goto tr330;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr330:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st745;
st745:
	if ( ++p == pe )
		goto _test_eof745;
case 745:
#line 7715 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 66: goto st286;
		case 98: goto st286;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st286:
	if ( ++p == pe )
		goto _test_eof286;
case 286:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 83: goto tr319;
		case 115: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
st287:
	if ( ++p == pe )
		goto _test_eof287;
case 287:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 73: goto tr331;
		case 79: goto tr332;
		case 85: goto tr333;
		case 105: goto tr331;
		case 111: goto tr332;
		case 117: goto tr333;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr331:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st746;
st746:
	if ( ++p == pe )
		goto _test_eof746;
case 746:
#line 7783 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 76: goto tr319;
		case 108: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
tr332:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st747;
st747:
	if ( ++p == pe )
		goto _test_eof747;
case 747:
#line 7809 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 66: goto st288;
		case 98: goto st288;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st288:
	if ( ++p == pe )
		goto _test_eof288;
case 288:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 73: goto tr319;
		case 105: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
tr333:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st748;
st748:
	if ( ++p == pe )
		goto _test_eof748;
case 748:
#line 7854 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 83: goto st289;
		case 115: goto st289;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st289:
	if ( ++p == pe )
		goto _test_eof289;
case 289:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 69: goto st290;
		case 101: goto st290;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
st290:
	if ( ++p == pe )
		goto _test_eof290;
case 290:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 85: goto st291;
		case 117: goto st291;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
st291:
	if ( ++p == pe )
		goto _test_eof291;
case 291:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 77: goto tr319;
		case 109: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
st292:
	if ( ++p == pe )
		goto _test_eof292;
case 292:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 65: goto tr336;
		case 69: goto tr325;
		case 97: goto tr336;
		case 101: goto tr325;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr336:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st749;
st749:
	if ( ++p == pe )
		goto _test_eof749;
case 749:
#line 7958 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 77: goto st293;
		case 109: goto st293;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st293:
	if ( ++p == pe )
		goto _test_eof293;
case 293:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 69: goto tr319;
		case 101: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
st294:
	if ( ++p == pe )
		goto _test_eof294;
case 294:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 82: goto tr337;
		case 114: goto tr337;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr337:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st750;
st750:
	if ( ++p == pe )
		goto _test_eof750;
case 750:
#line 8022 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 71: goto tr319;
		case 103: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st295:
	if ( ++p == pe )
		goto _test_eof295;
case 295:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 82: goto tr338;
		case 114: goto tr338;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr338:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st751;
st751:
	if ( ++p == pe )
		goto _test_eof751;
case 751:
#line 8067 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 79: goto tr319;
		case 111: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st296:
	if ( ++p == pe )
		goto _test_eof296;
case 296:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 69: goto tr331;
		case 82: goto tr339;
		case 101: goto tr331;
		case 114: goto tr339;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr339:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st752;
st752:
	if ( ++p == pe )
		goto _test_eof752;
case 752:
#line 8114 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 65: goto st297;
		case 97: goto st297;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st297:
	if ( ++p == pe )
		goto _test_eof297;
case 297:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 86: goto st298;
		case 118: goto st298;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
st298:
	if ( ++p == pe )
		goto _test_eof298;
case 298:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 69: goto st299;
		case 101: goto st299;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
st299:
	if ( ++p == pe )
		goto _test_eof299;
case 299:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 76: goto tr319;
		case 108: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr323;
st300:
	if ( ++p == pe )
		goto _test_eof300;
case 300:
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 88: goto tr342;
		case 120: goto tr342;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr319;
	} else
		goto tr319;
	goto tr5;
tr342:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st753;
st753:
	if ( ++p == pe )
		goto _test_eof753;
case 753:
#line 8216 "tokenizer.cc"
	switch( (*p) ) {
		case 45: goto st273;
		case 46: goto st274;
		case 88: goto tr319;
		case 120: goto tr319;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st272;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st272;
	} else
		goto st272;
	goto tr927;
st301:
	if ( ++p == pe )
		goto _test_eof301;
case 301:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto tr343;
	goto tr5;
tr343:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st754;
st754:
	if ( ++p == pe )
		goto _test_eof754;
case 754:
#line 8263 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st249;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr343;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
st302:
	if ( ++p == pe )
		goto _test_eof302;
case 302:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr344;
		} else if ( (*p) >= 65 )
			goto tr344;
	} else
		goto st249;
	goto tr5;
tr344:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 31;}
	goto st755;
st755:
	if ( ++p == pe )
		goto _test_eof755;
case 755:
#line 8337 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr344;
	} else
		goto tr344;
	goto tr797;
st303:
	if ( ++p == pe )
		goto _test_eof303;
case 303:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st304;
		case 64: goto st250;
		case 77: goto tr346;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st304:
	if ( ++p == pe )
		goto _test_eof304;
case 304:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 77: goto st305;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st305:
	if ( ++p == pe )
		goto _test_eof305;
case 305:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto tr348;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr1088:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st756;
tr346:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/units.rl"
	{n2="am";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st756;
tr348:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "actions.rl"
	{ s.suffix += 4; }
#line 47 "rules/units.rl"
	{n2="am";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st756;
tr350:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st756;
tr352:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 48 "rules/units.rl"
	{n2="pm";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st756;
tr354:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "actions.rl"
	{ s.suffix += 4; }
#line 48 "rules/units.rl"
	{n2="pm";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st756;
tr357:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "actions.rl"
	{ s.suffix += 3; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st756;
tr363:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "actions.rl"
	{ s.suffix += 4; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st756;
tr967:
#line 1 "NONE"
	{ s.te = p+1;}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st756;
tr624:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st756;
tr738:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "rules/main.rl"
	{ s.act = 35;}
	goto st756;
tr941:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st756;
st756:
	if ( ++p == pe )
		goto _test_eof756;
case 756:
#line 8517 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr5;
st306:
	if ( ++p == pe )
		goto _test_eof306;
case 306:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 66: goto tr349;
		case 95: goto st249;
		case 98: goto tr349;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr349:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st757;
st757:
	if ( ++p == pe )
		goto _test_eof757;
case 757:
#line 8570 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto st307;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
st307:
	if ( ++p == pe )
		goto _test_eof307;
case 307:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr350;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
tr942:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st758;
st758:
	if ( ++p == pe )
		goto _test_eof758;
case 758:
#line 8623 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 66: goto tr349;
		case 95: goto st249;
		case 98: goto tr349;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
st308:
	if ( ++p == pe )
		goto _test_eof308;
case 308:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st309;
		case 64: goto st250;
		case 77: goto tr352;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st309:
	if ( ++p == pe )
		goto _test_eof309;
case 309:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 77: goto st310;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st310:
	if ( ++p == pe )
		goto _test_eof310;
case 310:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto tr354;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st311:
	if ( ++p == pe )
		goto _test_eof311;
case 311:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st312;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr346;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st312:
	if ( ++p == pe )
		goto _test_eof312;
case 312:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto st305;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st313:
	if ( ++p == pe )
		goto _test_eof313;
case 313:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 112: goto st314;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st314:
	if ( ++p == pe )
		goto _test_eof314;
case 314:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr357;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr946:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st759;
st759:
	if ( ++p == pe )
		goto _test_eof759;
case 759:
#line 8805 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr964;
		case 117: goto st315;
		case 119: goto st307;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
tr361:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st760;
tr964:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st760;
st760:
	if ( ++p == pe )
		goto _test_eof760;
case 760:
#line 8846 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 94: goto st47;
		case 95: goto st249;
	}
	if ( (*p) < 50 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto st249;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 51 ) {
		if ( (*p) < 65 ) {
			if ( 52 <= (*p) && (*p) <= 57 )
				goto st249;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else
			goto st249;
	} else
		goto tr941;
	goto tr798;
st315:
	if ( ++p == pe )
		goto _test_eof315;
case 315:
	switch( (*p) ) {
		case 32: goto tr52;
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st316;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr5;
st316:
	if ( ++p == pe )
		goto _test_eof316;
case 316:
	switch( (*p) ) {
		case 32: goto tr60;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr5;
st317:
	if ( ++p == pe )
		goto _test_eof317;
case 317:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 66: goto tr350;
		case 95: goto st249;
		case 112: goto st318;
		case 114: goto tr350;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st318:
	if ( ++p == pe )
		goto _test_eof318;
case 318:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr357;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st319:
	if ( ++p == pe )
		goto _test_eof319;
case 319:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto st320;
		case 116: goto tr361;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st320:
	if ( ++p == pe )
		goto _test_eof320;
case 320:
	switch( (*p) ) {
		case 32: goto st59;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr949:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st761;
st761:
	if ( ++p == pe )
		goto _test_eof761;
case 761:
#line 9017 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto st321;
		case 98: goto st322;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 99 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
st321:
	if ( ++p == pe )
		goto _test_eof321;
case 321:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr357;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
st322:
	if ( ++p == pe )
		goto _test_eof322;
case 322:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto st323;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
st323:
	if ( ++p == pe )
		goto _test_eof323;
case 323:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr363;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
st324:
	if ( ++p == pe )
		goto _test_eof324;
case 324:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr364;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr364:
#line 1 "NONE"
	{ s.te = p+1;}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st762;
st762:
	if ( ++p == pe )
		goto _test_eof762;
case 762:
#line 9135 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr967;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
st325:
	if ( ++p == pe )
		goto _test_eof325;
case 325:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr361;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr952:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st763;
st763:
	if ( ++p == pe )
		goto _test_eof763;
case 763:
#line 9188 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 66: goto tr350;
		case 95: goto st249;
		case 98: goto tr968;
		case 103: goto tr350;
		case 109: goto tr969;
		case 112: goto st327;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
tr968:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st764;
st764:
	if ( ++p == pe )
		goto _test_eof764;
case 764:
#line 9223 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto st326;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
st326:
	if ( ++p == pe )
		goto _test_eof326;
case 326:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr357;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
tr969:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st765;
st765:
	if ( ++p == pe )
		goto _test_eof765;
case 765:
#line 9276 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 47: goto st67;
		case 64: goto st250;
		case 94: goto st47;
		case 95: goto st249;
		case 104: goto tr941;
	}
	if ( (*p) < 52 ) {
		if ( (*p) > 49 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr941;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
st327:
	if ( ++p == pe )
		goto _test_eof327;
case 327:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 104: goto tr357;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
st328:
	if ( ++p == pe )
		goto _test_eof328;
case 328:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 98: goto tr365;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr365:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st766;
st766:
	if ( ++p == pe )
		goto _test_eof766;
case 766:
#line 9356 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr941;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
tr954:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st767;
st767:
	if ( ++p == pe )
		goto _test_eof767;
case 767:
#line 9387 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 47: goto st70;
		case 64: goto st250;
		case 76: goto tr941;
		case 94: goto st47;
		case 95: goto st249;
		case 98: goto st329;
		case 103: goto tr941;
		case 105: goto tr972;
		case 109: goto tr964;
		case 110: goto st336;
		case 111: goto tr364;
		case 112: goto st338;
		case 116: goto st337;
	}
	if ( (*p) < 52 ) {
		if ( (*p) > 49 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr941;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
st329:
	if ( ++p == pe )
		goto _test_eof329;
case 329:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto st326;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
tr972:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st768;
st768:
	if ( ++p == pe )
		goto _test_eof768;
case 768:
#line 9453 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 94: goto st47;
		case 95: goto st249;
		case 99: goto st330;
		case 108: goto tr976;
		case 110: goto tr364;
	}
	if ( (*p) < 50 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto st249;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 51 ) {
		if ( (*p) < 65 ) {
			if ( 52 <= (*p) && (*p) <= 57 )
				goto st249;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else
			goto st249;
	} else
		goto tr941;
	goto tr798;
st330:
	if ( ++p == pe )
		goto _test_eof330;
case 330:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto st331;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
st331:
	if ( ++p == pe )
		goto _test_eof331;
case 331:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto st332;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr5;
st332:
	if ( ++p == pe )
		goto _test_eof332;
case 332:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto st333;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr5;
st333:
	if ( ++p == pe )
		goto _test_eof333;
case 333:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto st334;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr5;
st334:
	if ( ++p == pe )
		goto _test_eof334;
case 334:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr364;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr5;
tr976:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st769;
st769:
	if ( ++p == pe )
		goto _test_eof769;
case 769:
#line 9603 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto st335;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
st335:
	if ( ++p == pe )
		goto _test_eof335;
case 335:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto st332;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
st336:
	if ( ++p == pe )
		goto _test_eof336;
case 336:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto st337;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
st337:
	if ( ++p == pe )
		goto _test_eof337;
case 337:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 104: goto tr364;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
st338:
	if ( ++p == pe )
		goto _test_eof338;
case 338:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 104: goto tr350;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr44;
st339:
	if ( ++p == pe )
		goto _test_eof339;
case 339:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto st340;
		case 111: goto tr350;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st340:
	if ( ++p == pe )
		goto _test_eof340;
case 340:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto st331;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st341:
	if ( ++p == pe )
		goto _test_eof341;
case 341:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 122: goto tr350;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st342:
	if ( ++p == pe )
		goto _test_eof342;
case 342:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st343;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr352;
		case 116: goto tr350;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st343:
	if ( ++p == pe )
		goto _test_eof343;
case 343:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto st310;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st344:
	if ( ++p == pe )
		goto _test_eof344;
case 344:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr350;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st345:
	if ( ++p == pe )
		goto _test_eof345;
case 345:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 112: goto st346;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st346:
	if ( ++p == pe )
		goto _test_eof346;
case 346:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr357;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st347:
	if ( ++p == pe )
		goto _test_eof347;
case 347:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto st334;
		case 113: goto st315;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st348:
	if ( ++p == pe )
		goto _test_eof348;
case 348:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto st349;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st349:
	if ( ++p == pe )
		goto _test_eof349;
case 349:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr357;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st350:
	if ( ++p == pe )
		goto _test_eof350;
case 350:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr361;
		case 114: goto tr364;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr926:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st770;
st770:
	if ( ++p == pe )
		goto _test_eof770;
case 770:
#line 9987 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto st351;
		case 46: goto st301;
		case 47: goto st94;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr926;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
st351:
	if ( ++p == pe )
		goto _test_eof351;
case 351:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr344;
		} else if ( (*p) >= 65 )
			goto tr344;
	} else
		goto st352;
	goto tr82;
st352:
	if ( ++p == pe )
		goto _test_eof352;
case 352:
	switch( (*p) ) {
		case 43: goto st249;
		case 47: goto st94;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st352;
	goto tr82;
tr705:
#line 1 "NONE"
	{ s.te = p+1;}
#line 25 "rules/main.rl"
	{ s.act = 11;}
	goto st771;
st771:
	if ( ++p == pe )
		goto _test_eof771;
case 771:
#line 10085 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 32: goto st353;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr981;
		case 46: goto st381;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr983;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr979;
st353:
	if ( ++p == pe )
		goto _test_eof353;
case 353:
	if ( (*p) == 45 )
		goto st354;
	goto tr379;
st354:
	if ( ++p == pe )
		goto _test_eof354;
case 354:
	if ( (*p) == 32 )
		goto st355;
	goto tr379;
st355:
	if ( ++p == pe )
		goto _test_eof355;
case 355:
	if ( (*p) == 45 )
		goto st356;
	goto tr379;
st356:
	if ( ++p == pe )
		goto _test_eof356;
case 356:
	if ( (*p) == 32 )
		goto st357;
	goto tr379;
st357:
	if ( ++p == pe )
		goto _test_eof357;
case 357:
	if ( (*p) == 45 )
		goto tr384;
	goto tr5;
tr384:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st772;
st772:
	if ( ++p == pe )
		goto _test_eof772;
case 772:
#line 10161 "tokenizer.cc"
	if ( (*p) == 32 )
		goto st357;
	goto tr771;
tr981:
#line 1 "NONE"
	{ s.te = p+1;}
#line 25 "rules/main.rl"
	{ s.act = 11;}
	goto st773;
st773:
	if ( ++p == pe )
		goto _test_eof773;
case 773:
#line 10175 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr984;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st377;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st380;
	} else
		goto st378;
	goto tr979;
st358:
	if ( ++p == pe )
		goto _test_eof358;
case 358:
	if ( (*p) == -128 )
		goto st359;
	goto tr5;
st359:
	if ( ++p == pe )
		goto _test_eof359;
case 359:
	if ( (*p) == -103 )
		goto tr170;
	goto tr5;
st360:
	if ( ++p == pe )
		goto _test_eof360;
case 360:
	switch( (*p) ) {
		case 35: goto st179;
		case 97: goto st361;
		case 114: goto st195;
	}
	goto tr5;
st361:
	if ( ++p == pe )
		goto _test_eof361;
case 361:
	switch( (*p) ) {
		case 109: goto st362;
		case 112: goto st192;
	}
	goto tr5;
st362:
	if ( ++p == pe )
		goto _test_eof362;
case 362:
	if ( (*p) == 112 )
		goto st363;
	goto tr5;
st363:
	if ( ++p == pe )
		goto _test_eof363;
case 363:
	if ( (*p) == 59 )
		goto st364;
	goto tr5;
st364:
	if ( ++p == pe )
		goto _test_eof364;
case 364:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 45: goto st364;
		case 46: goto st200;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr5;
st365:
	if ( ++p == pe )
		goto _test_eof365;
case 365:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st366;
		case 39: goto tr395;
		case 45: goto st364;
		case 46: goto st369;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr5;
st366:
	if ( ++p == pe )
		goto _test_eof366;
case 366:
	switch( (*p) ) {
		case 35: goto st179;
		case 97: goto st361;
		case 114: goto st195;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto st364;
	goto tr5;
tr395:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st367;
st367:
	if ( ++p == pe )
		goto _test_eof367;
case 367:
#line 10302 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr397;
		case 76: goto st370;
		case 77: goto tr399;
		case 82: goto st371;
		case 83: goto tr401;
		case 86: goto st372;
		case 100: goto tr403;
		case 108: goto st373;
		case 109: goto tr405;
		case 114: goto st374;
		case 115: goto tr401;
		case 118: goto st375;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st364;
	} else if ( (*p) >= 65 )
		goto st364;
	goto tr5;
tr397:
#line 1 "NONE"
	{ s.te = p+1;}
#line 10 "rules/contractions.rl"
	{n2="'D";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
	goto st774;
tr399:
#line 1 "NONE"
	{ s.te = p+1;}
#line 10 "rules/contractions.rl"
	{n2="'M";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
	goto st774;
tr401:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 49 "rules/main.rl"
	{n2="'s";}
#line 49 "rules/main.rl"
	{ s.act = 27;}
	goto st774;
tr403:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "rules/contractions.rl"
	{n2="'d";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
	goto st774;
tr405:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "rules/contractions.rl"
	{n2="'m";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
	goto st774;
st774:
	if ( ++p == pe )
		goto _test_eof774;
case 774:
#line 10377 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 45: goto st364;
		case 46: goto st200;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr5;
st368:
	if ( ++p == pe )
		goto _test_eof368;
case 368:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr395;
		case 45: goto st364;
		case 46: goto st369;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr5;
st369:
	if ( ++p == pe )
		goto _test_eof369;
case 369:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st364;
	} else if ( (*p) >= 65 )
		goto st364;
	goto tr5;
st370:
	if ( ++p == pe )
		goto _test_eof370;
case 370:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 45: goto st364;
		case 46: goto st200;
		case 76: goto tr408;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr5;
tr408:
#line 1 "NONE"
	{ s.te = p+1;}
#line 13 "rules/contractions.rl"
	{n2="'LL";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st775;
tr409:
#line 1 "NONE"
	{ s.te = p+1;}
#line 13 "rules/contractions.rl"
	{n2="'RE";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st775;
tr410:
#line 1 "NONE"
	{ s.te = p+1;}
#line 13 "rules/contractions.rl"
	{n2="'VE";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st775;
st775:
	if ( ++p == pe )
		goto _test_eof775;
case 775:
#line 10484 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st366;
		case 39: goto tr395;
		case 45: goto st364;
		case 46: goto st369;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr985;
st371:
	if ( ++p == pe )
		goto _test_eof371;
case 371:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 45: goto st364;
		case 46: goto st200;
		case 69: goto tr409;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr5;
st372:
	if ( ++p == pe )
		goto _test_eof372;
case 372:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 45: goto st364;
		case 46: goto st200;
		case 69: goto tr410;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr5;
st373:
	if ( ++p == pe )
		goto _test_eof373;
case 373:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 45: goto st364;
		case 46: goto st200;
		case 108: goto tr411;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr5;
tr411:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "rules/contractions.rl"
	{n2="'ll";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st776;
tr412:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "rules/contractions.rl"
	{n2="'re";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st776;
tr413:
#line 1 "NONE"
	{ s.te = p+1;}
#line 12 "rules/contractions.rl"
	{n2="'ve";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/main.rl"
	{ s.act = 26;}
	goto st776;
st776:
	if ( ++p == pe )
		goto _test_eof776;
case 776:
#line 10598 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr395;
		case 45: goto st364;
		case 46: goto st369;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr985;
st374:
	if ( ++p == pe )
		goto _test_eof374;
case 374:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 45: goto st364;
		case 46: goto st200;
		case 101: goto tr412;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr5;
st375:
	if ( ++p == pe )
		goto _test_eof375;
case 375:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 45: goto st364;
		case 46: goto st200;
		case 101: goto tr413;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st364;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st368;
	} else
		goto st365;
	goto tr5;
tr986:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st777;
tr984:
#line 1 "NONE"
	{ s.te = p+1;}
#line 25 "rules/main.rl"
	{ s.act = 11;}
	goto st777;
st777:
	if ( ++p == pe )
		goto _test_eof777;
case 777:
#line 10673 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr986;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st377;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st380;
	} else
		goto st378;
	goto tr5;
st376:
	if ( ++p == pe )
		goto _test_eof376;
case 376:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr5;
st377:
	if ( ++p == pe )
		goto _test_eof377;
case 377:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st377;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st377;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st380;
	} else
		goto st378;
	goto tr5;
st378:
	if ( ++p == pe )
		goto _test_eof378;
case 378:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st366;
		case 39: goto tr395;
		case 43: goto st249;
		case 45: goto st377;
		case 46: goto st379;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st377;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st380;
	} else
		goto st378;
	goto tr5;
st379:
	if ( ++p == pe )
		goto _test_eof379;
case 379:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st377;
		} else if ( (*p) >= 65 )
			goto st377;
	} else
		goto st249;
	goto tr5;
st380:
	if ( ++p == pe )
		goto _test_eof380;
case 380:
	switch( (*p) ) {
		case -30: goto st358;
		case 38: goto st360;
		case 39: goto tr395;
		case 43: goto st249;
		case 45: goto st377;
		case 46: goto st379;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st377;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st380;
	} else
		goto st378;
	goto tr5;
st381:
	if ( ++p == pe )
		goto _test_eof381;
case 381:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto tr343;
	goto tr5;
tr983:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st778;
st778:
	if ( ++p == pe )
		goto _test_eof778;
case 778:
#line 10844 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr988;
		case 46: goto st381;
		case 47: goto st94;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr983;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
st382:
	if ( ++p == pe )
		goto _test_eof382;
case 382:
	switch( (*p) ) {
		case 35: goto st383;
		case 97: goto st191;
		case 99: goto st31;
		case 114: goto st195;
	}
	goto tr82;
st383:
	if ( ++p == pe )
		goto _test_eof383;
case 383:
	switch( (*p) ) {
		case 49: goto st27;
		case 51: goto st180;
		case 56: goto st182;
		case 88: goto st384;
		case 120: goto st384;
	}
	goto tr82;
st384:
	if ( ++p == pe )
		goto _test_eof384;
case 384:
	switch( (*p) ) {
		case 50: goto st187;
		case 65: goto st28;
		case 97: goto st28;
	}
	goto tr82;
tr988:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st779;
st779:
	if ( ++p == pe )
		goto _test_eof779;
case 779:
#line 10943 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st385;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1013;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1015;
		} else
			goto tr1014;
	} else
		goto st9;
	goto tr763;
st385:
	if ( ++p == pe )
		goto _test_eof385;
case 385:
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st385;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr14;
tr422:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st780;
tr1034:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st780;
st780:
	if ( ++p == pe )
		goto _test_eof780;
case 780:
#line 11024 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr5;
tr1016:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st781;
st781:
	if ( ++p == pe )
		goto _test_eof781;
case 781:
#line 11064 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st385;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr423:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st782;
tr1020:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/units.rl"
	{n2="am";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st782;
tr1036:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st782;
tr1026:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 48 "rules/units.rl"
	{n2="pm";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st782;
tr991:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st782;
st782:
	if ( ++p == pe )
		goto _test_eof782;
case 782:
#line 11140 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr5;
st386:
	if ( ++p == pe )
		goto _test_eof386;
case 386:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr422;
		} else if ( (*p) >= 65 )
			goto tr422;
	} else
		goto st249;
	goto tr5;
tr424:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st783;
tr1029:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 47 "rules/units.rl"
	{n2="am";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st783;
tr1024:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st783;
tr1066:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 48 "rules/units.rl"
	{n2="pm";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st783;
tr1031:
#line 1 "NONE"
	{ s.te = p+1;}
#line 8 "actions.rl"
	{ s.suffix += 3; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st783;
tr1043:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "actions.rl"
	{ s.suffix += 4; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st783;
tr1045:
#line 1 "NONE"
	{ s.te = p+1;}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st783;
tr1050:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st783;
st783:
	if ( ++p == pe )
		goto _test_eof783;
case 783:
#line 11262 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr5;
tr1013:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st784;
st784:
	if ( ++p == pe )
		goto _test_eof784;
case 784:
#line 11302 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st376;
		case 47: goto st94;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1013;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1014:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 31;}
	goto st785;
st785:
	if ( ++p == pe )
		goto _test_eof785;
case 785:
#line 11343 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1018;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1015;
		} else
			goto tr1014;
	} else
		goto st9;
	goto tr797;
tr1018:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st786;
st786:
	if ( ++p == pe )
		goto _test_eof786;
case 786:
#line 11383 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st385;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1015;
		} else
			goto tr1014;
	} else
		goto st9;
	goto tr763;
tr1015:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 31;}
	goto st787;
st787:
	if ( ++p == pe )
		goto _test_eof787;
case 787:
#line 11423 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1018;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1015;
		} else
			goto tr1014;
	} else
		goto st9;
	goto tr797;
tr989:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st788;
st788:
	if ( ++p == pe )
		goto _test_eof788;
case 788:
#line 11463 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st387;
		case 64: goto st250;
		case 77: goto tr1020;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
st387:
	if ( ++p == pe )
		goto _test_eof387;
case 387:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 77: goto tr425;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr422;
		} else if ( (*p) >= 65 )
			goto tr422;
	} else
		goto st249;
	goto tr14;
tr425:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st789;
st789:
	if ( ++p == pe )
		goto _test_eof789;
case 789:
#line 11529 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto tr1021;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1021:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "actions.rl"
	{ s.suffix += 4; }
#line 47 "rules/units.rl"
	{n2="am";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st790;
tr1027:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "actions.rl"
	{ s.suffix += 4; }
#line 48 "rules/units.rl"
	{n2="pm";}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st790;
st790:
	if ( ++p == pe )
		goto _test_eof790;
case 790:
#line 11583 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr798;
tr990:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st791;
tr992:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st791;
st791:
	if ( ++p == pe )
		goto _test_eof791;
case 791:
#line 11622 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 66: goto tr1022;
		case 95: goto st249;
		case 98: goto tr1023;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr5;
tr1022:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st792;
st792:
	if ( ++p == pe )
		goto _test_eof792;
case 792:
#line 11666 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1008;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr798;
tr1008:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st793;
st793:
	if ( ++p == pe )
		goto _test_eof793;
case 793:
#line 11707 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1024;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1023:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st794;
st794:
	if ( ++p == pe )
		goto _test_eof794;
case 794:
#line 11750 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1008;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr798;
tr993:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st795;
st795:
	if ( ++p == pe )
		goto _test_eof795;
case 795:
#line 11791 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st388;
		case 64: goto st250;
		case 77: goto tr1026;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
st388:
	if ( ++p == pe )
		goto _test_eof388;
case 388:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 77: goto tr426;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr422;
		} else if ( (*p) >= 65 )
			goto tr422;
	} else
		goto st249;
	goto tr14;
tr426:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st796;
st796:
	if ( ++p == pe )
		goto _test_eof796;
case 796:
#line 11857 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto tr1027;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr994:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st797;
st797:
	if ( ++p == pe )
		goto _test_eof797;
case 797:
#line 11897 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st389;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr1029;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
st389:
	if ( ++p == pe )
		goto _test_eof389;
case 389:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr425;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr422;
		} else if ( (*p) >= 65 )
			goto tr422;
	} else
		goto st249;
	goto tr14;
tr995:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st798;
st798:
	if ( ++p == pe )
		goto _test_eof798;
case 798:
#line 11963 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 112: goto tr1030;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1030:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st799;
st799:
	if ( ++p == pe )
		goto _test_eof799;
case 799:
#line 12004 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1031;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr996:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st800;
st800:
	if ( ++p == pe )
		goto _test_eof800;
case 800:
#line 12047 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr1032;
		case 117: goto tr1033;
		case 119: goto tr1008;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr798;
tr1039:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st801;
tr1032:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st801;
st801:
	if ( ++p == pe )
		goto _test_eof801;
case 801:
#line 12100 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 94: goto st47;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1034;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr424;
			} else if ( (*p) >= 65 )
				goto tr423;
		} else
			goto tr422;
	} else
		goto tr422;
	goto tr798;
tr1033:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st802;
st802:
	if ( ++p == pe )
		goto _test_eof802;
case 802:
#line 12147 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 32: goto tr52;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st390;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
st390:
	if ( ++p == pe )
		goto _test_eof390;
case 390:
	switch( (*p) ) {
		case -30: goto st201;
		case 32: goto tr60;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr422;
		} else if ( (*p) >= 65 )
			goto tr422;
	} else
		goto st249;
	goto tr14;
tr997:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st803;
st803:
	if ( ++p == pe )
		goto _test_eof803;
case 803:
#line 12213 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 66: goto tr1036;
		case 95: goto st249;
		case 112: goto tr1037;
		case 114: goto tr1024;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1037:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st804;
st804:
	if ( ++p == pe )
		goto _test_eof804;
case 804:
#line 12256 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1031;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr998:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st805;
st805:
	if ( ++p == pe )
		goto _test_eof805;
case 805:
#line 12297 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1038;
		case 116: goto tr1039;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1038:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st806;
st806:
	if ( ++p == pe )
		goto _test_eof806;
case 806:
#line 12339 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 32: goto st59;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr999:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st807;
st807:
	if ( ++p == pe )
		goto _test_eof807;
case 807:
#line 12382 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1040;
		case 98: goto tr1041;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 99 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr798;
tr1040:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st808;
st808:
	if ( ++p == pe )
		goto _test_eof808;
case 808:
#line 12424 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1031;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1041:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st809;
st809:
	if ( ++p == pe )
		goto _test_eof809;
case 809:
#line 12465 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1042;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1042:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st810;
st810:
	if ( ++p == pe )
		goto _test_eof810;
case 810:
#line 12506 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1043;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1000:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st811;
st811:
	if ( ++p == pe )
		goto _test_eof811;
case 811:
#line 12547 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1044;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1044:
#line 1 "NONE"
	{ s.te = p+1;}
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st812;
st812:
	if ( ++p == pe )
		goto _test_eof812;
case 812:
#line 12588 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1045;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr798;
tr1001:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st813;
st813:
	if ( ++p == pe )
		goto _test_eof813;
case 813:
#line 12629 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1039;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1002:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st814;
st814:
	if ( ++p == pe )
		goto _test_eof814;
case 814:
#line 12672 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 66: goto tr1036;
		case 95: goto st249;
		case 98: goto tr1046;
		case 103: goto tr1024;
		case 109: goto tr1047;
		case 112: goto tr1048;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr798;
tr1052:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st815;
tr1046:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st815;
st815:
	if ( ++p == pe )
		goto _test_eof815;
case 815:
#line 12725 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1049;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr5;
tr1049:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st816;
st816:
	if ( ++p == pe )
		goto _test_eof816;
case 816:
#line 12766 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1031;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1047:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st817;
st817:
	if ( ++p == pe )
		goto _test_eof817;
case 817:
#line 12809 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 47: goto st67;
		case 64: goto st250;
		case 94: goto st47;
		case 95: goto st249;
		case 104: goto tr1050;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1034;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr424;
			} else if ( (*p) >= 65 )
				goto tr423;
		} else
			goto tr422;
	} else
		goto tr422;
	goto tr798;
tr1048:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st818;
st818:
	if ( ++p == pe )
		goto _test_eof818;
case 818:
#line 12858 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 104: goto tr1031;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1003:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st819;
st819:
	if ( ++p == pe )
		goto _test_eof819;
case 819:
#line 12899 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 98: goto tr1051;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1051:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st820;
st820:
	if ( ++p == pe )
		goto _test_eof820;
case 820:
#line 12942 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1050;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr798;
tr1004:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st821;
st821:
	if ( ++p == pe )
		goto _test_eof821;
case 821:
#line 12985 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 47: goto st70;
		case 64: goto st250;
		case 76: goto tr991;
		case 94: goto st47;
		case 95: goto st249;
		case 98: goto tr1052;
		case 103: goto tr1050;
		case 105: goto tr1053;
		case 109: goto tr1032;
		case 110: goto tr1054;
		case 111: goto tr1044;
		case 112: goto tr1055;
		case 116: goto tr1056;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1034;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr424;
			} else if ( (*p) >= 65 )
				goto tr423;
		} else
			goto tr422;
	} else
		goto tr422;
	goto tr798;
tr1053:
#line 1 "NONE"
	{ s.te = p+1;}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st822;
st822:
	if ( ++p == pe )
		goto _test_eof822;
case 822:
#line 13044 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 94: goto st47;
		case 95: goto st249;
		case 99: goto tr1057;
		case 108: goto tr1058;
		case 110: goto tr1044;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 49 ) {
		if ( (*p) < 52 ) {
			if ( 50 <= (*p) && (*p) <= 51 )
				goto tr1034;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 97 <= (*p) && (*p) <= 122 )
					goto tr424;
			} else if ( (*p) >= 65 )
				goto tr423;
		} else
			goto tr422;
	} else
		goto tr422;
	goto tr798;
tr1057:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st823;
st823:
	if ( ++p == pe )
		goto _test_eof823;
case 823:
#line 13094 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1059;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1059:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st824;
st824:
	if ( ++p == pe )
		goto _test_eof824;
case 824:
#line 13135 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto tr1060;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1060:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st825;
st825:
	if ( ++p == pe )
		goto _test_eof825;
case 825:
#line 13176 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1061;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1061:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st826;
st826:
	if ( ++p == pe )
		goto _test_eof826;
case 826:
#line 13217 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1062;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1062:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st827;
st827:
	if ( ++p == pe )
		goto _test_eof827;
case 827:
#line 13258 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1044;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1058:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 51 "rules/main.rl"
	{ s.act = 28;}
	goto st828;
st828:
	if ( ++p == pe )
		goto _test_eof828;
case 828:
#line 13301 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1063;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr798;
tr1063:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st829;
st829:
	if ( ++p == pe )
		goto _test_eof829;
case 829:
#line 13342 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1060;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1054:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st830;
st830:
	if ( ++p == pe )
		goto _test_eof830;
case 830:
#line 13383 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1056;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1056:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st831;
st831:
	if ( ++p == pe )
		goto _test_eof831;
case 831:
#line 13424 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 104: goto tr1044;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1055:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st832;
st832:
	if ( ++p == pe )
		goto _test_eof832;
case 832:
#line 13465 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 104: goto tr1024;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1005:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st833;
st833:
	if ( ++p == pe )
		goto _test_eof833;
case 833:
#line 13506 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1064;
		case 111: goto tr1024;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1064:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st834;
st834:
	if ( ++p == pe )
		goto _test_eof834;
case 834:
#line 13548 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1059;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1006:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st835;
st835:
	if ( ++p == pe )
		goto _test_eof835;
case 835:
#line 13589 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 122: goto tr1024;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1007:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st836;
st836:
	if ( ++p == pe )
		goto _test_eof836;
case 836:
#line 13630 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st391;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr1066;
		case 116: goto tr1024;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
st391:
	if ( ++p == pe )
		goto _test_eof391;
case 391:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr426;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr422;
		} else if ( (*p) >= 65 )
			goto tr422;
	} else
		goto st249;
	goto tr14;
tr1009:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st837;
st837:
	if ( ++p == pe )
		goto _test_eof837;
case 837:
#line 13697 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 112: goto tr1067;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1067:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st838;
st838:
	if ( ++p == pe )
		goto _test_eof838;
case 838:
#line 13738 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr1031;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1010:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st839;
st839:
	if ( ++p == pe )
		goto _test_eof839;
case 839:
#line 13779 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1062;
		case 113: goto tr1033;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1011:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st840;
st840:
	if ( ++p == pe )
		goto _test_eof840;
case 840:
#line 13821 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto tr1068;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1068:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st841;
st841:
	if ( ++p == pe )
		goto _test_eof841;
case 841:
#line 13862 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1031;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1012:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st842;
st842:
	if ( ++p == pe )
		goto _test_eof842;
case 842:
#line 13903 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st386;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1039;
		case 114: goto tr1044;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr706:
#line 1 "NONE"
	{ s.te = p+1;}
#line 20 "rules/main.rl"
	{ s.act = 7;}
	goto st843;
st843:
	if ( ++p == pe )
		goto _test_eof843;
case 843:
#line 13945 "tokenizer.cc"
	switch( (*p) ) {
		case 32: goto st392;
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st395;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr343;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr1069;
st392:
	if ( ++p == pe )
		goto _test_eof392;
case 392:
	if ( (*p) == 46 )
		goto st393;
	goto tr427;
st393:
	if ( ++p == pe )
		goto _test_eof393;
case 393:
	if ( (*p) == 32 )
		goto st394;
	goto tr427;
st394:
	if ( ++p == pe )
		goto _test_eof394;
case 394:
	if ( (*p) == 46 )
		goto tr155;
	goto tr427;
st395:
	if ( ++p == pe )
		goto _test_eof395;
case 395:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto tr430;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr427;
tr1072:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st844;
tr430:
#line 1 "NONE"
	{ s.te = p+1;}
#line 23 "rules/main.rl"
	{ s.act = 10;}
	goto st844;
st844:
	if ( ++p == pe )
		goto _test_eof844;
case 844:
#line 14020 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto tr1072;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr5;
tr707:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st845;
st845:
	if ( ++p == pe )
		goto _test_eof845;
case 845:
#line 14047 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr1073;
		case 46: goto st396;
		case 47: goto st420;
		case 48: goto tr1076;
		case 58: goto st425;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1077;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
tr1073:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st846;
st846:
	if ( ++p == pe )
		goto _test_eof846;
case 846:
#line 14115 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st385;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1079;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1015;
		} else
			goto tr1014;
	} else
		goto st9;
	goto tr763;
tr1079:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st847;
st847:
	if ( ++p == pe )
		goto _test_eof847;
case 847:
#line 14155 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1080;
		case 46: goto st376;
		case 47: goto st94;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1081;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1080:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st848;
st848:
	if ( ++p == pe )
		goto _test_eof848;
case 848:
#line 14196 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st385;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1082;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1082:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st849;
st849:
	if ( ++p == pe )
		goto _test_eof849;
case 849:
#line 14236 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1083;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1083:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st850;
st850:
	if ( ++p == pe )
		goto _test_eof850;
case 850:
#line 14276 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st376;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1084;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
tr1084:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st851;
st851:
	if ( ++p == pe )
		goto _test_eof851;
case 851:
#line 14340 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1085;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
tr1085:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st852;
st852:
	if ( ++p == pe )
		goto _test_eof852;
case 852:
#line 14380 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1016;
		case 46: goto st376;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr422;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
tr1081:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st853;
st853:
	if ( ++p == pe )
		goto _test_eof853;
case 853:
#line 14444 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1080;
		case 46: goto st376;
		case 47: goto st94;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1013;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr763;
st396:
	if ( ++p == pe )
		goto _test_eof396;
case 396:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 50: goto tr432;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 51 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto tr431;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto tr433;
	goto tr82;
tr431:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st854;
st854:
	if ( ++p == pe )
		goto _test_eof854;
case 854:
#line 14513 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st397;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1087;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
st397:
	if ( ++p == pe )
		goto _test_eof397;
case 397:
	switch( (*p) ) {
		case 43: goto st249;
		case 50: goto st401;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 51 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto st398;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st403;
	goto tr82;
st398:
	if ( ++p == pe )
		goto _test_eof398;
case 398:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr438;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st399:
	if ( ++p == pe )
		goto _test_eof399;
case 399:
	switch( (*p) ) {
		case 43: goto st249;
		case 50: goto tr440;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 51 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto tr439;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto tr441;
	goto tr82;
tr439:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st855;
st855:
	if ( ++p == pe )
		goto _test_eof855;
case 855:
#line 14636 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto tr441;
	goto tr927;
tr441:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st856;
st856:
	if ( ++p == pe )
		goto _test_eof856;
case 856:
#line 14664 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto tr1088;
	goto tr927;
tr440:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st857;
st857:
	if ( ++p == pe )
		goto _test_eof857;
case 857:
#line 14692 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 53: goto tr1089;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 52 )
				goto tr441;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto tr1088;
	goto tr927;
tr1089:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st858;
st858:
	if ( ++p == pe )
		goto _test_eof858;
case 858:
#line 14724 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 54 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 53 )
				goto tr1088;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr927;
tr438:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st859;
st859:
	if ( ++p == pe )
		goto _test_eof859;
case 859:
#line 14755 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st400;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
st400:
	if ( ++p == pe )
		goto _test_eof400;
case 400:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr442;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr442:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st860;
st860:
	if ( ++p == pe )
		goto _test_eof860;
case 860:
#line 14828 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 66 )
			goto st249;
	} else
		goto st249;
	goto tr771;
st401:
	if ( ++p == pe )
		goto _test_eof401;
case 401:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 53: goto tr443;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 52 )
			goto tr438;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto tr444;
	goto tr82;
tr443:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st861;
st861:
	if ( ++p == pe )
		goto _test_eof861;
case 861:
#line 14906 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st400;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 66 )
			goto st249;
	} else
		goto st402;
	goto tr771;
st402:
	if ( ++p == pe )
		goto _test_eof402;
case 402:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto tr442;
	goto tr82;
tr444:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st862;
st862:
	if ( ++p == pe )
		goto _test_eof862;
case 862:
#line 14983 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st402;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
st403:
	if ( ++p == pe )
		goto _test_eof403;
case 403:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr444;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr1087:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st863;
st863:
	if ( ++p == pe )
		goto _test_eof863;
case 863:
#line 15056 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st397;
		case 64: goto st250;
		case 65: goto st410;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st413;
		case 95: goto st249;
		case 97: goto st416;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st418;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1092;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
tr1092:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st864;
st864:
	if ( ++p == pe )
		goto _test_eof864;
case 864:
#line 15109 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st404;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr343;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
st404:
	if ( ++p == pe )
		goto _test_eof404;
case 404:
	switch( (*p) ) {
		case 43: goto st249;
		case 50: goto st408;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 51 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto st405;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st406;
	goto tr82;
st405:
	if ( ++p == pe )
		goto _test_eof405;
case 405:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st406;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st406:
	if ( ++p == pe )
		goto _test_eof406;
case 406:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st407;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st407:
	if ( ++p == pe )
		goto _test_eof407;
case 407:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st408:
	if ( ++p == pe )
		goto _test_eof408;
case 408:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 53: goto st409;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 52 )
			goto st406;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st407;
	goto tr82;
st409:
	if ( ++p == pe )
		goto _test_eof409;
case 409:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st399;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto st407;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st410:
	if ( ++p == pe )
		goto _test_eof410;
case 410:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st411;
		case 64: goto st250;
		case 77: goto tr346;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st411:
	if ( ++p == pe )
		goto _test_eof411;
case 411:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 77: goto st412;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st412:
	if ( ++p == pe )
		goto _test_eof412;
case 412:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto tr348;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st413:
	if ( ++p == pe )
		goto _test_eof413;
case 413:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st414;
		case 64: goto st250;
		case 77: goto tr352;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st414:
	if ( ++p == pe )
		goto _test_eof414;
case 414:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 77: goto st415;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st415:
	if ( ++p == pe )
		goto _test_eof415;
case 415:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto tr354;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st416:
	if ( ++p == pe )
		goto _test_eof416;
case 416:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st417;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr346;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st417:
	if ( ++p == pe )
		goto _test_eof417;
case 417:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto st412;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st418:
	if ( ++p == pe )
		goto _test_eof418;
case 418:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto st419;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr352;
		case 116: goto tr350;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr82;
st419:
	if ( ++p == pe )
		goto _test_eof419;
case 419:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto st415;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr82;
tr432:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st865;
st865:
	if ( ++p == pe )
		goto _test_eof865;
case 865:
#line 15507 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st397;
		case 53: goto tr1098;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 52 )
			goto tr1087;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 66 )
			goto st249;
	} else
		goto tr1099;
	goto tr771;
tr1098:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st866;
st866:
	if ( ++p == pe )
		goto _test_eof866;
case 866:
#line 15564 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st397;
		case 64: goto st250;
		case 65: goto st410;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st413;
		case 95: goto st249;
		case 97: goto st416;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st418;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto tr1092;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 66 )
			goto st249;
	} else
		goto tr343;
	goto tr771;
tr1099:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st867;
st867:
	if ( ++p == pe )
		goto _test_eof867;
case 867:
#line 15620 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st397;
		case 64: goto st250;
		case 65: goto st410;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st413;
		case 95: goto st249;
		case 97: goto st416;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st418;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr343;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
tr433:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st868;
st868:
	if ( ++p == pe )
		goto _test_eof868;
case 868:
#line 15673 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st397;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1099;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
st420:
	if ( ++p == pe )
		goto _test_eof420;
case 420:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr456;
	goto tr82;
tr456:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st869;
st869:
	if ( ++p == pe )
		goto _test_eof869;
case 869:
#line 15733 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 47: goto st421;
		case 65: goto st35;
		case 71: goto st38;
		case 76: goto tr59;
		case 77: goto tr777;
		case 80: goto st40;
		case 97: goto st43;
		case 98: goto st45;
		case 99: goto tr781;
		case 100: goto st55;
		case 102: goto st57;
		case 103: goto tr784;
		case 104: goto st64;
		case 105: goto st65;
		case 107: goto tr787;
		case 108: goto st69;
		case 109: goto tr789;
		case 110: goto st80;
		case 111: goto st82;
		case 112: goto st83;
		case 113: goto st51;
		case 114: goto st85;
		case 115: goto st87;
		case 116: goto st88;
		case 121: goto st90;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr1101;
	goto tr771;
st421:
	if ( ++p == pe )
		goto _test_eof421;
case 421:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st422;
	goto tr82;
st422:
	if ( ++p == pe )
		goto _test_eof422;
case 422:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr458;
	goto tr82;
tr458:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st870;
st870:
	if ( ++p == pe )
		goto _test_eof870;
case 870:
#line 15790 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 65: goto st35;
		case 71: goto st38;
		case 76: goto tr59;
		case 77: goto tr777;
		case 80: goto st40;
		case 97: goto st43;
		case 98: goto st45;
		case 99: goto tr781;
		case 100: goto st55;
		case 102: goto st57;
		case 103: goto tr784;
		case 104: goto st64;
		case 105: goto st65;
		case 107: goto tr787;
		case 108: goto st69;
		case 109: goto tr789;
		case 110: goto st80;
		case 111: goto st82;
		case 112: goto st83;
		case 113: goto st51;
		case 114: goto st85;
		case 115: goto st87;
		case 116: goto st88;
		case 121: goto st90;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st246;
	goto tr771;
tr1101:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st871;
st871:
	if ( ++p == pe )
		goto _test_eof871;
case 871:
#line 15832 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 47: goto st421;
		case 65: goto st35;
		case 71: goto st38;
		case 76: goto tr59;
		case 77: goto tr777;
		case 80: goto st40;
		case 97: goto st43;
		case 98: goto st45;
		case 99: goto tr781;
		case 100: goto st55;
		case 102: goto st57;
		case 103: goto tr784;
		case 104: goto st64;
		case 105: goto st65;
		case 107: goto tr787;
		case 108: goto st69;
		case 109: goto tr789;
		case 110: goto st80;
		case 111: goto st82;
		case 112: goto st83;
		case 113: goto st51;
		case 114: goto st85;
		case 115: goto st87;
		case 116: goto st88;
		case 121: goto st90;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr84;
	goto tr771;
tr1076:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st872;
st872:
	if ( ++p == pe )
		goto _test_eof872;
case 872:
#line 15875 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr1102;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr1073;
		case 46: goto st396;
		case 47: goto st420;
		case 58: goto st425;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1103;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
tr1102:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st423;
st423:
	if ( ++p == pe )
		goto _test_eof423;
case 423:
#line 15940 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr171;
		case 76: goto st172;
		case 77: goto tr173;
		case 82: goto st173;
		case 83: goto tr175;
		case 86: goto st174;
		case 100: goto tr177;
		case 108: goto st175;
		case 109: goto tr179;
		case 114: goto st176;
		case 115: goto tr459;
		case 118: goto st177;
	}
	goto tr82;
tr1103:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st873;
st873:
	if ( ++p == pe )
		goto _test_eof873;
case 873:
#line 15966 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr988;
		case 46: goto st424;
		case 47: goto st94;
		case 48: goto tr1105;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr983;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
st424:
	if ( ++p == pe )
		goto _test_eof424;
case 424:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 50: goto tr461;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 51 ) {
		if ( (*p) > 46 ) {
			if ( 48 <= (*p) && (*p) <= 49 )
				goto tr460;
		} else if ( (*p) >= 45 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto tr462;
	goto tr82;
tr460:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st874;
st874:
	if ( ++p == pe )
		goto _test_eof874;
case 874:
#line 16061 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st404;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr462;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
tr462:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st875;
st875:
	if ( ++p == pe )
		goto _test_eof875;
case 875:
#line 16114 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st404;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr1092;
	} else if ( (*p) > 90 ) {
		if ( 101 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr771;
tr461:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st876;
st876:
	if ( ++p == pe )
		goto _test_eof876;
case 876:
#line 16167 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st404;
		case 53: goto tr1106;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 52 )
			goto tr462;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 66 )
			goto st249;
	} else
		goto tr1092;
	goto tr771;
tr1106:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st877;
st877:
	if ( ++p == pe )
		goto _test_eof877;
case 877:
#line 16224 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 43: goto st249;
		case 45: goto st302;
		case 46: goto st404;
		case 64: goto st250;
		case 65: goto st303;
		case 71: goto st306;
		case 76: goto tr941;
		case 77: goto tr942;
		case 80: goto st308;
		case 95: goto st249;
		case 97: goto st311;
		case 98: goto st313;
		case 99: goto tr946;
		case 100: goto st317;
		case 102: goto st319;
		case 103: goto tr949;
		case 104: goto st324;
		case 105: goto st325;
		case 107: goto tr952;
		case 108: goto st328;
		case 109: goto tr954;
		case 110: goto st339;
		case 111: goto st341;
		case 112: goto st342;
		case 113: goto st344;
		case 114: goto st345;
		case 115: goto st347;
		case 116: goto st348;
		case 121: goto st350;
	}
	if ( (*p) < 54 ) {
		if ( 48 <= (*p) && (*p) <= 53 )
			goto tr1092;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 66 )
			goto st249;
	} else
		goto tr343;
	goto tr771;
tr1105:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st878;
st878:
	if ( ++p == pe )
		goto _test_eof878;
case 878:
#line 16280 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr1102;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr988;
		case 46: goto st381;
		case 47: goto st94;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr983;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
st425:
	if ( ++p == pe )
		goto _test_eof425;
case 425:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st426;
	goto tr82;
st426:
	if ( ++p == pe )
		goto _test_eof426;
case 426:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr464;
	goto tr82;
tr464:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st879;
st879:
	if ( ++p == pe )
		goto _test_eof879;
case 879:
#line 16360 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st24;
		case 38: goto st25;
		case 65: goto st427;
		case 71: goto st38;
		case 76: goto tr59;
		case 77: goto tr777;
		case 80: goto st430;
		case 97: goto st433;
		case 98: goto st45;
		case 99: goto tr781;
		case 100: goto st55;
		case 102: goto st57;
		case 103: goto tr784;
		case 104: goto st64;
		case 105: goto st65;
		case 107: goto tr787;
		case 108: goto st69;
		case 109: goto tr789;
		case 110: goto st80;
		case 111: goto st82;
		case 112: goto st435;
		case 113: goto st51;
		case 114: goto st85;
		case 115: goto st87;
		case 116: goto st88;
		case 121: goto st90;
	}
	goto tr771;
st427:
	if ( ++p == pe )
		goto _test_eof427;
case 427:
	switch( (*p) ) {
		case 46: goto st428;
		case 77: goto tr40;
	}
	goto tr82;
st428:
	if ( ++p == pe )
		goto _test_eof428;
case 428:
	if ( (*p) == 77 )
		goto st429;
	goto tr82;
st429:
	if ( ++p == pe )
		goto _test_eof429;
case 429:
	if ( (*p) == 46 )
		goto tr42;
	goto tr82;
st430:
	if ( ++p == pe )
		goto _test_eof430;
case 430:
	switch( (*p) ) {
		case 46: goto st431;
		case 77: goto tr46;
	}
	goto tr82;
st431:
	if ( ++p == pe )
		goto _test_eof431;
case 431:
	if ( (*p) == 77 )
		goto st432;
	goto tr82;
st432:
	if ( ++p == pe )
		goto _test_eof432;
case 432:
	if ( (*p) == 46 )
		goto tr48;
	goto tr82;
st433:
	if ( ++p == pe )
		goto _test_eof433;
case 433:
	switch( (*p) ) {
		case 46: goto st434;
		case 109: goto tr40;
	}
	goto tr82;
st434:
	if ( ++p == pe )
		goto _test_eof434;
case 434:
	if ( (*p) == 109 )
		goto st429;
	goto tr82;
st435:
	if ( ++p == pe )
		goto _test_eof435;
case 435:
	switch( (*p) ) {
		case 46: goto st436;
		case 109: goto tr46;
		case 116: goto tr28;
	}
	goto tr82;
st436:
	if ( ++p == pe )
		goto _test_eof436;
case 436:
	if ( (*p) == 109 )
		goto st432;
	goto tr82;
tr1077:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st880;
st880:
	if ( ++p == pe )
		goto _test_eof880;
case 880:
#line 16479 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr1073;
		case 46: goto st396;
		case 47: goto st420;
		case 58: goto st425;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1103;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
tr708:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st881;
st881:
	if ( ++p == pe )
		goto _test_eof881;
case 881:
#line 16546 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr1073;
		case 46: goto st396;
		case 47: goto st420;
		case 48: goto tr1076;
		case 53: goto tr1111;
		case 58: goto st425;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < 49 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 52 ) {
		if ( (*p) < 66 ) {
			if ( 54 <= (*p) && (*p) <= 57 )
				goto tr1112;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto tr1077;
	goto tr771;
tr1111:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st882;
st882:
	if ( ++p == pe )
		goto _test_eof882;
case 882:
#line 16618 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr1073;
		case 46: goto st396;
		case 47: goto st420;
		case 58: goto st425;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 53 ) {
		if ( (*p) < 66 ) {
			if ( 54 <= (*p) && (*p) <= 57 )
				goto tr1113;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto tr1103;
	goto tr771;
tr1113:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st883;
st883:
	if ( ++p == pe )
		goto _test_eof883;
case 883:
#line 16688 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr988;
		case 46: goto st381;
		case 47: goto st94;
		case 48: goto tr1105;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr983;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
tr1112:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st884;
st884:
	if ( ++p == pe )
		goto _test_eof884;
case 884:
#line 16755 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr1073;
		case 46: goto st396;
		case 47: goto st420;
		case 58: goto st425;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1113;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
tr709:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st885;
st885:
	if ( ++p == pe )
		goto _test_eof885;
case 885:
#line 16822 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr214;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr1073;
		case 46: goto st396;
		case 47: goto st420;
		case 48: goto tr1114;
		case 58: goto st425;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 49 <= (*p) && (*p) <= 57 )
				goto tr1112;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
tr1114:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st886;
st886:
	if ( ++p == pe )
		goto _test_eof886;
case 886:
#line 16890 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st91;
		case -30: goto st169;
		case 38: goto st382;
		case 39: goto tr1102;
		case 43: goto st249;
		case 44: goto st93;
		case 45: goto tr1073;
		case 46: goto st396;
		case 47: goto st420;
		case 58: goto st425;
		case 64: goto st250;
		case 65: goto tr989;
		case 71: goto tr990;
		case 76: goto tr991;
		case 77: goto tr992;
		case 80: goto tr993;
		case 95: goto st249;
		case 97: goto tr994;
		case 98: goto tr995;
		case 99: goto tr996;
		case 100: goto tr997;
		case 102: goto tr998;
		case 103: goto tr999;
		case 104: goto tr1000;
		case 105: goto tr1001;
		case 107: goto tr1002;
		case 108: goto tr1003;
		case 109: goto tr1004;
		case 110: goto tr1005;
		case 111: goto tr1006;
		case 112: goto tr1007;
		case 113: goto tr1008;
		case 114: goto tr1009;
		case 115: goto tr1010;
		case 116: goto tr1011;
		case 121: goto tr1012;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr1113;
		} else if ( (*p) > 90 ) {
			if ( 101 <= (*p) && (*p) <= 122 )
				goto tr424;
		} else
			goto tr423;
	} else
		goto st9;
	goto tr771;
tr710:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st887;
st887:
	if ( ++p == pe )
		goto _test_eof887;
case 887:
#line 16955 "tokenizer.cc"
	switch( (*p) ) {
		case 33: goto st438;
		case 47: goto st446;
		case 62: goto tr472;
		case 83: goto st452;
		case 108: goto st479;
		case 111: goto st481;
		case 112: goto st483;
		case 115: goto st452;
		case 117: goto st481;
	}
	goto st437;
st437:
	if ( ++p == pe )
		goto _test_eof437;
case 437:
	if ( (*p) == 62 )
		goto tr472;
	goto st437;
st438:
	if ( ++p == pe )
		goto _test_eof438;
case 438:
	switch( (*p) ) {
		case 45: goto st439;
		case 62: goto tr472;
	}
	goto st437;
st439:
	if ( ++p == pe )
		goto _test_eof439;
case 439:
	switch( (*p) ) {
		case 45: goto st440;
		case 62: goto tr472;
	}
	goto st437;
st440:
	if ( ++p == pe )
		goto _test_eof440;
case 440:
	switch( (*p) ) {
		case 45: goto st441;
		case 62: goto tr476;
	}
	goto st440;
st441:
	if ( ++p == pe )
		goto _test_eof441;
case 441:
	switch( (*p) ) {
		case 45: goto st442;
		case 62: goto tr476;
	}
	goto st440;
st442:
	if ( ++p == pe )
		goto _test_eof442;
case 442:
	switch( (*p) ) {
		case 45: goto st442;
		case 62: goto tr472;
	}
	goto st440;
tr476:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st888;
st888:
	if ( ++p == pe )
		goto _test_eof888;
case 888:
#line 17028 "tokenizer.cc"
	if ( (*p) == 45 )
		goto st444;
	goto st443;
st443:
	if ( ++p == pe )
		goto _test_eof443;
case 443:
	if ( (*p) == 45 )
		goto st444;
	goto st443;
st444:
	if ( ++p == pe )
		goto _test_eof444;
case 444:
	if ( (*p) == 45 )
		goto st445;
	goto st443;
st445:
	if ( ++p == pe )
		goto _test_eof445;
case 445:
	switch( (*p) ) {
		case 45: goto st445;
		case 62: goto tr472;
	}
	goto st443;
st446:
	if ( ++p == pe )
		goto _test_eof446;
case 446:
	switch( (*p) ) {
		case 62: goto tr472;
		case 108: goto st447;
		case 111: goto st449;
		case 112: goto st451;
		case 117: goto st449;
	}
	goto st437;
st447:
	if ( ++p == pe )
		goto _test_eof447;
case 447:
	switch( (*p) ) {
		case 62: goto tr472;
		case 105: goto st448;
	}
	goto st437;
st448:
	if ( ++p == pe )
		goto _test_eof448;
case 448:
	if ( (*p) == 62 )
		goto tr486;
	goto st448;
st449:
	if ( ++p == pe )
		goto _test_eof449;
case 449:
	switch( (*p) ) {
		case 62: goto tr472;
		case 108: goto st450;
	}
	goto st437;
st450:
	if ( ++p == pe )
		goto _test_eof450;
case 450:
	if ( (*p) == 62 )
		goto tr488;
	goto st450;
st451:
	if ( ++p == pe )
		goto _test_eof451;
case 451:
	if ( (*p) == 62 )
		goto tr489;
	goto st451;
st452:
	if ( ++p == pe )
		goto _test_eof452;
case 452:
	switch( (*p) ) {
		case 62: goto tr472;
		case 67: goto st453;
		case 84: goto st467;
		case 99: goto st453;
		case 116: goto st467;
	}
	goto st437;
st453:
	if ( ++p == pe )
		goto _test_eof453;
case 453:
	switch( (*p) ) {
		case 62: goto tr472;
		case 82: goto st454;
		case 114: goto st454;
	}
	goto st437;
st454:
	if ( ++p == pe )
		goto _test_eof454;
case 454:
	switch( (*p) ) {
		case 62: goto tr472;
		case 73: goto st455;
		case 105: goto st455;
	}
	goto st437;
st455:
	if ( ++p == pe )
		goto _test_eof455;
case 455:
	switch( (*p) ) {
		case 62: goto tr472;
		case 80: goto st456;
		case 112: goto st456;
	}
	goto st437;
st456:
	if ( ++p == pe )
		goto _test_eof456;
case 456:
	switch( (*p) ) {
		case 62: goto tr472;
		case 84: goto st457;
		case 116: goto st457;
	}
	goto st437;
st457:
	if ( ++p == pe )
		goto _test_eof457;
case 457:
	if ( (*p) == 62 )
		goto tr496;
	goto st457;
tr496:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st889;
st889:
	if ( ++p == pe )
		goto _test_eof889;
case 889:
#line 17173 "tokenizer.cc"
	if ( (*p) == 60 )
		goto st459;
	goto st458;
st458:
	if ( ++p == pe )
		goto _test_eof458;
case 458:
	if ( (*p) == 60 )
		goto st459;
	goto st458;
st459:
	if ( ++p == pe )
		goto _test_eof459;
case 459:
	switch( (*p) ) {
		case 47: goto st460;
		case 60: goto st459;
	}
	goto st458;
st460:
	if ( ++p == pe )
		goto _test_eof460;
case 460:
	switch( (*p) ) {
		case 60: goto st459;
		case 83: goto st461;
		case 115: goto st461;
	}
	goto st458;
st461:
	if ( ++p == pe )
		goto _test_eof461;
case 461:
	switch( (*p) ) {
		case 60: goto st459;
		case 67: goto st462;
		case 99: goto st462;
	}
	goto st458;
st462:
	if ( ++p == pe )
		goto _test_eof462;
case 462:
	switch( (*p) ) {
		case 60: goto st459;
		case 82: goto st463;
		case 114: goto st463;
	}
	goto st458;
st463:
	if ( ++p == pe )
		goto _test_eof463;
case 463:
	switch( (*p) ) {
		case 60: goto st459;
		case 73: goto st464;
		case 105: goto st464;
	}
	goto st458;
st464:
	if ( ++p == pe )
		goto _test_eof464;
case 464:
	switch( (*p) ) {
		case 60: goto st459;
		case 80: goto st465;
		case 112: goto st465;
	}
	goto st458;
st465:
	if ( ++p == pe )
		goto _test_eof465;
case 465:
	switch( (*p) ) {
		case 60: goto st459;
		case 84: goto st466;
		case 116: goto st466;
	}
	goto st458;
st466:
	if ( ++p == pe )
		goto _test_eof466;
case 466:
	switch( (*p) ) {
		case 60: goto st459;
		case 62: goto tr472;
	}
	goto st458;
st467:
	if ( ++p == pe )
		goto _test_eof467;
case 467:
	switch( (*p) ) {
		case 62: goto tr472;
		case 89: goto st468;
		case 121: goto st468;
	}
	goto st437;
st468:
	if ( ++p == pe )
		goto _test_eof468;
case 468:
	switch( (*p) ) {
		case 62: goto tr472;
		case 76: goto st469;
		case 108: goto st469;
	}
	goto st437;
st469:
	if ( ++p == pe )
		goto _test_eof469;
case 469:
	switch( (*p) ) {
		case 62: goto tr472;
		case 69: goto st470;
		case 101: goto st470;
	}
	goto st437;
st470:
	if ( ++p == pe )
		goto _test_eof470;
case 470:
	if ( (*p) == 62 )
		goto tr509;
	goto st470;
tr509:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st890;
st890:
	if ( ++p == pe )
		goto _test_eof890;
case 890:
#line 17307 "tokenizer.cc"
	if ( (*p) == 60 )
		goto st472;
	goto st471;
st471:
	if ( ++p == pe )
		goto _test_eof471;
case 471:
	if ( (*p) == 60 )
		goto st472;
	goto st471;
st472:
	if ( ++p == pe )
		goto _test_eof472;
case 472:
	switch( (*p) ) {
		case 47: goto st473;
		case 60: goto st472;
	}
	goto st471;
st473:
	if ( ++p == pe )
		goto _test_eof473;
case 473:
	switch( (*p) ) {
		case 60: goto st472;
		case 83: goto st474;
		case 115: goto st474;
	}
	goto st471;
st474:
	if ( ++p == pe )
		goto _test_eof474;
case 474:
	switch( (*p) ) {
		case 60: goto st472;
		case 84: goto st475;
		case 116: goto st475;
	}
	goto st471;
st475:
	if ( ++p == pe )
		goto _test_eof475;
case 475:
	switch( (*p) ) {
		case 60: goto st472;
		case 89: goto st476;
		case 121: goto st476;
	}
	goto st471;
st476:
	if ( ++p == pe )
		goto _test_eof476;
case 476:
	switch( (*p) ) {
		case 60: goto st472;
		case 76: goto st477;
		case 108: goto st477;
	}
	goto st471;
st477:
	if ( ++p == pe )
		goto _test_eof477;
case 477:
	switch( (*p) ) {
		case 60: goto st472;
		case 69: goto st478;
		case 101: goto st478;
	}
	goto st471;
st478:
	if ( ++p == pe )
		goto _test_eof478;
case 478:
	switch( (*p) ) {
		case 60: goto st472;
		case 62: goto tr472;
	}
	goto st471;
st479:
	if ( ++p == pe )
		goto _test_eof479;
case 479:
	switch( (*p) ) {
		case 62: goto tr472;
		case 105: goto st480;
	}
	goto st437;
st480:
	if ( ++p == pe )
		goto _test_eof480;
case 480:
	if ( (*p) == 62 )
		goto tr519;
	goto st480;
st481:
	if ( ++p == pe )
		goto _test_eof481;
case 481:
	switch( (*p) ) {
		case 62: goto tr472;
		case 108: goto st482;
	}
	goto st437;
st482:
	if ( ++p == pe )
		goto _test_eof482;
case 482:
	if ( (*p) == 62 )
		goto tr521;
	goto st482;
st483:
	if ( ++p == pe )
		goto _test_eof483;
case 483:
	switch( (*p) ) {
		case 32: goto st483;
		case 47: goto st485;
		case 62: goto tr525;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st483;
	goto st484;
st484:
	if ( ++p == pe )
		goto _test_eof484;
case 484:
	if ( (*p) == 62 )
		goto tr525;
	goto st484;
st485:
	if ( ++p == pe )
		goto _test_eof485;
case 485:
	if ( (*p) == 62 )
		goto tr525;
	goto st484;
tr711:
#line 1 "NONE"
	{ s.te = p+1;}
	goto st891;
st891:
	if ( ++p == pe )
		goto _test_eof891;
case 891:
#line 17452 "tokenizer.cc"
	if ( (*p) == 61 )
		goto st486;
	goto tr903;
st486:
	if ( ++p == pe )
		goto _test_eof486;
case 486:
	if ( (*p) == 61 )
		goto st892;
	goto tr133;
st892:
	if ( ++p == pe )
		goto _test_eof892;
case 892:
	if ( (*p) == 61 )
		goto st892;
	goto tr771;
tr713:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st893;
st893:
	if ( ++p == pe )
		goto _test_eof893;
case 893:
#line 17480 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1125;
		case 64: goto st250;
		case 73: goto tr1127;
		case 78: goto tr1128;
		case 82: goto tr1129;
		case 95: goto st249;
		case 97: goto tr534;
		case 100: goto tr1131;
		case 101: goto tr534;
		case 105: goto tr1132;
		case 108: goto tr1133;
		case 111: goto tr534;
		case 112: goto tr1134;
		case 114: goto tr1135;
		case 115: goto tr1136;
		case 117: goto tr1137;
		case 118: goto tr1138;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
st487:
	if ( ++p == pe )
		goto _test_eof487;
case 487:
	switch( (*p) ) {
		case 35: goto st179;
		case 38: goto st488;
		case 97: goto st191;
		case 114: goto st195;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr528;
	goto tr5;
st488:
	if ( ++p == pe )
		goto _test_eof488;
case 488:
	if ( (*p) == 38 )
		goto st488;
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr529;
	goto tr5;
tr529:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st894;
st894:
	if ( ++p == pe )
		goto _test_eof894;
case 894:
#line 17558 "tokenizer.cc"
	if ( (*p) == 38 )
		goto st488;
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr529;
	goto tr1139;
tr528:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st895;
st895:
	if ( ++p == pe )
		goto _test_eof895;
case 895:
#line 17574 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st489;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr1141;
	} else
		goto st9;
	goto tr1139;
st489:
	if ( ++p == pe )
		goto _test_eof489;
case 489:
	switch( (*p) ) {
		case 35: goto st179;
		case 38: goto st488;
		case 97: goto st191;
		case 114: goto st195;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr529;
	goto tr530;
tr1141:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st896;
st896:
	if ( ++p == pe )
		goto _test_eof896;
case 896:
#line 17624 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr1141;
	} else
		goto st9;
	goto tr1139;
st490:
	if ( ++p == pe )
		goto _test_eof490;
case 490:
	switch( (*p) ) {
		case 36: goto tr2;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st249;
		} else if ( (*p) >= 65 )
			goto st249;
	} else
		goto st249;
	goto tr14;
tr1124:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st897;
st897:
	if ( ++p == pe )
		goto _test_eof897;
case 897:
#line 17683 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st491;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st491:
	if ( ++p == pe )
		goto _test_eof491;
case 491:
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st491;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr14;
tr532:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st898;
st898:
	if ( ++p == pe )
		goto _test_eof898;
case 898:
#line 17756 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st492;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st492:
	if ( ++p == pe )
		goto _test_eof492;
case 492:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st493;
		} else if ( (*p) >= 65 )
			goto st493;
	} else
		goto st249;
	goto tr14;
st493:
	if ( ++p == pe )
		goto _test_eof493;
case 493:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr5;
st494:
	if ( ++p == pe )
		goto _test_eof494;
case 494:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr5;
st495:
	if ( ++p == pe )
		goto _test_eof495;
case 495:
	switch( (*p) ) {
		case 43: goto st249;
		case 64: goto st250;
		case 65: goto st496;
		case 66: goto st499;
		case 67: goto st500;
		case 69: goto st503;
		case 71: goto st504;
		case 73: goto st505;
		case 74: goto st506;
		case 77: goto st508;
		case 78: goto st513;
		case 79: goto st515;
		case 80: goto st516;
		case 84: goto st517;
		case 88: goto st521;
		case 95: goto st249;
		case 97: goto st496;
		case 98: goto st499;
		case 99: goto st500;
		case 101: goto st503;
		case 103: goto st504;
		case 105: goto st505;
		case 106: goto st506;
		case 109: goto st508;
		case 110: goto st513;
		case 111: goto st515;
		case 112: goto st516;
		case 116: goto st517;
		case 120: goto st521;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto st502;
		} else if ( (*p) >= 68 )
			goto st502;
	} else
		goto st249;
	goto tr5;
st496:
	if ( ++p == pe )
		goto _test_eof496;
case 496:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 69: goto tr553;
		case 82: goto tr554;
		case 83: goto tr555;
		case 95: goto st249;
		case 101: goto tr553;
		case 114: goto tr554;
		case 115: goto tr555;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr552:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st899;
st899:
	if ( ++p == pe )
		goto _test_eof899;
case 899:
#line 17933 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
tr553:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st900;
st900:
	if ( ++p == pe )
		goto _test_eof900;
case 900:
#line 17960 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 82: goto st497;
		case 95: goto st249;
		case 114: goto st497;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st497:
	if ( ++p == pe )
		goto _test_eof497;
case 497:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 79: goto tr552;
		case 95: goto st249;
		case 111: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
tr554:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st901;
st901:
	if ( ++p == pe )
		goto _test_eof901;
case 901:
#line 18011 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 80: goto st498;
		case 95: goto st249;
		case 112: goto st498;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st498:
	if ( ++p == pe )
		goto _test_eof498;
case 498:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 65: goto tr552;
		case 95: goto st249;
		case 97: goto tr552;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
tr555:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st902;
st902:
	if ( ++p == pe )
		goto _test_eof902;
case 902:
#line 18062 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 73: goto st498;
		case 95: goto st249;
		case 105: goto st498;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st499:
	if ( ++p == pe )
		goto _test_eof499;
case 499:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 73: goto tr556;
		case 95: goto st249;
		case 105: goto tr556;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr556:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st903;
st903:
	if ( ++p == pe )
		goto _test_eof903;
case 903:
#line 18113 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 90: goto tr552;
		case 95: goto st249;
		case 122: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 89 ) {
		if ( 97 <= (*p) && (*p) <= 121 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st500:
	if ( ++p == pe )
		goto _test_eof500;
case 500:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 65: goto tr557;
		case 79: goto tr558;
		case 95: goto st249;
		case 97: goto tr557;
		case 111: goto tr558;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr557:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st904;
st904:
	if ( ++p == pe )
		goto _test_eof904;
case 904:
#line 18166 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 84: goto tr552;
		case 95: goto st249;
		case 116: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
tr558:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st905;
st905:
	if ( ++p == pe )
		goto _test_eof905;
case 905:
#line 18195 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 77: goto tr552;
		case 79: goto st501;
		case 95: goto st249;
		case 109: goto tr552;
		case 111: goto st501;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st501:
	if ( ++p == pe )
		goto _test_eof501;
case 501:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 80: goto tr552;
		case 95: goto st249;
		case 112: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
st502:
	if ( ++p == pe )
		goto _test_eof502;
case 502:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
st503:
	if ( ++p == pe )
		goto _test_eof503;
case 503:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 68: goto tr559;
		case 95: goto st249;
		case 100: goto tr559;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr559:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st906;
st906:
	if ( ++p == pe )
		goto _test_eof906;
case 906:
#line 18290 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 85: goto tr552;
		case 95: goto st249;
		case 117: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st504:
	if ( ++p == pe )
		goto _test_eof504;
case 504:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 79: goto tr560;
		case 95: goto st249;
		case 111: goto tr560;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr560:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st907;
st907:
	if ( ++p == pe )
		goto _test_eof907;
case 907:
#line 18341 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 86: goto tr552;
		case 95: goto st249;
		case 118: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st505:
	if ( ++p == pe )
		goto _test_eof505;
case 505:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 78: goto tr561;
		case 95: goto st249;
		case 110: goto tr561;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr561:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st908;
st908:
	if ( ++p == pe )
		goto _test_eof908;
case 908:
#line 18392 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 70: goto st497;
		case 84: goto tr552;
		case 95: goto st249;
		case 102: goto st497;
		case 116: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st506:
	if ( ++p == pe )
		goto _test_eof506;
case 506:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 79: goto tr562;
		case 95: goto st249;
		case 111: goto tr562;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr562:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st909;
st909:
	if ( ++p == pe )
		goto _test_eof909;
case 909:
#line 18445 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 66: goto st507;
		case 95: goto st249;
		case 98: goto st507;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st507:
	if ( ++p == pe )
		goto _test_eof507;
case 507:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 83: goto tr552;
		case 95: goto st249;
		case 115: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
st508:
	if ( ++p == pe )
		goto _test_eof508;
case 508:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 73: goto tr563;
		case 79: goto tr564;
		case 85: goto tr565;
		case 95: goto st249;
		case 105: goto tr563;
		case 111: goto tr564;
		case 117: goto tr565;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr563:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st910;
st910:
	if ( ++p == pe )
		goto _test_eof910;
case 910:
#line 18522 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 76: goto tr552;
		case 95: goto st249;
		case 108: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
tr564:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st911;
st911:
	if ( ++p == pe )
		goto _test_eof911;
case 911:
#line 18551 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 66: goto st509;
		case 95: goto st249;
		case 98: goto st509;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st509:
	if ( ++p == pe )
		goto _test_eof509;
case 509:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 73: goto tr552;
		case 95: goto st249;
		case 105: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
tr565:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st912;
st912:
	if ( ++p == pe )
		goto _test_eof912;
case 912:
#line 18602 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 83: goto st510;
		case 95: goto st249;
		case 115: goto st510;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st510:
	if ( ++p == pe )
		goto _test_eof510;
case 510:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 69: goto st511;
		case 95: goto st249;
		case 101: goto st511;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
st511:
	if ( ++p == pe )
		goto _test_eof511;
case 511:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 85: goto st512;
		case 95: goto st249;
		case 117: goto st512;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
st512:
	if ( ++p == pe )
		goto _test_eof512;
case 512:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 77: goto tr552;
		case 95: goto st249;
		case 109: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
st513:
	if ( ++p == pe )
		goto _test_eof513;
case 513:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 65: goto tr568;
		case 69: goto tr557;
		case 95: goto st249;
		case 97: goto tr568;
		case 101: goto tr557;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr568:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st913;
st913:
	if ( ++p == pe )
		goto _test_eof913;
case 913:
#line 18721 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 77: goto st514;
		case 95: goto st249;
		case 109: goto st514;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st514:
	if ( ++p == pe )
		goto _test_eof514;
case 514:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 69: goto tr552;
		case 95: goto st249;
		case 101: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
st515:
	if ( ++p == pe )
		goto _test_eof515;
case 515:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 82: goto tr569;
		case 95: goto st249;
		case 114: goto tr569;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr569:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st914;
st914:
	if ( ++p == pe )
		goto _test_eof914;
case 914:
#line 18794 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 71: goto tr552;
		case 95: goto st249;
		case 103: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st516:
	if ( ++p == pe )
		goto _test_eof516;
case 516:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 82: goto tr570;
		case 95: goto st249;
		case 114: goto tr570;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr570:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st915;
st915:
	if ( ++p == pe )
		goto _test_eof915;
case 915:
#line 18845 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 79: goto tr552;
		case 95: goto st249;
		case 111: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st517:
	if ( ++p == pe )
		goto _test_eof517;
case 517:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 69: goto tr563;
		case 82: goto tr571;
		case 95: goto st249;
		case 101: goto tr563;
		case 114: goto tr571;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr571:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st916;
st916:
	if ( ++p == pe )
		goto _test_eof916;
case 916:
#line 18898 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 65: goto st518;
		case 95: goto st249;
		case 97: goto st518;
	}
	if ( (*p) < 66 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
st518:
	if ( ++p == pe )
		goto _test_eof518;
case 518:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 86: goto st519;
		case 95: goto st249;
		case 118: goto st519;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
st519:
	if ( ++p == pe )
		goto _test_eof519;
case 519:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 69: goto st520;
		case 95: goto st249;
		case 101: goto st520;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
st520:
	if ( ++p == pe )
		goto _test_eof520;
case 520:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 76: goto tr552;
		case 95: goto st249;
		case 108: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr323;
st521:
	if ( ++p == pe )
		goto _test_eof521;
case 521:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 88: goto tr574;
		case 95: goto st249;
		case 120: goto tr574;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr552;
	} else
		goto tr552;
	goto tr5;
tr574:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st917;
st917:
	if ( ++p == pe )
		goto _test_eof917;
case 917:
#line 19015 "tokenizer.cc"
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st494;
		case 46: goto st495;
		case 64: goto st250;
		case 88: goto tr552;
		case 95: goto st249;
		case 120: goto tr552;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st493;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st493;
	} else
		goto st493;
	goto tr927;
tr533:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st918;
st918:
	if ( ++p == pe )
		goto _test_eof918;
case 918:
#line 19044 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st522:
	if ( ++p == pe )
		goto _test_eof522;
case 522:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr575;
		} else if ( (*p) >= 65 )
			goto tr575;
	} else
		goto st249;
	goto tr5;
tr575:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st919;
st919:
	if ( ++p == pe )
		goto _test_eof919;
case 919:
#line 19108 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1152:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st920;
st920:
	if ( ++p == pe )
		goto _test_eof920;
case 920:
#line 19148 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st523;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
st523:
	if ( ++p == pe )
		goto _test_eof523;
case 523:
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st523;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr14;
tr577:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st921;
tr1155:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st921;
st921:
	if ( ++p == pe )
		goto _test_eof921;
case 921:
#line 19227 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr5;
st524:
	if ( ++p == pe )
		goto _test_eof524;
case 524:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 65: goto tr579;
		case 66: goto tr580;
		case 67: goto tr581;
		case 69: goto tr583;
		case 71: goto tr584;
		case 73: goto tr585;
		case 74: goto tr586;
		case 77: goto tr587;
		case 78: goto tr588;
		case 79: goto tr589;
		case 80: goto tr590;
		case 84: goto tr591;
		case 88: goto tr592;
		case 95: goto st249;
		case 97: goto tr579;
		case 98: goto tr580;
		case 99: goto tr581;
		case 101: goto tr583;
		case 103: goto tr584;
		case 105: goto tr585;
		case 106: goto tr586;
		case 109: goto tr587;
		case 110: goto tr588;
		case 111: goto tr589;
		case 112: goto tr590;
		case 116: goto tr591;
		case 120: goto tr592;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto tr582;
		} else if ( (*p) >= 68 )
			goto tr582;
	} else
		goto st249;
	goto tr5;
tr579:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st922;
st922:
	if ( ++p == pe )
		goto _test_eof922;
case 922:
#line 19317 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 69: goto tr1156;
		case 82: goto tr1157;
		case 83: goto tr1158;
		case 95: goto st249;
		case 101: goto tr1160;
		case 114: goto tr1161;
		case 115: goto tr1162;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
st525:
	if ( ++p == pe )
		goto _test_eof525;
case 525:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 65: goto st496;
		case 66: goto st499;
		case 67: goto st500;
		case 69: goto st503;
		case 71: goto st504;
		case 73: goto st505;
		case 74: goto st506;
		case 77: goto st508;
		case 78: goto st513;
		case 79: goto st515;
		case 80: goto st516;
		case 84: goto st517;
		case 88: goto st521;
		case 95: goto st249;
		case 97: goto st496;
		case 98: goto st499;
		case 99: goto st500;
		case 101: goto st503;
		case 103: goto st504;
		case 105: goto st505;
		case 106: goto st506;
		case 109: goto st508;
		case 110: goto st513;
		case 111: goto st515;
		case 112: goto st516;
		case 116: goto st517;
		case 120: goto st521;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto st502;
		} else if ( (*p) >= 68 )
			goto st502;
	} else
		goto st249;
	goto tr14;
tr1156:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st923;
st923:
	if ( ++p == pe )
		goto _test_eof923;
case 923:
#line 19413 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 82: goto tr1163;
		case 95: goto st249;
		case 114: goto tr1164;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1163:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st924;
tr1205:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st924;
st924:
	if ( ++p == pe )
		goto _test_eof924;
case 924:
#line 19461 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 79: goto tr1155;
		case 95: goto st249;
		case 111: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr5;
tr578:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st925;
tr1159:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st925;
st925:
	if ( ++p == pe )
		goto _test_eof925;
case 925:
#line 19509 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr5;
tr1164:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st926;
tr1206:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st926;
st926:
	if ( ++p == pe )
		goto _test_eof926;
case 926:
#line 19555 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 79: goto tr1155;
		case 95: goto st249;
		case 111: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr5;
tr1157:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st927;
st927:
	if ( ++p == pe )
		goto _test_eof927;
case 927:
#line 19597 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 80: goto tr1165;
		case 95: goto st249;
		case 112: goto tr1166;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1165:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st928;
st928:
	if ( ++p == pe )
		goto _test_eof928;
case 928:
#line 19639 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 65: goto tr1155;
		case 95: goto st249;
		case 97: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1166:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st929;
st929:
	if ( ++p == pe )
		goto _test_eof929;
case 929:
#line 19681 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 65: goto tr1155;
		case 95: goto st249;
		case 97: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1158:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st930;
st930:
	if ( ++p == pe )
		goto _test_eof930;
case 930:
#line 19723 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 73: goto tr1165;
		case 95: goto st249;
		case 105: goto tr1166;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1160:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st931;
st931:
	if ( ++p == pe )
		goto _test_eof931;
case 931:
#line 19765 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 82: goto tr1163;
		case 95: goto st249;
		case 114: goto tr1164;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1161:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st932;
st932:
	if ( ++p == pe )
		goto _test_eof932;
case 932:
#line 19807 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 80: goto tr1165;
		case 95: goto st249;
		case 112: goto tr1166;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1162:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st933;
st933:
	if ( ++p == pe )
		goto _test_eof933;
case 933:
#line 19849 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 73: goto tr1165;
		case 95: goto st249;
		case 105: goto tr1166;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr580:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st934;
st934:
	if ( ++p == pe )
		goto _test_eof934;
case 934:
#line 19891 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 73: goto tr1167;
		case 95: goto st249;
		case 105: goto tr1168;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1167:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st935;
st935:
	if ( ++p == pe )
		goto _test_eof935;
case 935:
#line 19933 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 90: goto tr1155;
		case 95: goto st249;
		case 122: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 89 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1168:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st936;
st936:
	if ( ++p == pe )
		goto _test_eof936;
case 936:
#line 19975 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 90: goto tr1155;
		case 95: goto st249;
		case 122: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 89 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr581:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st937;
st937:
	if ( ++p == pe )
		goto _test_eof937;
case 937:
#line 20017 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 65: goto tr1169;
		case 79: goto tr1170;
		case 95: goto st249;
		case 97: goto tr1171;
		case 111: goto tr1172;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1169:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st938;
st938:
	if ( ++p == pe )
		goto _test_eof938;
case 938:
#line 20061 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 84: goto tr1155;
		case 95: goto st249;
		case 116: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1170:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st939;
st939:
	if ( ++p == pe )
		goto _test_eof939;
case 939:
#line 20103 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 77: goto tr1155;
		case 79: goto tr1173;
		case 95: goto st249;
		case 109: goto tr1159;
		case 111: goto tr1174;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1173:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st940;
st940:
	if ( ++p == pe )
		goto _test_eof940;
case 940:
#line 20147 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 80: goto tr1155;
		case 95: goto st249;
		case 112: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1174:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st941;
st941:
	if ( ++p == pe )
		goto _test_eof941;
case 941:
#line 20189 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 80: goto tr1155;
		case 95: goto st249;
		case 112: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1171:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st942;
st942:
	if ( ++p == pe )
		goto _test_eof942;
case 942:
#line 20231 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 84: goto tr1155;
		case 95: goto st249;
		case 116: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1172:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st943;
st943:
	if ( ++p == pe )
		goto _test_eof943;
case 943:
#line 20273 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 77: goto tr1155;
		case 79: goto tr1173;
		case 95: goto st249;
		case 109: goto tr1159;
		case 111: goto tr1174;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr582:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st944;
st944:
	if ( ++p == pe )
		goto _test_eof944;
case 944:
#line 20317 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr583:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st945;
st945:
	if ( ++p == pe )
		goto _test_eof945;
case 945:
#line 20357 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 68: goto tr1175;
		case 95: goto st249;
		case 100: goto tr1176;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1175:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st946;
st946:
	if ( ++p == pe )
		goto _test_eof946;
case 946:
#line 20399 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 85: goto tr1155;
		case 95: goto st249;
		case 117: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1176:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st947;
st947:
	if ( ++p == pe )
		goto _test_eof947;
case 947:
#line 20441 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 85: goto tr1155;
		case 95: goto st249;
		case 117: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr584:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st948;
st948:
	if ( ++p == pe )
		goto _test_eof948;
case 948:
#line 20483 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 79: goto tr1177;
		case 95: goto st249;
		case 111: goto tr1178;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1177:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st949;
st949:
	if ( ++p == pe )
		goto _test_eof949;
case 949:
#line 20525 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 86: goto tr1155;
		case 95: goto st249;
		case 118: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1178:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st950;
st950:
	if ( ++p == pe )
		goto _test_eof950;
case 950:
#line 20567 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 86: goto tr1155;
		case 95: goto st249;
		case 118: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr585:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st951;
st951:
	if ( ++p == pe )
		goto _test_eof951;
case 951:
#line 20609 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 78: goto tr1179;
		case 95: goto st249;
		case 110: goto tr1180;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1179:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st952;
st952:
	if ( ++p == pe )
		goto _test_eof952;
case 952:
#line 20651 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 70: goto tr1163;
		case 84: goto tr1155;
		case 95: goto st249;
		case 102: goto tr1164;
		case 116: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1180:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st953;
st953:
	if ( ++p == pe )
		goto _test_eof953;
case 953:
#line 20695 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 70: goto tr1163;
		case 84: goto tr1155;
		case 95: goto st249;
		case 102: goto tr1164;
		case 116: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr586:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st954;
st954:
	if ( ++p == pe )
		goto _test_eof954;
case 954:
#line 20739 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 79: goto tr1181;
		case 95: goto st249;
		case 111: goto tr1182;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1181:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st955;
st955:
	if ( ++p == pe )
		goto _test_eof955;
case 955:
#line 20781 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 66: goto tr1183;
		case 95: goto st249;
		case 98: goto tr1184;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1183:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st956;
st956:
	if ( ++p == pe )
		goto _test_eof956;
case 956:
#line 20823 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 83: goto tr1155;
		case 95: goto st249;
		case 115: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1184:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st957;
st957:
	if ( ++p == pe )
		goto _test_eof957;
case 957:
#line 20865 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 83: goto tr1155;
		case 95: goto st249;
		case 115: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1182:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st958;
st958:
	if ( ++p == pe )
		goto _test_eof958;
case 958:
#line 20907 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 66: goto tr1183;
		case 95: goto st249;
		case 98: goto tr1184;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr587:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st959;
st959:
	if ( ++p == pe )
		goto _test_eof959;
case 959:
#line 20949 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 73: goto tr1185;
		case 79: goto tr1186;
		case 85: goto tr1187;
		case 95: goto st249;
		case 105: goto tr1188;
		case 111: goto tr1189;
		case 117: goto tr1190;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1213:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st960;
tr1185:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st960;
st960:
	if ( ++p == pe )
		goto _test_eof960;
case 960:
#line 21001 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 76: goto tr1155;
		case 95: goto st249;
		case 108: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr5;
tr1186:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st961;
st961:
	if ( ++p == pe )
		goto _test_eof961;
case 961:
#line 21043 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 66: goto tr1191;
		case 95: goto st249;
		case 98: goto tr1192;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1191:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st962;
st962:
	if ( ++p == pe )
		goto _test_eof962;
case 962:
#line 21085 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 73: goto tr1155;
		case 95: goto st249;
		case 105: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1192:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st963;
st963:
	if ( ++p == pe )
		goto _test_eof963;
case 963:
#line 21127 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 73: goto tr1155;
		case 95: goto st249;
		case 105: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1187:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st964;
st964:
	if ( ++p == pe )
		goto _test_eof964;
case 964:
#line 21169 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 83: goto tr1193;
		case 95: goto st249;
		case 115: goto tr1194;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1193:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st965;
st965:
	if ( ++p == pe )
		goto _test_eof965;
case 965:
#line 21211 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 69: goto tr1195;
		case 95: goto st249;
		case 101: goto tr1196;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1195:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st966;
st966:
	if ( ++p == pe )
		goto _test_eof966;
case 966:
#line 21253 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 85: goto tr1197;
		case 95: goto st249;
		case 117: goto tr1198;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1197:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st967;
st967:
	if ( ++p == pe )
		goto _test_eof967;
case 967:
#line 21295 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 77: goto tr1155;
		case 95: goto st249;
		case 109: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1198:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st968;
st968:
	if ( ++p == pe )
		goto _test_eof968;
case 968:
#line 21337 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 77: goto tr1155;
		case 95: goto st249;
		case 109: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1196:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st969;
st969:
	if ( ++p == pe )
		goto _test_eof969;
case 969:
#line 21379 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 85: goto tr1197;
		case 95: goto st249;
		case 117: goto tr1198;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1194:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st970;
st970:
	if ( ++p == pe )
		goto _test_eof970;
case 970:
#line 21421 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 69: goto tr1195;
		case 95: goto st249;
		case 101: goto tr1196;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1214:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st971;
tr1188:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st971;
st971:
	if ( ++p == pe )
		goto _test_eof971;
case 971:
#line 21469 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 76: goto tr1155;
		case 95: goto st249;
		case 108: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr5;
tr1189:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st972;
st972:
	if ( ++p == pe )
		goto _test_eof972;
case 972:
#line 21511 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 66: goto tr1191;
		case 95: goto st249;
		case 98: goto tr1192;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1190:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st973;
st973:
	if ( ++p == pe )
		goto _test_eof973;
case 973:
#line 21553 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 83: goto tr1193;
		case 95: goto st249;
		case 115: goto tr1194;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr588:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st974;
st974:
	if ( ++p == pe )
		goto _test_eof974;
case 974:
#line 21595 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 65: goto tr1199;
		case 69: goto tr1169;
		case 95: goto st249;
		case 97: goto tr1200;
		case 101: goto tr1171;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1199:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st975;
st975:
	if ( ++p == pe )
		goto _test_eof975;
case 975:
#line 21639 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 77: goto tr1201;
		case 95: goto st249;
		case 109: goto tr1202;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1201:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st976;
st976:
	if ( ++p == pe )
		goto _test_eof976;
case 976:
#line 21681 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 69: goto tr1155;
		case 95: goto st249;
		case 101: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1202:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st977;
st977:
	if ( ++p == pe )
		goto _test_eof977;
case 977:
#line 21723 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 69: goto tr1155;
		case 95: goto st249;
		case 101: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1200:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st978;
st978:
	if ( ++p == pe )
		goto _test_eof978;
case 978:
#line 21765 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 77: goto tr1201;
		case 95: goto st249;
		case 109: goto tr1202;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr589:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st979;
st979:
	if ( ++p == pe )
		goto _test_eof979;
case 979:
#line 21807 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 82: goto tr1203;
		case 95: goto st249;
		case 114: goto tr1204;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1203:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st980;
st980:
	if ( ++p == pe )
		goto _test_eof980;
case 980:
#line 21849 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 71: goto tr1155;
		case 95: goto st249;
		case 103: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1204:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st981;
st981:
	if ( ++p == pe )
		goto _test_eof981;
case 981:
#line 21891 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 71: goto tr1155;
		case 95: goto st249;
		case 103: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr590:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st982;
st982:
	if ( ++p == pe )
		goto _test_eof982;
case 982:
#line 21933 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 82: goto tr1205;
		case 95: goto st249;
		case 114: goto tr1206;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr591:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st983;
st983:
	if ( ++p == pe )
		goto _test_eof983;
case 983:
#line 21975 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 69: goto tr1185;
		case 82: goto tr1207;
		case 95: goto st249;
		case 101: goto tr1188;
		case 114: goto tr1208;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1207:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st984;
st984:
	if ( ++p == pe )
		goto _test_eof984;
case 984:
#line 22019 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 65: goto tr1209;
		case 95: goto st249;
		case 97: goto tr1210;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1209:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st985;
st985:
	if ( ++p == pe )
		goto _test_eof985;
case 985:
#line 22061 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 86: goto tr1211;
		case 95: goto st249;
		case 118: goto tr1212;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1211:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st986;
st986:
	if ( ++p == pe )
		goto _test_eof986;
case 986:
#line 22103 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 69: goto tr1213;
		case 95: goto st249;
		case 101: goto tr1214;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1212:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st987;
st987:
	if ( ++p == pe )
		goto _test_eof987;
case 987:
#line 22145 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 69: goto tr1213;
		case 95: goto st249;
		case 101: goto tr1214;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1210:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st988;
st988:
	if ( ++p == pe )
		goto _test_eof988;
case 988:
#line 22187 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 86: goto tr1211;
		case 95: goto st249;
		case 118: goto tr1212;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1208:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st989;
st989:
	if ( ++p == pe )
		goto _test_eof989;
case 989:
#line 22229 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 65: goto tr1209;
		case 95: goto st249;
		case 97: goto tr1210;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr592:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st990;
st990:
	if ( ++p == pe )
		goto _test_eof990;
case 990:
#line 22271 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st525;
		case 64: goto st250;
		case 88: goto tr1215;
		case 95: goto st249;
		case 120: goto tr1216;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr1159;
		} else
			goto tr1155;
	} else
		goto st9;
	goto tr763;
tr1215:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st991;
st991:
	if ( ++p == pe )
		goto _test_eof991;
case 991:
#line 22313 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 88: goto tr1155;
		case 95: goto st249;
		case 120: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr1216:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 38;}
	goto st992;
st992:
	if ( ++p == pe )
		goto _test_eof992;
case 992:
#line 22355 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto st524;
		case 64: goto st250;
		case 88: goto tr1155;
		case 95: goto st249;
		case 120: goto tr1159;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr927;
tr534:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st993;
tr1242:
#line 1 "NONE"
	{ s.te = p+1;}
#line 30 "rules/contractions.rl"
	{n2="n't";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 43 "rules/main.rl"
	{ s.act = 23;}
	goto st993;
st993:
	if ( ++p == pe )
		goto _test_eof993;
case 993:
#line 22407 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr5;
tr1125:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st994;
st994:
	if ( ++p == pe )
		goto _test_eof994;
case 994:
#line 22447 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 77: goto tr678;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr677;
		} else if ( (*p) >= 65 )
			goto tr677;
	} else
		goto st249;
	goto tr1139;
tr677:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st995;
st995:
	if ( ++p == pe )
		goto _test_eof995;
case 995:
#line 22479 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto tr1217;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1217:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st996;
tr1218:
#line 1 "NONE"
	{ s.te = p+1;}
#line 54 "rules/main.rl"
	{n1="am";}
#line 7 "rules/main.rl"
	{ s.act = 30;}
	goto st996;
tr1465:
#line 1 "NONE"
	{ s.te = p+1;}
#line 54 "rules/main.rl"
	{n1="pm";}
#line 7 "rules/main.rl"
	{ s.act = 30;}
	goto st996;
st996:
	if ( ++p == pe )
		goto _test_eof996;
case 996:
#line 22535 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 65: goto st496;
		case 66: goto st499;
		case 67: goto st500;
		case 69: goto st503;
		case 71: goto st504;
		case 73: goto st505;
		case 74: goto st506;
		case 77: goto st508;
		case 78: goto st513;
		case 79: goto st515;
		case 80: goto st516;
		case 84: goto st517;
		case 88: goto st521;
		case 95: goto st249;
		case 97: goto st496;
		case 98: goto st499;
		case 99: goto st500;
		case 101: goto st503;
		case 103: goto st504;
		case 105: goto st505;
		case 106: goto st506;
		case 109: goto st508;
		case 110: goto st513;
		case 111: goto st515;
		case 112: goto st516;
		case 116: goto st517;
		case 120: goto st521;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 100 <= (*p) && (*p) <= 122 )
				goto st502;
		} else if ( (*p) >= 68 )
			goto st502;
	} else
		goto st249;
	goto tr5;
tr678:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st997;
st997:
	if ( ++p == pe )
		goto _test_eof997;
case 997:
#line 22592 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto tr1218;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1126:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st998;
tr1225:
#line 1 "NONE"
	{ s.te = p+1;}
#line 31 "rules/contractions.rl"
	{n2="N'T";}
#line 7 "actions.rl"
	{ s.suffix += 2; }
#line 44 "rules/main.rl"
	{ s.act = 24;}
	goto st998;
st998:
	if ( ++p == pe )
		goto _test_eof998;
case 998:
#line 22642 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr5;
tr1127:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st999;
st999:
	if ( ++p == pe )
		goto _test_eof999;
case 999:
#line 22682 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1219;
		case 78: goto tr1220;
		case 95: goto st249;
		case 109: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1219:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1000;
st1000:
	if ( ++p == pe )
		goto _test_eof1000;
case 1000:
#line 22725 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st526;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
st526:
	if ( ++p == pe )
		goto _test_eof526;
case 526:
	switch( (*p) ) {
		case 35: goto st1001;
		case 93: goto st1001;
		case 95: goto st1001;
		case 126: goto st1001;
	}
	if ( (*p) < 63 ) {
		if ( 45 <= (*p) && (*p) <= 58 )
			goto st1001;
	} else if ( (*p) > 91 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1001;
	} else
		goto st1001;
	goto tr5;
st1001:
	if ( ++p == pe )
		goto _test_eof1001;
case 1001:
	switch( (*p) ) {
		case 35: goto st1001;
		case 93: goto st1001;
		case 95: goto st1001;
		case 126: goto st1001;
	}
	if ( (*p) < 63 ) {
		if ( 45 <= (*p) && (*p) <= 58 )
			goto st1001;
	} else if ( (*p) > 91 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st1001;
	} else
		goto st1001;
	goto tr927;
tr1220:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1002;
st1002:
	if ( ++p == pe )
		goto _test_eof1002;
case 1002:
#line 22804 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st527;
		case 38: goto st530;
		case 39: goto tr1224;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1225;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
st527:
	if ( ++p == pe )
		goto _test_eof527;
case 527:
	switch( (*p) ) {
		case -128: goto st528;
		case -126: goto st7;
		case -120: goto st8;
	}
	if ( -127 <= (*p) && (*p) <= -65 )
		goto st3;
	goto tr530;
st528:
	if ( ++p == pe )
		goto _test_eof528;
case 528:
	switch( (*p) ) {
		case -103: goto tr595;
		case -102: goto tr0;
		case -98: goto tr0;
	}
	if ( (*p) < -106 ) {
		if ( (*p) <= -119 )
			goto tr0;
	} else if ( (*p) > -105 ) {
		if ( -96 <= (*p) && (*p) <= -65 )
			goto tr0;
	} else
		goto tr0;
	goto tr530;
tr595:
#line 8 "actions.rl"
	{ s.suffix += 3; }
	goto st529;
tr604:
#line 10 "actions.rl"
	{ s.suffix += 5; }
	goto st529;
tr608:
#line 12 "actions.rl"
	{ s.suffix += 7; }
	goto st529;
tr613:
#line 13 "actions.rl"
	{ s.suffix += 8; }
	goto st529;
tr617:
#line 11 "actions.rl"
	{ s.suffix += 6; }
	goto st529;
st529:
	if ( ++p == pe )
		goto _test_eof529;
case 529:
#line 22889 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr171;
		case 76: goto st172;
		case 77: goto tr173;
		case 82: goto st173;
		case 83: goto tr175;
		case 84: goto tr596;
		case 86: goto st174;
		case 100: goto tr177;
		case 108: goto st175;
		case 109: goto tr179;
		case 114: goto st176;
		case 115: goto tr175;
		case 118: goto st177;
	}
	goto tr530;
st530:
	if ( ++p == pe )
		goto _test_eof530;
case 530:
	switch( (*p) ) {
		case 35: goto st531;
		case 38: goto st488;
		case 97: goto st543;
		case 114: goto st547;
	}
	if ( 65 <= (*p) && (*p) <= 90 )
		goto tr528;
	goto tr530;
st531:
	if ( ++p == pe )
		goto _test_eof531;
case 531:
	switch( (*p) ) {
		case 51: goto st532;
		case 56: goto st534;
		case 88: goto st538;
		case 120: goto st538;
	}
	goto tr530;
st532:
	if ( ++p == pe )
		goto _test_eof532;
case 532:
	if ( (*p) == 57 )
		goto st533;
	goto tr530;
st533:
	if ( ++p == pe )
		goto _test_eof533;
case 533:
	if ( (*p) == 59 )
		goto tr604;
	goto tr530;
st534:
	if ( ++p == pe )
		goto _test_eof534;
case 534:
	if ( (*p) == 50 )
		goto st535;
	goto tr530;
st535:
	if ( ++p == pe )
		goto _test_eof535;
case 535:
	if ( (*p) == 49 )
		goto st536;
	goto tr530;
st536:
	if ( ++p == pe )
		goto _test_eof536;
case 536:
	if ( (*p) == 55 )
		goto st537;
	goto tr530;
st537:
	if ( ++p == pe )
		goto _test_eof537;
case 537:
	if ( (*p) == 59 )
		goto tr608;
	goto tr530;
st538:
	if ( ++p == pe )
		goto _test_eof538;
case 538:
	if ( (*p) == 50 )
		goto st539;
	goto tr530;
st539:
	if ( ++p == pe )
		goto _test_eof539;
case 539:
	if ( (*p) == 48 )
		goto st540;
	goto tr530;
st540:
	if ( ++p == pe )
		goto _test_eof540;
case 540:
	if ( (*p) == 49 )
		goto st541;
	goto tr530;
st541:
	if ( ++p == pe )
		goto _test_eof541;
case 541:
	if ( (*p) == 57 )
		goto st542;
	goto tr530;
st542:
	if ( ++p == pe )
		goto _test_eof542;
case 542:
	if ( (*p) == 59 )
		goto tr613;
	goto tr530;
st543:
	if ( ++p == pe )
		goto _test_eof543;
case 543:
	switch( (*p) ) {
		case 109: goto st167;
		case 112: goto st544;
	}
	goto tr530;
st544:
	if ( ++p == pe )
		goto _test_eof544;
case 544:
	if ( (*p) == 111 )
		goto st545;
	goto tr530;
st545:
	if ( ++p == pe )
		goto _test_eof545;
case 545:
	if ( (*p) == 115 )
		goto st546;
	goto tr530;
st546:
	if ( ++p == pe )
		goto _test_eof546;
case 546:
	if ( (*p) == 59 )
		goto tr617;
	goto tr530;
st547:
	if ( ++p == pe )
		goto _test_eof547;
case 547:
	if ( (*p) == 115 )
		goto st548;
	goto tr530;
st548:
	if ( ++p == pe )
		goto _test_eof548;
case 548:
	if ( (*p) == 113 )
		goto st549;
	goto tr530;
st549:
	if ( ++p == pe )
		goto _test_eof549;
case 549:
	if ( (*p) == 117 )
		goto st550;
	goto tr530;
st550:
	if ( ++p == pe )
		goto _test_eof550;
case 550:
	if ( (*p) == 111 )
		goto st537;
	goto tr530;
tr1224:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st551;
st551:
	if ( ++p == pe )
		goto _test_eof551;
case 551:
#line 23073 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 84: goto tr621;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr530;
tr1221:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1003;
st1003:
	if ( ++p == pe )
		goto _test_eof1003;
case 1003:
#line 23105 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st526;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1128:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1004;
st1004:
	if ( ++p == pe )
		goto _test_eof1004;
case 1004:
#line 23146 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 68: goto tr1226;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1226:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1005;
st1005:
	if ( ++p == pe )
		goto _test_eof1005;
case 1005:
#line 23187 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 47: goto st552;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
st552:
	if ( ++p == pe )
		goto _test_eof552;
case 552:
	if ( (*p) == 79 )
		goto st553;
	goto tr530;
st553:
	if ( ++p == pe )
		goto _test_eof553;
case 553:
	if ( (*p) == 82 )
		goto tr623;
	goto tr530;
tr1129:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1006;
st1006:
	if ( ++p == pe )
		goto _test_eof1006;
case 1006:
#line 23242 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1228;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1228:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1007;
st1007:
	if ( ++p == pe )
		goto _test_eof1007;
case 1007:
#line 23283 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 78: goto tr1220;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1130:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1008;
st1008:
	if ( ++p == pe )
		goto _test_eof1008;
case 1008:
#line 23324 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st554:
	if ( ++p == pe )
		goto _test_eof554;
case 554:
	switch( (*p) ) {
		case 43: goto st249;
		case 45: goto st249;
		case 46: goto tr624;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st249;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st249;
	} else
		goto st249;
	goto tr14;
tr1131:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1009;
st1009:
	if ( ++p == pe )
		goto _test_eof1009;
case 1009:
#line 23384 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1230;
		case 109: goto tr1231;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1230:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1010;
st1010:
	if ( ++p == pe )
		goto _test_eof1010;
case 1010:
#line 23426 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1232;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1232:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st555;
st555:
	if ( ++p == pe )
		goto _test_eof555;
case 555:
#line 23464 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr625;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr1231:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1011;
st1011:
	if ( ++p == pe )
		goto _test_eof1011;
case 1011:
#line 23495 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1234;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1236:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 37;}
	goto st1012;
tr1233:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1012;
tr1244:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 36;}
	goto st1012;
st1012:
	if ( ++p == pe )
		goto _test_eof1012;
case 1012:
#line 23548 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr575;
		} else if ( (*p) >= 65 )
			goto tr575;
	} else
		goto st249;
	goto tr5;
tr1234:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1013;
st1013:
	if ( ++p == pe )
		goto _test_eof1013;
case 1013:
#line 23579 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1235:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1014;
st1014:
	if ( ++p == pe )
		goto _test_eof1014;
case 1014:
#line 23620 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1236;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1132:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1015;
st1015:
	if ( ++p == pe )
		goto _test_eof1015;
case 1015:
#line 23660 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1237;
		case 95: goto st249;
		case 109: goto tr1221;
		case 110: goto tr1238;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1237:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1016;
st1016:
	if ( ++p == pe )
		goto _test_eof1016;
case 1016:
#line 23703 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st526;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1238:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1017;
st1017:
	if ( ++p == pe )
		goto _test_eof1017;
case 1017:
#line 23744 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st556;
		case 38: goto st559;
		case 39: goto tr1241;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1242;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st556:
	if ( ++p == pe )
		goto _test_eof556;
case 556:
	switch( (*p) ) {
		case -128: goto st557;
		case -126: goto st7;
		case -120: goto st8;
	}
	if ( -127 <= (*p) && (*p) <= -65 )
		goto st3;
	goto tr14;
st557:
	if ( ++p == pe )
		goto _test_eof557;
case 557:
	switch( (*p) ) {
		case -103: goto tr627;
		case -102: goto tr0;
		case -98: goto tr0;
	}
	if ( (*p) < -106 ) {
		if ( (*p) <= -119 )
			goto tr0;
	} else if ( (*p) > -105 ) {
		if ( -96 <= (*p) && (*p) <= -65 )
			goto tr0;
	} else
		goto tr0;
	goto tr14;
tr627:
#line 8 "actions.rl"
	{ s.suffix += 3; }
	goto st558;
tr636:
#line 10 "actions.rl"
	{ s.suffix += 5; }
	goto st558;
tr640:
#line 12 "actions.rl"
	{ s.suffix += 7; }
	goto st558;
tr645:
#line 13 "actions.rl"
	{ s.suffix += 8; }
	goto st558;
tr649:
#line 11 "actions.rl"
	{ s.suffix += 6; }
	goto st558;
st558:
	if ( ++p == pe )
		goto _test_eof558;
case 558:
#line 23829 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr171;
		case 76: goto st172;
		case 77: goto tr173;
		case 82: goto st173;
		case 83: goto tr175;
		case 86: goto st174;
		case 100: goto tr177;
		case 108: goto st175;
		case 109: goto tr179;
		case 114: goto st176;
		case 115: goto tr175;
		case 116: goto tr628;
		case 118: goto st177;
	}
	goto tr14;
st559:
	if ( ++p == pe )
		goto _test_eof559;
case 559:
	switch( (*p) ) {
		case 35: goto st560;
		case 97: goto st572;
		case 114: goto st576;
	}
	goto tr14;
st560:
	if ( ++p == pe )
		goto _test_eof560;
case 560:
	switch( (*p) ) {
		case 51: goto st561;
		case 56: goto st563;
		case 88: goto st567;
		case 120: goto st567;
	}
	goto tr14;
st561:
	if ( ++p == pe )
		goto _test_eof561;
case 561:
	if ( (*p) == 57 )
		goto st562;
	goto tr14;
st562:
	if ( ++p == pe )
		goto _test_eof562;
case 562:
	if ( (*p) == 59 )
		goto tr636;
	goto tr14;
st563:
	if ( ++p == pe )
		goto _test_eof563;
case 563:
	if ( (*p) == 50 )
		goto st564;
	goto tr14;
st564:
	if ( ++p == pe )
		goto _test_eof564;
case 564:
	if ( (*p) == 49 )
		goto st565;
	goto tr14;
st565:
	if ( ++p == pe )
		goto _test_eof565;
case 565:
	if ( (*p) == 55 )
		goto st566;
	goto tr14;
st566:
	if ( ++p == pe )
		goto _test_eof566;
case 566:
	if ( (*p) == 59 )
		goto tr640;
	goto tr14;
st567:
	if ( ++p == pe )
		goto _test_eof567;
case 567:
	if ( (*p) == 50 )
		goto st568;
	goto tr14;
st568:
	if ( ++p == pe )
		goto _test_eof568;
case 568:
	if ( (*p) == 48 )
		goto st569;
	goto tr14;
st569:
	if ( ++p == pe )
		goto _test_eof569;
case 569:
	if ( (*p) == 49 )
		goto st570;
	goto tr14;
st570:
	if ( ++p == pe )
		goto _test_eof570;
case 570:
	if ( (*p) == 57 )
		goto st571;
	goto tr14;
st571:
	if ( ++p == pe )
		goto _test_eof571;
case 571:
	if ( (*p) == 59 )
		goto tr645;
	goto tr14;
st572:
	if ( ++p == pe )
		goto _test_eof572;
case 572:
	switch( (*p) ) {
		case 109: goto st167;
		case 112: goto st573;
	}
	goto tr14;
st573:
	if ( ++p == pe )
		goto _test_eof573;
case 573:
	if ( (*p) == 111 )
		goto st574;
	goto tr14;
st574:
	if ( ++p == pe )
		goto _test_eof574;
case 574:
	if ( (*p) == 115 )
		goto st575;
	goto tr14;
st575:
	if ( ++p == pe )
		goto _test_eof575;
case 575:
	if ( (*p) == 59 )
		goto tr649;
	goto tr14;
st576:
	if ( ++p == pe )
		goto _test_eof576;
case 576:
	if ( (*p) == 115 )
		goto st577;
	goto tr14;
st577:
	if ( ++p == pe )
		goto _test_eof577;
case 577:
	if ( (*p) == 113 )
		goto st578;
	goto tr14;
st578:
	if ( ++p == pe )
		goto _test_eof578;
case 578:
	if ( (*p) == 117 )
		goto st579;
	goto tr14;
st579:
	if ( ++p == pe )
		goto _test_eof579;
case 579:
	if ( (*p) == 111 )
		goto st566;
	goto tr14;
tr1241:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st580;
st580:
	if ( ++p == pe )
		goto _test_eof580;
case 580:
#line 24010 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 116: goto tr653;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr1133:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1018;
st1018:
	if ( ++p == pe )
		goto _test_eof1018;
case 1018:
#line 24042 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1243;
		case 121: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1243:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1019;
st1019:
	if ( ++p == pe )
		goto _test_eof1019;
case 1019:
#line 24084 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1134:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1020;
st1020:
	if ( ++p == pe )
		goto _test_eof1020;
case 1020:
#line 24124 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1243;
		case 116: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1135:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1021;
st1021:
	if ( ++p == pe )
		goto _test_eof1021;
case 1021:
#line 24166 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1245;
		case 105: goto tr1246;
		case 107: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1245:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1022;
st1022:
	if ( ++p == pe )
		goto _test_eof1022;
case 1022:
#line 24209 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1238;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1246:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1023;
st1023:
	if ( ++p == pe )
		goto _test_eof1023;
case 1023:
#line 24250 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 122: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1136:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1024;
st1024:
	if ( ++p == pe )
		goto _test_eof1024;
case 1024:
#line 24291 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1247;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1247:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1025;
st1025:
	if ( ++p == pe )
		goto _test_eof1025;
case 1025:
#line 24332 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1235;
		case 111: goto tr1248;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1248:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1026;
st1026:
	if ( ++p == pe )
		goto _test_eof1026;
case 1026:
#line 24374 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1249:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1027;
st1027:
	if ( ++p == pe )
		goto _test_eof1027;
case 1027:
#line 24415 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1236;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1137:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1028;
st1028:
	if ( ++p == pe )
		goto _test_eof1028;
case 1028:
#line 24456 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 103: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1138:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1029;
st1029:
	if ( ++p == pe )
		goto _test_eof1029;
case 1029:
#line 24497 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr714:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1030;
st1030:
	if ( ++p == pe )
		goto _test_eof1030;
case 1030:
#line 24538 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 95: goto st249;
		case 98: goto tr1130;
		case 99: goto tr1251;
		case 100: goto tr1130;
		case 105: goto tr534;
		case 108: goto tr1252;
		case 111: goto tr1253;
		case 114: goto tr1254;
		case 115: goto tr1255;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr1126;
		} else if ( (*p) > 101 ) {
			if ( 102 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr534;
	} else
		goto tr532;
	goto tr763;
tr1250:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1031;
st1031:
	if ( ++p == pe )
		goto _test_eof1031;
case 1031:
#line 24592 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr677;
		} else if ( (*p) >= 65 )
			goto tr677;
	} else
		goto st249;
	goto tr1139;
tr1251:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1032;
st1032:
	if ( ++p == pe )
		goto _test_eof1032;
case 1032:
#line 24623 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 104: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1252:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1033;
st1033:
	if ( ++p == pe )
		goto _test_eof1033;
case 1033:
#line 24664 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1137;
		case 118: goto tr1256;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1256:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1034;
st1034:
	if ( ++p == pe )
		goto _test_eof1034;
case 1034:
#line 24706 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1253:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1035;
st1035:
	if ( ++p == pe )
		goto _test_eof1035;
case 1035:
#line 24747 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 117: goto tr1257;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1257:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1036;
st1036:
	if ( ++p == pe )
		goto _test_eof1036;
case 1036:
#line 24788 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1254:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1037;
st1037:
	if ( ++p == pe )
		goto _test_eof1037;
case 1037:
#line 24829 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1258;
		case 111: goto tr1259;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1258:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1038;
st1038:
	if ( ++p == pe )
		goto _test_eof1038;
case 1038:
#line 24871 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 103: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1260:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1039;
st1039:
	if ( ++p == pe )
		goto _test_eof1039;
case 1039:
#line 24912 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1259:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1040;
st1040:
	if ( ++p == pe )
		goto _test_eof1040;
case 1040:
#line 24952 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1255:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1041;
st1041:
	if ( ++p == pe )
		goto _test_eof1041;
case 1041:
#line 24993 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr1261;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1261:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1042;
st1042:
	if ( ++p == pe )
		goto _test_eof1042;
case 1042:
#line 25034 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr715:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1043;
st1043:
	if ( ++p == pe )
		goto _test_eof1043;
case 1043:
#line 25075 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr1262;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 65: goto tr1263;
		case 72: goto tr1264;
		case 73: goto tr1265;
		case 79: goto tr1266;
		case 86: goto tr1267;
		case 95: goto st249;
		case 97: goto tr1268;
		case 99: goto tr1269;
		case 100: goto tr1270;
		case 101: goto tr534;
		case 104: goto tr1271;
		case 105: goto tr1272;
		case 109: goto tr1273;
		case 111: goto tr1274;
		case 114: goto tr1275;
		case 116: goto tr1276;
		case 117: goto tr534;
		case 118: goto tr1277;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1262:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st581;
st581:
	if ( ++p == pe )
		goto _test_eof581;
case 581:
#line 25132 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr654;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr654:
#line 1 "NONE"
	{ s.te = p+1;}
#line 9 "rules/contractions.rl"
	{n2="'m";}
#line 6 "actions.rl"
	{ s.suffix++; }
#line 46 "rules/main.rl"
	{ s.act = 25;}
	goto st1044;
st1044:
	if ( ++p == pe )
		goto _test_eof1044;
case 1044:
#line 25167 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 111: goto tr1279;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr1278;
tr1279:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1045;
st1045:
	if ( ++p == pe )
		goto _test_eof1045;
case 1045:
#line 25205 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
		case 110: goto tr1280;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1263:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1046;
st1046:
	if ( ++p == pe )
		goto _test_eof1046;
case 1046:
#line 25243 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1281;
		case 78: goto tr1220;
		case 95: goto st249;
		case 108: goto tr1282;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1281:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1047;
st1047:
	if ( ++p == pe )
		goto _test_eof1047;
case 1047:
#line 25286 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1283;
		case 95: goto st249;
		case 108: goto tr1284;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1283:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1048;
st1048:
	if ( ++p == pe )
		goto _test_eof1048;
case 1048:
#line 25328 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1285;
		case 95: goto st249;
		case 116: goto tr1286;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1285:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1049;
st1049:
	if ( ++p == pe )
		goto _test_eof1049;
case 1049:
#line 25370 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 79: goto tr1219;
		case 95: goto st249;
		case 111: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1286:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1050;
st1050:
	if ( ++p == pe )
		goto _test_eof1050;
case 1050:
#line 25412 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 79: goto tr1237;
		case 95: goto st249;
		case 111: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1284:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1051;
st1051:
	if ( ++p == pe )
		goto _test_eof1051;
case 1051:
#line 25454 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1287;
		case 95: goto st249;
		case 116: goto tr1286;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1287:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1052;
st1052:
	if ( ++p == pe )
		goto _test_eof1052;
case 1052:
#line 25496 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 79: goto tr1237;
		case 95: goto st249;
		case 111: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1282:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1053;
st1053:
	if ( ++p == pe )
		goto _test_eof1053;
case 1053:
#line 25538 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1288;
		case 95: goto st249;
		case 108: goto tr1284;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1288:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1054;
st1054:
	if ( ++p == pe )
		goto _test_eof1054;
case 1054:
#line 25580 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1287;
		case 95: goto st249;
		case 116: goto tr1286;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1264:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1055;
st1055:
	if ( ++p == pe )
		goto _test_eof1055;
case 1055:
#line 25622 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1289;
		case 95: goto st249;
		case 114: goto tr1290;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1289:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1056;
st1056:
	if ( ++p == pe )
		goto _test_eof1056;
case 1056:
#line 25664 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 79: goto tr1291;
		case 95: goto st249;
		case 111: goto tr1292;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1291:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1057;
st1057:
	if ( ++p == pe )
		goto _test_eof1057;
case 1057:
#line 25706 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1293;
		case 95: goto st249;
		case 109: goto tr1294;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1293:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1058;
st1058:
	if ( ++p == pe )
		goto _test_eof1058;
case 1058:
#line 25748 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1295;
		case 95: goto st249;
		case 101: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1295:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1059;
st1059:
	if ( ++p == pe )
		goto _test_eof1059;
case 1059:
#line 25790 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
st582:
	if ( ++p == pe )
		goto _test_eof582;
case 582:
	if ( (*p) == 47 )
		goto st583;
	goto tr5;
st583:
	if ( ++p == pe )
		goto _test_eof583;
case 583:
	if ( (*p) == 47 )
		goto st526;
	goto tr5;
tr1296:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1060;
st1060:
	if ( ++p == pe )
		goto _test_eof1060;
case 1060:
#line 25845 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1294:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1061;
st1061:
	if ( ++p == pe )
		goto _test_eof1061;
case 1061:
#line 25886 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1298;
		case 95: goto st249;
		case 101: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1298:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1062;
st1062:
	if ( ++p == pe )
		goto _test_eof1062;
case 1062:
#line 25928 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1292:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1063;
st1063:
	if ( ++p == pe )
		goto _test_eof1063;
case 1063:
#line 25969 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1299;
		case 95: goto st249;
		case 109: goto tr1294;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1299:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1064;
st1064:
	if ( ++p == pe )
		goto _test_eof1064;
case 1064:
#line 26011 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1298;
		case 95: goto st249;
		case 101: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1290:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1065;
st1065:
	if ( ++p == pe )
		goto _test_eof1065;
case 1065:
#line 26053 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 79: goto tr1300;
		case 95: goto st249;
		case 111: goto tr1292;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1300:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1066;
st1066:
	if ( ++p == pe )
		goto _test_eof1066;
case 1066:
#line 26095 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1299;
		case 95: goto st249;
		case 109: goto tr1294;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1265:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1067;
st1067:
	if ( ++p == pe )
		goto _test_eof1067;
case 1067:
#line 26137 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 68: goto tr1219;
		case 95: goto st249;
		case 100: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1266:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1068;
st1068:
	if ( ++p == pe )
		goto _test_eof1068;
case 1068:
#line 26179 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 85: goto tr1301;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1301:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1069;
st1069:
	if ( ++p == pe )
		goto _test_eof1069;
case 1069:
#line 26220 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1302;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1302:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1070;
st1070:
	if ( ++p == pe )
		goto _test_eof1070;
case 1070:
#line 26261 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 68: goto tr1228;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1267:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1071;
st1071:
	if ( ++p == pe )
		goto _test_eof1071;
case 1071:
#line 26302 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1295;
		case 95: goto st249;
		case 115: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1268:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1072;
st1072:
	if ( ++p == pe )
		goto _test_eof1072;
case 1072:
#line 26344 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 76: goto tr1303;
		case 95: goto st249;
		case 108: goto tr1304;
		case 110: goto tr1238;
		case 112: goto tr1305;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1303:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1073;
st1073:
	if ( ++p == pe )
		goto _test_eof1073;
case 1073:
#line 26388 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1288;
		case 95: goto st249;
		case 108: goto tr1284;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1304:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1074;
st1074:
	if ( ++p == pe )
		goto _test_eof1074;
case 1074:
#line 26430 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1288;
		case 95: goto st249;
		case 105: goto tr1306;
		case 108: goto tr1284;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1306:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1075;
st1075:
	if ( ++p == pe )
		goto _test_eof1075;
case 1075:
#line 26473 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 102: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1305:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1076;
st1076:
	if ( ++p == pe )
		goto _test_eof1076;
case 1076:
#line 26514 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1269:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1077;
st1077:
	if ( ++p == pe )
		goto _test_eof1077;
case 1077:
#line 26555 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1270:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1078;
st1078:
	if ( ++p == pe )
		goto _test_eof1078;
case 1078:
#line 26596 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1271:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1079;
st1079:
	if ( ++p == pe )
		goto _test_eof1079;
case 1079:
#line 26637 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1307;
		case 95: goto st249;
		case 114: goto tr1290;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1307:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1080;
st1080:
	if ( ++p == pe )
		goto _test_eof1080;
case 1080:
#line 26679 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 79: goto tr1300;
		case 95: goto st249;
		case 111: goto tr1292;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1272:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1081;
st1081:
	if ( ++p == pe )
		goto _test_eof1081;
case 1081:
#line 26721 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 68: goto tr1237;
		case 95: goto st249;
		case 100: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1273:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1082;
st1082:
	if ( ++p == pe )
		goto _test_eof1082;
case 1082:
#line 26763 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1308;
		case 110: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1308:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1083;
st1083:
	if ( ++p == pe )
		goto _test_eof1083;
case 1083:
#line 26805 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1274:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1084;
st1084:
	if ( ++p == pe )
		goto _test_eof1084;
case 1084:
#line 26846 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1309;
		case 46: goto tr1236;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1310;
		case 110: goto tr1311;
		case 111: goto tr1312;
		case 114: goto tr1312;
		case 117: goto tr1313;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1309:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1085;
st1085:
	if ( ++p == pe )
		goto _test_eof1085;
case 1085:
#line 26891 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto st491;
		case 46: goto st376;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto tr1312;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1312:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1086;
st1086:
	if ( ++p == pe )
		goto _test_eof1086;
case 1086:
#line 26932 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 112: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1310:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1087;
st1087:
	if ( ++p == pe )
		goto _test_eof1087;
case 1087:
#line 26973 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1311:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1088;
st1088:
	if ( ++p == pe )
		goto _test_eof1088;
case 1088:
#line 27014 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1243;
		case 116: goto tr1314;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1314:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1089;
st1089:
	if ( ++p == pe )
		goto _test_eof1089;
case 1089:
#line 27056 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1315;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1315:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st584;
st584:
	if ( ++p == pe )
		goto _test_eof584;
case 584:
#line 27094 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr1313:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1090;
st1090:
	if ( ++p == pe )
		goto _test_eof1090;
case 1090:
#line 27125 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1316;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1316:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1091;
st1091:
	if ( ++p == pe )
		goto _test_eof1091;
case 1091:
#line 27166 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1245;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1275:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1092;
st1092:
	if ( ++p == pe )
		goto _test_eof1092;
case 1092:
#line 27207 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1317;
		case 107: goto tr1243;
		case 112: goto tr1318;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1317:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1093;
st1093:
	if ( ++p == pe )
		goto _test_eof1093;
case 1093:
#line 27250 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1318:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1094;
st1094:
	if ( ++p == pe )
		goto _test_eof1094;
case 1094:
#line 27291 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1276:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1095;
st1095:
	if ( ++p == pe )
		goto _test_eof1095;
case 1095:
#line 27332 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1277:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1096;
st1096:
	if ( ++p == pe )
		goto _test_eof1096;
case 1096:
#line 27373 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1298;
		case 95: goto st249;
		case 115: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr716:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1097;
st1097:
	if ( ++p == pe )
		goto _test_eof1097;
case 1097:
#line 27415 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 65: goto tr1319;
		case 73: goto tr1320;
		case 78: goto tr1267;
		case 79: goto tr1321;
		case 95: goto st249;
		case 97: goto tr1322;
		case 101: goto tr1323;
		case 105: goto tr1324;
		case 110: goto tr1277;
		case 111: goto tr1325;
		case 114: goto tr1326;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1319:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1098;
st1098:
	if ( ++p == pe )
		goto _test_eof1098;
case 1098:
#line 27468 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1327;
		case 86: goto tr1295;
		case 95: goto st249;
		case 116: goto tr1328;
		case 118: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1327:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1099;
st1099:
	if ( ++p == pe )
		goto _test_eof1099;
case 1099:
#line 27512 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1219;
		case 95: goto st249;
		case 97: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1328:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1100;
st1100:
	if ( ++p == pe )
		goto _test_eof1100;
case 1100:
#line 27554 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1237;
		case 95: goto st249;
		case 97: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1320:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1101;
st1101:
	if ( ++p == pe )
		goto _test_eof1101;
case 1101:
#line 27596 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1329;
		case 68: goto tr1228;
		case 95: goto st249;
		case 99: goto tr1330;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1329:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1102;
st1102:
	if ( ++p == pe )
		goto _test_eof1102;
case 1102:
#line 27639 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1295;
		case 95: goto st249;
		case 116: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1330:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1103;
st1103:
	if ( ++p == pe )
		goto _test_eof1103;
case 1103:
#line 27681 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1298;
		case 95: goto st249;
		case 116: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1321:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1104;
st1104:
	if ( ++p == pe )
		goto _test_eof1104;
case 1104:
#line 27723 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1331;
		case 73: goto tr1219;
		case 78: goto tr1220;
		case 95: goto st249;
		case 105: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1331:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1105;
st1105:
	if ( ++p == pe )
		goto _test_eof1105;
case 1105:
#line 27767 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1228;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1322:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1106;
st1106:
	if ( ++p == pe )
		goto _test_eof1106;
case 1106:
#line 27808 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1332;
		case 86: goto tr1298;
		case 95: goto st249;
		case 116: goto tr1328;
		case 118: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1332:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1107;
st1107:
	if ( ++p == pe )
		goto _test_eof1107;
case 1107:
#line 27852 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1237;
		case 95: goto st249;
		case 97: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1323:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1108;
st1108:
	if ( ++p == pe )
		goto _test_eof1108;
case 1108:
#line 27894 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1243;
		case 108: goto tr1243;
		case 112: goto tr1333;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1333:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1109;
st1109:
	if ( ++p == pe )
		goto _test_eof1109;
case 1109:
#line 27937 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1334;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1334:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st585;
st585:
	if ( ++p == pe )
		goto _test_eof585;
case 585:
#line 27976 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 116: goto tr657;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr1324:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1110;
st1110:
	if ( ++p == pe )
		goto _test_eof1110;
case 1110:
#line 28008 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1335;
		case 95: goto st249;
		case 99: goto tr1330;
		case 100: goto tr1245;
		case 115: goto tr1336;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1335:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1111;
st1111:
	if ( ++p == pe )
		goto _test_eof1111;
case 1111:
#line 28052 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1298;
		case 95: goto st249;
		case 116: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1336:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1112;
st1112:
	if ( ++p == pe )
		goto _test_eof1112;
case 1112:
#line 28094 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1325:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1113;
st1113:
	if ( ++p == pe )
		goto _test_eof1113;
case 1113:
#line 28135 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 73: goto tr1237;
		case 95: goto st249;
		case 101: goto tr1337;
		case 105: goto tr1221;
		case 110: goto tr1238;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1337:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1114;
st1114:
	if ( ++p == pe )
		goto _test_eof1114;
case 1114:
#line 28179 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1245;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1326:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1115;
st1115:
	if ( ++p == pe )
		goto _test_eof1115;
case 1115:
#line 28220 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1260;
		case 118: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr717:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1116;
st1116:
	if ( ++p == pe )
		goto _test_eof1116;
case 1116:
#line 28262 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr534;
		case 100: goto tr1338;
		case 101: goto tr534;
		case 105: goto tr534;
		case 111: goto tr534;
		case 115: goto tr1339;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1338:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1117;
st1117:
	if ( ++p == pe )
		goto _test_eof1117;
case 1117:
#line 28311 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 117: goto tr1340;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1340:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1118;
st1118:
	if ( ++p == pe )
		goto _test_eof1118;
case 1118:
#line 28352 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1339:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1119;
st1119:
	if ( ++p == pe )
		goto _test_eof1119;
case 1119:
#line 28393 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 113: goto tr1260;
		case 116: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr718:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1120;
st1120:
	if ( ++p == pe )
		goto _test_eof1120;
case 1120:
#line 28435 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 69: goto tr1341;
		case 73: goto tr1342;
		case 84: goto tr1343;
		case 95: goto st249;
		case 97: goto tr534;
		case 101: goto tr1344;
		case 105: goto tr1345;
		case 108: goto tr1346;
		case 111: goto tr534;
		case 114: goto tr1347;
		case 116: goto tr1348;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1341:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1121;
st1121:
	if ( ++p == pe )
		goto _test_eof1121;
case 1121:
#line 28488 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1265;
		case 95: goto st249;
		case 101: goto tr1272;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1342:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1122;
st1122:
	if ( ++p == pe )
		goto _test_eof1122;
case 1122:
#line 28530 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1293;
		case 95: goto st249;
		case 108: goto tr1294;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1343:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1123;
st1123:
	if ( ++p == pe )
		goto _test_eof1123;
case 1123:
#line 28572 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 80: goto tr1295;
		case 95: goto st249;
		case 112: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1344:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1124;
st1124:
	if ( ++p == pe )
		goto _test_eof1124;
case 1124:
#line 28614 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1349;
		case 95: goto st249;
		case 98: goto tr1243;
		case 101: goto tr1272;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1349:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1125;
st1125:
	if ( ++p == pe )
		goto _test_eof1125;
case 1125:
#line 28657 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 68: goto tr1237;
		case 95: goto st249;
		case 100: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1345:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1126;
st1126:
	if ( ++p == pe )
		goto _test_eof1126;
case 1126:
#line 28699 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1299;
		case 95: goto st249;
		case 108: goto tr1294;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1346:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1127;
st1127:
	if ( ++p == pe )
		goto _test_eof1127;
case 1127:
#line 28741 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1347:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1128;
st1128:
	if ( ++p == pe )
		goto _test_eof1128;
case 1128:
#line 28782 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1348:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1129;
st1129:
	if ( ++p == pe )
		goto _test_eof1129;
case 1129:
#line 28823 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 80: goto tr1298;
		case 95: goto st249;
		case 112: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr719:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1130;
st1130:
	if ( ++p == pe )
		goto _test_eof1130;
case 1130:
#line 28865 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 69: goto tr1285;
		case 73: goto tr1329;
		case 79: goto tr1350;
		case 84: goto tr1351;
		case 95: goto st249;
		case 97: goto tr1243;
		case 101: goto tr1352;
		case 105: goto tr1330;
		case 111: goto tr1353;
		case 116: goto tr1354;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1350:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1131;
st1131:
	if ( ++p == pe )
		goto _test_eof1131;
case 1131:
#line 28917 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 80: goto tr1355;
		case 95: goto st249;
		case 112: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1355:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1132;
st1132:
	if ( ++p == pe )
		goto _test_eof1132;
case 1132:
#line 28960 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 72: goto tr1357;
		case 95: goto st249;
		case 104: goto tr1358;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1357:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1133;
st1133:
	if ( ++p == pe )
		goto _test_eof1133;
case 1133:
#line 29002 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1359;
		case 95: goto st249;
		case 101: goto tr1360;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1359:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1134;
st1134:
	if ( ++p == pe )
		goto _test_eof1134;
case 1134:
#line 29044 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1295;
		case 95: goto st249;
		case 114: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1360:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1135;
st1135:
	if ( ++p == pe )
		goto _test_eof1135;
case 1135:
#line 29086 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1298;
		case 95: goto st249;
		case 114: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1358:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1136;
st1136:
	if ( ++p == pe )
		goto _test_eof1136;
case 1136:
#line 29128 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1361;
		case 95: goto st249;
		case 101: goto tr1360;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1361:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1137;
st1137:
	if ( ++p == pe )
		goto _test_eof1137;
case 1137:
#line 29170 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1298;
		case 95: goto st249;
		case 114: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1356:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1138;
st1138:
	if ( ++p == pe )
		goto _test_eof1138;
case 1138:
#line 29212 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 72: goto tr1362;
		case 95: goto st249;
		case 104: goto tr1358;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1362:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1139;
st1139:
	if ( ++p == pe )
		goto _test_eof1139;
case 1139:
#line 29254 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1361;
		case 95: goto st249;
		case 101: goto tr1360;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1351:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1140;
st1140:
	if ( ++p == pe )
		goto _test_eof1140;
case 1140:
#line 29296 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1363;
		case 95: goto st249;
		case 97: goto tr1364;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1363:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1141;
st1141:
	if ( ++p == pe )
		goto _test_eof1141;
case 1141:
#line 29338 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1365;
		case 95: goto st249;
		case 108: goto tr1366;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1365:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1142;
st1142:
	if ( ++p == pe )
		goto _test_eof1142;
case 1142:
#line 29380 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 75: goto tr1219;
		case 95: goto st249;
		case 107: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1366:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1143;
st1143:
	if ( ++p == pe )
		goto _test_eof1143;
case 1143:
#line 29422 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 75: goto tr1237;
		case 95: goto st249;
		case 107: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1364:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1144;
st1144:
	if ( ++p == pe )
		goto _test_eof1144;
case 1144:
#line 29464 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1367;
		case 95: goto st249;
		case 108: goto tr1366;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1367:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1145;
st1145:
	if ( ++p == pe )
		goto _test_eof1145;
case 1145:
#line 29506 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 75: goto tr1237;
		case 95: goto st249;
		case 107: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1352:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1146;
st1146:
	if ( ++p == pe )
		goto _test_eof1146;
case 1146:
#line 29548 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 79: goto tr1237;
		case 95: goto st249;
		case 110: goto tr1260;
		case 111: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1353:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1147;
st1147:
	if ( ++p == pe )
		goto _test_eof1147;
case 1147:
#line 29591 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 80: goto tr1368;
		case 95: goto st249;
		case 112: goto tr1356;
		case 118: goto tr1369;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1368:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1148;
st1148:
	if ( ++p == pe )
		goto _test_eof1148;
case 1148:
#line 29635 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 72: goto tr1362;
		case 95: goto st249;
		case 104: goto tr1358;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1369:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1149;
st1149:
	if ( ++p == pe )
		goto _test_eof1149;
case 1149:
#line 29677 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1334;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1354:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1150;
st1150:
	if ( ++p == pe )
		goto _test_eof1150;
case 1150:
#line 29718 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1370;
		case 95: goto st249;
		case 97: goto tr1364;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1370:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1151;
st1151:
	if ( ++p == pe )
		goto _test_eof1151;
case 1151:
#line 29760 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1367;
		case 95: goto st249;
		case 108: goto tr1366;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr720:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1152;
st1152:
	if ( ++p == pe )
		goto _test_eof1152;
case 1152:
#line 29802 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 65: goto tr1371;
		case 84: goto tr1372;
		case 95: goto st249;
		case 97: goto tr1373;
		case 98: goto tr1374;
		case 101: goto tr534;
		case 103: goto tr1375;
		case 105: goto tr534;
		case 111: goto tr1376;
		case 116: goto tr1377;
		case 117: goto tr534;
		case 119: goto tr1378;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 99 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1371:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1153;
st1153:
	if ( ++p == pe )
		goto _test_eof1153;
case 1153:
#line 29854 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 68: goto tr1228;
		case 83: goto tr1228;
		case 86: goto tr1129;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1372:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1154;
st1154:
	if ( ++p == pe )
		goto _test_eof1154;
case 1154:
#line 29897 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1379;
		case 95: goto st249;
		case 116: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1379:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1155;
st1155:
	if ( ++p == pe )
		goto _test_eof1155;
case 1155:
#line 29939 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 80: goto tr1381;
		case 95: goto st249;
		case 112: goto tr1382;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1381:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1156;
st1156:
	if ( ++p == pe )
		goto _test_eof1156;
case 1156:
#line 29981 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 83: goto tr1295;
		case 95: goto st249;
		case 115: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1382:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1157;
st1157:
	if ( ++p == pe )
		goto _test_eof1157;
case 1157:
#line 30024 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 83: goto tr1298;
		case 95: goto st249;
		case 115: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1380:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1158;
st1158:
	if ( ++p == pe )
		goto _test_eof1158;
case 1158:
#line 30067 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 80: goto tr1383;
		case 95: goto st249;
		case 112: goto tr1382;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1383:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1159;
st1159:
	if ( ++p == pe )
		goto _test_eof1159;
case 1159:
#line 30109 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 83: goto tr1298;
		case 95: goto st249;
		case 115: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1373:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1160;
st1160:
	if ( ++p == pe )
		goto _test_eof1160;
case 1160:
#line 30152 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1245;
		case 115: goto tr1245;
		case 118: goto tr1384;
		case 119: goto tr1385;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1384:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1161;
st1161:
	if ( ++p == pe )
		goto _test_eof1161;
case 1161:
#line 30196 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1245;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1385:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1162;
st1162:
	if ( ++p == pe )
		goto _test_eof1162;
case 1162:
#line 30237 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1386;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1386:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1163;
st1163:
	if ( ++p == pe )
		goto _test_eof1163;
case 1163:
#line 30278 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1387;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1387:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1164;
st1164:
	if ( ++p == pe )
		goto _test_eof1164;
case 1164:
#line 30319 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1388;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1388:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st586;
st586:
	if ( ++p == pe )
		goto _test_eof586;
case 586:
#line 30357 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 105: goto tr657;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr1374:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1165;
st1165:
	if ( ++p == pe )
		goto _test_eof1165;
case 1165:
#line 30389 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1375:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1166;
st1166:
	if ( ++p == pe )
		goto _test_eof1166;
case 1166:
#line 30430 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1317;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1376:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1167;
st1167:
	if ( ++p == pe )
		goto _test_eof1167;
case 1167:
#line 30471 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1377:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1168;
st1168:
	if ( ++p == pe )
		goto _test_eof1168;
case 1168:
#line 30512 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1389;
		case 95: goto st249;
		case 115: goto tr1243;
		case 116: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1389:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1169;
st1169:
	if ( ++p == pe )
		goto _test_eof1169;
case 1169:
#line 30555 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 80: goto tr1383;
		case 95: goto st249;
		case 112: goto tr1382;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1378:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1170;
st1170:
	if ( ++p == pe )
		goto _test_eof1170;
case 1170:
#line 30597 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 121: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr721:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1171;
st1171:
	if ( ++p == pe )
		goto _test_eof1171;
case 1171:
#line 30638 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 77: goto tr1390;
		case 82: goto tr1391;
		case 83: goto tr1228;
		case 95: goto st249;
		case 97: goto tr534;
		case 101: goto tr534;
		case 105: goto tr534;
		case 108: goto tr1392;
		case 109: goto tr1393;
		case 110: goto tr1394;
		case 111: goto tr534;
		case 114: goto tr1395;
		case 115: goto tr1396;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1390:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1172;
st1172:
	if ( ++p == pe )
		goto _test_eof1172;
case 1172:
#line 30693 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 65: goto tr1343;
		case 95: goto st249;
		case 97: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1397:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1173;
st1173:
	if ( ++p == pe )
		goto _test_eof1173;
case 1173:
#line 30736 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 80: goto tr1298;
		case 95: goto st249;
		case 112: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1391:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1174;
st1174:
	if ( ++p == pe )
		goto _test_eof1174;
case 1174:
#line 30778 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1381;
		case 95: goto st249;
		case 99: goto tr1382;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1392:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1175;
st1175:
	if ( ++p == pe )
		goto _test_eof1175;
case 1175:
#line 30820 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1393:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1176;
st1176:
	if ( ++p == pe )
		goto _test_eof1176;
case 1176:
#line 30861 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 65: goto tr1398;
		case 95: goto st249;
		case 97: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1398:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1177;
st1177:
	if ( ++p == pe )
		goto _test_eof1177;
case 1177:
#line 30904 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 80: goto tr1298;
		case 95: goto st249;
		case 112: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1394:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1178;
st1178:
	if ( ++p == pe )
		goto _test_eof1178;
case 1178:
#line 30946 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1235;
		case 100: goto tr1399;
		case 116: goto tr1400;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1399:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1179;
st1179:
	if ( ++p == pe )
		goto _test_eof1179;
case 1179:
#line 30989 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1400:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1180;
st1180:
	if ( ++p == pe )
		goto _test_eof1180;
case 1180:
#line 31029 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1232;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1236;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1395:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1181;
st1181:
	if ( ++p == pe )
		goto _test_eof1181;
case 1181:
#line 31070 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1383;
		case 95: goto st249;
		case 99: goto tr1382;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1396:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1182;
st1182:
	if ( ++p == pe )
		goto _test_eof1182;
case 1182:
#line 31112 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1238;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr722:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1183;
st1183:
	if ( ++p == pe )
		goto _test_eof1183;
case 1183:
#line 31153 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1401;
		case 99: goto tr1402;
		case 101: goto tr534;
		case 105: goto tr534;
		case 110: goto tr1270;
		case 111: goto tr534;
		case 114: goto tr1403;
		case 117: goto tr1404;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1401:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1184;
st1184:
	if ( ++p == pe )
		goto _test_eof1184;
case 1184:
#line 31203 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1402:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1185;
st1185:
	if ( ++p == pe )
		goto _test_eof1185;
case 1185:
#line 31244 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1401;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1403:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1186;
st1186:
	if ( ++p == pe )
		goto _test_eof1186;
case 1186:
#line 31285 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1404:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1187;
st1187:
	if ( ++p == pe )
		goto _test_eof1187;
case 1187:
#line 31325 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1243;
		case 110: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr723:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1188;
st1188:
	if ( ++p == pe )
		goto _test_eof1188;
case 1188:
#line 31367 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1401;
		case 101: goto tr534;
		case 105: goto tr534;
		case 111: goto tr534;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr724:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1189;
st1189:
	if ( ++p == pe )
		goto _test_eof1189;
case 1189:
#line 31414 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 65: goto tr1405;
		case 68: goto tr1406;
		case 95: goto st249;
		case 97: goto tr1407;
		case 98: goto tr1408;
		case 100: goto tr1409;
		case 101: goto tr534;
		case 105: goto tr534;
		case 107: goto tr1410;
		case 110: goto tr1411;
		case 111: goto tr534;
		case 116: goto tr1412;
		case 117: goto tr534;
		case 118: goto tr1392;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 99 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1405:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1190;
st1190:
	if ( ++p == pe )
		goto _test_eof1190;
case 1190:
#line 31469 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1413;
		case 95: goto st249;
		case 115: goto tr1414;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1413:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1191;
st1191:
	if ( ++p == pe )
		goto _test_eof1191;
case 1191:
#line 31511 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1415;
		case 95: goto st249;
		case 116: goto tr1416;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1415:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1192;
st1192:
	if ( ++p == pe )
		goto _test_eof1192;
case 1192:
#line 31553 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 70: goto tr1417;
		case 95: goto st249;
		case 102: goto tr1418;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1417:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1193;
st1193:
	if ( ++p == pe )
		goto _test_eof1193;
case 1193:
#line 31595 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1295;
		case 95: goto st249;
		case 109: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1418:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1194;
st1194:
	if ( ++p == pe )
		goto _test_eof1194;
case 1194:
#line 31637 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1298;
		case 95: goto st249;
		case 109: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1416:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1195;
st1195:
	if ( ++p == pe )
		goto _test_eof1195;
case 1195:
#line 31679 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 70: goto tr1419;
		case 95: goto st249;
		case 102: goto tr1418;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1419:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1196;
st1196:
	if ( ++p == pe )
		goto _test_eof1196;
case 1196:
#line 31721 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1298;
		case 95: goto st249;
		case 109: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1414:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1197;
st1197:
	if ( ++p == pe )
		goto _test_eof1197;
case 1197:
#line 31763 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1420;
		case 95: goto st249;
		case 116: goto tr1416;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1420:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1198;
st1198:
	if ( ++p == pe )
		goto _test_eof1198;
case 1198:
#line 31805 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 70: goto tr1419;
		case 95: goto st249;
		case 102: goto tr1418;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1406:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1199;
st1199:
	if ( ++p == pe )
		goto _test_eof1199;
case 1199:
#line 31847 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1379;
		case 95: goto st249;
		case 97: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1407:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1200;
st1200:
	if ( ++p == pe )
		goto _test_eof1200;
case 1200:
#line 31889 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 83: goto tr1421;
		case 95: goto st249;
		case 115: goto tr1414;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1421:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1201;
st1201:
	if ( ++p == pe )
		goto _test_eof1201;
case 1201:
#line 31931 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1420;
		case 95: goto st249;
		case 116: goto tr1416;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1408:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1202;
st1202:
	if ( ++p == pe )
		goto _test_eof1202;
case 1202:
#line 31973 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 98: goto tr1378;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1409:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1203;
st1203:
	if ( ++p == pe )
		goto _test_eof1203;
case 1203:
#line 32014 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1389;
		case 95: goto st249;
		case 97: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1410:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1204;
st1204:
	if ( ++p == pe )
		goto _test_eof1204;
case 1204:
#line 32056 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1411:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1205;
st1205:
	if ( ++p == pe )
		goto _test_eof1205;
case 1205:
#line 32097 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1137;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1412:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1206;
st1206:
	if ( ++p == pe )
		goto _test_eof1206;
case 1206:
#line 32138 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr725:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1207;
st1207:
	if ( ++p == pe )
		goto _test_eof1207;
case 1207:
#line 32179 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 65: goto tr1422;
		case 77: goto tr1267;
		case 95: goto st249;
		case 97: goto tr1423;
		case 100: goto tr1424;
		case 101: goto tr534;
		case 105: goto tr1425;
		case 107: goto tr1426;
		case 108: goto tr1410;
		case 109: goto tr1277;
		case 110: goto tr1427;
		case 111: goto tr1428;
		case 114: goto tr1429;
		case 115: goto tr1430;
		case 116: goto tr1431;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1422:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1208;
st1208:
	if ( ++p == pe )
		goto _test_eof1208;
case 1208:
#line 32236 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 73: goto tr1281;
		case 95: goto st249;
		case 105: goto tr1282;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1423:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1209;
st1209:
	if ( ++p == pe )
		goto _test_eof1209;
case 1209:
#line 32278 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1432;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 73: goto tr1303;
		case 95: goto st249;
		case 105: goto tr1282;
		case 106: goto tr1260;
		case 110: goto tr1433;
		case 114: goto tr1243;
		case 115: goto tr1317;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1432:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st587;
st587:
	if ( ++p == pe )
		goto _test_eof587;
case 587:
#line 32322 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 97: goto tr658;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr658:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1210;
st1210:
	if ( ++p == pe )
		goto _test_eof1210;
case 1210:
#line 32354 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 109: goto tr1280;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1433:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1211;
st1211:
	if ( ++p == pe )
		goto _test_eof1211;
case 1211:
#line 32392 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1261;
		case 117: goto tr1434;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1434:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1212;
st1212:
	if ( ++p == pe )
		goto _test_eof1212;
case 1212:
#line 32434 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 102: goto tr1435;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1435:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1213;
st1213:
	if ( ++p == pe )
		goto _test_eof1213;
case 1213:
#line 32475 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1340;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1424:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1214;
st1214:
	if ( ++p == pe )
		goto _test_eof1214;
case 1214:
#line 32516 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 119: goto tr1436;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1436:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1215;
st1215:
	if ( ++p == pe )
		goto _test_eof1215;
case 1215:
#line 32557 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1425:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1216;
st1216:
	if ( ++p == pe )
		goto _test_eof1216;
case 1216:
#line 32598 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1437;
		case 110: goto tr1401;
		case 115: goto tr1317;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1437:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1217;
st1217:
	if ( ++p == pe )
		goto _test_eof1217;
case 1217:
#line 32641 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 104: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1426:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1218;
st1218:
	if ( ++p == pe )
		goto _test_eof1218;
case 1218:
#line 32682 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1438;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1438:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1219;
st1219:
	if ( ++p == pe )
		goto _test_eof1219;
case 1219:
#line 32723 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1427:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1220;
st1220:
	if ( ++p == pe )
		goto _test_eof1220;
case 1220:
#line 32764 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1243;
		case 116: goto tr1439;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1439:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1221;
st1221:
	if ( ++p == pe )
		goto _test_eof1221;
case 1221:
#line 32806 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1436;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1428:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1222;
st1222:
	if ( ++p == pe )
		goto _test_eof1222;
case 1222:
#line 32847 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1440;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1440:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1223;
st1223:
	if ( ++p == pe )
		goto _test_eof1223;
case 1223:
#line 32888 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1429:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1224;
st1224:
	if ( ++p == pe )
		goto _test_eof1224;
case 1224:
#line 32929 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1430:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1225;
st1225:
	if ( ++p == pe )
		goto _test_eof1225;
case 1225:
#line 32970 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1441;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1441:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1226;
st1226:
	if ( ++p == pe )
		goto _test_eof1226;
case 1226:
#line 33011 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1442;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1442:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1227;
st1227:
	if ( ++p == pe )
		goto _test_eof1227;
case 1227:
#line 33052 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1431:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1228;
st1228:
	if ( ++p == pe )
		goto _test_eof1228;
case 1228:
#line 33093 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr726:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1229;
st1229:
	if ( ++p == pe )
		goto _test_eof1229;
case 1229:
#line 33134 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1443;
		case 64: goto st250;
		case 69: goto tr1444;
		case 95: goto st249;
		case 97: goto tr1445;
		case 101: goto tr1446;
		case 105: goto tr534;
		case 111: goto tr1447;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1443:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1230;
st1230:
	if ( ++p == pe )
		goto _test_eof1230;
case 1230:
#line 33182 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st201;
		case 32: goto st588;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st249;
		} else if ( (*p) >= 9 )
			goto st588;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr677;
		} else if ( (*p) >= 65 )
			goto tr677;
	} else
		goto st249;
	goto tr1139;
st588:
	if ( ++p == pe )
		goto _test_eof588;
case 588:
	switch( (*p) ) {
		case 32: goto st588;
		case 67: goto st589;
		case 68: goto st594;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st588;
	goto tr5;
st589:
	if ( ++p == pe )
		goto _test_eof589;
case 589:
	if ( (*p) == 97 )
		goto st590;
	goto tr5;
st590:
	if ( ++p == pe )
		goto _test_eof590;
case 590:
	if ( (*p) == 114 )
		goto st591;
	goto tr5;
st591:
	if ( ++p == pe )
		goto _test_eof591;
case 591:
	if ( (*p) == 111 )
		goto st592;
	goto tr5;
st592:
	if ( ++p == pe )
		goto _test_eof592;
case 592:
	if ( (*p) == 108 )
		goto st593;
	goto tr5;
st593:
	if ( ++p == pe )
		goto _test_eof593;
case 593:
	if ( (*p) == 46 )
		goto tr666;
	goto tr5;
st594:
	if ( ++p == pe )
		goto _test_eof594;
case 594:
	if ( (*p) == 97 )
		goto st595;
	goto tr5;
st595:
	if ( ++p == pe )
		goto _test_eof595;
case 595:
	if ( (*p) == 107 )
		goto st593;
	goto tr5;
tr1444:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1231;
st1231:
	if ( ++p == pe )
		goto _test_eof1231;
case 1231:
#line 33278 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 87: goto tr1448;
		case 95: goto st249;
		case 119: goto tr1449;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1448:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1232;
st1232:
	if ( ++p == pe )
		goto _test_eof1232;
case 1232:
#line 33320 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1219;
		case 95: goto st249;
		case 115: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1449:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1233;
st1233:
	if ( ++p == pe )
		goto _test_eof1233;
case 1233:
#line 33362 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1237;
		case 95: goto st249;
		case 115: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1445:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1234;
st1234:
	if ( ++p == pe )
		goto _test_eof1234;
case 1234:
#line 33404 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1230;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1446:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1235;
st1235:
	if ( ++p == pe )
		goto _test_eof1235;
case 1235:
#line 33445 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 87: goto tr1450;
		case 95: goto st249;
		case 98: goto tr1243;
		case 118: goto tr1243;
		case 119: goto tr1449;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1450:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1236;
st1236:
	if ( ++p == pe )
		goto _test_eof1236;
case 1236:
#line 33489 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1237;
		case 95: goto st249;
		case 115: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1447:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1237;
st1237:
	if ( ++p == pe )
		goto _test_eof1237;
case 1237:
#line 33531 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st596;
		case 64: goto st250;
		case 95: goto st249;
		case 118: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st596:
	if ( ++p == pe )
		goto _test_eof596;
case 596:
	switch( (*p) ) {
		case -30: goto st201;
		case 32: goto st588;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st249;
		} else if ( (*p) >= 9 )
			goto st588;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr575;
		} else if ( (*p) >= 65 )
			goto tr575;
	} else
		goto st249;
	goto tr14;
tr727:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1238;
st1238:
	if ( ++p == pe )
		goto _test_eof1238;
case 1238:
#line 33600 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr1452;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 95: goto st249;
		case 98: goto tr1130;
		case 99: goto tr1269;
		case 100: goto tr1130;
		case 105: goto tr534;
		case 107: goto tr1453;
		case 110: goto tr1269;
		case 111: goto tr534;
		case 114: goto tr1454;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr1126;
		} else if ( (*p) > 101 ) {
			if ( 102 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr534;
	} else
		goto tr532;
	goto tr763;
tr1452:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st597;
st597:
	if ( ++p == pe )
		goto _test_eof597;
case 597:
#line 33652 "tokenizer.cc"
	switch( (*p) ) {
		case 67: goto tr668;
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 99: goto tr668;
		case 100: goto tr254;
		case 101: goto tr669;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr668:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1239;
st1239:
	if ( ++p == pe )
		goto _test_eof1239;
case 1239:
#line 33686 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 108: goto tr1455;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1455:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1240;
st1240:
	if ( ++p == pe )
		goto _test_eof1240;
case 1240:
#line 33724 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
		case 111: goto tr1456;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1456:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1241;
st1241:
	if ( ++p == pe )
		goto _test_eof1241;
case 1241:
#line 33762 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
		case 99: goto tr1457;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1457:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1242;
st1242:
	if ( ++p == pe )
		goto _test_eof1242;
case 1242:
#line 33800 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
		case 107: goto tr1280;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr669:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1243;
st1243:
	if ( ++p == pe )
		goto _test_eof1243;
case 1243:
#line 33838 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 114: goto tr1280;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1453:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1244;
st1244:
	if ( ++p == pe )
		goto _test_eof1244;
case 1244:
#line 33876 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1458;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1458:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1245;
st1245:
	if ( ++p == pe )
		goto _test_eof1245;
case 1245:
#line 33917 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1454:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1246;
st1246:
	if ( ++p == pe )
		goto _test_eof1246;
case 1246:
#line 33958 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr728:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1247;
st1247:
	if ( ++p == pe )
		goto _test_eof1247;
case 1247:
#line 33999 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1459;
		case 64: goto st250;
		case 79: goto tr1343;
		case 95: goto st249;
		case 97: goto tr1243;
		case 101: goto tr1460;
		case 105: goto tr534;
		case 107: goto tr1461;
		case 108: goto tr1462;
		case 111: goto tr1397;
		case 114: goto tr1463;
		case 116: goto tr1464;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1459:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1248;
st1248:
	if ( ++p == pe )
		goto _test_eof1248;
case 1248:
#line 34051 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 77: goto tr687;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr677;
		} else if ( (*p) >= 65 )
			goto tr677;
	} else
		goto st249;
	goto tr1139;
tr687:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1249;
st1249:
	if ( ++p == pe )
		goto _test_eof1249;
case 1249:
#line 34083 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto tr1465;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1460:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1250;
st1250:
	if ( ++p == pe )
		goto _test_eof1250;
case 1250:
#line 34123 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1466;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1466:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1251;
st1251:
	if ( ++p == pe )
		goto _test_eof1251;
case 1251:
#line 34164 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1467;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1467:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1252;
st1252:
	if ( ++p == pe )
		goto _test_eof1252;
case 1252:
#line 34205 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1461:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1253;
st1253:
	if ( ++p == pe )
		goto _test_eof1253;
case 1253:
#line 34246 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 119: goto tr1378;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1462:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1254;
st1254:
	if ( ++p == pe )
		goto _test_eof1254;
case 1254:
#line 34287 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1235;
		case 110: goto tr1436;
		case 122: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1463:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1255;
st1255:
	if ( ++p == pe )
		goto _test_eof1255;
case 1255:
#line 34330 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1259;
		case 111: goto tr1468;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1468:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1256;
st1256:
	if ( ++p == pe )
		goto _test_eof1256;
case 1256:
#line 34372 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 102: goto tr1260;
		case 118: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1464:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1257;
st1257:
	if ( ++p == pe )
		goto _test_eof1257;
case 1257:
#line 34414 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 121: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr729:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1258;
st1258:
	if ( ++p == pe )
		goto _test_eof1258;
case 1258:
#line 34455 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr534;
		case 101: goto tr534;
		case 105: goto tr534;
		case 111: goto tr534;
		case 117: goto tr1469;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1469:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1259;
st1259:
	if ( ++p == pe )
		goto _test_eof1259;
case 1259:
#line 34502 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1243;
		case 114: goto tr1470;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1470:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1260;
st1260:
	if ( ++p == pe )
		goto _test_eof1260;
case 1260:
#line 34544 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1471;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1471:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st598;
st598:
	if ( ++p == pe )
		goto _test_eof598;
case 598:
#line 34582 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 97: goto tr670;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr670:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1261;
st1261:
	if ( ++p == pe )
		goto _test_eof1261;
case 1261:
#line 34614 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 110: goto tr1280;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr730:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1262;
st1262:
	if ( ++p == pe )
		goto _test_eof1262;
case 1262:
#line 34652 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 77: goto tr1472;
		case 83: goto tr1473;
		case 84: goto tr1474;
		case 95: goto st249;
		case 97: goto tr534;
		case 100: goto tr1276;
		case 101: goto tr1475;
		case 105: goto tr534;
		case 109: goto tr1476;
		case 111: goto tr534;
		case 115: goto tr1477;
		case 116: goto tr1478;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1472:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1263;
st1263:
	if ( ++p == pe )
		goto _test_eof1263;
case 1263:
#line 34706 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 73: goto tr1295;
		case 95: goto st249;
		case 105: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1473:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1264;
st1264:
	if ( ++p == pe )
		goto _test_eof1264;
case 1264:
#line 34748 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 89: goto tr1479;
		case 95: goto st249;
		case 121: goto tr1480;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1479:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1265;
st1265:
	if ( ++p == pe )
		goto _test_eof1265;
case 1265:
#line 34790 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 78: goto tr1481;
		case 95: goto st249;
		case 110: goto tr1482;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1481:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1266;
st1266:
	if ( ++p == pe )
		goto _test_eof1266;
case 1266:
#line 34832 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1295;
		case 95: goto st249;
		case 99: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1482:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1267;
st1267:
	if ( ++p == pe )
		goto _test_eof1267;
case 1267:
#line 34874 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1298;
		case 95: goto st249;
		case 99: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1480:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1268;
st1268:
	if ( ++p == pe )
		goto _test_eof1268;
case 1268:
#line 34916 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 78: goto tr1483;
		case 95: goto st249;
		case 110: goto tr1482;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1483:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1269;
st1269:
	if ( ++p == pe )
		goto _test_eof1269;
case 1269:
#line 34958 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1298;
		case 95: goto st249;
		case 99: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1474:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1270;
st1270:
	if ( ++p == pe )
		goto _test_eof1270;
case 1270:
#line 35000 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1343;
		case 95: goto st249;
		case 109: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1475:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1271;
st1271:
	if ( ++p == pe )
		goto _test_eof1271;
case 1271:
#line 35042 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 112: goto tr1260;
		case 113: goto tr1314;
		case 118: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1476:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1272;
st1272:
	if ( ++p == pe )
		goto _test_eof1272;
case 1272:
#line 35085 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 73: goto tr1298;
		case 95: goto st249;
		case 105: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1477:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1273;
st1273:
	if ( ++p == pe )
		goto _test_eof1273;
case 1273:
#line 35127 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 89: goto tr1484;
		case 95: goto st249;
		case 121: goto tr1480;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1484:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1274;
st1274:
	if ( ++p == pe )
		goto _test_eof1274;
case 1274:
#line 35169 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 78: goto tr1483;
		case 95: goto st249;
		case 110: goto tr1482;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1478:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1275;
st1275:
	if ( ++p == pe )
		goto _test_eof1275;
case 1275:
#line 35211 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1233;
		case 64: goto st250;
		case 77: goto tr1398;
		case 95: goto st249;
		case 100: goto tr1260;
		case 109: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr731:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1276;
st1276:
	if ( ++p == pe )
		goto _test_eof1276;
case 1276:
#line 35254 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1443;
		case 64: goto st250;
		case 70: goto tr1485;
		case 72: goto tr1486;
		case 75: goto tr1487;
		case 77: goto tr1488;
		case 79: goto tr1489;
		case 83: goto tr1490;
		case 95: goto st249;
		case 97: goto tr1261;
		case 101: goto tr1491;
		case 102: goto tr1492;
		case 103: goto tr1493;
		case 104: goto tr1494;
		case 105: goto tr534;
		case 107: goto tr1495;
		case 109: goto tr1496;
		case 110: goto tr1270;
		case 111: goto tr1497;
		case 114: goto tr1403;
		case 115: goto tr1498;
		case 116: goto tr1499;
		case 117: goto tr1401;
		case 118: goto tr1500;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1485:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1277;
st1277:
	if ( ++p == pe )
		goto _test_eof1277;
case 1277:
#line 35317 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1343;
		case 95: goto st249;
		case 116: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1486:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1278;
st1278:
	if ( ++p == pe )
		goto _test_eof1278;
case 1278:
#line 35359 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 79: goto tr1266;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1487:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1279;
st1279:
	if ( ++p == pe )
		goto _test_eof1279;
case 1279:
#line 35400 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 89: goto tr1501;
		case 95: goto st249;
		case 121: goto tr1502;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1501:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1280;
st1280:
	if ( ++p == pe )
		goto _test_eof1280;
case 1280:
#line 35442 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 80: goto tr1503;
		case 95: goto st249;
		case 112: goto tr1504;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1503:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1281;
st1281:
	if ( ++p == pe )
		goto _test_eof1281;
case 1281:
#line 35484 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1219;
		case 95: goto st249;
		case 101: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1504:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1282;
st1282:
	if ( ++p == pe )
		goto _test_eof1282;
case 1282:
#line 35526 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1237;
		case 95: goto st249;
		case 101: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1502:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1283;
st1283:
	if ( ++p == pe )
		goto _test_eof1283;
case 1283:
#line 35568 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 80: goto tr1505;
		case 95: goto st249;
		case 112: goto tr1504;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1505:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1284;
st1284:
	if ( ++p == pe )
		goto _test_eof1284;
case 1284:
#line 35610 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1237;
		case 95: goto st249;
		case 101: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1488:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1285;
st1285:
	if ( ++p == pe )
		goto _test_eof1285;
case 1285:
#line 35652 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 66: goto tr1295;
		case 83: goto tr1219;
		case 84: goto tr1343;
		case 95: goto st249;
		case 98: goto tr1296;
		case 115: goto tr1221;
		case 116: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1489:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1286;
st1286:
	if ( ++p == pe )
		goto _test_eof1286;
case 1286:
#line 35698 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1343;
		case 95: goto st249;
		case 97: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1490:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1287;
st1287:
	if ( ++p == pe )
		goto _test_eof1287;
case 1287:
#line 35740 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 72: goto tr1295;
		case 95: goto st249;
		case 104: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1491:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1288;
st1288:
	if ( ++p == pe )
		goto _test_eof1288;
case 1288:
#line 35782 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1260;
		case 110: goto tr1260;
		case 112: goto tr1440;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1492:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1289;
st1289:
	if ( ++p == pe )
		goto _test_eof1289;
case 1289:
#line 35825 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1398;
		case 95: goto st249;
		case 116: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1493:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1290;
st1290:
	if ( ++p == pe )
		goto _test_eof1290;
case 1290:
#line 35867 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1260;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1494:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1291;
st1291:
	if ( ++p == pe )
		goto _test_eof1291;
case 1291:
#line 35908 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto tr1506;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1506:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1292;
st1292:
	if ( ++p == pe )
		goto _test_eof1292;
case 1292:
#line 35949 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 117: goto tr1313;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1495:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1293;
st1293:
	if ( ++p == pe )
		goto _test_eof1293;
case 1293:
#line 35990 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 89: goto tr1507;
		case 95: goto st249;
		case 121: goto tr1502;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1507:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1294;
st1294:
	if ( ++p == pe )
		goto _test_eof1294;
case 1294:
#line 36032 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 80: goto tr1505;
		case 95: goto st249;
		case 112: goto tr1504;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1496:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1295;
st1295:
	if ( ++p == pe )
		goto _test_eof1295;
case 1295:
#line 36074 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 66: goto tr1298;
		case 83: goto tr1237;
		case 84: goto tr1398;
		case 95: goto st249;
		case 98: goto tr1296;
		case 115: goto tr1221;
		case 116: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1497:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1296;
st1296:
	if ( ++p == pe )
		goto _test_eof1296;
case 1296:
#line 36120 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st596;
		case 64: goto st250;
		case 65: goto tr1398;
		case 95: goto st249;
		case 97: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1498:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1297;
st1297:
	if ( ++p == pe )
		goto _test_eof1297;
case 1297:
#line 36162 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 72: goto tr1298;
		case 95: goto st249;
		case 104: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1499:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1298;
st1298:
	if ( ++p == pe )
		goto _test_eof1298;
case 1298:
#line 36204 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1243;
		case 110: goto tr1243;
		case 114: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1500:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1299;
st1299:
	if ( ++p == pe )
		goto _test_eof1299;
case 1299:
#line 36247 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1438;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr732:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1300;
st1300:
	if ( ++p == pe )
		goto _test_eof1300;
case 1300:
#line 36288 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 69: goto tr1508;
		case 95: goto st249;
		case 97: goto tr534;
		case 101: goto tr1509;
		case 104: goto tr1510;
		case 105: goto tr534;
		case 111: goto tr534;
		case 112: goto tr1511;
		case 114: goto tr1512;
		case 117: goto tr1513;
		case 119: goto tr1312;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1508:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1301;
st1301:
	if ( ++p == pe )
		goto _test_eof1301;
case 1301:
#line 36339 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1514;
		case 95: goto st249;
		case 108: goto tr1515;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1514:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1302;
st1302:
	if ( ++p == pe )
		goto _test_eof1302;
case 1302:
#line 36381 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 78: goto tr1516;
		case 95: goto st249;
		case 110: goto tr1517;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1516:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1303;
st1303:
	if ( ++p == pe )
		goto _test_eof1303;
case 1303:
#line 36423 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1329;
		case 95: goto st249;
		case 101: goto tr1330;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1517:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1304;
st1304:
	if ( ++p == pe )
		goto _test_eof1304;
case 1304:
#line 36465 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1335;
		case 95: goto st249;
		case 101: goto tr1330;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1515:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1305;
st1305:
	if ( ++p == pe )
		goto _test_eof1305;
case 1305:
#line 36507 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 78: goto tr1518;
		case 95: goto st249;
		case 110: goto tr1517;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1518:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1306;
st1306:
	if ( ++p == pe )
		goto _test_eof1306;
case 1306:
#line 36549 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1335;
		case 95: goto st249;
		case 101: goto tr1330;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1509:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1307;
st1307:
	if ( ++p == pe )
		goto _test_eof1307;
case 1307:
#line 36591 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 76: goto tr1519;
		case 95: goto st249;
		case 108: goto tr1515;
		case 110: goto tr1401;
		case 120: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1519:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1308;
st1308:
	if ( ++p == pe )
		goto _test_eof1308;
case 1308:
#line 36635 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 78: goto tr1518;
		case 95: goto st249;
		case 110: goto tr1517;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1510:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1309;
st1309:
	if ( ++p == pe )
		goto _test_eof1309;
case 1309:
#line 36677 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 117: goto tr1520;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1520:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1310;
st1310:
	if ( ++p == pe )
		goto _test_eof1310;
case 1310:
#line 36718 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1317;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1511:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1311;
st1311:
	if ( ++p == pe )
		goto _test_eof1311;
case 1311:
#line 36759 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 107: goto tr1521;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1521:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1312;
st1312:
	if ( ++p == pe )
		goto _test_eof1312;
case 1312:
#line 36800 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1512:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1313;
st1313:
	if ( ++p == pe )
		goto _test_eof1313;
case 1313:
#line 36841 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 112: goto tr1522;
	}
	if ( (*p) < 48 ) {
		if ( (*p) < -32 ) {
			if ( -61 <= (*p) && (*p) <= -33 )
				goto st3;
		} else if ( (*p) > -17 ) {
			if ( -16 <= (*p) && (*p) <= -12 )
				goto st9;
		} else
			goto st4;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr533;
		} else if ( (*p) > 106 ) {
			if ( (*p) > 108 ) {
				if ( 109 <= (*p) && (*p) <= 122 )
					goto tr534;
			} else if ( (*p) >= 107 )
				goto tr1243;
		} else
			goto tr534;
	} else
		goto tr532;
	goto tr763;
tr1522:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1314;
st1314:
	if ( ++p == pe )
		goto _test_eof1314;
case 1314:
#line 36888 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 107: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1513:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1315;
st1315:
	if ( ++p == pe )
		goto _test_eof1315;
case 1315:
#line 36929 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr733:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1316;
st1316:
	if ( ++p == pe )
		goto _test_eof1316;
case 1316:
#line 36970 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 78: goto tr1523;
		case 95: goto st249;
		case 97: goto tr534;
		case 101: goto tr534;
		case 105: goto tr534;
		case 110: goto tr1524;
		case 111: goto tr534;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1523:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1317;
st1317:
	if ( ++p == pe )
		goto _test_eof1317;
case 1317:
#line 37019 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1525;
		case 95: goto st249;
		case 114: goto tr1526;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1525:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1318;
st1318:
	if ( ++p == pe )
		goto _test_eof1318;
case 1318:
#line 37061 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1527;
		case 95: goto st249;
		case 101: goto tr1528;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1527:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1319;
st1319:
	if ( ++p == pe )
		goto _test_eof1319;
case 1319:
#line 37103 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1529;
		case 95: goto st249;
		case 97: goto tr1530;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1529:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1320;
st1320:
	if ( ++p == pe )
		goto _test_eof1320;
case 1320:
#line 37145 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1295;
		case 95: goto st249;
		case 108: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1530:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1321;
st1321:
	if ( ++p == pe )
		goto _test_eof1321;
case 1321:
#line 37187 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1298;
		case 95: goto st249;
		case 108: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1528:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1322;
st1322:
	if ( ++p == pe )
		goto _test_eof1322;
case 1322:
#line 37229 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1531;
		case 95: goto st249;
		case 97: goto tr1530;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1531:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1323;
st1323:
	if ( ++p == pe )
		goto _test_eof1323;
case 1323:
#line 37271 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1298;
		case 95: goto st249;
		case 108: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1526:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1324;
st1324:
	if ( ++p == pe )
		goto _test_eof1324;
case 1324:
#line 37313 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1532;
		case 95: goto st249;
		case 101: goto tr1528;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1532:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1325;
st1325:
	if ( ++p == pe )
		goto _test_eof1325;
case 1325:
#line 37355 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1531;
		case 95: goto st249;
		case 97: goto tr1530;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1524:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1326;
st1326:
	if ( ++p == pe )
		goto _test_eof1326;
case 1326:
#line 37397 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1533;
		case 95: goto st249;
		case 105: goto tr1534;
		case 114: goto tr1526;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1533:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1327;
st1327:
	if ( ++p == pe )
		goto _test_eof1327;
case 1327:
#line 37440 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1532;
		case 95: goto st249;
		case 101: goto tr1528;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1534:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1328;
st1328:
	if ( ++p == pe )
		goto _test_eof1328;
case 1328:
#line 37482 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1236;
		case 64: goto st250;
		case 95: goto st249;
		case 118: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr734:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1329;
st1329:
	if ( ++p == pe )
		goto _test_eof1329;
case 1329:
#line 37523 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1243;
		case 100: goto tr1535;
		case 101: goto tr534;
		case 105: goto tr1458;
		case 108: goto tr1536;
		case 111: goto tr534;
		case 116: goto tr1537;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1535:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1330;
st1330:
	if ( ++p == pe )
		goto _test_eof1330;
case 1330:
#line 37573 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1261;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1536:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1331;
st1331:
	if ( ++p == pe )
		goto _test_eof1331;
case 1331:
#line 37614 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 103: goto tr1243;
		case 121: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1537:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1332;
st1332:
	if ( ++p == pe )
		goto _test_eof1332;
case 1332:
#line 37656 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr735:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1333;
st1333:
	if ( ++p == pe )
		goto _test_eof1333;
case 1333:
#line 37696 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr1538;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1539;
		case 64: goto st250;
		case 65: goto tr1331;
		case 69: goto tr1540;
		case 79: goto tr1541;
		case 83: goto tr1219;
		case 95: goto st249;
		case 97: goto tr1542;
		case 101: goto tr1543;
		case 105: goto tr1544;
		case 108: goto tr1276;
		case 111: goto tr1545;
		case 115: goto tr1546;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr1310;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1538:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st599;
st599:
	if ( ++p == pe )
		goto _test_eof599;
case 599:
#line 37747 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 101: goto tr671;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr671:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1334;
st1334:
	if ( ++p == pe )
		goto _test_eof1334;
case 1334:
#line 37779 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 110: goto tr1547;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1547:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1335;
st1335:
	if ( ++p == pe )
		goto _test_eof1335;
case 1335:
#line 37817 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
		case 100: goto tr1280;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1539:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1336;
st1336:
	if ( ++p == pe )
		goto _test_eof1336;
case 1336:
#line 37855 "tokenizer.cc"
	switch( (*p) ) {
		case -30: goto st201;
		case 32: goto st600;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st249;
		} else if ( (*p) >= 9 )
			goto st600;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr677;
		} else if ( (*p) >= 65 )
			goto tr677;
	} else
		goto st249;
	goto tr1139;
st600:
	if ( ++p == pe )
		goto _test_eof600;
case 600:
	switch( (*p) ) {
		case 32: goto st600;
		case 86: goto st601;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st600;
	goto tr530;
st601:
	if ( ++p == pe )
		goto _test_eof601;
case 601:
	if ( (*p) == 97 )
		goto st593;
	goto tr530;
tr1540:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1337;
st1337:
	if ( ++p == pe )
		goto _test_eof1337;
case 1337:
#line 37908 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 66: goto tr1548;
		case 82: goto tr1129;
		case 95: goto st249;
		case 98: goto tr1549;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1548:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1338;
st1338:
	if ( ++p == pe )
		goto _test_eof1338;
case 1338:
#line 37951 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1527;
		case 95: goto st249;
		case 99: goto tr1528;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1549:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1339;
st1339:
	if ( ++p == pe )
		goto _test_eof1339;
case 1339:
#line 37993 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1532;
		case 95: goto st249;
		case 99: goto tr1528;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1541:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 34;}
	goto st1340;
st1340:
	if ( ++p == pe )
		goto _test_eof1340;
case 1340:
#line 38035 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 78: goto tr1220;
		case 85: goto tr1301;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr1139;
tr1542:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1341;
st1341:
	if ( ++p == pe )
		goto _test_eof1341;
case 1341:
#line 38077 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1550;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1550:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1342;
st1342:
	if ( ++p == pe )
		goto _test_eof1342;
case 1342:
#line 38118 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 104: goto tr1243;
		case 110: goto tr1238;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1543:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1343;
st1343:
	if ( ++p == pe )
		goto _test_eof1343;
case 1343:
#line 38160 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 66: goto tr1551;
		case 95: goto st249;
		case 98: goto tr1549;
		case 100: goto tr1243;
		case 114: goto tr1384;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1551:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1344;
st1344:
	if ( ++p == pe )
		goto _test_eof1344;
case 1344:
#line 38204 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1532;
		case 95: goto st249;
		case 99: goto tr1528;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1544:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1345;
st1345:
	if ( ++p == pe )
		goto _test_eof1345;
case 1345:
#line 38246 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1552;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1552:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1346;
st1346:
	if ( ++p == pe )
		goto _test_eof1346;
case 1346:
#line 38287 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1244;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1243;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1545:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1347;
st1347:
	if ( ++p == pe )
		goto _test_eof1347;
case 1347:
#line 38328 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1238;
		case 117: goto tr1313;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1546:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1348;
st1348:
	if ( ++p == pe )
		goto _test_eof1348;
case 1348:
#line 38370 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st554;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st526;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr736:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1349;
st1349:
	if ( ++p == pe )
		goto _test_eof1349;
case 1349:
#line 38411 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr913;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr534;
		case 101: goto tr534;
		case 105: goto tr534;
		case 111: goto tr534;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr737:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1350;
st1350:
	if ( ++p == pe )
		goto _test_eof1350;
case 1350:
#line 38458 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st487;
		case 39: goto tr1553;
		case 43: goto st490;
		case 45: goto tr1124;
		case 46: goto tr1250;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1554;
		case 101: goto tr534;
		case 105: goto tr534;
		case 111: goto tr534;
		case 117: goto tr534;
		case 119: goto tr534;
		case 121: goto tr534;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr1130;
		} else
			goto tr1126;
	} else
		goto st9;
	goto tr763;
tr1553:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st602;
st602:
	if ( ++p == pe )
		goto _test_eof602;
case 602:
#line 38503 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 97: goto tr674;
		case 100: goto tr254;
		case 107: goto tr675;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr674:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1351;
st1351:
	if ( ++p == pe )
		goto _test_eof1351;
case 1351:
#line 38536 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 108: goto tr1555;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1555:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1352;
st1352:
	if ( ++p == pe )
		goto _test_eof1352;
case 1352:
#line 38574 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
		case 108: goto tr1280;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr675:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1353;
st1353:
	if ( ++p == pe )
		goto _test_eof1353;
case 1353:
#line 38612 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 110: goto tr1556;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1556:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1354;
st1354:
	if ( ++p == pe )
		goto _test_eof1354;
case 1354:
#line 38650 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
		case 111: goto tr1557;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1557:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1355;
st1355:
	if ( ++p == pe )
		goto _test_eof1355;
case 1355:
#line 38688 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
		case 119: goto tr1280;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1554:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1356;
st1356:
	if ( ++p == pe )
		goto _test_eof1356;
case 1356:
#line 38726 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1558;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1558:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st603;
st603:
	if ( ++p == pe )
		goto _test_eof603;
case 603:
#line 38764 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr676;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr676:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1357;
st1357:
	if ( ++p == pe )
		goto _test_eof1357;
case 1357:
#line 38795 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 108: goto tr918;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
st1358:
	if ( ++p == pe )
		goto _test_eof1358;
case 1358:
	if ( (*p) == 96 )
		goto tr96;
	goto tr896;
tr740:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1359;
st1359:
	if ( ++p == pe )
		goto _test_eof1359;
case 1359:
#line 38840 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st604;
		case 64: goto st250;
		case 73: goto tr1560;
		case 95: goto st249;
		case 99: goto tr1561;
		case 100: goto tr1562;
		case 105: goto tr1132;
		case 109: goto tr1563;
		case 110: goto tr1564;
		case 112: goto tr1565;
		case 114: goto tr1384;
		case 118: goto tr1566;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st604:
	if ( ++p == pe )
		goto _test_eof604;
case 604:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr678;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr677;
		} else if ( (*p) >= 65 )
			goto tr677;
	} else
		goto st249;
	goto tr14;
tr1560:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1360;
st1360:
	if ( ++p == pe )
		goto _test_eof1360;
case 1360:
#line 38914 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1237;
		case 95: goto st249;
		case 109: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1561:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1361;
st1361:
	if ( ++p == pe )
		goto _test_eof1361;
case 1361:
#line 38956 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1567;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1567:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1362;
st1362:
	if ( ++p == pe )
		goto _test_eof1362;
case 1362:
#line 38997 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1562:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1363;
st1363:
	if ( ++p == pe )
		goto _test_eof1363;
case 1363:
#line 39038 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1230;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1563:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1364;
st1364:
	if ( ++p == pe )
		goto _test_eof1364;
case 1364:
#line 39079 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto tr1568;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1568:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1365;
st1365:
	if ( ++p == pe )
		goto _test_eof1365;
case 1365:
#line 39120 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1336;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1564:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1366;
st1366:
	if ( ++p == pe )
		goto _test_eof1366;
case 1366:
#line 39161 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1569;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1569:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1367;
st1367:
	if ( ++p == pe )
		goto _test_eof1367;
case 1367:
#line 39202 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 47: goto st605;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st605:
	if ( ++p == pe )
		goto _test_eof605;
case 605:
	if ( (*p) == 111 )
		goto st606;
	goto tr14;
st606:
	if ( ++p == pe )
		goto _test_eof606;
case 606:
	if ( (*p) == 114 )
		goto tr623;
	goto tr14;
tr1565:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1368;
st1368:
	if ( ++p == pe )
		goto _test_eof1368;
case 1368:
#line 39257 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 112: goto tr1571;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1571:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1369;
st1369:
	if ( ++p == pe )
		goto _test_eof1369;
case 1369:
#line 39298 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1572;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1572:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1370;
st1370:
	if ( ++p == pe )
		goto _test_eof1370;
case 1370:
#line 39339 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto tr1573;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1573:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1371;
st1371:
	if ( ++p == pe )
		goto _test_eof1371;
case 1371:
#line 39380 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 120: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1566:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1372;
st1372:
	if ( ++p == pe )
		goto _test_eof1372;
case 1372:
#line 39421 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 103: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr741:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1373;
st1373:
	if ( ++p == pe )
		goto _test_eof1373;
case 1373:
#line 39462 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st607:
	if ( ++p == pe )
		goto _test_eof607;
case 607:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr677;
		} else if ( (*p) >= 65 )
			goto tr677;
	} else
		goto st249;
	goto tr14;
tr742:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1374;
st1374:
	if ( ++p == pe )
		goto _test_eof1374;
case 1374:
#line 39526 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1262;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 65: goto tr1575;
		case 72: goto tr1576;
		case 73: goto tr1349;
		case 86: goto tr1577;
		case 95: goto st249;
		case 97: goto tr1578;
		case 101: goto tr1568;
		case 102: goto tr1235;
		case 104: goto tr1579;
		case 105: goto tr1272;
		case 111: goto tr1580;
		case 118: goto tr1581;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1575:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1375;
st1375:
	if ( ++p == pe )
		goto _test_eof1375;
case 1375:
#line 39577 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1303;
		case 95: goto st249;
		case 108: goto tr1282;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1576:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1376;
st1376:
	if ( ++p == pe )
		goto _test_eof1376;
case 1376:
#line 39619 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1307;
		case 95: goto st249;
		case 114: goto tr1290;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1577:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1377;
st1377:
	if ( ++p == pe )
		goto _test_eof1377;
case 1377:
#line 39661 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1298;
		case 95: goto st249;
		case 115: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1578:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1378;
st1378:
	if ( ++p == pe )
		goto _test_eof1378;
case 1378:
#line 39703 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1303;
		case 95: goto st249;
		case 108: goto tr1282;
		case 110: goto tr1238;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1579:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1379;
st1379:
	if ( ++p == pe )
		goto _test_eof1379;
case 1379:
#line 39746 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1307;
		case 95: goto st249;
		case 114: goto tr1290;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1580:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1380;
st1380:
	if ( ++p == pe )
		goto _test_eof1380;
case 1380:
#line 39788 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1582;
		case 117: goto tr1313;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1582:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1381;
st1381:
	if ( ++p == pe )
		goto _test_eof1381;
case 1381:
#line 39830 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1583;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1583:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1382;
st1382:
	if ( ++p == pe )
		goto _test_eof1382;
case 1382:
#line 39871 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1584;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1236;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1584:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st608;
st608:
	if ( ++p == pe )
		goto _test_eof608;
case 608:
#line 39909 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr680;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr1581:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1383;
st1383:
	if ( ++p == pe )
		goto _test_eof1383;
case 1383:
#line 39940 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1298;
		case 95: goto st249;
		case 115: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr743:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1384;
st1384:
	if ( ++p == pe )
		goto _test_eof1384;
case 1384:
#line 39982 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 65: goto tr1585;
		case 73: goto tr1586;
		case 78: goto tr1577;
		case 79: goto tr1587;
		case 95: goto st249;
		case 97: goto tr1322;
		case 101: goto tr1588;
		case 105: goto tr1589;
		case 110: goto tr1581;
		case 111: goto tr1325;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1585:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1385;
st1385:
	if ( ++p == pe )
		goto _test_eof1385;
case 1385:
#line 40031 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1332;
		case 86: goto tr1298;
		case 95: goto st249;
		case 116: goto tr1328;
		case 118: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1586:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1386;
st1386:
	if ( ++p == pe )
		goto _test_eof1386;
case 1386:
#line 40075 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1335;
		case 95: goto st249;
		case 99: goto tr1330;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1587:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1387;
st1387:
	if ( ++p == pe )
		goto _test_eof1387;
case 1387:
#line 40117 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 73: goto tr1237;
		case 95: goto st249;
		case 105: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1588:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1388;
st1388:
	if ( ++p == pe )
		goto _test_eof1388;
case 1388:
#line 40159 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 112: goto tr1590;
		case 118: goto tr1591;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1590:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1389;
st1389:
	if ( ++p == pe )
		goto _test_eof1389;
case 1389:
#line 40201 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1334;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1591:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1390;
st1390:
	if ( ++p == pe )
		goto _test_eof1390;
case 1390:
#line 40241 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1592;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1592:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1391;
st1391:
	if ( ++p == pe )
		goto _test_eof1391;
case 1391:
#line 40282 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1589:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1392;
st1392:
	if ( ++p == pe )
		goto _test_eof1392;
case 1392:
#line 40323 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1335;
		case 95: goto st249;
		case 99: goto tr1330;
		case 100: goto tr1245;
		case 115: goto tr1593;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1593:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1393;
st1393:
	if ( ++p == pe )
		goto _test_eof1393;
case 1393:
#line 40367 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1594;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1594:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1394;
st1394:
	if ( ++p == pe )
		goto _test_eof1394;
case 1394:
#line 40408 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1236;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto tr1595;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1595:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1395;
st1395:
	if ( ++p == pe )
		goto _test_eof1395;
case 1395:
#line 40449 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1336;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr744:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1396;
st1396:
	if ( ++p == pe )
		goto _test_eof1396;
case 1396:
#line 40490 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st609;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1249;
		case 103: goto tr1235;
		case 113: goto tr1597;
		case 115: goto tr1336;
		case 116: goto tr1340;
		case 120: goto tr1598;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st609:
	if ( ++p == pe )
		goto _test_eof609;
case 609:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 103: goto tr681;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr677;
		} else if ( (*p) >= 65 )
			goto tr677;
	} else
		goto st249;
	goto tr14;
tr681:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1397;
st1397:
	if ( ++p == pe )
		goto _test_eof1397;
case 1397:
#line 40561 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 43: goto st249;
		case 45: goto tr1152;
		case 46: goto tr1217;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr575;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr578;
		} else
			goto tr577;
	} else
		goto st9;
	goto tr763;
tr1597:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1398;
st1398:
	if ( ++p == pe )
		goto _test_eof1398;
case 1398:
#line 40601 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 117: goto tr1599;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1599:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1399;
st1399:
	if ( ++p == pe )
		goto _test_eof1399;
case 1399:
#line 40642 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1600;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1600:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1400;
st1400:
	if ( ++p == pe )
		goto _test_eof1400;
case 1400:
#line 40683 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 118: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1598:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1401;
st1401:
	if ( ++p == pe )
		goto _test_eof1401;
case 1401:
#line 40724 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1592;
		case 112: goto tr1601;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1601:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1402;
st1402:
	if ( ++p == pe )
		goto _test_eof1402;
case 1402:
#line 40766 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1602;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1602:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1403;
st1403:
	if ( ++p == pe )
		goto _test_eof1403;
case 1403:
#line 40807 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1592;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr745:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1404;
st1404:
	if ( ++p == pe )
		goto _test_eof1404;
case 1404:
#line 40848 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 69: goto tr1603;
		case 73: goto tr1604;
		case 84: goto tr1398;
		case 95: goto st249;
		case 97: goto tr1605;
		case 101: goto tr1606;
		case 105: goto tr1607;
		case 116: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1603:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1405;
st1405:
	if ( ++p == pe )
		goto _test_eof1405;
case 1405:
#line 40895 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1349;
		case 95: goto st249;
		case 101: goto tr1272;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1604:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1406;
st1406:
	if ( ++p == pe )
		goto _test_eof1406;
case 1406:
#line 40937 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1299;
		case 95: goto st249;
		case 108: goto tr1294;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1605:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1407;
st1407:
	if ( ++p == pe )
		goto _test_eof1407;
case 1407:
#line 40979 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 120: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1606:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1408;
st1408:
	if ( ++p == pe )
		goto _test_eof1408;
case 1408:
#line 41020 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 69: goto tr1349;
		case 95: goto st249;
		case 101: goto tr1272;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1607:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1409;
st1409:
	if ( ++p == pe )
		goto _test_eof1409;
case 1409:
#line 41062 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1299;
		case 95: goto st249;
		case 108: goto tr1294;
		case 110: goto tr1608;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1608:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1410;
st1410:
	if ( ++p == pe )
		goto _test_eof1410;
case 1410:
#line 41105 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1236;
		case 64: goto st250;
		case 95: goto st249;
		case 103: goto tr1609;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1609:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1411;
st1411:
	if ( ++p == pe )
		goto _test_eof1411;
case 1411:
#line 41146 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1610;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1610:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1412;
st1412:
	if ( ++p == pe )
		goto _test_eof1412;
case 1412:
#line 41187 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr746:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1413;
st1413:
	if ( ++p == pe )
		goto _test_eof1413;
case 1413:
#line 41228 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 69: goto tr1287;
		case 73: goto tr1335;
		case 79: goto tr1611;
		case 84: goto tr1612;
		case 95: goto st249;
		case 101: goto tr1286;
		case 105: goto tr1330;
		case 111: goto tr1613;
		case 116: goto tr1614;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1611:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1414;
st1414:
	if ( ++p == pe )
		goto _test_eof1414;
case 1414:
#line 41276 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 80: goto tr1368;
		case 95: goto st249;
		case 112: goto tr1356;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1612:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1415;
st1415:
	if ( ++p == pe )
		goto _test_eof1415;
case 1415:
#line 41319 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1370;
		case 95: goto st249;
		case 97: goto tr1364;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1613:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1416;
st1416:
	if ( ++p == pe )
		goto _test_eof1416;
case 1416:
#line 41361 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 80: goto tr1368;
		case 95: goto st249;
		case 112: goto tr1356;
		case 118: goto tr1615;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1615:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1417;
st1417:
	if ( ++p == pe )
		goto _test_eof1417;
case 1417:
#line 41405 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1616;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1616:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st610;
st610:
	if ( ++p == pe )
		goto _test_eof610;
case 610:
#line 41443 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 116: goto tr682;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr1614:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1418;
st1418:
	if ( ++p == pe )
		goto _test_eof1418;
case 1418:
#line 41475 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1370;
		case 95: goto st249;
		case 97: goto tr1364;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr747:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1419;
st1419:
	if ( ++p == pe )
		goto _test_eof1419;
case 1419:
#line 41517 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 84: goto tr1617;
		case 95: goto st249;
		case 97: goto tr1618;
		case 116: goto tr1619;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1617:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1420;
st1420:
	if ( ++p == pe )
		goto _test_eof1420;
case 1420:
#line 41560 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1389;
		case 95: goto st249;
		case 116: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1618:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1421;
st1421:
	if ( ++p == pe )
		goto _test_eof1421;
case 1421:
#line 41602 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1245;
		case 115: goto tr1245;
		case 118: goto tr1384;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1619:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1422;
st1422:
	if ( ++p == pe )
		goto _test_eof1422;
case 1422:
#line 41645 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1389;
		case 95: goto st249;
		case 116: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr748:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1423;
st1423:
	if ( ++p == pe )
		goto _test_eof1423;
case 1423:
#line 41687 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st611;
		case 64: goto st250;
		case 77: goto tr1621;
		case 82: goto tr1622;
		case 95: goto st249;
		case 101: goto tr1235;
		case 109: goto tr1623;
		case 110: goto tr1624;
		case 114: goto tr1625;
		case 115: goto tr1245;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st611:
	if ( ++p == pe )
		goto _test_eof611;
case 611:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr681;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr677;
		} else if ( (*p) >= 65 )
			goto tr677;
	} else
		goto st249;
	goto tr14;
tr1621:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1424;
st1424:
	if ( ++p == pe )
		goto _test_eof1424;
case 1424:
#line 41759 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 65: goto tr1398;
		case 95: goto st249;
		case 97: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1622:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1425;
st1425:
	if ( ++p == pe )
		goto _test_eof1425;
case 1425:
#line 41802 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1383;
		case 95: goto st249;
		case 99: goto tr1382;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1623:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1426;
st1426:
	if ( ++p == pe )
		goto _test_eof1426;
case 1426:
#line 41844 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 58: goto st582;
		case 64: goto st250;
		case 65: goto tr1398;
		case 95: goto st249;
		case 97: goto tr1397;
		case 112: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1624:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1427;
st1427:
	if ( ++p == pe )
		goto _test_eof1427;
case 1427:
#line 41888 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1592;
		case 115: goto tr1235;
		case 116: goto tr1626;
		case 118: goto tr1627;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1626:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1428;
st1428:
	if ( ++p == pe )
		goto _test_eof1428;
case 1428:
#line 41932 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1628;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1628:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st612;
st612:
	if ( ++p == pe )
		goto _test_eof612;
case 612:
#line 41970 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr683;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr1627:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1429;
st1429:
	if ( ++p == pe )
		goto _test_eof1429;
case 1429:
#line 42001 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto tr1236;
		case 64: goto st250;
		case 95: goto st249;
		case 101: goto tr1629;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1629:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1430;
st1430:
	if ( ++p == pe )
		goto _test_eof1430;
case 1430:
#line 42042 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 115: goto tr1336;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1625:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1431;
st1431:
	if ( ++p == pe )
		goto _test_eof1431;
case 1431:
#line 42083 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 67: goto tr1383;
		case 95: goto st249;
		case 99: goto tr1382;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr749:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1432;
st1432:
	if ( ++p == pe )
		goto _test_eof1432;
case 1432:
#line 42125 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 65: goto tr1630;
		case 68: goto tr1631;
		case 95: goto st249;
		case 97: goto tr1632;
		case 100: goto tr1633;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1630:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1433;
st1433:
	if ( ++p == pe )
		goto _test_eof1433;
case 1433:
#line 42169 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1421;
		case 95: goto st249;
		case 115: goto tr1414;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1631:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1434;
st1434:
	if ( ++p == pe )
		goto _test_eof1434;
case 1434:
#line 42211 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1389;
		case 95: goto st249;
		case 97: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1632:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1435;
st1435:
	if ( ++p == pe )
		goto _test_eof1435;
case 1435:
#line 42253 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 83: goto tr1421;
		case 95: goto st249;
		case 115: goto tr1414;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1633:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1436;
st1436:
	if ( ++p == pe )
		goto _test_eof1436;
case 1436:
#line 42295 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1389;
		case 95: goto st249;
		case 97: goto tr1380;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr750:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1437;
st1437:
	if ( ++p == pe )
		goto _test_eof1437;
case 1437:
#line 42337 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 65: goto tr1634;
		case 77: goto tr1577;
		case 95: goto st249;
		case 97: goto tr1635;
		case 109: goto tr1581;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1634:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1438;
st1438:
	if ( ++p == pe )
		goto _test_eof1438;
case 1438:
#line 42381 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 73: goto tr1303;
		case 95: goto st249;
		case 105: goto tr1282;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1635:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1439;
st1439:
	if ( ++p == pe )
		goto _test_eof1439;
case 1439:
#line 42423 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1636;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 73: goto tr1303;
		case 95: goto st249;
		case 105: goto tr1282;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1636:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st613;
st613:
	if ( ++p == pe )
		goto _test_eof613;
case 613:
#line 42463 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 97: goto tr684;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr684:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1440;
st1440:
	if ( ++p == pe )
		goto _test_eof1440;
case 1440:
#line 42495 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 109: goto tr1637;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr751:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1441;
st1441:
	if ( ++p == pe )
		goto _test_eof1441;
case 1441:
#line 42533 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 69: goto tr1638;
		case 95: goto st249;
		case 97: goto tr1639;
		case 101: goto tr1640;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1638:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1442;
st1442:
	if ( ++p == pe )
		goto _test_eof1442;
case 1442:
#line 42576 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 87: goto tr1450;
		case 95: goto st249;
		case 119: goto tr1449;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1639:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1443;
st1443:
	if ( ++p == pe )
		goto _test_eof1443;
case 1443:
#line 42618 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 116: goto tr1641;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1641:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1444;
st1444:
	if ( ++p == pe )
		goto _test_eof1444;
case 1444:
#line 42659 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1642;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1642:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st614;
st614:
	if ( ++p == pe )
		goto _test_eof614;
case 614:
#line 42697 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr685;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr1640:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1445;
st1445:
	if ( ++p == pe )
		goto _test_eof1445;
case 1445:
#line 42728 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 87: goto tr1450;
		case 95: goto st249;
		case 119: goto tr1449;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr752:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1446;
st1446:
	if ( ++p == pe )
		goto _test_eof1446;
case 1446:
#line 42770 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1643;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1567;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1643:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st615;
st615:
	if ( ++p == pe )
		goto _test_eof615;
case 615:
#line 42809 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 99: goto tr668;
		case 100: goto tr254;
		case 101: goto tr686;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr686:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1447;
st1447:
	if ( ++p == pe )
		goto _test_eof1447;
case 1447:
#line 42842 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 114: goto tr1644;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr753:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1448;
st1448:
	if ( ++p == pe )
		goto _test_eof1448;
case 1448:
#line 42880 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st616;
		case 64: goto st250;
		case 79: goto tr1398;
		case 95: goto st249;
		case 101: goto tr1602;
		case 111: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
st616:
	if ( ++p == pe )
		goto _test_eof616;
case 616:
	switch( (*p) ) {
		case -30: goto st201;
		case 38: goto st204;
		case 39: goto tr221;
		case 43: goto st249;
		case 64: goto st250;
		case 95: goto st249;
		case 109: goto tr687;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st249;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr677;
		} else if ( (*p) >= 65 )
			goto tr677;
	} else
		goto st249;
	goto tr14;
tr754:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1449;
st1449:
	if ( ++p == pe )
		goto _test_eof1449;
case 1449:
#line 42948 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 95: goto st249;
		case 117: goto tr1646;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1646:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1450;
st1450:
	if ( ++p == pe )
		goto _test_eof1450;
case 1450:
#line 42989 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1470;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr755:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1451;
st1451:
	if ( ++p == pe )
		goto _test_eof1451;
case 1451:
#line 43030 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 77: goto tr1647;
		case 83: goto tr1648;
		case 84: goto tr1649;
		case 95: goto st249;
		case 101: goto tr1650;
		case 109: goto tr1651;
		case 111: goto tr1652;
		case 115: goto tr1653;
		case 116: goto tr1654;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1647:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1452;
st1452:
	if ( ++p == pe )
		goto _test_eof1452;
case 1452:
#line 43078 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 73: goto tr1298;
		case 95: goto st249;
		case 105: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1648:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1453;
st1453:
	if ( ++p == pe )
		goto _test_eof1453;
case 1453:
#line 43120 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 89: goto tr1484;
		case 95: goto st249;
		case 121: goto tr1480;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1649:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1454;
st1454:
	if ( ++p == pe )
		goto _test_eof1454;
case 1454:
#line 43162 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1398;
		case 95: goto st249;
		case 109: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1650:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1455;
st1455:
	if ( ++p == pe )
		goto _test_eof1455;
case 1455:
#line 43204 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1655;
		case 112: goto tr1656;
		case 113: goto tr1657;
		case 118: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1655:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1456;
st1456:
	if ( ++p == pe )
		goto _test_eof1456;
case 1456:
#line 43248 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 100: goto tr1658;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1658:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1457;
st1457:
	if ( ++p == pe )
		goto _test_eof1457;
case 1457:
#line 43289 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1659;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1659:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1458;
st1458:
	if ( ++p == pe )
		goto _test_eof1458;
case 1458:
#line 43330 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1660;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1660:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1459;
st1459:
	if ( ++p == pe )
		goto _test_eof1459;
case 1459:
#line 43371 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1661;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1661:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st1460;
st1460:
	if ( ++p == pe )
		goto _test_eof1460;
case 1460:
#line 43409 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr1662;
tr1656:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1461;
st1461:
	if ( ++p == pe )
		goto _test_eof1461;
case 1461:
#line 43440 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 114: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1657:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1462;
st1462:
	if ( ++p == pe )
		goto _test_eof1462;
case 1462:
#line 43481 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1663;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1663:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st617;
st617:
	if ( ++p == pe )
		goto _test_eof617;
case 617:
#line 43519 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr688;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr1651:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1463;
st1463:
	if ( ++p == pe )
		goto _test_eof1463;
case 1463:
#line 43550 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 73: goto tr1298;
		case 95: goto st249;
		case 105: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1652:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1464;
st1464:
	if ( ++p == pe )
		goto _test_eof1464;
case 1464:
#line 43592 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 99: goto tr1664;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1664:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1465;
st1465:
	if ( ++p == pe )
		goto _test_eof1465;
case 1465:
#line 43633 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 107: goto tr1665;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1665:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1466;
st1466:
	if ( ++p == pe )
		goto _test_eof1466;
case 1466:
#line 43674 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1666;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1666:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st618;
st618:
	if ( ++p == pe )
		goto _test_eof618;
case 618:
#line 43712 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 108: goto tr255;
		case 109: goto tr256;
		case 110: goto tr689;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr689:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1467;
st1467:
	if ( ++p == pe )
		goto _test_eof1467;
case 1467:
#line 43744 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1667;
		case 45: goto tr911;
		case 46: goto st200;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1667:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st619;
st619:
	if ( ++p == pe )
		goto _test_eof619;
case 619:
#line 43779 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr171;
		case 76: goto st172;
		case 77: goto tr173;
		case 82: goto st173;
		case 83: goto tr175;
		case 86: goto st174;
		case 100: goto tr177;
		case 108: goto st175;
		case 109: goto tr179;
		case 114: goto st620;
		case 115: goto tr175;
		case 118: goto st177;
	}
	goto tr14;
st620:
	if ( ++p == pe )
		goto _test_eof620;
case 620:
	switch( (*p) ) {
		case 101: goto tr186;
		case 111: goto st621;
	}
	goto tr14;
st621:
	if ( ++p == pe )
		goto _test_eof621;
case 621:
	if ( (*p) == 108 )
		goto st622;
	goto tr14;
st622:
	if ( ++p == pe )
		goto _test_eof622;
case 622:
	if ( (*p) == 108 )
		goto tr275;
	goto tr14;
tr1653:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1468;
st1468:
	if ( ++p == pe )
		goto _test_eof1468;
case 1468:
#line 43828 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 89: goto tr1484;
		case 95: goto st249;
		case 121: goto tr1480;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1654:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1469;
st1469:
	if ( ++p == pe )
		goto _test_eof1469;
case 1469:
#line 43870 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 77: goto tr1398;
		case 95: goto st249;
		case 109: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr756:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1470;
st1470:
	if ( ++p == pe )
		goto _test_eof1470;
case 1470:
#line 43912 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 70: goto tr1668;
		case 75: goto tr1669;
		case 77: goto tr1670;
		case 79: goto tr1671;
		case 83: goto tr1672;
		case 95: goto st249;
		case 102: goto tr1673;
		case 104: goto tr1674;
		case 107: goto tr1675;
		case 109: goto tr1676;
		case 111: goto tr1677;
		case 115: goto tr1678;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1668:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1471;
st1471:
	if ( ++p == pe )
		goto _test_eof1471;
case 1471:
#line 43963 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1398;
		case 95: goto st249;
		case 116: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1669:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1472;
st1472:
	if ( ++p == pe )
		goto _test_eof1472;
case 1472:
#line 44005 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 89: goto tr1507;
		case 95: goto st249;
		case 121: goto tr1502;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1670:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1473;
st1473:
	if ( ++p == pe )
		goto _test_eof1473;
case 1473:
#line 44047 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 66: goto tr1298;
		case 83: goto tr1237;
		case 84: goto tr1398;
		case 95: goto st249;
		case 98: goto tr1296;
		case 115: goto tr1221;
		case 116: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1671:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1474;
st1474:
	if ( ++p == pe )
		goto _test_eof1474;
case 1474:
#line 44093 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1398;
		case 95: goto st249;
		case 97: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1672:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1475;
st1475:
	if ( ++p == pe )
		goto _test_eof1475;
case 1475:
#line 44135 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 72: goto tr1298;
		case 95: goto st249;
		case 104: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1673:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1476;
st1476:
	if ( ++p == pe )
		goto _test_eof1476;
case 1476:
#line 44177 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 84: goto tr1398;
		case 95: goto st249;
		case 116: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1674:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1477;
st1477:
	if ( ++p == pe )
		goto _test_eof1477;
case 1477:
#line 44219 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 111: goto tr1506;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1675:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1478;
st1478:
	if ( ++p == pe )
		goto _test_eof1478;
case 1478:
#line 44260 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 89: goto tr1507;
		case 95: goto st249;
		case 121: goto tr1502;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1676:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1479;
st1479:
	if ( ++p == pe )
		goto _test_eof1479;
case 1479:
#line 44302 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 66: goto tr1298;
		case 83: goto tr1237;
		case 84: goto tr1398;
		case 95: goto st249;
		case 98: goto tr1296;
		case 115: goto tr1221;
		case 116: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1677:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1480;
st1480:
	if ( ++p == pe )
		goto _test_eof1480;
case 1480:
#line 44348 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 65: goto tr1398;
		case 95: goto st249;
		case 97: goto tr1397;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 66 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1678:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1481;
st1481:
	if ( ++p == pe )
		goto _test_eof1481;
case 1481:
#line 44390 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 72: goto tr1298;
		case 95: goto st249;
		case 104: goto tr1296;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr757:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1482;
st1482:
	if ( ++p == pe )
		goto _test_eof1482;
case 1482:
#line 44432 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 69: goto tr1679;
		case 95: goto st249;
		case 101: goto tr1680;
		case 114: goto tr1681;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1679:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1483;
st1483:
	if ( ++p == pe )
		goto _test_eof1483;
case 1483:
#line 44475 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1519;
		case 95: goto st249;
		case 108: goto tr1515;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1680:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1484;
st1484:
	if ( ++p == pe )
		goto _test_eof1484;
case 1484:
#line 44517 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 76: goto tr1519;
		case 95: goto st249;
		case 108: goto tr1515;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1681:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1485;
st1485:
	if ( ++p == pe )
		goto _test_eof1485;
case 1485:
#line 44559 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1682;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1682:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1486;
st1486:
	if ( ++p == pe )
		goto _test_eof1486;
case 1486:
#line 44600 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 110: goto tr1438;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr758:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1487;
st1487:
	if ( ++p == pe )
		goto _test_eof1487;
case 1487:
#line 44641 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 78: goto tr1683;
		case 95: goto st249;
		case 110: goto tr1684;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1683:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1488;
st1488:
	if ( ++p == pe )
		goto _test_eof1488;
case 1488:
#line 44683 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1533;
		case 95: goto st249;
		case 114: goto tr1526;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1684:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1489;
st1489:
	if ( ++p == pe )
		goto _test_eof1489;
case 1489:
#line 44725 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 82: goto tr1533;
		case 95: goto st249;
		case 114: goto tr1526;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr759:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1490;
st1490:
	if ( ++p == pe )
		goto _test_eof1490;
case 1490:
#line 44767 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 95: goto st249;
		case 105: goto tr1685;
		case 111: goto tr1686;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1685:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1491;
st1491:
	if ( ++p == pe )
		goto _test_eof1491;
case 1491:
#line 44809 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 122: goto tr1235;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 121 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1686:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1492;
st1492:
	if ( ++p == pe )
		goto _test_eof1492;
case 1492:
#line 44850 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 95: goto st249;
		case 108: goto tr1249;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr760:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1493;
st1493:
	if ( ++p == pe )
		goto _test_eof1493;
case 1493:
#line 44891 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1687;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 69: goto tr1688;
		case 83: goto tr1237;
		case 95: goto st249;
		case 97: goto tr1337;
		case 101: goto tr1689;
		case 111: goto tr1545;
		case 115: goto tr1221;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1687:
#line 6 "actions.rl"
	{ s.suffix++; }
	goto st623;
st623:
	if ( ++p == pe )
		goto _test_eof623;
case 623:
#line 44935 "tokenizer.cc"
	switch( (*p) ) {
		case 68: goto tr248;
		case 76: goto tr249;
		case 77: goto tr250;
		case 82: goto tr251;
		case 83: goto tr252;
		case 86: goto tr253;
		case 100: goto tr254;
		case 101: goto tr693;
		case 108: goto tr255;
		case 109: goto tr256;
		case 114: goto tr257;
		case 115: goto tr252;
		case 118: goto tr258;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr168;
	} else if ( (*p) >= 65 )
		goto tr168;
	goto tr14;
tr693:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1494;
st1494:
	if ( ++p == pe )
		goto _test_eof1494;
case 1494:
#line 44967 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr214;
		case 45: goto tr911;
		case 46: goto st200;
		case 110: goto tr1690;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1690:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1495;
st1495:
	if ( ++p == pe )
		goto _test_eof1495;
case 1495:
#line 45005 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 45: goto tr911;
		case 46: goto st227;
		case 100: goto tr1691;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr168;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr218;
		} else
			goto tr217;
	} else
		goto st9;
	goto tr763;
tr1688:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1496;
st1496:
	if ( ++p == pe )
		goto _test_eof1496;
case 1496:
#line 45043 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st225;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 66: goto tr1551;
		case 95: goto st249;
		case 98: goto tr1549;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr1689:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1497;
st1497:
	if ( ++p == pe )
		goto _test_eof1497;
case 1497:
#line 45085 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr913;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st522;
		case 64: goto st250;
		case 66: goto tr1551;
		case 95: goto st249;
		case 98: goto tr1549;
		case 114: goto tr1384;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
tr761:
#line 1 "NONE"
	{ s.te = p+1;}
#line 7 "rules/main.rl"
	{ s.act = 39;}
	goto st1498;
st1498:
	if ( ++p == pe )
		goto _test_eof1498;
case 1498:
#line 45128 "tokenizer.cc"
	switch( (*p) ) {
		case -62: goto st2;
		case -30: goto st169;
		case 38: goto st178;
		case 39: goto tr1553;
		case 43: goto st249;
		case 45: goto tr1124;
		case 46: goto st607;
		case 64: goto st250;
		case 95: goto st249;
		case 97: goto tr1554;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st4;
		} else if ( (*p) >= -61 )
			goto st3;
	} else if ( (*p) > -12 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr532;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr534;
		} else
			goto tr533;
	} else
		goto st9;
	goto tr763;
	}
	_test_eof624:  s.cs = 624; goto _test_eof; 
	_test_eof1:  s.cs = 1; goto _test_eof; 
	_test_eof625:  s.cs = 625; goto _test_eof; 
	_test_eof2:  s.cs = 2; goto _test_eof; 
	_test_eof3:  s.cs = 3; goto _test_eof; 
	_test_eof4:  s.cs = 4; goto _test_eof; 
	_test_eof5:  s.cs = 5; goto _test_eof; 
	_test_eof6:  s.cs = 6; goto _test_eof; 
	_test_eof7:  s.cs = 7; goto _test_eof; 
	_test_eof8:  s.cs = 8; goto _test_eof; 
	_test_eof9:  s.cs = 9; goto _test_eof; 
	_test_eof10:  s.cs = 10; goto _test_eof; 
	_test_eof11:  s.cs = 11; goto _test_eof; 
	_test_eof12:  s.cs = 12; goto _test_eof; 
	_test_eof13:  s.cs = 13; goto _test_eof; 
	_test_eof626:  s.cs = 626; goto _test_eof; 
	_test_eof14:  s.cs = 14; goto _test_eof; 
	_test_eof627:  s.cs = 627; goto _test_eof; 
	_test_eof15:  s.cs = 15; goto _test_eof; 
	_test_eof16:  s.cs = 16; goto _test_eof; 
	_test_eof628:  s.cs = 628; goto _test_eof; 
	_test_eof629:  s.cs = 629; goto _test_eof; 
	_test_eof17:  s.cs = 17; goto _test_eof; 
	_test_eof18:  s.cs = 18; goto _test_eof; 
	_test_eof19:  s.cs = 19; goto _test_eof; 
	_test_eof20:  s.cs = 20; goto _test_eof; 
	_test_eof21:  s.cs = 21; goto _test_eof; 
	_test_eof22:  s.cs = 22; goto _test_eof; 
	_test_eof630:  s.cs = 630; goto _test_eof; 
	_test_eof23:  s.cs = 23; goto _test_eof; 
	_test_eof631:  s.cs = 631; goto _test_eof; 
	_test_eof24:  s.cs = 24; goto _test_eof; 
	_test_eof25:  s.cs = 25; goto _test_eof; 
	_test_eof26:  s.cs = 26; goto _test_eof; 
	_test_eof27:  s.cs = 27; goto _test_eof; 
	_test_eof28:  s.cs = 28; goto _test_eof; 
	_test_eof29:  s.cs = 29; goto _test_eof; 
	_test_eof30:  s.cs = 30; goto _test_eof; 
	_test_eof31:  s.cs = 31; goto _test_eof; 
	_test_eof32:  s.cs = 32; goto _test_eof; 
	_test_eof33:  s.cs = 33; goto _test_eof; 
	_test_eof34:  s.cs = 34; goto _test_eof; 
	_test_eof632:  s.cs = 632; goto _test_eof; 
	_test_eof35:  s.cs = 35; goto _test_eof; 
	_test_eof36:  s.cs = 36; goto _test_eof; 
	_test_eof37:  s.cs = 37; goto _test_eof; 
	_test_eof38:  s.cs = 38; goto _test_eof; 
	_test_eof633:  s.cs = 633; goto _test_eof; 
	_test_eof39:  s.cs = 39; goto _test_eof; 
	_test_eof634:  s.cs = 634; goto _test_eof; 
	_test_eof40:  s.cs = 40; goto _test_eof; 
	_test_eof41:  s.cs = 41; goto _test_eof; 
	_test_eof42:  s.cs = 42; goto _test_eof; 
	_test_eof43:  s.cs = 43; goto _test_eof; 
	_test_eof44:  s.cs = 44; goto _test_eof; 
	_test_eof45:  s.cs = 45; goto _test_eof; 
	_test_eof46:  s.cs = 46; goto _test_eof; 
	_test_eof635:  s.cs = 635; goto _test_eof; 
	_test_eof636:  s.cs = 636; goto _test_eof; 
	_test_eof47:  s.cs = 47; goto _test_eof; 
	_test_eof48:  s.cs = 48; goto _test_eof; 
	_test_eof49:  s.cs = 49; goto _test_eof; 
	_test_eof50:  s.cs = 50; goto _test_eof; 
	_test_eof51:  s.cs = 51; goto _test_eof; 
	_test_eof52:  s.cs = 52; goto _test_eof; 
	_test_eof637:  s.cs = 637; goto _test_eof; 
	_test_eof53:  s.cs = 53; goto _test_eof; 
	_test_eof54:  s.cs = 54; goto _test_eof; 
	_test_eof55:  s.cs = 55; goto _test_eof; 
	_test_eof56:  s.cs = 56; goto _test_eof; 
	_test_eof57:  s.cs = 57; goto _test_eof; 
	_test_eof58:  s.cs = 58; goto _test_eof; 
	_test_eof59:  s.cs = 59; goto _test_eof; 
	_test_eof60:  s.cs = 60; goto _test_eof; 
	_test_eof638:  s.cs = 638; goto _test_eof; 
	_test_eof61:  s.cs = 61; goto _test_eof; 
	_test_eof62:  s.cs = 62; goto _test_eof; 
	_test_eof63:  s.cs = 63; goto _test_eof; 
	_test_eof64:  s.cs = 64; goto _test_eof; 
	_test_eof639:  s.cs = 639; goto _test_eof; 
	_test_eof65:  s.cs = 65; goto _test_eof; 
	_test_eof640:  s.cs = 640; goto _test_eof; 
	_test_eof641:  s.cs = 641; goto _test_eof; 
	_test_eof66:  s.cs = 66; goto _test_eof; 
	_test_eof642:  s.cs = 642; goto _test_eof; 
	_test_eof67:  s.cs = 67; goto _test_eof; 
	_test_eof68:  s.cs = 68; goto _test_eof; 
	_test_eof69:  s.cs = 69; goto _test_eof; 
	_test_eof643:  s.cs = 643; goto _test_eof; 
	_test_eof644:  s.cs = 644; goto _test_eof; 
	_test_eof70:  s.cs = 70; goto _test_eof; 
	_test_eof71:  s.cs = 71; goto _test_eof; 
	_test_eof645:  s.cs = 645; goto _test_eof; 
	_test_eof72:  s.cs = 72; goto _test_eof; 
	_test_eof73:  s.cs = 73; goto _test_eof; 
	_test_eof74:  s.cs = 74; goto _test_eof; 
	_test_eof75:  s.cs = 75; goto _test_eof; 
	_test_eof76:  s.cs = 76; goto _test_eof; 
	_test_eof646:  s.cs = 646; goto _test_eof; 
	_test_eof77:  s.cs = 77; goto _test_eof; 
	_test_eof78:  s.cs = 78; goto _test_eof; 
	_test_eof79:  s.cs = 79; goto _test_eof; 
	_test_eof80:  s.cs = 80; goto _test_eof; 
	_test_eof81:  s.cs = 81; goto _test_eof; 
	_test_eof82:  s.cs = 82; goto _test_eof; 
	_test_eof83:  s.cs = 83; goto _test_eof; 
	_test_eof84:  s.cs = 84; goto _test_eof; 
	_test_eof85:  s.cs = 85; goto _test_eof; 
	_test_eof86:  s.cs = 86; goto _test_eof; 
	_test_eof87:  s.cs = 87; goto _test_eof; 
	_test_eof88:  s.cs = 88; goto _test_eof; 
	_test_eof89:  s.cs = 89; goto _test_eof; 
	_test_eof90:  s.cs = 90; goto _test_eof; 
	_test_eof647:  s.cs = 647; goto _test_eof; 
	_test_eof91:  s.cs = 91; goto _test_eof; 
	_test_eof92:  s.cs = 92; goto _test_eof; 
	_test_eof93:  s.cs = 93; goto _test_eof; 
	_test_eof648:  s.cs = 648; goto _test_eof; 
	_test_eof649:  s.cs = 649; goto _test_eof; 
	_test_eof650:  s.cs = 650; goto _test_eof; 
	_test_eof94:  s.cs = 94; goto _test_eof; 
	_test_eof651:  s.cs = 651; goto _test_eof; 
	_test_eof652:  s.cs = 652; goto _test_eof; 
	_test_eof653:  s.cs = 653; goto _test_eof; 
	_test_eof654:  s.cs = 654; goto _test_eof; 
	_test_eof655:  s.cs = 655; goto _test_eof; 
	_test_eof95:  s.cs = 95; goto _test_eof; 
	_test_eof656:  s.cs = 656; goto _test_eof; 
	_test_eof657:  s.cs = 657; goto _test_eof; 
	_test_eof658:  s.cs = 658; goto _test_eof; 
	_test_eof659:  s.cs = 659; goto _test_eof; 
	_test_eof660:  s.cs = 660; goto _test_eof; 
	_test_eof661:  s.cs = 661; goto _test_eof; 
	_test_eof96:  s.cs = 96; goto _test_eof; 
	_test_eof662:  s.cs = 662; goto _test_eof; 
	_test_eof663:  s.cs = 663; goto _test_eof; 
	_test_eof97:  s.cs = 97; goto _test_eof; 
	_test_eof664:  s.cs = 664; goto _test_eof; 
	_test_eof665:  s.cs = 665; goto _test_eof; 
	_test_eof666:  s.cs = 666; goto _test_eof; 
	_test_eof667:  s.cs = 667; goto _test_eof; 
	_test_eof668:  s.cs = 668; goto _test_eof; 
	_test_eof98:  s.cs = 98; goto _test_eof; 
	_test_eof669:  s.cs = 669; goto _test_eof; 
	_test_eof670:  s.cs = 670; goto _test_eof; 
	_test_eof671:  s.cs = 671; goto _test_eof; 
	_test_eof672:  s.cs = 672; goto _test_eof; 
	_test_eof673:  s.cs = 673; goto _test_eof; 
	_test_eof674:  s.cs = 674; goto _test_eof; 
	_test_eof675:  s.cs = 675; goto _test_eof; 
	_test_eof676:  s.cs = 676; goto _test_eof; 
	_test_eof677:  s.cs = 677; goto _test_eof; 
	_test_eof678:  s.cs = 678; goto _test_eof; 
	_test_eof679:  s.cs = 679; goto _test_eof; 
	_test_eof680:  s.cs = 680; goto _test_eof; 
	_test_eof681:  s.cs = 681; goto _test_eof; 
	_test_eof682:  s.cs = 682; goto _test_eof; 
	_test_eof683:  s.cs = 683; goto _test_eof; 
	_test_eof684:  s.cs = 684; goto _test_eof; 
	_test_eof685:  s.cs = 685; goto _test_eof; 
	_test_eof686:  s.cs = 686; goto _test_eof; 
	_test_eof687:  s.cs = 687; goto _test_eof; 
	_test_eof688:  s.cs = 688; goto _test_eof; 
	_test_eof689:  s.cs = 689; goto _test_eof; 
	_test_eof690:  s.cs = 690; goto _test_eof; 
	_test_eof691:  s.cs = 691; goto _test_eof; 
	_test_eof692:  s.cs = 692; goto _test_eof; 
	_test_eof693:  s.cs = 693; goto _test_eof; 
	_test_eof694:  s.cs = 694; goto _test_eof; 
	_test_eof695:  s.cs = 695; goto _test_eof; 
	_test_eof696:  s.cs = 696; goto _test_eof; 
	_test_eof697:  s.cs = 697; goto _test_eof; 
	_test_eof698:  s.cs = 698; goto _test_eof; 
	_test_eof699:  s.cs = 699; goto _test_eof; 
	_test_eof700:  s.cs = 700; goto _test_eof; 
	_test_eof701:  s.cs = 701; goto _test_eof; 
	_test_eof702:  s.cs = 702; goto _test_eof; 
	_test_eof99:  s.cs = 99; goto _test_eof; 
	_test_eof703:  s.cs = 703; goto _test_eof; 
	_test_eof704:  s.cs = 704; goto _test_eof; 
	_test_eof705:  s.cs = 705; goto _test_eof; 
	_test_eof706:  s.cs = 706; goto _test_eof; 
	_test_eof707:  s.cs = 707; goto _test_eof; 
	_test_eof708:  s.cs = 708; goto _test_eof; 
	_test_eof100:  s.cs = 100; goto _test_eof; 
	_test_eof101:  s.cs = 101; goto _test_eof; 
	_test_eof709:  s.cs = 709; goto _test_eof; 
	_test_eof102:  s.cs = 102; goto _test_eof; 
	_test_eof103:  s.cs = 103; goto _test_eof; 
	_test_eof104:  s.cs = 104; goto _test_eof; 
	_test_eof105:  s.cs = 105; goto _test_eof; 
	_test_eof106:  s.cs = 106; goto _test_eof; 
	_test_eof107:  s.cs = 107; goto _test_eof; 
	_test_eof108:  s.cs = 108; goto _test_eof; 
	_test_eof109:  s.cs = 109; goto _test_eof; 
	_test_eof110:  s.cs = 110; goto _test_eof; 
	_test_eof111:  s.cs = 111; goto _test_eof; 
	_test_eof112:  s.cs = 112; goto _test_eof; 
	_test_eof113:  s.cs = 113; goto _test_eof; 
	_test_eof114:  s.cs = 114; goto _test_eof; 
	_test_eof115:  s.cs = 115; goto _test_eof; 
	_test_eof116:  s.cs = 116; goto _test_eof; 
	_test_eof117:  s.cs = 117; goto _test_eof; 
	_test_eof710:  s.cs = 710; goto _test_eof; 
	_test_eof118:  s.cs = 118; goto _test_eof; 
	_test_eof119:  s.cs = 119; goto _test_eof; 
	_test_eof120:  s.cs = 120; goto _test_eof; 
	_test_eof121:  s.cs = 121; goto _test_eof; 
	_test_eof122:  s.cs = 122; goto _test_eof; 
	_test_eof123:  s.cs = 123; goto _test_eof; 
	_test_eof124:  s.cs = 124; goto _test_eof; 
	_test_eof125:  s.cs = 125; goto _test_eof; 
	_test_eof126:  s.cs = 126; goto _test_eof; 
	_test_eof127:  s.cs = 127; goto _test_eof; 
	_test_eof128:  s.cs = 128; goto _test_eof; 
	_test_eof129:  s.cs = 129; goto _test_eof; 
	_test_eof130:  s.cs = 130; goto _test_eof; 
	_test_eof131:  s.cs = 131; goto _test_eof; 
	_test_eof132:  s.cs = 132; goto _test_eof; 
	_test_eof133:  s.cs = 133; goto _test_eof; 
	_test_eof134:  s.cs = 134; goto _test_eof; 
	_test_eof135:  s.cs = 135; goto _test_eof; 
	_test_eof136:  s.cs = 136; goto _test_eof; 
	_test_eof711:  s.cs = 711; goto _test_eof; 
	_test_eof712:  s.cs = 712; goto _test_eof; 
	_test_eof713:  s.cs = 713; goto _test_eof; 
	_test_eof714:  s.cs = 714; goto _test_eof; 
	_test_eof715:  s.cs = 715; goto _test_eof; 
	_test_eof137:  s.cs = 137; goto _test_eof; 
	_test_eof716:  s.cs = 716; goto _test_eof; 
	_test_eof138:  s.cs = 138; goto _test_eof; 
	_test_eof139:  s.cs = 139; goto _test_eof; 
	_test_eof140:  s.cs = 140; goto _test_eof; 
	_test_eof141:  s.cs = 141; goto _test_eof; 
	_test_eof142:  s.cs = 142; goto _test_eof; 
	_test_eof143:  s.cs = 143; goto _test_eof; 
	_test_eof144:  s.cs = 144; goto _test_eof; 
	_test_eof145:  s.cs = 145; goto _test_eof; 
	_test_eof146:  s.cs = 146; goto _test_eof; 
	_test_eof147:  s.cs = 147; goto _test_eof; 
	_test_eof148:  s.cs = 148; goto _test_eof; 
	_test_eof149:  s.cs = 149; goto _test_eof; 
	_test_eof150:  s.cs = 150; goto _test_eof; 
	_test_eof151:  s.cs = 151; goto _test_eof; 
	_test_eof152:  s.cs = 152; goto _test_eof; 
	_test_eof153:  s.cs = 153; goto _test_eof; 
	_test_eof154:  s.cs = 154; goto _test_eof; 
	_test_eof155:  s.cs = 155; goto _test_eof; 
	_test_eof156:  s.cs = 156; goto _test_eof; 
	_test_eof157:  s.cs = 157; goto _test_eof; 
	_test_eof158:  s.cs = 158; goto _test_eof; 
	_test_eof159:  s.cs = 159; goto _test_eof; 
	_test_eof160:  s.cs = 160; goto _test_eof; 
	_test_eof161:  s.cs = 161; goto _test_eof; 
	_test_eof162:  s.cs = 162; goto _test_eof; 
	_test_eof163:  s.cs = 163; goto _test_eof; 
	_test_eof164:  s.cs = 164; goto _test_eof; 
	_test_eof165:  s.cs = 165; goto _test_eof; 
	_test_eof166:  s.cs = 166; goto _test_eof; 
	_test_eof167:  s.cs = 167; goto _test_eof; 
	_test_eof168:  s.cs = 168; goto _test_eof; 
	_test_eof717:  s.cs = 717; goto _test_eof; 
	_test_eof169:  s.cs = 169; goto _test_eof; 
	_test_eof170:  s.cs = 170; goto _test_eof; 
	_test_eof171:  s.cs = 171; goto _test_eof; 
	_test_eof172:  s.cs = 172; goto _test_eof; 
	_test_eof173:  s.cs = 173; goto _test_eof; 
	_test_eof174:  s.cs = 174; goto _test_eof; 
	_test_eof175:  s.cs = 175; goto _test_eof; 
	_test_eof176:  s.cs = 176; goto _test_eof; 
	_test_eof177:  s.cs = 177; goto _test_eof; 
	_test_eof178:  s.cs = 178; goto _test_eof; 
	_test_eof179:  s.cs = 179; goto _test_eof; 
	_test_eof180:  s.cs = 180; goto _test_eof; 
	_test_eof181:  s.cs = 181; goto _test_eof; 
	_test_eof182:  s.cs = 182; goto _test_eof; 
	_test_eof183:  s.cs = 183; goto _test_eof; 
	_test_eof184:  s.cs = 184; goto _test_eof; 
	_test_eof185:  s.cs = 185; goto _test_eof; 
	_test_eof186:  s.cs = 186; goto _test_eof; 
	_test_eof187:  s.cs = 187; goto _test_eof; 
	_test_eof188:  s.cs = 188; goto _test_eof; 
	_test_eof189:  s.cs = 189; goto _test_eof; 
	_test_eof190:  s.cs = 190; goto _test_eof; 
	_test_eof191:  s.cs = 191; goto _test_eof; 
	_test_eof192:  s.cs = 192; goto _test_eof; 
	_test_eof193:  s.cs = 193; goto _test_eof; 
	_test_eof194:  s.cs = 194; goto _test_eof; 
	_test_eof195:  s.cs = 195; goto _test_eof; 
	_test_eof196:  s.cs = 196; goto _test_eof; 
	_test_eof197:  s.cs = 197; goto _test_eof; 
	_test_eof198:  s.cs = 198; goto _test_eof; 
	_test_eof718:  s.cs = 718; goto _test_eof; 
	_test_eof199:  s.cs = 199; goto _test_eof; 
	_test_eof200:  s.cs = 200; goto _test_eof; 
	_test_eof201:  s.cs = 201; goto _test_eof; 
	_test_eof202:  s.cs = 202; goto _test_eof; 
	_test_eof203:  s.cs = 203; goto _test_eof; 
	_test_eof204:  s.cs = 204; goto _test_eof; 
	_test_eof205:  s.cs = 205; goto _test_eof; 
	_test_eof206:  s.cs = 206; goto _test_eof; 
	_test_eof207:  s.cs = 207; goto _test_eof; 
	_test_eof208:  s.cs = 208; goto _test_eof; 
	_test_eof209:  s.cs = 209; goto _test_eof; 
	_test_eof210:  s.cs = 210; goto _test_eof; 
	_test_eof211:  s.cs = 211; goto _test_eof; 
	_test_eof212:  s.cs = 212; goto _test_eof; 
	_test_eof213:  s.cs = 213; goto _test_eof; 
	_test_eof214:  s.cs = 214; goto _test_eof; 
	_test_eof215:  s.cs = 215; goto _test_eof; 
	_test_eof216:  s.cs = 216; goto _test_eof; 
	_test_eof217:  s.cs = 217; goto _test_eof; 
	_test_eof218:  s.cs = 218; goto _test_eof; 
	_test_eof219:  s.cs = 219; goto _test_eof; 
	_test_eof220:  s.cs = 220; goto _test_eof; 
	_test_eof221:  s.cs = 221; goto _test_eof; 
	_test_eof222:  s.cs = 222; goto _test_eof; 
	_test_eof223:  s.cs = 223; goto _test_eof; 
	_test_eof224:  s.cs = 224; goto _test_eof; 
	_test_eof719:  s.cs = 719; goto _test_eof; 
	_test_eof225:  s.cs = 225; goto _test_eof; 
	_test_eof226:  s.cs = 226; goto _test_eof; 
	_test_eof720:  s.cs = 720; goto _test_eof; 
	_test_eof721:  s.cs = 721; goto _test_eof; 
	_test_eof227:  s.cs = 227; goto _test_eof; 
	_test_eof722:  s.cs = 722; goto _test_eof; 
	_test_eof723:  s.cs = 723; goto _test_eof; 
	_test_eof724:  s.cs = 724; goto _test_eof; 
	_test_eof725:  s.cs = 725; goto _test_eof; 
	_test_eof726:  s.cs = 726; goto _test_eof; 
	_test_eof228:  s.cs = 228; goto _test_eof; 
	_test_eof229:  s.cs = 229; goto _test_eof; 
	_test_eof230:  s.cs = 230; goto _test_eof; 
	_test_eof231:  s.cs = 231; goto _test_eof; 
	_test_eof232:  s.cs = 232; goto _test_eof; 
	_test_eof233:  s.cs = 233; goto _test_eof; 
	_test_eof234:  s.cs = 234; goto _test_eof; 
	_test_eof235:  s.cs = 235; goto _test_eof; 
	_test_eof236:  s.cs = 236; goto _test_eof; 
	_test_eof237:  s.cs = 237; goto _test_eof; 
	_test_eof238:  s.cs = 238; goto _test_eof; 
	_test_eof239:  s.cs = 239; goto _test_eof; 
	_test_eof240:  s.cs = 240; goto _test_eof; 
	_test_eof241:  s.cs = 241; goto _test_eof; 
	_test_eof242:  s.cs = 242; goto _test_eof; 
	_test_eof243:  s.cs = 243; goto _test_eof; 
	_test_eof244:  s.cs = 244; goto _test_eof; 
	_test_eof245:  s.cs = 245; goto _test_eof; 
	_test_eof727:  s.cs = 727; goto _test_eof; 
	_test_eof246:  s.cs = 246; goto _test_eof; 
	_test_eof728:  s.cs = 728; goto _test_eof; 
	_test_eof247:  s.cs = 247; goto _test_eof; 
	_test_eof248:  s.cs = 248; goto _test_eof; 
	_test_eof729:  s.cs = 729; goto _test_eof; 
	_test_eof730:  s.cs = 730; goto _test_eof; 
	_test_eof249:  s.cs = 249; goto _test_eof; 
	_test_eof250:  s.cs = 250; goto _test_eof; 
	_test_eof251:  s.cs = 251; goto _test_eof; 
	_test_eof252:  s.cs = 252; goto _test_eof; 
	_test_eof253:  s.cs = 253; goto _test_eof; 
	_test_eof254:  s.cs = 254; goto _test_eof; 
	_test_eof255:  s.cs = 255; goto _test_eof; 
	_test_eof256:  s.cs = 256; goto _test_eof; 
	_test_eof731:  s.cs = 731; goto _test_eof; 
	_test_eof732:  s.cs = 732; goto _test_eof; 
	_test_eof733:  s.cs = 733; goto _test_eof; 
	_test_eof734:  s.cs = 734; goto _test_eof; 
	_test_eof257:  s.cs = 257; goto _test_eof; 
	_test_eof258:  s.cs = 258; goto _test_eof; 
	_test_eof259:  s.cs = 259; goto _test_eof; 
	_test_eof260:  s.cs = 260; goto _test_eof; 
	_test_eof261:  s.cs = 261; goto _test_eof; 
	_test_eof262:  s.cs = 262; goto _test_eof; 
	_test_eof263:  s.cs = 263; goto _test_eof; 
	_test_eof264:  s.cs = 264; goto _test_eof; 
	_test_eof265:  s.cs = 265; goto _test_eof; 
	_test_eof266:  s.cs = 266; goto _test_eof; 
	_test_eof267:  s.cs = 267; goto _test_eof; 
	_test_eof268:  s.cs = 268; goto _test_eof; 
	_test_eof269:  s.cs = 269; goto _test_eof; 
	_test_eof270:  s.cs = 270; goto _test_eof; 
	_test_eof271:  s.cs = 271; goto _test_eof; 
	_test_eof272:  s.cs = 272; goto _test_eof; 
	_test_eof273:  s.cs = 273; goto _test_eof; 
	_test_eof274:  s.cs = 274; goto _test_eof; 
	_test_eof275:  s.cs = 275; goto _test_eof; 
	_test_eof735:  s.cs = 735; goto _test_eof; 
	_test_eof736:  s.cs = 736; goto _test_eof; 
	_test_eof276:  s.cs = 276; goto _test_eof; 
	_test_eof737:  s.cs = 737; goto _test_eof; 
	_test_eof277:  s.cs = 277; goto _test_eof; 
	_test_eof738:  s.cs = 738; goto _test_eof; 
	_test_eof278:  s.cs = 278; goto _test_eof; 
	_test_eof739:  s.cs = 739; goto _test_eof; 
	_test_eof279:  s.cs = 279; goto _test_eof; 
	_test_eof740:  s.cs = 740; goto _test_eof; 
	_test_eof741:  s.cs = 741; goto _test_eof; 
	_test_eof280:  s.cs = 280; goto _test_eof; 
	_test_eof281:  s.cs = 281; goto _test_eof; 
	_test_eof282:  s.cs = 282; goto _test_eof; 
	_test_eof742:  s.cs = 742; goto _test_eof; 
	_test_eof283:  s.cs = 283; goto _test_eof; 
	_test_eof743:  s.cs = 743; goto _test_eof; 
	_test_eof284:  s.cs = 284; goto _test_eof; 
	_test_eof744:  s.cs = 744; goto _test_eof; 
	_test_eof285:  s.cs = 285; goto _test_eof; 
	_test_eof745:  s.cs = 745; goto _test_eof; 
	_test_eof286:  s.cs = 286; goto _test_eof; 
	_test_eof287:  s.cs = 287; goto _test_eof; 
	_test_eof746:  s.cs = 746; goto _test_eof; 
	_test_eof747:  s.cs = 747; goto _test_eof; 
	_test_eof288:  s.cs = 288; goto _test_eof; 
	_test_eof748:  s.cs = 748; goto _test_eof; 
	_test_eof289:  s.cs = 289; goto _test_eof; 
	_test_eof290:  s.cs = 290; goto _test_eof; 
	_test_eof291:  s.cs = 291; goto _test_eof; 
	_test_eof292:  s.cs = 292; goto _test_eof; 
	_test_eof749:  s.cs = 749; goto _test_eof; 
	_test_eof293:  s.cs = 293; goto _test_eof; 
	_test_eof294:  s.cs = 294; goto _test_eof; 
	_test_eof750:  s.cs = 750; goto _test_eof; 
	_test_eof295:  s.cs = 295; goto _test_eof; 
	_test_eof751:  s.cs = 751; goto _test_eof; 
	_test_eof296:  s.cs = 296; goto _test_eof; 
	_test_eof752:  s.cs = 752; goto _test_eof; 
	_test_eof297:  s.cs = 297; goto _test_eof; 
	_test_eof298:  s.cs = 298; goto _test_eof; 
	_test_eof299:  s.cs = 299; goto _test_eof; 
	_test_eof300:  s.cs = 300; goto _test_eof; 
	_test_eof753:  s.cs = 753; goto _test_eof; 
	_test_eof301:  s.cs = 301; goto _test_eof; 
	_test_eof754:  s.cs = 754; goto _test_eof; 
	_test_eof302:  s.cs = 302; goto _test_eof; 
	_test_eof755:  s.cs = 755; goto _test_eof; 
	_test_eof303:  s.cs = 303; goto _test_eof; 
	_test_eof304:  s.cs = 304; goto _test_eof; 
	_test_eof305:  s.cs = 305; goto _test_eof; 
	_test_eof756:  s.cs = 756; goto _test_eof; 
	_test_eof306:  s.cs = 306; goto _test_eof; 
	_test_eof757:  s.cs = 757; goto _test_eof; 
	_test_eof307:  s.cs = 307; goto _test_eof; 
	_test_eof758:  s.cs = 758; goto _test_eof; 
	_test_eof308:  s.cs = 308; goto _test_eof; 
	_test_eof309:  s.cs = 309; goto _test_eof; 
	_test_eof310:  s.cs = 310; goto _test_eof; 
	_test_eof311:  s.cs = 311; goto _test_eof; 
	_test_eof312:  s.cs = 312; goto _test_eof; 
	_test_eof313:  s.cs = 313; goto _test_eof; 
	_test_eof314:  s.cs = 314; goto _test_eof; 
	_test_eof759:  s.cs = 759; goto _test_eof; 
	_test_eof760:  s.cs = 760; goto _test_eof; 
	_test_eof315:  s.cs = 315; goto _test_eof; 
	_test_eof316:  s.cs = 316; goto _test_eof; 
	_test_eof317:  s.cs = 317; goto _test_eof; 
	_test_eof318:  s.cs = 318; goto _test_eof; 
	_test_eof319:  s.cs = 319; goto _test_eof; 
	_test_eof320:  s.cs = 320; goto _test_eof; 
	_test_eof761:  s.cs = 761; goto _test_eof; 
	_test_eof321:  s.cs = 321; goto _test_eof; 
	_test_eof322:  s.cs = 322; goto _test_eof; 
	_test_eof323:  s.cs = 323; goto _test_eof; 
	_test_eof324:  s.cs = 324; goto _test_eof; 
	_test_eof762:  s.cs = 762; goto _test_eof; 
	_test_eof325:  s.cs = 325; goto _test_eof; 
	_test_eof763:  s.cs = 763; goto _test_eof; 
	_test_eof764:  s.cs = 764; goto _test_eof; 
	_test_eof326:  s.cs = 326; goto _test_eof; 
	_test_eof765:  s.cs = 765; goto _test_eof; 
	_test_eof327:  s.cs = 327; goto _test_eof; 
	_test_eof328:  s.cs = 328; goto _test_eof; 
	_test_eof766:  s.cs = 766; goto _test_eof; 
	_test_eof767:  s.cs = 767; goto _test_eof; 
	_test_eof329:  s.cs = 329; goto _test_eof; 
	_test_eof768:  s.cs = 768; goto _test_eof; 
	_test_eof330:  s.cs = 330; goto _test_eof; 
	_test_eof331:  s.cs = 331; goto _test_eof; 
	_test_eof332:  s.cs = 332; goto _test_eof; 
	_test_eof333:  s.cs = 333; goto _test_eof; 
	_test_eof334:  s.cs = 334; goto _test_eof; 
	_test_eof769:  s.cs = 769; goto _test_eof; 
	_test_eof335:  s.cs = 335; goto _test_eof; 
	_test_eof336:  s.cs = 336; goto _test_eof; 
	_test_eof337:  s.cs = 337; goto _test_eof; 
	_test_eof338:  s.cs = 338; goto _test_eof; 
	_test_eof339:  s.cs = 339; goto _test_eof; 
	_test_eof340:  s.cs = 340; goto _test_eof; 
	_test_eof341:  s.cs = 341; goto _test_eof; 
	_test_eof342:  s.cs = 342; goto _test_eof; 
	_test_eof343:  s.cs = 343; goto _test_eof; 
	_test_eof344:  s.cs = 344; goto _test_eof; 
	_test_eof345:  s.cs = 345; goto _test_eof; 
	_test_eof346:  s.cs = 346; goto _test_eof; 
	_test_eof347:  s.cs = 347; goto _test_eof; 
	_test_eof348:  s.cs = 348; goto _test_eof; 
	_test_eof349:  s.cs = 349; goto _test_eof; 
	_test_eof350:  s.cs = 350; goto _test_eof; 
	_test_eof770:  s.cs = 770; goto _test_eof; 
	_test_eof351:  s.cs = 351; goto _test_eof; 
	_test_eof352:  s.cs = 352; goto _test_eof; 
	_test_eof771:  s.cs = 771; goto _test_eof; 
	_test_eof353:  s.cs = 353; goto _test_eof; 
	_test_eof354:  s.cs = 354; goto _test_eof; 
	_test_eof355:  s.cs = 355; goto _test_eof; 
	_test_eof356:  s.cs = 356; goto _test_eof; 
	_test_eof357:  s.cs = 357; goto _test_eof; 
	_test_eof772:  s.cs = 772; goto _test_eof; 
	_test_eof773:  s.cs = 773; goto _test_eof; 
	_test_eof358:  s.cs = 358; goto _test_eof; 
	_test_eof359:  s.cs = 359; goto _test_eof; 
	_test_eof360:  s.cs = 360; goto _test_eof; 
	_test_eof361:  s.cs = 361; goto _test_eof; 
	_test_eof362:  s.cs = 362; goto _test_eof; 
	_test_eof363:  s.cs = 363; goto _test_eof; 
	_test_eof364:  s.cs = 364; goto _test_eof; 
	_test_eof365:  s.cs = 365; goto _test_eof; 
	_test_eof366:  s.cs = 366; goto _test_eof; 
	_test_eof367:  s.cs = 367; goto _test_eof; 
	_test_eof774:  s.cs = 774; goto _test_eof; 
	_test_eof368:  s.cs = 368; goto _test_eof; 
	_test_eof369:  s.cs = 369; goto _test_eof; 
	_test_eof370:  s.cs = 370; goto _test_eof; 
	_test_eof775:  s.cs = 775; goto _test_eof; 
	_test_eof371:  s.cs = 371; goto _test_eof; 
	_test_eof372:  s.cs = 372; goto _test_eof; 
	_test_eof373:  s.cs = 373; goto _test_eof; 
	_test_eof776:  s.cs = 776; goto _test_eof; 
	_test_eof374:  s.cs = 374; goto _test_eof; 
	_test_eof375:  s.cs = 375; goto _test_eof; 
	_test_eof777:  s.cs = 777; goto _test_eof; 
	_test_eof376:  s.cs = 376; goto _test_eof; 
	_test_eof377:  s.cs = 377; goto _test_eof; 
	_test_eof378:  s.cs = 378; goto _test_eof; 
	_test_eof379:  s.cs = 379; goto _test_eof; 
	_test_eof380:  s.cs = 380; goto _test_eof; 
	_test_eof381:  s.cs = 381; goto _test_eof; 
	_test_eof778:  s.cs = 778; goto _test_eof; 
	_test_eof382:  s.cs = 382; goto _test_eof; 
	_test_eof383:  s.cs = 383; goto _test_eof; 
	_test_eof384:  s.cs = 384; goto _test_eof; 
	_test_eof779:  s.cs = 779; goto _test_eof; 
	_test_eof385:  s.cs = 385; goto _test_eof; 
	_test_eof780:  s.cs = 780; goto _test_eof; 
	_test_eof781:  s.cs = 781; goto _test_eof; 
	_test_eof782:  s.cs = 782; goto _test_eof; 
	_test_eof386:  s.cs = 386; goto _test_eof; 
	_test_eof783:  s.cs = 783; goto _test_eof; 
	_test_eof784:  s.cs = 784; goto _test_eof; 
	_test_eof785:  s.cs = 785; goto _test_eof; 
	_test_eof786:  s.cs = 786; goto _test_eof; 
	_test_eof787:  s.cs = 787; goto _test_eof; 
	_test_eof788:  s.cs = 788; goto _test_eof; 
	_test_eof387:  s.cs = 387; goto _test_eof; 
	_test_eof789:  s.cs = 789; goto _test_eof; 
	_test_eof790:  s.cs = 790; goto _test_eof; 
	_test_eof791:  s.cs = 791; goto _test_eof; 
	_test_eof792:  s.cs = 792; goto _test_eof; 
	_test_eof793:  s.cs = 793; goto _test_eof; 
	_test_eof794:  s.cs = 794; goto _test_eof; 
	_test_eof795:  s.cs = 795; goto _test_eof; 
	_test_eof388:  s.cs = 388; goto _test_eof; 
	_test_eof796:  s.cs = 796; goto _test_eof; 
	_test_eof797:  s.cs = 797; goto _test_eof; 
	_test_eof389:  s.cs = 389; goto _test_eof; 
	_test_eof798:  s.cs = 798; goto _test_eof; 
	_test_eof799:  s.cs = 799; goto _test_eof; 
	_test_eof800:  s.cs = 800; goto _test_eof; 
	_test_eof801:  s.cs = 801; goto _test_eof; 
	_test_eof802:  s.cs = 802; goto _test_eof; 
	_test_eof390:  s.cs = 390; goto _test_eof; 
	_test_eof803:  s.cs = 803; goto _test_eof; 
	_test_eof804:  s.cs = 804; goto _test_eof; 
	_test_eof805:  s.cs = 805; goto _test_eof; 
	_test_eof806:  s.cs = 806; goto _test_eof; 
	_test_eof807:  s.cs = 807; goto _test_eof; 
	_test_eof808:  s.cs = 808; goto _test_eof; 
	_test_eof809:  s.cs = 809; goto _test_eof; 
	_test_eof810:  s.cs = 810; goto _test_eof; 
	_test_eof811:  s.cs = 811; goto _test_eof; 
	_test_eof812:  s.cs = 812; goto _test_eof; 
	_test_eof813:  s.cs = 813; goto _test_eof; 
	_test_eof814:  s.cs = 814; goto _test_eof; 
	_test_eof815:  s.cs = 815; goto _test_eof; 
	_test_eof816:  s.cs = 816; goto _test_eof; 
	_test_eof817:  s.cs = 817; goto _test_eof; 
	_test_eof818:  s.cs = 818; goto _test_eof; 
	_test_eof819:  s.cs = 819; goto _test_eof; 
	_test_eof820:  s.cs = 820; goto _test_eof; 
	_test_eof821:  s.cs = 821; goto _test_eof; 
	_test_eof822:  s.cs = 822; goto _test_eof; 
	_test_eof823:  s.cs = 823; goto _test_eof; 
	_test_eof824:  s.cs = 824; goto _test_eof; 
	_test_eof825:  s.cs = 825; goto _test_eof; 
	_test_eof826:  s.cs = 826; goto _test_eof; 
	_test_eof827:  s.cs = 827; goto _test_eof; 
	_test_eof828:  s.cs = 828; goto _test_eof; 
	_test_eof829:  s.cs = 829; goto _test_eof; 
	_test_eof830:  s.cs = 830; goto _test_eof; 
	_test_eof831:  s.cs = 831; goto _test_eof; 
	_test_eof832:  s.cs = 832; goto _test_eof; 
	_test_eof833:  s.cs = 833; goto _test_eof; 
	_test_eof834:  s.cs = 834; goto _test_eof; 
	_test_eof835:  s.cs = 835; goto _test_eof; 
	_test_eof836:  s.cs = 836; goto _test_eof; 
	_test_eof391:  s.cs = 391; goto _test_eof; 
	_test_eof837:  s.cs = 837; goto _test_eof; 
	_test_eof838:  s.cs = 838; goto _test_eof; 
	_test_eof839:  s.cs = 839; goto _test_eof; 
	_test_eof840:  s.cs = 840; goto _test_eof; 
	_test_eof841:  s.cs = 841; goto _test_eof; 
	_test_eof842:  s.cs = 842; goto _test_eof; 
	_test_eof843:  s.cs = 843; goto _test_eof; 
	_test_eof392:  s.cs = 392; goto _test_eof; 
	_test_eof393:  s.cs = 393; goto _test_eof; 
	_test_eof394:  s.cs = 394; goto _test_eof; 
	_test_eof395:  s.cs = 395; goto _test_eof; 
	_test_eof844:  s.cs = 844; goto _test_eof; 
	_test_eof845:  s.cs = 845; goto _test_eof; 
	_test_eof846:  s.cs = 846; goto _test_eof; 
	_test_eof847:  s.cs = 847; goto _test_eof; 
	_test_eof848:  s.cs = 848; goto _test_eof; 
	_test_eof849:  s.cs = 849; goto _test_eof; 
	_test_eof850:  s.cs = 850; goto _test_eof; 
	_test_eof851:  s.cs = 851; goto _test_eof; 
	_test_eof852:  s.cs = 852; goto _test_eof; 
	_test_eof853:  s.cs = 853; goto _test_eof; 
	_test_eof396:  s.cs = 396; goto _test_eof; 
	_test_eof854:  s.cs = 854; goto _test_eof; 
	_test_eof397:  s.cs = 397; goto _test_eof; 
	_test_eof398:  s.cs = 398; goto _test_eof; 
	_test_eof399:  s.cs = 399; goto _test_eof; 
	_test_eof855:  s.cs = 855; goto _test_eof; 
	_test_eof856:  s.cs = 856; goto _test_eof; 
	_test_eof857:  s.cs = 857; goto _test_eof; 
	_test_eof858:  s.cs = 858; goto _test_eof; 
	_test_eof859:  s.cs = 859; goto _test_eof; 
	_test_eof400:  s.cs = 400; goto _test_eof; 
	_test_eof860:  s.cs = 860; goto _test_eof; 
	_test_eof401:  s.cs = 401; goto _test_eof; 
	_test_eof861:  s.cs = 861; goto _test_eof; 
	_test_eof402:  s.cs = 402; goto _test_eof; 
	_test_eof862:  s.cs = 862; goto _test_eof; 
	_test_eof403:  s.cs = 403; goto _test_eof; 
	_test_eof863:  s.cs = 863; goto _test_eof; 
	_test_eof864:  s.cs = 864; goto _test_eof; 
	_test_eof404:  s.cs = 404; goto _test_eof; 
	_test_eof405:  s.cs = 405; goto _test_eof; 
	_test_eof406:  s.cs = 406; goto _test_eof; 
	_test_eof407:  s.cs = 407; goto _test_eof; 
	_test_eof408:  s.cs = 408; goto _test_eof; 
	_test_eof409:  s.cs = 409; goto _test_eof; 
	_test_eof410:  s.cs = 410; goto _test_eof; 
	_test_eof411:  s.cs = 411; goto _test_eof; 
	_test_eof412:  s.cs = 412; goto _test_eof; 
	_test_eof413:  s.cs = 413; goto _test_eof; 
	_test_eof414:  s.cs = 414; goto _test_eof; 
	_test_eof415:  s.cs = 415; goto _test_eof; 
	_test_eof416:  s.cs = 416; goto _test_eof; 
	_test_eof417:  s.cs = 417; goto _test_eof; 
	_test_eof418:  s.cs = 418; goto _test_eof; 
	_test_eof419:  s.cs = 419; goto _test_eof; 
	_test_eof865:  s.cs = 865; goto _test_eof; 
	_test_eof866:  s.cs = 866; goto _test_eof; 
	_test_eof867:  s.cs = 867; goto _test_eof; 
	_test_eof868:  s.cs = 868; goto _test_eof; 
	_test_eof420:  s.cs = 420; goto _test_eof; 
	_test_eof869:  s.cs = 869; goto _test_eof; 
	_test_eof421:  s.cs = 421; goto _test_eof; 
	_test_eof422:  s.cs = 422; goto _test_eof; 
	_test_eof870:  s.cs = 870; goto _test_eof; 
	_test_eof871:  s.cs = 871; goto _test_eof; 
	_test_eof872:  s.cs = 872; goto _test_eof; 
	_test_eof423:  s.cs = 423; goto _test_eof; 
	_test_eof873:  s.cs = 873; goto _test_eof; 
	_test_eof424:  s.cs = 424; goto _test_eof; 
	_test_eof874:  s.cs = 874; goto _test_eof; 
	_test_eof875:  s.cs = 875; goto _test_eof; 
	_test_eof876:  s.cs = 876; goto _test_eof; 
	_test_eof877:  s.cs = 877; goto _test_eof; 
	_test_eof878:  s.cs = 878; goto _test_eof; 
	_test_eof425:  s.cs = 425; goto _test_eof; 
	_test_eof426:  s.cs = 426; goto _test_eof; 
	_test_eof879:  s.cs = 879; goto _test_eof; 
	_test_eof427:  s.cs = 427; goto _test_eof; 
	_test_eof428:  s.cs = 428; goto _test_eof; 
	_test_eof429:  s.cs = 429; goto _test_eof; 
	_test_eof430:  s.cs = 430; goto _test_eof; 
	_test_eof431:  s.cs = 431; goto _test_eof; 
	_test_eof432:  s.cs = 432; goto _test_eof; 
	_test_eof433:  s.cs = 433; goto _test_eof; 
	_test_eof434:  s.cs = 434; goto _test_eof; 
	_test_eof435:  s.cs = 435; goto _test_eof; 
	_test_eof436:  s.cs = 436; goto _test_eof; 
	_test_eof880:  s.cs = 880; goto _test_eof; 
	_test_eof881:  s.cs = 881; goto _test_eof; 
	_test_eof882:  s.cs = 882; goto _test_eof; 
	_test_eof883:  s.cs = 883; goto _test_eof; 
	_test_eof884:  s.cs = 884; goto _test_eof; 
	_test_eof885:  s.cs = 885; goto _test_eof; 
	_test_eof886:  s.cs = 886; goto _test_eof; 
	_test_eof887:  s.cs = 887; goto _test_eof; 
	_test_eof437:  s.cs = 437; goto _test_eof; 
	_test_eof438:  s.cs = 438; goto _test_eof; 
	_test_eof439:  s.cs = 439; goto _test_eof; 
	_test_eof440:  s.cs = 440; goto _test_eof; 
	_test_eof441:  s.cs = 441; goto _test_eof; 
	_test_eof442:  s.cs = 442; goto _test_eof; 
	_test_eof888:  s.cs = 888; goto _test_eof; 
	_test_eof443:  s.cs = 443; goto _test_eof; 
	_test_eof444:  s.cs = 444; goto _test_eof; 
	_test_eof445:  s.cs = 445; goto _test_eof; 
	_test_eof446:  s.cs = 446; goto _test_eof; 
	_test_eof447:  s.cs = 447; goto _test_eof; 
	_test_eof448:  s.cs = 448; goto _test_eof; 
	_test_eof449:  s.cs = 449; goto _test_eof; 
	_test_eof450:  s.cs = 450; goto _test_eof; 
	_test_eof451:  s.cs = 451; goto _test_eof; 
	_test_eof452:  s.cs = 452; goto _test_eof; 
	_test_eof453:  s.cs = 453; goto _test_eof; 
	_test_eof454:  s.cs = 454; goto _test_eof; 
	_test_eof455:  s.cs = 455; goto _test_eof; 
	_test_eof456:  s.cs = 456; goto _test_eof; 
	_test_eof457:  s.cs = 457; goto _test_eof; 
	_test_eof889:  s.cs = 889; goto _test_eof; 
	_test_eof458:  s.cs = 458; goto _test_eof; 
	_test_eof459:  s.cs = 459; goto _test_eof; 
	_test_eof460:  s.cs = 460; goto _test_eof; 
	_test_eof461:  s.cs = 461; goto _test_eof; 
	_test_eof462:  s.cs = 462; goto _test_eof; 
	_test_eof463:  s.cs = 463; goto _test_eof; 
	_test_eof464:  s.cs = 464; goto _test_eof; 
	_test_eof465:  s.cs = 465; goto _test_eof; 
	_test_eof466:  s.cs = 466; goto _test_eof; 
	_test_eof467:  s.cs = 467; goto _test_eof; 
	_test_eof468:  s.cs = 468; goto _test_eof; 
	_test_eof469:  s.cs = 469; goto _test_eof; 
	_test_eof470:  s.cs = 470; goto _test_eof; 
	_test_eof890:  s.cs = 890; goto _test_eof; 
	_test_eof471:  s.cs = 471; goto _test_eof; 
	_test_eof472:  s.cs = 472; goto _test_eof; 
	_test_eof473:  s.cs = 473; goto _test_eof; 
	_test_eof474:  s.cs = 474; goto _test_eof; 
	_test_eof475:  s.cs = 475; goto _test_eof; 
	_test_eof476:  s.cs = 476; goto _test_eof; 
	_test_eof477:  s.cs = 477; goto _test_eof; 
	_test_eof478:  s.cs = 478; goto _test_eof; 
	_test_eof479:  s.cs = 479; goto _test_eof; 
	_test_eof480:  s.cs = 480; goto _test_eof; 
	_test_eof481:  s.cs = 481; goto _test_eof; 
	_test_eof482:  s.cs = 482; goto _test_eof; 
	_test_eof483:  s.cs = 483; goto _test_eof; 
	_test_eof484:  s.cs = 484; goto _test_eof; 
	_test_eof485:  s.cs = 485; goto _test_eof; 
	_test_eof891:  s.cs = 891; goto _test_eof; 
	_test_eof486:  s.cs = 486; goto _test_eof; 
	_test_eof892:  s.cs = 892; goto _test_eof; 
	_test_eof893:  s.cs = 893; goto _test_eof; 
	_test_eof487:  s.cs = 487; goto _test_eof; 
	_test_eof488:  s.cs = 488; goto _test_eof; 
	_test_eof894:  s.cs = 894; goto _test_eof; 
	_test_eof895:  s.cs = 895; goto _test_eof; 
	_test_eof489:  s.cs = 489; goto _test_eof; 
	_test_eof896:  s.cs = 896; goto _test_eof; 
	_test_eof490:  s.cs = 490; goto _test_eof; 
	_test_eof897:  s.cs = 897; goto _test_eof; 
	_test_eof491:  s.cs = 491; goto _test_eof; 
	_test_eof898:  s.cs = 898; goto _test_eof; 
	_test_eof492:  s.cs = 492; goto _test_eof; 
	_test_eof493:  s.cs = 493; goto _test_eof; 
	_test_eof494:  s.cs = 494; goto _test_eof; 
	_test_eof495:  s.cs = 495; goto _test_eof; 
	_test_eof496:  s.cs = 496; goto _test_eof; 
	_test_eof899:  s.cs = 899; goto _test_eof; 
	_test_eof900:  s.cs = 900; goto _test_eof; 
	_test_eof497:  s.cs = 497; goto _test_eof; 
	_test_eof901:  s.cs = 901; goto _test_eof; 
	_test_eof498:  s.cs = 498; goto _test_eof; 
	_test_eof902:  s.cs = 902; goto _test_eof; 
	_test_eof499:  s.cs = 499; goto _test_eof; 
	_test_eof903:  s.cs = 903; goto _test_eof; 
	_test_eof500:  s.cs = 500; goto _test_eof; 
	_test_eof904:  s.cs = 904; goto _test_eof; 
	_test_eof905:  s.cs = 905; goto _test_eof; 
	_test_eof501:  s.cs = 501; goto _test_eof; 
	_test_eof502:  s.cs = 502; goto _test_eof; 
	_test_eof503:  s.cs = 503; goto _test_eof; 
	_test_eof906:  s.cs = 906; goto _test_eof; 
	_test_eof504:  s.cs = 504; goto _test_eof; 
	_test_eof907:  s.cs = 907; goto _test_eof; 
	_test_eof505:  s.cs = 505; goto _test_eof; 
	_test_eof908:  s.cs = 908; goto _test_eof; 
	_test_eof506:  s.cs = 506; goto _test_eof; 
	_test_eof909:  s.cs = 909; goto _test_eof; 
	_test_eof507:  s.cs = 507; goto _test_eof; 
	_test_eof508:  s.cs = 508; goto _test_eof; 
	_test_eof910:  s.cs = 910; goto _test_eof; 
	_test_eof911:  s.cs = 911; goto _test_eof; 
	_test_eof509:  s.cs = 509; goto _test_eof; 
	_test_eof912:  s.cs = 912; goto _test_eof; 
	_test_eof510:  s.cs = 510; goto _test_eof; 
	_test_eof511:  s.cs = 511; goto _test_eof; 
	_test_eof512:  s.cs = 512; goto _test_eof; 
	_test_eof513:  s.cs = 513; goto _test_eof; 
	_test_eof913:  s.cs = 913; goto _test_eof; 
	_test_eof514:  s.cs = 514; goto _test_eof; 
	_test_eof515:  s.cs = 515; goto _test_eof; 
	_test_eof914:  s.cs = 914; goto _test_eof; 
	_test_eof516:  s.cs = 516; goto _test_eof; 
	_test_eof915:  s.cs = 915; goto _test_eof; 
	_test_eof517:  s.cs = 517; goto _test_eof; 
	_test_eof916:  s.cs = 916; goto _test_eof; 
	_test_eof518:  s.cs = 518; goto _test_eof; 
	_test_eof519:  s.cs = 519; goto _test_eof; 
	_test_eof520:  s.cs = 520; goto _test_eof; 
	_test_eof521:  s.cs = 521; goto _test_eof; 
	_test_eof917:  s.cs = 917; goto _test_eof; 
	_test_eof918:  s.cs = 918; goto _test_eof; 
	_test_eof522:  s.cs = 522; goto _test_eof; 
	_test_eof919:  s.cs = 919; goto _test_eof; 
	_test_eof920:  s.cs = 920; goto _test_eof; 
	_test_eof523:  s.cs = 523; goto _test_eof; 
	_test_eof921:  s.cs = 921; goto _test_eof; 
	_test_eof524:  s.cs = 524; goto _test_eof; 
	_test_eof922:  s.cs = 922; goto _test_eof; 
	_test_eof525:  s.cs = 525; goto _test_eof; 
	_test_eof923:  s.cs = 923; goto _test_eof; 
	_test_eof924:  s.cs = 924; goto _test_eof; 
	_test_eof925:  s.cs = 925; goto _test_eof; 
	_test_eof926:  s.cs = 926; goto _test_eof; 
	_test_eof927:  s.cs = 927; goto _test_eof; 
	_test_eof928:  s.cs = 928; goto _test_eof; 
	_test_eof929:  s.cs = 929; goto _test_eof; 
	_test_eof930:  s.cs = 930; goto _test_eof; 
	_test_eof931:  s.cs = 931; goto _test_eof; 
	_test_eof932:  s.cs = 932; goto _test_eof; 
	_test_eof933:  s.cs = 933; goto _test_eof; 
	_test_eof934:  s.cs = 934; goto _test_eof; 
	_test_eof935:  s.cs = 935; goto _test_eof; 
	_test_eof936:  s.cs = 936; goto _test_eof; 
	_test_eof937:  s.cs = 937; goto _test_eof; 
	_test_eof938:  s.cs = 938; goto _test_eof; 
	_test_eof939:  s.cs = 939; goto _test_eof; 
	_test_eof940:  s.cs = 940; goto _test_eof; 
	_test_eof941:  s.cs = 941; goto _test_eof; 
	_test_eof942:  s.cs = 942; goto _test_eof; 
	_test_eof943:  s.cs = 943; goto _test_eof; 
	_test_eof944:  s.cs = 944; goto _test_eof; 
	_test_eof945:  s.cs = 945; goto _test_eof; 
	_test_eof946:  s.cs = 946; goto _test_eof; 
	_test_eof947:  s.cs = 947; goto _test_eof; 
	_test_eof948:  s.cs = 948; goto _test_eof; 
	_test_eof949:  s.cs = 949; goto _test_eof; 
	_test_eof950:  s.cs = 950; goto _test_eof; 
	_test_eof951:  s.cs = 951; goto _test_eof; 
	_test_eof952:  s.cs = 952; goto _test_eof; 
	_test_eof953:  s.cs = 953; goto _test_eof; 
	_test_eof954:  s.cs = 954; goto _test_eof; 
	_test_eof955:  s.cs = 955; goto _test_eof; 
	_test_eof956:  s.cs = 956; goto _test_eof; 
	_test_eof957:  s.cs = 957; goto _test_eof; 
	_test_eof958:  s.cs = 958; goto _test_eof; 
	_test_eof959:  s.cs = 959; goto _test_eof; 
	_test_eof960:  s.cs = 960; goto _test_eof; 
	_test_eof961:  s.cs = 961; goto _test_eof; 
	_test_eof962:  s.cs = 962; goto _test_eof; 
	_test_eof963:  s.cs = 963; goto _test_eof; 
	_test_eof964:  s.cs = 964; goto _test_eof; 
	_test_eof965:  s.cs = 965; goto _test_eof; 
	_test_eof966:  s.cs = 966; goto _test_eof; 
	_test_eof967:  s.cs = 967; goto _test_eof; 
	_test_eof968:  s.cs = 968; goto _test_eof; 
	_test_eof969:  s.cs = 969; goto _test_eof; 
	_test_eof970:  s.cs = 970; goto _test_eof; 
	_test_eof971:  s.cs = 971; goto _test_eof; 
	_test_eof972:  s.cs = 972; goto _test_eof; 
	_test_eof973:  s.cs = 973; goto _test_eof; 
	_test_eof974:  s.cs = 974; goto _test_eof; 
	_test_eof975:  s.cs = 975; goto _test_eof; 
	_test_eof976:  s.cs = 976; goto _test_eof; 
	_test_eof977:  s.cs = 977; goto _test_eof; 
	_test_eof978:  s.cs = 978; goto _test_eof; 
	_test_eof979:  s.cs = 979; goto _test_eof; 
	_test_eof980:  s.cs = 980; goto _test_eof; 
	_test_eof981:  s.cs = 981; goto _test_eof; 
	_test_eof982:  s.cs = 982; goto _test_eof; 
	_test_eof983:  s.cs = 983; goto _test_eof; 
	_test_eof984:  s.cs = 984; goto _test_eof; 
	_test_eof985:  s.cs = 985; goto _test_eof; 
	_test_eof986:  s.cs = 986; goto _test_eof; 
	_test_eof987:  s.cs = 987; goto _test_eof; 
	_test_eof988:  s.cs = 988; goto _test_eof; 
	_test_eof989:  s.cs = 989; goto _test_eof; 
	_test_eof990:  s.cs = 990; goto _test_eof; 
	_test_eof991:  s.cs = 991; goto _test_eof; 
	_test_eof992:  s.cs = 992; goto _test_eof; 
	_test_eof993:  s.cs = 993; goto _test_eof; 
	_test_eof994:  s.cs = 994; goto _test_eof; 
	_test_eof995:  s.cs = 995; goto _test_eof; 
	_test_eof996:  s.cs = 996; goto _test_eof; 
	_test_eof997:  s.cs = 997; goto _test_eof; 
	_test_eof998:  s.cs = 998; goto _test_eof; 
	_test_eof999:  s.cs = 999; goto _test_eof; 
	_test_eof1000:  s.cs = 1000; goto _test_eof; 
	_test_eof526:  s.cs = 526; goto _test_eof; 
	_test_eof1001:  s.cs = 1001; goto _test_eof; 
	_test_eof1002:  s.cs = 1002; goto _test_eof; 
	_test_eof527:  s.cs = 527; goto _test_eof; 
	_test_eof528:  s.cs = 528; goto _test_eof; 
	_test_eof529:  s.cs = 529; goto _test_eof; 
	_test_eof530:  s.cs = 530; goto _test_eof; 
	_test_eof531:  s.cs = 531; goto _test_eof; 
	_test_eof532:  s.cs = 532; goto _test_eof; 
	_test_eof533:  s.cs = 533; goto _test_eof; 
	_test_eof534:  s.cs = 534; goto _test_eof; 
	_test_eof535:  s.cs = 535; goto _test_eof; 
	_test_eof536:  s.cs = 536; goto _test_eof; 
	_test_eof537:  s.cs = 537; goto _test_eof; 
	_test_eof538:  s.cs = 538; goto _test_eof; 
	_test_eof539:  s.cs = 539; goto _test_eof; 
	_test_eof540:  s.cs = 540; goto _test_eof; 
	_test_eof541:  s.cs = 541; goto _test_eof; 
	_test_eof542:  s.cs = 542; goto _test_eof; 
	_test_eof543:  s.cs = 543; goto _test_eof; 
	_test_eof544:  s.cs = 544; goto _test_eof; 
	_test_eof545:  s.cs = 545; goto _test_eof; 
	_test_eof546:  s.cs = 546; goto _test_eof; 
	_test_eof547:  s.cs = 547; goto _test_eof; 
	_test_eof548:  s.cs = 548; goto _test_eof; 
	_test_eof549:  s.cs = 549; goto _test_eof; 
	_test_eof550:  s.cs = 550; goto _test_eof; 
	_test_eof551:  s.cs = 551; goto _test_eof; 
	_test_eof1003:  s.cs = 1003; goto _test_eof; 
	_test_eof1004:  s.cs = 1004; goto _test_eof; 
	_test_eof1005:  s.cs = 1005; goto _test_eof; 
	_test_eof552:  s.cs = 552; goto _test_eof; 
	_test_eof553:  s.cs = 553; goto _test_eof; 
	_test_eof1006:  s.cs = 1006; goto _test_eof; 
	_test_eof1007:  s.cs = 1007; goto _test_eof; 
	_test_eof1008:  s.cs = 1008; goto _test_eof; 
	_test_eof554:  s.cs = 554; goto _test_eof; 
	_test_eof1009:  s.cs = 1009; goto _test_eof; 
	_test_eof1010:  s.cs = 1010; goto _test_eof; 
	_test_eof555:  s.cs = 555; goto _test_eof; 
	_test_eof1011:  s.cs = 1011; goto _test_eof; 
	_test_eof1012:  s.cs = 1012; goto _test_eof; 
	_test_eof1013:  s.cs = 1013; goto _test_eof; 
	_test_eof1014:  s.cs = 1014; goto _test_eof; 
	_test_eof1015:  s.cs = 1015; goto _test_eof; 
	_test_eof1016:  s.cs = 1016; goto _test_eof; 
	_test_eof1017:  s.cs = 1017; goto _test_eof; 
	_test_eof556:  s.cs = 556; goto _test_eof; 
	_test_eof557:  s.cs = 557; goto _test_eof; 
	_test_eof558:  s.cs = 558; goto _test_eof; 
	_test_eof559:  s.cs = 559; goto _test_eof; 
	_test_eof560:  s.cs = 560; goto _test_eof; 
	_test_eof561:  s.cs = 561; goto _test_eof; 
	_test_eof562:  s.cs = 562; goto _test_eof; 
	_test_eof563:  s.cs = 563; goto _test_eof; 
	_test_eof564:  s.cs = 564; goto _test_eof; 
	_test_eof565:  s.cs = 565; goto _test_eof; 
	_test_eof566:  s.cs = 566; goto _test_eof; 
	_test_eof567:  s.cs = 567; goto _test_eof; 
	_test_eof568:  s.cs = 568; goto _test_eof; 
	_test_eof569:  s.cs = 569; goto _test_eof; 
	_test_eof570:  s.cs = 570; goto _test_eof; 
	_test_eof571:  s.cs = 571; goto _test_eof; 
	_test_eof572:  s.cs = 572; goto _test_eof; 
	_test_eof573:  s.cs = 573; goto _test_eof; 
	_test_eof574:  s.cs = 574; goto _test_eof; 
	_test_eof575:  s.cs = 575; goto _test_eof; 
	_test_eof576:  s.cs = 576; goto _test_eof; 
	_test_eof577:  s.cs = 577; goto _test_eof; 
	_test_eof578:  s.cs = 578; goto _test_eof; 
	_test_eof579:  s.cs = 579; goto _test_eof; 
	_test_eof580:  s.cs = 580; goto _test_eof; 
	_test_eof1018:  s.cs = 1018; goto _test_eof; 
	_test_eof1019:  s.cs = 1019; goto _test_eof; 
	_test_eof1020:  s.cs = 1020; goto _test_eof; 
	_test_eof1021:  s.cs = 1021; goto _test_eof; 
	_test_eof1022:  s.cs = 1022; goto _test_eof; 
	_test_eof1023:  s.cs = 1023; goto _test_eof; 
	_test_eof1024:  s.cs = 1024; goto _test_eof; 
	_test_eof1025:  s.cs = 1025; goto _test_eof; 
	_test_eof1026:  s.cs = 1026; goto _test_eof; 
	_test_eof1027:  s.cs = 1027; goto _test_eof; 
	_test_eof1028:  s.cs = 1028; goto _test_eof; 
	_test_eof1029:  s.cs = 1029; goto _test_eof; 
	_test_eof1030:  s.cs = 1030; goto _test_eof; 
	_test_eof1031:  s.cs = 1031; goto _test_eof; 
	_test_eof1032:  s.cs = 1032; goto _test_eof; 
	_test_eof1033:  s.cs = 1033; goto _test_eof; 
	_test_eof1034:  s.cs = 1034; goto _test_eof; 
	_test_eof1035:  s.cs = 1035; goto _test_eof; 
	_test_eof1036:  s.cs = 1036; goto _test_eof; 
	_test_eof1037:  s.cs = 1037; goto _test_eof; 
	_test_eof1038:  s.cs = 1038; goto _test_eof; 
	_test_eof1039:  s.cs = 1039; goto _test_eof; 
	_test_eof1040:  s.cs = 1040; goto _test_eof; 
	_test_eof1041:  s.cs = 1041; goto _test_eof; 
	_test_eof1042:  s.cs = 1042; goto _test_eof; 
	_test_eof1043:  s.cs = 1043; goto _test_eof; 
	_test_eof581:  s.cs = 581; goto _test_eof; 
	_test_eof1044:  s.cs = 1044; goto _test_eof; 
	_test_eof1045:  s.cs = 1045; goto _test_eof; 
	_test_eof1046:  s.cs = 1046; goto _test_eof; 
	_test_eof1047:  s.cs = 1047; goto _test_eof; 
	_test_eof1048:  s.cs = 1048; goto _test_eof; 
	_test_eof1049:  s.cs = 1049; goto _test_eof; 
	_test_eof1050:  s.cs = 1050; goto _test_eof; 
	_test_eof1051:  s.cs = 1051; goto _test_eof; 
	_test_eof1052:  s.cs = 1052; goto _test_eof; 
	_test_eof1053:  s.cs = 1053; goto _test_eof; 
	_test_eof1054:  s.cs = 1054; goto _test_eof; 
	_test_eof1055:  s.cs = 1055; goto _test_eof; 
	_test_eof1056:  s.cs = 1056; goto _test_eof; 
	_test_eof1057:  s.cs = 1057; goto _test_eof; 
	_test_eof1058:  s.cs = 1058; goto _test_eof; 
	_test_eof1059:  s.cs = 1059; goto _test_eof; 
	_test_eof582:  s.cs = 582; goto _test_eof; 
	_test_eof583:  s.cs = 583; goto _test_eof; 
	_test_eof1060:  s.cs = 1060; goto _test_eof; 
	_test_eof1061:  s.cs = 1061; goto _test_eof; 
	_test_eof1062:  s.cs = 1062; goto _test_eof; 
	_test_eof1063:  s.cs = 1063; goto _test_eof; 
	_test_eof1064:  s.cs = 1064; goto _test_eof; 
	_test_eof1065:  s.cs = 1065; goto _test_eof; 
	_test_eof1066:  s.cs = 1066; goto _test_eof; 
	_test_eof1067:  s.cs = 1067; goto _test_eof; 
	_test_eof1068:  s.cs = 1068; goto _test_eof; 
	_test_eof1069:  s.cs = 1069; goto _test_eof; 
	_test_eof1070:  s.cs = 1070; goto _test_eof; 
	_test_eof1071:  s.cs = 1071; goto _test_eof; 
	_test_eof1072:  s.cs = 1072; goto _test_eof; 
	_test_eof1073:  s.cs = 1073; goto _test_eof; 
	_test_eof1074:  s.cs = 1074; goto _test_eof; 
	_test_eof1075:  s.cs = 1075; goto _test_eof; 
	_test_eof1076:  s.cs = 1076; goto _test_eof; 
	_test_eof1077:  s.cs = 1077; goto _test_eof; 
	_test_eof1078:  s.cs = 1078; goto _test_eof; 
	_test_eof1079:  s.cs = 1079; goto _test_eof; 
	_test_eof1080:  s.cs = 1080; goto _test_eof; 
	_test_eof1081:  s.cs = 1081; goto _test_eof; 
	_test_eof1082:  s.cs = 1082; goto _test_eof; 
	_test_eof1083:  s.cs = 1083; goto _test_eof; 
	_test_eof1084:  s.cs = 1084; goto _test_eof; 
	_test_eof1085:  s.cs = 1085; goto _test_eof; 
	_test_eof1086:  s.cs = 1086; goto _test_eof; 
	_test_eof1087:  s.cs = 1087; goto _test_eof; 
	_test_eof1088:  s.cs = 1088; goto _test_eof; 
	_test_eof1089:  s.cs = 1089; goto _test_eof; 
	_test_eof584:  s.cs = 584; goto _test_eof; 
	_test_eof1090:  s.cs = 1090; goto _test_eof; 
	_test_eof1091:  s.cs = 1091; goto _test_eof; 
	_test_eof1092:  s.cs = 1092; goto _test_eof; 
	_test_eof1093:  s.cs = 1093; goto _test_eof; 
	_test_eof1094:  s.cs = 1094; goto _test_eof; 
	_test_eof1095:  s.cs = 1095; goto _test_eof; 
	_test_eof1096:  s.cs = 1096; goto _test_eof; 
	_test_eof1097:  s.cs = 1097; goto _test_eof; 
	_test_eof1098:  s.cs = 1098; goto _test_eof; 
	_test_eof1099:  s.cs = 1099; goto _test_eof; 
	_test_eof1100:  s.cs = 1100; goto _test_eof; 
	_test_eof1101:  s.cs = 1101; goto _test_eof; 
	_test_eof1102:  s.cs = 1102; goto _test_eof; 
	_test_eof1103:  s.cs = 1103; goto _test_eof; 
	_test_eof1104:  s.cs = 1104; goto _test_eof; 
	_test_eof1105:  s.cs = 1105; goto _test_eof; 
	_test_eof1106:  s.cs = 1106; goto _test_eof; 
	_test_eof1107:  s.cs = 1107; goto _test_eof; 
	_test_eof1108:  s.cs = 1108; goto _test_eof; 
	_test_eof1109:  s.cs = 1109; goto _test_eof; 
	_test_eof585:  s.cs = 585; goto _test_eof; 
	_test_eof1110:  s.cs = 1110; goto _test_eof; 
	_test_eof1111:  s.cs = 1111; goto _test_eof; 
	_test_eof1112:  s.cs = 1112; goto _test_eof; 
	_test_eof1113:  s.cs = 1113; goto _test_eof; 
	_test_eof1114:  s.cs = 1114; goto _test_eof; 
	_test_eof1115:  s.cs = 1115; goto _test_eof; 
	_test_eof1116:  s.cs = 1116; goto _test_eof; 
	_test_eof1117:  s.cs = 1117; goto _test_eof; 
	_test_eof1118:  s.cs = 1118; goto _test_eof; 
	_test_eof1119:  s.cs = 1119; goto _test_eof; 
	_test_eof1120:  s.cs = 1120; goto _test_eof; 
	_test_eof1121:  s.cs = 1121; goto _test_eof; 
	_test_eof1122:  s.cs = 1122; goto _test_eof; 
	_test_eof1123:  s.cs = 1123; goto _test_eof; 
	_test_eof1124:  s.cs = 1124; goto _test_eof; 
	_test_eof1125:  s.cs = 1125; goto _test_eof; 
	_test_eof1126:  s.cs = 1126; goto _test_eof; 
	_test_eof1127:  s.cs = 1127; goto _test_eof; 
	_test_eof1128:  s.cs = 1128; goto _test_eof; 
	_test_eof1129:  s.cs = 1129; goto _test_eof; 
	_test_eof1130:  s.cs = 1130; goto _test_eof; 
	_test_eof1131:  s.cs = 1131; goto _test_eof; 
	_test_eof1132:  s.cs = 1132; goto _test_eof; 
	_test_eof1133:  s.cs = 1133; goto _test_eof; 
	_test_eof1134:  s.cs = 1134; goto _test_eof; 
	_test_eof1135:  s.cs = 1135; goto _test_eof; 
	_test_eof1136:  s.cs = 1136; goto _test_eof; 
	_test_eof1137:  s.cs = 1137; goto _test_eof; 
	_test_eof1138:  s.cs = 1138; goto _test_eof; 
	_test_eof1139:  s.cs = 1139; goto _test_eof; 
	_test_eof1140:  s.cs = 1140; goto _test_eof; 
	_test_eof1141:  s.cs = 1141; goto _test_eof; 
	_test_eof1142:  s.cs = 1142; goto _test_eof; 
	_test_eof1143:  s.cs = 1143; goto _test_eof; 
	_test_eof1144:  s.cs = 1144; goto _test_eof; 
	_test_eof1145:  s.cs = 1145; goto _test_eof; 
	_test_eof1146:  s.cs = 1146; goto _test_eof; 
	_test_eof1147:  s.cs = 1147; goto _test_eof; 
	_test_eof1148:  s.cs = 1148; goto _test_eof; 
	_test_eof1149:  s.cs = 1149; goto _test_eof; 
	_test_eof1150:  s.cs = 1150; goto _test_eof; 
	_test_eof1151:  s.cs = 1151; goto _test_eof; 
	_test_eof1152:  s.cs = 1152; goto _test_eof; 
	_test_eof1153:  s.cs = 1153; goto _test_eof; 
	_test_eof1154:  s.cs = 1154; goto _test_eof; 
	_test_eof1155:  s.cs = 1155; goto _test_eof; 
	_test_eof1156:  s.cs = 1156; goto _test_eof; 
	_test_eof1157:  s.cs = 1157; goto _test_eof; 
	_test_eof1158:  s.cs = 1158; goto _test_eof; 
	_test_eof1159:  s.cs = 1159; goto _test_eof; 
	_test_eof1160:  s.cs = 1160; goto _test_eof; 
	_test_eof1161:  s.cs = 1161; goto _test_eof; 
	_test_eof1162:  s.cs = 1162; goto _test_eof; 
	_test_eof1163:  s.cs = 1163; goto _test_eof; 
	_test_eof1164:  s.cs = 1164; goto _test_eof; 
	_test_eof586:  s.cs = 586; goto _test_eof; 
	_test_eof1165:  s.cs = 1165; goto _test_eof; 
	_test_eof1166:  s.cs = 1166; goto _test_eof; 
	_test_eof1167:  s.cs = 1167; goto _test_eof; 
	_test_eof1168:  s.cs = 1168; goto _test_eof; 
	_test_eof1169:  s.cs = 1169; goto _test_eof; 
	_test_eof1170:  s.cs = 1170; goto _test_eof; 
	_test_eof1171:  s.cs = 1171; goto _test_eof; 
	_test_eof1172:  s.cs = 1172; goto _test_eof; 
	_test_eof1173:  s.cs = 1173; goto _test_eof; 
	_test_eof1174:  s.cs = 1174; goto _test_eof; 
	_test_eof1175:  s.cs = 1175; goto _test_eof; 
	_test_eof1176:  s.cs = 1176; goto _test_eof; 
	_test_eof1177:  s.cs = 1177; goto _test_eof; 
	_test_eof1178:  s.cs = 1178; goto _test_eof; 
	_test_eof1179:  s.cs = 1179; goto _test_eof; 
	_test_eof1180:  s.cs = 1180; goto _test_eof; 
	_test_eof1181:  s.cs = 1181; goto _test_eof; 
	_test_eof1182:  s.cs = 1182; goto _test_eof; 
	_test_eof1183:  s.cs = 1183; goto _test_eof; 
	_test_eof1184:  s.cs = 1184; goto _test_eof; 
	_test_eof1185:  s.cs = 1185; goto _test_eof; 
	_test_eof1186:  s.cs = 1186; goto _test_eof; 
	_test_eof1187:  s.cs = 1187; goto _test_eof; 
	_test_eof1188:  s.cs = 1188; goto _test_eof; 
	_test_eof1189:  s.cs = 1189; goto _test_eof; 
	_test_eof1190:  s.cs = 1190; goto _test_eof; 
	_test_eof1191:  s.cs = 1191; goto _test_eof; 
	_test_eof1192:  s.cs = 1192; goto _test_eof; 
	_test_eof1193:  s.cs = 1193; goto _test_eof; 
	_test_eof1194:  s.cs = 1194; goto _test_eof; 
	_test_eof1195:  s.cs = 1195; goto _test_eof; 
	_test_eof1196:  s.cs = 1196; goto _test_eof; 
	_test_eof1197:  s.cs = 1197; goto _test_eof; 
	_test_eof1198:  s.cs = 1198; goto _test_eof; 
	_test_eof1199:  s.cs = 1199; goto _test_eof; 
	_test_eof1200:  s.cs = 1200; goto _test_eof; 
	_test_eof1201:  s.cs = 1201; goto _test_eof; 
	_test_eof1202:  s.cs = 1202; goto _test_eof; 
	_test_eof1203:  s.cs = 1203; goto _test_eof; 
	_test_eof1204:  s.cs = 1204; goto _test_eof; 
	_test_eof1205:  s.cs = 1205; goto _test_eof; 
	_test_eof1206:  s.cs = 1206; goto _test_eof; 
	_test_eof1207:  s.cs = 1207; goto _test_eof; 
	_test_eof1208:  s.cs = 1208; goto _test_eof; 
	_test_eof1209:  s.cs = 1209; goto _test_eof; 
	_test_eof587:  s.cs = 587; goto _test_eof; 
	_test_eof1210:  s.cs = 1210; goto _test_eof; 
	_test_eof1211:  s.cs = 1211; goto _test_eof; 
	_test_eof1212:  s.cs = 1212; goto _test_eof; 
	_test_eof1213:  s.cs = 1213; goto _test_eof; 
	_test_eof1214:  s.cs = 1214; goto _test_eof; 
	_test_eof1215:  s.cs = 1215; goto _test_eof; 
	_test_eof1216:  s.cs = 1216; goto _test_eof; 
	_test_eof1217:  s.cs = 1217; goto _test_eof; 
	_test_eof1218:  s.cs = 1218; goto _test_eof; 
	_test_eof1219:  s.cs = 1219; goto _test_eof; 
	_test_eof1220:  s.cs = 1220; goto _test_eof; 
	_test_eof1221:  s.cs = 1221; goto _test_eof; 
	_test_eof1222:  s.cs = 1222; goto _test_eof; 
	_test_eof1223:  s.cs = 1223; goto _test_eof; 
	_test_eof1224:  s.cs = 1224; goto _test_eof; 
	_test_eof1225:  s.cs = 1225; goto _test_eof; 
	_test_eof1226:  s.cs = 1226; goto _test_eof; 
	_test_eof1227:  s.cs = 1227; goto _test_eof; 
	_test_eof1228:  s.cs = 1228; goto _test_eof; 
	_test_eof1229:  s.cs = 1229; goto _test_eof; 
	_test_eof1230:  s.cs = 1230; goto _test_eof; 
	_test_eof588:  s.cs = 588; goto _test_eof; 
	_test_eof589:  s.cs = 589; goto _test_eof; 
	_test_eof590:  s.cs = 590; goto _test_eof; 
	_test_eof591:  s.cs = 591; goto _test_eof; 
	_test_eof592:  s.cs = 592; goto _test_eof; 
	_test_eof593:  s.cs = 593; goto _test_eof; 
	_test_eof594:  s.cs = 594; goto _test_eof; 
	_test_eof595:  s.cs = 595; goto _test_eof; 
	_test_eof1231:  s.cs = 1231; goto _test_eof; 
	_test_eof1232:  s.cs = 1232; goto _test_eof; 
	_test_eof1233:  s.cs = 1233; goto _test_eof; 
	_test_eof1234:  s.cs = 1234; goto _test_eof; 
	_test_eof1235:  s.cs = 1235; goto _test_eof; 
	_test_eof1236:  s.cs = 1236; goto _test_eof; 
	_test_eof1237:  s.cs = 1237; goto _test_eof; 
	_test_eof596:  s.cs = 596; goto _test_eof; 
	_test_eof1238:  s.cs = 1238; goto _test_eof; 
	_test_eof597:  s.cs = 597; goto _test_eof; 
	_test_eof1239:  s.cs = 1239; goto _test_eof; 
	_test_eof1240:  s.cs = 1240; goto _test_eof; 
	_test_eof1241:  s.cs = 1241; goto _test_eof; 
	_test_eof1242:  s.cs = 1242; goto _test_eof; 
	_test_eof1243:  s.cs = 1243; goto _test_eof; 
	_test_eof1244:  s.cs = 1244; goto _test_eof; 
	_test_eof1245:  s.cs = 1245; goto _test_eof; 
	_test_eof1246:  s.cs = 1246; goto _test_eof; 
	_test_eof1247:  s.cs = 1247; goto _test_eof; 
	_test_eof1248:  s.cs = 1248; goto _test_eof; 
	_test_eof1249:  s.cs = 1249; goto _test_eof; 
	_test_eof1250:  s.cs = 1250; goto _test_eof; 
	_test_eof1251:  s.cs = 1251; goto _test_eof; 
	_test_eof1252:  s.cs = 1252; goto _test_eof; 
	_test_eof1253:  s.cs = 1253; goto _test_eof; 
	_test_eof1254:  s.cs = 1254; goto _test_eof; 
	_test_eof1255:  s.cs = 1255; goto _test_eof; 
	_test_eof1256:  s.cs = 1256; goto _test_eof; 
	_test_eof1257:  s.cs = 1257; goto _test_eof; 
	_test_eof1258:  s.cs = 1258; goto _test_eof; 
	_test_eof1259:  s.cs = 1259; goto _test_eof; 
	_test_eof1260:  s.cs = 1260; goto _test_eof; 
	_test_eof598:  s.cs = 598; goto _test_eof; 
	_test_eof1261:  s.cs = 1261; goto _test_eof; 
	_test_eof1262:  s.cs = 1262; goto _test_eof; 
	_test_eof1263:  s.cs = 1263; goto _test_eof; 
	_test_eof1264:  s.cs = 1264; goto _test_eof; 
	_test_eof1265:  s.cs = 1265; goto _test_eof; 
	_test_eof1266:  s.cs = 1266; goto _test_eof; 
	_test_eof1267:  s.cs = 1267; goto _test_eof; 
	_test_eof1268:  s.cs = 1268; goto _test_eof; 
	_test_eof1269:  s.cs = 1269; goto _test_eof; 
	_test_eof1270:  s.cs = 1270; goto _test_eof; 
	_test_eof1271:  s.cs = 1271; goto _test_eof; 
	_test_eof1272:  s.cs = 1272; goto _test_eof; 
	_test_eof1273:  s.cs = 1273; goto _test_eof; 
	_test_eof1274:  s.cs = 1274; goto _test_eof; 
	_test_eof1275:  s.cs = 1275; goto _test_eof; 
	_test_eof1276:  s.cs = 1276; goto _test_eof; 
	_test_eof1277:  s.cs = 1277; goto _test_eof; 
	_test_eof1278:  s.cs = 1278; goto _test_eof; 
	_test_eof1279:  s.cs = 1279; goto _test_eof; 
	_test_eof1280:  s.cs = 1280; goto _test_eof; 
	_test_eof1281:  s.cs = 1281; goto _test_eof; 
	_test_eof1282:  s.cs = 1282; goto _test_eof; 
	_test_eof1283:  s.cs = 1283; goto _test_eof; 
	_test_eof1284:  s.cs = 1284; goto _test_eof; 
	_test_eof1285:  s.cs = 1285; goto _test_eof; 
	_test_eof1286:  s.cs = 1286; goto _test_eof; 
	_test_eof1287:  s.cs = 1287; goto _test_eof; 
	_test_eof1288:  s.cs = 1288; goto _test_eof; 
	_test_eof1289:  s.cs = 1289; goto _test_eof; 
	_test_eof1290:  s.cs = 1290; goto _test_eof; 
	_test_eof1291:  s.cs = 1291; goto _test_eof; 
	_test_eof1292:  s.cs = 1292; goto _test_eof; 
	_test_eof1293:  s.cs = 1293; goto _test_eof; 
	_test_eof1294:  s.cs = 1294; goto _test_eof; 
	_test_eof1295:  s.cs = 1295; goto _test_eof; 
	_test_eof1296:  s.cs = 1296; goto _test_eof; 
	_test_eof1297:  s.cs = 1297; goto _test_eof; 
	_test_eof1298:  s.cs = 1298; goto _test_eof; 
	_test_eof1299:  s.cs = 1299; goto _test_eof; 
	_test_eof1300:  s.cs = 1300; goto _test_eof; 
	_test_eof1301:  s.cs = 1301; goto _test_eof; 
	_test_eof1302:  s.cs = 1302; goto _test_eof; 
	_test_eof1303:  s.cs = 1303; goto _test_eof; 
	_test_eof1304:  s.cs = 1304; goto _test_eof; 
	_test_eof1305:  s.cs = 1305; goto _test_eof; 
	_test_eof1306:  s.cs = 1306; goto _test_eof; 
	_test_eof1307:  s.cs = 1307; goto _test_eof; 
	_test_eof1308:  s.cs = 1308; goto _test_eof; 
	_test_eof1309:  s.cs = 1309; goto _test_eof; 
	_test_eof1310:  s.cs = 1310; goto _test_eof; 
	_test_eof1311:  s.cs = 1311; goto _test_eof; 
	_test_eof1312:  s.cs = 1312; goto _test_eof; 
	_test_eof1313:  s.cs = 1313; goto _test_eof; 
	_test_eof1314:  s.cs = 1314; goto _test_eof; 
	_test_eof1315:  s.cs = 1315; goto _test_eof; 
	_test_eof1316:  s.cs = 1316; goto _test_eof; 
	_test_eof1317:  s.cs = 1317; goto _test_eof; 
	_test_eof1318:  s.cs = 1318; goto _test_eof; 
	_test_eof1319:  s.cs = 1319; goto _test_eof; 
	_test_eof1320:  s.cs = 1320; goto _test_eof; 
	_test_eof1321:  s.cs = 1321; goto _test_eof; 
	_test_eof1322:  s.cs = 1322; goto _test_eof; 
	_test_eof1323:  s.cs = 1323; goto _test_eof; 
	_test_eof1324:  s.cs = 1324; goto _test_eof; 
	_test_eof1325:  s.cs = 1325; goto _test_eof; 
	_test_eof1326:  s.cs = 1326; goto _test_eof; 
	_test_eof1327:  s.cs = 1327; goto _test_eof; 
	_test_eof1328:  s.cs = 1328; goto _test_eof; 
	_test_eof1329:  s.cs = 1329; goto _test_eof; 
	_test_eof1330:  s.cs = 1330; goto _test_eof; 
	_test_eof1331:  s.cs = 1331; goto _test_eof; 
	_test_eof1332:  s.cs = 1332; goto _test_eof; 
	_test_eof1333:  s.cs = 1333; goto _test_eof; 
	_test_eof599:  s.cs = 599; goto _test_eof; 
	_test_eof1334:  s.cs = 1334; goto _test_eof; 
	_test_eof1335:  s.cs = 1335; goto _test_eof; 
	_test_eof1336:  s.cs = 1336; goto _test_eof; 
	_test_eof600:  s.cs = 600; goto _test_eof; 
	_test_eof601:  s.cs = 601; goto _test_eof; 
	_test_eof1337:  s.cs = 1337; goto _test_eof; 
	_test_eof1338:  s.cs = 1338; goto _test_eof; 
	_test_eof1339:  s.cs = 1339; goto _test_eof; 
	_test_eof1340:  s.cs = 1340; goto _test_eof; 
	_test_eof1341:  s.cs = 1341; goto _test_eof; 
	_test_eof1342:  s.cs = 1342; goto _test_eof; 
	_test_eof1343:  s.cs = 1343; goto _test_eof; 
	_test_eof1344:  s.cs = 1344; goto _test_eof; 
	_test_eof1345:  s.cs = 1345; goto _test_eof; 
	_test_eof1346:  s.cs = 1346; goto _test_eof; 
	_test_eof1347:  s.cs = 1347; goto _test_eof; 
	_test_eof1348:  s.cs = 1348; goto _test_eof; 
	_test_eof1349:  s.cs = 1349; goto _test_eof; 
	_test_eof1350:  s.cs = 1350; goto _test_eof; 
	_test_eof602:  s.cs = 602; goto _test_eof; 
	_test_eof1351:  s.cs = 1351; goto _test_eof; 
	_test_eof1352:  s.cs = 1352; goto _test_eof; 
	_test_eof1353:  s.cs = 1353; goto _test_eof; 
	_test_eof1354:  s.cs = 1354; goto _test_eof; 
	_test_eof1355:  s.cs = 1355; goto _test_eof; 
	_test_eof1356:  s.cs = 1356; goto _test_eof; 
	_test_eof603:  s.cs = 603; goto _test_eof; 
	_test_eof1357:  s.cs = 1357; goto _test_eof; 
	_test_eof1358:  s.cs = 1358; goto _test_eof; 
	_test_eof1359:  s.cs = 1359; goto _test_eof; 
	_test_eof604:  s.cs = 604; goto _test_eof; 
	_test_eof1360:  s.cs = 1360; goto _test_eof; 
	_test_eof1361:  s.cs = 1361; goto _test_eof; 
	_test_eof1362:  s.cs = 1362; goto _test_eof; 
	_test_eof1363:  s.cs = 1363; goto _test_eof; 
	_test_eof1364:  s.cs = 1364; goto _test_eof; 
	_test_eof1365:  s.cs = 1365; goto _test_eof; 
	_test_eof1366:  s.cs = 1366; goto _test_eof; 
	_test_eof1367:  s.cs = 1367; goto _test_eof; 
	_test_eof605:  s.cs = 605; goto _test_eof; 
	_test_eof606:  s.cs = 606; goto _test_eof; 
	_test_eof1368:  s.cs = 1368; goto _test_eof; 
	_test_eof1369:  s.cs = 1369; goto _test_eof; 
	_test_eof1370:  s.cs = 1370; goto _test_eof; 
	_test_eof1371:  s.cs = 1371; goto _test_eof; 
	_test_eof1372:  s.cs = 1372; goto _test_eof; 
	_test_eof1373:  s.cs = 1373; goto _test_eof; 
	_test_eof607:  s.cs = 607; goto _test_eof; 
	_test_eof1374:  s.cs = 1374; goto _test_eof; 
	_test_eof1375:  s.cs = 1375; goto _test_eof; 
	_test_eof1376:  s.cs = 1376; goto _test_eof; 
	_test_eof1377:  s.cs = 1377; goto _test_eof; 
	_test_eof1378:  s.cs = 1378; goto _test_eof; 
	_test_eof1379:  s.cs = 1379; goto _test_eof; 
	_test_eof1380:  s.cs = 1380; goto _test_eof; 
	_test_eof1381:  s.cs = 1381; goto _test_eof; 
	_test_eof1382:  s.cs = 1382; goto _test_eof; 
	_test_eof608:  s.cs = 608; goto _test_eof; 
	_test_eof1383:  s.cs = 1383; goto _test_eof; 
	_test_eof1384:  s.cs = 1384; goto _test_eof; 
	_test_eof1385:  s.cs = 1385; goto _test_eof; 
	_test_eof1386:  s.cs = 1386; goto _test_eof; 
	_test_eof1387:  s.cs = 1387; goto _test_eof; 
	_test_eof1388:  s.cs = 1388; goto _test_eof; 
	_test_eof1389:  s.cs = 1389; goto _test_eof; 
	_test_eof1390:  s.cs = 1390; goto _test_eof; 
	_test_eof1391:  s.cs = 1391; goto _test_eof; 
	_test_eof1392:  s.cs = 1392; goto _test_eof; 
	_test_eof1393:  s.cs = 1393; goto _test_eof; 
	_test_eof1394:  s.cs = 1394; goto _test_eof; 
	_test_eof1395:  s.cs = 1395; goto _test_eof; 
	_test_eof1396:  s.cs = 1396; goto _test_eof; 
	_test_eof609:  s.cs = 609; goto _test_eof; 
	_test_eof1397:  s.cs = 1397; goto _test_eof; 
	_test_eof1398:  s.cs = 1398; goto _test_eof; 
	_test_eof1399:  s.cs = 1399; goto _test_eof; 
	_test_eof1400:  s.cs = 1400; goto _test_eof; 
	_test_eof1401:  s.cs = 1401; goto _test_eof; 
	_test_eof1402:  s.cs = 1402; goto _test_eof; 
	_test_eof1403:  s.cs = 1403; goto _test_eof; 
	_test_eof1404:  s.cs = 1404; goto _test_eof; 
	_test_eof1405:  s.cs = 1405; goto _test_eof; 
	_test_eof1406:  s.cs = 1406; goto _test_eof; 
	_test_eof1407:  s.cs = 1407; goto _test_eof; 
	_test_eof1408:  s.cs = 1408; goto _test_eof; 
	_test_eof1409:  s.cs = 1409; goto _test_eof; 
	_test_eof1410:  s.cs = 1410; goto _test_eof; 
	_test_eof1411:  s.cs = 1411; goto _test_eof; 
	_test_eof1412:  s.cs = 1412; goto _test_eof; 
	_test_eof1413:  s.cs = 1413; goto _test_eof; 
	_test_eof1414:  s.cs = 1414; goto _test_eof; 
	_test_eof1415:  s.cs = 1415; goto _test_eof; 
	_test_eof1416:  s.cs = 1416; goto _test_eof; 
	_test_eof1417:  s.cs = 1417; goto _test_eof; 
	_test_eof610:  s.cs = 610; goto _test_eof; 
	_test_eof1418:  s.cs = 1418; goto _test_eof; 
	_test_eof1419:  s.cs = 1419; goto _test_eof; 
	_test_eof1420:  s.cs = 1420; goto _test_eof; 
	_test_eof1421:  s.cs = 1421; goto _test_eof; 
	_test_eof1422:  s.cs = 1422; goto _test_eof; 
	_test_eof1423:  s.cs = 1423; goto _test_eof; 
	_test_eof611:  s.cs = 611; goto _test_eof; 
	_test_eof1424:  s.cs = 1424; goto _test_eof; 
	_test_eof1425:  s.cs = 1425; goto _test_eof; 
	_test_eof1426:  s.cs = 1426; goto _test_eof; 
	_test_eof1427:  s.cs = 1427; goto _test_eof; 
	_test_eof1428:  s.cs = 1428; goto _test_eof; 
	_test_eof612:  s.cs = 612; goto _test_eof; 
	_test_eof1429:  s.cs = 1429; goto _test_eof; 
	_test_eof1430:  s.cs = 1430; goto _test_eof; 
	_test_eof1431:  s.cs = 1431; goto _test_eof; 
	_test_eof1432:  s.cs = 1432; goto _test_eof; 
	_test_eof1433:  s.cs = 1433; goto _test_eof; 
	_test_eof1434:  s.cs = 1434; goto _test_eof; 
	_test_eof1435:  s.cs = 1435; goto _test_eof; 
	_test_eof1436:  s.cs = 1436; goto _test_eof; 
	_test_eof1437:  s.cs = 1437; goto _test_eof; 
	_test_eof1438:  s.cs = 1438; goto _test_eof; 
	_test_eof1439:  s.cs = 1439; goto _test_eof; 
	_test_eof613:  s.cs = 613; goto _test_eof; 
	_test_eof1440:  s.cs = 1440; goto _test_eof; 
	_test_eof1441:  s.cs = 1441; goto _test_eof; 
	_test_eof1442:  s.cs = 1442; goto _test_eof; 
	_test_eof1443:  s.cs = 1443; goto _test_eof; 
	_test_eof1444:  s.cs = 1444; goto _test_eof; 
	_test_eof614:  s.cs = 614; goto _test_eof; 
	_test_eof1445:  s.cs = 1445; goto _test_eof; 
	_test_eof1446:  s.cs = 1446; goto _test_eof; 
	_test_eof615:  s.cs = 615; goto _test_eof; 
	_test_eof1447:  s.cs = 1447; goto _test_eof; 
	_test_eof1448:  s.cs = 1448; goto _test_eof; 
	_test_eof616:  s.cs = 616; goto _test_eof; 
	_test_eof1449:  s.cs = 1449; goto _test_eof; 
	_test_eof1450:  s.cs = 1450; goto _test_eof; 
	_test_eof1451:  s.cs = 1451; goto _test_eof; 
	_test_eof1452:  s.cs = 1452; goto _test_eof; 
	_test_eof1453:  s.cs = 1453; goto _test_eof; 
	_test_eof1454:  s.cs = 1454; goto _test_eof; 
	_test_eof1455:  s.cs = 1455; goto _test_eof; 
	_test_eof1456:  s.cs = 1456; goto _test_eof; 
	_test_eof1457:  s.cs = 1457; goto _test_eof; 
	_test_eof1458:  s.cs = 1458; goto _test_eof; 
	_test_eof1459:  s.cs = 1459; goto _test_eof; 
	_test_eof1460:  s.cs = 1460; goto _test_eof; 
	_test_eof1461:  s.cs = 1461; goto _test_eof; 
	_test_eof1462:  s.cs = 1462; goto _test_eof; 
	_test_eof617:  s.cs = 617; goto _test_eof; 
	_test_eof1463:  s.cs = 1463; goto _test_eof; 
	_test_eof1464:  s.cs = 1464; goto _test_eof; 
	_test_eof1465:  s.cs = 1465; goto _test_eof; 
	_test_eof1466:  s.cs = 1466; goto _test_eof; 
	_test_eof618:  s.cs = 618; goto _test_eof; 
	_test_eof1467:  s.cs = 1467; goto _test_eof; 
	_test_eof619:  s.cs = 619; goto _test_eof; 
	_test_eof620:  s.cs = 620; goto _test_eof; 
	_test_eof621:  s.cs = 621; goto _test_eof; 
	_test_eof622:  s.cs = 622; goto _test_eof; 
	_test_eof1468:  s.cs = 1468; goto _test_eof; 
	_test_eof1469:  s.cs = 1469; goto _test_eof; 
	_test_eof1470:  s.cs = 1470; goto _test_eof; 
	_test_eof1471:  s.cs = 1471; goto _test_eof; 
	_test_eof1472:  s.cs = 1472; goto _test_eof; 
	_test_eof1473:  s.cs = 1473; goto _test_eof; 
	_test_eof1474:  s.cs = 1474; goto _test_eof; 
	_test_eof1475:  s.cs = 1475; goto _test_eof; 
	_test_eof1476:  s.cs = 1476; goto _test_eof; 
	_test_eof1477:  s.cs = 1477; goto _test_eof; 
	_test_eof1478:  s.cs = 1478; goto _test_eof; 
	_test_eof1479:  s.cs = 1479; goto _test_eof; 
	_test_eof1480:  s.cs = 1480; goto _test_eof; 
	_test_eof1481:  s.cs = 1481; goto _test_eof; 
	_test_eof1482:  s.cs = 1482; goto _test_eof; 
	_test_eof1483:  s.cs = 1483; goto _test_eof; 
	_test_eof1484:  s.cs = 1484; goto _test_eof; 
	_test_eof1485:  s.cs = 1485; goto _test_eof; 
	_test_eof1486:  s.cs = 1486; goto _test_eof; 
	_test_eof1487:  s.cs = 1487; goto _test_eof; 
	_test_eof1488:  s.cs = 1488; goto _test_eof; 
	_test_eof1489:  s.cs = 1489; goto _test_eof; 
	_test_eof1490:  s.cs = 1490; goto _test_eof; 
	_test_eof1491:  s.cs = 1491; goto _test_eof; 
	_test_eof1492:  s.cs = 1492; goto _test_eof; 
	_test_eof1493:  s.cs = 1493; goto _test_eof; 
	_test_eof623:  s.cs = 623; goto _test_eof; 
	_test_eof1494:  s.cs = 1494; goto _test_eof; 
	_test_eof1495:  s.cs = 1495; goto _test_eof; 
	_test_eof1496:  s.cs = 1496; goto _test_eof; 
	_test_eof1497:  s.cs = 1497; goto _test_eof; 
	_test_eof1498:  s.cs = 1498; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch (  s.cs ) {
	case 625: goto tr5;
	case 2: goto tr5;
	case 3: goto tr5;
	case 4: goto tr5;
	case 5: goto tr5;
	case 6: goto tr5;
	case 7: goto tr5;
	case 8: goto tr5;
	case 9: goto tr5;
	case 10: goto tr5;
	case 11: goto tr5;
	case 12: goto tr5;
	case 13: goto tr5;
	case 626: goto tr763;
	case 14: goto tr14;
	case 627: goto tr5;
	case 15: goto tr5;
	case 16: goto tr5;
	case 628: goto tr5;
	case 629: goto tr766;
	case 17: goto tr22;
	case 18: goto tr22;
	case 19: goto tr5;
	case 20: goto tr5;
	case 21: goto tr5;
	case 22: goto tr5;
	case 630: goto tr763;
	case 23: goto tr5;
	case 631: goto tr771;
	case 24: goto tr5;
	case 25: goto tr5;
	case 26: goto tr5;
	case 27: goto tr5;
	case 28: goto tr5;
	case 29: goto tr5;
	case 30: goto tr5;
	case 31: goto tr5;
	case 32: goto tr5;
	case 33: goto tr5;
	case 34: goto tr5;
	case 632: goto tr797;
	case 35: goto tr5;
	case 36: goto tr5;
	case 37: goto tr5;
	case 38: goto tr5;
	case 633: goto tr798;
	case 39: goto tr44;
	case 634: goto tr798;
	case 40: goto tr5;
	case 41: goto tr5;
	case 42: goto tr5;
	case 43: goto tr5;
	case 44: goto tr5;
	case 45: goto tr5;
	case 46: goto tr5;
	case 635: goto tr798;
	case 636: goto tr798;
	case 47: goto tr44;
	case 48: goto tr5;
	case 49: goto tr5;
	case 50: goto tr5;
	case 51: goto tr5;
	case 52: goto tr5;
	case 637: goto tr798;
	case 53: goto tr5;
	case 54: goto tr5;
	case 55: goto tr5;
	case 56: goto tr5;
	case 57: goto tr5;
	case 58: goto tr5;
	case 59: goto tr5;
	case 60: goto tr5;
	case 638: goto tr798;
	case 61: goto tr44;
	case 62: goto tr44;
	case 63: goto tr44;
	case 64: goto tr5;
	case 639: goto tr798;
	case 65: goto tr5;
	case 640: goto tr798;
	case 641: goto tr798;
	case 66: goto tr44;
	case 642: goto tr798;
	case 67: goto tr44;
	case 68: goto tr44;
	case 69: goto tr5;
	case 643: goto tr798;
	case 644: goto tr798;
	case 70: goto tr44;
	case 71: goto tr44;
	case 645: goto tr798;
	case 72: goto tr44;
	case 73: goto tr5;
	case 74: goto tr5;
	case 75: goto tr5;
	case 76: goto tr5;
	case 646: goto tr798;
	case 77: goto tr44;
	case 78: goto tr44;
	case 79: goto tr44;
	case 80: goto tr5;
	case 81: goto tr5;
	case 82: goto tr5;
	case 83: goto tr5;
	case 84: goto tr5;
	case 85: goto tr5;
	case 86: goto tr5;
	case 87: goto tr5;
	case 88: goto tr5;
	case 89: goto tr5;
	case 90: goto tr5;
	case 647: goto tr771;
	case 91: goto tr82;
	case 92: goto tr82;
	case 93: goto tr82;
	case 648: goto tr771;
	case 649: goto tr763;
	case 650: goto tr763;
	case 94: goto tr5;
	case 651: goto tr771;
	case 652: goto tr797;
	case 653: goto tr763;
	case 654: goto tr797;
	case 655: goto tr763;
	case 95: goto tr14;
	case 656: goto tr763;
	case 657: goto tr5;
	case 658: goto tr798;
	case 659: goto tr763;
	case 660: goto tr798;
	case 661: goto tr763;
	case 96: goto tr14;
	case 662: goto tr763;
	case 663: goto tr763;
	case 97: goto tr14;
	case 664: goto tr763;
	case 665: goto tr763;
	case 666: goto tr798;
	case 667: goto tr798;
	case 668: goto tr763;
	case 98: goto tr14;
	case 669: goto tr763;
	case 670: goto tr763;
	case 671: goto tr763;
	case 672: goto tr763;
	case 673: goto tr798;
	case 674: goto tr763;
	case 675: goto tr763;
	case 676: goto tr763;
	case 677: goto tr763;
	case 678: goto tr798;
	case 679: goto tr763;
	case 680: goto tr798;
	case 681: goto tr5;
	case 682: goto tr763;
	case 683: goto tr798;
	case 684: goto tr763;
	case 685: goto tr763;
	case 686: goto tr798;
	case 687: goto tr798;
	case 688: goto tr798;
	case 689: goto tr763;
	case 690: goto tr763;
	case 691: goto tr763;
	case 692: goto tr763;
	case 693: goto tr763;
	case 694: goto tr798;
	case 695: goto tr763;
	case 696: goto tr763;
	case 697: goto tr763;
	case 698: goto tr763;
	case 699: goto tr763;
	case 700: goto tr763;
	case 701: goto tr763;
	case 702: goto tr763;
	case 99: goto tr14;
	case 703: goto tr763;
	case 704: goto tr763;
	case 705: goto tr763;
	case 706: goto tr763;
	case 707: goto tr763;
	case 708: goto tr763;
	case 709: goto tr896;
	case 102: goto tr99;
	case 103: goto tr99;
	case 104: goto tr99;
	case 105: goto tr99;
	case 106: goto tr99;
	case 107: goto tr99;
	case 108: goto tr99;
	case 109: goto tr5;
	case 110: goto tr99;
	case 111: goto tr99;
	case 112: goto tr99;
	case 113: goto tr99;
	case 114: goto tr99;
	case 115: goto tr5;
	case 116: goto tr5;
	case 117: goto tr5;
	case 710: goto tr899;
	case 118: goto tr114;
	case 119: goto tr114;
	case 120: goto tr114;
	case 121: goto tr114;
	case 122: goto tr114;
	case 123: goto tr114;
	case 124: goto tr114;
	case 125: goto tr5;
	case 126: goto tr114;
	case 127: goto tr114;
	case 128: goto tr114;
	case 129: goto tr114;
	case 130: goto tr114;
	case 131: goto tr5;
	case 132: goto tr5;
	case 133: goto tr5;
	case 711: goto tr766;
	case 712: goto tr766;
	case 713: goto tr766;
	case 714: goto tr903;
	case 715: goto tr903;
	case 137: goto tr133;
	case 716: goto tr903;
	case 138: goto tr133;
	case 139: goto tr133;
	case 140: goto tr133;
	case 141: goto tr133;
	case 142: goto tr133;
	case 143: goto tr133;
	case 144: goto tr133;
	case 145: goto tr133;
	case 146: goto tr133;
	case 147: goto tr133;
	case 148: goto tr133;
	case 149: goto tr133;
	case 150: goto tr133;
	case 151: goto tr133;
	case 152: goto tr133;
	case 153: goto tr133;
	case 154: goto tr133;
	case 155: goto tr133;
	case 156: goto tr133;
	case 157: goto tr133;
	case 158: goto tr133;
	case 159: goto tr133;
	case 160: goto tr133;
	case 161: goto tr133;
	case 162: goto tr133;
	case 163: goto tr133;
	case 164: goto tr133;
	case 165: goto tr133;
	case 166: goto tr133;
	case 167: goto tr5;
	case 168: goto tr5;
	case 717: goto tr5;
	case 169: goto tr5;
	case 170: goto tr5;
	case 171: goto tr5;
	case 172: goto tr5;
	case 173: goto tr5;
	case 174: goto tr5;
	case 175: goto tr5;
	case 176: goto tr5;
	case 177: goto tr5;
	case 178: goto tr5;
	case 179: goto tr5;
	case 180: goto tr5;
	case 181: goto tr5;
	case 182: goto tr5;
	case 183: goto tr5;
	case 184: goto tr5;
	case 185: goto tr5;
	case 186: goto tr5;
	case 187: goto tr5;
	case 188: goto tr5;
	case 189: goto tr5;
	case 190: goto tr5;
	case 191: goto tr5;
	case 192: goto tr5;
	case 193: goto tr5;
	case 194: goto tr5;
	case 195: goto tr5;
	case 196: goto tr5;
	case 197: goto tr5;
	case 198: goto tr5;
	case 718: goto tr763;
	case 199: goto tr14;
	case 200: goto tr5;
	case 201: goto tr5;
	case 202: goto tr5;
	case 203: goto tr5;
	case 204: goto tr5;
	case 205: goto tr5;
	case 206: goto tr5;
	case 207: goto tr5;
	case 208: goto tr5;
	case 209: goto tr5;
	case 210: goto tr5;
	case 211: goto tr5;
	case 212: goto tr5;
	case 213: goto tr5;
	case 214: goto tr5;
	case 215: goto tr5;
	case 216: goto tr5;
	case 217: goto tr5;
	case 218: goto tr5;
	case 219: goto tr5;
	case 220: goto tr5;
	case 221: goto tr5;
	case 222: goto tr5;
	case 223: goto tr5;
	case 224: goto tr5;
	case 719: goto tr5;
	case 225: goto tr5;
	case 226: goto tr5;
	case 720: goto tr763;
	case 721: goto tr5;
	case 227: goto tr5;
	case 722: goto tr763;
	case 723: goto tr763;
	case 724: goto tr5;
	case 725: goto tr763;
	case 726: goto tr763;
	case 228: goto tr133;
	case 229: goto tr133;
	case 230: goto tr133;
	case 231: goto tr133;
	case 232: goto tr133;
	case 233: goto tr133;
	case 234: goto tr133;
	case 235: goto tr133;
	case 236: goto tr133;
	case 237: goto tr133;
	case 238: goto tr133;
	case 239: goto tr133;
	case 240: goto tr133;
	case 241: goto tr133;
	case 242: goto tr133;
	case 243: goto tr133;
	case 244: goto tr133;
	case 245: goto tr133;
	case 727: goto tr921;
	case 246: goto tr5;
	case 728: goto tr5;
	case 247: goto tr273;
	case 248: goto tr273;
	case 729: goto tr5;
	case 730: goto tr903;
	case 249: goto tr5;
	case 250: goto tr5;
	case 251: goto tr5;
	case 252: goto tr5;
	case 253: goto tr5;
	case 254: goto tr5;
	case 255: goto tr5;
	case 256: goto tr5;
	case 731: goto tr927;
	case 732: goto tr927;
	case 733: goto tr927;
	case 734: goto tr927;
	case 257: goto tr5;
	case 258: goto tr5;
	case 259: goto tr5;
	case 260: goto tr5;
	case 261: goto tr5;
	case 262: goto tr5;
	case 263: goto tr5;
	case 264: goto tr5;
	case 265: goto tr5;
	case 266: goto tr5;
	case 267: goto tr5;
	case 268: goto tr5;
	case 269: goto tr5;
	case 270: goto tr5;
	case 271: goto tr5;
	case 272: goto tr5;
	case 273: goto tr5;
	case 274: goto tr5;
	case 275: goto tr5;
	case 735: goto tr927;
	case 736: goto tr927;
	case 276: goto tr323;
	case 737: goto tr927;
	case 277: goto tr323;
	case 738: goto tr927;
	case 278: goto tr5;
	case 739: goto tr927;
	case 279: goto tr5;
	case 740: goto tr927;
	case 741: goto tr927;
	case 280: goto tr323;
	case 281: goto tr5;
	case 282: goto tr5;
	case 742: goto tr927;
	case 283: goto tr5;
	case 743: goto tr927;
	case 284: goto tr5;
	case 744: goto tr927;
	case 285: goto tr5;
	case 745: goto tr927;
	case 286: goto tr323;
	case 287: goto tr5;
	case 746: goto tr927;
	case 747: goto tr927;
	case 288: goto tr323;
	case 748: goto tr927;
	case 289: goto tr323;
	case 290: goto tr323;
	case 291: goto tr323;
	case 292: goto tr5;
	case 749: goto tr927;
	case 293: goto tr323;
	case 294: goto tr5;
	case 750: goto tr927;
	case 295: goto tr5;
	case 751: goto tr927;
	case 296: goto tr5;
	case 752: goto tr927;
	case 297: goto tr323;
	case 298: goto tr323;
	case 299: goto tr323;
	case 300: goto tr5;
	case 753: goto tr927;
	case 301: goto tr5;
	case 754: goto tr771;
	case 302: goto tr5;
	case 755: goto tr797;
	case 303: goto tr82;
	case 304: goto tr82;
	case 305: goto tr82;
	case 756: goto tr5;
	case 306: goto tr82;
	case 757: goto tr798;
	case 307: goto tr44;
	case 758: goto tr798;
	case 308: goto tr82;
	case 309: goto tr82;
	case 310: goto tr82;
	case 311: goto tr82;
	case 312: goto tr82;
	case 313: goto tr82;
	case 314: goto tr82;
	case 759: goto tr798;
	case 760: goto tr798;
	case 315: goto tr5;
	case 316: goto tr5;
	case 317: goto tr82;
	case 318: goto tr82;
	case 319: goto tr82;
	case 320: goto tr82;
	case 761: goto tr798;
	case 321: goto tr44;
	case 322: goto tr44;
	case 323: goto tr44;
	case 324: goto tr82;
	case 762: goto tr798;
	case 325: goto tr82;
	case 763: goto tr798;
	case 764: goto tr798;
	case 326: goto tr44;
	case 765: goto tr798;
	case 327: goto tr44;
	case 328: goto tr82;
	case 766: goto tr798;
	case 767: goto tr798;
	case 329: goto tr44;
	case 768: goto tr798;
	case 330: goto tr44;
	case 331: goto tr5;
	case 332: goto tr5;
	case 333: goto tr5;
	case 334: goto tr5;
	case 769: goto tr798;
	case 335: goto tr44;
	case 336: goto tr44;
	case 337: goto tr44;
	case 338: goto tr44;
	case 339: goto tr82;
	case 340: goto tr82;
	case 341: goto tr82;
	case 342: goto tr82;
	case 343: goto tr82;
	case 344: goto tr82;
	case 345: goto tr82;
	case 346: goto tr82;
	case 347: goto tr82;
	case 348: goto tr82;
	case 349: goto tr82;
	case 350: goto tr82;
	case 770: goto tr771;
	case 351: goto tr82;
	case 352: goto tr82;
	case 771: goto tr979;
	case 353: goto tr379;
	case 354: goto tr379;
	case 355: goto tr379;
	case 356: goto tr379;
	case 357: goto tr5;
	case 772: goto tr771;
	case 773: goto tr979;
	case 358: goto tr5;
	case 359: goto tr5;
	case 360: goto tr5;
	case 361: goto tr5;
	case 362: goto tr5;
	case 363: goto tr5;
	case 364: goto tr5;
	case 365: goto tr5;
	case 366: goto tr5;
	case 367: goto tr5;
	case 774: goto tr5;
	case 368: goto tr5;
	case 369: goto tr5;
	case 370: goto tr5;
	case 775: goto tr985;
	case 371: goto tr5;
	case 372: goto tr5;
	case 373: goto tr5;
	case 776: goto tr985;
	case 374: goto tr5;
	case 375: goto tr5;
	case 777: goto tr5;
	case 376: goto tr5;
	case 377: goto tr5;
	case 378: goto tr5;
	case 379: goto tr5;
	case 380: goto tr5;
	case 381: goto tr5;
	case 778: goto tr771;
	case 382: goto tr82;
	case 383: goto tr82;
	case 384: goto tr82;
	case 779: goto tr763;
	case 385: goto tr14;
	case 780: goto tr5;
	case 781: goto tr763;
	case 782: goto tr5;
	case 386: goto tr5;
	case 783: goto tr5;
	case 784: goto tr763;
	case 785: goto tr797;
	case 786: goto tr763;
	case 787: goto tr797;
	case 788: goto tr763;
	case 387: goto tr14;
	case 789: goto tr763;
	case 790: goto tr798;
	case 791: goto tr5;
	case 792: goto tr798;
	case 793: goto tr763;
	case 794: goto tr798;
	case 795: goto tr763;
	case 388: goto tr14;
	case 796: goto tr763;
	case 797: goto tr763;
	case 389: goto tr14;
	case 798: goto tr763;
	case 799: goto tr763;
	case 800: goto tr798;
	case 801: goto tr798;
	case 802: goto tr763;
	case 390: goto tr14;
	case 803: goto tr763;
	case 804: goto tr763;
	case 805: goto tr763;
	case 806: goto tr763;
	case 807: goto tr798;
	case 808: goto tr763;
	case 809: goto tr763;
	case 810: goto tr763;
	case 811: goto tr763;
	case 812: goto tr798;
	case 813: goto tr763;
	case 814: goto tr798;
	case 815: goto tr5;
	case 816: goto tr763;
	case 817: goto tr798;
	case 818: goto tr763;
	case 819: goto tr763;
	case 820: goto tr798;
	case 821: goto tr798;
	case 822: goto tr798;
	case 823: goto tr763;
	case 824: goto tr763;
	case 825: goto tr763;
	case 826: goto tr763;
	case 827: goto tr763;
	case 828: goto tr798;
	case 829: goto tr763;
	case 830: goto tr763;
	case 831: goto tr763;
	case 832: goto tr763;
	case 833: goto tr763;
	case 834: goto tr763;
	case 835: goto tr763;
	case 836: goto tr763;
	case 391: goto tr14;
	case 837: goto tr763;
	case 838: goto tr763;
	case 839: goto tr763;
	case 840: goto tr763;
	case 841: goto tr763;
	case 842: goto tr763;
	case 843: goto tr1069;
	case 392: goto tr427;
	case 393: goto tr427;
	case 394: goto tr427;
	case 395: goto tr427;
	case 844: goto tr5;
	case 845: goto tr771;
	case 846: goto tr763;
	case 847: goto tr763;
	case 848: goto tr763;
	case 849: goto tr763;
	case 850: goto tr771;
	case 851: goto tr763;
	case 852: goto tr771;
	case 853: goto tr763;
	case 396: goto tr82;
	case 854: goto tr771;
	case 397: goto tr82;
	case 398: goto tr82;
	case 399: goto tr82;
	case 855: goto tr927;
	case 856: goto tr927;
	case 857: goto tr927;
	case 858: goto tr927;
	case 859: goto tr771;
	case 400: goto tr82;
	case 860: goto tr771;
	case 401: goto tr82;
	case 861: goto tr771;
	case 402: goto tr82;
	case 862: goto tr771;
	case 403: goto tr82;
	case 863: goto tr771;
	case 864: goto tr771;
	case 404: goto tr82;
	case 405: goto tr82;
	case 406: goto tr82;
	case 407: goto tr82;
	case 408: goto tr82;
	case 409: goto tr82;
	case 410: goto tr82;
	case 411: goto tr82;
	case 412: goto tr82;
	case 413: goto tr82;
	case 414: goto tr82;
	case 415: goto tr82;
	case 416: goto tr82;
	case 417: goto tr82;
	case 418: goto tr82;
	case 419: goto tr82;
	case 865: goto tr771;
	case 866: goto tr771;
	case 867: goto tr771;
	case 868: goto tr771;
	case 420: goto tr82;
	case 869: goto tr771;
	case 421: goto tr82;
	case 422: goto tr82;
	case 870: goto tr771;
	case 871: goto tr771;
	case 872: goto tr771;
	case 423: goto tr82;
	case 873: goto tr771;
	case 424: goto tr82;
	case 874: goto tr771;
	case 875: goto tr771;
	case 876: goto tr771;
	case 877: goto tr771;
	case 878: goto tr771;
	case 425: goto tr82;
	case 426: goto tr82;
	case 879: goto tr771;
	case 427: goto tr82;
	case 428: goto tr82;
	case 429: goto tr82;
	case 430: goto tr82;
	case 431: goto tr82;
	case 432: goto tr82;
	case 433: goto tr82;
	case 434: goto tr82;
	case 435: goto tr82;
	case 436: goto tr82;
	case 880: goto tr771;
	case 881: goto tr771;
	case 882: goto tr771;
	case 883: goto tr771;
	case 884: goto tr771;
	case 885: goto tr771;
	case 886: goto tr771;
	case 887: goto tr903;
	case 437: goto tr133;
	case 438: goto tr133;
	case 439: goto tr133;
	case 440: goto tr133;
	case 441: goto tr133;
	case 442: goto tr133;
	case 888: goto tr1120;
	case 443: goto tr478;
	case 444: goto tr478;
	case 445: goto tr478;
	case 446: goto tr133;
	case 447: goto tr133;
	case 448: goto tr133;
	case 449: goto tr133;
	case 450: goto tr133;
	case 451: goto tr133;
	case 452: goto tr133;
	case 453: goto tr133;
	case 454: goto tr133;
	case 455: goto tr133;
	case 456: goto tr133;
	case 457: goto tr133;
	case 889: goto tr1120;
	case 458: goto tr478;
	case 459: goto tr478;
	case 460: goto tr478;
	case 461: goto tr478;
	case 462: goto tr478;
	case 463: goto tr478;
	case 464: goto tr478;
	case 465: goto tr478;
	case 466: goto tr478;
	case 467: goto tr133;
	case 468: goto tr133;
	case 469: goto tr133;
	case 470: goto tr133;
	case 890: goto tr1120;
	case 471: goto tr478;
	case 472: goto tr478;
	case 473: goto tr478;
	case 474: goto tr478;
	case 475: goto tr478;
	case 476: goto tr478;
	case 477: goto tr478;
	case 478: goto tr478;
	case 479: goto tr133;
	case 480: goto tr133;
	case 481: goto tr133;
	case 482: goto tr133;
	case 483: goto tr133;
	case 484: goto tr133;
	case 485: goto tr133;
	case 891: goto tr903;
	case 486: goto tr133;
	case 892: goto tr771;
	case 893: goto tr763;
	case 487: goto tr5;
	case 488: goto tr5;
	case 894: goto tr1139;
	case 895: goto tr1139;
	case 489: goto tr530;
	case 896: goto tr1139;
	case 490: goto tr14;
	case 897: goto tr763;
	case 491: goto tr14;
	case 898: goto tr763;
	case 492: goto tr14;
	case 493: goto tr5;
	case 494: goto tr5;
	case 495: goto tr5;
	case 496: goto tr5;
	case 899: goto tr927;
	case 900: goto tr927;
	case 497: goto tr323;
	case 901: goto tr927;
	case 498: goto tr323;
	case 902: goto tr927;
	case 499: goto tr5;
	case 903: goto tr927;
	case 500: goto tr5;
	case 904: goto tr927;
	case 905: goto tr927;
	case 501: goto tr323;
	case 502: goto tr5;
	case 503: goto tr5;
	case 906: goto tr927;
	case 504: goto tr5;
	case 907: goto tr927;
	case 505: goto tr5;
	case 908: goto tr927;
	case 506: goto tr5;
	case 909: goto tr927;
	case 507: goto tr323;
	case 508: goto tr5;
	case 910: goto tr927;
	case 911: goto tr927;
	case 509: goto tr323;
	case 912: goto tr927;
	case 510: goto tr323;
	case 511: goto tr323;
	case 512: goto tr323;
	case 513: goto tr5;
	case 913: goto tr927;
	case 514: goto tr323;
	case 515: goto tr5;
	case 914: goto tr927;
	case 516: goto tr5;
	case 915: goto tr927;
	case 517: goto tr5;
	case 916: goto tr927;
	case 518: goto tr323;
	case 519: goto tr323;
	case 520: goto tr323;
	case 521: goto tr5;
	case 917: goto tr927;
	case 918: goto tr763;
	case 522: goto tr5;
	case 919: goto tr763;
	case 920: goto tr763;
	case 523: goto tr14;
	case 921: goto tr5;
	case 524: goto tr5;
	case 922: goto tr763;
	case 525: goto tr14;
	case 923: goto tr927;
	case 924: goto tr5;
	case 925: goto tr5;
	case 926: goto tr5;
	case 927: goto tr927;
	case 928: goto tr763;
	case 929: goto tr763;
	case 930: goto tr927;
	case 931: goto tr927;
	case 932: goto tr927;
	case 933: goto tr927;
	case 934: goto tr763;
	case 935: goto tr927;
	case 936: goto tr927;
	case 937: goto tr763;
	case 938: goto tr927;
	case 939: goto tr927;
	case 940: goto tr763;
	case 941: goto tr763;
	case 942: goto tr927;
	case 943: goto tr927;
	case 944: goto tr763;
	case 945: goto tr763;
	case 946: goto tr927;
	case 947: goto tr927;
	case 948: goto tr763;
	case 949: goto tr927;
	case 950: goto tr927;
	case 951: goto tr763;
	case 952: goto tr927;
	case 953: goto tr927;
	case 954: goto tr763;
	case 955: goto tr927;
	case 956: goto tr763;
	case 957: goto tr763;
	case 958: goto tr927;
	case 959: goto tr763;
	case 960: goto tr5;
	case 961: goto tr927;
	case 962: goto tr763;
	case 963: goto tr763;
	case 964: goto tr927;
	case 965: goto tr763;
	case 966: goto tr763;
	case 967: goto tr763;
	case 968: goto tr763;
	case 969: goto tr763;
	case 970: goto tr763;
	case 971: goto tr5;
	case 972: goto tr927;
	case 973: goto tr927;
	case 974: goto tr763;
	case 975: goto tr927;
	case 976: goto tr763;
	case 977: goto tr763;
	case 978: goto tr927;
	case 979: goto tr763;
	case 980: goto tr927;
	case 981: goto tr927;
	case 982: goto tr763;
	case 983: goto tr763;
	case 984: goto tr927;
	case 985: goto tr763;
	case 986: goto tr763;
	case 987: goto tr763;
	case 988: goto tr763;
	case 989: goto tr927;
	case 990: goto tr763;
	case 991: goto tr927;
	case 992: goto tr927;
	case 993: goto tr5;
	case 994: goto tr1139;
	case 995: goto tr763;
	case 996: goto tr5;
	case 997: goto tr763;
	case 998: goto tr5;
	case 999: goto tr1139;
	case 1000: goto tr1139;
	case 526: goto tr5;
	case 1001: goto tr927;
	case 1002: goto tr1139;
	case 527: goto tr530;
	case 528: goto tr530;
	case 529: goto tr530;
	case 530: goto tr530;
	case 531: goto tr530;
	case 532: goto tr530;
	case 533: goto tr530;
	case 534: goto tr530;
	case 535: goto tr530;
	case 536: goto tr530;
	case 537: goto tr530;
	case 538: goto tr530;
	case 539: goto tr530;
	case 540: goto tr530;
	case 541: goto tr530;
	case 542: goto tr530;
	case 543: goto tr530;
	case 544: goto tr530;
	case 545: goto tr530;
	case 546: goto tr530;
	case 547: goto tr530;
	case 548: goto tr530;
	case 549: goto tr530;
	case 550: goto tr530;
	case 551: goto tr530;
	case 1003: goto tr763;
	case 1004: goto tr1139;
	case 1005: goto tr1139;
	case 552: goto tr530;
	case 553: goto tr530;
	case 1006: goto tr1139;
	case 1007: goto tr1139;
	case 1008: goto tr763;
	case 554: goto tr14;
	case 1009: goto tr763;
	case 1010: goto tr763;
	case 555: goto tr14;
	case 1011: goto tr763;
	case 1012: goto tr5;
	case 1013: goto tr763;
	case 1014: goto tr763;
	case 1015: goto tr763;
	case 1016: goto tr763;
	case 1017: goto tr763;
	case 556: goto tr14;
	case 557: goto tr14;
	case 558: goto tr14;
	case 559: goto tr14;
	case 560: goto tr14;
	case 561: goto tr14;
	case 562: goto tr14;
	case 563: goto tr14;
	case 564: goto tr14;
	case 565: goto tr14;
	case 566: goto tr14;
	case 567: goto tr14;
	case 568: goto tr14;
	case 569: goto tr14;
	case 570: goto tr14;
	case 571: goto tr14;
	case 572: goto tr14;
	case 573: goto tr14;
	case 574: goto tr14;
	case 575: goto tr14;
	case 576: goto tr14;
	case 577: goto tr14;
	case 578: goto tr14;
	case 579: goto tr14;
	case 580: goto tr14;
	case 1018: goto tr763;
	case 1019: goto tr763;
	case 1020: goto tr763;
	case 1021: goto tr763;
	case 1022: goto tr763;
	case 1023: goto tr763;
	case 1024: goto tr763;
	case 1025: goto tr763;
	case 1026: goto tr763;
	case 1027: goto tr763;
	case 1028: goto tr763;
	case 1029: goto tr763;
	case 1030: goto tr763;
	case 1031: goto tr1139;
	case 1032: goto tr763;
	case 1033: goto tr763;
	case 1034: goto tr763;
	case 1035: goto tr763;
	case 1036: goto tr763;
	case 1037: goto tr763;
	case 1038: goto tr763;
	case 1039: goto tr763;
	case 1040: goto tr763;
	case 1041: goto tr763;
	case 1042: goto tr763;
	case 1043: goto tr763;
	case 581: goto tr14;
	case 1044: goto tr1278;
	case 1045: goto tr763;
	case 1046: goto tr1139;
	case 1047: goto tr1139;
	case 1048: goto tr1139;
	case 1049: goto tr1139;
	case 1050: goto tr763;
	case 1051: goto tr763;
	case 1052: goto tr763;
	case 1053: goto tr763;
	case 1054: goto tr763;
	case 1055: goto tr1139;
	case 1056: goto tr1139;
	case 1057: goto tr1139;
	case 1058: goto tr1139;
	case 1059: goto tr1139;
	case 582: goto tr5;
	case 583: goto tr5;
	case 1060: goto tr763;
	case 1061: goto tr763;
	case 1062: goto tr763;
	case 1063: goto tr763;
	case 1064: goto tr763;
	case 1065: goto tr763;
	case 1066: goto tr763;
	case 1067: goto tr1139;
	case 1068: goto tr1139;
	case 1069: goto tr1139;
	case 1070: goto tr1139;
	case 1071: goto tr1139;
	case 1072: goto tr763;
	case 1073: goto tr763;
	case 1074: goto tr763;
	case 1075: goto tr763;
	case 1076: goto tr763;
	case 1077: goto tr763;
	case 1078: goto tr763;
	case 1079: goto tr763;
	case 1080: goto tr763;
	case 1081: goto tr763;
	case 1082: goto tr763;
	case 1083: goto tr763;
	case 1084: goto tr763;
	case 1085: goto tr763;
	case 1086: goto tr763;
	case 1087: goto tr763;
	case 1088: goto tr763;
	case 1089: goto tr763;
	case 584: goto tr14;
	case 1090: goto tr763;
	case 1091: goto tr763;
	case 1092: goto tr763;
	case 1093: goto tr763;
	case 1094: goto tr763;
	case 1095: goto tr763;
	case 1096: goto tr763;
	case 1097: goto tr763;
	case 1098: goto tr1139;
	case 1099: goto tr1139;
	case 1100: goto tr763;
	case 1101: goto tr1139;
	case 1102: goto tr1139;
	case 1103: goto tr763;
	case 1104: goto tr1139;
	case 1105: goto tr1139;
	case 1106: goto tr763;
	case 1107: goto tr763;
	case 1108: goto tr763;
	case 1109: goto tr763;
	case 585: goto tr14;
	case 1110: goto tr763;
	case 1111: goto tr763;
	case 1112: goto tr763;
	case 1113: goto tr763;
	case 1114: goto tr763;
	case 1115: goto tr763;
	case 1116: goto tr763;
	case 1117: goto tr763;
	case 1118: goto tr763;
	case 1119: goto tr763;
	case 1120: goto tr763;
	case 1121: goto tr1139;
	case 1122: goto tr1139;
	case 1123: goto tr1139;
	case 1124: goto tr763;
	case 1125: goto tr763;
	case 1126: goto tr763;
	case 1127: goto tr763;
	case 1128: goto tr763;
	case 1129: goto tr763;
	case 1130: goto tr763;
	case 1131: goto tr1139;
	case 1132: goto tr1139;
	case 1133: goto tr1139;
	case 1134: goto tr1139;
	case 1135: goto tr763;
	case 1136: goto tr763;
	case 1137: goto tr763;
	case 1138: goto tr763;
	case 1139: goto tr763;
	case 1140: goto tr1139;
	case 1141: goto tr1139;
	case 1142: goto tr1139;
	case 1143: goto tr763;
	case 1144: goto tr763;
	case 1145: goto tr763;
	case 1146: goto tr763;
	case 1147: goto tr763;
	case 1148: goto tr763;
	case 1149: goto tr763;
	case 1150: goto tr763;
	case 1151: goto tr763;
	case 1152: goto tr763;
	case 1153: goto tr1139;
	case 1154: goto tr1139;
	case 1155: goto tr1139;
	case 1156: goto tr1139;
	case 1157: goto tr763;
	case 1158: goto tr763;
	case 1159: goto tr763;
	case 1160: goto tr763;
	case 1161: goto tr763;
	case 1162: goto tr763;
	case 1163: goto tr763;
	case 1164: goto tr763;
	case 586: goto tr14;
	case 1165: goto tr763;
	case 1166: goto tr763;
	case 1167: goto tr763;
	case 1168: goto tr763;
	case 1169: goto tr763;
	case 1170: goto tr763;
	case 1171: goto tr763;
	case 1172: goto tr1139;
	case 1173: goto tr763;
	case 1174: goto tr1139;
	case 1175: goto tr763;
	case 1176: goto tr763;
	case 1177: goto tr763;
	case 1178: goto tr763;
	case 1179: goto tr763;
	case 1180: goto tr763;
	case 1181: goto tr763;
	case 1182: goto tr763;
	case 1183: goto tr763;
	case 1184: goto tr763;
	case 1185: goto tr763;
	case 1186: goto tr763;
	case 1187: goto tr763;
	case 1188: goto tr763;
	case 1189: goto tr763;
	case 1190: goto tr1139;
	case 1191: goto tr1139;
	case 1192: goto tr1139;
	case 1193: goto tr1139;
	case 1194: goto tr763;
	case 1195: goto tr763;
	case 1196: goto tr763;
	case 1197: goto tr763;
	case 1198: goto tr763;
	case 1199: goto tr1139;
	case 1200: goto tr763;
	case 1201: goto tr763;
	case 1202: goto tr763;
	case 1203: goto tr763;
	case 1204: goto tr763;
	case 1205: goto tr763;
	case 1206: goto tr763;
	case 1207: goto tr763;
	case 1208: goto tr1139;
	case 1209: goto tr763;
	case 587: goto tr14;
	case 1210: goto tr763;
	case 1211: goto tr763;
	case 1212: goto tr763;
	case 1213: goto tr763;
	case 1214: goto tr763;
	case 1215: goto tr763;
	case 1216: goto tr763;
	case 1217: goto tr763;
	case 1218: goto tr763;
	case 1219: goto tr763;
	case 1220: goto tr763;
	case 1221: goto tr763;
	case 1222: goto tr763;
	case 1223: goto tr763;
	case 1224: goto tr763;
	case 1225: goto tr763;
	case 1226: goto tr763;
	case 1227: goto tr763;
	case 1228: goto tr763;
	case 1229: goto tr763;
	case 1230: goto tr1139;
	case 588: goto tr5;
	case 589: goto tr5;
	case 590: goto tr5;
	case 591: goto tr5;
	case 592: goto tr5;
	case 593: goto tr5;
	case 594: goto tr5;
	case 595: goto tr5;
	case 1231: goto tr1139;
	case 1232: goto tr1139;
	case 1233: goto tr763;
	case 1234: goto tr763;
	case 1235: goto tr763;
	case 1236: goto tr763;
	case 1237: goto tr763;
	case 596: goto tr14;
	case 1238: goto tr763;
	case 597: goto tr14;
	case 1239: goto tr763;
	case 1240: goto tr763;
	case 1241: goto tr763;
	case 1242: goto tr763;
	case 1243: goto tr763;
	case 1244: goto tr763;
	case 1245: goto tr763;
	case 1246: goto tr763;
	case 1247: goto tr763;
	case 1248: goto tr1139;
	case 1249: goto tr763;
	case 1250: goto tr763;
	case 1251: goto tr763;
	case 1252: goto tr763;
	case 1253: goto tr763;
	case 1254: goto tr763;
	case 1255: goto tr763;
	case 1256: goto tr763;
	case 1257: goto tr763;
	case 1258: goto tr763;
	case 1259: goto tr763;
	case 1260: goto tr763;
	case 598: goto tr14;
	case 1261: goto tr763;
	case 1262: goto tr763;
	case 1263: goto tr1139;
	case 1264: goto tr1139;
	case 1265: goto tr1139;
	case 1266: goto tr1139;
	case 1267: goto tr763;
	case 1268: goto tr763;
	case 1269: goto tr763;
	case 1270: goto tr1139;
	case 1271: goto tr763;
	case 1272: goto tr763;
	case 1273: goto tr763;
	case 1274: goto tr763;
	case 1275: goto tr763;
	case 1276: goto tr763;
	case 1277: goto tr1139;
	case 1278: goto tr1139;
	case 1279: goto tr1139;
	case 1280: goto tr1139;
	case 1281: goto tr1139;
	case 1282: goto tr763;
	case 1283: goto tr763;
	case 1284: goto tr763;
	case 1285: goto tr1139;
	case 1286: goto tr1139;
	case 1287: goto tr1139;
	case 1288: goto tr763;
	case 1289: goto tr763;
	case 1290: goto tr763;
	case 1291: goto tr763;
	case 1292: goto tr763;
	case 1293: goto tr763;
	case 1294: goto tr763;
	case 1295: goto tr763;
	case 1296: goto tr763;
	case 1297: goto tr763;
	case 1298: goto tr763;
	case 1299: goto tr763;
	case 1300: goto tr763;
	case 1301: goto tr1139;
	case 1302: goto tr1139;
	case 1303: goto tr1139;
	case 1304: goto tr763;
	case 1305: goto tr763;
	case 1306: goto tr763;
	case 1307: goto tr763;
	case 1308: goto tr763;
	case 1309: goto tr763;
	case 1310: goto tr763;
	case 1311: goto tr763;
	case 1312: goto tr763;
	case 1313: goto tr763;
	case 1314: goto tr763;
	case 1315: goto tr763;
	case 1316: goto tr763;
	case 1317: goto tr1139;
	case 1318: goto tr1139;
	case 1319: goto tr1139;
	case 1320: goto tr1139;
	case 1321: goto tr763;
	case 1322: goto tr763;
	case 1323: goto tr763;
	case 1324: goto tr763;
	case 1325: goto tr763;
	case 1326: goto tr763;
	case 1327: goto tr763;
	case 1328: goto tr763;
	case 1329: goto tr763;
	case 1330: goto tr763;
	case 1331: goto tr763;
	case 1332: goto tr763;
	case 1333: goto tr763;
	case 599: goto tr14;
	case 1334: goto tr763;
	case 1335: goto tr763;
	case 1336: goto tr1139;
	case 600: goto tr530;
	case 601: goto tr530;
	case 1337: goto tr1139;
	case 1338: goto tr1139;
	case 1339: goto tr763;
	case 1340: goto tr1139;
	case 1341: goto tr763;
	case 1342: goto tr763;
	case 1343: goto tr763;
	case 1344: goto tr763;
	case 1345: goto tr763;
	case 1346: goto tr763;
	case 1347: goto tr763;
	case 1348: goto tr763;
	case 1349: goto tr763;
	case 1350: goto tr763;
	case 602: goto tr14;
	case 1351: goto tr763;
	case 1352: goto tr763;
	case 1353: goto tr763;
	case 1354: goto tr763;
	case 1355: goto tr763;
	case 1356: goto tr763;
	case 603: goto tr14;
	case 1357: goto tr763;
	case 1358: goto tr896;
	case 1359: goto tr763;
	case 604: goto tr14;
	case 1360: goto tr763;
	case 1361: goto tr763;
	case 1362: goto tr763;
	case 1363: goto tr763;
	case 1364: goto tr763;
	case 1365: goto tr763;
	case 1366: goto tr763;
	case 1367: goto tr763;
	case 605: goto tr14;
	case 606: goto tr14;
	case 1368: goto tr763;
	case 1369: goto tr763;
	case 1370: goto tr763;
	case 1371: goto tr763;
	case 1372: goto tr763;
	case 1373: goto tr763;
	case 607: goto tr14;
	case 1374: goto tr763;
	case 1375: goto tr763;
	case 1376: goto tr763;
	case 1377: goto tr763;
	case 1378: goto tr763;
	case 1379: goto tr763;
	case 1380: goto tr763;
	case 1381: goto tr763;
	case 1382: goto tr763;
	case 608: goto tr14;
	case 1383: goto tr763;
	case 1384: goto tr763;
	case 1385: goto tr763;
	case 1386: goto tr763;
	case 1387: goto tr763;
	case 1388: goto tr763;
	case 1389: goto tr763;
	case 1390: goto tr763;
	case 1391: goto tr763;
	case 1392: goto tr763;
	case 1393: goto tr763;
	case 1394: goto tr763;
	case 1395: goto tr763;
	case 1396: goto tr763;
	case 609: goto tr14;
	case 1397: goto tr763;
	case 1398: goto tr763;
	case 1399: goto tr763;
	case 1400: goto tr763;
	case 1401: goto tr763;
	case 1402: goto tr763;
	case 1403: goto tr763;
	case 1404: goto tr763;
	case 1405: goto tr763;
	case 1406: goto tr763;
	case 1407: goto tr763;
	case 1408: goto tr763;
	case 1409: goto tr763;
	case 1410: goto tr763;
	case 1411: goto tr763;
	case 1412: goto tr763;
	case 1413: goto tr763;
	case 1414: goto tr763;
	case 1415: goto tr763;
	case 1416: goto tr763;
	case 1417: goto tr763;
	case 610: goto tr14;
	case 1418: goto tr763;
	case 1419: goto tr763;
	case 1420: goto tr763;
	case 1421: goto tr763;
	case 1422: goto tr763;
	case 1423: goto tr763;
	case 611: goto tr14;
	case 1424: goto tr763;
	case 1425: goto tr763;
	case 1426: goto tr763;
	case 1427: goto tr763;
	case 1428: goto tr763;
	case 612: goto tr14;
	case 1429: goto tr763;
	case 1430: goto tr763;
	case 1431: goto tr763;
	case 1432: goto tr763;
	case 1433: goto tr763;
	case 1434: goto tr763;
	case 1435: goto tr763;
	case 1436: goto tr763;
	case 1437: goto tr763;
	case 1438: goto tr763;
	case 1439: goto tr763;
	case 613: goto tr14;
	case 1440: goto tr763;
	case 1441: goto tr763;
	case 1442: goto tr763;
	case 1443: goto tr763;
	case 1444: goto tr763;
	case 614: goto tr14;
	case 1445: goto tr763;
	case 1446: goto tr763;
	case 615: goto tr14;
	case 1447: goto tr763;
	case 1448: goto tr763;
	case 616: goto tr14;
	case 1449: goto tr763;
	case 1450: goto tr763;
	case 1451: goto tr763;
	case 1452: goto tr763;
	case 1453: goto tr763;
	case 1454: goto tr763;
	case 1455: goto tr763;
	case 1456: goto tr763;
	case 1457: goto tr763;
	case 1458: goto tr763;
	case 1459: goto tr763;
	case 1460: goto tr1662;
	case 1461: goto tr763;
	case 1462: goto tr763;
	case 617: goto tr14;
	case 1463: goto tr763;
	case 1464: goto tr763;
	case 1465: goto tr763;
	case 1466: goto tr763;
	case 618: goto tr14;
	case 1467: goto tr763;
	case 619: goto tr14;
	case 620: goto tr14;
	case 621: goto tr14;
	case 622: goto tr14;
	case 1468: goto tr763;
	case 1469: goto tr763;
	case 1470: goto tr763;
	case 1471: goto tr763;
	case 1472: goto tr763;
	case 1473: goto tr763;
	case 1474: goto tr763;
	case 1475: goto tr763;
	case 1476: goto tr763;
	case 1477: goto tr763;
	case 1478: goto tr763;
	case 1479: goto tr763;
	case 1480: goto tr763;
	case 1481: goto tr763;
	case 1482: goto tr763;
	case 1483: goto tr763;
	case 1484: goto tr763;
	case 1485: goto tr763;
	case 1486: goto tr763;
	case 1487: goto tr763;
	case 1488: goto tr763;
	case 1489: goto tr763;
	case 1490: goto tr763;
	case 1491: goto tr763;
	case 1492: goto tr763;
	case 1493: goto tr763;
	case 623: goto tr14;
	case 1494: goto tr763;
	case 1495: goto tr763;
	case 1496: goto tr763;
	case 1497: goto tr763;
	case 1498: goto tr763;
	}
	}

	_out: {}
	}

#line 182 "tokenizer.rl"

  if(s.cs == tokenizer_error)
    return false;

  return true;
}

bool
Tokenizer::tokenize(Stream &dest, const char *data, offset_type len) const {
  State s;

  
#line 48173 "tokenizer.cc"
	{
	 s.cs = tokenizer_start;
	 s.ts = 0;
	 s.te = 0;
	 s.act = 0;
	}

#line 194 "tokenizer.rl"

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

  
#line 48199 "tokenizer.cc"
	{
	 s.cs = tokenizer_start;
	 s.ts = 0;
	 s.te = 0;
	 s.act = 0;
	}

#line 211 "tokenizer.rl"

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
