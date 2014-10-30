
#line 1 "../ragel/formats/uri/main.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 11 "../ragel/formats/uri/main.rl"


#include <schwa/formats/uri.h>


namespace schwa {
namespace formats {


#line 18 "schwa/formats/uri_gen.cc"
static const int uri_start = 1;

static const int uri_en_main = 1;


#line 20 "../ragel/formats/uri/main.rl"

bool
URILexer::matches(const uint8_t *const input, const size_t nbytes) const {
  URI uri;
  return matches(input, nbytes, uri);
}


bool
URILexer::matches(const uint8_t *const input, const size_t nbytes, URI &uri) const {
  (void)uri_en_main;  // Shoosh compiler warning about unused variable.
  const uint8_t *p = input, *pe = p + nbytes;
  int cs = 0;

  uri.clear();

  
#line 42 "schwa/formats/uri_gen.cc"
	{
	cs = uri_start;
	}

#line 37 "../ragel/formats/uri/main.rl"
  
#line 49 "schwa/formats/uri_gen.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st2;
	} else if ( (*p) >= 65u )
		goto st2;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 43u: goto st2;
		case 58u: goto st169;
	}
	if ( (*p) < 48u ) {
		if ( 45u <= (*p) && (*p) <= 46u )
			goto st2;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st2;
		} else if ( (*p) >= 65u )
			goto st2;
	} else
		goto st2;
	goto st0;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
	switch( (*p) ) {
		case 33u: goto st170;
		case 35u: goto st171;
		case 37u: goto st5;
		case 47u: goto st172;
		case 61u: goto st170;
		case 95u: goto st170;
		case 126u: goto st170;
	}
	if ( (*p) < 63u ) {
		if ( 36u <= (*p) && (*p) <= 59u )
			goto st170;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st170;
	} else
		goto st170;
	goto st0;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
	switch( (*p) ) {
		case 33u: goto st170;
		case 35u: goto st171;
		case 37u: goto st5;
		case 61u: goto st170;
		case 95u: goto st170;
		case 126u: goto st170;
	}
	if ( (*p) < 63u ) {
		if ( 36u <= (*p) && (*p) <= 59u )
			goto st170;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st170;
	} else
		goto st170;
	goto st0;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
	switch( (*p) ) {
		case 33u: goto st171;
		case 37u: goto st3;
		case 61u: goto st171;
		case 95u: goto st171;
		case 126u: goto st171;
	}
	if ( (*p) < 63u ) {
		if ( 36u <= (*p) && (*p) <= 59u )
			goto st171;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st171;
	} else
		goto st171;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st4;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st4;
	} else
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st171;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st171;
	} else
		goto st171;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st6;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st6;
	} else
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st170;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st170;
	} else
		goto st170;
	goto st0;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
	switch( (*p) ) {
		case 33u: goto st170;
		case 35u: goto st171;
		case 37u: goto st5;
		case 47u: goto st173;
		case 61u: goto st170;
		case 95u: goto st170;
		case 126u: goto st170;
	}
	if ( (*p) < 63u ) {
		if ( 36u <= (*p) && (*p) <= 59u )
			goto st170;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st170;
	} else
		goto st170;
	goto st0;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
	switch( (*p) ) {
		case 33u: goto st174;
		case 35u: goto st171;
		case 37u: goto st7;
		case 47u: goto st170;
		case 58u: goto st175;
		case 61u: goto st174;
		case 63u: goto st170;
		case 64u: goto st176;
		case 91u: goto st14;
		case 95u: goto st174;
		case 126u: goto st174;
	}
	if ( (*p) < 65u ) {
		if ( 36u <= (*p) && (*p) <= 59u )
			goto st174;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st174;
	} else
		goto st174;
	goto st0;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
	switch( (*p) ) {
		case 33u: goto st174;
		case 35u: goto st171;
		case 37u: goto st7;
		case 47u: goto st170;
		case 58u: goto st175;
		case 61u: goto st174;
		case 63u: goto st170;
		case 64u: goto st176;
		case 95u: goto st174;
		case 126u: goto st174;
	}
	if ( (*p) < 65u ) {
		if ( 36u <= (*p) && (*p) <= 59u )
			goto st174;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st174;
	} else
		goto st174;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st8;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st8;
	} else
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st174;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st174;
	} else
		goto st174;
	goto st0;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	switch( (*p) ) {
		case 33u: goto st9;
		case 35u: goto st171;
		case 37u: goto st10;
		case 47u: goto st170;
		case 61u: goto st9;
		case 63u: goto st170;
		case 64u: goto st176;
		case 95u: goto st9;
		case 126u: goto st9;
	}
	if ( (*p) < 58u ) {
		if ( (*p) > 46u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto st175;
		} else if ( (*p) >= 36u )
			goto st9;
	} else if ( (*p) > 59u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st9;
		} else if ( (*p) >= 65u )
			goto st9;
	} else
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 33u: goto st9;
		case 37u: goto st10;
		case 61u: goto st9;
		case 64u: goto st176;
		case 95u: goto st9;
		case 126u: goto st9;
	}
	if ( (*p) < 48u ) {
		if ( 36u <= (*p) && (*p) <= 46u )
			goto st9;
	} else if ( (*p) > 59u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st9;
		} else if ( (*p) >= 65u )
			goto st9;
	} else
		goto st9;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st11;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st11;
	} else
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st9;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st9;
	} else
		goto st9;
	goto st0;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
	switch( (*p) ) {
		case 33u: goto st177;
		case 35u: goto st171;
		case 37u: goto st12;
		case 47u: goto st170;
		case 58u: goto st178;
		case 61u: goto st177;
		case 63u: goto st170;
		case 91u: goto st14;
		case 95u: goto st177;
		case 126u: goto st177;
	}
	if ( (*p) < 65u ) {
		if ( 36u <= (*p) && (*p) <= 59u )
			goto st177;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st177;
	} else
		goto st177;
	goto st0;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
	switch( (*p) ) {
		case 33u: goto st177;
		case 35u: goto st171;
		case 37u: goto st12;
		case 47u: goto st170;
		case 58u: goto st178;
		case 61u: goto st177;
		case 63u: goto st170;
		case 95u: goto st177;
		case 126u: goto st177;
	}
	if ( (*p) < 65u ) {
		if ( 36u <= (*p) && (*p) <= 59u )
			goto st177;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto st177;
	} else
		goto st177;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st13;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st13;
	} else
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st177;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st177;
	} else
		goto st177;
	goto st0;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
	switch( (*p) ) {
		case 35u: goto st171;
		case 47u: goto st170;
		case 63u: goto st170;
	}
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st178;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 58u: goto st150;
		case 118u: goto st165;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st15;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st15;
	} else
		goto st15;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 58u )
		goto st19;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st16;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st16;
	} else
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 58u )
		goto st19;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st17;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st17;
	} else
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 58u )
		goto st19;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st18;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st18;
	} else
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 58u )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 58u )
		goto st136;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st20;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st20;
	} else
		goto st20;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 58u )
		goto st24;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st21;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st21;
	} else
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) == 58u )
		goto st24;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st22;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st22;
	} else
		goto st22;
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 58u )
		goto st24;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st23;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st23;
	} else
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 58u )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 58u )
		goto st122;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st25;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st25;
	} else
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 58u )
		goto st29;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st26;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st26;
	} else
		goto st26;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 58u )
		goto st29;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st27;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st27;
	} else
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 58u )
		goto st29;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st28;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st28;
	} else
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 58u )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 58u )
		goto st108;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st30;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st30;
	} else
		goto st30;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 58u )
		goto st34;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st31;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st31;
	} else
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 58u )
		goto st34;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st32;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st32;
	} else
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 58u )
		goto st34;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st33;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st33;
	} else
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	if ( (*p) == 58u )
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 58u )
		goto st94;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st35;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st35;
	} else
		goto st35;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 58u )
		goto st39;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st36;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st36;
	} else
		goto st36;
	goto st0;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 58u )
		goto st39;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st37;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st37;
	} else
		goto st37;
	goto st0;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 58u )
		goto st39;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st38;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st38;
	} else
		goto st38;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 58u )
		goto st39;
	goto st0;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 58u )
		goto st80;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st40;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st40;
	} else
		goto st40;
	goto st0;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	if ( (*p) == 58u )
		goto st44;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st41;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st41;
	} else
		goto st41;
	goto st0;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 58u )
		goto st44;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st42;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st42;
	} else
		goto st42;
	goto st0;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	if ( (*p) == 58u )
		goto st44;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st43;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st43;
	} else
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 58u )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 48u: goto st45;
		case 49u: goto st71;
		case 50u: goto st74;
		case 58u: goto st78;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st77;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st79;
	} else
		goto st79;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st67;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st64;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st64;
	} else
		goto st64;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 48u: goto st47;
		case 49u: goto st60;
		case 50u: goto st62;
	}
	if ( 51u <= (*p) && (*p) <= 57u )
		goto st61;
	goto st0;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 46u )
		goto st48;
	goto st0;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 48u: goto st49;
		case 49u: goto st56;
		case 50u: goto st58;
	}
	if ( 51u <= (*p) && (*p) <= 57u )
		goto st57;
	goto st0;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 46u )
		goto st50;
	goto st0;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 48u: goto st51;
		case 49u: goto st52;
		case 50u: goto st54;
	}
	if ( 51u <= (*p) && (*p) <= 57u )
		goto st53;
	goto st0;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	if ( (*p) == 93u )
		goto st179;
	goto st0;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
	switch( (*p) ) {
		case 35u: goto st171;
		case 47u: goto st170;
		case 58u: goto st178;
		case 63u: goto st170;
	}
	goto st0;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 93u )
		goto st179;
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st53;
	goto st0;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	if ( (*p) == 93u )
		goto st179;
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st51;
	goto st0;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	switch( (*p) ) {
		case 53u: goto st55;
		case 93u: goto st179;
	}
	if ( (*p) > 52u ) {
		if ( 54u <= (*p) && (*p) <= 57u )
			goto st51;
	} else if ( (*p) >= 48u )
		goto st53;
	goto st0;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	if ( (*p) == 93u )
		goto st179;
	if ( 48u <= (*p) && (*p) <= 53u )
		goto st51;
	goto st0;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 46u )
		goto st50;
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st57;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 46u )
		goto st50;
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st49;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 46u: goto st50;
		case 53u: goto st59;
	}
	if ( (*p) > 52u ) {
		if ( 54u <= (*p) && (*p) <= 57u )
			goto st49;
	} else if ( (*p) >= 48u )
		goto st57;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 46u )
		goto st50;
	if ( 48u <= (*p) && (*p) <= 53u )
		goto st49;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 46u )
		goto st48;
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 46u )
		goto st48;
	if ( 48u <= (*p) && (*p) <= 57u )
		goto st47;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	switch( (*p) ) {
		case 46u: goto st48;
		case 53u: goto st63;
	}
	if ( (*p) > 52u ) {
		if ( 54u <= (*p) && (*p) <= 57u )
			goto st47;
	} else if ( (*p) >= 48u )
		goto st61;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	if ( (*p) == 46u )
		goto st48;
	if ( 48u <= (*p) && (*p) <= 53u )
		goto st47;
	goto st0;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 58u )
		goto st67;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st65;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st65;
	} else
		goto st65;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 58u )
		goto st67;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st66;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st66;
	} else
		goto st66;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 58u )
		goto st67;
	goto st0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 58u )
		goto st51;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st68;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st68;
	} else
		goto st68;
	goto st0;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 93u )
		goto st179;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st69;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st69;
	} else
		goto st69;
	goto st0;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	if ( (*p) == 93u )
		goto st179;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st70;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st70;
	} else
		goto st70;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 93u )
		goto st179;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st51;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st51;
	} else
		goto st51;
	goto st0;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st67;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st72;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st64;
	} else
		goto st64;
	goto st0;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st67;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st73;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st65;
	} else
		goto st65;
	goto st0;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st67;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st66;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st66;
	} else
		goto st66;
	goto st0;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 46u: goto st46;
		case 53u: goto st75;
		case 58u: goto st67;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 52u )
			goto st72;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st64;
		} else if ( (*p) >= 65u )
			goto st64;
	} else
		goto st76;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st67;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 53u )
			goto st73;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st65;
		} else if ( (*p) >= 65u )
			goto st65;
	} else
		goto st65;
	goto st0;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st67;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st65;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st65;
	} else
		goto st65;
	goto st0;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st67;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st76;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st64;
	} else
		goto st64;
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 93u )
		goto st179;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st68;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st68;
	} else
		goto st68;
	goto st0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 58u )
		goto st67;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st64;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st64;
	} else
		goto st64;
	goto st0;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 48u: goto st81;
		case 49u: goto st86;
		case 50u: goto st89;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st92;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st93;
	} else
		goto st93;
	goto st0;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st82;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st82;
	} else
		goto st82;
	goto st0;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st83;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st83;
	} else
		goto st83;
	goto st0;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st84;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st84;
	} else
		goto st84;
	goto st0;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 58u: goto st85;
		case 93u: goto st179;
	}
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st68;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st68;
	} else
		goto st68;
	goto st0;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st87;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st82;
	} else
		goto st82;
	goto st0;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st88;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st83;
	} else
		goto st83;
	goto st0;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st84;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st84;
	} else
		goto st84;
	goto st0;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 46u: goto st46;
		case 53u: goto st90;
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 52u )
			goto st87;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st82;
		} else if ( (*p) >= 65u )
			goto st82;
	} else
		goto st91;
	goto st0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 53u )
			goto st88;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st83;
		} else if ( (*p) >= 65u )
			goto st83;
	} else
		goto st83;
	goto st0;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st83;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st83;
	} else
		goto st83;
	goto st0;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st91;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st82;
	} else
		goto st82;
	goto st0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	switch( (*p) ) {
		case 58u: goto st85;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st82;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st82;
	} else
		goto st82;
	goto st0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	switch( (*p) ) {
		case 48u: goto st95;
		case 49u: goto st100;
		case 50u: goto st103;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st106;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st107;
	} else
		goto st107;
	goto st0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st96;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st96;
	} else
		goto st96;
	goto st0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st97;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st97;
	} else
		goto st97;
	goto st0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	switch( (*p) ) {
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st98;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st98;
	} else
		goto st98;
	goto st0;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	switch( (*p) ) {
		case 58u: goto st99;
		case 93u: goto st179;
	}
	goto st0;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	switch( (*p) ) {
		case 48u: goto st81;
		case 49u: goto st86;
		case 50u: goto st89;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st92;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st93;
	} else
		goto st93;
	goto st0;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st101;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st96;
	} else
		goto st96;
	goto st0;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st102;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st97;
	} else
		goto st97;
	goto st0;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st98;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st98;
	} else
		goto st98;
	goto st0;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	switch( (*p) ) {
		case 46u: goto st46;
		case 53u: goto st104;
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 52u )
			goto st101;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st96;
		} else if ( (*p) >= 65u )
			goto st96;
	} else
		goto st105;
	goto st0;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 53u )
			goto st102;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st97;
		} else if ( (*p) >= 65u )
			goto st97;
	} else
		goto st97;
	goto st0;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st97;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st97;
	} else
		goto st97;
	goto st0;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st105;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st96;
	} else
		goto st96;
	goto st0;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 58u: goto st99;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st96;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st96;
	} else
		goto st96;
	goto st0;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	switch( (*p) ) {
		case 48u: goto st109;
		case 49u: goto st114;
		case 50u: goto st117;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st120;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st121;
	} else
		goto st121;
	goto st0;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st110;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st110;
	} else
		goto st110;
	goto st0;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	switch( (*p) ) {
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st111;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st111;
	} else
		goto st111;
	goto st0;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	switch( (*p) ) {
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st112;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st112;
	} else
		goto st112;
	goto st0;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	switch( (*p) ) {
		case 58u: goto st113;
		case 93u: goto st179;
	}
	goto st0;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	switch( (*p) ) {
		case 48u: goto st95;
		case 49u: goto st100;
		case 50u: goto st103;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st106;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st107;
	} else
		goto st107;
	goto st0;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st115;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st110;
	} else
		goto st110;
	goto st0;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st116;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st111;
	} else
		goto st111;
	goto st0;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st112;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st112;
	} else
		goto st112;
	goto st0;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	switch( (*p) ) {
		case 46u: goto st46;
		case 53u: goto st118;
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 52u )
			goto st115;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st110;
		} else if ( (*p) >= 65u )
			goto st110;
	} else
		goto st119;
	goto st0;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 53u )
			goto st116;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st111;
		} else if ( (*p) >= 65u )
			goto st111;
	} else
		goto st111;
	goto st0;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st111;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st111;
	} else
		goto st111;
	goto st0;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st119;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st110;
	} else
		goto st110;
	goto st0;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	switch( (*p) ) {
		case 58u: goto st113;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st110;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st110;
	} else
		goto st110;
	goto st0;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
	switch( (*p) ) {
		case 48u: goto st123;
		case 49u: goto st128;
		case 50u: goto st131;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st134;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st135;
	} else
		goto st135;
	goto st0;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st124;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st124;
	} else
		goto st124;
	goto st0;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	switch( (*p) ) {
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st125;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st125;
	} else
		goto st125;
	goto st0;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	switch( (*p) ) {
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st126;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st126;
	} else
		goto st126;
	goto st0;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	switch( (*p) ) {
		case 58u: goto st127;
		case 93u: goto st179;
	}
	goto st0;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	switch( (*p) ) {
		case 48u: goto st109;
		case 49u: goto st114;
		case 50u: goto st117;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st120;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st121;
	} else
		goto st121;
	goto st0;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st129;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st124;
	} else
		goto st124;
	goto st0;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st130;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st125;
	} else
		goto st125;
	goto st0;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st126;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st126;
	} else
		goto st126;
	goto st0;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	switch( (*p) ) {
		case 46u: goto st46;
		case 53u: goto st132;
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 52u )
			goto st129;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st124;
		} else if ( (*p) >= 65u )
			goto st124;
	} else
		goto st133;
	goto st0;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 53u )
			goto st130;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st125;
		} else if ( (*p) >= 65u )
			goto st125;
	} else
		goto st125;
	goto st0;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st125;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st125;
	} else
		goto st125;
	goto st0;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st133;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st124;
	} else
		goto st124;
	goto st0;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
	switch( (*p) ) {
		case 58u: goto st127;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st124;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st124;
	} else
		goto st124;
	goto st0;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	switch( (*p) ) {
		case 48u: goto st137;
		case 49u: goto st142;
		case 50u: goto st145;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st148;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st149;
	} else
		goto st149;
	goto st0;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st138;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st138;
	} else
		goto st138;
	goto st0;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	switch( (*p) ) {
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st139;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st139;
	} else
		goto st139;
	goto st0;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	switch( (*p) ) {
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st140;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st140;
	} else
		goto st140;
	goto st0;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
	switch( (*p) ) {
		case 58u: goto st141;
		case 93u: goto st179;
	}
	goto st0;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
	switch( (*p) ) {
		case 48u: goto st123;
		case 49u: goto st128;
		case 50u: goto st131;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st134;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st135;
	} else
		goto st135;
	goto st0;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st143;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st138;
	} else
		goto st138;
	goto st0;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st144;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st139;
	} else
		goto st139;
	goto st0;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st140;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st140;
	} else
		goto st140;
	goto st0;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
	switch( (*p) ) {
		case 46u: goto st46;
		case 53u: goto st146;
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 52u )
			goto st143;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st138;
		} else if ( (*p) >= 65u )
			goto st138;
	} else
		goto st147;
	goto st0;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 53u )
			goto st144;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st139;
		} else if ( (*p) >= 65u )
			goto st139;
	} else
		goto st139;
	goto st0;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st139;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st139;
	} else
		goto st139;
	goto st0;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st147;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st138;
	} else
		goto st138;
	goto st0;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
	switch( (*p) ) {
		case 58u: goto st141;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st138;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st138;
	} else
		goto st138;
	goto st0;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
	if ( (*p) == 58u )
		goto st151;
	goto st0;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
	switch( (*p) ) {
		case 48u: goto st152;
		case 49u: goto st157;
		case 50u: goto st160;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st163;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st164;
	} else
		goto st164;
	goto st0;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st153;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st153;
	} else
		goto st153;
	goto st0;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	switch( (*p) ) {
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st154;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st154;
	} else
		goto st154;
	goto st0;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
	switch( (*p) ) {
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st155;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st155;
	} else
		goto st155;
	goto st0;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
	switch( (*p) ) {
		case 58u: goto st156;
		case 93u: goto st179;
	}
	goto st0;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
	switch( (*p) ) {
		case 48u: goto st137;
		case 49u: goto st142;
		case 50u: goto st145;
	}
	if ( (*p) < 65u ) {
		if ( 51u <= (*p) && (*p) <= 57u )
			goto st148;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st149;
	} else
		goto st149;
	goto st0;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st158;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st153;
	} else
		goto st153;
	goto st0;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st159;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st154;
	} else
		goto st154;
	goto st0;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st155;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st155;
	} else
		goto st155;
	goto st0;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
	switch( (*p) ) {
		case 46u: goto st46;
		case 53u: goto st161;
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 52u )
			goto st158;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st153;
		} else if ( (*p) >= 65u )
			goto st153;
	} else
		goto st162;
	goto st0;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 54u ) {
		if ( 48u <= (*p) && (*p) <= 53u )
			goto st159;
	} else if ( (*p) > 57u ) {
		if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto st154;
		} else if ( (*p) >= 65u )
			goto st154;
	} else
		goto st154;
	goto st0;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st154;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st154;
	} else
		goto st154;
	goto st0;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
	switch( (*p) ) {
		case 46u: goto st46;
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st162;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st153;
	} else
		goto st153;
	goto st0;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
	switch( (*p) ) {
		case 58u: goto st156;
		case 93u: goto st179;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st153;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st153;
	} else
		goto st153;
	goto st0;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st166;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st166;
	} else
		goto st166;
	goto st0;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
	if ( (*p) == 46u )
		goto st167;
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 57u )
			goto st166;
	} else if ( (*p) > 70u ) {
		if ( 97u <= (*p) && (*p) <= 102u )
			goto st166;
	} else
		goto st166;
	goto st0;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
	switch( (*p) ) {
		case 33u: goto st168;
		case 36u: goto st168;
		case 61u: goto st168;
		case 95u: goto st168;
		case 126u: goto st168;
	}
	if ( (*p) < 48u ) {
		if ( 38u <= (*p) && (*p) <= 46u )
			goto st168;
	} else if ( (*p) > 59u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st168;
		} else if ( (*p) >= 65u )
			goto st168;
	} else
		goto st168;
	goto st0;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
	switch( (*p) ) {
		case 33u: goto st168;
		case 36u: goto st168;
		case 61u: goto st168;
		case 93u: goto st179;
		case 95u: goto st168;
		case 126u: goto st168;
	}
	if ( (*p) < 48u ) {
		if ( 38u <= (*p) && (*p) <= 46u )
			goto st168;
	} else if ( (*p) > 59u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st168;
		} else if ( (*p) >= 65u )
			goto st168;
	} else
		goto st168;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof169: cs = 169; goto _test_eof; 
	_test_eof170: cs = 170; goto _test_eof; 
	_test_eof171: cs = 171; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof172: cs = 172; goto _test_eof; 
	_test_eof173: cs = 173; goto _test_eof; 
	_test_eof174: cs = 174; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof175: cs = 175; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof176: cs = 176; goto _test_eof; 
	_test_eof177: cs = 177; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof178: cs = 178; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
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
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof179: cs = 179; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 
	_test_eof129: cs = 129; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 
	_test_eof132: cs = 132; goto _test_eof; 
	_test_eof133: cs = 133; goto _test_eof; 
	_test_eof134: cs = 134; goto _test_eof; 
	_test_eof135: cs = 135; goto _test_eof; 
	_test_eof136: cs = 136; goto _test_eof; 
	_test_eof137: cs = 137; goto _test_eof; 
	_test_eof138: cs = 138; goto _test_eof; 
	_test_eof139: cs = 139; goto _test_eof; 
	_test_eof140: cs = 140; goto _test_eof; 
	_test_eof141: cs = 141; goto _test_eof; 
	_test_eof142: cs = 142; goto _test_eof; 
	_test_eof143: cs = 143; goto _test_eof; 
	_test_eof144: cs = 144; goto _test_eof; 
	_test_eof145: cs = 145; goto _test_eof; 
	_test_eof146: cs = 146; goto _test_eof; 
	_test_eof147: cs = 147; goto _test_eof; 
	_test_eof148: cs = 148; goto _test_eof; 
	_test_eof149: cs = 149; goto _test_eof; 
	_test_eof150: cs = 150; goto _test_eof; 
	_test_eof151: cs = 151; goto _test_eof; 
	_test_eof152: cs = 152; goto _test_eof; 
	_test_eof153: cs = 153; goto _test_eof; 
	_test_eof154: cs = 154; goto _test_eof; 
	_test_eof155: cs = 155; goto _test_eof; 
	_test_eof156: cs = 156; goto _test_eof; 
	_test_eof157: cs = 157; goto _test_eof; 
	_test_eof158: cs = 158; goto _test_eof; 
	_test_eof159: cs = 159; goto _test_eof; 
	_test_eof160: cs = 160; goto _test_eof; 
	_test_eof161: cs = 161; goto _test_eof; 
	_test_eof162: cs = 162; goto _test_eof; 
	_test_eof163: cs = 163; goto _test_eof; 
	_test_eof164: cs = 164; goto _test_eof; 
	_test_eof165: cs = 165; goto _test_eof; 
	_test_eof166: cs = 166; goto _test_eof; 
	_test_eof167: cs = 167; goto _test_eof; 
	_test_eof168: cs = 168; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 38 "../ragel/formats/uri/main.rl"

  return cs != 
#line 3102 "schwa/formats/uri_gen.cc"
0
#line 39 "../ragel/formats/uri/main.rl"
;
}

}  // namespace formats
}  // namespace schwa
