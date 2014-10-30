
#line 1 "../ragel/formats/html/content_type_main.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 46 "../ragel/formats/html/content_type_main.rl"


#include <schwa/formats/html.h>

namespace schwa {
namespace formats {


#line 17 "schwa/formats/html_charset_sniff_content_type_gen.cc"
static const int html_charset_sniff_content_type_start = 1;

static const int html_charset_sniff_content_type_en_main = 1;


#line 54 "../ragel/formats/html/content_type_main.rl"

bool
HTMLCharsetSniffer::_run_content_type(const uint8_t *const input, const size_t nbytes) {
  (void)html_charset_sniff_content_type_en_main;  // Shoosh compiler warning about unused variable.
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  int cs = 0;

  
#line 32 "schwa/formats/html_charset_sniff_content_type_gen.cc"
	{
	cs = html_charset_sniff_content_type_start;
	}

#line 62 "../ragel/formats/html/content_type_main.rl"
  
#line 39 "schwa/formats/html_charset_sniff_content_type_gen.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 32: goto st2;
		case 33: goto st3;
		case 124: goto st3;
		case 126: goto st3;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 35 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto st1;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st3;
		} else
			goto st3;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st3;
		} else if ( (*p) > 90 ) {
			if ( 94 <= (*p) && (*p) <= 122 )
				goto st3;
		} else
			goto st3;
	} else
		goto st3;
	goto st0;
st0:
cs = 0;
	goto _out;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 32: goto st2;
		case 33: goto st3;
		case 47: goto st4;
		case 124: goto st3;
		case 126: goto st3;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 39 )
				goto st3;
		} else if ( (*p) >= 9 )
			goto st1;
	} else if ( (*p) > 43 ) {
		if ( (*p) < 65 ) {
			if ( 45 <= (*p) && (*p) <= 57 )
				goto st3;
		} else if ( (*p) > 90 ) {
			if ( 94 <= (*p) && (*p) <= 122 )
				goto st3;
		} else
			goto st3;
	} else
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 47: goto st4;
		case 124: goto st3;
		case 126: goto st3;
	}
	if ( (*p) < 42 ) {
		if ( (*p) > 33 ) {
			if ( 35 <= (*p) && (*p) <= 39 )
				goto st3;
		} else if ( (*p) >= 32 )
			goto st3;
	} else if ( (*p) > 43 ) {
		if ( (*p) < 65 ) {
			if ( 45 <= (*p) && (*p) <= 57 )
				goto st3;
		} else if ( (*p) > 90 ) {
			if ( 94 <= (*p) && (*p) <= 122 )
				goto st3;
		} else
			goto st3;
	} else
		goto st3;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 124: goto st11;
		case 126: goto st11;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 35 ) {
			if ( 32 <= (*p) && (*p) <= 33 )
				goto st11;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st11;
		} else
			goto st11;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st11;
		} else if ( (*p) > 90 ) {
			if ( 94 <= (*p) && (*p) <= 122 )
				goto st11;
		} else
			goto st11;
	} else
		goto st11;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 59: goto st5;
		case 124: goto st11;
		case 126: goto st11;
	}
	if ( (*p) < 42 ) {
		if ( (*p) < 32 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto st12;
		} else if ( (*p) > 33 ) {
			if ( 35 <= (*p) && (*p) <= 39 )
				goto st11;
		} else
			goto st11;
	} else if ( (*p) > 43 ) {
		if ( (*p) < 48 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st11;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 90 ) {
				if ( 94 <= (*p) && (*p) <= 122 )
					goto st11;
			} else if ( (*p) >= 65 )
				goto st11;
		} else
			goto st11;
	} else
		goto st11;
	goto st0;
