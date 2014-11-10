
#line 1 "../ragel/corpora/tipster/tipster.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 53 "../ragel/corpora/tipster/tipster.rl"



#include <schwa/corpora/tipster.h>


namespace schwa {
namespace corpora {


#line 19 "schwa/corpora/tipster_gen.cc"
static const int tipster_start = 46;

static const int tipster_en_main = 46;


#line 63 "../ragel/corpora/tipster/tipster.rl"

bool
TipsterTextLexer::_lex(void) {
  (void)tipster_en_main;
  
#line 31 "schwa/corpora/tipster_gen.cc"
	{
	 _state.cs = tipster_start;
	 _state.ts = 0;
	 _state.te = 0;
	 _state.act = 0;
	}

#line 68 "../ragel/corpora/tipster/tipster.rl"
  
#line 41 "schwa/corpora/tipster_gen.cc"
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
	case 3:
	{{( _state.p) = (( _state.te))-1;} _create_paragraph(); }
	break;
	default:
	{{( _state.p) = (( _state.te))-1;}}
	break;
	}
	}
	goto st46;
tr11:
#line 49 "../ragel/corpora/tipster/tipster.rl"
	{{( _state.p) = (( _state.te))-1;}{ _create_paragraph(); }}
	goto st46;
tr16:
#line 51 "../ragel/corpora/tipster/tipster.rl"
	{{( _state.p) = (( _state.te))-1;}}
	goto st46;
tr34:
#line 51 "../ragel/corpora/tipster/tipster.rl"
	{ _state.te = ( _state.p)+1;}
	goto st46;
tr43:
#line 48 "../ragel/corpora/tipster/tipster.rl"
	{{( _state.p) = (( _state.te))-1;}{ _create_paragraph(); }}
	goto st46;
tr51:
#line 51 "../ragel/corpora/tipster/tipster.rl"
	{ _state.te = ( _state.p);( _state.p)--;}
	goto st46;
tr52:
#line 49 "../ragel/corpora/tipster/tipster.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_paragraph(); }}
	goto st46;
tr57:
#line 50 "../ragel/corpora/tipster/tipster.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_paragraph(); }}
	goto st46;
tr58:
#line 48 "../ragel/corpora/tipster/tipster.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_paragraph(); }}
	goto st46;
st46:
#line 1 "NONE"
	{ _state.ts = 0;}
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof46;
case 46:
#line 1 "NONE"
	{ _state.ts = ( _state.p);}
#line 102 "schwa/corpora/tipster_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr41;
		case 13u: goto tr41;
		case 32u: goto tr36;
		case 194u: goto st35;
		case 225u: goto st37;
		case 226u: goto st39;
		case 227u: goto st44;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st34;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st45;
	} else
		goto st36;
	goto tr34;
tr41:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 51 "../ragel/corpora/tipster/tipster.rl"
	{ _state.act = 4;}
	goto st47;
st47:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof47;
case 47:
#line 137 "schwa/corpora/tipster_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr12;
		case 13u: goto tr12;
		case 32u: goto st18;
		case 45u: goto st1;
		case 194u: goto st19;
		case 225u: goto st20;
		case 226u: goto st22;
		case 227u: goto st25;
	}
	goto tr51;
tr12:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 49 "../ragel/corpora/tipster/tipster.rl"
	{ _state.act = 2;}
	goto st48;
st48:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof48;
case 48:
#line 159 "schwa/corpora/tipster_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr12;
		case 13u: goto tr12;
		case 32u: goto tr12;
		case 45u: goto st1;
		case 194u: goto st11;
		case 225u: goto st12;
		case 226u: goto st14;
		case 227u: goto st17;
	}
	goto tr52;
st1:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof1;
case 1:
	if ( (*( _state.p)) == 45u )
		goto st2;
	goto tr0;
st2:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof2;
case 2:
	if ( (*( _state.p)) == 45u )
		goto st3;
	goto tr0;
