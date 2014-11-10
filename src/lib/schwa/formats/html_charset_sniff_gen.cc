
#line 1 "../ragel/formats/html/charset_sniff_main.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 14 "../ragel/formats/html/charset_sniff_main.rl"


#include <schwa/formats/html.h>


namespace schwa {
namespace formats {


#line 18 "schwa/formats/html_charset_sniff_gen.cc"
static const int html_charset_sniff_start = 28;

static const int html_charset_sniff_en_main = 28;


#line 23 "../ragel/formats/html/charset_sniff_main.rl"

bool
HTMLCharsetSniffer::_run_charset_sniff(const uint8_t *const input, const size_t nbytes) {
  (void)html_charset_sniff_en_main;  // Shoosh compiler warning about unused variable.
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  const uint8_t *te = nullptr, *ts = nullptr;
  int cs = 0, act = 0;
  (void)te;
  (void)ts;
  (void)act;

  
#line 37 "schwa/formats/html_charset_sniff_gen.cc"
	{
	cs = html_charset_sniff_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 35 "../ragel/formats/html/charset_sniff_main.rl"
  
#line 47 "schwa/formats/html_charset_sniff_gen.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr0:
#line 12 "../ragel/formats/html/charset_sniff_main.rl"
	{{p = ((te))-1;}}
	goto st28;
tr6:
#line 9 "../ragel/formats/html/charset_sniff_main.rl"
	{te = p+1;}
	goto st28;
tr15:
#line 10 "../ragel/formats/html/charset_sniff_main.rl"
	{te = p+1;}
	goto st28;
tr43:
#line 12 "../ragel/formats/html/charset_sniff_main.rl"
	{te = p+1;}
	goto st28;
tr45:
#line 12 "../ragel/formats/html/charset_sniff_main.rl"
	{te = p;p--;}
	goto st28;
tr48:
#line 12 "../ragel/formats/html/charset_sniff.rl"
	{ _meta_end(); }
#line 11 "../ragel/formats/html/charset_sniff_main.rl"
	{te = p;p--;}
	goto st28;
st28:
#line 1 "NONE"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 1 "NONE"
	{ts = p;}
#line 87 "schwa/formats/html_charset_sniff_gen.cc"
	if ( (*p) == 60u )
		goto tr44;
	goto tr43;
tr44:
#line 1 "NONE"
	{te = p+1;}
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 99 "schwa/formats/html_charset_sniff_gen.cc"
	switch( (*p) ) {
		case 33u: goto st0;
		case 77u: goto st14;
		case 109u: goto st14;
	}
	goto tr45;
st0:
	if ( ++p == pe )
		goto _test_eof0;
case 0:
	switch( (*p) ) {
		case 45u: goto st1;
		case 91u: goto st5;
	}
	goto tr0;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	if ( (*p) == 45u )
		goto st2;
	goto tr0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 45u )
		goto st3;
	goto st2;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 45u )
		goto st4;
	goto st2;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 45u: goto st4;
		case 62u: goto tr6;
	}
	goto st2;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 67u )
		goto st6;
	goto tr0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 68u )
		goto st7;
	goto tr0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 65u )
		goto st8;
	goto tr0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 84u )
		goto st9;
	goto tr0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 65u )
		goto st10;
	goto tr0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 91u )
		goto st11;
	goto tr0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 93u )
		goto st12;
	goto st11;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 93u )
		goto st13;
	goto st11;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 62u: goto tr15;
		case 93u: goto st13;
	}
	goto st11;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 69u: goto st15;
		case 101u: goto st15;
	}
	goto tr0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 84u: goto st16;
		case 116u: goto st16;
	}
	goto tr0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 65u: goto st17;
		case 97u: goto st17;
	}
	goto tr0;
tr40:
#line 11 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_value_end(p); }
	goto st17;
tr38:
#line 10 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_value_start(p); }
#line 11 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_value_end(p); }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 251 "schwa/formats/html_charset_sniff_gen.cc"
	switch( (*p) ) {
		case 32u: goto st18;
		case 47u: goto st21;
		case 62u: goto st30;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st18;
	goto tr0;
tr34:
#line 11 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_value_end(p); }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 268 "schwa/formats/html_charset_sniff_gen.cc"
	switch( (*p) ) {
		case 32u: goto st18;
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st21;
		case 61u: goto tr0;
		case 62u: goto st30;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st18;
	goto tr22;
tr22:
#line 8 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_name_start(p); }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 288 "schwa/formats/html_charset_sniff_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr24;
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto tr25;
		case 61u: goto tr26;
		case 62u: goto tr27;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto tr24;
	goto st19;
tr24:
#line 9 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_name_end(p); }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 308 "schwa/formats/html_charset_sniff_gen.cc"
	switch( (*p) ) {
		case 32u: goto st20;
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st21;
		case 61u: goto st22;
		case 62u: goto st30;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st20;
	goto tr22;
tr25:
#line 9 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_name_end(p); }
	goto st21;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 328 "schwa/formats/html_charset_sniff_gen.cc"
	if ( (*p) == 62u )
		goto st30;
	goto tr0;
tr27:
#line 9 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_name_end(p); }
	goto st30;
tr36:
#line 11 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_value_end(p); }
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 344 "schwa/formats/html_charset_sniff_gen.cc"
	goto tr48;
tr26:
#line 9 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_name_end(p); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 354 "schwa/formats/html_charset_sniff_gen.cc"
	switch( (*p) ) {
		case 32u: goto st22;
		case 34u: goto st24;
		case 39u: goto st26;
		case 60u: goto tr0;
		case 62u: goto tr0;
		case 96u: goto tr0;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st22;
	goto tr30;
tr30:
#line 10 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_value_start(p); }
	goto st23;
tr35:
#line 11 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_value_end(p); }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 378 "schwa/formats/html_charset_sniff_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr34;
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto tr35;
		case 60u: goto tr0;
		case 62u: goto tr36;
		case 96u: goto tr0;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto tr34;
	goto st23;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 34u )
		goto tr38;
	goto tr37;
tr37:
#line 10 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_value_start(p); }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 406 "schwa/formats/html_charset_sniff_gen.cc"
	if ( (*p) == 34u )
		goto tr40;
	goto st25;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 39u )
		goto tr38;
	goto tr41;
tr41:
#line 10 "../ragel/formats/html/charset_sniff.rl"
	{ _attribute_value_start(p); }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 425 "schwa/formats/html_charset_sniff_gen.cc"
	if ( (*p) == 39u )
		goto tr40;
	goto st27;
	}
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof0: cs = 0; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
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
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 29: goto tr45;
	case 0: goto tr0;
	case 1: goto tr0;
	case 2: goto tr0;
	case 3: goto tr0;
	case 4: goto tr0;
	case 5: goto tr0;
	case 6: goto tr0;
	case 7: goto tr0;
	case 8: goto tr0;
	case 9: goto tr0;
	case 10: goto tr0;
	case 11: goto tr0;
	case 12: goto tr0;
	case 13: goto tr0;
	case 14: goto tr0;
	case 15: goto tr0;
	case 16: goto tr0;
	case 17: goto tr0;
	case 18: goto tr0;
	case 19: goto tr0;
	case 20: goto tr0;
	case 21: goto tr0;
	case 30: goto tr48;
	case 22: goto tr0;
	case 23: goto tr0;
	case 24: goto tr0;
	case 25: goto tr0;
	case 26: goto tr0;
	case 27: goto tr0;
	}
	}

	}

#line 36 "../ragel/formats/html/charset_sniff_main.rl"

  return cs != -1;
}

}  // namespace formats
}  // namespace schwa
