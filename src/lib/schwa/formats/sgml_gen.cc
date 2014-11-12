
#line 1 "../ragel/formats/sgml/sgml.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 73 "../ragel/formats/sgml/sgml.rl"



#include <schwa/formats/sgml.h>


namespace schwa {
namespace formats {


#line 19 "schwa/formats/sgml_gen.cc"
static const int sgml_start = 168;

static const int sgml_en_main = 168;


#line 83 "../ragel/formats/sgml/sgml.rl"

void
SGMLishLexer::_init(void) {
  (void)sgml_en_main;
  
#line 31 "schwa/formats/sgml_gen.cc"
	{
	 _state.cs = sgml_start;
	 _state.ts = 0;
	 _state.te = 0;
	 _state.act = 0;
	}

#line 88 "../ragel/formats/sgml/sgml.rl"
}


SGMLishNode *
SGMLishLexer::lex(Pool &node_pool) {
  _node_pool = &node_pool;

  // Don't attempt to lex if we're at EOF.
  if (_state.at_eof())
    return nullptr;
  const auto p_before = _state.p;

  
#line 53 "schwa/formats/sgml_gen.cc"
	{
	if ( ( _state.p) == ( _state.pe) )
		goto _test_eof;
	switch (  _state.cs )
	{
tr0:
#line 1 "NONE"
	{	switch(  _state.act ) {
	case 0:
	{{goto st0;}}
	break;
	case 4:
	{{( _state.p) = (( _state.te))-1;} _create_text_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }
	break;
	}
	}
	goto st168;
tr26:
#line 20 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_end(( _state.p)); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_start_tag_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr33:
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_start_tag_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr56:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_start_tag_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr69:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_start_tag_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr82:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_start_tag_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr94:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_start_tag_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr112:
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_start_tag_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr113:
#line 66 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_empty_tag_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr193:
#line 70 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_comment_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr205:
#line 69 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_cdata_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr212:
#line 20 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_end(( _state.p)); }
#line 67 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_end_tag_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr217:
#line 67 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_end_tag_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr261:
#line 71 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_xml_decl_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr376:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_text_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr382:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_text_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr388:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_text_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
tr394:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_text_node(); {( _state.p)++;  _state.cs = 168; goto _out;} }}
	goto st168;
st168:
#line 1 "NONE"
	{ _state.ts = 0;}
#line 1 "NONE"
	{ _state.act = 0;}
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof168;
case 168:
#line 1 "NONE"
	{ _state.ts = ( _state.p);}
#line 179 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr371;
		case 60u: goto st13;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr373;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr375;
	} else
		goto tr374;
	goto tr370;
st0:
 _state.cs = 0;
	goto _out;
tr1:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st169;
tr370:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st169;
tr377:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st169;
tr383:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st169;
tr389:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st169;
tr395:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st169;
st169:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof169;
case 169:
#line 262 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto tr376;
		case 38u: goto tr378;
		case 60u: goto tr376;
		case 62u: goto tr376;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr379;
		} else if ( (*( _state.p)) >= 128u )
			goto tr376;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto tr376;
		} else if ( (*( _state.p)) >= 240u )
			goto tr381;
	} else
		goto tr380;
	goto tr377;
tr371:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st1;
tr378:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st1;
tr384:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st1;
tr390:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st1;
tr396:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st1;
st1:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof1;
case 1:
#line 316 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 10u: goto tr1;
		case 32u: goto tr1;
		case 35u: goto st2;
	}
	if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st9;
	} else if ( (*( _state.p)) >= 65u )
		goto st9;
	goto tr0;
st2:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof2;
case 2:
	switch( (*( _state.p)) ) {
		case 88u: goto st7;
		case 120u: goto st7;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st3;
	goto tr0;
st3:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof3;
case 3:
	if ( (*( _state.p)) == 59u )
		goto tr6;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st3;
	goto tr0;
tr6:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st170;
st170:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof170;
case 170:
#line 358 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto tr382;
		case 38u: goto tr384;
		case 60u: goto tr382;
		case 62u: goto tr382;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr385;
		} else if ( (*( _state.p)) >= 128u )
			goto tr382;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto tr382;
		} else if ( (*( _state.p)) >= 240u )
			goto tr387;
	} else
		goto tr386;
	goto tr383;
tr373:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st4;
tr379:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st4;
tr385:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st4;
tr391:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st4;
tr397:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st4;
st4:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof4;
case 4:
#line 412 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto tr1;
	goto tr0;
tr374:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st5;
tr380:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st5;
tr386:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st5;
tr392:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st5;
tr398:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st5;
st5:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof5;
case 5:
#line 448 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st4;
	goto tr0;
tr375:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st6;
tr381:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st6;
tr387:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st6;
tr393:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st6;
tr399:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st6;
st6:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof6;
case 6:
#line 484 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st5;
	goto tr0;
st7:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof7;
case 7:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st8;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st8;
	} else
		goto st8;
	goto tr0;
st8:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof8;
case 8:
	if ( (*( _state.p)) == 59u )
		goto tr10;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st8;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st8;
	} else
		goto st8;
	goto tr0;
tr10:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st171;
st171:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof171;
case 171:
#line 526 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto tr388;
		case 38u: goto tr390;
		case 60u: goto tr388;
		case 62u: goto tr388;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr391;
		} else if ( (*( _state.p)) >= 128u )
			goto tr388;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto tr388;
		} else if ( (*( _state.p)) >= 240u )
			goto tr393;
	} else
		goto tr392;
	goto tr389;
st9:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof9;
case 9:
	if ( (*( _state.p)) == 59u )
		goto tr12;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st10;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st10;
	} else
		goto st10;
	goto tr0;
st10:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof10;
case 10:
	if ( (*( _state.p)) == 59u )
		goto tr12;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st11;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st11;
	} else
		goto st11;
	goto tr0;
st11:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof11;
case 11:
	if ( (*( _state.p)) == 59u )
		goto tr12;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st12;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st12;
	} else
		goto st12;
	goto tr0;