st3:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof3;
case 3:
	switch( (*( _state.p)) ) {
		case 10u: goto tr3;
		case 13u: goto tr3;
		case 32u: goto tr3;
		case 194u: goto st4;
		case 225u: goto st5;
		case 226u: goto st7;
		case 227u: goto st10;
	}
	goto tr0;
tr3:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 50 "../ragel/corpora/tipster/tipster.rl"
	{ _state.act = 3;}
	goto st49;
st49:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof49;
case 49:
#line 209 "schwa/corpora/tipster_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr3;
		case 13u: goto tr3;
		case 32u: goto tr3;
		case 194u: goto st4;
		case 225u: goto st5;
		case 226u: goto st7;
		case 227u: goto st10;
	}
	goto tr57;
st4:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof4;
case 4:
	if ( (*( _state.p)) == 160u )
		goto tr3;
	goto tr0;
st5:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof5;
case 5:
	if ( (*( _state.p)) == 154u )
		goto st6;
	goto tr0;
st6:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof6;
case 6:
	if ( (*( _state.p)) == 128u )
		goto tr3;
	goto tr0;
st7:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof7;
case 7:
	switch( (*( _state.p)) ) {
		case 128u: goto st8;
		case 129u: goto st9;
	}
	goto tr0;
st8:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof8;
case 8:
	switch( (*( _state.p)) ) {
		case 168u: goto tr3;
		case 175u: goto tr3;
	}
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 138u )
		goto tr3;
	goto tr0;
st9:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof9;
case 9:
	if ( (*( _state.p)) == 159u )
		goto tr3;
	goto tr0;
st10:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof10;
case 10:
	if ( (*( _state.p)) == 128u )
		goto st6;
	goto tr0;
st11:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof11;
case 11:
	if ( (*( _state.p)) == 160u )
		goto tr12;
	goto tr11;
st12:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof12;
case 12:
	if ( (*( _state.p)) == 154u )
		goto st13;
	goto tr11;
st13:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof13;
case 13:
	if ( (*( _state.p)) == 128u )
		goto tr12;
	goto tr11;
st14:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof14;
case 14:
	switch( (*( _state.p)) ) {
		case 128u: goto st15;
		case 129u: goto st16;
	}
	goto tr11;
st15:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof15;
case 15:
	switch( (*( _state.p)) ) {
		case 168u: goto tr12;
		case 175u: goto tr12;
	}
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 138u )
		goto tr12;
	goto tr11;
st16:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof16;
case 16:
	if ( (*( _state.p)) == 159u )
		goto tr12;
	goto tr11;
st17:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof17;
case 17:
	if ( (*( _state.p)) == 128u )
		goto st13;
	goto tr11;
st18:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof18;
case 18:
	switch( (*( _state.p)) ) {
		case 10u: goto tr12;
		case 13u: goto tr12;
		case 32u: goto st18;
		case 45u: goto st1;
		case 194u: goto st19;
		case 225u: goto st20;
		case 226u: goto st22;
		case 227u: goto st25;
	}
	goto tr16;
st19:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof19;
case 19:
	if ( (*( _state.p)) == 160u )
		goto st18;
	goto tr16;
st20:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof20;
case 20:
	if ( (*( _state.p)) == 154u )
		goto st21;
	goto tr16;
st21:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof21;
case 21:
	if ( (*( _state.p)) == 128u )
		goto st18;
	goto tr16;
st22:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof22;
case 22:
	switch( (*( _state.p)) ) {
		case 128u: goto st23;
		case 129u: goto st24;
	}
	goto tr16;
st23:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof23;
case 23:
	switch( (*( _state.p)) ) {
		case 168u: goto tr12;
		case 175u: goto st18;
	}
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 138u )
		goto st18;
	goto tr16;
st24:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof24;
case 24:
	if ( (*( _state.p)) == 159u )
		goto st18;
	goto tr16;
