
#line 1 "../ragel/formats/sgml/sgml.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 70 "../ragel/formats/sgml/sgml.rl"



#include <schwa/formats/sgml.h>


namespace schwa {
namespace formats {


#line 19 "schwa/formats/sgml_gen.cc"
static const int sgml_start = 154;

static const int sgml_en_main = 154;


#line 80 "../ragel/formats/sgml/sgml.rl"

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

#line 85 "../ragel/formats/sgml/sgml.rl"
}


SGMLishNode *
SGMLishLexer::lex(void) {
  // Don't attempt to lex if we're at EOF.
  if (_state.at_eof())
    return nullptr;
  const auto p_before = _state.p;

  
#line 51 "schwa/formats/sgml_gen.cc"
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
	{{( _state.p) = (( _state.te))-1;} _create_text_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }
	break;
	}
	}
	goto st154;
tr30:
#line 20 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_end(( _state.p)); }
#line 62 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_start_tag_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr37:
#line 62 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_start_tag_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr65:
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
#line 62 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_start_tag_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr66:
#line 63 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_empty_tag_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr154:
#line 67 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_comment_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr166:
#line 66 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_cdata_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr173:
#line 20 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_end(( _state.p)); }
#line 64 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_end_tag_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr178:
#line 64 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_end_tag_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr212:
#line 68 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p)+1;{ _create_xml_decl_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr298:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_text_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr304:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_text_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr310:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_text_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
tr316:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.te = ( _state.p);( _state.p)--;{ _create_text_node(); {( _state.p)++;  _state.cs = 154; goto _out;} }}
	goto st154;
st154:
#line 1 "NONE"
	{ _state.ts = 0;}
#line 1 "NONE"
	{ _state.act = 0;}
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof154;
case 154:
#line 1 "NONE"
	{ _state.ts = ( _state.p);}
#line 145 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr293;
		case 60u: goto st17;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr295;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr297;
	} else
		goto tr296;
	goto tr292;
st0:
 _state.cs = 0;
	goto _out;
tr1:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st155;
tr292:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st155;
tr299:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st155;
tr305:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st155;
tr311:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st155;
tr317:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st155;
st155:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof155;
case 155:
#line 227 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto tr298;
		case 38u: goto tr300;
		case 60u: goto tr298;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr301;
		} else if ( (*( _state.p)) >= 128u )
			goto tr298;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto tr298;
		} else if ( (*( _state.p)) >= 240u )
			goto tr303;
	} else
		goto tr302;
	goto tr299;
tr293:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st1;
tr300:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st1;
tr306:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st1;
tr312:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st1;
tr318:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st1;
st1:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof1;
case 1:
#line 280 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 32u: goto tr1;
		case 35u: goto st2;
		case 65u: goto st9;
		case 71u: goto st14;
		case 76u: goto st14;
		case 81u: goto st15;
		case 97u: goto st9;
		case 103u: goto st14;
		case 108u: goto st14;
		case 113u: goto st15;
	}
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
		goto tr8;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st3;
	goto tr0;
tr8:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st156;
st156:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof156;
case 156:
#line 324 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto tr304;
		case 38u: goto tr306;
		case 60u: goto tr304;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr307;
		} else if ( (*( _state.p)) >= 128u )
			goto tr304;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto tr304;
		} else if ( (*( _state.p)) >= 240u )
			goto tr309;
	} else
		goto tr308;
	goto tr305;
tr295:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st4;
tr301:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st4;
tr307:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st4;
tr313:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st4;
tr319:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st4;
st4:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof4;
case 4:
#line 377 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto tr1;
	goto tr0;
tr296:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st5;
tr302:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st5;
tr308:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st5;
tr314:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st5;
tr320:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st5;
st5:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof5;
case 5:
#line 413 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st4;
	goto tr0;
tr297:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st6;
tr303:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st6;
tr309:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st6;
tr315:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st6;
tr321:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st6;
st6:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof6;
case 6:
#line 449 "schwa/formats/sgml_gen.cc"
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
		goto tr12;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st8;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st8;
	} else
		goto st8;
	goto tr0;
tr12:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st157;
st157:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof157;
case 157:
#line 491 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto tr310;
		case 38u: goto tr312;
		case 60u: goto tr310;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr313;
		} else if ( (*( _state.p)) >= 128u )
			goto tr310;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto tr310;
		} else if ( (*( _state.p)) >= 240u )
			goto tr315;
	} else
		goto tr314;
	goto tr311;