st12:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof12;
case 12:
	if ( (*( _state.p)) == 59u )
		goto tr12;
	goto tr0;
tr12:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st172;
st172:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof172;
case 172:
#line 610 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto tr394;
		case 38u: goto tr396;
		case 60u: goto tr394;
		case 62u: goto tr394;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr397;
		} else if ( (*( _state.p)) >= 128u )
			goto tr394;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto tr394;
		} else if ( (*( _state.p)) >= 240u )
			goto tr399;
	} else
		goto tr398;
	goto tr395;
st13:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof13;
case 13:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto st0;
		case 32u: goto st0;
		case 33u: goto st78;
		case 38u: goto st0;
		case 47u: goto st98;
		case 63u: goto st104;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto st0;
		} else if ( (*( _state.p)) > 62u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr21;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr22;
	} else
		goto tr20;
	goto tr15;
tr15:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st14;
st14:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof14;
case 14:
#line 674 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr24;
		case 32u: goto tr24;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto tr25;
		case 62u: goto tr26;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto tr24;
		} else if ( (*( _state.p)) > 63u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto st76;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto st77;
	} else
		goto st75;
	goto st14;
tr24:
#line 20 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_end(( _state.p)); }
	goto st15;
tr53:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st15;
tr66:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st15;
tr79:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st15;
tr91:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st15;
tr110:
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st15;
st15:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof15;
case 15:
#line 741 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto st15;
		case 32u: goto st15;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto st39;
		case 62u: goto tr33;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto st15;
		} else if ( (*( _state.p)) > 63u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr35;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr36;
	} else
		goto tr34;
	goto tr30;
tr30:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st16;
st16:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof16;
case 16:
#line 780 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr38;
		case 32u: goto tr38;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto st0;
		case 61u: goto tr39;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto tr38;
		} else if ( (*( _state.p)) > 63u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto st73;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto st74;
	} else
		goto st72;
	goto st16;
tr38:
#line 13 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_end(( _state.p)); }
	goto st17;
st17:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof17;
case 17:
#line 819 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto st17;
		case 32u: goto st17;
		case 61u: goto st18;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto st17;
	goto st0;
tr39:
#line 13 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_end(( _state.p)); }
	goto st18;
st18:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof18;
case 18:
#line 836 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto st18;
		case 32u: goto st18;
		case 34u: goto st36;
		case 38u: goto tr47;
		case 39u: goto st55;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto st18;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr50;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr51;
	} else
		goto tr49;
	goto tr45;
tr45:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st19;
tr52:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st19;
tr65:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st19;
tr78:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st19;
tr90:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st19;
st19:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof19;
case 19:
#line 897 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr53;
		case 32u: goto tr53;
		case 34u: goto st0;
		case 38u: goto tr54;
		case 39u: goto st0;
		case 47u: goto tr55;
		case 60u: goto st0;
		case 62u: goto tr56;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto tr53;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr58;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr59;
	} else
		goto tr57;
	goto tr52;
tr47:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st20;
tr54:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st20;
tr67:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st20;
tr80:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st20;
tr92:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st20;
st20:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof20;
case 20:
#line 959 "schwa/formats/sgml_gen.cc"
	if ( (*( _state.p)) == 35u )
		goto st21;
	if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st31;
	} else if ( (*( _state.p)) >= 65u )
		goto st31;
	goto st0;
st21:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof21;
case 21:
	switch( (*( _state.p)) ) {
		case 88u: goto st28;
		case 120u: goto st28;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st22;
	goto st0;
st22:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof22;
case 22:
	if ( (*( _state.p)) == 59u )
		goto st23;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st22;
	goto st0;
st23:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof23;
case 23:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr66;
		case 32u: goto tr66;
		case 34u: goto st0;
		case 38u: goto tr67;
		case 39u: goto st0;
		case 47u: goto tr68;
		case 60u: goto st0;
		case 62u: goto tr69;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto tr66;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr71;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr72;
	} else
		goto tr70;
	goto tr65;
tr55:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st24;
tr68:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st24;
tr81:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st24;
tr93:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st24;
st24:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof24;
case 24:
#line 1057 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr53;
		case 32u: goto tr53;
		case 34u: goto st0;
		case 38u: goto tr54;
		case 39u: goto st0;
		case 47u: goto tr55;
		case 60u: goto st0;
		case 62u: goto tr56;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto tr53;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr58;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr59;
	} else
		goto tr57;
	goto tr52;
tr49:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st25;
tr57:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st25;
tr70:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st25;
tr83:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st25;
tr95:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st25;
st25:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof25;
case 25:
#line 1119 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st19;
	goto st0;
tr50:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st26;
tr58:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st26;
tr71:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st26;
tr84:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st26;
tr96:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st26;
st26:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof26;
case 26:
#line 1155 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st25;
	goto st0;
tr51:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st27;
tr59:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st27;
tr72:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st27;
tr85:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st27;
tr97:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st27;
st27:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof27;
case 27:
#line 1191 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st26;
	goto st0;
st28:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof28;
case 28:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st29;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st29;
	} else
		goto st29;
	goto st0;
st29:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof29;
case 29:
	if ( (*( _state.p)) == 59u )
		goto st30;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st29;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st29;
	} else
		goto st29;
	goto st0;
st30:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof30;
case 30:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr79;
		case 32u: goto tr79;
		case 34u: goto st0;
		case 38u: goto tr80;
		case 39u: goto st0;
		case 47u: goto tr81;
		case 60u: goto st0;
		case 62u: goto tr82;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto tr79;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr84;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr85;
	} else
		goto tr83;
	goto tr78;
st31:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof31;
case 31:
	if ( (*( _state.p)) == 59u )
		goto st35;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st32;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st32;
	} else
		goto st32;
	goto st0;
st32:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof32;
case 32:
	if ( (*( _state.p)) == 59u )
		goto st35;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st33;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st33;
	} else
		goto st33;
	goto st0;