st25:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof25;
case 25:
	if ( (*( _state.p)) == 128u )
		goto st21;
	goto tr16;
tr36:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 51 "../ragel/corpora/tipster/tipster.rl"
	{ _state.act = 4;}
	goto st50;
st50:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof50;
case 50:
#line 410 "schwa/corpora/tipster_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto st26;
		case 13u: goto st26;
		case 32u: goto st26;
		case 45u: goto st1;
		case 194u: goto st27;
		case 225u: goto st28;
		case 226u: goto st30;
		case 227u: goto st33;
	}
	goto tr51;
st26:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof26;
case 26:
	switch( (*( _state.p)) ) {
		case 10u: goto st26;
		case 13u: goto st26;
		case 32u: goto st26;
		case 45u: goto st1;
		case 194u: goto st27;
		case 225u: goto st28;
		case 226u: goto st30;
		case 227u: goto st33;
	}
	goto tr16;
st27:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof27;
case 27:
	if ( (*( _state.p)) == 160u )
		goto st26;
	goto tr16;
st28:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof28;
case 28:
	if ( (*( _state.p)) == 154u )
		goto st29;
	goto tr16;
st29:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof29;
case 29:
	if ( (*( _state.p)) == 128u )
		goto st26;
	goto tr16;
st30:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof30;
case 30:
	switch( (*( _state.p)) ) {
		case 128u: goto st31;
		case 129u: goto st32;
	}
	goto tr16;
st31:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof31;
case 31:
	switch( (*( _state.p)) ) {
		case 168u: goto st26;
		case 175u: goto st26;
	}
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 138u )
		goto st26;
	goto tr16;
st32:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof32;
case 32:
	if ( (*( _state.p)) == 159u )
		goto st26;
	goto tr16;
st33:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof33;
case 33:
	if ( (*( _state.p)) == 128u )
		goto st29;
	goto tr16;
st0:
 _state.cs = 0;
	goto _out;
st34:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof34;
case 34:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto tr34;
	goto st0;
st35:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof35;
case 35:
	if ( (*( _state.p)) == 160u )
		goto tr36;
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto tr34;
	goto st0;
st36:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof36;
case 36:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st34;
	goto st0;
st37:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof37;
case 37:
	if ( (*( _state.p)) == 154u )
		goto st38;
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st34;
	goto st0;