st9:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof9;
case 9:
	switch( (*( _state.p)) ) {
		case 77u: goto st10;
		case 80u: goto st12;
		case 109u: goto st10;
		case 112u: goto st12;
	}
	goto tr0;
st10:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof10;
case 10:
	switch( (*( _state.p)) ) {
		case 80u: goto st11;
		case 112u: goto st11;
	}
	goto tr0;
st11:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof11;
case 11:
	if ( (*( _state.p)) == 59u )
		goto tr16;
	goto tr0;
tr16:
#line 1 "NONE"
	{ _state.te = ( _state.p)+1;}
#line 65 "../ragel/formats/sgml/sgml.rl"
	{ _state.act = 4;}
	goto st158;
st158:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof158;
case 158:
#line 549 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto tr316;
		case 38u: goto tr318;
		case 60u: goto tr316;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr319;
		} else if ( (*( _state.p)) >= 128u )
			goto tr316;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto tr316;
		} else if ( (*( _state.p)) >= 240u )
			goto tr321;
	} else
		goto tr320;
	goto tr317;
st12:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof12;
case 12:
	switch( (*( _state.p)) ) {
		case 79u: goto st13;
		case 111u: goto st13;
	}
	goto tr0;
st13:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof13;
case 13:
	switch( (*( _state.p)) ) {
		case 83u: goto st11;
		case 115u: goto st11;
	}
	goto tr0;
st14:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof14;
case 14:
	switch( (*( _state.p)) ) {
		case 84u: goto st11;
		case 116u: goto st11;
	}
	goto tr0;
st15:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof15;
case 15:
	switch( (*( _state.p)) ) {
		case 85u: goto st16;
		case 117u: goto st16;
	}
	goto tr0;
st16:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof16;
case 16:
	switch( (*( _state.p)) ) {
		case 79u: goto st14;
		case 111u: goto st14;
	}
	goto tr0;
st17:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof17;
case 17:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto st0;
		case 32u: goto st0;
		case 33u: goto st73;
		case 38u: goto st0;
		case 47u: goto st93;
		case 63u: goto st99;
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
				goto tr25;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr26;
	} else
		goto tr24;
	goto tr19;
tr19:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st18;
st18:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof18;
case 18:
#line 657 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr28;
		case 32u: goto tr28;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto tr29;
		case 62u: goto tr30;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto tr28;
		} else if ( (*( _state.p)) > 63u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto st71;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto st72;
	} else
		goto st70;
	goto st18;
tr28:
#line 20 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_end(( _state.p)); }
	goto st19;
tr63:
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st19;
st19:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof19;
case 19:
#line 700 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto st19;
		case 32u: goto st19;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto st26;
		case 62u: goto tr37;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto st19;
		} else if ( (*( _state.p)) > 63u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr39;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr40;
	} else
		goto tr38;
	goto tr34;
tr34:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st20;
st20:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof20;
case 20:
#line 739 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr42;
		case 32u: goto tr42;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto st0;
		case 61u: goto tr43;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto tr42;
		} else if ( (*( _state.p)) > 63u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto st68;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto st69;
	} else
		goto st67;
	goto st20;
tr42:
#line 13 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_end(( _state.p)); }
	goto st21;
st21:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof21;
case 21:
#line 778 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto st21;
		case 32u: goto st21;
		case 61u: goto st22;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto st21;
	goto st0;
tr43:
#line 13 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_end(( _state.p)); }
	goto st22;
st22:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof22;
case 22:
#line 795 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto st22;
		case 32u: goto st22;
		case 34u: goto st23;
		case 39u: goto st46;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto st22;
	goto st0;
st23:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof23;
case 23:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto st25;
		case 38u: goto tr53;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr54;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr56;
	} else
		goto tr55;
	goto tr51;
tr51:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st24;
tr57:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st24;
tr74:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st24;
tr85:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st24;
tr95:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st24;
st24:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof24;
case 24:
#line 862 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr58;
		case 38u: goto tr59;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr60;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr62;
	} else
		goto tr61;
	goto tr57;
tr58:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
	goto st25;
tr75:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
	goto st25;
tr86:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
	goto st25;
tr96:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
	goto st25;
st25:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof25;
case 25:
#line 904 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto tr63;
		case 32u: goto tr63;
		case 47u: goto tr64;
		case 62u: goto tr65;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto tr63;
	goto st0;