st33:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof33;
case 33:
	if ( (*( _state.p)) == 59u )
		goto st35;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st34;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st34;
	} else
		goto st34;
	goto st0;
st34:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof34;
case 34:
	if ( (*( _state.p)) == 59u )
		goto st35;
	goto st0;
st35:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof35;
case 35:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr91;
		case 32u: goto tr91;
		case 34u: goto st0;
		case 38u: goto tr92;
		case 39u: goto st0;
		case 47u: goto tr93;
		case 60u: goto st0;
		case 62u: goto tr94;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto tr91;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr96;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr97;
	} else
		goto tr95;
	goto tr90;
st36:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof36;
case 36:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto st38;
		case 38u: goto tr100;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr101;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr103;
	} else
		goto tr102;
	goto tr98;
tr98:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st37;
tr104:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st37;
tr119:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st37;
tr130:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st37;
tr140:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st37;
st37:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof37;
case 37:
#line 1399 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr105;
		case 38u: goto tr106;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr107;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr109;
	} else
		goto tr108;
	goto tr104;
tr105:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
	goto st38;
tr120:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
	goto st38;
tr131:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
	goto st38;
tr141:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
	goto st38;
st38:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof38;
case 38:
#line 1442 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto tr110;
		case 32u: goto tr110;
		case 47u: goto tr111;
		case 62u: goto tr112;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto tr110;
	goto st0;
tr25:
#line 20 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_end(( _state.p)); }
	goto st39;
tr111:
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st39;
st39:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof39;
case 39:
#line 1464 "schwa/formats/sgml_gen.cc"
	if ( (*( _state.p)) == 62u )
		goto tr113;
	goto st0;
tr100:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st40;
tr106:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st40;
tr121:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st40;
tr132:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st40;
tr142:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st40;
st40:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof40;
case 40:
#line 1500 "schwa/formats/sgml_gen.cc"
	if ( (*( _state.p)) == 35u )
		goto st41;
	if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st50;
	} else if ( (*( _state.p)) >= 65u )
		goto st50;
	goto st0;
st41:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof41;
case 41:
	switch( (*( _state.p)) ) {
		case 88u: goto st47;
		case 120u: goto st47;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st42;
	goto st0;
st42:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof42;
case 42:
	if ( (*( _state.p)) == 59u )
		goto st43;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st42;
	goto st0;
st43:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof43;
case 43:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr120;
		case 38u: goto tr121;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr122;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr124;
	} else
		goto tr123;
	goto tr119;
tr101:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st44;
tr107:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st44;
tr122:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st44;
tr133:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st44;
tr143:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st44;
st44:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof44;
case 44:
#line 1587 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st37;
	goto st0;
tr102:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st45;
tr108:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st45;
tr123:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st45;
tr134:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st45;
tr144:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st45;
st45:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof45;
case 45:
#line 1623 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st44;
	goto st0;
tr103:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st46;
tr109:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st46;
tr124:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st46;
tr135:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st46;
tr145:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st46;
st46:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof46;
case 46:
#line 1659 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st45;
	goto st0;
st47:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof47;
case 47:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st48;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st48;
	} else
		goto st48;
	goto st0;
st48:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof48;
case 48:
	if ( (*( _state.p)) == 59u )
		goto st49;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st48;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st48;
	} else
		goto st48;
	goto st0;
st49:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof49;
case 49:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr131;
		case 38u: goto tr132;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr133;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr135;
	} else
		goto tr134;
	goto tr130;
st50:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof50;
case 50:
	if ( (*( _state.p)) == 59u )
		goto st54;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st51;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st51;
	} else
		goto st51;
	goto st0;
st51:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof51;
case 51:
	if ( (*( _state.p)) == 59u )
		goto st54;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st52;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st52;
	} else
		goto st52;
	goto st0;
st52:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof52;
case 52:
	if ( (*( _state.p)) == 59u )
		goto st54;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st53;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st53;
	} else
		goto st53;
	goto st0;
st53:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof53;
case 53:
	if ( (*( _state.p)) == 59u )
		goto st54;
	goto st0;
st54:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof54;
case 54:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr141;
		case 38u: goto tr142;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr143;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr145;
	} else
		goto tr144;
	goto tr140;
st55:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof55;
case 55:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr147;
		case 39u: goto st38;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr148;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr150;
	} else
		goto tr149;
	goto tr146;
tr146:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st56;
tr151:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st56;
tr161:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st56;
tr171:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st56;
tr180:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st56;
st56:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof56;
case 56:
#line 1853 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr152;
		case 39u: goto tr105;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr153;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr155;
	} else
		goto tr154;
	goto tr151;
tr147:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st57;
tr152:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st57;
tr162:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st57;
tr172:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st57;
tr181:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st57;
st57:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof57;
case 57:
#line 1908 "schwa/formats/sgml_gen.cc"
	if ( (*( _state.p)) == 35u )
		goto st58;
	if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st67;
	} else if ( (*( _state.p)) >= 65u )
		goto st67;
	goto st0;
st58:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof58;
case 58:
	switch( (*( _state.p)) ) {
		case 88u: goto st64;
		case 120u: goto st64;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st59;
	goto st0;
st59:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof59;
case 59:
	if ( (*( _state.p)) == 59u )
		goto st60;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st59;
	goto st0;
st60:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof60;
case 60:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr162;
		case 39u: goto tr120;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr163;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr165;
	} else
		goto tr164;
	goto tr161;
tr148:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st61;
tr153:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st61;
tr163:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st61;
tr173:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st61;
tr182:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st61;
st61:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof61;
case 61:
#line 1995 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st56;
	goto st0;
tr149:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st62;
tr154:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st62;
tr164:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st62;
tr174:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st62;
tr183:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st62;
st62:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof62;
case 62:
#line 2031 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st61;
	goto st0;
