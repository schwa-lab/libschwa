
#line 1 "../ragel/formats/plain-text/plain-text.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 52 "../ragel/formats/plain-text/plain-text.rl"



#include <schwa/formats/plain-text.h>


namespace schwa {
namespace formats {


#line 19 "schwa/formats/plain-text_gen.cc"
static const int plain_text_start = 23;

static const int plain_text_en_main = 23;


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
#line 49 "../ragel/formats/plain-text/plain-text.rl"
	{{( _state.p) = (( _state.te))-1;}{ _create_paragraph(); }}
	goto st23;
tr5:
#line 50 "../ragel/formats/plain-text/plain-text.rl"
	{{( _state.p) = (( _state.te))-1;}}
	goto st23;
tr14:
#line 50 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.te = ( _state.p)+1;}
	goto st23;
tr20:
#line 48 "../ragel/formats/plain-text/plain-text.rl"
	{{( _state.p) = (( _state.te))-1;}{ _create_paragraph(); }}
	goto st23;
tr25:
#line 50 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.te = ( _state.p);( _state.p)--;}
	goto st23;
tr26:
#line 49 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_paragraph(); }}
	goto st23;
tr31:
#line 48 "../ragel/formats/plain-text/plain-text.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_paragraph(); }}
	goto st23;
st23:
#line 1 "NONE"
	{ _state.ts = 0;}
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof23;
case 23:
#line 1 "NONE"
	{ _state.ts = ( _state.p);}
#line 83 "schwa/formats/plain-text_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr18;
		case 13u: goto tr18;
		case 226u: goto st18;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st16;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st22;
	} else
		goto st17;
	goto tr14;
tr18:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
	goto st24;
st24:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof24;
case 24:
#line 112 "schwa/formats/plain-text_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr1;
		case 13u: goto tr1;
		case 32u: goto st8;
		case 194u: goto st9;
		case 225u: goto st10;
		case 226u: goto st12;
		case 227u: goto st15;
	}
	goto tr25;
tr1:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
	goto st25;
st25:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof25;
case 25:
#line 131 "schwa/formats/plain-text_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr1;
		case 13u: goto tr1;
		case 32u: goto tr1;
		case 194u: goto st1;
		case 225u: goto st2;
		case 226u: goto st4;
		case 227u: goto st7;
	}
	goto tr26;
st1:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof1;
case 1:
	if ( (*( _state.p)) == 160u )
		goto tr1;
	goto tr0;
st2:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof2;
case 2:
	if ( (*( _state.p)) == 154u )
		goto st3;
	goto tr0;
st3:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof3;
case 3:
	if ( (*( _state.p)) == 128u )
		goto tr1;
	goto tr0;
st4:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof4;
case 4:
	switch( (*( _state.p)) ) {
		case 128u: goto st5;
		case 129u: goto st6;
	}
	goto tr0;
st5:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof5;
case 5:
	switch( (*( _state.p)) ) {
		case 168u: goto tr1;
		case 175u: goto tr1;
	}
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 138u )
		goto tr1;
	goto tr0;
st6:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof6;
case 6:
	if ( (*( _state.p)) == 159u )
		goto tr1;
	goto tr0;
st7:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof7;
case 7:
	if ( (*( _state.p)) == 128u )
		goto st3;
	goto tr0;
st8:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof8;
case 8:
	switch( (*( _state.p)) ) {
		case 10u: goto tr1;
		case 13u: goto tr1;
		case 32u: goto st8;
		case 194u: goto st9;
		case 225u: goto st10;
		case 226u: goto st12;
		case 227u: goto st15;
	}
	goto tr5;
st9:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof9;
case 9:
	if ( (*( _state.p)) == 160u )
		goto st8;
	goto tr5;
st10:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof10;
case 10:
	if ( (*( _state.p)) == 154u )
		goto st11;
	goto tr5;
st11:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof11;
case 11:
	if ( (*( _state.p)) == 128u )
		goto st8;
	goto tr5;
st12:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof12;
case 12:
	switch( (*( _state.p)) ) {
		case 128u: goto st13;
		case 129u: goto st14;
	}
	goto tr5;
st13:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof13;
case 13:
	switch( (*( _state.p)) ) {
		case 168u: goto tr1;
		case 175u: goto st8;
	}
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 138u )
		goto st8;
	goto tr5;
st14:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof14;
case 14:
	if ( (*( _state.p)) == 159u )
		goto st8;
	goto tr5;
st15:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof15;
case 15:
	if ( (*( _state.p)) == 128u )
		goto st11;
	goto tr5;
st0:
 _state.cs = 0;
	goto _out;
st16:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof16;
case 16:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto tr14;
	goto st0;
st17:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof17;
case 17:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st16;
	goto st0;
st18:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof18;
case 18:
	if ( (*( _state.p)) == 128u )
		goto st19;
	if ( 129u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st16;
	goto st0;
st19:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof19;
case 19:
	switch( (*( _state.p)) ) {
		case 168u: goto tr18;
		case 169u: goto tr19;
	}
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto tr14;
	goto st0;
tr19:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
	goto st26;
st26:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof26;
case 26:
#line 311 "schwa/formats/plain-text_gen.cc"
	if ( (*( _state.p)) == 226u )
		goto st20;
	goto tr31;
st20:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof20;
case 20:
	if ( (*( _state.p)) == 128u )
		goto st21;
	goto tr20;
st21:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof21;
case 21:
	if ( (*( _state.p)) == 169u )
		goto tr19;
	goto tr20;
st22:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof22;
case 22:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st17;
	goto st0;
	}
	_test_eof23:  _state.cs = 23; goto _test_eof; 
	_test_eof24:  _state.cs = 24; goto _test_eof; 
	_test_eof25:  _state.cs = 25; goto _test_eof; 
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
	_test_eof17:  _state.cs = 17; goto _test_eof; 
	_test_eof18:  _state.cs = 18; goto _test_eof; 
	_test_eof19:  _state.cs = 19; goto _test_eof; 
	_test_eof26:  _state.cs = 26; goto _test_eof; 
	_test_eof20:  _state.cs = 20; goto _test_eof; 
	_test_eof21:  _state.cs = 21; goto _test_eof; 
	_test_eof22:  _state.cs = 22; goto _test_eof; 

	_test_eof: {}
	if ( ( _state.p) == ( _state.eof) )
	{
	switch (  _state.cs ) {
	case 24: goto tr25;
	case 25: goto tr26;
	case 1: goto tr0;
	case 2: goto tr0;
	case 3: goto tr0;
	case 4: goto tr0;
	case 5: goto tr0;
	case 6: goto tr0;
	case 7: goto tr0;
	case 8: goto tr5;
	case 9: goto tr5;
	case 10: goto tr5;
	case 11: goto tr5;
	case 12: goto tr5;
	case 13: goto tr5;
	case 14: goto tr5;
	case 15: goto tr5;
	case 26: goto tr31;
	case 20: goto tr20;
	case 21: goto tr20;
	}
	}

	_out: {}
	}

#line 68 "../ragel/formats/plain-text/plain-text.rl"
  return _state.cs != 0;
}

}  // namespace formats
}  // namespace schwa