tr17:
#line 10 "../ragel/formats/html/content_type_main.rl"
	{ _content_type_param_val_end(p); }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 205 "schwa/formats/html_charset_sniff_content_type_gen.cc"
	switch( (*p) ) {
		case 32: goto st12;
		case 59: goto st5;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st12;
	goto st0;
tr20:
#line 10 "../ragel/formats/html/content_type_main.rl"
	{ _content_type_param_val_end(p); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 221 "schwa/formats/html_charset_sniff_content_type_gen.cc"
	switch( (*p) ) {
		case 32: goto tr7;
		case 33: goto tr8;
		case 124: goto tr8;
		case 126: goto tr8;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 35 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto st5;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr8;
		} else
			goto tr8;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr8;
		} else if ( (*p) > 90 ) {
			if ( 94 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else
			goto tr8;
	} else
		goto tr8;
	goto st0;
tr7:
#line 7 "../ragel/formats/html/content_type_main.rl"
	{ _content_type_param_key_start(p); }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 257 "schwa/formats/html_charset_sniff_content_type_gen.cc"
	switch( (*p) ) {
		case 32: goto tr7;
		case 33: goto tr8;
		case 61: goto tr9;
		case 124: goto tr8;
		case 126: goto tr8;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 35 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto st5;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr8;
		} else
			goto tr8;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr8;
		} else if ( (*p) > 90 ) {
			if ( 94 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else
			goto tr8;
	} else
		goto tr8;
	goto st0;
tr8:
#line 7 "../ragel/formats/html/content_type_main.rl"
	{ _content_type_param_key_start(p); }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 294 "schwa/formats/html_charset_sniff_content_type_gen.cc"
	switch( (*p) ) {
		case 61: goto tr9;
		case 124: goto st7;
		case 126: goto st7;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 35 ) {
			if ( 32 <= (*p) && (*p) <= 33 )
				goto st7;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st7;
		} else
			goto st7;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st7;
		} else if ( (*p) > 90 ) {
			if ( 94 <= (*p) && (*p) <= 122 )
				goto st7;
		} else
			goto st7;
	} else
		goto st7;
	goto st0;
tr9:
#line 8 "../ragel/formats/html/content_type_main.rl"
	{ _content_type_param_key_end(p); }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 329 "schwa/formats/html_charset_sniff_content_type_gen.cc"
	switch( (*p) ) {
		case 34: goto tr12;
		case 124: goto tr11;
		case 126: goto tr11;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr11;
		} else if ( (*p) >= 32 )
			goto tr11;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr11;
		} else if ( (*p) > 90 ) {
			if ( 94 <= (*p) && (*p) <= 122 )
				goto tr11;
		} else
			goto tr11;
	} else
		goto tr11;
	goto st0;
tr11:
#line 9 "../ragel/formats/html/content_type_main.rl"
	{ _content_type_param_val_start(p); }
	goto st13;
tr18:
#line 10 "../ragel/formats/html/content_type_main.rl"
	{ _content_type_param_val_end(p); }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 365 "schwa/formats/html_charset_sniff_content_type_gen.cc"
	switch( (*p) ) {
		case 32: goto tr18;
		case 33: goto st13;
		case 59: goto tr20;
		case 124: goto st13;
		case 126: goto st13;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 35 ) {
			if ( 9 <= (*p) && (*p) <= 13 )
				goto tr17;
		} else if ( (*p) > 39 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto st13;
		} else
			goto st13;
	} else if ( (*p) > 46 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st13;
		} else if ( (*p) > 90 ) {
			if ( 94 <= (*p) && (*p) <= 122 )
				goto st13;
		} else
			goto st13;
	} else
		goto st13;
	goto st0;
tr12:
#line 9 "../ragel/formats/html/content_type_main.rl"
	{ _content_type_param_val_start(p); }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 402 "schwa/formats/html_charset_sniff_content_type_gen.cc"
	switch( (*p) ) {
		case 34: goto st14;
		case 92: goto st10;
		case 127: goto st0;
	}
	if ( (*p) > 8 ) {
		if ( 10 <= (*p) && (*p) <= 31 )
			goto st0;
	} else if ( (*p) >= 0 )
		goto st0;
	goto st9;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 32: goto tr17;
		case 59: goto tr20;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr17;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 127 )
		goto st0;
	if ( (*p) > 8 ) {
		if ( 10 <= (*p) && (*p) <= 31 )
			goto st0;
	} else if ( (*p) >= 0 )
		goto st0;
	goto st9;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 13: 
	case 14: 
#line 10 "../ragel/formats/html/content_type_main.rl"
	{ _content_type_param_val_end(p); }
	break;
#line 462 "schwa/formats/html_charset_sniff_content_type_gen.cc"
	}
	}

	_out: {}
	}

#line 63 "../ragel/formats/html/content_type_main.rl"

  return cs != 
#line 472 "schwa/formats/html_charset_sniff_content_type_gen.cc"
0
#line 64 "../ragel/formats/html/content_type_main.rl"
;
}

}  // namespace formats
}  // namespace schwa