tr150:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st63;
tr155:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st63;
tr165:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st63;
tr175:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st63;
tr184:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st63;
st63:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof63;
case 63:
#line 2067 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st62;
	goto st0;
st64:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof64;
case 64:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st65;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st65;
	} else
		goto st65;
	goto st0;
st65:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof65;
case 65:
	if ( (*( _state.p)) == 59u )
		goto st66;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st65;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st65;
	} else
		goto st65;
	goto st0;
st66:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof66;
case 66:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr172;
		case 39u: goto tr131;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr173;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr175;
	} else
		goto tr174;
	goto tr171;
st67:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof67;
case 67:
	if ( (*( _state.p)) == 59u )
		goto st71;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st68;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st68;
	} else
		goto st68;
	goto st0;
st68:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof68;
case 68:
	if ( (*( _state.p)) == 59u )
		goto st71;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st69;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st69;
	} else
		goto st69;
	goto st0;
st69:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof69;
case 69:
	if ( (*( _state.p)) == 59u )
		goto st71;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st70;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st70;
	} else
		goto st70;
	goto st0;
st70:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof70;
case 70:
	if ( (*( _state.p)) == 59u )
		goto st71;
	goto st0;
st71:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof71;
case 71:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr181;
		case 39u: goto tr141;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr182;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr184;
	} else
		goto tr183;
	goto tr180;
tr34:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st72;
st72:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof72;
case 72:
#line 2211 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st16;
	goto st0;
tr35:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st73;
st73:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof73;
case 73:
#line 2223 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st72;
	goto st0;
tr36:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st74;
st74:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof74;
case 74:
#line 2235 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st73;
	goto st0;
tr20:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st75;
st75:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof75;
case 75:
#line 2247 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st14;
	goto st0;
tr21:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st76;
st76:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof76;
case 76:
#line 2259 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st75;
	goto st0;
tr22:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st77;
st77:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof77;
case 77:
#line 2271 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st76;
	goto st0;
st78:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof78;
case 78:
	switch( (*( _state.p)) ) {
		case 45u: goto st79;
		case 91u: goto st86;
	}
	goto st0;
st79:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof79;
case 79:
	if ( (*( _state.p)) == 45u )
		goto st80;
	goto st0;
st80:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof80;
case 80:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 45u: goto st81;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st83;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st85;
	} else
		goto st84;
	goto st80;
st81:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof81;
case 81:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 45u: goto st82;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st83;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st85;
	} else
		goto st84;
	goto st80;
st82:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof82;
case 82:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 45u: goto st82;
		case 62u: goto tr193;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st83;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st85;
	} else
		goto st84;
	goto st80;
st83:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof83;
case 83:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st80;
	goto st0;
st84:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof84;
case 84:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st83;
	goto st0;
st85:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof85;
case 85:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st84;
	goto st0;
st86:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof86;
case 86:
	if ( (*( _state.p)) == 67u )
		goto st87;
	goto st0;
st87:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof87;
case 87:
	if ( (*( _state.p)) == 68u )
		goto st88;
	goto st0;
st88:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof88;
case 88:
	if ( (*( _state.p)) == 65u )
		goto st89;
	goto st0;
st89:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof89;
case 89:
	if ( (*( _state.p)) == 84u )
		goto st90;
	goto st0;
st90:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof90;
case 90:
	if ( (*( _state.p)) == 65u )
		goto st91;
	goto st0;
st91:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof91;
case 91:
	if ( (*( _state.p)) == 91u )
		goto st92;
	goto st0;
st92:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof92;
case 92:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 93u: goto st93;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st95;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st97;
	} else
		goto st96;
	goto st92;
st93:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof93;
case 93:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 93u: goto st94;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st95;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st97;
	} else
		goto st96;
	goto st92;
st94:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof94;
case 94:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 62u: goto tr205;
		case 93u: goto st94;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st95;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st97;
	} else
		goto st96;
	goto st92;
st95:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof95;
case 95:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st92;
	goto st0;
st96:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof96;
case 96:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st95;
	goto st0;
st97:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof97;
case 97:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st96;
	goto st0;
st98:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof98;
case 98:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto st0;
		case 38u: goto st0;
		case 47u: goto st0;
	}
	if ( (*( _state.p)) < 128u ) {
		if ( (*( _state.p)) < 32u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto st0;
		} else if ( (*( _state.p)) > 33u ) {
			if ( 60u <= (*( _state.p)) && (*( _state.p)) <= 63u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 191u ) {
		if ( (*( _state.p)) < 224u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr207;
		} else if ( (*( _state.p)) > 239u ) {
			if ( (*( _state.p)) > 247u ) {
				if ( 248u <= (*( _state.p)) )
					goto st0;
			} else if ( (*( _state.p)) >= 240u )
				goto tr209;
		} else
			goto tr208;
	} else
		goto st0;
	goto tr206;
tr206:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st99;
st99:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof99;
case 99:
#line 2557 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr211;
		case 32u: goto tr211;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto st0;
		case 62u: goto tr212;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto tr211;
		} else if ( (*( _state.p)) > 63u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto st102;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto st103;
	} else
		goto st101;
	goto st99;
tr211:
#line 20 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_end(( _state.p)); }
	goto st100;
st100:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof100;
case 100:
#line 2596 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto st100;
		case 32u: goto st100;
		case 62u: goto tr217;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto st100;
	goto st0;
tr207:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st101;
st101:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof101;
case 101:
#line 2613 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st99;
	goto st0;
tr208:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st102;
st102:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof102;
case 102:
#line 2625 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st101;
	goto st0;
tr209:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st103;
st103:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof103;
case 103:
#line 2637 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st102;
	goto st0;
st104:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof104;
case 104:
	switch( (*( _state.p)) ) {
		case 88u: goto st105;
		case 120u: goto st105;
	}
	goto st0;
st105:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof105;
case 105:
	switch( (*( _state.p)) ) {
		case 77u: goto st106;
		case 109u: goto st106;
	}
	goto st0;
st106:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof106;
case 106:
	switch( (*( _state.p)) ) {
		case 76u: goto st107;
		case 108u: goto st107;
	}
	goto st0;
st107:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof107;
case 107:
	switch( (*( _state.p)) ) {
		case 13u: goto st108;
		case 32u: goto st108;
		case 63u: goto st132;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto st108;
	goto st0;
tr243:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st108;
tr255:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st108;
tr268:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st108;
tr279:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st108;
tr297:
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st108;
st108:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof108;
case 108:
#line 2712 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto st108;
		case 32u: goto st108;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto st0;
		case 63u: goto st132;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto st108;
		} else if ( (*( _state.p)) > 62u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr225;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr226;
	} else
		goto tr224;
	goto tr223;
tr223:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st109;
st109:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof109;
case 109:
#line 2751 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr228;
		case 32u: goto tr228;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto st0;
		case 61u: goto tr229;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto tr228;
		} else if ( (*( _state.p)) > 63u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto st166;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto st167;
	} else
		goto st165;
	goto st109;