tr29:
#line 20 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_end(( _state.p)); }
	goto st26;
tr64:
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st26;
st26:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof26;
case 26:
#line 926 "schwa/formats/sgml_gen.cc"
	if ( (*( _state.p)) == 62u )
		goto tr66;
	goto st0;
tr53:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st27;
tr59:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st27;
tr76:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st27;
tr87:
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
#line 962 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 35u: goto st28;
		case 65u: goto st37;
		case 71u: goto st43;
		case 76u: goto st43;
		case 81u: goto st44;
		case 97u: goto st37;
		case 103u: goto st43;
		case 108u: goto st43;
		case 113u: goto st44;
	}
	goto st0;
st28:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof28;
case 28:
	switch( (*( _state.p)) ) {
		case 88u: goto st34;
		case 120u: goto st34;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st29;
	goto st0;
st29:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof29;
case 29:
	if ( (*( _state.p)) == 59u )
		goto st30;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st29;
	goto st0;
st30:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof30;
case 30:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr75;
		case 38u: goto tr76;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr77;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr79;
	} else
		goto tr78;
	goto tr74;
tr54:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st31;
tr60:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st31;
tr77:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st31;
tr88:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st31;
tr98:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st31;
st31:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof31;
case 31:
#line 1052 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st24;
	goto st0;
tr55:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st32;
tr61:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st32;
tr78:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st32;
tr89:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st32;
tr99:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st32;
st32:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof32;
case 32:
#line 1088 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st31;
	goto st0;
tr56:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st33;
tr62:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st33;
tr79:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st33;
tr90:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st33;
tr100:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st33;
st33:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof33;
case 33:
#line 1124 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st32;
	goto st0;
st34:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof34;
case 34:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st35;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st35;
	} else
		goto st35;
	goto st0;
st35:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof35;
case 35:
	if ( (*( _state.p)) == 59u )
		goto st36;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st35;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st35;
	} else
		goto st35;
	goto st0;
st36:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof36;
case 36:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr86;
		case 38u: goto tr87;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr88;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr90;
	} else
		goto tr89;
	goto tr85;
st37:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof37;
case 37:
	switch( (*( _state.p)) ) {
		case 77u: goto st38;
		case 80u: goto st41;
		case 109u: goto st38;
		case 112u: goto st41;
	}
	goto st0;
st38:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof38;
case 38:
	switch( (*( _state.p)) ) {
		case 80u: goto st39;
		case 112u: goto st39;
	}
	goto st0;
st39:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof39;
case 39:
	if ( (*( _state.p)) == 59u )
		goto st40;
	goto st0;
st40:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof40;
case 40:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr96;
		case 38u: goto tr97;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr98;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr100;
	} else
		goto tr99;
	goto tr95;
st41:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof41;
case 41:
	switch( (*( _state.p)) ) {
		case 79u: goto st42;
		case 111u: goto st42;
	}
	goto st0;
st42:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof42;
case 42:
	switch( (*( _state.p)) ) {
		case 83u: goto st39;
		case 115u: goto st39;
	}
	goto st0;
st43:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof43;
case 43:
	switch( (*( _state.p)) ) {
		case 84u: goto st39;
		case 116u: goto st39;
	}
	goto st0;
st44:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof44;
case 44:
	switch( (*( _state.p)) ) {
		case 85u: goto st45;
		case 117u: goto st45;
	}
	goto st0;
st45:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof45;
case 45:
	switch( (*( _state.p)) ) {
		case 79u: goto st43;
		case 111u: goto st43;
	}
	goto st0;
st46:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof46;
case 46:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr104;
		case 39u: goto st25;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr105;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr107;
	} else
		goto tr106;
	goto tr103;
tr103:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st47;
tr108:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st47;
tr120:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st47;
tr130:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st47;
tr139:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st47;
st47:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof47;
case 47:
#line 1335 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr109;
		case 39u: goto tr58;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr110;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr112;
	} else
		goto tr111;
	goto tr108;
tr104:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st48;
tr109:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st48;
tr121:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st48;
tr131:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st48;
tr140:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st48;
st48:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof48;
case 48:
#line 1389 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 35u: goto st49;
		case 65u: goto st58;
		case 71u: goto st64;
		case 76u: goto st64;
		case 81u: goto st65;
		case 97u: goto st58;
		case 103u: goto st64;
		case 108u: goto st64;
		case 113u: goto st65;
	}
	goto st0;
