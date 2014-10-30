
#line 1 "../ragel/formats/html/main.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 15 "../ragel/formats/html/main.rl"


#include <schwa/formats/html.h>
#include <iostream>

namespace schwa {
namespace formats {


#line 18 "schwa/formats/html_gen.cc"
static const int html_start = 320;

static const int html_en_try_recover = 319;
static const int html_en_main = 320;


#line 24 "../ragel/formats/html/main.rl"

bool
HTMLLexer::_run(const uint8_t *const input, const size_t nbytes) {
  (void)html_en_main;  // Shoosh compiler warning about unused variable.
  (void)html_en_try_recover;  // Shoosh compiler warning about unused variable.
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  int cs;

  
#line 35 "schwa/formats/html_gen.cc"
	{
	cs = html_start;
	}

#line 33 "../ragel/formats/html/main.rl"

  _begin_document();
  
#line 44 "schwa/formats/html_gen.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
st320:
	if ( ++p == pe )
		goto _test_eof320;
case 320:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr631;
		case 60u: goto tr632;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr633;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr635;
	} else
		goto tr634;
	goto tr630;
tr0:
#line 9 "../ragel/formats/html/main.rl"
	{
    p--; {goto st319;}
  }
	goto st0;
#line 80 "schwa/formats/html_gen.cc"
st0:
cs = 0;
	goto _out;
tr630:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st321;
tr636:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st321;
tr647:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st321;
tr702:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st321;
tr704:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st321;
tr648:
#line 20 "../ragel/formats/html/html.rl"
	{ _comment_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st321;
tr654:
#line 22 "../ragel/formats/html/html.rl"
	{ _doctype_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st321;
tr660:
#line 18 "../ragel/formats/html/html.rl"
	{ _cdata_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st321;
tr666:
#line 30 "../ragel/formats/html/html.rl"
	{ _close_tag_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st321;
tr672:
#line 25 "../ragel/formats/html/html.rl"
	{ _open_tag_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st321;
st321:
	if ( ++p == pe )
		goto _test_eof321;
case 321:
#line 152 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr637;
		case 60u: goto tr638;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr639;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr641;
	} else
		goto tr640;
	goto tr636;
tr631:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st322;
tr637:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st322;
tr649:
#line 20 "../ragel/formats/html/html.rl"
	{ _comment_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st322;
tr655:
#line 22 "../ragel/formats/html/html.rl"
	{ _doctype_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st322;
tr661:
#line 18 "../ragel/formats/html/html.rl"
	{ _cdata_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st322;
tr667:
#line 30 "../ragel/formats/html/html.rl"
	{ _close_tag_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st322;
tr673:
#line 25 "../ragel/formats/html/html.rl"
	{ _open_tag_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st322;
st322:
	if ( ++p == pe )
		goto _test_eof322;
case 322:
#line 217 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 35u: goto tr642;
		case 38u: goto tr637;
		case 60u: goto tr638;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr636;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr643;
			} else if ( (*p) >= 58u )
				goto tr636;
		} else
			goto tr643;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr636;
			} else if ( (*p) >= 97u )
				goto tr643;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr641;
			} else if ( (*p) >= 224u )
				goto tr640;
		} else
			goto tr639;
	} else
		goto tr636;
	goto tr0;
tr642:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st323;
st323:
	if ( ++p == pe )
		goto _test_eof323;
case 323:
#line 263 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr637;
		case 60u: goto tr638;
		case 88u: goto tr645;
		case 120u: goto tr645;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr644;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr640;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr641;
	} else
		goto tr639;
	goto tr636;
tr646:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st324;
tr644:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st324;
st324:
	if ( ++p == pe )
		goto _test_eof324;
case 324:
#line 307 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr637;
		case 59u: goto tr647;
		case 60u: goto tr638;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr646;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr640;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr641;
	} else
		goto tr639;
	goto tr636;
tr632:
#line 19 "../ragel/formats/html/html.rl"
	{ _comment_start(p); }
#line 17 "../ragel/formats/html/html.rl"
	{ _cdata_start(p); }
#line 21 "../ragel/formats/html/html.rl"
	{ _doctype_start(p); }
#line 29 "../ragel/formats/html/html.rl"
	{ _close_tag_start(p); }
#line 24 "../ragel/formats/html/html.rl"
	{ _open_tag_start(p); }
	goto st1;
tr638:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 19 "../ragel/formats/html/html.rl"
	{ _comment_start(p); }
#line 17 "../ragel/formats/html/html.rl"
	{ _cdata_start(p); }
#line 21 "../ragel/formats/html/html.rl"
	{ _doctype_start(p); }
#line 29 "../ragel/formats/html/html.rl"
	{ _close_tag_start(p); }
#line 24 "../ragel/formats/html/html.rl"
	{ _open_tag_start(p); }
	goto st1;
tr650:
#line 20 "../ragel/formats/html/html.rl"
	{ _comment_end(p); }
#line 19 "../ragel/formats/html/html.rl"
	{ _comment_start(p); }
#line 17 "../ragel/formats/html/html.rl"
	{ _cdata_start(p); }
#line 21 "../ragel/formats/html/html.rl"
	{ _doctype_start(p); }
#line 29 "../ragel/formats/html/html.rl"
	{ _close_tag_start(p); }
#line 24 "../ragel/formats/html/html.rl"
	{ _open_tag_start(p); }
	goto st1;
tr656:
#line 22 "../ragel/formats/html/html.rl"
	{ _doctype_end(p); }
#line 19 "../ragel/formats/html/html.rl"
	{ _comment_start(p); }
#line 17 "../ragel/formats/html/html.rl"
	{ _cdata_start(p); }
#line 21 "../ragel/formats/html/html.rl"
	{ _doctype_start(p); }
#line 29 "../ragel/formats/html/html.rl"
	{ _close_tag_start(p); }
#line 24 "../ragel/formats/html/html.rl"
	{ _open_tag_start(p); }
	goto st1;
tr662:
#line 18 "../ragel/formats/html/html.rl"
	{ _cdata_end(p); }
#line 19 "../ragel/formats/html/html.rl"
	{ _comment_start(p); }
#line 17 "../ragel/formats/html/html.rl"
	{ _cdata_start(p); }
#line 21 "../ragel/formats/html/html.rl"
	{ _doctype_start(p); }
#line 29 "../ragel/formats/html/html.rl"
	{ _close_tag_start(p); }
#line 24 "../ragel/formats/html/html.rl"
	{ _open_tag_start(p); }
	goto st1;
tr668:
#line 30 "../ragel/formats/html/html.rl"
	{ _close_tag_end(p); }
#line 19 "../ragel/formats/html/html.rl"
	{ _comment_start(p); }
#line 17 "../ragel/formats/html/html.rl"
	{ _cdata_start(p); }
#line 21 "../ragel/formats/html/html.rl"
	{ _doctype_start(p); }
#line 29 "../ragel/formats/html/html.rl"
	{ _close_tag_start(p); }
#line 24 "../ragel/formats/html/html.rl"
	{ _open_tag_start(p); }
	goto st1;
tr674:
#line 25 "../ragel/formats/html/html.rl"
	{ _open_tag_end(p); }
#line 19 "../ragel/formats/html/html.rl"
	{ _comment_start(p); }
#line 17 "../ragel/formats/html/html.rl"
	{ _cdata_start(p); }
#line 21 "../ragel/formats/html/html.rl"
	{ _doctype_start(p); }
#line 29 "../ragel/formats/html/html.rl"
	{ _close_tag_start(p); }
#line 24 "../ragel/formats/html/html.rl"
	{ _open_tag_start(p); }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 432 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 33u: goto st2;
		case 37u: goto st46;
		case 47u: goto st48;
		case 63u: goto st90;
		case 83u: goto tr6;
		case 84u: goto tr7;
		case 115u: goto tr6;
		case 116u: goto tr7;
	}
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 58u )
			goto tr4;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr4;
	} else
		goto tr4;
	goto tr0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 45u: goto st3;
		case 68u: goto st20;
		case 91u: goto st27;
		case 100u: goto st20;
	}
	goto tr0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 45u )
		goto st4;
	goto tr0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr13;
		case 45u: goto tr14;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr15;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr17;
	} else
		goto tr16;
	goto tr12;
tr12:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st5;
tr18:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st5;
tr37:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st5;
tr40:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st5;
tr42:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 532 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr19;
		case 45u: goto tr20;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr21;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr23;
	} else
		goto tr22;
	goto tr18;
tr13:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st6;
tr19:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 567 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 35u: goto tr24;
		case 38u: goto tr19;
		case 45u: goto tr20;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr18;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr25;
			} else if ( (*p) >= 58u )
				goto tr18;
		} else
			goto tr25;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr18;
			} else if ( (*p) >= 97u )
				goto tr25;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr23;
			} else if ( (*p) >= 224u )
				goto tr22;
		} else
			goto tr21;
	} else
		goto tr18;
	goto tr0;
tr24:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 613 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr19;
		case 45u: goto tr20;
		case 88u: goto tr27;
		case 120u: goto tr27;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr26;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr22;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr23;
	} else
		goto tr21;
	goto tr18;
tr14:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st8;
tr20:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 653 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr19;
		case 45u: goto tr28;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr21;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr23;
	} else
		goto tr22;
	goto tr18;
tr28:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 684 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr19;
		case 45u: goto tr28;
		case 62u: goto st325;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr21;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr23;
	} else
		goto tr22;
	goto tr18;
st325:
	if ( ++p == pe )
		goto _test_eof325;
case 325:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr649;
		case 60u: goto tr650;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr651;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr653;
	} else
		goto tr652;
	goto tr648;
tr633:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st10;
tr639:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st10;
tr651:
#line 20 "../ragel/formats/html/html.rl"
	{ _comment_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st10;
tr657:
#line 22 "../ragel/formats/html/html.rl"
	{ _doctype_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st10;
tr663:
#line 18 "../ragel/formats/html/html.rl"
	{ _cdata_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st10;
tr669:
#line 30 "../ragel/formats/html/html.rl"
	{ _close_tag_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st10;
tr675:
#line 25 "../ragel/formats/html/html.rl"
	{ _open_tag_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 774 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st321;
	goto tr0;
tr634:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st11;
tr640:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st11;
tr652:
#line 20 "../ragel/formats/html/html.rl"
	{ _comment_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st11;
tr658:
#line 22 "../ragel/formats/html/html.rl"
	{ _doctype_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st11;
tr664:
#line 18 "../ragel/formats/html/html.rl"
	{ _cdata_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st11;
tr670:
#line 30 "../ragel/formats/html/html.rl"
	{ _close_tag_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st11;
tr676:
#line 25 "../ragel/formats/html/html.rl"
	{ _open_tag_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 822 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st10;
	goto tr0;
tr635:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st12;
tr641:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st12;
tr653:
#line 20 "../ragel/formats/html/html.rl"
	{ _comment_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st12;
tr659:
#line 22 "../ragel/formats/html/html.rl"
	{ _doctype_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st12;
tr665:
#line 18 "../ragel/formats/html/html.rl"
	{ _cdata_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st12;
tr671:
#line 30 "../ragel/formats/html/html.rl"
	{ _close_tag_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st12;
tr677:
#line 25 "../ragel/formats/html/html.rl"
	{ _open_tag_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 870 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st11;
	goto tr0;
tr15:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st13;
tr21:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 888 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st5;
	goto tr0;
tr16:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st14;
tr22:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 906 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st13;
	goto tr0;
tr17:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st15;
tr23:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 924 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st14;
	goto tr0;
tr36:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st16;
tr26:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 946 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr19;
		case 45u: goto tr20;
		case 59u: goto tr37;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr36;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr22;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr23;
	} else
		goto tr21;
	goto tr18;
tr27:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 981 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr19;
		case 45u: goto tr20;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr38;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr38;
		} else
			goto tr38;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr21;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr23;
		} else
			goto tr22;
	} else
		goto tr0;
	goto tr18;
tr39:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st18;
tr38:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 1029 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr19;
		case 45u: goto tr20;
		case 59u: goto tr40;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr39;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr39;
		} else
			goto tr39;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr21;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr23;
		} else
			goto tr22;
	} else
		goto tr0;
	goto tr18;
tr41:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st19;
tr25:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 1078 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 38u: goto tr19;
		case 45u: goto tr20;
		case 59u: goto tr42;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr18;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr41;
			} else if ( (*p) >= 58u )
				goto tr18;
		} else
			goto tr41;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr18;
			} else if ( (*p) >= 97u )
				goto tr41;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr23;
			} else if ( (*p) >= 224u )
				goto tr22;
		} else
			goto tr21;
	} else
		goto tr18;
	goto tr0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 79u: goto st21;
		case 111u: goto st21;
	}
	goto tr0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 67u: goto st22;
		case 99u: goto st22;
	}
	goto tr0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 84u: goto st23;
		case 116u: goto st23;
	}
	goto tr0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 89u: goto st24;
		case 121u: goto st24;
	}
	goto tr0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 80u: goto st25;
		case 112u: goto st25;
	}
	goto tr0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 69u: goto st26;
		case 101u: goto st26;
	}
	goto tr0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 62u )
		goto st326;
	goto st26;
st326:
	if ( ++p == pe )
		goto _test_eof326;
case 326:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr655;
		case 60u: goto tr656;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr657;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr659;
	} else
		goto tr658;
	goto tr654;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 67u )
		goto st28;
	goto tr0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 68u )
		goto st29;
	goto tr0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 65u )
		goto st30;
	goto tr0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 84u )
		goto st31;
	goto tr0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 65u )
		goto st32;
	goto tr0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 91u )
		goto st33;
	goto tr0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr57;
		case 60u: goto tr0;
		case 93u: goto tr58;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr59;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr61;
	} else
		goto tr60;
	goto tr56;
tr56:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st34;
tr62:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st34;
tr73:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st34;
tr81:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st34;
tr83:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 1304 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr63;
		case 60u: goto tr0;
		case 93u: goto tr64;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr65;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr67;
	} else
		goto tr66;
	goto tr62;
tr57:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st35;
tr63:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 1340 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 35u: goto tr68;
		case 38u: goto tr63;
		case 93u: goto tr64;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr69;
			} else if ( (*p) >= 1u )
				goto tr62;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr69;
			} else if ( (*p) >= 61u )
				goto tr62;
		} else
			goto tr62;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr62;
			} else if ( (*p) >= 97u )
				goto tr69;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr67;
			} else if ( (*p) >= 224u )
				goto tr66;
		} else
			goto tr65;
	} else
		goto tr62;
	goto tr0;
tr68:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 1389 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr63;
		case 60u: goto tr0;
		case 88u: goto tr71;
		case 93u: goto tr64;
		case 120u: goto tr71;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr70;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr66;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr67;
	} else
		goto tr65;
	goto tr62;
tr72:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st37;
tr70:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 1434 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr63;
		case 59u: goto tr73;
		case 60u: goto tr0;
		case 93u: goto tr64;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr72;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr66;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr67;
	} else
		goto tr65;
	goto tr62;
tr58:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st38;
tr64:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 1474 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr63;
		case 60u: goto tr0;
		case 93u: goto tr74;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr65;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr67;
	} else
		goto tr66;
	goto tr62;
tr74:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1506 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr63;
		case 60u: goto tr0;
		case 62u: goto st327;
		case 93u: goto tr74;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr65;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr67;
	} else
		goto tr66;
	goto tr62;
st327:
	if ( ++p == pe )
		goto _test_eof327;
case 327:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr661;
		case 60u: goto tr662;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr663;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr665;
	} else
		goto tr664;
	goto tr660;
tr59:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st40;
tr65:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 1567 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st34;
	goto tr0;
tr60:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st41;
tr66:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 1585 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st40;
	goto tr0;
tr61:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st42;
tr67:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
#line 1603 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st41;
	goto tr0;
tr71:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 1617 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr63;
		case 60u: goto tr0;
		case 93u: goto tr64;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr79;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr79;
		} else
			goto tr79;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr65;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr67;
		} else
			goto tr66;
	} else
		goto tr0;
	goto tr62;
