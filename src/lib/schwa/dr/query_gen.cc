
#line 1 "../ragel/dr-query/language.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#define PUSH_TOKEN(type) _push_token(TokenType::type, ts, te)


#line 65 "../ragel/dr-query/language.rl"



#include <schwa/dr/query.h>

namespace schwa {
namespace dr {
namespace query {


#line 20 "schwa/dr/query_gen.cc"
static const int language_start = 11;
static const int language_first_final = 11;
static const int language_error = 0;

static const int language_en_main = 11;


#line 75 "../ragel/dr-query/language.rl"

void
Interpreter::_tokenise(const char *const str, const size_t len) {
  int cs;
  int act;
  const char *ts, *te;
  const char *p = str;
  const char *pe = p + len;
  const char *eof = pe;

  // Initialise and run the FSA.
  
#line 41 "schwa/dr/query_gen.cc"
	{
	cs = language_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 87 "../ragel/dr-query/language.rl"
  
#line 51 "schwa/dr/query_gen.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr0:
#line 51 "../ragel/dr-query/language.rl"
	{te = p+1;{ PUSH_TOKEN(OP_COMPARISON); }}
	goto st11;
tr2:
#line 1 "NONE"
	{	switch( act ) {
	case 0:
	{{goto st0;}}
	break;
	case 3:
	{{p = ((te))-1;} _push_token(TokenType::LITERAL_STRING, ts + 1, te - 1); }
	break;
	case 8:
	{{p = ((te))-1;} PUSH_TOKEN(FUNCTION); }
	break;
	case 9:
	{{p = ((te))-1;} PUSH_TOKEN(VAR); }
	break;
	}
	}
	goto st11;
tr4:
#line 48 "../ragel/dr-query/language.rl"
	{te = p+1;{ _push_token(TokenType::LITERAL_STRING, ts + 1, te - 1); }}
	goto st11;
tr7:
#line 50 "../ragel/dr-query/language.rl"
	{te = p+1;{ PUSH_TOKEN(OP_BOOLEAN); }}
	goto st11;
tr9:
#line 53 "../ragel/dr-query/language.rl"
	{{p = ((te))-1;}{ PUSH_TOKEN(OP_NUMERIC4); }}
	goto st11;
tr11:
#line 47 "../ragel/dr-query/language.rl"
	{te = p+1;{ _push_token(TokenType::LITERAL_REGEX, ts + 1, te - 1); }}
	goto st11;
tr17:
#line 52 "../ragel/dr-query/language.rl"
	{te = p+1;{ PUSH_TOKEN(OP_NUMERIC3); }}
	goto st11;
tr19:
#line 59 "../ragel/dr-query/language.rl"
	{te = p+1;{ PUSH_TOKEN(OPEN_PAREN); }}
	goto st11;
tr20:
#line 60 "../ragel/dr-query/language.rl"
	{te = p+1;{ PUSH_TOKEN(CLOSE_PAREN); }}
	goto st11;
tr21:
#line 53 "../ragel/dr-query/language.rl"
	{te = p+1;{ PUSH_TOKEN(OP_NUMERIC4); }}
	goto st11;
tr23:
#line 61 "../ragel/dr-query/language.rl"
	{te = p+1;{ PUSH_TOKEN(COMMA); }}
	goto st11;
tr26:
#line 46 "../ragel/dr-query/language.rl"
	{te = p+1;{ PUSH_TOKEN(LITERAL_INTEGER); }}
	goto st11;
tr35:
#line 63 "../ragel/dr-query/language.rl"
	{te = p;p--;}
	goto st11;
tr36:
#line 48 "../ragel/dr-query/language.rl"
	{te = p;p--;{ _push_token(TokenType::LITERAL_STRING, ts + 1, te - 1); }}
	goto st11;
tr37:
#line 52 "../ragel/dr-query/language.rl"
	{te = p;p--;{ PUSH_TOKEN(OP_NUMERIC3); }}
	goto st11;
tr38:
#line 46 "../ragel/dr-query/language.rl"
	{te = p;p--;{ PUSH_TOKEN(LITERAL_INTEGER); }}
	goto st11;
tr39:
#line 57 "../ragel/dr-query/language.rl"
	{te = p;p--;{ _push_token(TokenType::VAR_ATTRIBUTE, ts + 1, te); }}
	goto st11;
tr40:
#line 53 "../ragel/dr-query/language.rl"
	{te = p;p--;{ PUSH_TOKEN(OP_NUMERIC4); }}
	goto st11;
tr41:
#line 51 "../ragel/dr-query/language.rl"
	{te = p;p--;{ PUSH_TOKEN(OP_COMPARISON); }}
	goto st11;
tr42:
#line 56 "../ragel/dr-query/language.rl"
	{te = p;p--;{ PUSH_TOKEN(VAR); }}
	goto st11;
st11:
#line 1 "NONE"
	{ts = 0;}
#line 1 "NONE"
	{act = 0;}
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 1 "NONE"
	{ts = p;}
#line 161 "schwa/dr/query_gen.cc"
	switch( (*p) ) {
		case 32: goto st12;
		case 33: goto st1;
		case 34: goto st2;
		case 37: goto tr17;
		case 38: goto st4;
		case 40: goto tr19;
		case 41: goto tr20;
		case 42: goto tr21;
		case 44: goto tr23;
		case 46: goto st5;
		case 47: goto tr25;
		case 48: goto tr26;
		case 61: goto st1;
		case 95: goto tr29;
		case 97: goto st20;
		case 105: goto st23;
		case 108: goto st25;
		case 115: goto st27;
		case 124: goto st10;
		case 126: goto st18;
	}
	if ( (*p) < 49 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto st14;
		} else if ( (*p) >= 9 )
			goto st12;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 60 <= (*p) && (*p) <= 62 )
				goto st18;
		} else if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr29;
		} else
			goto tr29;
	} else
		goto st15;
	goto st0;
st0:
cs = 0;
	goto _out;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 32 )
		goto st12;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st12;
	goto tr35;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	if ( (*p) == 61 )
		goto tr0;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 34: goto tr4;
		case 92: goto st3;
	}
	goto st2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 34: goto tr6;
		case 92: goto st3;
	}
	goto st2;