st49:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof49;
case 49:
	switch( (*( _state.p)) ) {
		case 88u: goto st55;
		case 120u: goto st55;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st50;
	goto st0;
st50:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof50;
case 50:
	if ( (*( _state.p)) == 59u )
		goto st51;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st50;
	goto st0;
st51:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof51;
case 51:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr121;
		case 39u: goto tr75;
		case 60u: goto st0;
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
	goto tr120;
tr105:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st52;
tr110:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st52;
tr122:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st52;
tr132:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st52;
tr141:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st52;
st52:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof52;
case 52:
#line 1479 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st47;
	goto st0;
tr106:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st53;
tr111:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st53;
tr123:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st53;
tr133:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st53;
tr142:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st53;
st53:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof53;
case 53:
#line 1515 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st52;
	goto st0;
tr107:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st54;
tr112:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st54;
tr124:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st54;
tr134:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st54;
tr143:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st54;
st54:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof54;
case 54:
#line 1551 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st53;
	goto st0;
st55:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof55;
case 55:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st56;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st56;
	} else
		goto st56;
	goto st0;
st56:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof56;
case 56:
	if ( (*( _state.p)) == 59u )
		goto st57;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st56;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st56;
	} else
		goto st56;
	goto st0;
st57:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof57;
case 57:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr131;
		case 39u: goto tr86;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr132;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr134;
	} else
		goto tr133;
	goto tr130;
st58:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof58;
case 58:
	switch( (*( _state.p)) ) {
		case 77u: goto st59;
		case 80u: goto st62;
		case 109u: goto st59;
		case 112u: goto st62;
	}
	goto st0;
st59:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof59;
case 59:
	switch( (*( _state.p)) ) {
		case 80u: goto st60;
		case 112u: goto st60;
	}
	goto st0;
st60:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof60;
case 60:
	if ( (*( _state.p)) == 59u )
		goto st61;
	goto st0;
st61:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof61;
case 61:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr140;
		case 39u: goto tr96;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr141;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr143;
	} else
		goto tr142;
	goto tr139;
st62:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof62;
case 62:
	switch( (*( _state.p)) ) {
		case 79u: goto st63;
		case 111u: goto st63;
	}
	goto st0;
st63:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof63;
case 63:
	switch( (*( _state.p)) ) {
		case 83u: goto st60;
		case 115u: goto st60;
	}
	goto st0;
st64:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof64;
case 64:
	switch( (*( _state.p)) ) {
		case 84u: goto st60;
		case 116u: goto st60;
	}
	goto st0;
st65:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof65;
case 65:
	switch( (*( _state.p)) ) {
		case 85u: goto st66;
		case 117u: goto st66;
	}
	goto st0;
st66:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof66;
case 66:
	switch( (*( _state.p)) ) {
		case 79u: goto st64;
		case 111u: goto st64;
	}
	goto st0;
tr38:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st67;
st67:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof67;
case 67:
#line 1713 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st20;
	goto st0;
tr39:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st68;
st68:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof68;
case 68:
#line 1725 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st67;
	goto st0;
tr40:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st69;
st69:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof69;
case 69:
#line 1737 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st68;
	goto st0;
tr24:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st70;
st70:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof70;
case 70:
#line 1749 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st18;
	goto st0;
tr25:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st71;
st71:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof71;
case 71:
#line 1761 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st70;
	goto st0;
tr26:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st72;
st72:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof72;
case 72:
#line 1773 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st71;
	goto st0;
st73:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof73;
case 73:
	switch( (*( _state.p)) ) {
		case 45u: goto st74;
		case 91u: goto st81;
	}
	goto st0;
st74:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof74;
case 74:
	if ( (*( _state.p)) == 45u )
		goto st75;
	goto st0;
st75:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof75;
case 75:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 45u: goto st76;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st78;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st80;
	} else
		goto st79;
	goto st75;
st76:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof76;
case 76:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 45u: goto st77;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st78;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st80;
	} else
		goto st79;
	goto st75;
st77:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof77;
case 77:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 45u: goto st77;
		case 62u: goto tr154;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st78;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st80;
	} else
		goto st79;
	goto st75;
st78:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof78;
case 78:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st75;
	goto st0;
st79:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof79;
case 79:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st78;
	goto st0;