tr80:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st44;
tr79:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 1666 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr63;
		case 59u: goto tr81;
		case 60u: goto tr0;
		case 93u: goto tr64;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr80;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr80;
		} else
			goto tr80;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr65;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr67;
		} else
			goto tr66;
	} else
		goto tr0;
	goto tr62;
tr82:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st45;
tr69:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
#line 1716 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 38u: goto tr63;
		case 58u: goto tr62;
		case 59u: goto tr83;
		case 93u: goto tr64;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr62;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr82;
			} else if ( (*p) >= 61u )
				goto tr62;
		} else
			goto tr82;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr62;
			} else if ( (*p) >= 97u )
				goto tr82;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr67;
			} else if ( (*p) >= 224u )
				goto tr66;
		} else
			goto tr65;
	} else
		goto tr62;
	goto tr0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 37u )
		goto st47;
	goto st46;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 37u: goto st47;
		case 62u: goto st320;
	}
	goto st46;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	if ( (*p) < 65u ) {
		if ( 48u <= (*p) && (*p) <= 58u )
			goto tr86;
	} else if ( (*p) > 90u ) {
		if ( 97u <= (*p) && (*p) <= 122u )
			goto tr86;
	} else
		goto tr86;
	goto tr0;
tr86:
#line 31 "../ragel/formats/html/html.rl"
	{ _close_tag_name_start(p); }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 1790 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr87;
		case 62u: goto tr89;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr87;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st49;
		} else if ( (*p) >= 65u )
			goto st49;
	} else
		goto st49;
	goto tr0;
tr87:
#line 32 "../ragel/formats/html/html.rl"
	{ _close_tag_name_end(p); }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 1815 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st50;
		case 62u: goto st328;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st50;
	goto tr0;
tr89:
#line 32 "../ragel/formats/html/html.rl"
	{ _close_tag_name_end(p); }
	goto st328;
st328:
	if ( ++p == pe )
		goto _test_eof328;
case 328:
#line 1831 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr667;
		case 60u: goto tr668;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr669;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr671;
	} else
		goto tr670;
	goto tr666;
tr4:
#line 26 "../ragel/formats/html/html.rl"
	{ _open_tag_name_start(p); }
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 1860 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
tr92:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st52;
tr110:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 1890 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st52;
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st54;
		case 61u: goto tr0;
		case 62u: goto st329;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st52;
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st54;
		case 61u: goto st55;
		case 62u: goto st329;
	}
	goto st53;
tr93:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 1922 "schwa/formats/html_gen.cc"
	if ( (*p) == 62u )
		goto tr101;
	goto tr0;
tr95:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st329;
tr101:
#line 28 "../ragel/formats/html/html.rl"
	{ _open_tag_self_closing(); }
	goto st329;
tr113:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st329;
tr121:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 28 "../ragel/formats/html/html.rl"
	{ _open_tag_self_closing(); }
	goto st329;
st329:
	if ( ++p == pe )
		goto _test_eof329;
case 329:
#line 1948 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr673;
		case 60u: goto tr674;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr675;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr677;
	} else
		goto tr676;
	goto tr672;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto st55;
		case 34u: goto st67;
		case 38u: goto tr104;
		case 39u: goto st79;
		case 60u: goto tr0;
		case 62u: goto tr0;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto st55;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr107;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr108;
	} else
		goto tr106;
	goto tr102;
tr102:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st56;
tr109:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st56;
tr126:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st56;
tr129:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st56;
tr131:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 2039 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr110;
		case 34u: goto tr0;
		case 38u: goto tr111;
		case 39u: goto tr0;
		case 47u: goto tr112;
		case 60u: goto tr0;
		case 62u: goto tr113;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto tr110;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr115;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr116;
	} else
		goto tr114;
	goto tr109;
tr104:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st57;
tr111:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st57;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
#line 2083 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr110;
		case 35u: goto tr117;
		case 38u: goto tr111;
		case 47u: goto tr112;
		case 62u: goto tr113;
	}
	if ( (*p) < 61u ) {
		if ( (*p) < 36u ) {
			if ( (*p) < 9u ) {
				if ( 1u <= (*p) && (*p) <= 8u )
					goto tr109;
			} else if ( (*p) > 13u ) {
				if ( 14u <= (*p) && (*p) <= 33u )
					goto tr109;
			} else
				goto tr110;
		} else if ( (*p) > 37u ) {
			if ( (*p) < 48u ) {
				if ( 40u <= (*p) && (*p) <= 46u )
					goto tr109;
			} else if ( (*p) > 57u ) {
				if ( 58u <= (*p) && (*p) <= 59u )
					goto tr109;
			} else
				goto tr118;
		} else
			goto tr109;
	} else if ( (*p) > 64u ) {
		if ( (*p) < 123u ) {
			if ( (*p) < 91u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr118;
			} else if ( (*p) > 95u ) {
				if ( 97u <= (*p) && (*p) <= 122u )
					goto tr118;
			} else
				goto tr109;
		} else if ( (*p) > 127u ) {
			if ( (*p) < 224u ) {
				if ( 192u <= (*p) && (*p) <= 223u )
					goto tr114;
			} else if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr116;
			} else
				goto tr115;
		} else
			goto tr109;
	} else
		goto tr109;
	goto tr0;
tr117:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
#line 2146 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr110;
		case 34u: goto tr0;
		case 38u: goto tr111;
		case 39u: goto tr0;
		case 47u: goto tr112;
		case 60u: goto tr0;
		case 62u: goto tr113;
		case 88u: goto tr120;
		case 96u: goto tr0;
		case 120u: goto tr120;
	}
	if ( (*p) < 192u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr110;
		} else if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else
			goto tr119;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr115;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr116;
	} else
		goto tr114;
	goto tr109;
tr112:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st59;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
#line 2191 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr110;
		case 34u: goto tr0;
		case 38u: goto tr111;
		case 39u: goto tr0;
		case 47u: goto tr112;
		case 60u: goto tr0;
		case 62u: goto tr121;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto tr110;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr115;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr116;
	} else
		goto tr114;
	goto tr109;
tr106:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st60;
tr114:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st60;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
#line 2235 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st56;
	goto tr0;
tr107:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st61;
tr115:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st61;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
#line 2253 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st60;
	goto tr0;
tr108:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st62;
tr116:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st62;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
#line 2271 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st61;
	goto tr0;
tr125:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st63;
tr119:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st63;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
#line 2293 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr110;
		case 34u: goto tr0;
		case 38u: goto tr111;
		case 39u: goto tr0;
		case 47u: goto tr112;
		case 59u: goto tr126;
		case 60u: goto tr0;
		case 62u: goto tr113;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr110;
		} else if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else
			goto tr125;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr115;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr116;
	} else
		goto tr114;
	goto tr109;
tr120:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 2337 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr110;
		case 34u: goto tr0;
		case 38u: goto tr111;
		case 39u: goto tr0;
		case 47u: goto tr112;
		case 60u: goto tr0;
		case 62u: goto tr113;
		case 96u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr110;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 70u ) {
				if ( 97u <= (*p) && (*p) <= 102u )
					goto tr127;
			} else if ( (*p) >= 65u )
				goto tr127;
		} else
			goto tr127;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr114;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr116;
		} else
			goto tr115;
	} else
		goto tr0;
	goto tr109;
tr128:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st65;
tr127:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
#line 2394 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr110;
		case 34u: goto tr0;
		case 38u: goto tr111;
		case 39u: goto tr0;
		case 47u: goto tr112;
		case 59u: goto tr129;
		case 60u: goto tr0;
		case 62u: goto tr113;
		case 96u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr110;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 70u ) {
				if ( 97u <= (*p) && (*p) <= 102u )
					goto tr128;
			} else if ( (*p) >= 65u )
				goto tr128;
		} else
			goto tr128;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr114;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr116;
		} else
			goto tr115;
	} else
		goto tr0;
	goto tr109;
tr130:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st66;
tr118:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st66;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
#line 2452 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr110;
		case 38u: goto tr111;
		case 47u: goto tr112;
		case 58u: goto tr109;
		case 59u: goto tr131;
		case 62u: goto tr113;
	}
	if ( (*p) < 61u ) {
		if ( (*p) < 14u ) {
			if ( (*p) > 8u ) {
				if ( 9u <= (*p) && (*p) <= 13u )
					goto tr110;
			} else if ( (*p) >= 1u )
				goto tr109;
		} else if ( (*p) > 33u ) {
			if ( (*p) < 40u ) {
				if ( 35u <= (*p) && (*p) <= 37u )
					goto tr109;
			} else if ( (*p) > 46u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr130;
			} else
				goto tr109;
		} else
			goto tr109;
	} else if ( (*p) > 64u ) {
		if ( (*p) < 123u ) {
			if ( (*p) < 91u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr130;
			} else if ( (*p) > 95u ) {
				if ( 97u <= (*p) && (*p) <= 122u )
					goto tr130;
			} else
				goto tr109;
		} else if ( (*p) > 127u ) {
			if ( (*p) < 224u ) {
				if ( 192u <= (*p) && (*p) <= 223u )
					goto tr114;
			} else if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr116;
			} else
				goto tr115;
		} else
			goto tr109;
	} else
		goto tr109;
	goto tr0;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto st69;
		case 38u: goto tr134;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr135;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr137;
	} else
		goto tr136;
	goto tr132;
tr132:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st68;
tr138:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st68;
tr149:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st68;
tr155:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st68;
tr157:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st68;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
#line 2566 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr139;
		case 38u: goto tr140;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr141;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr143;
	} else
		goto tr142;
	goto tr138;
tr139:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
#line 2596 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st52;
		case 47u: goto st54;
		case 62u: goto st329;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st52;
	goto tr0;
tr134:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st70;
tr140:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st70;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
#line 2619 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 34u: goto tr139;
		case 35u: goto tr144;
		case 38u: goto tr140;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr145;
			} else if ( (*p) >= 1u )
				goto tr138;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr145;
			} else if ( (*p) >= 61u )
				goto tr138;
		} else
			goto tr138;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr138;
			} else if ( (*p) >= 97u )
				goto tr145;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr143;
			} else if ( (*p) >= 224u )
				goto tr142;
		} else
			goto tr141;
	} else
		goto tr138;
	goto tr0;
tr144:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 2668 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr139;
		case 38u: goto tr140;
		case 60u: goto tr0;
		case 88u: goto tr147;
		case 120u: goto tr147;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr146;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr142;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr143;
	} else
		goto tr141;
	goto tr138;
tr148:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st72;
tr146:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 2713 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr139;
		case 38u: goto tr140;
		case 59u: goto tr149;
		case 60u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr148;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr142;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr143;
	} else
		goto tr141;
	goto tr138;
tr135:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st73;
tr141:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
#line 2753 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st68;
	goto tr0;
tr136:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st74;
tr142:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 2771 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st73;
	goto tr0;
tr137:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st75;
tr143:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st75;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
#line 2789 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st74;
	goto tr0;
tr147:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
#line 2803 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr139;
		case 38u: goto tr140;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr153;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr153;
		} else
			goto tr153;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr141;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr143;
		} else
			goto tr142;
	} else
		goto tr0;
	goto tr138;
tr154:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st77;
tr153:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st77;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
#line 2852 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr139;
		case 38u: goto tr140;
		case 59u: goto tr155;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr154;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr154;
		} else
			goto tr154;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr141;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr143;
		} else
			goto tr142;
	} else
		goto tr0;
	goto tr138;
tr156:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st78;
tr145:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
#line 2902 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 34u: goto tr139;
		case 38u: goto tr140;
		case 58u: goto tr138;
		case 59u: goto tr157;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr138;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr156;
			} else if ( (*p) >= 61u )
				goto tr138;
		} else
			goto tr156;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr138;
			} else if ( (*p) >= 97u )
				goto tr156;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr143;
			} else if ( (*p) >= 224u )
				goto tr142;
		} else
			goto tr141;
	} else
		goto tr138;
	goto tr0;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr159;
		case 39u: goto st69;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr160;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr162;
	} else
		goto tr161;
	goto tr158;
tr158:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st80;
tr163:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st80;
tr173:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st80;
tr179:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st80;
tr181:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 3002 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr164;
		case 39u: goto tr139;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr165;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr167;
	} else
		goto tr166;
	goto tr163;
tr159:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st81;
tr164:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 3038 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 35u: goto tr168;
		case 38u: goto tr164;
		case 39u: goto tr139;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr169;
			} else if ( (*p) >= 1u )
				goto tr163;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr169;
			} else if ( (*p) >= 61u )
				goto tr163;
		} else
			goto tr163;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr163;
			} else if ( (*p) >= 97u )
				goto tr169;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr167;
			} else if ( (*p) >= 224u )
				goto tr166;
		} else
			goto tr165;
	} else
		goto tr163;
	goto tr0;
tr168:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 3087 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr164;
		case 39u: goto tr139;
		case 60u: goto tr0;
		case 88u: goto tr171;
		case 120u: goto tr171;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr170;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr166;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr167;
	} else
		goto tr165;
	goto tr163;
tr172:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st83;
tr170:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st83;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
#line 3132 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr164;
		case 39u: goto tr139;
		case 59u: goto tr173;
		case 60u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr172;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr166;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr167;
	} else
		goto tr165;
	goto tr163;
tr160:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st84;
tr165:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
#line 3172 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st80;
	goto tr0;
tr161:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st85;
tr166:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st85;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
#line 3190 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st84;
	goto tr0;
tr162:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st86;
tr167:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 3208 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st85;
	goto tr0;
tr171:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 3222 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr164;
		case 39u: goto tr139;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr177;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr177;
		} else
			goto tr177;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr165;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr167;
		} else
			goto tr166;
	} else
		goto tr0;
	goto tr163;
tr178:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st88;
tr177:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
#line 3271 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr164;
		case 39u: goto tr139;
		case 59u: goto tr179;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr178;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr178;
		} else
			goto tr178;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr165;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr167;
		} else
			goto tr166;
	} else
		goto tr0;
	goto tr163;
tr180:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st89;
tr169:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st89;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
#line 3321 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 38u: goto tr164;
		case 39u: goto tr139;
		case 58u: goto tr163;
		case 59u: goto tr181;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr163;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr180;
			} else if ( (*p) >= 61u )
				goto tr163;
		} else
			goto tr180;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr163;
			} else if ( (*p) >= 97u )
				goto tr180;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr167;
			} else if ( (*p) >= 224u )
				goto tr166;
		} else
			goto tr165;
	} else
		goto tr163;
	goto tr0;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	if ( (*p) == 63u )
		goto st91;
	goto st90;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case 62u: goto st320;
		case 63u: goto st91;
	}
	goto st90;
tr6:
#line 26 "../ragel/formats/html/html.rl"
	{ _open_tag_name_start(p); }
	goto st92;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
#line 3382 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 67u: goto st93;
		case 84u: goto st147;
		case 99u: goto st93;
		case 116u: goto st147;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 82u: goto st94;
		case 114u: goto st94;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 73u: goto st95;
		case 105u: goto st95;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 80u: goto st96;
		case 112u: goto st96;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 84u: goto st97;
		case 116u: goto st97;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	switch( (*p) ) {
		case 32u: goto tr189;
		case 47u: goto tr93;
		case 62u: goto tr190;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr189;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
tr189:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st98;
tr203:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
#line 3529 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st98;
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st54;
		case 61u: goto tr0;
		case 62u: goto st330;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st98;
	goto st99;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	switch( (*p) ) {
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st54;
		case 61u: goto st100;
		case 62u: goto st330;
	}
	goto st99;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto st100;
		case 34u: goto st124;
		case 38u: goto tr197;
		case 39u: goto st136;
		case 60u: goto tr0;
		case 62u: goto tr0;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto st100;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr200;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr201;
	} else
		goto tr199;
	goto tr195;
tr195:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st101;
tr202:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st101;
tr234:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st101;
tr237:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st101;
tr239:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st101;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
#line 3623 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr203;
		case 34u: goto tr0;
		case 38u: goto tr204;
		case 39u: goto tr0;
		case 47u: goto tr205;
		case 60u: goto tr0;
		case 62u: goto tr206;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto tr203;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr208;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr209;
	} else
		goto tr207;
	goto tr202;