tr6:
#line 1 "NONE"
	{te = p+1;}
#line 48 "../ragel/dr-query/language.rl"
	{act = 3;}
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 249 "schwa/dr/query_gen.cc"
	switch( (*p) ) {
		case 34: goto tr4;
		case 92: goto st3;
	}
	goto st2;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 38 )
		goto tr7;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 48 )
		goto tr26;
	if ( 49 <= (*p) && (*p) <= 57 )
		goto st15;
	goto tr37;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st15;
	goto tr38;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 95 )
		goto st16;
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st16;
	} else if ( (*p) >= 65 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 95 )
		goto st16;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st16;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st16;
	} else
		goto st16;
	goto tr39;
tr25:
#line 1 "NONE"
	{te = p+1;}
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 313 "schwa/dr/query_gen.cc"
	switch( (*p) ) {
		case 10: goto tr40;
		case 42: goto tr40;
		case 47: goto tr40;
		case 91: goto st7;
		case 92: goto st9;
	}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 10: goto tr9;
		case 47: goto tr11;
		case 91: goto st7;
		case 92: goto st9;
	}
	goto st6;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 10: goto tr9;
		case 92: goto st8;
		case 93: goto st6;
	}
	goto st7;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 10 )
		goto tr9;
	goto st7;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 10 )
		goto tr9;
	goto st6;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 61 )
		goto tr0;
	goto tr41;
tr29:
#line 1 "NONE"
	{te = p+1;}
#line 56 "../ragel/dr-query/language.rl"
	{act = 9;}
	goto st19;
tr45:
#line 1 "NONE"
	{te = p+1;}
#line 55 "../ragel/dr-query/language.rl"
	{act = 8;}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 380 "schwa/dr/query_gen.cc"
	if ( (*p) == 95 )
		goto tr29;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr2;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 95: goto tr29;
		case 108: goto st21;
		case 110: goto st22;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr42;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 95: goto tr29;
		case 108: goto tr45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr42;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 95: goto tr29;
		case 121: goto tr45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr42;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 95: goto tr29;
		case 110: goto st24;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr42;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 95: goto tr29;
		case 116: goto tr45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr42;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 95: goto tr29;
		case 101: goto st26;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr42;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 95: goto tr29;
		case 110: goto tr45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr42;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 95: goto tr29;
		case 116: goto st28;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr42;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 95: goto tr29;
		case 114: goto tr45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr29;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr42;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 124 )
		goto tr7;
	goto st0;
	}
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 12: goto tr35;
	case 2: goto tr2;
	case 3: goto tr2;
	case 13: goto tr36;
	case 14: goto tr37;
	case 15: goto tr38;
	case 16: goto tr39;
	case 17: goto tr40;
	case 6: goto tr9;
	case 7: goto tr9;
	case 8: goto tr9;
	case 9: goto tr9;
	case 18: goto tr41;
	case 19: goto tr2;
	case 20: goto tr42;
	case 21: goto tr42;
	case 22: goto tr42;
	case 23: goto tr42;
	case 24: goto tr42;
	case 25: goto tr42;
	case 26: goto tr42;
	case 27: goto tr42;
	case 28: goto tr42;
	}
	}

	_out: {}
	}

#line 88 "../ragel/dr-query/language.rl"

  // Did the FSA terminate on an accepting state?
  if (cs < language_first_final)
    _throw_compile_error(p, pe);
}

}  // namespace query
}  // namesapce dr
}  // namespace schwa