st80:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof80;
case 80:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st79;
	goto st0;
st81:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof81;
case 81:
	if ( (*( _state.p)) == 67u )
		goto st82;
	goto st0;
st82:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof82;
case 82:
	if ( (*( _state.p)) == 68u )
		goto st83;
	goto st0;
st83:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof83;
case 83:
	if ( (*( _state.p)) == 65u )
		goto st84;
	goto st0;
st84:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof84;
case 84:
	if ( (*( _state.p)) == 84u )
		goto st85;
	goto st0;
st85:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof85;
case 85:
	if ( (*( _state.p)) == 65u )
		goto st86;
	goto st0;
st86:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof86;
case 86:
	if ( (*( _state.p)) == 91u )
		goto st87;
	goto st0;
st87:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof87;
case 87:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 93u: goto st88;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st90;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st92;
	} else
		goto st91;
	goto st87;
st88:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof88;
case 88:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 93u: goto st89;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st90;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st92;
	} else
		goto st91;
	goto st87;
st89:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof89;
case 89:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 62u: goto tr166;
		case 93u: goto st89;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto st90;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto st92;
	} else
		goto st91;
	goto st87;
st90:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof90;
case 90:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st87;
	goto st0;
st91:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof91;
case 91:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st90;
	goto st0;
st92:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof92;
case 92:
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st91;
	goto st0;
st93:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof93;
case 93:
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
				goto tr168;
		} else if ( (*( _state.p)) > 239u ) {
			if ( (*( _state.p)) > 247u ) {
				if ( 248u <= (*( _state.p)) )
					goto st0;
			} else if ( (*( _state.p)) >= 240u )
				goto tr170;
		} else
			goto tr169;
	} else
		goto st0;
	goto tr167;
tr167:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st94;
st94:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof94;
case 94:
#line 2059 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr172;
		case 32u: goto tr172;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto st0;
		case 62u: goto tr173;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto tr172;
		} else if ( (*( _state.p)) > 63u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto st97;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto st98;
	} else
		goto st96;
	goto st94;
tr172:
#line 20 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_end(( _state.p)); }
	goto st95;
st95:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof95;
case 95:
#line 2098 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto st95;
		case 32u: goto st95;
		case 62u: goto tr178;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto st95;
	goto st0;
tr168:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st96;
st96:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof96;
case 96:
#line 2115 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st94;
	goto st0;
tr169:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st97;
st97:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof97;
case 97:
#line 2127 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st96;
	goto st0;
tr170:
#line 19 "../ragel/formats/sgml/sgml.rl"
	{ _tag_name_start(( _state.p)); }
	goto st98;
st98:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof98;
case 98:
#line 2139 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st97;
	goto st0;
st99:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof99;
case 99:
	switch( (*( _state.p)) ) {
		case 88u: goto st100;
		case 120u: goto st100;
	}
	goto st0;
st100:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof100;
case 100:
	switch( (*( _state.p)) ) {
		case 77u: goto st101;
		case 109u: goto st101;
	}
	goto st0;
st101:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof101;
case 101:
	switch( (*( _state.p)) ) {
		case 76u: goto st102;
		case 108u: goto st102;
	}
	goto st0;
st102:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof102;
case 102:
	switch( (*( _state.p)) ) {
		case 13u: goto st103;
		case 32u: goto st103;
		case 63u: goto st110;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto st103;
	goto st0;
tr210:
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st103;
st103:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof103;
case 103:
#line 2190 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto st103;
		case 32u: goto st103;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto st0;
		case 63u: goto st110;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto st103;
		} else if ( (*( _state.p)) > 62u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto tr186;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto tr187;
	} else
		goto tr185;
	goto tr184;
tr184:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st104;
st104:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof104;
case 104:
#line 2229 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 13u: goto tr189;
		case 32u: goto tr189;
		case 33u: goto st0;
		case 38u: goto st0;
		case 47u: goto st0;
		case 61u: goto tr190;
	}
	if ( (*( _state.p)) < 192u ) {
		if ( (*( _state.p)) < 60u ) {
			if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
				goto tr189;
		} else if ( (*( _state.p)) > 63u ) {
			if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
				goto st0;
		} else
			goto st0;
	} else if ( (*( _state.p)) > 223u ) {
		if ( (*( _state.p)) < 240u ) {
			if ( 224u <= (*( _state.p)) && (*( _state.p)) <= 239u )
				goto st152;
		} else if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else
			goto st153;
	} else
		goto st151;
	goto st104;