tr197:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st102;
tr204:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st102;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
#line 3667 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr203;
		case 35u: goto tr210;
		case 38u: goto tr204;
		case 47u: goto tr205;
		case 62u: goto tr206;
	}
	if ( (*p) < 61u ) {
		if ( (*p) < 36u ) {
			if ( (*p) < 9u ) {
				if ( 1u <= (*p) && (*p) <= 8u )
					goto tr202;
			} else if ( (*p) > 13u ) {
				if ( 14u <= (*p) && (*p) <= 33u )
					goto tr202;
			} else
				goto tr203;
		} else if ( (*p) > 37u ) {
			if ( (*p) < 48u ) {
				if ( 40u <= (*p) && (*p) <= 46u )
					goto tr202;
			} else if ( (*p) > 57u ) {
				if ( 58u <= (*p) && (*p) <= 59u )
					goto tr202;
			} else
				goto tr211;
		} else
			goto tr202;
	} else if ( (*p) > 64u ) {
		if ( (*p) < 123u ) {
			if ( (*p) < 91u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr211;
			} else if ( (*p) > 95u ) {
				if ( 97u <= (*p) && (*p) <= 122u )
					goto tr211;
			} else
				goto tr202;
		} else if ( (*p) > 127u ) {
			if ( (*p) < 224u ) {
				if ( 192u <= (*p) && (*p) <= 223u )
					goto tr207;
			} else if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr209;
			} else
				goto tr208;
		} else
			goto tr202;
	} else
		goto tr202;
	goto tr0;
tr210:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st103;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
#line 3730 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr203;
		case 34u: goto tr0;
		case 38u: goto tr204;
		case 39u: goto tr0;
		case 47u: goto tr205;
		case 60u: goto tr0;
		case 62u: goto tr206;
		case 88u: goto tr213;
		case 96u: goto tr0;
		case 120u: goto tr213;
	}
	if ( (*p) < 192u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr203;
		} else if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else
			goto tr212;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr208;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr209;
	} else
		goto tr207;
	goto tr202;
tr205:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st104;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
#line 3775 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr203;
		case 34u: goto tr0;
		case 38u: goto tr204;
		case 39u: goto tr0;
		case 47u: goto tr205;
		case 60u: goto tr0;
		case 62u: goto tr214;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto tr203;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr208;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr209;
	} else
		goto tr207;
	goto tr202;
tr190:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st330;
tr206:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st330;
tr214:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 28 "../ragel/formats/html/html.rl"
	{ _open_tag_self_closing(); }
	goto st330;
st330:
	if ( ++p == pe )
		goto _test_eof330;
case 330:
#line 3823 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr679;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr680;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr682;
	} else
		goto tr681;
	goto tr678;
tr678:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st105;
tr215:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st105;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
#line 3857 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr216;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr217;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr219;
	} else
		goto tr218;
	goto tr215;
tr679:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st106;
tr216:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st106;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
#line 3891 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 47u: goto tr220;
		case 60u: goto tr216;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr217;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr219;
	} else
		goto tr218;
	goto tr215;
tr220:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st107;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
#line 3922 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr216;
		case 83u: goto tr221;
		case 115u: goto tr221;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr217;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr219;
	} else
		goto tr218;
	goto tr215;
tr221:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st108;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
#line 3954 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr216;
		case 67u: goto tr222;
		case 99u: goto tr222;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr217;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr219;
	} else
		goto tr218;
	goto tr215;
tr222:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st109;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
#line 3986 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr216;
		case 82u: goto tr223;
		case 114u: goto tr223;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr217;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr219;
	} else
		goto tr218;
	goto tr215;
tr223:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st110;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
#line 4018 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr216;
		case 73u: goto tr224;
		case 105u: goto tr224;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr217;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr219;
	} else
		goto tr218;
	goto tr215;
tr224:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st111;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
#line 4050 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr216;
		case 80u: goto tr225;
		case 112u: goto tr225;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr217;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr219;
	} else
		goto tr218;
	goto tr215;
tr225:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st112;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
#line 4082 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr216;
		case 84u: goto st113;
		case 116u: goto st113;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr217;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr219;
	} else
		goto tr218;
	goto tr215;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	switch( (*p) ) {
		case 32u: goto st113;
		case 62u: goto st320;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st113;
	goto tr0;
tr680:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st114;
tr217:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st114;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
#line 4129 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st105;
	goto tr0;
tr681:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st115;
tr218:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st115;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
#line 4147 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st114;
	goto tr0;
tr682:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st116;
tr219:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st116;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
#line 4165 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st115;
	goto tr0;
tr199:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st117;
tr207:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st117;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
#line 4183 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st101;
	goto tr0;
tr200:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st118;
tr208:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st118;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
#line 4201 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st117;
	goto tr0;
tr201:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st119;
tr209:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st119;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
#line 4219 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st118;
	goto tr0;
tr233:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st120;
tr212:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st120;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
#line 4241 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr203;
		case 34u: goto tr0;
		case 38u: goto tr204;
		case 39u: goto tr0;
		case 47u: goto tr205;
		case 59u: goto tr234;
		case 60u: goto tr0;
		case 62u: goto tr206;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr203;
		} else if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else
			goto tr233;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr208;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr209;
	} else
		goto tr207;
	goto tr202;
tr213:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st121;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
#line 4285 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr203;
		case 34u: goto tr0;
		case 38u: goto tr204;
		case 39u: goto tr0;
		case 47u: goto tr205;
		case 60u: goto tr0;
		case 62u: goto tr206;
		case 96u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr203;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 70u ) {
				if ( 97u <= (*p) && (*p) <= 102u )
					goto tr235;
			} else if ( (*p) >= 65u )
				goto tr235;
		} else
			goto tr235;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr207;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr209;
		} else
			goto tr208;
	} else
		goto tr0;
	goto tr202;
tr236:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st122;
tr235:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st122;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
#line 4342 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr203;
		case 34u: goto tr0;
		case 38u: goto tr204;
		case 39u: goto tr0;
		case 47u: goto tr205;
		case 59u: goto tr237;
		case 60u: goto tr0;
		case 62u: goto tr206;
		case 96u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr203;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 70u ) {
				if ( 97u <= (*p) && (*p) <= 102u )
					goto tr236;
			} else if ( (*p) >= 65u )
				goto tr236;
		} else
			goto tr236;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr207;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr209;
		} else
			goto tr208;
	} else
		goto tr0;
	goto tr202;
tr238:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st123;
tr211:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st123;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
#line 4400 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr203;
		case 38u: goto tr204;
		case 47u: goto tr205;
		case 58u: goto tr202;
		case 59u: goto tr239;
		case 62u: goto tr206;
	}
	if ( (*p) < 61u ) {
		if ( (*p) < 14u ) {
			if ( (*p) > 8u ) {
				if ( 9u <= (*p) && (*p) <= 13u )
					goto tr203;
			} else if ( (*p) >= 1u )
				goto tr202;
		} else if ( (*p) > 33u ) {
			if ( (*p) < 40u ) {
				if ( 35u <= (*p) && (*p) <= 37u )
					goto tr202;
			} else if ( (*p) > 46u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr238;
			} else
				goto tr202;
		} else
			goto tr202;
	} else if ( (*p) > 64u ) {
		if ( (*p) < 123u ) {
			if ( (*p) < 91u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr238;
			} else if ( (*p) > 95u ) {
				if ( 97u <= (*p) && (*p) <= 122u )
					goto tr238;
			} else
				goto tr202;
		} else if ( (*p) > 127u ) {
			if ( (*p) < 224u ) {
				if ( 192u <= (*p) && (*p) <= 223u )
					goto tr207;
			} else if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr209;
			} else
				goto tr208;
		} else
			goto tr202;
	} else
		goto tr202;
	goto tr0;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto st126;
		case 38u: goto tr242;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr243;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr245;
	} else
		goto tr244;
	goto tr240;
tr240:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st125;
tr246:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st125;
tr257:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st125;
tr263:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st125;
tr265:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st125;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
#line 4514 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr247;
		case 38u: goto tr248;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr249;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr251;
	} else
		goto tr250;
	goto tr246;
tr247:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st126;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
#line 4544 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st98;
		case 47u: goto st54;
		case 62u: goto st330;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st98;
	goto tr0;
tr242:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st127;
tr248:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st127;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
#line 4567 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 34u: goto tr247;
		case 35u: goto tr252;
		case 38u: goto tr248;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr253;
			} else if ( (*p) >= 1u )
				goto tr246;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr253;
			} else if ( (*p) >= 61u )
				goto tr246;
		} else
			goto tr246;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr246;
			} else if ( (*p) >= 97u )
				goto tr253;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr251;
			} else if ( (*p) >= 224u )
				goto tr250;
		} else
			goto tr249;
	} else
		goto tr246;
	goto tr0;
tr252:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st128;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
#line 4616 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr247;
		case 38u: goto tr248;
		case 60u: goto tr0;
		case 88u: goto tr255;
		case 120u: goto tr255;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr254;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr250;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr251;
	} else
		goto tr249;
	goto tr246;
tr256:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st129;
tr254:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st129;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
#line 4661 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr247;
		case 38u: goto tr248;
		case 59u: goto tr257;
		case 60u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr256;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr250;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr251;
	} else
		goto tr249;
	goto tr246;
tr243:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st130;
tr249:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st130;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
#line 4701 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st125;
	goto tr0;
tr244:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st131;
tr250:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st131;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
#line 4719 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st130;
	goto tr0;
tr245:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st132;
tr251:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st132;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
#line 4737 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st131;
	goto tr0;
tr255:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st133;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
#line 4751 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr247;
		case 38u: goto tr248;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr261;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr261;
		} else
			goto tr261;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr249;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr251;
		} else
			goto tr250;
	} else
		goto tr0;
	goto tr246;
tr262:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st134;
tr261:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st134;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
#line 4800 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr247;
		case 38u: goto tr248;
		case 59u: goto tr263;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr262;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr262;
		} else
			goto tr262;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr249;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr251;
		} else
			goto tr250;
	} else
		goto tr0;
	goto tr246;
tr264:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st135;
tr253:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st135;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
#line 4850 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 34u: goto tr247;
		case 38u: goto tr248;
		case 58u: goto tr246;
		case 59u: goto tr265;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr246;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr264;
			} else if ( (*p) >= 61u )
				goto tr246;
		} else
			goto tr264;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr246;
			} else if ( (*p) >= 97u )
				goto tr264;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr251;
			} else if ( (*p) >= 224u )
				goto tr250;
		} else
			goto tr249;
	} else
		goto tr246;
	goto tr0;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr267;
		case 39u: goto st126;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr268;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr270;
	} else
		goto tr269;
	goto tr266;
tr266:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st137;
tr271:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st137;
tr281:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st137;
tr287:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st137;
tr289:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st137;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
#line 4950 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr272;
		case 39u: goto tr247;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr273;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr275;
	} else
		goto tr274;
	goto tr271;
tr267:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st138;
tr272:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st138;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
#line 4986 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 35u: goto tr276;
		case 38u: goto tr272;
		case 39u: goto tr247;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr277;
			} else if ( (*p) >= 1u )
				goto tr271;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr277;
			} else if ( (*p) >= 61u )
				goto tr271;
		} else
			goto tr271;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr271;
			} else if ( (*p) >= 97u )
				goto tr277;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr275;
			} else if ( (*p) >= 224u )
				goto tr274;
		} else
			goto tr273;
	} else
		goto tr271;
	goto tr0;
tr276:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st139;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
#line 5035 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr272;
		case 39u: goto tr247;
		case 60u: goto tr0;
		case 88u: goto tr279;
		case 120u: goto tr279;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr278;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr274;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr275;
	} else
		goto tr273;
	goto tr271;
tr280:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st140;
tr278:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st140;
st140:
	if ( ++p == pe )
		goto _test_eof140;
case 140:
#line 5080 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr272;
		case 39u: goto tr247;
		case 59u: goto tr281;
		case 60u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr280;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr274;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr275;
	} else
		goto tr273;
	goto tr271;
tr268:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st141;
tr273:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st141;
st141:
	if ( ++p == pe )
		goto _test_eof141;
case 141:
#line 5120 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st137;
	goto tr0;
tr269:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st142;
tr274:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st142;
st142:
	if ( ++p == pe )
		goto _test_eof142;
case 142:
#line 5138 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st141;
	goto tr0;
tr270:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st143;
tr275:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st143;
st143:
	if ( ++p == pe )
		goto _test_eof143;
case 143:
#line 5156 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st142;
	goto tr0;
tr279:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st144;
st144:
	if ( ++p == pe )
		goto _test_eof144;
case 144:
#line 5170 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr272;
		case 39u: goto tr247;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr285;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr285;
		} else
			goto tr285;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr273;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr275;
		} else
			goto tr274;
	} else
		goto tr0;
	goto tr271;
tr286:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st145;
tr285:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st145;
st145:
	if ( ++p == pe )
		goto _test_eof145;
case 145:
#line 5219 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr272;
		case 39u: goto tr247;
		case 59u: goto tr287;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr286;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr286;
		} else
			goto tr286;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr273;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr275;
		} else
			goto tr274;
	} else
		goto tr0;
	goto tr271;
tr288:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st146;
tr277:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st146;
st146:
	if ( ++p == pe )
		goto _test_eof146;
case 146:
#line 5269 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 38u: goto tr272;
		case 39u: goto tr247;
		case 58u: goto tr271;
		case 59u: goto tr289;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr271;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr288;
			} else if ( (*p) >= 61u )
				goto tr271;
		} else
			goto tr288;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr271;
			} else if ( (*p) >= 97u )
				goto tr288;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr275;
			} else if ( (*p) >= 224u )
				goto tr274;
		} else
			goto tr273;
	} else
		goto tr271;
	goto tr0;
st147:
	if ( ++p == pe )
		goto _test_eof147;
case 147:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 89u: goto st148;
		case 121u: goto st148;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st148:
	if ( ++p == pe )
		goto _test_eof148;
case 148:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 76u: goto st149;
		case 108u: goto st149;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st149:
	if ( ++p == pe )
		goto _test_eof149;
case 149:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 69u: goto st150;
		case 101u: goto st150;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st150:
	if ( ++p == pe )
		goto _test_eof150;
case 150:
	switch( (*p) ) {
		case 32u: goto tr293;
		case 47u: goto tr93;
		case 62u: goto tr294;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr293;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
tr293:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st151;
tr307:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st151;
st151:
	if ( ++p == pe )
		goto _test_eof151;
case 151:
#line 5408 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st151;
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st54;
		case 61u: goto tr0;
		case 62u: goto st331;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st151;
	goto st152;
st152:
	if ( ++p == pe )
		goto _test_eof152;
case 152:
	switch( (*p) ) {
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st54;
		case 61u: goto st153;
		case 62u: goto st331;
	}
	goto st152;
st153:
	if ( ++p == pe )
		goto _test_eof153;
case 153:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto st153;
		case 34u: goto st175;
		case 38u: goto tr301;
		case 39u: goto st187;
		case 60u: goto tr0;
		case 62u: goto tr0;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto st153;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr304;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr305;
	} else
		goto tr303;
	goto tr299;
tr299:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st154;
tr306:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st154;
tr336:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st154;
tr339:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st154;
tr341:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st154;
st154:
	if ( ++p == pe )
		goto _test_eof154;
case 154:
#line 5502 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr307;
		case 34u: goto tr0;
		case 38u: goto tr308;
		case 39u: goto tr0;
		case 47u: goto tr309;
		case 60u: goto tr0;
		case 62u: goto tr310;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto tr307;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr312;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr313;
	} else
		goto tr311;
	goto tr306;
tr301:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st155;
tr308:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st155;
st155:
	if ( ++p == pe )
		goto _test_eof155;
