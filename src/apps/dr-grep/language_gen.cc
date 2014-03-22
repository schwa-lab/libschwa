
#line 1 "../../ragel/dr-grep/language.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#define PUSH_TOKEN(type) _push_token(TokenType::type, ts, te)


#line 63 "../../ragel/dr-grep/language.rl"



#include <dr-grep/language.h>

namespace schwa {
namespace dr_grep {


#line 19 "language_gen.cc"
static const int language_start = 23;
static const int language_first_final = 23;
static const int language_error = 0;

static const int language_en_main = 23;


#line 72 "../../ragel/dr-grep/language.rl"

void
Interpreter::_tokenise(const char *const str, const size_t len) {
  int cs;
  int act;
  const char *ts, *te;
  const char *p = str;
  const char *pe = p + len;
  const char *eof = pe;

  // Initialise and run the FSA.
  
#line 40 "language_gen.cc"
	{
	cs = language_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 84 "../../ragel/dr-grep/language.rl"
  
#line 50 "language_gen.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr0:
#line 1 "NONE"
	{	switch( act ) {
	case 0:
	{{goto st0;}}
	break;
	case 3:
	{{p = ((te))-1;} PUSH_TOKEN(LITERAL_STRING); }
	break;
	case 4:
	{{p = ((te))-1;} PUSH_TOKEN(OP_BOOLEAN); }
	break;
	case 5:
	{{p = ((te))-1;} PUSH_TOKEN(OP_COMPARISON); }
	break;
	}
	}
	goto st23;
tr2:
#line 46 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(LITERAL_STRING); }}
	goto st23;
tr5:
#line 48 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(OP_BOOLEAN); }}
	goto st23;
tr7:
#line 51 "../../ragel/dr-grep/language.rl"
	{{p = ((te))-1;}{ PUSH_TOKEN(OP_NUMERIC4); }}
	goto st23;
tr9:
#line 45 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(LITERAL_REGEX); }}
	goto st23;
tr13:
#line 49 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(OP_COMPARISON); }}
	goto st23;
tr16:
#line 54 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(FUNCTION); }}
	goto st23;
tr24:
#line 55 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(INDEX); }}
	goto st23;
tr28:
#line 50 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(OP_NUMERIC3); }}
	goto st23;
tr30:
#line 57 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(OPEN_PAREN); }}
	goto st23;
tr31:
#line 58 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(CLOSE_PAREN); }}
	goto st23;
tr32:
#line 51 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(OP_NUMERIC4); }}
	goto st23;
tr34:
#line 59 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(COMMA); }}
	goto st23;
tr36:
#line 44 "../../ragel/dr-grep/language.rl"
	{te = p+1;{ PUSH_TOKEN(LITERAL_INTEGER); }}
	goto st23;
tr45:
#line 61 "../../ragel/dr-grep/language.rl"
	{te = p;p--;}
	goto st23;
tr46:
#line 46 "../../ragel/dr-grep/language.rl"
	{te = p;p--;{ PUSH_TOKEN(LITERAL_STRING); }}
	goto st23;
tr47:
#line 50 "../../ragel/dr-grep/language.rl"
	{te = p;p--;{ PUSH_TOKEN(OP_NUMERIC3); }}
	goto st23;
tr48:
#line 44 "../../ragel/dr-grep/language.rl"
	{te = p;p--;{ PUSH_TOKEN(LITERAL_INTEGER); }}
	goto st23;
tr49:
#line 51 "../../ragel/dr-grep/language.rl"
	{te = p;p--;{ PUSH_TOKEN(OP_NUMERIC4); }}
	goto st23;
tr50:
#line 53 "../../ragel/dr-grep/language.rl"
	{te = p;p--;{ PUSH_TOKEN(ATTRIBUTE_ACCESS); }}
	goto st23;
st23:
#line 1 "NONE"
	{ts = 0;}
#line 1 "NONE"
	{act = 0;}
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 1 "NONE"
	{ts = p;}
#line 160 "language_gen.cc"
	switch( (*p) ) {
		case 32: goto st24;
		case 33: goto tr27;
		case 34: goto st1;
		case 37: goto tr28;
		case 38: goto st3;
		case 40: goto tr30;
		case 41: goto tr31;
		case 42: goto tr32;
		case 44: goto tr34;
		case 47: goto tr35;
		case 48: goto tr36;
		case 61: goto st8;
		case 97: goto st9;
		case 100: goto st14;
		case 105: goto st16;
		case 115: goto st20;
		case 124: goto st22;
		case 126: goto tr38;
	}
	if ( (*p) < 43 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st24;
	} else if ( (*p) > 45 ) {
		if ( (*p) > 57 ) {
			if ( 60 <= (*p) && (*p) <= 62 )
				goto tr38;
		} else if ( (*p) >= 49 )
			goto st28;
	} else
		goto st27;
	goto st0;
st0:
cs = 0;
	goto _out;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 32 )
		goto st24;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st24;
	goto tr45;
tr27:
#line 1 "NONE"
	{te = p+1;}
#line 48 "../../ragel/dr-grep/language.rl"
	{act = 4;}
	goto st25;
tr38:
#line 1 "NONE"
	{te = p+1;}
#line 49 "../../ragel/dr-grep/language.rl"
	{act = 5;}
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 221 "language_gen.cc"
	if ( (*p) == 61 )
		goto tr13;
	goto tr0;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 34: goto tr2;
		case 92: goto st2;
	}
	goto st1;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 34: goto tr4;
		case 92: goto st2;
	}
	goto st1;
tr4:
#line 1 "NONE"
	{te = p+1;}
#line 46 "../../ragel/dr-grep/language.rl"
	{act = 3;}
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 253 "language_gen.cc"
	switch( (*p) ) {
		case 34: goto tr2;
		case 92: goto st2;
	}
	goto st1;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 38 )
		goto tr5;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 48 )
		goto tr36;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st28;
	goto tr47;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st28;
	goto tr48;