tr189:
#line 13 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_end(( _state.p)); }
	goto st105;
st105:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof105;
case 105:
#line 2268 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto st105;
		case 32u: goto st105;
		case 61u: goto st106;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto st105;
	goto st0;
tr190:
#line 13 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_end(( _state.p)); }
	goto st106;
st106:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof106;
case 106:
#line 2285 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto st106;
		case 32u: goto st106;
		case 34u: goto st107;
		case 39u: goto st130;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto st106;
	goto st0;
st107:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof107;
case 107:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto st109;
		case 38u: goto tr200;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr201;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr203;
	} else
		goto tr202;
	goto tr198;
tr198:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st108;
tr204:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st108;
tr220:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st108;
tr231:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st108;
tr241:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st108;
st108:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof108;
case 108:
#line 2352 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr205;
		case 38u: goto tr206;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr207;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr209;
	} else
		goto tr208;
	goto tr204;
tr205:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
	goto st109;
tr221:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
	goto st109;
tr232:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
	goto st109;
tr242:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
	goto st109;
st109:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof109;
case 109:
#line 2394 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 13u: goto tr210;
		case 32u: goto tr210;
		case 63u: goto tr211;
	}
	if ( 9u <= (*( _state.p)) && (*( _state.p)) <= 10u )
		goto tr210;
	goto st0;
tr211:
#line 22 "../ragel/formats/sgml/sgml.rl"
	{ _create_attr(); }
	goto st110;
st110:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof110;
case 110:
#line 2411 "schwa/formats/sgml_gen.cc"
	if ( (*( _state.p)) == 62u )
		goto tr212;
	goto st0;
tr200:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st111;
tr206:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st111;
tr222:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st111;
tr233:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st111;
tr243:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st111;
st111:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof111;
case 111:
#line 2447 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 35u: goto st112;
		case 65u: goto st121;
		case 71u: goto st127;
		case 76u: goto st127;
		case 81u: goto st128;
		case 97u: goto st121;
		case 103u: goto st127;
		case 108u: goto st127;
		case 113u: goto st128;
	}
	goto st0;
st112:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof112;
case 112:
	switch( (*( _state.p)) ) {
		case 88u: goto st118;
		case 120u: goto st118;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st113;
	goto st0;
st113:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof113;
case 113:
	if ( (*( _state.p)) == 59u )
		goto st114;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st113;
	goto st0;
st114:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof114;
case 114:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr221;
		case 38u: goto tr222;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr223;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr225;
	} else
		goto tr224;
	goto tr220;
tr201:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st115;
tr207:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st115;
tr223:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st115;
tr234:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st115;
tr244:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st115;
st115:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof115;
case 115:
#line 2537 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st108;
	goto st0;
tr202:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st116;
tr208:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st116;
tr224:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st116;
tr235:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st116;
tr245:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st116;
st116:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof116;
case 116:
#line 2573 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st115;
	goto st0;
tr203:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st117;
tr209:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st117;
tr225:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st117;
tr236:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st117;
tr246:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st117;
st117:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof117;
case 117:
#line 2609 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st116;
	goto st0;
st118:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof118;
case 118:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st119;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st119;
	} else
		goto st119;
	goto st0;
st119:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof119;
case 119:
	if ( (*( _state.p)) == 59u )
		goto st120;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st119;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st119;
	} else
		goto st119;
	goto st0;
st120:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof120;
case 120:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr232;
		case 38u: goto tr233;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr234;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr236;
	} else
		goto tr235;
	goto tr231;
st121:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof121;
case 121:
	switch( (*( _state.p)) ) {
		case 77u: goto st122;
		case 80u: goto st125;
		case 109u: goto st122;
		case 112u: goto st125;
	}
	goto st0;
st122:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof122;
case 122:
	switch( (*( _state.p)) ) {
		case 80u: goto st123;
		case 112u: goto st123;
	}
	goto st0;
st123:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof123;
case 123:
	if ( (*( _state.p)) == 59u )
		goto st124;
	goto st0;
st124:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof124;
case 124:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 34u: goto tr242;
		case 38u: goto tr243;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr244;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr246;
	} else
		goto tr245;
	goto tr241;
st125:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof125;
case 125:
	switch( (*( _state.p)) ) {
		case 79u: goto st126;
		case 111u: goto st126;
	}
	goto st0;