case 155:
#line 5546 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr307;
		case 35u: goto tr314;
		case 38u: goto tr308;
		case 47u: goto tr309;
		case 62u: goto tr310;
	}
	if ( (*p) < 61u ) {
		if ( (*p) < 36u ) {
			if ( (*p) < 9u ) {
				if ( 1u <= (*p) && (*p) <= 8u )
					goto tr306;
			} else if ( (*p) > 13u ) {
				if ( 14u <= (*p) && (*p) <= 33u )
					goto tr306;
			} else
				goto tr307;
		} else if ( (*p) > 37u ) {
			if ( (*p) < 48u ) {
				if ( 40u <= (*p) && (*p) <= 46u )
					goto tr306;
			} else if ( (*p) > 57u ) {
				if ( 58u <= (*p) && (*p) <= 59u )
					goto tr306;
			} else
				goto tr315;
		} else
			goto tr306;
	} else if ( (*p) > 64u ) {
		if ( (*p) < 123u ) {
			if ( (*p) < 91u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr315;
			} else if ( (*p) > 95u ) {
				if ( 97u <= (*p) && (*p) <= 122u )
					goto tr315;
			} else
				goto tr306;
		} else if ( (*p) > 127u ) {
			if ( (*p) < 224u ) {
				if ( 192u <= (*p) && (*p) <= 223u )
					goto tr311;
			} else if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr313;
			} else
				goto tr312;
		} else
			goto tr306;
	} else
		goto tr306;
	goto tr0;
tr314:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st156;
st156:
	if ( ++p == pe )
		goto _test_eof156;
case 156:
#line 5609 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr307;
		case 34u: goto tr0;
		case 38u: goto tr308;
		case 39u: goto tr0;
		case 47u: goto tr309;
		case 60u: goto tr0;
		case 62u: goto tr310;
		case 88u: goto tr317;
		case 96u: goto tr0;
		case 120u: goto tr317;
	}
	if ( (*p) < 192u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr307;
		} else if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else
			goto tr316;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr312;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr313;
	} else
		goto tr311;
	goto tr306;
tr309:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st157;
st157:
	if ( ++p == pe )
		goto _test_eof157;
case 157:
#line 5654 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr307;
		case 34u: goto tr0;
		case 38u: goto tr308;
		case 39u: goto tr0;
		case 47u: goto tr309;
		case 60u: goto tr0;
		case 62u: goto tr318;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto tr307;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr312;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr313;
	} else
		goto tr311;
	goto tr306;
tr294:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st331;
tr310:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st331;
tr318:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 28 "../ragel/formats/html/html.rl"
	{ _open_tag_self_closing(); }
	goto st331;
st331:
	if ( ++p == pe )
		goto _test_eof331;
case 331:
#line 5702 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr684;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr685;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr687;
	} else
		goto tr686;
	goto tr683;
tr683:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st158;
tr319:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st158;
st158:
	if ( ++p == pe )
		goto _test_eof158;
case 158:
#line 5736 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr320;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr321;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr323;
	} else
		goto tr322;
	goto tr319;
tr684:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st159;
tr320:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st159;
st159:
	if ( ++p == pe )
		goto _test_eof159;
case 159:
#line 5770 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 47u: goto tr324;
		case 60u: goto tr320;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr321;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr323;
	} else
		goto tr322;
	goto tr319;
tr324:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st160;
st160:
	if ( ++p == pe )
		goto _test_eof160;
case 160:
#line 5801 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr320;
		case 83u: goto tr325;
		case 115u: goto tr325;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr321;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr323;
	} else
		goto tr322;
	goto tr319;
tr325:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st161;
st161:
	if ( ++p == pe )
		goto _test_eof161;
case 161:
#line 5833 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr320;
		case 84u: goto tr326;
		case 116u: goto tr326;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr321;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr323;
	} else
		goto tr322;
	goto tr319;
tr326:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st162;
st162:
	if ( ++p == pe )
		goto _test_eof162;
case 162:
#line 5865 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr320;
		case 89u: goto tr327;
		case 121u: goto tr327;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr321;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr323;
	} else
		goto tr322;
	goto tr319;
tr327:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st163;
st163:
	if ( ++p == pe )
		goto _test_eof163;
case 163:
#line 5897 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr320;
		case 76u: goto tr328;
		case 108u: goto tr328;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr321;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr323;
	} else
		goto tr322;
	goto tr319;
tr328:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st164;
st164:
	if ( ++p == pe )
		goto _test_eof164;
case 164:
#line 5929 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 60u: goto tr320;
		case 69u: goto st113;
		case 101u: goto st113;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr321;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr323;
	} else
		goto tr322;
	goto tr319;
tr685:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st165;
tr321:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st165;
st165:
	if ( ++p == pe )
		goto _test_eof165;
case 165:
#line 5965 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st158;
	goto tr0;
tr686:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st166;
tr322:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st166;
st166:
	if ( ++p == pe )
		goto _test_eof166;
case 166:
#line 5983 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st165;
	goto tr0;
tr687:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st167;
tr323:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st167;
st167:
	if ( ++p == pe )
		goto _test_eof167;
case 167:
#line 6001 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st166;
	goto tr0;
tr303:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st168;
tr311:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st168;
st168:
	if ( ++p == pe )
		goto _test_eof168;
case 168:
#line 6019 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st154;
	goto tr0;
tr304:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st169;
tr312:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st169;
st169:
	if ( ++p == pe )
		goto _test_eof169;
case 169:
#line 6037 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st168;
	goto tr0;
tr305:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st170;
tr313:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st170;
st170:
	if ( ++p == pe )
		goto _test_eof170;
case 170:
#line 6055 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st169;
	goto tr0;
tr335:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st171;
tr316:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st171;
st171:
	if ( ++p == pe )
		goto _test_eof171;
case 171:
#line 6077 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr307;
		case 34u: goto tr0;
		case 38u: goto tr308;
		case 39u: goto tr0;
		case 47u: goto tr309;
		case 59u: goto tr336;
		case 60u: goto tr0;
		case 62u: goto tr310;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr307;
		} else if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else
			goto tr335;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr312;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr313;
	} else
		goto tr311;
	goto tr306;
tr317:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st172;
st172:
	if ( ++p == pe )
		goto _test_eof172;
case 172:
#line 6121 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr307;
		case 34u: goto tr0;
		case 38u: goto tr308;
		case 39u: goto tr0;
		case 47u: goto tr309;
		case 60u: goto tr0;
		case 62u: goto tr310;
		case 96u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr307;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 70u ) {
				if ( 97u <= (*p) && (*p) <= 102u )
					goto tr337;
			} else if ( (*p) >= 65u )
				goto tr337;
		} else
			goto tr337;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr311;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr313;
		} else
			goto tr312;
	} else
		goto tr0;
	goto tr306;
tr338:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st173;
tr337:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st173;
st173:
	if ( ++p == pe )
		goto _test_eof173;
case 173:
#line 6178 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr307;
		case 34u: goto tr0;
		case 38u: goto tr308;
		case 39u: goto tr0;
		case 47u: goto tr309;
		case 59u: goto tr339;
		case 60u: goto tr0;
		case 62u: goto tr310;
		case 96u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr307;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 70u ) {
				if ( 97u <= (*p) && (*p) <= 102u )
					goto tr338;
			} else if ( (*p) >= 65u )
				goto tr338;
		} else
			goto tr338;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr311;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr313;
		} else
			goto tr312;
	} else
		goto tr0;
	goto tr306;
tr340:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st174;
tr315:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st174;
st174:
	if ( ++p == pe )
		goto _test_eof174;
case 174:
#line 6236 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr307;
		case 38u: goto tr308;
		case 47u: goto tr309;
		case 58u: goto tr306;
		case 59u: goto tr341;
		case 62u: goto tr310;
	}
	if ( (*p) < 61u ) {
		if ( (*p) < 14u ) {
			if ( (*p) > 8u ) {
				if ( 9u <= (*p) && (*p) <= 13u )
					goto tr307;
			} else if ( (*p) >= 1u )
				goto tr306;
		} else if ( (*p) > 33u ) {
			if ( (*p) < 40u ) {
				if ( 35u <= (*p) && (*p) <= 37u )
					goto tr306;
			} else if ( (*p) > 46u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr340;
			} else
				goto tr306;
		} else
			goto tr306;
	} else if ( (*p) > 64u ) {
		if ( (*p) < 123u ) {
			if ( (*p) < 91u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr340;
			} else if ( (*p) > 95u ) {
				if ( 97u <= (*p) && (*p) <= 122u )
					goto tr340;
			} else
				goto tr306;
		} else if ( (*p) > 127u ) {
			if ( (*p) < 224u ) {
				if ( 192u <= (*p) && (*p) <= 223u )
					goto tr311;
			} else if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr313;
			} else
				goto tr312;
		} else
			goto tr306;
	} else
		goto tr306;
	goto tr0;
st175:
	if ( ++p == pe )
		goto _test_eof175;
case 175:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto st177;
		case 38u: goto tr344;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr345;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr347;
	} else
		goto tr346;
	goto tr342;
tr342:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st176;
tr348:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st176;
tr359:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st176;
tr365:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st176;
tr367:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st176;
st176:
	if ( ++p == pe )
		goto _test_eof176;
case 176:
#line 6350 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr349;
		case 38u: goto tr350;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr351;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr353;
	} else
		goto tr352;
	goto tr348;
tr349:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st177;
st177:
	if ( ++p == pe )
		goto _test_eof177;
case 177:
#line 6380 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st151;
		case 47u: goto st54;
		case 62u: goto st331;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st151;
	goto tr0;
tr344:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st178;
tr350:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st178;
st178:
	if ( ++p == pe )
		goto _test_eof178;
case 178:
#line 6403 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 34u: goto tr349;
		case 35u: goto tr354;
		case 38u: goto tr350;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr355;
			} else if ( (*p) >= 1u )
				goto tr348;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr355;
			} else if ( (*p) >= 61u )
				goto tr348;
		} else
			goto tr348;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr348;
			} else if ( (*p) >= 97u )
				goto tr355;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr353;
			} else if ( (*p) >= 224u )
				goto tr352;
		} else
			goto tr351;
	} else
		goto tr348;
	goto tr0;
tr354:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st179;
st179:
	if ( ++p == pe )
		goto _test_eof179;
case 179:
#line 6452 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr349;
		case 38u: goto tr350;
		case 60u: goto tr0;
		case 88u: goto tr357;
		case 120u: goto tr357;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr356;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr352;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr353;
	} else
		goto tr351;
	goto tr348;
tr358:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st180;
tr356:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st180;
st180:
	if ( ++p == pe )
		goto _test_eof180;
case 180:
#line 6497 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr349;
		case 38u: goto tr350;
		case 59u: goto tr359;
		case 60u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr358;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr352;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr353;
	} else
		goto tr351;
	goto tr348;
tr345:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st181;
tr351:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st181;
st181:
	if ( ++p == pe )
		goto _test_eof181;
case 181:
#line 6537 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st176;
	goto tr0;
tr346:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st182;
tr352:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st182;
st182:
	if ( ++p == pe )
		goto _test_eof182;
case 182:
#line 6555 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st181;
	goto tr0;
tr347:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st183;
tr353:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st183;
st183:
	if ( ++p == pe )
		goto _test_eof183;
case 183:
#line 6573 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st182;
	goto tr0;
tr357:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st184;
st184:
	if ( ++p == pe )
		goto _test_eof184;
case 184:
#line 6587 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr349;
		case 38u: goto tr350;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr363;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr363;
		} else
			goto tr363;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr351;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr353;
		} else
			goto tr352;
	} else
		goto tr0;
	goto tr348;
tr364:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st185;
tr363:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st185;
st185:
	if ( ++p == pe )
		goto _test_eof185;
case 185:
#line 6636 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr349;
		case 38u: goto tr350;
		case 59u: goto tr365;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr364;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr364;
		} else
			goto tr364;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr351;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr353;
		} else
			goto tr352;
	} else
		goto tr0;
	goto tr348;
tr366:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st186;
tr355:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st186;
st186:
	if ( ++p == pe )
		goto _test_eof186;
case 186:
#line 6686 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 34u: goto tr349;
		case 38u: goto tr350;
		case 58u: goto tr348;
		case 59u: goto tr367;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr348;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr366;
			} else if ( (*p) >= 61u )
				goto tr348;
		} else
			goto tr366;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr348;
			} else if ( (*p) >= 97u )
				goto tr366;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr353;
			} else if ( (*p) >= 224u )
				goto tr352;
		} else
			goto tr351;
	} else
		goto tr348;
	goto tr0;
st187:
	if ( ++p == pe )
		goto _test_eof187;
case 187:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr369;
		case 39u: goto st177;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr370;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr372;
	} else
		goto tr371;
	goto tr368;
tr368:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st188;
tr373:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st188;
tr383:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st188;
tr389:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st188;
tr391:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st188;
st188:
	if ( ++p == pe )
		goto _test_eof188;
case 188:
#line 6786 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr374;
		case 39u: goto tr349;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr375;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr377;
	} else
		goto tr376;
	goto tr373;
tr369:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st189;
tr374:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st189;
st189:
	if ( ++p == pe )
		goto _test_eof189;
case 189:
#line 6822 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 35u: goto tr378;
		case 38u: goto tr374;
		case 39u: goto tr349;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr379;
			} else if ( (*p) >= 1u )
				goto tr373;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr379;
			} else if ( (*p) >= 61u )
				goto tr373;
		} else
			goto tr373;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr373;
			} else if ( (*p) >= 97u )
				goto tr379;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr377;
			} else if ( (*p) >= 224u )
				goto tr376;
		} else
			goto tr375;
	} else
		goto tr373;
	goto tr0;
tr378:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st190;
st190:
	if ( ++p == pe )
		goto _test_eof190;
case 190:
#line 6871 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr374;
		case 39u: goto tr349;
		case 60u: goto tr0;
		case 88u: goto tr381;
		case 120u: goto tr381;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr380;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr376;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr377;
	} else
		goto tr375;
	goto tr373;
tr382:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st191;
tr380:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st191;
st191:
	if ( ++p == pe )
		goto _test_eof191;
case 191:
#line 6916 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr374;
		case 39u: goto tr349;
		case 59u: goto tr383;
		case 60u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr382;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr376;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr377;
	} else
		goto tr375;
	goto tr373;
tr370:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st192;
tr375:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st192;
st192:
	if ( ++p == pe )
		goto _test_eof192;
case 192:
#line 6956 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st188;
	goto tr0;
tr371:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st193;
tr376:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st193;
st193:
	if ( ++p == pe )
		goto _test_eof193;
case 193:
#line 6974 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st192;
	goto tr0;
tr372:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st194;
tr377:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st194;
st194:
	if ( ++p == pe )
		goto _test_eof194;
case 194:
#line 6992 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st193;
	goto tr0;
tr381:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st195;
st195:
	if ( ++p == pe )
		goto _test_eof195;
case 195:
#line 7006 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr374;
		case 39u: goto tr349;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr387;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr387;
		} else
			goto tr387;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr375;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr377;
		} else
			goto tr376;
	} else
		goto tr0;
	goto tr373;
tr388:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st196;
tr387:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st196;
st196:
	if ( ++p == pe )
		goto _test_eof196;
case 196:
#line 7055 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr374;
		case 39u: goto tr349;
		case 59u: goto tr389;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr388;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr388;
		} else
			goto tr388;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr375;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr377;
		} else
			goto tr376;
	} else
		goto tr0;
	goto tr373;
tr390:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st197;
tr379:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st197;
st197:
	if ( ++p == pe )
		goto _test_eof197;
case 197:
#line 7105 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 38u: goto tr374;
		case 39u: goto tr349;
		case 58u: goto tr373;
		case 59u: goto tr391;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr373;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr390;
			} else if ( (*p) >= 61u )
				goto tr373;
		} else
			goto tr390;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr373;
			} else if ( (*p) >= 97u )
				goto tr390;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr377;
			} else if ( (*p) >= 224u )
				goto tr376;
		} else
			goto tr375;
	} else
		goto tr373;
	goto tr0;
tr7:
#line 26 "../ragel/formats/html/html.rl"
	{ _open_tag_name_start(p); }
	goto st198;
st198:
	if ( ++p == pe )
		goto _test_eof198;
case 198:
#line 7150 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 69u: goto st199;
		case 73u: goto st262;
		case 101u: goto st199;
		case 105u: goto st262;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st199:
	if ( ++p == pe )
		goto _test_eof199;
