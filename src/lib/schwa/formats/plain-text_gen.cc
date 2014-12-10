
#line 1 "../ragel/formats/plain-text/plain-text.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 52 "../ragel/formats/plain-text/plain-text.rl"



#include <schwa/formats/plain-text.h>


namespace schwa {
namespace formats {


#line 19 "schwa/formats/plain-text_gen.cc"
static const int plain_text_start = 24;

static const int plain_text_en_main = 24;


#line 62 "../ragel/formats/plain-text/plain-text.rl"

bool
PlainTextLexer::_lex(void) {
  (void)plain_text_en_main;
  
#line 31 "schwa/formats/plain-text_gen.cc"
	{
	 _state.cs = plain_text_start;
	 _state.ts = 0;
	 _state.te = 0;
	 _state.act = 0;
	}

#line 67 "../ragel/formats/plain-text/plain-text.rl"
  
#line 41 "schwa/formats/plain-text_gen.cc"
	{
	if ( ( _state.p) == ( _state.pe) )
		goto _test_eof;
	switch (  _state.cs )
	{
tr0:
#line 1 "NONE"
	{	switch(  _state.act ) {
	case 2:
	{{( _state.p) = (( _state.te))-1;} _create_paragraph(); }
	break;
	default:
	{{( _state.p) = (( _state.te))-1;}}
	break;
	}
	}
	goto st24;
tr2:
#line 49 "../ragel/formats/plain-text/plain-text.rl"
	{{( _state.p) = (( _state.te))-1;}{ _create_paragraph(); }}
	goto st24;
tr6:
#line 50 "../ragel/formats/plain-text/plain-text.rl"
	{{( _state.p) = (( _state.te))-1;}}
	goto st24;
tr16:
#line 50 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.te = ( _state.p)+1;}
	goto st24;
tr22:
#line 48 "../ragel/formats/plain-text/plain-text.rl"
	{{( _state.p) = (( _state.te))-1;}{ _create_paragraph(); }}
	goto st24;
tr28:
#line 50 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.te = ( _state.p);( _state.p)--;}
	goto st24;
tr29:
#line 49 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_paragraph(); }}
	goto st24;
tr34:
#line 48 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_paragraph(); }}
	goto st24;
st24:
#line 1 "NONE"
	{ _state.ts = 0;}
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof24;
case 24:
#line 1 "NONE"
	{ _state.ts = ( _state.p);}
#line 95 "schwa/formats/plain-text_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr20;
		case 13u: goto tr25;
		case 226u: goto st19;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st17;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st23;
	} else
		goto st18;
	goto tr16;
tr20:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 50 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.act = 3;}
	goto st25;
st25:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof25;
case 25:
#line 126 "schwa/formats/plain-text_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr1;
		case 13u: goto st1;
		case 32u: goto st9;
		case 194u: goto st10;
		case 225u: goto st11;
		case 226u: goto st13;
		case 227u: goto st16;
	}
	goto tr28;
tr1:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 49 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.act = 2;}
	goto st26;
st26:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof26;
case 26:
#line 147 "schwa/formats/plain-text_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr1;
		case 13u: goto st1;
		case 32u: goto tr1;
		case 194u: goto st2;
		case 225u: goto st3;
		case 226u: goto st5;
		case 227u: goto st8;
	}
	goto tr29;
st1:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof1;
case 1:
	if ( (*( _state.p)) == 10u )
		goto tr1;
	goto tr0;
st2:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof2;
case 2:
	if ( (*( _state.p)) == 160u )
		goto tr1;
	goto tr2;
st3:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof3;
case 3:
	if ( (*( _state.p)) == 154u )
		goto st4;
	goto tr2;
st4:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof4;
case 4:
	if ( (*( _state.p)) == 128u )
		goto tr1;
	goto tr2;
st5:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof5;
case 5:
	switch( (*( _state.p)) ) {
		case 128u: goto st6;
		case 129u: goto st7;
	}
	goto tr2;
st6:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof6;
case 6:
	switch( (*( _state.p)) ) {
		case 168u: goto tr1;
		case 175u: goto tr1;
	}
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 138u )
		goto tr1;
	goto tr2;
st7:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof7;
case 7:
	if ( (*( _state.p)) == 159u )
		goto tr1;
	goto tr2;
st8:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof8;
case 8:
	if ( (*( _state.p)) == 128u )
		goto st4;
	goto tr2;
st9:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof9;
case 9:
	switch( (*( _state.p)) ) {
		case 10u: goto tr1;
		case 13u: goto st1;
		case 32u: goto st9;
		case 194u: goto st10;
		case 225u: goto st11;
		case 226u: goto st13;
		case 227u: goto st16;
	}
	goto tr6;
st10:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof10;
case 10:
	if ( (*( _state.p)) == 160u )
		goto st9;
	goto tr6;
st11:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof11;
case 11:
	if ( (*( _state.p)) == 154u )
		goto st12;
	goto tr6;
st12:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof12;
case 12:
	if ( (*( _state.p)) == 128u )
		goto st9;
	goto tr6;