st126:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof126;
case 126:
	switch( (*( _state.p)) ) {
		case 83u: goto st123;
		case 115u: goto st123;
	}
	goto st0;
st127:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof127;
case 127:
	switch( (*( _state.p)) ) {
		case 84u: goto st123;
		case 116u: goto st123;
	}
	goto st0;
st128:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof128;
case 128:
	switch( (*( _state.p)) ) {
		case 85u: goto st129;
		case 117u: goto st129;
	}
	goto st0;
st129:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof129;
case 129:
	switch( (*( _state.p)) ) {
		case 79u: goto st127;
		case 111u: goto st127;
	}
	goto st0;
st130:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof130;
case 130:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr250;
		case 39u: goto st109;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr251;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr253;
	} else
		goto tr252;
	goto tr249;
tr249:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st131;
tr254:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st131;
tr266:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st131;
tr276:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st131;
tr285:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st131;
st131:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof131;
case 131:
#line 2820 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr255;
		case 39u: goto tr205;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr256;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr258;
	} else
		goto tr257;
	goto tr254;
tr250:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st132;
tr255:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st132;
tr267:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st132;
tr277:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st132;
tr286:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st132;
st132:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof132;
case 132:
#line 2874 "schwa/formats/sgml_gen.cc"
	switch( (*( _state.p)) ) {
		case 35u: goto st133;
		case 65u: goto st142;
		case 71u: goto st148;
		case 76u: goto st148;
		case 81u: goto st149;
		case 97u: goto st142;
		case 103u: goto st148;
		case 108u: goto st148;
		case 113u: goto st149;
	}
	goto st0;
st133:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof133;
case 133:
	switch( (*( _state.p)) ) {
		case 88u: goto st139;
		case 120u: goto st139;
	}
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st134;
	goto st0;
st134:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof134;
case 134:
	if ( (*( _state.p)) == 59u )
		goto st135;
	if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
		goto st134;
	goto st0;
st135:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof135;
case 135:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr267;
		case 39u: goto tr221;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr268;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr270;
	} else
		goto tr269;
	goto tr266;
tr251:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st136;
tr256:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st136;
tr268:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st136;
tr278:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st136;
tr287:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st136;
st136:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof136;
case 136:
#line 2964 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st131;
	goto st0;
tr252:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st137;
tr257:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st137;
tr269:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st137;
tr279:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st137;
tr288:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st137;
st137:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof137;
case 137:
#line 3000 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st136;
	goto st0;
tr253:
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st138;
tr258:
#line 15 "../ragel/formats/sgml/sgml.rl"
	{ _character_end(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st138;
tr270:
#line 16 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_decimal(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st138;
tr280:
#line 17 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_hex(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st138;
tr289:
#line 18 "../ragel/formats/sgml/sgml.rl"
	{ _character_reference_named(( _state.p)); }
#line 14 "../ragel/formats/sgml/sgml.rl"
	{ _character_start(( _state.p)); }
	goto st138;
st138:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof138;
case 138:
#line 3036 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st137;
	goto st0;
st139:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof139;
case 139:
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st140;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st140;
	} else
		goto st140;
	goto st0;
st140:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof140;
case 140:
	if ( (*( _state.p)) == 59u )
		goto st141;
	if ( (*( _state.p)) < 65u ) {
		if ( 48u <= (*( _state.p)) && (*( _state.p)) <= 57u )
			goto st140;
	} else if ( (*( _state.p)) > 70u ) {
		if ( 97u <= (*( _state.p)) && (*( _state.p)) <= 102u )
			goto st140;
	} else
		goto st140;
	goto st0;
st141:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof141;
case 141:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr277;
		case 39u: goto tr232;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr278;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr280;
	} else
		goto tr279;
	goto tr276;
st142:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof142;
case 142:
	switch( (*( _state.p)) ) {
		case 77u: goto st143;
		case 80u: goto st146;
		case 109u: goto st143;
		case 112u: goto st146;
	}
	goto st0;
st143:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof143;
case 143:
	switch( (*( _state.p)) ) {
		case 80u: goto st144;
		case 112u: goto st144;
	}
	goto st0;
st144:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof144;
case 144:
	if ( (*( _state.p)) == 59u )
		goto st145;
	goto st0;