case 199:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 88u: goto st200;
		case 120u: goto st200;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st200:
	if ( ++p == pe )
		goto _test_eof200;
case 200:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 84u: goto st201;
		case 116u: goto st201;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st201:
	if ( ++p == pe )
		goto _test_eof201;
case 201:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 65u: goto st202;
		case 97u: goto st202;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 98u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 66u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st202:
	if ( ++p == pe )
		goto _test_eof202;
case 202:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 82u: goto st203;
		case 114u: goto st203;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st203:
	if ( ++p == pe )
		goto _test_eof203;
case 203:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 69u: goto st204;
		case 101u: goto st204;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st204:
	if ( ++p == pe )
		goto _test_eof204;
case 204:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 65u: goto st205;
		case 97u: goto st205;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 98u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 66u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st205:
	if ( ++p == pe )
		goto _test_eof205;
case 205:
	switch( (*p) ) {
		case 32u: goto tr400;
		case 47u: goto tr93;
		case 62u: goto tr401;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr400;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
tr400:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st206;
tr414:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st206;
st206:
	if ( ++p == pe )
		goto _test_eof206;
case 206:
#line 7343 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st206;
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st54;
		case 61u: goto tr0;
		case 62u: goto st332;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st206;
	goto st207;
st207:
	if ( ++p == pe )
		goto _test_eof207;
case 207:
	switch( (*p) ) {
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st54;
		case 61u: goto st208;
		case 62u: goto st332;
	}
	goto st207;
st208:
	if ( ++p == pe )
		goto _test_eof208;
case 208:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto st208;
		case 34u: goto st239;
		case 38u: goto tr408;
		case 39u: goto st251;
		case 60u: goto tr0;
		case 62u: goto tr0;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto st208;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr411;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr412;
	} else
		goto tr410;
	goto tr406;
tr406:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st209;
tr413:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st209;
tr458:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st209;
tr461:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st209;
tr463:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st209;
st209:
	if ( ++p == pe )
		goto _test_eof209;
case 209:
#line 7437 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr414;
		case 34u: goto tr0;
		case 38u: goto tr415;
		case 39u: goto tr0;
		case 47u: goto tr416;
		case 60u: goto tr0;
		case 62u: goto tr417;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto tr414;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr419;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr420;
	} else
		goto tr418;
	goto tr413;
tr408:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st210;
tr415:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st210;
st210:
	if ( ++p == pe )
		goto _test_eof210;
case 210:
#line 7481 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr414;
		case 35u: goto tr421;
		case 38u: goto tr415;
		case 47u: goto tr416;
		case 62u: goto tr417;
	}
	if ( (*p) < 61u ) {
		if ( (*p) < 36u ) {
			if ( (*p) < 9u ) {
				if ( 1u <= (*p) && (*p) <= 8u )
					goto tr413;
			} else if ( (*p) > 13u ) {
				if ( 14u <= (*p) && (*p) <= 33u )
					goto tr413;
			} else
				goto tr414;
		} else if ( (*p) > 37u ) {
			if ( (*p) < 48u ) {
				if ( 40u <= (*p) && (*p) <= 46u )
					goto tr413;
			} else if ( (*p) > 57u ) {
				if ( 58u <= (*p) && (*p) <= 59u )
					goto tr413;
			} else
				goto tr422;
		} else
			goto tr413;
	} else if ( (*p) > 64u ) {
		if ( (*p) < 123u ) {
			if ( (*p) < 91u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr422;
			} else if ( (*p) > 95u ) {
				if ( 97u <= (*p) && (*p) <= 122u )
					goto tr422;
			} else
				goto tr413;
		} else if ( (*p) > 127u ) {
			if ( (*p) < 224u ) {
				if ( 192u <= (*p) && (*p) <= 223u )
					goto tr418;
			} else if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr420;
			} else
				goto tr419;
		} else
			goto tr413;
	} else
		goto tr413;
	goto tr0;
tr421:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st211;
st211:
	if ( ++p == pe )
		goto _test_eof211;
case 211:
#line 7544 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr414;
		case 34u: goto tr0;
		case 38u: goto tr415;
		case 39u: goto tr0;
		case 47u: goto tr416;
		case 60u: goto tr0;
		case 62u: goto tr417;
		case 88u: goto tr424;
		case 96u: goto tr0;
		case 120u: goto tr424;
	}
	if ( (*p) < 192u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr414;
		} else if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else
			goto tr423;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr419;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr420;
	} else
		goto tr418;
	goto tr413;
tr416:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st212;
st212:
	if ( ++p == pe )
		goto _test_eof212;
case 212:
#line 7589 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr414;
		case 34u: goto tr0;
		case 38u: goto tr415;
		case 39u: goto tr0;
		case 47u: goto tr416;
		case 60u: goto tr0;
		case 62u: goto tr425;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto tr414;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr419;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr420;
	} else
		goto tr418;
	goto tr413;
tr401:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st332;
tr417:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st332;
tr425:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 28 "../ragel/formats/html/html.rl"
	{ _open_tag_self_closing(); }
	goto st332;
st332:
	if ( ++p == pe )
		goto _test_eof332;
case 332:
#line 7637 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr689;
		case 60u: goto tr690;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr691;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr693;
	} else
		goto tr692;
	goto tr688;
tr688:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st213;
tr426:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st213;
tr437:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st213;
tr451:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st213;
tr453:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st213;
st213:
	if ( ++p == pe )
		goto _test_eof213;
case 213:
#line 7696 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr431;
	} else
		goto tr430;
	goto tr426;
tr689:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st214;
tr427:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st214;
st214:
	if ( ++p == pe )
		goto _test_eof214;
case 214:
#line 7731 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 35u: goto tr432;
		case 38u: goto tr427;
		case 60u: goto tr428;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr426;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr433;
			} else if ( (*p) >= 58u )
				goto tr426;
		} else
			goto tr433;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr426;
			} else if ( (*p) >= 97u )
				goto tr433;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr431;
			} else if ( (*p) >= 224u )
				goto tr430;
		} else
			goto tr429;
	} else
		goto tr426;
	goto tr0;
tr432:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st215;
st215:
	if ( ++p == pe )
		goto _test_eof215;
case 215:
#line 7777 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
		case 88u: goto tr435;
		case 120u: goto tr435;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr434;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr430;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr431;
	} else
		goto tr429;
	goto tr426;
tr436:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st216;
tr434:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st216;
st216:
	if ( ++p == pe )
		goto _test_eof216;
case 216:
#line 7821 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 59u: goto tr437;
		case 60u: goto tr428;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr436;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr430;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr431;
	} else
		goto tr429;
	goto tr426;
tr690:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st217;
tr428:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st217;
st217:
	if ( ++p == pe )
		goto _test_eof217;
case 217:
#line 7860 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 47u: goto tr438;
		case 60u: goto tr428;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr431;
	} else
		goto tr430;
	goto tr426;
tr438:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st218;
st218:
	if ( ++p == pe )
		goto _test_eof218;
case 218:
#line 7892 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
		case 84u: goto tr439;
		case 116u: goto tr439;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr431;
	} else
		goto tr430;
	goto tr426;
tr439:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st219;
st219:
	if ( ++p == pe )
		goto _test_eof219;
case 219:
#line 7925 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
		case 69u: goto tr440;
		case 101u: goto tr440;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr431;
	} else
		goto tr430;
	goto tr426;
tr440:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st220;
st220:
	if ( ++p == pe )
		goto _test_eof220;
case 220:
#line 7958 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
		case 88u: goto tr441;
		case 120u: goto tr441;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr431;
	} else
		goto tr430;
	goto tr426;
tr441:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st221;
st221:
	if ( ++p == pe )
		goto _test_eof221;
case 221:
#line 7991 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
		case 84u: goto tr442;
		case 116u: goto tr442;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr431;
	} else
		goto tr430;
	goto tr426;
tr442:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st222;
st222:
	if ( ++p == pe )
		goto _test_eof222;
case 222:
#line 8024 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
		case 65u: goto tr443;
		case 97u: goto tr443;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr431;
	} else
		goto tr430;
	goto tr426;
tr443:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st223;
st223:
	if ( ++p == pe )
		goto _test_eof223;
case 223:
#line 8057 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
		case 82u: goto tr444;
		case 114u: goto tr444;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr431;
	} else
		goto tr430;
	goto tr426;
tr444:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st224;
st224:
	if ( ++p == pe )
		goto _test_eof224;
case 224:
#line 8090 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
		case 69u: goto tr445;
		case 101u: goto tr445;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr431;
	} else
		goto tr430;
	goto tr426;
tr445:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st225;
st225:
	if ( ++p == pe )
		goto _test_eof225;
case 225:
#line 8123 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
		case 65u: goto st113;
		case 97u: goto st113;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr431;
	} else
		goto tr430;
	goto tr426;
tr691:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st226;
tr429:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st226;
st226:
	if ( ++p == pe )
		goto _test_eof226;
case 226:
#line 8160 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st213;
	goto tr0;
tr692:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st227;
tr430:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st227;
st227:
	if ( ++p == pe )
		goto _test_eof227;
case 227:
#line 8178 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st226;
	goto tr0;
tr693:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st228;
tr431:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st228;
st228:
	if ( ++p == pe )
		goto _test_eof228;
case 228:
#line 8196 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st227;
	goto tr0;
tr435:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st229;
st229:
	if ( ++p == pe )
		goto _test_eof229;
case 229:
#line 8210 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 60u: goto tr428;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr449;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr449;
		} else
			goto tr449;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr431;
		} else
			goto tr430;
	} else
		goto tr0;
	goto tr426;
tr450:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st230;
tr449:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st230;
st230:
	if ( ++p == pe )
		goto _test_eof230;
case 230:
#line 8258 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr427;
		case 59u: goto tr451;
		case 60u: goto tr428;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr450;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr450;
		} else
			goto tr450;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr429;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr431;
		} else
			goto tr430;
	} else
		goto tr0;
	goto tr426;
tr452:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st231;
tr433:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st231;
st231:
	if ( ++p == pe )
		goto _test_eof231;
case 231:
#line 8307 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 38u: goto tr427;
		case 59u: goto tr453;
		case 60u: goto tr428;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr426;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr452;
			} else if ( (*p) >= 58u )
				goto tr426;
		} else
			goto tr452;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr426;
			} else if ( (*p) >= 97u )
				goto tr452;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr431;
			} else if ( (*p) >= 224u )
				goto tr430;
		} else
			goto tr429;
	} else
		goto tr426;
	goto tr0;
tr410:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st232;
tr418:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st232;
st232:
	if ( ++p == pe )
		goto _test_eof232;
case 232:
#line 8357 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st209;
	goto tr0;
tr411:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st233;
tr419:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st233;
st233:
	if ( ++p == pe )
		goto _test_eof233;
case 233:
#line 8375 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st232;
	goto tr0;
tr412:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st234;
tr420:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st234;
st234:
	if ( ++p == pe )
		goto _test_eof234;
case 234:
#line 8393 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st233;
	goto tr0;
tr457:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st235;
tr423:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st235;
st235:
	if ( ++p == pe )
		goto _test_eof235;
case 235:
#line 8415 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr414;
		case 34u: goto tr0;
		case 38u: goto tr415;
		case 39u: goto tr0;
		case 47u: goto tr416;
		case 59u: goto tr458;
		case 60u: goto tr0;
		case 62u: goto tr417;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr414;
		} else if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else
			goto tr457;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr419;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr420;
	} else
		goto tr418;
	goto tr413;
tr424:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st236;
st236:
	if ( ++p == pe )
		goto _test_eof236;
case 236:
#line 8459 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr414;
		case 34u: goto tr0;
		case 38u: goto tr415;
		case 39u: goto tr0;
		case 47u: goto tr416;
		case 60u: goto tr0;
		case 62u: goto tr417;
		case 96u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr414;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 70u ) {
				if ( 97u <= (*p) && (*p) <= 102u )
					goto tr459;
			} else if ( (*p) >= 65u )
				goto tr459;
		} else
			goto tr459;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr418;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr420;
		} else
			goto tr419;
	} else
		goto tr0;
	goto tr413;
tr460:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st237;
tr459:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st237;
st237:
	if ( ++p == pe )
		goto _test_eof237;
case 237:
#line 8516 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr414;
		case 34u: goto tr0;
		case 38u: goto tr415;
		case 39u: goto tr0;
		case 47u: goto tr416;
		case 59u: goto tr461;
		case 60u: goto tr0;
		case 62u: goto tr417;
		case 96u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr414;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 70u ) {
				if ( 97u <= (*p) && (*p) <= 102u )
					goto tr460;
			} else if ( (*p) >= 65u )
				goto tr460;
		} else
			goto tr460;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr418;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr420;
		} else
			goto tr419;
	} else
		goto tr0;
	goto tr413;
tr462:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st238;
tr422:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st238;
st238:
	if ( ++p == pe )
		goto _test_eof238;
case 238:
#line 8574 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr414;
		case 38u: goto tr415;
		case 47u: goto tr416;
		case 58u: goto tr413;
		case 59u: goto tr463;
		case 62u: goto tr417;
	}
	if ( (*p) < 61u ) {
		if ( (*p) < 14u ) {
			if ( (*p) > 8u ) {
				if ( 9u <= (*p) && (*p) <= 13u )
					goto tr414;
			} else if ( (*p) >= 1u )
				goto tr413;
		} else if ( (*p) > 33u ) {
			if ( (*p) < 40u ) {
				if ( 35u <= (*p) && (*p) <= 37u )
					goto tr413;
			} else if ( (*p) > 46u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr462;
			} else
				goto tr413;
		} else
			goto tr413;
	} else if ( (*p) > 64u ) {
		if ( (*p) < 123u ) {
			if ( (*p) < 91u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr462;
			} else if ( (*p) > 95u ) {
				if ( 97u <= (*p) && (*p) <= 122u )
					goto tr462;
			} else
				goto tr413;
		} else if ( (*p) > 127u ) {
			if ( (*p) < 224u ) {
				if ( 192u <= (*p) && (*p) <= 223u )
					goto tr418;
			} else if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr420;
			} else
				goto tr419;
		} else
			goto tr413;
	} else
		goto tr413;
	goto tr0;
st239:
	if ( ++p == pe )
		goto _test_eof239;
case 239:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto st241;
		case 38u: goto tr466;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr467;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr469;
	} else
		goto tr468;
	goto tr464;
tr464:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st240;
tr470:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st240;
tr481:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st240;
tr487:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st240;
tr489:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st240;
st240:
	if ( ++p == pe )
		goto _test_eof240;
case 240:
#line 8688 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr471;
		case 38u: goto tr472;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr473;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr475;
	} else
		goto tr474;
	goto tr470;
tr471:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st241;
st241:
	if ( ++p == pe )
		goto _test_eof241;
case 241:
#line 8718 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st206;
		case 47u: goto st54;
		case 62u: goto st332;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st206;
	goto tr0;
tr466:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st242;
tr472:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st242;
st242:
	if ( ++p == pe )
		goto _test_eof242;
case 242:
#line 8741 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 34u: goto tr471;
		case 35u: goto tr476;
		case 38u: goto tr472;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr477;
			} else if ( (*p) >= 1u )
				goto tr470;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr477;
			} else if ( (*p) >= 61u )
				goto tr470;
		} else
			goto tr470;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr470;
			} else if ( (*p) >= 97u )
				goto tr477;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr475;
			} else if ( (*p) >= 224u )
				goto tr474;
		} else
			goto tr473;
	} else
		goto tr470;
	goto tr0;
tr476:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st243;
st243:
	if ( ++p == pe )
		goto _test_eof243;
case 243:
#line 8790 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr471;
		case 38u: goto tr472;
		case 60u: goto tr0;
		case 88u: goto tr479;
		case 120u: goto tr479;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr478;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr474;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr475;
	} else
		goto tr473;
	goto tr470;
tr480:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st244;
tr478:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st244;
st244:
	if ( ++p == pe )
		goto _test_eof244;