tr35:
#line 1 "NONE"
	{te = p+1;}
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 290 "language_gen.cc"
	switch( (*p) ) {
		case 10: goto tr49;
		case 42: goto tr49;
		case 47: goto tr49;
		case 91: goto st5;
		case 92: goto st7;
	}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 10: goto tr7;
		case 47: goto tr9;
		case 91: goto st5;
		case 92: goto st7;
	}
	goto st4;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 10: goto tr7;
		case 92: goto st6;
		case 93: goto st4;
	}
	goto st5;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 10 )
		goto tr7;
	goto st5;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 10 )
		goto tr7;
	goto st4;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 61 )
		goto tr13;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 108: goto st10;
		case 110: goto st11;
	}
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 108 )
		goto tr16;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 110: goto st12;
		case 121: goto tr16;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 46 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 95 )
		goto st30;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st30;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st30;
	} else
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 95 )
		goto st30;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st30;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st30;
	} else
		goto st30;
	goto tr50;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 111 )
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 99 )
		goto st12;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 110 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 100: goto st18;
		case 116: goto tr16;
	}
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 101 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 120 )
		goto tr24;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 116 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 114 )
		goto tr16;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 124 )
		goto tr5;
	goto st0;
	}
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 24: goto tr45;
	case 25: goto tr0;
	case 1: goto tr0;
	case 2: goto tr0;
	case 26: goto tr46;
	case 27: goto tr47;
	case 28: goto tr48;
	case 29: goto tr49;
	case 4: goto tr7;
	case 5: goto tr7;
	case 6: goto tr7;
	case 7: goto tr7;
	case 30: goto tr50;
	}
	}

	_out: {}
	}

#line 85 "../../ragel/dr-grep/language.rl"

  // Did the FSA terminate on an accepting state?
  if (cs < language_first_final)
    _throw_compile_error(p, pe);
}

}  // namesapce dr_grep
}  // namespace schwa