tr228:
#line 13 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_end(( _state.p)); }
	goto st110;
st110:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof110;
case 110:
#line 2790 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto st110;
		case 32u: goto st110;
		case 61u: goto st111;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto st110;
	goto st0;
tr229:
#line 13 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_end(( _state.p)); }
	goto st111;
st111:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof111;
case 111:
#line 2807 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto st111;
		case 32u: goto st111;
		case 34u: goto st129;
		case 38u: goto tr237;
		case 39u: goto st148;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto st111;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr240;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr241;
	} else
		goto tr239;
	goto tr235;
tr235:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st112;
tr242:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st112;
tr254:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st112;
tr267:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st112;
tr278:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st112;
st112:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof112;
case 112:
#line 2868 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr243;
		case 32u: goto tr243;
		case 34u: goto st0;
		case 38u: goto tr244;
		case 39u: goto st0;
		case 60u: goto st0;
		case 62u: goto st0;
		case 63u: goto tr245;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto tr243;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr247;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr248;
	} else
		goto tr246;
	goto tr242;
tr237:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st113;
tr244:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st113;
tr256:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st113;
tr269:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st113;
tr280:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st113;
st113:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof113;
case 113:
#line 2930 "schwa/formats/sgml_gen.cc"
	if ( (*( _state.p)) == 35u )
		goto st114;
	if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st124;
	} else if ( (*( _state.p)) >= 65u )
		goto st124;
	goto st0;
st114:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof114;
case 114:
	switch( (*( _state.p)) ) {
		case 88u: goto st121;
		case 120u: goto st121;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st115;
	goto st0;
st115:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof115;
case 115:
	if ( (*( _state.p)) == 59u )
		goto st116;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st115;
	goto st0;
st116:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof116;
case 116:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr255;
		case 32u: goto tr255;
		case 34u: goto st0;
		case 38u: goto tr256;
		case 39u: goto st0;
		case 60u: goto st0;
		case 62u: goto st0;
		case 63u: goto tr257;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto tr255;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr259;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr260;
	} else
		goto tr258;
	goto tr254;
tr245:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st117;
tr257:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st117;
tr270:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st117;
tr281:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st117;
st117:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof117;
case 117:
#line 3028 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr243;
		case 32u: goto tr243;
		case 34u: goto st0;
		case 38u: goto tr244;
		case 39u: goto st0;
		case 60u: goto st0;
		case 62u: goto tr261;
		case 63u: goto tr245;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto tr243;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr247;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr248;
	} else
		goto tr246;
	goto tr242;
tr239:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st118;
tr246:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st118;
tr258:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st118;
tr271:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st118;
tr282:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st118;
st118:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof118;
case 118:
#line 3090 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st112;
	goto st0;
tr240:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st119;
tr247:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st119;
tr259:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st119;
tr272:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st119;
tr283:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st119;
st119:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof119;
case 119:
#line 3126 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st118;
	goto st0;
tr241:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st120;
tr248:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st120;
tr260:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st120;
tr273:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st120;
tr284:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st120;
st120:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof120;
case 120:
#line 3162 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st119;
	goto st0;
st121:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof121;
case 121:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st122;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st122;
	} else
		goto st122;
	goto st0;
st122:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof122;
case 122:
	if ( (*( _state.p)) == 59u )
		goto st123;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st122;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st122;
	} else
		goto st122;
	goto st0;
st123:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof123;
case 123:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr268;
		case 32u: goto tr268;
		case 34u: goto st0;
		case 38u: goto tr269;
		case 39u: goto st0;
		case 60u: goto st0;
		case 62u: goto st0;
		case 63u: goto tr270;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto tr268;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr272;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr273;
	} else
		goto tr271;
	goto tr267;
st124:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof124;
case 124:
	if ( (*( _state.p)) == 59u )
		goto st128;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st125;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st125;
	} else
		goto st125;
	goto st0;
st125:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof125;
case 125:
	if ( (*( _state.p)) == 59u )
		goto st128;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st126;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st126;
	} else
		goto st126;
	goto st0;
st126:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof126;
case 126:
	if ( (*( _state.p)) == 59u )
		goto st128;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st127;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st127;
	} else
		goto st127;
	goto st0;
st127:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof127;
case 127:
	if ( (*( _state.p)) == 59u )
		goto st128;
	goto st0;
st128:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof128;
case 128:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr279;
		case 32u: goto tr279;
		case 34u: goto st0;
		case 38u: goto tr280;
		case 39u: goto st0;
		case 60u: goto st0;
		case 62u: goto st0;
		case 63u: goto tr281;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) > 10u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else if ( (*( _state.p)) >= 9u )
			goto tr279;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr283;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr284;
	} else
		goto tr282;
	goto tr278;
st129:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof129;
case 129:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto st131;
		case 38u: goto tr287;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr288;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr290;
	} else
		goto tr289;
	goto tr285;