case 244:
#line 8835 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr471;
		case 38u: goto tr472;
		case 59u: goto tr481;
		case 60u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr480;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr474;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr475;
	} else
		goto tr473;
	goto tr470;
tr467:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st245;
tr473:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st245;
st245:
	if ( ++p == pe )
		goto _test_eof245;
case 245:
#line 8875 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st240;
	goto tr0;
tr468:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st246;
tr474:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st246;
st246:
	if ( ++p == pe )
		goto _test_eof246;
case 246:
#line 8893 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st245;
	goto tr0;
tr469:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st247;
tr475:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st247;
st247:
	if ( ++p == pe )
		goto _test_eof247;
case 247:
#line 8911 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st246;
	goto tr0;
tr479:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st248;
st248:
	if ( ++p == pe )
		goto _test_eof248;
case 248:
#line 8925 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr471;
		case 38u: goto tr472;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr485;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr485;
		} else
			goto tr485;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr473;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr475;
		} else
			goto tr474;
	} else
		goto tr0;
	goto tr470;
tr486:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st249;
tr485:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st249;
st249:
	if ( ++p == pe )
		goto _test_eof249;
case 249:
#line 8974 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr471;
		case 38u: goto tr472;
		case 59u: goto tr487;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr486;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr486;
		} else
			goto tr486;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr473;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr475;
		} else
			goto tr474;
	} else
		goto tr0;
	goto tr470;
tr488:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st250;
tr477:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st250;
st250:
	if ( ++p == pe )
		goto _test_eof250;
case 250:
#line 9024 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 34u: goto tr471;
		case 38u: goto tr472;
		case 58u: goto tr470;
		case 59u: goto tr489;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr470;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr488;
			} else if ( (*p) >= 61u )
				goto tr470;
		} else
			goto tr488;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr470;
			} else if ( (*p) >= 97u )
				goto tr488;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr475;
			} else if ( (*p) >= 224u )
				goto tr474;
		} else
			goto tr473;
	} else
		goto tr470;
	goto tr0;
st251:
	if ( ++p == pe )
		goto _test_eof251;
case 251:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr491;
		case 39u: goto st241;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr492;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr494;
	} else
		goto tr493;
	goto tr490;
tr490:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st252;
tr495:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st252;
tr505:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st252;
tr511:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st252;
tr513:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st252;
st252:
	if ( ++p == pe )
		goto _test_eof252;
case 252:
#line 9124 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr496;
		case 39u: goto tr471;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr497;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr499;
	} else
		goto tr498;
	goto tr495;
tr491:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st253;
tr496:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st253;
st253:
	if ( ++p == pe )
		goto _test_eof253;
case 253:
#line 9160 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 35u: goto tr500;
		case 38u: goto tr496;
		case 39u: goto tr471;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr501;
			} else if ( (*p) >= 1u )
				goto tr495;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr501;
			} else if ( (*p) >= 61u )
				goto tr495;
		} else
			goto tr495;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr495;
			} else if ( (*p) >= 97u )
				goto tr501;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr499;
			} else if ( (*p) >= 224u )
				goto tr498;
		} else
			goto tr497;
	} else
		goto tr495;
	goto tr0;
tr500:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st254;
st254:
	if ( ++p == pe )
		goto _test_eof254;
case 254:
#line 9209 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr496;
		case 39u: goto tr471;
		case 60u: goto tr0;
		case 88u: goto tr503;
		case 120u: goto tr503;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr502;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr498;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr499;
	} else
		goto tr497;
	goto tr495;
tr504:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st255;
tr502:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st255;
st255:
	if ( ++p == pe )
		goto _test_eof255;
case 255:
#line 9254 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr496;
		case 39u: goto tr471;
		case 59u: goto tr505;
		case 60u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr504;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr498;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr499;
	} else
		goto tr497;
	goto tr495;
tr492:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st256;
tr497:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st256;
st256:
	if ( ++p == pe )
		goto _test_eof256;
case 256:
#line 9294 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st252;
	goto tr0;
tr493:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st257;
tr498:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st257;
st257:
	if ( ++p == pe )
		goto _test_eof257;
case 257:
#line 9312 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st256;
	goto tr0;
tr494:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st258;
tr499:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st258;
st258:
	if ( ++p == pe )
		goto _test_eof258;
case 258:
#line 9330 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st257;
	goto tr0;
tr503:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st259;
st259:
	if ( ++p == pe )
		goto _test_eof259;
case 259:
#line 9344 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr496;
		case 39u: goto tr471;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr509;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr509;
		} else
			goto tr509;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr497;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr499;
		} else
			goto tr498;
	} else
		goto tr0;
	goto tr495;
tr510:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st260;
tr509:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st260;
st260:
	if ( ++p == pe )
		goto _test_eof260;
case 260:
#line 9393 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr496;
		case 39u: goto tr471;
		case 59u: goto tr511;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr510;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr510;
		} else
			goto tr510;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr497;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr499;
		} else
			goto tr498;
	} else
		goto tr0;
	goto tr495;
tr512:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st261;
tr501:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st261;
st261:
	if ( ++p == pe )
		goto _test_eof261;
case 261:
#line 9443 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 38u: goto tr496;
		case 39u: goto tr471;
		case 58u: goto tr495;
		case 59u: goto tr513;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr495;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr512;
			} else if ( (*p) >= 61u )
				goto tr495;
		} else
			goto tr512;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr495;
			} else if ( (*p) >= 97u )
				goto tr512;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr499;
			} else if ( (*p) >= 224u )
				goto tr498;
		} else
			goto tr497;
	} else
		goto tr495;
	goto tr0;
st262:
	if ( ++p == pe )
		goto _test_eof262;
case 262:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 84u: goto st263;
		case 116u: goto st263;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st263:
	if ( ++p == pe )
		goto _test_eof263;
case 263:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 76u: goto st264;
		case 108u: goto st264;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st264:
	if ( ++p == pe )
		goto _test_eof264;
case 264:
	switch( (*p) ) {
		case 32u: goto tr92;
		case 47u: goto tr93;
		case 62u: goto tr95;
		case 69u: goto st265;
		case 101u: goto st265;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr92;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
st265:
	if ( ++p == pe )
		goto _test_eof265;
case 265:
	switch( (*p) ) {
		case 32u: goto tr517;
		case 47u: goto tr93;
		case 62u: goto tr518;
	}
	if ( (*p) < 48u ) {
		if ( 9u <= (*p) && (*p) <= 13u )
			goto tr517;
	} else if ( (*p) > 58u ) {
		if ( (*p) > 90u ) {
			if ( 97u <= (*p) && (*p) <= 122u )
				goto st51;
		} else if ( (*p) >= 65u )
			goto st51;
	} else
		goto st51;
	goto tr0;
tr517:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st266;
tr531:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st266;
st266:
	if ( ++p == pe )
		goto _test_eof266;
case 266:
#line 9582 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st266;
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st54;
		case 61u: goto tr0;
		case 62u: goto st333;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st266;
	goto st267;
st267:
	if ( ++p == pe )
		goto _test_eof267;
case 267:
	switch( (*p) ) {
		case 34u: goto tr0;
		case 39u: goto tr0;
		case 47u: goto st54;
		case 61u: goto st268;
		case 62u: goto st333;
	}
	goto st267;
st268:
	if ( ++p == pe )
		goto _test_eof268;
case 268:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto st268;
		case 34u: goto st296;
		case 38u: goto tr525;
		case 39u: goto st308;
		case 60u: goto tr0;
		case 62u: goto tr0;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto st268;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr528;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr529;
	} else
		goto tr527;
	goto tr523;
tr523:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st269;
tr530:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st269;
tr572:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st269;
tr575:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st269;
tr577:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st269;
st269:
	if ( ++p == pe )
		goto _test_eof269;
case 269:
#line 9676 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr531;
		case 34u: goto tr0;
		case 38u: goto tr532;
		case 39u: goto tr0;
		case 47u: goto tr533;
		case 60u: goto tr0;
		case 62u: goto tr534;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto tr531;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr536;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr537;
	} else
		goto tr535;
	goto tr530;
tr525:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st270;
tr532:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st270;
st270:
	if ( ++p == pe )
		goto _test_eof270;
case 270:
#line 9720 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr531;
		case 35u: goto tr538;
		case 38u: goto tr532;
		case 47u: goto tr533;
		case 62u: goto tr534;
	}
	if ( (*p) < 61u ) {
		if ( (*p) < 36u ) {
			if ( (*p) < 9u ) {
				if ( 1u <= (*p) && (*p) <= 8u )
					goto tr530;
			} else if ( (*p) > 13u ) {
				if ( 14u <= (*p) && (*p) <= 33u )
					goto tr530;
			} else
				goto tr531;
		} else if ( (*p) > 37u ) {
			if ( (*p) < 48u ) {
				if ( 40u <= (*p) && (*p) <= 46u )
					goto tr530;
			} else if ( (*p) > 57u ) {
				if ( 58u <= (*p) && (*p) <= 59u )
					goto tr530;
			} else
				goto tr539;
		} else
			goto tr530;
	} else if ( (*p) > 64u ) {
		if ( (*p) < 123u ) {
			if ( (*p) < 91u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr539;
			} else if ( (*p) > 95u ) {
				if ( 97u <= (*p) && (*p) <= 122u )
					goto tr539;
			} else
				goto tr530;
		} else if ( (*p) > 127u ) {
			if ( (*p) < 224u ) {
				if ( 192u <= (*p) && (*p) <= 223u )
					goto tr535;
			} else if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr537;
			} else
				goto tr536;
		} else
			goto tr530;
	} else
		goto tr530;
	goto tr0;
tr538:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st271;
st271:
	if ( ++p == pe )
		goto _test_eof271;
case 271:
#line 9783 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr531;
		case 34u: goto tr0;
		case 38u: goto tr532;
		case 39u: goto tr0;
		case 47u: goto tr533;
		case 60u: goto tr0;
		case 62u: goto tr534;
		case 88u: goto tr541;
		case 96u: goto tr0;
		case 120u: goto tr541;
	}
	if ( (*p) < 192u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr531;
		} else if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else
			goto tr540;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr536;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr537;
	} else
		goto tr535;
	goto tr530;
tr533:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st272;
st272:
	if ( ++p == pe )
		goto _test_eof272;
case 272:
#line 9828 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr531;
		case 34u: goto tr0;
		case 38u: goto tr532;
		case 39u: goto tr0;
		case 47u: goto tr533;
		case 60u: goto tr0;
		case 62u: goto tr542;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 13u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 9u )
			goto tr531;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr536;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr537;
	} else
		goto tr535;
	goto tr530;
tr518:
#line 27 "../ragel/formats/html/html.rl"
	{ _open_tag_name_end(p); }
	goto st333;
tr534:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st333;
tr542:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 28 "../ragel/formats/html/html.rl"
	{ _open_tag_self_closing(); }
	goto st333;
st333:
	if ( ++p == pe )
		goto _test_eof333;
case 333:
#line 9876 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr695;
		case 60u: goto tr696;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr697;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr699;
	} else
		goto tr698;
	goto tr694;
tr694:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st273;
tr543:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st273;
tr554:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st273;
tr565:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st273;
tr567:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st273;
st273:
	if ( ++p == pe )
		goto _test_eof273;
case 273:
#line 9935 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 60u: goto tr545;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr546;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr548;
	} else
		goto tr547;
	goto tr543;
tr695:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st274;
tr544:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st274;
st274:
	if ( ++p == pe )
		goto _test_eof274;
case 274:
#line 9970 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 35u: goto tr549;
		case 38u: goto tr544;
		case 60u: goto tr545;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr543;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr550;
			} else if ( (*p) >= 58u )
				goto tr543;
		} else
			goto tr550;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr543;
			} else if ( (*p) >= 97u )
				goto tr550;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr548;
			} else if ( (*p) >= 224u )
				goto tr547;
		} else
			goto tr546;
	} else
		goto tr543;
	goto tr0;
tr549:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st275;
st275:
	if ( ++p == pe )
		goto _test_eof275;
case 275:
#line 10016 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 60u: goto tr545;
		case 88u: goto tr552;
		case 120u: goto tr552;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr551;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr547;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr548;
	} else
		goto tr546;
	goto tr543;
tr553:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st276;
tr551:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st276;
st276:
	if ( ++p == pe )
		goto _test_eof276;
case 276:
#line 10060 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 59u: goto tr554;
		case 60u: goto tr545;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr553;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr547;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr548;
	} else
		goto tr546;
	goto tr543;
tr696:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st277;
tr545:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st277;
st277:
	if ( ++p == pe )
		goto _test_eof277;
case 277:
#line 10099 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 47u: goto tr555;
		case 60u: goto tr545;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr546;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr548;
	} else
		goto tr547;
	goto tr543;
tr555:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st278;
st278:
	if ( ++p == pe )
		goto _test_eof278;
case 278:
#line 10131 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 60u: goto tr545;
		case 84u: goto tr556;
		case 116u: goto tr556;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr546;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr548;
	} else
		goto tr547;
	goto tr543;
tr556:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st279;
st279:
	if ( ++p == pe )
		goto _test_eof279;
case 279:
#line 10164 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 60u: goto tr545;
		case 73u: goto tr557;
		case 105u: goto tr557;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr546;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr548;
	} else
		goto tr547;
	goto tr543;
tr557:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st280;
st280:
	if ( ++p == pe )
		goto _test_eof280;
case 280:
#line 10197 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 60u: goto tr545;
		case 84u: goto tr558;
		case 116u: goto tr558;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr546;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr548;
	} else
		goto tr547;
	goto tr543;
tr558:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st281;
st281:
	if ( ++p == pe )
		goto _test_eof281;
case 281:
#line 10230 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 60u: goto tr545;
		case 76u: goto tr559;
		case 108u: goto tr559;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr546;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr548;
	} else
		goto tr547;
	goto tr543;
tr559:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st282;
st282:
	if ( ++p == pe )
		goto _test_eof282;
case 282:
#line 10263 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 60u: goto tr545;
		case 69u: goto st113;
		case 101u: goto st113;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr546;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr548;
	} else
		goto tr547;
	goto tr543;
tr697:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st283;
tr546:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st283;
st283:
	if ( ++p == pe )
		goto _test_eof283;
case 283:
#line 10300 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st273;
	goto tr0;
tr698:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st284;
tr547:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st284;
st284:
	if ( ++p == pe )
		goto _test_eof284;
case 284:
#line 10318 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st283;
	goto tr0;
tr699:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st285;
tr548:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st285;
st285:
	if ( ++p == pe )
		goto _test_eof285;
case 285:
#line 10336 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st284;
	goto tr0;
tr552:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st286;
st286:
	if ( ++p == pe )
		goto _test_eof286;
case 286:
#line 10350 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 60u: goto tr545;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr563;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr563;
		} else
			goto tr563;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr546;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr548;
		} else
			goto tr547;
	} else
		goto tr0;
	goto tr543;
tr564:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st287;
tr563:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st287;
st287:
	if ( ++p == pe )
		goto _test_eof287;
case 287:
#line 10398 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr544;
		case 59u: goto tr565;
		case 60u: goto tr545;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr564;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr564;
		} else
			goto tr564;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr546;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr548;
		} else
			goto tr547;
	} else
		goto tr0;
	goto tr543;
tr566:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st288;
tr550:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st288;
st288:
	if ( ++p == pe )
		goto _test_eof288;
case 288:
#line 10447 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 38u: goto tr544;
		case 59u: goto tr567;
		case 60u: goto tr545;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr543;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr566;
			} else if ( (*p) >= 58u )
				goto tr543;
		} else
			goto tr566;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr543;
			} else if ( (*p) >= 97u )
				goto tr566;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr548;
			} else if ( (*p) >= 224u )
				goto tr547;
		} else
			goto tr546;
	} else
		goto tr543;
	goto tr0;
tr527:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st289;
tr535:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st289;
st289:
	if ( ++p == pe )
		goto _test_eof289;
case 289:
#line 10497 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st269;
	goto tr0;
tr528:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st290;
tr536:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st290;
st290:
	if ( ++p == pe )
		goto _test_eof290;
case 290:
#line 10515 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st289;
	goto tr0;