st13:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof13;
case 13:
	switch( (*( _state.p)) ) {
		case 128u: goto st14;
		case 129u: goto st15;
	}
	goto tr6;
st14:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof14;
case 14:
	switch( (*( _state.p)) ) {
		case 168u: goto tr1;
		case 175u: goto st9;
	}
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 138u )
		goto st9;
	goto tr6;
st15:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof15;
case 15:
	if ( (*( _state.p)) == 159u )
		goto st9;
	goto tr6;
st16:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof16;
case 16:
	if ( (*( _state.p)) == 128u )
		goto st12;
	goto tr6;
tr25:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 50 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.act = 3;}
	goto st27;
st27:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof27;
case 27:
#line 299 "schwa/formats/plain-text_gen.cc"
	if ( (*( _state.p)) == 10u )
		goto st9;
	goto tr28;
st0:
 _state.cs = 0;
	goto _out;
st17:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof17;
case 17:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto tr16;
	goto st0;
st18:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof18;
case 18:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st17;
	goto st0;
st19:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof19;
case 19:
	if ( (*( _state.p)) == 128u )
		goto st20;
	if ( 129u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st17;
	goto st0;
st20:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof20;
case 20:
	switch( (*( _state.p)) ) {
		case 168u: goto tr20;
		case 169u: goto tr21;
	}
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto tr16;
	goto st0;
tr21:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
	goto st28;
st28:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof28;
case 28:
#line 348 "schwa/formats/plain-text_gen.cc"
	if ( (*( _state.p)) == 226u )
		goto st21;
	goto tr34;
st21:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof21;
case 21:
	if ( (*( _state.p)) == 128u )
		goto st22;
	goto tr22;
st22:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof22;
case 22:
	if ( (*( _state.p)) == 169u )
		goto tr21;
	goto tr22;
st23:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof23;
case 23:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st18;
	goto st0;
	}
	_test_eof24:  _state.cs = 24; goto _test_eof; 
	_test_eof25:  _state.cs = 25; goto _test_eof; 
	_test_eof26:  _state.cs = 26; goto _test_eof; 
	_test_eof1:  _state.cs = 1; goto _test_eof; 
	_test_eof2:  _state.cs = 2; goto _test_eof; 
	_test_eof3:  _state.cs = 3; goto _test_eof; 
	_test_eof4:  _state.cs = 4; goto _test_eof; 
	_test_eof5:  _state.cs = 5; goto _test_eof; 
	_test_eof6:  _state.cs = 6; goto _test_eof; 
	_test_eof7:  _state.cs = 7; goto _test_eof; 
	_test_eof8:  _state.cs = 8; goto _test_eof; 
	_test_eof9:  _state.cs = 9; goto _test_eof; 
	_test_eof10:  _state.cs = 10; goto _test_eof; 
	_test_eof11:  _state.cs = 11; goto _test_eof; 
	_test_eof12:  _state.cs = 12; goto _test_eof; 
	_test_eof13:  _state.cs = 13; goto _test_eof; 
	_test_eof14:  _state.cs = 14; goto _test_eof; 
	_test_eof15:  _state.cs = 15; goto _test_eof; 
	_test_eof16:  _state.cs = 16; goto _test_eof; 
	_test_eof27:  _state.cs = 27; goto _test_eof; 
	_test_eof17:  _state.cs = 17; goto _test_eof; 
	_test_eof18:  _state.cs = 18; goto _test_eof; 
	_test_eof19:  _state.cs = 19; goto _test_eof; 
	_test_eof20:  _state.cs = 20; goto _test_eof; 
	_test_eof28:  _state.cs = 28; goto _test_eof; 
	_test_eof21:  _state.cs = 21; goto _test_eof; 
	_test_eof22:  _state.cs = 22; goto _test_eof; 
	_test_eof23:  _state.cs = 23; goto _test_eof; 

	_test_eof: {}
	if ( ( _state.p) == ( _state.eof) )
	{
	switch (  _state.cs ) {
	case 25: goto tr28;
	case 26: goto tr29;
	case 1: goto tr0;
	case 2: goto tr2;
	case 3: goto tr2;
	case 4: goto tr2;
	case 5: goto tr2;
	case 6: goto tr2;
	case 7: goto tr2;
	case 8: goto tr2;
	case 9: goto tr6;
	case 10: goto tr6;
	case 11: goto tr6;
	case 12: goto tr6;
	case 13: goto tr6;
	case 14: goto tr6;
	case 15: goto tr6;
	case 16: goto tr6;
	case 27: goto tr28;
	case 28: goto tr34;
	case 21: goto tr22;
	case 22: goto tr22;
	}
	}

	_out: {}
	}

#line 68 "../ragel/formats/plain-text/plain-text.rl"
  return _state.cs != 
#line 437 "schwa/formats/plain-text_gen.cc"
0
#line 68 "../ragel/formats/plain-text/plain-text.rl"
;
}

}  // namespace formats
}  // namespace schwa