tr285:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st130;
tr291:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st130;
tr304:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st130;
tr315:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st130;
tr325:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st130;
st130:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof130;
case 130:
#line 3370 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr292;
		case 38u: goto tr293;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr294;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr296;
	} else
		goto tr295;
	goto tr291;
tr292:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
	goto st131;
tr305:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
	goto st131;
tr316:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
	goto st131;
tr326:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
	goto st131;
st131:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof131;
case 131:
#line 3413 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto tr297;
		case 32u: goto tr297;
		case 63u: goto tr298;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto tr297;
	goto st0;
tr298:
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st132;
st132:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof132;
case 132:
#line 3430 "schwa/formats/sgml_gen.cc"
	if ( (*( _state.p)) == 62u )
		goto tr261;
	goto st0;
tr287:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st133;
tr293:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st133;
tr306:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st133;
tr317:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st133;
tr327:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st133;
st133:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof133;
case 133:
#line 3466 "schwa/formats/sgml_gen.cc"
	if ( (*( _state.p)) == 35u )
		goto st134;
	if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st143;
	} else if ( (*( _state.p)) >= 65u )
		goto st143;
	goto st0;
st134:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof134;
case 134:
	switch( (*( _state.p)) ) {
		case 88u: goto st140;
		case 120u: goto st140;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st135;
	goto st0;
st135:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof135;
case 135:
	if ( (*( _state.p)) == 59u )
		goto st136;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st135;
	goto st0;
st136:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof136;
case 136:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr305;
		case 38u: goto tr306;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr307;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr309;
	} else
		goto tr308;
	goto tr304;
tr288:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st137;
tr294:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st137;
tr307:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st137;
tr318:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st137;
tr328:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st137;
st137:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof137;
case 137:
#line 3553 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st130;
	goto st0;
tr289:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st138;
tr295:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st138;
tr308:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st138;
tr319:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st138;
tr329:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st138;
st138:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof138;
case 138:
#line 3589 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st137;
	goto st0;
tr290:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st139;
tr296:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st139;
tr309:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st139;
tr320:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st139;
tr330:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st139;
st139:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof139;
case 139:
#line 3625 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st138;
	goto st0;
st140:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof140;
case 140:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st141;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st141;
	} else
		goto st141;
	goto st0;
st141:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof141;
case 141:
	if ( (*( _state.p)) == 59u )
		goto st142;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st141;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st141;
	} else
		goto st141;
	goto st0;
st142:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof142;
case 142:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr316;
		case 38u: goto tr317;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr318;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr320;
	} else
		goto tr319;
	goto tr315;
st143:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof143;
case 143:
	if ( (*( _state.p)) == 59u )
		goto st147;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st144;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st144;
	} else
		goto st144;
	goto st0;
st144:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof144;
case 144:
	if ( (*( _state.p)) == 59u )
		goto st147;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st145;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st145;
	} else
		goto st145;
	goto st0;
st145:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof145;
case 145:
	if ( (*( _state.p)) == 59u )
		goto st147;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st146;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st146;
	} else
		goto st146;
	goto st0;
st146:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof146;
case 146:
	if ( (*( _state.p)) == 59u )
		goto st147;
	goto st0;
st147:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof147;
case 147:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr326;
		case 38u: goto tr327;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr328;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr330;
	} else
		goto tr329;
	goto tr325;
st148:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof148;
case 148:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr332;
		case 39u: goto st131;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr333;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr335;
	} else
		goto tr334;
	goto tr331;
tr331:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st149;
tr336:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st149;
tr346:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st149;
tr356:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st149;
tr365:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st149;
st149:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof149;
case 149:
#line 3819 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr337;
		case 39u: goto tr292;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr338;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr340;
	} else
		goto tr339;
	goto tr336;
tr332:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st150;
tr337:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st150;
tr347:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st150;
tr357:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st150;
tr366:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st150;
st150:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof150;
case 150:
#line 3874 "schwa/formats/sgml_gen.cc"
	if ( (*( _state.p)) == 35u )
		goto st151;
	if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st160;
	} else if ( (*( _state.p)) >= 65u )
		goto st160;
	goto st0;
st151:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof151;
case 151:
	switch( (*( _state.p)) ) {
		case 88u: goto st157;
		case 120u: goto st157;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st152;
	goto st0;
st152:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof152;
case 152:
	if ( (*( _state.p)) == 59u )
		goto st153;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st152;
	goto st0;
st153:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof153;
case 153:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr347;
		case 39u: goto tr305;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr348;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr350;
	} else
		goto tr349;
	goto tr346;
tr333:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st154;
tr338:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st154;
tr348:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st154;
tr358:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st154;
tr367:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st154;
st154:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof154;
case 154:
#line 3961 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st149;
	goto st0;
tr334:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st155;
tr339:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st155;
tr349:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st155;
tr359:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st155;
tr368:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st155;
st155:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof155;
case 155:
#line 3997 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st154;
	goto st0;
tr335:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st156;
tr340:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st156;
tr350:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st156;
tr360:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st156;
tr369:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st156;
st156:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof156;
case 156:
#line 4033 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st155;
	goto st0;
st157:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof157;
case 157:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st158;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st158;
	} else
		goto st158;
	goto st0;
st158:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof158;
case 158:
	if ( (*( _state.p)) == 59u )
		goto st159;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st158;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st158;
	} else
		goto st158;
	goto st0;
st159:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof159;
case 159:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr357;
		case 39u: goto tr316;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr358;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr360;
	} else
		goto tr359;
	goto tr356;
st160:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof160;
case 160:
	if ( (*( _state.p)) == 59u )
		goto st164;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st161;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st161;
	} else
		goto st161;
	goto st0;
st161:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof161;
case 161:
	if ( (*( _state.p)) == 59u )
		goto st164;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st162;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st162;
	} else
		goto st162;
	goto st0;
st162:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof162;
case 162:
	if ( (*( _state.p)) == 59u )
		goto st164;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st163;
	} else if ( (*( _state.p)) > 90u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 122u )
			goto st163;
	} else
		goto st163;
	goto st0;