tr529:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st291;
tr537:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st291;
st291:
	if ( ++p == pe )
		goto _test_eof291;
case 291:
#line 10533 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st290;
	goto tr0;
tr571:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st292;
tr540:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st292;
st292:
	if ( ++p == pe )
		goto _test_eof292;
case 292:
#line 10555 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr531;
		case 34u: goto tr0;
		case 38u: goto tr532;
		case 39u: goto tr0;
		case 47u: goto tr533;
		case 59u: goto tr572;
		case 60u: goto tr0;
		case 62u: goto tr534;
		case 96u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr531;
		} else if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else
			goto tr571;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr536;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr537;
	} else
		goto tr535;
	goto tr530;
tr541:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st293;
st293:
	if ( ++p == pe )
		goto _test_eof293;
case 293:
#line 10599 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr531;
		case 34u: goto tr0;
		case 38u: goto tr532;
		case 39u: goto tr0;
		case 47u: goto tr533;
		case 60u: goto tr0;
		case 62u: goto tr534;
		case 96u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr531;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 70u ) {
				if ( 97u <= (*p) && (*p) <= 102u )
					goto tr573;
			} else if ( (*p) >= 65u )
				goto tr573;
		} else
			goto tr573;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr535;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr537;
		} else
			goto tr536;
	} else
		goto tr0;
	goto tr530;
tr574:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st294;
tr573:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st294;
st294:
	if ( ++p == pe )
		goto _test_eof294;
case 294:
#line 10656 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 32u: goto tr531;
		case 34u: goto tr0;
		case 38u: goto tr532;
		case 39u: goto tr0;
		case 47u: goto tr533;
		case 59u: goto tr575;
		case 60u: goto tr0;
		case 62u: goto tr534;
		case 96u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 48u ) {
			if ( 9u <= (*p) && (*p) <= 13u )
				goto tr531;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 70u ) {
				if ( 97u <= (*p) && (*p) <= 102u )
					goto tr574;
			} else if ( (*p) >= 65u )
				goto tr574;
		} else
			goto tr574;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr535;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr537;
		} else
			goto tr536;
	} else
		goto tr0;
	goto tr530;
tr576:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st295;
tr539:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st295;
st295:
	if ( ++p == pe )
		goto _test_eof295;
case 295:
#line 10714 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto tr531;
		case 38u: goto tr532;
		case 47u: goto tr533;
		case 58u: goto tr530;
		case 59u: goto tr577;
		case 62u: goto tr534;
	}
	if ( (*p) < 61u ) {
		if ( (*p) < 14u ) {
			if ( (*p) > 8u ) {
				if ( 9u <= (*p) && (*p) <= 13u )
					goto tr531;
			} else if ( (*p) >= 1u )
				goto tr530;
		} else if ( (*p) > 33u ) {
			if ( (*p) < 40u ) {
				if ( 35u <= (*p) && (*p) <= 37u )
					goto tr530;
			} else if ( (*p) > 46u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr576;
			} else
				goto tr530;
		} else
			goto tr530;
	} else if ( (*p) > 64u ) {
		if ( (*p) < 123u ) {
			if ( (*p) < 91u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr576;
			} else if ( (*p) > 95u ) {
				if ( 97u <= (*p) && (*p) <= 122u )
					goto tr576;
			} else
				goto tr530;
		} else if ( (*p) > 127u ) {
			if ( (*p) < 224u ) {
				if ( 192u <= (*p) && (*p) <= 223u )
					goto tr535;
			} else if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr537;
			} else
				goto tr536;
		} else
			goto tr530;
	} else
		goto tr530;
	goto tr0;
st296:
	if ( ++p == pe )
		goto _test_eof296;
case 296:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto st298;
		case 38u: goto tr580;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr581;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr583;
	} else
		goto tr582;
	goto tr578;
tr578:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st297;
tr584:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st297;
tr595:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st297;
tr601:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st297;
tr603:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st297;
st297:
	if ( ++p == pe )
		goto _test_eof297;
case 297:
#line 10828 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr585;
		case 38u: goto tr586;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr587;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr589;
	} else
		goto tr588;
	goto tr584;
tr585:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	goto st298;
st298:
	if ( ++p == pe )
		goto _test_eof298;
case 298:
#line 10858 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 32u: goto st266;
		case 47u: goto st54;
		case 62u: goto st333;
	}
	if ( 9u <= (*p) && (*p) <= 13u )
		goto st266;
	goto tr0;
tr580:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st299;
tr586:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st299;
st299:
	if ( ++p == pe )
		goto _test_eof299;
case 299:
#line 10881 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 34u: goto tr585;
		case 35u: goto tr590;
		case 38u: goto tr586;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr591;
			} else if ( (*p) >= 1u )
				goto tr584;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr591;
			} else if ( (*p) >= 61u )
				goto tr584;
		} else
			goto tr584;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr584;
			} else if ( (*p) >= 97u )
				goto tr591;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr589;
			} else if ( (*p) >= 224u )
				goto tr588;
		} else
			goto tr587;
	} else
		goto tr584;
	goto tr0;
tr590:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st300;
st300:
	if ( ++p == pe )
		goto _test_eof300;
case 300:
#line 10930 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr585;
		case 38u: goto tr586;
		case 60u: goto tr0;
		case 88u: goto tr593;
		case 120u: goto tr593;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr592;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr588;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr589;
	} else
		goto tr587;
	goto tr584;
tr594:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st301;
tr592:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st301;
st301:
	if ( ++p == pe )
		goto _test_eof301;
case 301:
#line 10975 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr585;
		case 38u: goto tr586;
		case 59u: goto tr595;
		case 60u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr594;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr588;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr589;
	} else
		goto tr587;
	goto tr584;
tr581:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st302;
tr587:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st302;
st302:
	if ( ++p == pe )
		goto _test_eof302;
case 302:
#line 11015 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st297;
	goto tr0;
tr582:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st303;
tr588:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st303;
st303:
	if ( ++p == pe )
		goto _test_eof303;
case 303:
#line 11033 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st302;
	goto tr0;
tr583:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st304;
tr589:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st304;
st304:
	if ( ++p == pe )
		goto _test_eof304;
case 304:
#line 11051 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st303;
	goto tr0;
tr593:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st305;
st305:
	if ( ++p == pe )
		goto _test_eof305;
case 305:
#line 11065 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr585;
		case 38u: goto tr586;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr599;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr599;
		} else
			goto tr599;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr587;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr589;
		} else
			goto tr588;
	} else
		goto tr0;
	goto tr584;
tr600:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st306;
tr599:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st306;
st306:
	if ( ++p == pe )
		goto _test_eof306;
case 306:
#line 11114 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 34u: goto tr585;
		case 38u: goto tr586;
		case 59u: goto tr601;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr600;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr600;
		} else
			goto tr600;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr587;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr589;
		} else
			goto tr588;
	} else
		goto tr0;
	goto tr584;
tr602:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st307;
tr591:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st307;
st307:
	if ( ++p == pe )
		goto _test_eof307;
case 307:
#line 11164 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 34u: goto tr585;
		case 38u: goto tr586;
		case 58u: goto tr584;
		case 59u: goto tr603;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr584;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr602;
			} else if ( (*p) >= 61u )
				goto tr584;
		} else
			goto tr602;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr584;
			} else if ( (*p) >= 97u )
				goto tr602;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr589;
			} else if ( (*p) >= 224u )
				goto tr588;
		} else
			goto tr587;
	} else
		goto tr584;
	goto tr0;
st308:
	if ( ++p == pe )
		goto _test_eof308;
case 308:
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr605;
		case 39u: goto st298;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr606;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr608;
	} else
		goto tr607;
	goto tr604;
tr604:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st309;
tr609:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st309;
tr619:
#line 9 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st309;
tr625:
#line 11 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st309;
tr627:
#line 13 "../ragel/formats/html/html.rl"
	{ _named_character_reference_end(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st309;
st309:
	if ( ++p == pe )
		goto _test_eof309;
case 309:
#line 11264 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr610;
		case 39u: goto tr585;
		case 60u: goto tr0;
	}
	if ( (*p) < 224u ) {
		if ( (*p) > 191u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr611;
		} else if ( (*p) >= 128u )
			goto tr0;
	} else if ( (*p) > 239u ) {
		if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else if ( (*p) >= 240u )
			goto tr613;
	} else
		goto tr612;
	goto tr609;
tr605:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st310;
tr610:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st310;
st310:
	if ( ++p == pe )
		goto _test_eof310;
case 310:
#line 11300 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 35u: goto tr614;
		case 38u: goto tr610;
		case 39u: goto tr585;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 58u ) {
			if ( (*p) > 47u ) {
				if ( 48u <= (*p) && (*p) <= 57u )
					goto tr615;
			} else if ( (*p) >= 1u )
				goto tr609;
		} else if ( (*p) > 59u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr615;
			} else if ( (*p) >= 61u )
				goto tr609;
		} else
			goto tr609;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr609;
			} else if ( (*p) >= 97u )
				goto tr615;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr613;
			} else if ( (*p) >= 224u )
				goto tr612;
		} else
			goto tr611;
	} else
		goto tr609;
	goto tr0;
tr614:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st311;
st311:
	if ( ++p == pe )
		goto _test_eof311;
case 311:
#line 11349 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr610;
		case 39u: goto tr585;
		case 60u: goto tr0;
		case 88u: goto tr617;
		case 120u: goto tr617;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr616;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr612;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr613;
	} else
		goto tr611;
	goto tr609;
tr618:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st312;
tr616:
#line 8 "../ragel/formats/html/html.rl"
	{ _decimal_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st312;
st312:
	if ( ++p == pe )
		goto _test_eof312;
case 312:
#line 11394 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr610;
		case 39u: goto tr585;
		case 59u: goto tr619;
		case 60u: goto tr0;
	}
	if ( (*p) < 192u ) {
		if ( (*p) > 57u ) {
			if ( 128u <= (*p) && (*p) <= 191u )
				goto tr0;
		} else if ( (*p) >= 48u )
			goto tr618;
	} else if ( (*p) > 223u ) {
		if ( (*p) < 240u ) {
			if ( 224u <= (*p) && (*p) <= 239u )
				goto tr612;
		} else if ( (*p) > 247u ) {
			if ( 248u <= (*p) )
				goto tr0;
		} else
			goto tr613;
	} else
		goto tr611;
	goto tr609;
tr606:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st313;
tr611:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st313;
st313:
	if ( ++p == pe )
		goto _test_eof313;
case 313:
#line 11434 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st309;
	goto tr0;
tr607:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st314;
tr612:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st314;
st314:
	if ( ++p == pe )
		goto _test_eof314;
case 314:
#line 11452 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st313;
	goto tr0;
tr608:
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st315;
tr613:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st315;
st315:
	if ( ++p == pe )
		goto _test_eof315;
case 315:
#line 11470 "schwa/formats/html_gen.cc"
	if ( 128u <= (*p) && (*p) <= 191u )
		goto st314;
	goto tr0;
tr617:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st316;
st316:
	if ( ++p == pe )
		goto _test_eof316;
case 316:
#line 11484 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr610;
		case 39u: goto tr585;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr623;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr623;
		} else
			goto tr623;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr611;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr613;
		} else
			goto tr612;
	} else
		goto tr0;
	goto tr609;
tr624:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st317;
tr623:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st317;
st317:
	if ( ++p == pe )
		goto _test_eof317;
case 317:
#line 11533 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr610;
		case 39u: goto tr585;
		case 59u: goto tr625;
		case 60u: goto tr0;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr624;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr624;
		} else
			goto tr624;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr611;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr613;
		} else
			goto tr612;
	} else
		goto tr0;
	goto tr609;
tr626:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st318;
tr615:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st318;
st318:
	if ( ++p == pe )
		goto _test_eof318;
case 318:
#line 11583 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 38u: goto tr610;
		case 39u: goto tr585;
		case 58u: goto tr609;
		case 59u: goto tr627;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr609;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr626;
			} else if ( (*p) >= 61u )
				goto tr609;
		} else
			goto tr626;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr609;
			} else if ( (*p) >= 97u )
				goto tr626;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr613;
			} else if ( (*p) >= 224u )
				goto tr612;
		} else
			goto tr611;
	} else
		goto tr609;
	goto tr0;
tr645:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st334;
st334:
	if ( ++p == pe )
		goto _test_eof334;
case 334:
#line 11630 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr637;
		case 60u: goto tr638;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr700;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr700;
		} else
			goto tr700;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr639;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr641;
		} else
			goto tr640;
	} else
		goto tr0;
	goto tr636;
tr701:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st335;
tr700:
#line 10 "../ragel/formats/html/html.rl"
	{ _hex_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st335;
st335:
	if ( ++p == pe )
		goto _test_eof335;
case 335:
#line 11678 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 0u: goto tr0;
		case 38u: goto tr637;
		case 59u: goto tr702;
		case 60u: goto tr638;
	}
	if ( (*p) < 128u ) {
		if ( (*p) < 65u ) {
			if ( 48u <= (*p) && (*p) <= 57u )
				goto tr701;
		} else if ( (*p) > 70u ) {
			if ( 97u <= (*p) && (*p) <= 102u )
				goto tr701;
		} else
			goto tr701;
	} else if ( (*p) > 191u ) {
		if ( (*p) < 224u ) {
			if ( 192u <= (*p) && (*p) <= 223u )
				goto tr639;
		} else if ( (*p) > 239u ) {
			if ( (*p) > 247u ) {
				if ( 248u <= (*p) )
					goto tr0;
			} else if ( (*p) >= 240u )
				goto tr641;
		} else
			goto tr640;
	} else
		goto tr0;
	goto tr636;
tr703:
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st336;
tr643:
#line 12 "../ragel/formats/html/html.rl"
	{ _named_character_reference_start(p); }
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
#line 14 "../ragel/formats/html/html.rl"
	{ _text_character_start(p); }
	goto st336;
st336:
	if ( ++p == pe )
		goto _test_eof336;
case 336:
#line 11727 "schwa/formats/html_gen.cc"
	switch( (*p) ) {
		case 38u: goto tr637;
		case 59u: goto tr704;
		case 60u: goto tr638;
	}
	if ( (*p) < 91u ) {
		if ( (*p) < 48u ) {
			if ( 1u <= (*p) && (*p) <= 47u )
				goto tr636;
		} else if ( (*p) > 57u ) {
			if ( (*p) > 64u ) {
				if ( 65u <= (*p) && (*p) <= 90u )
					goto tr703;
			} else if ( (*p) >= 58u )
				goto tr636;
		} else
			goto tr703;
	} else if ( (*p) > 96u ) {
		if ( (*p) < 192u ) {
			if ( (*p) > 122u ) {
				if ( 123u <= (*p) && (*p) <= 127u )
					goto tr636;
			} else if ( (*p) >= 97u )
				goto tr703;
		} else if ( (*p) > 223u ) {
			if ( (*p) > 239u ) {
				if ( 240u <= (*p) && (*p) <= 247u )
					goto tr641;
			} else if ( (*p) >= 224u )
				goto tr640;
		} else
			goto tr639;
	} else
		goto tr636;
	goto tr0;
st319:
	if ( ++p == pe )
		goto _test_eof319;
case 319:
	if ( (*p) == 62u )
		goto tr629;
	goto st319;
tr629:
#line 13 "../ragel/formats/html/main.rl"
	{ {goto st320;} }
	goto st337;
st337:
	if ( ++p == pe )
		goto _test_eof337;