st145:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof145;
case 145:
	switch( (*( _state.p)) ) {
		case 0u: goto st0;
		case 38u: goto tr286;
		case 39u: goto tr242;
		case 60u: goto st0;
	}
	if ( (*( _state.p)) < 224u ) {
		if ( (*( _state.p)) > 191u ) {
			if ( 192u <= (*( _state.p)) && (*( _state.p)) <= 223u )
				goto tr287;
		} else if ( (*( _state.p)) >= 128u )
			goto st0;
	} else if ( (*( _state.p)) > 239u ) {
		if ( (*( _state.p)) > 247u ) {
			if ( 248u <= (*( _state.p)) )
				goto st0;
		} else if ( (*( _state.p)) >= 240u )
			goto tr289;
	} else
		goto tr288;
	goto tr285;
st146:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof146;
case 146:
	switch( (*( _state.p)) ) {
		case 79u: goto st147;
		case 111u: goto st147;
	}
	goto st0;
st147:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof147;
case 147:
	switch( (*( _state.p)) ) {
		case 83u: goto st144;
		case 115u: goto st144;
	}
	goto st0;
st148:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof148;
case 148:
	switch( (*( _state.p)) ) {
		case 84u: goto st144;
		case 116u: goto st144;
	}
	goto st0;
st149:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof149;
case 149:
	switch( (*( _state.p)) ) {
		case 85u: goto st150;
		case 117u: goto st150;
	}
	goto st0;
st150:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof150;
case 150:
	switch( (*( _state.p)) ) {
		case 79u: goto st148;
		case 111u: goto st148;
	}
	goto st0;
tr185:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st151;
st151:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof151;
case 151:
#line 3198 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st104;
	goto st0;
tr186:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st152;
st152:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof152;
case 152:
#line 3210 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st151;
	goto st0;
tr187:
#line 12 "../ragel/formats/sgml/sgml.rl"
	{ _attr_name_start(( _state.p)); }
	goto st153;
st153:
	if ( ++( _state.p) == ( _state.pe) )
		goto _test_eof153;
case 153:
#line 3222 "schwa/formats/sgml_gen.cc"
	if ( 128u <= (*( _state.p)) && (*( _state.p)) <= 191u )
		goto st152;
	goto st0;
	}
	_test_eof154:  _state.cs = 154; goto _test_eof; 
	_test_eof155:  _state.cs = 155; goto _test_eof; 
	_test_eof1:  _state.cs = 1; goto _test_eof; 
	_test_eof2:  _state.cs = 2; goto _test_eof; 
	_test_eof3:  _state.cs = 3; goto _test_eof; 
	_test_eof156:  _state.cs = 156; goto _test_eof; 
	_test_eof4:  _state.cs = 4; goto _test_eof; 
	_test_eof5:  _state.cs = 5; goto _test_eof; 
	_test_eof6:  _state.cs = 6; goto _test_eof; 
	_test_eof7:  _state.cs = 7; goto _test_eof; 
	_test_eof8:  _state.cs = 8; goto _test_eof; 
	_test_eof157:  _state.cs = 157; goto _test_eof; 
	_test_eof9:  _state.cs = 9; goto _test_eof; 
	_test_eof10:  _state.cs = 10; goto _test_eof; 
	_test_eof11:  _state.cs = 11; goto _test_eof; 
	_test_eof158:  _state.cs = 158; goto _test_eof; 
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

	_test_eof: {}
	if ( ( _state.p) == ( _state.eof) )
	{
	switch (  _state.cs ) {
	case 155: goto tr298;
	case 1: goto tr0;
	case 2: goto tr0;
	case 3: goto tr0;
	case 156: goto tr304;
	case 4: goto tr0;
	case 5: goto tr0;
	case 6: goto tr0;
	case 7: goto tr0;
	case 8: goto tr0;
	case 157: goto tr310;
	case 9: goto tr0;
	case 10: goto tr0;
	case 11: goto tr0;
	case 158: goto tr316;
	case 12: goto tr0;
	case 13: goto tr0;
	case 14: goto tr0;
	case 15: goto tr0;
	case 16: goto tr0;
	}
	}

	_out: {}
	}

#line 96 "../ragel/formats/sgml/sgml.rl"

  // Fail if we ended up in an error state or we didn't consume any input.
  if (_state.cs == 0)
    return nullptr;
  else if (_state.p == p_before)
    return nullptr;
  else
    return _node;
}

}  // namespace formats
}  // namespace schwa