st38:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof38;
case 38:
	if ( (*( _state.p)) == 128u )
		goto tr36;
	if ( 129u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto tr34;
	goto st0;
st39:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof39;
case 39:
	switch( (*( _state.p)) ) {
		case 128u: goto st40;
		case 129u: goto st43;
	}
	if ( 130u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st34;
	goto st0;
st40:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof40;
case 40:
	switch( (*( _state.p)) ) {
		case 168u: goto tr41;
		case 169u: goto tr42;
		case 175u: goto tr36;
	}
	if ( (*( _state.p)) > 138u ) {
		if ( 139u <= (*( _state.p)) && (*( _state.p)) <= 191u )
			goto tr34;
	} else if ( (*( _state.p)) >= 128u )
		goto tr36;
	goto st0;
tr42:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
	goto st51;
st51:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof51;
case 51:
#line 570 "schwa/corpora/tipster_gen.cc"
	if ( (*( _state.p)) == 226u )
		goto st41;
	goto tr58;
st41:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof41;
case 41:
	if ( (*( _state.p)) == 128u )
		goto st42;
	goto tr43;
st42:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof42;
case 42:
	if ( (*( _state.p)) == 169u )
		goto tr42;
	goto tr43;
st43:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof43;
case 43:
	if ( (*( _state.p)) == 159u )
		goto tr36;
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto tr34;
	goto st0;
st44:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof44;
case 44:
	if ( (*( _state.p)) == 128u )
		goto st38;
	if ( 129u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st34;
	goto st0;
st45:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof45;
case 45:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st36;
	goto st0;
	}
	_test_eof46:  _state.cs = 46; goto _test_eof; 
	_test_eof47:  _state.cs = 47; goto _test_eof; 
	_test_eof48:  _state.cs = 48; goto _test_eof; 
	_test_eof1:  _state.cs = 1; goto _test_eof; 
	_test_eof2:  _state.cs = 2; goto _test_eof; 
	_test_eof3:  _state.cs = 3; goto _test_eof; 
	_test_eof49:  _state.cs = 49; goto _test_eof; 
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
	_test_eof20:  _state.cs = 20; goto _test_eof; 
	_test_eof21:  _state.cs = 21; goto _test_eof; 
	_test_eof22:  _state.cs = 22; goto _test_eof; 
	_test_eof23:  _state.cs = 23; goto _test_eof; 
	_test_eof24:  _state.cs = 24; goto _test_eof; 
	_test_eof25:  _state.cs = 25; goto _test_eof; 
	_test_eof50:  _state.cs = 50; goto _test_eof; 
	_test_eof26:  _state.cs = 26; goto _test_eof; 
	_test_eof27:  _state.cs = 27; goto _test_eof; 
	_test_eof28:  _state.cs = 28; goto _test_eof; 
	_test_eof29:  _state.cs = 29; goto _test_eof; 
	_test_eof30:  _state.cs = 30; goto _test_eof; 
	_test_eof31:  _state.cs = 31; goto _test_eof; 
	_test_eof32:  _state.cs = 32; goto _test_eof; 
	_test_eof33:  _state.cs = 33; goto _test_eof; 
	_test_eof34:  _state.cs = 34; goto _test_eof; 
	_test_eof35:  _state.cs = 35; goto _test_eof; 
	_test_eof36:  _state.cs = 36; goto _test_eof; 
	_test_eof37:  _state.cs = 37; goto _test_eof; 
	_test_eof38:  _state.cs = 38; goto _test_eof; 
	_test_eof39:  _state.cs = 39; goto _test_eof; 
	_test_eof40:  _state.cs = 40; goto _test_eof; 
	_test_eof51:  _state.cs = 51; goto _test_eof; 
	_test_eof41:  _state.cs = 41; goto _test_eof; 
	_test_eof42:  _state.cs = 42; goto _test_eof; 
	_test_eof43:  _state.cs = 43; goto _test_eof; 
	_test_eof44:  _state.cs = 44; goto _test_eof; 
	_test_eof45:  _state.cs = 45; goto _test_eof; 

	_test_eof: {}
	if ( ( _state.p) == ( _state.eof) )
	{
	switch (  _state.cs ) {
	case 47: goto tr51;
	case 48: goto tr52;
	case 1: goto tr0;
	case 2: goto tr0;
	case 3: goto tr0;
	case 49: goto tr57;
	case 4: goto tr0;
	case 5: goto tr0;
	case 6: goto tr0;
	case 7: goto tr0;
	case 8: goto tr0;
	case 9: goto tr0;
	case 10: goto tr0;
	case 11: goto tr11;
	case 12: goto tr11;
	case 13: goto tr11;
	case 14: goto tr11;
	case 15: goto tr11;
	case 16: goto tr11;
	case 17: goto tr11;
	case 18: goto tr16;
	case 19: goto tr16;
	case 20: goto tr16;
	case 21: goto tr16;
	case 22: goto tr16;
	case 23: goto tr16;
	case 24: goto tr16;
	case 25: goto tr16;
	case 50: goto tr51;
	case 26: goto tr16;
	case 27: goto tr16;
	case 28: goto tr16;
	case 29: goto tr16;
	case 30: goto tr16;
	case 31: goto tr16;
	case 32: goto tr16;
	case 33: goto tr16;
	case 51: goto tr58;
	case 41: goto tr43;
	case 42: goto tr43;
	}
	}

	_out: {}
	}

#line 69 "../ragel/corpora/tipster/tipster.rl"
  return _state.cs != 0;
}

}  // namespace corpora
}  // namespace schwa