case 337:
#line 11778 "schwa/formats/html_gen.cc"
	goto st0;
	}
	_test_eof320: cs = 320; goto _test_eof; 
	_test_eof321: cs = 321; goto _test_eof; 
	_test_eof322: cs = 322; goto _test_eof; 
	_test_eof323: cs = 323; goto _test_eof; 
	_test_eof324: cs = 324; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof325: cs = 325; goto _test_eof; 
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
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof326: cs = 326; goto _test_eof; 
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
	_test_eof327: cs = 327; goto _test_eof; 
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
	_test_eof328: cs = 328; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof329: cs = 329; goto _test_eof; 
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
	_test_eof330: cs = 330; goto _test_eof; 
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
	_test_eof331: cs = 331; goto _test_eof; 
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
	_test_eof169: cs = 169; goto _test_eof; 
	_test_eof170: cs = 170; goto _test_eof; 
	_test_eof171: cs = 171; goto _test_eof; 
	_test_eof172: cs = 172; goto _test_eof; 
	_test_eof173: cs = 173; goto _test_eof; 
	_test_eof174: cs = 174; goto _test_eof; 
	_test_eof175: cs = 175; goto _test_eof; 
	_test_eof176: cs = 176; goto _test_eof; 
	_test_eof177: cs = 177; goto _test_eof; 
	_test_eof178: cs = 178; goto _test_eof; 
	_test_eof179: cs = 179; goto _test_eof; 
	_test_eof180: cs = 180; goto _test_eof; 
	_test_eof181: cs = 181; goto _test_eof; 
	_test_eof182: cs = 182; goto _test_eof; 
	_test_eof183: cs = 183; goto _test_eof; 
	_test_eof184: cs = 184; goto _test_eof; 
	_test_eof185: cs = 185; goto _test_eof; 
	_test_eof186: cs = 186; goto _test_eof; 
	_test_eof187: cs = 187; goto _test_eof; 
	_test_eof188: cs = 188; goto _test_eof; 
	_test_eof189: cs = 189; goto _test_eof; 
	_test_eof190: cs = 190; goto _test_eof; 
	_test_eof191: cs = 191; goto _test_eof; 
	_test_eof192: cs = 192; goto _test_eof; 
	_test_eof193: cs = 193; goto _test_eof; 
	_test_eof194: cs = 194; goto _test_eof; 
	_test_eof195: cs = 195; goto _test_eof; 
	_test_eof196: cs = 196; goto _test_eof; 
	_test_eof197: cs = 197; goto _test_eof; 
	_test_eof198: cs = 198; goto _test_eof; 
	_test_eof199: cs = 199; goto _test_eof; 
	_test_eof200: cs = 200; goto _test_eof; 
	_test_eof201: cs = 201; goto _test_eof; 
	_test_eof202: cs = 202; goto _test_eof; 
	_test_eof203: cs = 203; goto _test_eof; 
	_test_eof204: cs = 204; goto _test_eof; 
	_test_eof205: cs = 205; goto _test_eof; 
	_test_eof206: cs = 206; goto _test_eof; 
	_test_eof207: cs = 207; goto _test_eof; 
	_test_eof208: cs = 208; goto _test_eof; 
	_test_eof209: cs = 209; goto _test_eof; 
	_test_eof210: cs = 210; goto _test_eof; 
	_test_eof211: cs = 211; goto _test_eof; 
	_test_eof212: cs = 212; goto _test_eof; 
	_test_eof332: cs = 332; goto _test_eof; 
	_test_eof213: cs = 213; goto _test_eof; 
	_test_eof214: cs = 214; goto _test_eof; 
	_test_eof215: cs = 215; goto _test_eof; 
	_test_eof216: cs = 216; goto _test_eof; 
	_test_eof217: cs = 217; goto _test_eof; 
	_test_eof218: cs = 218; goto _test_eof; 
	_test_eof219: cs = 219; goto _test_eof; 
	_test_eof220: cs = 220; goto _test_eof; 
	_test_eof221: cs = 221; goto _test_eof; 
	_test_eof222: cs = 222; goto _test_eof; 
	_test_eof223: cs = 223; goto _test_eof; 
	_test_eof224: cs = 224; goto _test_eof; 
	_test_eof225: cs = 225; goto _test_eof; 
	_test_eof226: cs = 226; goto _test_eof; 
	_test_eof227: cs = 227; goto _test_eof; 
	_test_eof228: cs = 228; goto _test_eof; 
	_test_eof229: cs = 229; goto _test_eof; 
	_test_eof230: cs = 230; goto _test_eof; 
	_test_eof231: cs = 231; goto _test_eof; 
	_test_eof232: cs = 232; goto _test_eof; 
	_test_eof233: cs = 233; goto _test_eof; 
	_test_eof234: cs = 234; goto _test_eof; 
	_test_eof235: cs = 235; goto _test_eof; 
	_test_eof236: cs = 236; goto _test_eof; 
	_test_eof237: cs = 237; goto _test_eof; 
	_test_eof238: cs = 238; goto _test_eof; 
	_test_eof239: cs = 239; goto _test_eof; 
	_test_eof240: cs = 240; goto _test_eof; 
	_test_eof241: cs = 241; goto _test_eof; 
	_test_eof242: cs = 242; goto _test_eof; 
	_test_eof243: cs = 243; goto _test_eof; 
	_test_eof244: cs = 244; goto _test_eof; 
	_test_eof245: cs = 245; goto _test_eof; 
	_test_eof246: cs = 246; goto _test_eof; 
	_test_eof247: cs = 247; goto _test_eof; 
	_test_eof248: cs = 248; goto _test_eof; 
	_test_eof249: cs = 249; goto _test_eof; 
	_test_eof250: cs = 250; goto _test_eof; 
	_test_eof251: cs = 251; goto _test_eof; 
	_test_eof252: cs = 252; goto _test_eof; 
	_test_eof253: cs = 253; goto _test_eof; 
	_test_eof254: cs = 254; goto _test_eof; 
	_test_eof255: cs = 255; goto _test_eof; 
	_test_eof256: cs = 256; goto _test_eof; 
	_test_eof257: cs = 257; goto _test_eof; 
	_test_eof258: cs = 258; goto _test_eof; 
	_test_eof259: cs = 259; goto _test_eof; 
	_test_eof260: cs = 260; goto _test_eof; 
	_test_eof261: cs = 261; goto _test_eof; 
	_test_eof262: cs = 262; goto _test_eof; 
	_test_eof263: cs = 263; goto _test_eof; 
	_test_eof264: cs = 264; goto _test_eof; 
	_test_eof265: cs = 265; goto _test_eof; 
	_test_eof266: cs = 266; goto _test_eof; 
	_test_eof267: cs = 267; goto _test_eof; 
	_test_eof268: cs = 268; goto _test_eof; 
	_test_eof269: cs = 269; goto _test_eof; 
	_test_eof270: cs = 270; goto _test_eof; 
	_test_eof271: cs = 271; goto _test_eof; 
	_test_eof272: cs = 272; goto _test_eof; 
	_test_eof333: cs = 333; goto _test_eof; 
	_test_eof273: cs = 273; goto _test_eof; 
	_test_eof274: cs = 274; goto _test_eof; 
	_test_eof275: cs = 275; goto _test_eof; 
	_test_eof276: cs = 276; goto _test_eof; 
	_test_eof277: cs = 277; goto _test_eof; 
	_test_eof278: cs = 278; goto _test_eof; 
	_test_eof279: cs = 279; goto _test_eof; 
	_test_eof280: cs = 280; goto _test_eof; 
	_test_eof281: cs = 281; goto _test_eof; 
	_test_eof282: cs = 282; goto _test_eof; 
	_test_eof283: cs = 283; goto _test_eof; 
	_test_eof284: cs = 284; goto _test_eof; 
	_test_eof285: cs = 285; goto _test_eof; 
	_test_eof286: cs = 286; goto _test_eof; 
	_test_eof287: cs = 287; goto _test_eof; 
	_test_eof288: cs = 288; goto _test_eof; 
	_test_eof289: cs = 289; goto _test_eof; 
	_test_eof290: cs = 290; goto _test_eof; 
	_test_eof291: cs = 291; goto _test_eof; 
	_test_eof292: cs = 292; goto _test_eof; 
	_test_eof293: cs = 293; goto _test_eof; 
	_test_eof294: cs = 294; goto _test_eof; 
	_test_eof295: cs = 295; goto _test_eof; 
	_test_eof296: cs = 296; goto _test_eof; 
	_test_eof297: cs = 297; goto _test_eof; 
	_test_eof298: cs = 298; goto _test_eof; 
	_test_eof299: cs = 299; goto _test_eof; 
	_test_eof300: cs = 300; goto _test_eof; 
	_test_eof301: cs = 301; goto _test_eof; 
	_test_eof302: cs = 302; goto _test_eof; 
	_test_eof303: cs = 303; goto _test_eof; 
	_test_eof304: cs = 304; goto _test_eof; 
	_test_eof305: cs = 305; goto _test_eof; 
	_test_eof306: cs = 306; goto _test_eof; 
	_test_eof307: cs = 307; goto _test_eof; 
	_test_eof308: cs = 308; goto _test_eof; 
	_test_eof309: cs = 309; goto _test_eof; 
	_test_eof310: cs = 310; goto _test_eof; 
	_test_eof311: cs = 311; goto _test_eof; 
	_test_eof312: cs = 312; goto _test_eof; 
	_test_eof313: cs = 313; goto _test_eof; 
	_test_eof314: cs = 314; goto _test_eof; 
	_test_eof315: cs = 315; goto _test_eof; 
	_test_eof316: cs = 316; goto _test_eof; 
	_test_eof317: cs = 317; goto _test_eof; 
	_test_eof318: cs = 318; goto _test_eof; 
	_test_eof334: cs = 334; goto _test_eof; 
	_test_eof335: cs = 335; goto _test_eof; 
	_test_eof336: cs = 336; goto _test_eof; 
	_test_eof319: cs = 319; goto _test_eof; 
	_test_eof337: cs = 337; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 321: 
	case 322: 
	case 323: 
	case 324: 
	case 334: 
	case 335: 
	case 336: 
#line 15 "../ragel/formats/html/html.rl"
	{ _text_character_end(p); }
	break;
	case 327: 
#line 18 "../ragel/formats/html/html.rl"
	{ _cdata_end(p); }
	break;
	case 325: 
#line 20 "../ragel/formats/html/html.rl"
	{ _comment_end(p); }
	break;
	case 326: 
#line 22 "../ragel/formats/html/html.rl"
	{ _doctype_end(p); }
	break;
	case 329: 
	case 330: 
	case 331: 
	case 332: 
	case 333: 
#line 25 "../ragel/formats/html/html.rl"
	{ _open_tag_end(p); }
	break;
	case 328: 
#line 30 "../ragel/formats/html/html.rl"
	{ _close_tag_end(p); }
	break;
	case 1: 
	case 2: 
	case 3: 
	case 4: 
	case 5: 
	case 6: 
	case 7: 
	case 8: 
	case 9: 
	case 10: 
	case 11: 
	case 12: 
	case 13: 
	case 14: 
	case 15: 
	case 16: 
	case 17: 
	case 18: 
	case 19: 
	case 20: 
	case 21: 
	case 22: 
	case 23: 
	case 24: 
	case 25: 
	case 26: 
	case 27: 
	case 28: 
	case 29: 
	case 30: 
	case 31: 
	case 32: 
	case 33: 
	case 34: 
	case 35: 
	case 36: 
	case 37: 
	case 38: 
	case 39: 
	case 40: 
	case 41: 
	case 42: 
	case 43: 
	case 44: 
	case 45: 
	case 46: 
	case 47: 
	case 48: 
	case 49: 
	case 50: 
	case 51: 
	case 52: 
	case 53: 
	case 54: 
	case 55: 
	case 56: 
	case 57: 
	case 58: 
	case 59: 
	case 60: 
	case 61: 
	case 62: 
	case 63: 
	case 64: 
	case 65: 
	case 66: 
	case 67: 
	case 68: 
	case 69: 
	case 70: 
	case 71: 
	case 72: 
	case 73: 
	case 74: 
	case 75: 
	case 76: 
	case 77: 
	case 78: 
	case 79: 
	case 80: 
	case 81: 
	case 82: 
	case 83: 
	case 84: 
	case 85: 
	case 86: 
	case 87: 
	case 88: 
	case 89: 
	case 90: 
	case 91: 
	case 92: 
	case 93: 
	case 94: 
	case 95: 
	case 96: 
	case 97: 
	case 98: 
	case 99: 
	case 100: 
	case 101: 
	case 102: 
	case 103: 
	case 104: 
	case 105: 
	case 106: 
	case 107: 
	case 108: 
	case 109: 
	case 110: 
	case 111: 
	case 112: 
	case 113: 
	case 114: 
	case 115: 
	case 116: 
	case 117: 
	case 118: 
	case 119: 
	case 120: 
	case 121: 
	case 122: 
	case 123: 
	case 124: 
	case 125: 
	case 126: 
	case 127: 
	case 128: 
	case 129: 
	case 130: 
	case 131: 
	case 132: 
	case 133: 
	case 134: 
	case 135: 
	case 136: 
	case 137: 
	case 138: 
	case 139: 
	case 140: 
	case 141: 
	case 142: 
	case 143: 
	case 144: 
	case 145: 
	case 146: 
	case 147: 
	case 148: 
	case 149: 
	case 150: 
	case 151: 
	case 152: 
	case 153: 
	case 154: 
	case 155: 
	case 156: 
	case 157: 
	case 158: 
	case 159: 
	case 160: 
	case 161: 
	case 162: 
	case 163: 
	case 164: 
	case 165: 
	case 166: 
	case 167: 
	case 168: 
	case 169: 
	case 170: 
	case 171: 
	case 172: 
	case 173: 
	case 174: 
	case 175: 
	case 176: 
	case 177: 
	case 178: 
	case 179: 
	case 180: 
	case 181: 
	case 182: 
	case 183: 
	case 184: 
	case 185: 
	case 186: 
	case 187: 
	case 188: 
	case 189: 
	case 190: 
	case 191: 
	case 192: 
	case 193: 
	case 194: 
	case 195: 
	case 196: 
	case 197: 
	case 198: 
	case 199: 
	case 200: 
	case 201: 
	case 202: 
	case 203: 
	case 204: 
	case 205: 
	case 206: 
	case 207: 
	case 208: 
	case 209: 
	case 210: 
	case 211: 
	case 212: 
	case 213: 
	case 214: 
	case 215: 
	case 216: 
	case 217: 
	case 218: 
	case 219: 
	case 220: 
	case 221: 
	case 222: 
	case 223: 
	case 224: 
	case 225: 
	case 226: 
	case 227: 
	case 228: 
	case 229: 
	case 230: 
	case 231: 
	case 232: 
	case 233: 
	case 234: 
	case 235: 
	case 236: 
	case 237: 
	case 238: 
	case 239: 
	case 240: 
	case 241: 
	case 242: 
	case 243: 
	case 244: 
	case 245: 
	case 246: 
	case 247: 
	case 248: 
	case 249: 
	case 250: 
	case 251: 
	case 252: 
	case 253: 
	case 254: 
	case 255: 
	case 256: 
	case 257: 
	case 258: 
	case 259: 
	case 260: 
	case 261: 
	case 262: 
	case 263: 
	case 264: 
	case 265: 
	case 266: 
	case 267: 
	case 268: 
	case 269: 
	case 270: 
	case 271: 
	case 272: 
	case 273: 
	case 274: 
	case 275: 
	case 276: 
	case 277: 
	case 278: 
	case 279: 
	case 280: 
	case 281: 
	case 282: 
	case 283: 
	case 284: 
	case 285: 
	case 286: 
	case 287: 
	case 288: 
	case 289: 
	case 290: 
	case 291: 
	case 292: 
	case 293: 
	case 294: 
	case 295: 
	case 296: 
	case 297: 
	case 298: 
	case 299: 
	case 300: 
	case 301: 
	case 302: 
	case 303: 
	case 304: 
	case 305: 
	case 306: 
	case 307: 
	case 308: 
	case 309: 
	case 310: 
	case 311: 
	case 312: 
	case 313: 
	case 314: 
	case 315: 
	case 316: 
	case 317: 
	case 318: 
#line 9 "../ragel/formats/html/main.rl"
	{
    p--; {goto st319;}
  }
	break;
#line 12480 "schwa/formats/html_gen.cc"
	}
	}

	_out: {}
	}

#line 36 "../ragel/formats/html/main.rl"
  _end_document();

  return cs != 
#line 12491 "schwa/formats/html_gen.cc"
0
#line 38 "../ragel/formats/html/main.rl"
;
}

}  // namespace formats
}  // namespace schwa