st163:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof163;
case 163:
	if ( (*( _state.p)) == 59u )
		goto st164;
	goto st0;
st164:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof164;
case 164:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr366;
		case 39u: goto tr326;
		case 60u: goto st0;
		case 62u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr367;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr369;
	} else
		goto tr368;
	goto tr365;
tr224:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st165;
st165:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof165;
case 165:
#line 4177 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st109;
	goto st0;
tr225:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st166;
st166:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof166;
case 166:
#line 4189 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st165;
	goto st0;
tr226:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st167;
st167:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof167;
case 167:
#line 4201 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st166;
	goto st0;
	}
	_test_eof168:  _state.cs = 168; goto _test_eof; 
	_test_eof169:  _state.cs = 169; goto _test_eof; 
	_test_eof1:  _state.cs = 1; goto _test_eof; 
	_test_eof2:  _state.cs = 2; goto _test_eof; 
	_test_eof3:  _state.cs = 3; goto _test_eof; 
	_test_eof170:  _state.cs = 170; goto _test_eof; 
	_test_eof4:  _state.cs = 4; goto _test_eof; 
	_test_eof5:  _state.cs = 5; goto _test_eof; 
	_test_eof6:  _state.cs = 6; goto _test_eof; 
	_test_eof7:  _state.cs = 7; goto _test_eof; 
	_test_eof8:  _state.cs = 8; goto _test_eof; 
	_test_eof171:  _state.cs = 171; goto _test_eof; 
	_test_eof9:  _state.cs = 9; goto _test_eof; 
	_test_eof10:  _state.cs = 10; goto _test_eof; 
	_test_eof11:  _state.cs = 11; goto _test_eof; 
	_test_eof12:  _state.cs = 12; goto _test_eof; 
	_test_eof172:  _state.cs = 172; goto _test_eof; 
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
	_test_eof41:  _state.cs = 41; goto _test_eof; 
	_test_eof42:  _state.cs = 42; goto _test_eof; 
	_test_eof43:  _state.cs = 43; goto _test_eof; 
	_test_eof44:  _state.cs = 44; goto _test_eof; 
	_test_eof45:  _state.cs = 45; goto _test_eof; 
	_test_eof46:  _state.cs = 46; goto _test_eof; 
	_test_eof47:  _state.cs = 47; goto _test_eof; 
	_test_eof48:  _state.cs = 48; goto _test_eof; 
	_test_eof49:  _state.cs = 49; goto _test_eof; 
	_test_eof50:  _state.cs = 50; goto _test_eof; 
	_test_eof51:  _state.cs = 51; goto _test_eof; 
	_test_eof52:  _state.cs = 52; goto _test_eof; 
	_test_eof53:  _state.cs = 53; goto _test_eof; 
	_test_eof54:  _state.cs = 54; goto _test_eof; 
	_test_eof55:  _state.cs = 55; goto _test_eof; 
	_test_eof56:  _state.cs = 56; goto _test_eof; 
	_test_eof57:  _state.cs = 57; goto _test_eof; 
	_test_eof58:  _state.cs = 58; goto _test_eof; 
	_test_eof59:  _state.cs = 59; goto _test_eof; 
	_test_eof60:  _state.cs = 60; goto _test_eof; 
	_test_eof61:  _state.cs = 61; goto _test_eof; 
	_test_eof62:  _state.cs = 62; goto _test_eof; 
	_test_eof63:  _state.cs = 63; goto _test_eof; 
	_test_eof64:  _state.cs = 64; goto _test_eof; 
	_test_eof65:  _state.cs = 65; goto _test_eof; 
	_test_eof66:  _state.cs = 66; goto _test_eof; 
	_test_eof67:  _state.cs = 67; goto _test_eof; 
	_test_eof68:  _state.cs = 68; goto _test_eof; 
	_test_eof69:  _state.cs = 69; goto _test_eof; 
	_test_eof70:  _state.cs = 70; goto _test_eof; 
	_test_eof71:  _state.cs = 71; goto _test_eof; 
	_test_eof72:  _state.cs = 72; goto _test_eof; 
	_test_eof73:  _state.cs = 73; goto _test_eof; 
	_test_eof74:  _state.cs = 74; goto _test_eof; 
	_test_eof75:  _state.cs = 75; goto _test_eof; 
	_test_eof76:  _state.cs = 76; goto _test_eof; 
	_test_eof77:  _state.cs = 77; goto _test_eof; 
	_test_eof78:  _state.cs = 78; goto _test_eof; 
	_test_eof79:  _state.cs = 79; goto _test_eof; 
	_test_eof80:  _state.cs = 80; goto _test_eof; 
	_test_eof81:  _state.cs = 81; goto _test_eof; 
	_test_eof82:  _state.cs = 82; goto _test_eof; 
	_test_eof83:  _state.cs = 83; goto _test_eof; 
	_test_eof84:  _state.cs = 84; goto _test_eof; 
	_test_eof85:  _state.cs = 85; goto _test_eof; 
	_test_eof86:  _state.cs = 86; goto _test_eof; 
	_test_eof87:  _state.cs = 87; goto _test_eof; 
	_test_eof88:  _state.cs = 88; goto _test_eof; 
	_test_eof89:  _state.cs = 89; goto _test_eof; 
	_test_eof90:  _state.cs = 90; goto _test_eof; 
	_test_eof91:  _state.cs = 91; goto _test_eof; 
	_test_eof92:  _state.cs = 92; goto _test_eof; 
	_test_eof93:  _state.cs = 93; goto _test_eof; 
	_test_eof94:  _state.cs = 94; goto _test_eof; 
	_test_eof95:  _state.cs = 95; goto _test_eof; 
	_test_eof96:  _state.cs = 96; goto _test_eof; 
	_test_eof97:  _state.cs = 97; goto _test_eof; 
	_test_eof98:  _state.cs = 98; goto _test_eof; 
	_test_eof99:  _state.cs = 99; goto _test_eof; 
	_test_eof100:  _state.cs = 100; goto _test_eof; 
	_test_eof101:  _state.cs = 101; goto _test_eof; 
	_test_eof102:  _state.cs = 102; goto _test_eof; 
	_test_eof103:  _state.cs = 103; goto _test_eof; 
	_test_eof104:  _state.cs = 104; goto _test_eof; 
	_test_eof105:  _state.cs = 105; goto _test_eof; 
	_test_eof106:  _state.cs = 106; goto _test_eof; 
	_test_eof107:  _state.cs = 107; goto _test_eof; 
	_test_eof108:  _state.cs = 108; goto _test_eof; 
	_test_eof109:  _state.cs = 109; goto _test_eof; 
	_test_eof110:  _state.cs = 110; goto _test_eof; 
	_test_eof111:  _state.cs = 111; goto _test_eof; 
	_test_eof112:  _state.cs = 112; goto _test_eof; 
	_test_eof113:  _state.cs = 113; goto _test_eof; 
	_test_eof114:  _state.cs = 114; goto _test_eof; 
	_test_eof115:  _state.cs = 115; goto _test_eof; 
	_test_eof116:  _state.cs = 116; goto _test_eof; 
	_test_eof117:  _state.cs = 117; goto _test_eof; 
	_test_eof118:  _state.cs = 118; goto _test_eof; 
	_test_eof119:  _state.cs = 119; goto _test_eof; 
	_test_eof120:  _state.cs = 120; goto _test_eof; 
	_test_eof121:  _state.cs = 121; goto _test_eof; 
	_test_eof122:  _state.cs = 122; goto _test_eof; 
	_test_eof123:  _state.cs = 123; goto _test_eof; 
	_test_eof124:  _state.cs = 124; goto _test_eof; 
	_test_eof125:  _state.cs = 125; goto _test_eof; 
	_test_eof126:  _state.cs = 126; goto _test_eof; 
	_test_eof127:  _state.cs = 127; goto _test_eof; 
	_test_eof128:  _state.cs = 128; goto _test_eof; 
	_test_eof129:  _state.cs = 129; goto _test_eof; 
	_test_eof130:  _state.cs = 130; goto _test_eof; 
	_test_eof131:  _state.cs = 131; goto _test_eof; 
	_test_eof132:  _state.cs = 132; goto _test_eof; 
	_test_eof133:  _state.cs = 133; goto _test_eof; 
	_test_eof134:  _state.cs = 134; goto _test_eof; 
	_test_eof135:  _state.cs = 135; goto _test_eof; 
	_test_eof136:  _state.cs = 136; goto _test_eof; 
	_test_eof137:  _state.cs = 137; goto _test_eof; 
	_test_eof138:  _state.cs = 138; goto _test_eof; 
	_test_eof139:  _state.cs = 139; goto _test_eof; 
	_test_eof140:  _state.cs = 140; goto _test_eof; 
	_test_eof141:  _state.cs = 141; goto _test_eof; 
	_test_eof142:  _state.cs = 142; goto _test_eof; 
	_test_eof143:  _state.cs = 143; goto _test_eof; 
	_test_eof144:  _state.cs = 144; goto _test_eof; 
	_test_eof145:  _state.cs = 145; goto _test_eof; 
	_test_eof146:  _state.cs = 146; goto _test_eof; 
	_test_eof147:  _state.cs = 147; goto _test_eof; 
	_test_eof148:  _state.cs = 148; goto _test_eof; 
	_test_eof149:  _state.cs = 149; goto _test_eof; 
	_test_eof150:  _state.cs = 150; goto _test_eof; 
	_test_eof151:  _state.cs = 151; goto _test_eof; 
	_test_eof152:  _state.cs = 152; goto _test_eof; 
	_test_eof153:  _state.cs = 153; goto _test_eof; 
	_test_eof154:  _state.cs = 154; goto _test_eof; 
	_test_eof155:  _state.cs = 155; goto _test_eof; 
	_test_eof156:  _state.cs = 156; goto _test_eof; 
	_test_eof157:  _state.cs = 157; goto _test_eof; 
	_test_eof158:  _state.cs = 158; goto _test_eof; 
	_test_eof159:  _state.cs = 159; goto _test_eof; 
	_test_eof160:  _state.cs = 160; goto _test_eof; 
	_test_eof161:  _state.cs = 161; goto _test_eof; 
	_test_eof162:  _state.cs = 162; goto _test_eof; 
	_test_eof163:  _state.cs = 163; goto _test_eof; 
	_test_eof164:  _state.cs = 164; goto _test_eof; 
	_test_eof165:  _state.cs = 165; goto _test_eof; 
	_test_eof166:  _state.cs = 166; goto _test_eof; 
	_test_eof167:  _state.cs = 167; goto _test_eof; 

	_test_eof: {}
	if ( ( _state.p) == ( _state.eof) )
	{
	switch (  _state.cs ) {
	case 169: goto tr376;
	case 1: goto tr0;
	case 2: goto tr0;
	case 3: goto tr0;
	case 170: goto tr382;
	case 4: goto tr0;
	case 5: goto tr0;
	case 6: goto tr0;
	case 7: goto tr0;
	case 8: goto tr0;
	case 171: goto tr388;
	case 9: goto tr0;
	case 10: goto tr0;
	case 11: goto tr0;
	case 12: goto tr0;
	case 172: goto tr394;
	}
	}

	_out: {}
	}

#line 101 "../ragel/formats/sgml/sgml.rl"

  // Fail if we ended up in an error state or we didn't consume any input.
  if (_state.cs == 
#line 4409 "schwa/formats/sgml_gen.cc"
0
#line 103 "../ragel/formats/sgml/sgml.rl"
)
    return nullptr;
  else if (_state.p == p_before)
    return nullptr;
  else
    return _node;
}

}  // namespace formats
}  // namespace schwa
