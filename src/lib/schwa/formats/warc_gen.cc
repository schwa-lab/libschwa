
#line 1 "../ragel/formats/warc/warc.rl"
// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 77 "../ragel/formats/warc/warc.rl"



#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <schwa/formats/warc.h>


namespace schwa {
namespace formats {


#line 25 "schwa/formats/warc_gen.cc"
static const int warc_start = 77;

static const int warc_en_main = 77;


#line 93 "../ragel/formats/warc/warc.rl"

bool
WARCParser::_run(std::istream &input, const size_t buffer_size) {
  (void)warc_en_main;  // Shoosh compiler warning about unused variable.
  const uint8_t *te = nullptr, *ts = nullptr;
  int cs = 0;

  
#line 40 "schwa/formats/warc_gen.cc"
	{
	cs = warc_start;
	}

#line 101 "../ragel/formats/warc/warc.rl"

  std::unique_ptr<uint8_t[]> scoped_buffer(new uint8_t[buffer_size]);
  uint8_t *buffer = scoped_buffer.get();

  bool done = false;
  size_t have = 0;
  while (!done) {
    const size_t space = buffer_size - have;
    if (space == 0) {
      std::ostringstream msg;
      msg << "current lexeme is larger than buffer size of " << buffer_size;
      throw std::runtime_error(msg.str());
    }

    uint8_t *p = buffer + have;
    input.read(reinterpret_cast<char *>(p), space);
    size_t nread = input.gcount();

    uint8_t *pe = p + nread;
    uint8_t *eof = nullptr;
    if (nread == 0) {
      eof = pe;
      done = true;
    }

    
#line 72 "schwa/formats/warc_gen.cc"
	{
	short _widec;
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 77:
	if ( (*p) == 87u )
		goto tr134;
	goto st0;
st0:
cs = 0;
	goto _out;
tr134:
#line 20 "../ragel/formats/warc/warc.rl"
	{ _record_start(); }
	goto st1;
tr135:
#line 11 "../ragel/formats/warc/warc.rl"
	{ _block_end(); }
#line 21 "../ragel/formats/warc/warc.rl"
	{ _record_end(); }
#line 20 "../ragel/formats/warc/warc.rl"
	{ _record_start(); }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 102 "schwa/formats/warc_gen.cc"
	if ( (*p) == 65u )
		goto st2;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 82u )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 67u )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 47u )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 48u: goto st6;
		case 49u: goto st57;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 46u )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 49u )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 56u )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 10u: goto st10;
		case 13u: goto st33;
	}
	goto st0;
tr54:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st10;
tr56:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 180 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto st12;
		case 13u: goto st20;
		case 33u: goto tr15;
		case 124u: goto tr15;
		case 126u: goto tr15;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr15;
		} else if ( (*p) >= 35u )
			goto tr15;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr15;
		} else if ( (*p) >= 48u )
			goto tr15;
	} else
		goto tr15;
	goto st11;
tr55:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st11;
tr57:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 217 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st10;
	goto st11;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr16;
		case 269: goto tr17;
		case 522: goto tr19;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr18;
	goto st0;
tr16:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 246 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto st78;
		case 13u: goto st14;
	}
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 87u )
		goto tr135;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 10u )
		goto st78;
	goto st0;
tr17:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 274 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st13;
	goto st0;
tr18:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st16;
tr24:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 292 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st13;
		case 269: goto st15;
		case 522: goto tr25;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
tr19:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st17;
tr25:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 320 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st79;
		case 269: goto st18;
		case 522: goto tr28;
		case 525: goto tr29;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
tr37:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
#line 343 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto st78;
		case 13u: goto st14;
		case 87u: goto tr135;
	}
	goto st0;
tr38:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 358 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st79;
	goto st0;
tr39:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st80;
tr28:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 376 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st79;
		case 269: goto st18;
		case 343: goto tr135;
		case 522: goto tr28;
		case 525: goto tr29;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
tr40:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st19;
tr29:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 406 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st79;
		case 269: goto st15;
		case 522: goto tr28;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 10u )
		goto st21;
	goto st11;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr31;
		case 269: goto tr32;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr34;
		case 525: goto tr35;
		case 545: goto tr36;
		case 556: goto tr33;
		case 559: goto tr33;
		case 635: goto tr33;
		case 637: goto tr33;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr33;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr33;
			} else
				goto tr36;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr33;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr33;
			} else
				goto tr36;
		} else
			goto tr36;
	} else
		goto st11;
	goto st0;
tr31:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st22;
tr34:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 508 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr37;
		case 269: goto tr38;
		case 522: goto tr39;
		case 525: goto tr40;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr18;
	goto st0;
tr32:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 531 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st24;
	goto st11;
tr71:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 543 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr42;
		case 269: goto tr43;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr44;
		case 525: goto tr45;
		case 545: goto tr36;
		case 556: goto tr33;
		case 559: goto tr33;
		case 635: goto tr33;
		case 637: goto tr33;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr33;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr33;
			} else
				goto tr36;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr33;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr33;
			} else
				goto tr36;
		} else
			goto tr36;
	} else
		goto st11;
	goto st0;
tr42:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st81;
tr44:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st81;
tr76:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 625 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr37;
		case 269: goto tr38;
		case 343: goto tr135;
		case 522: goto tr39;
		case 525: goto tr40;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr18;
	goto st0;
tr43:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 649 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st82;
	goto st11;
tr79:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 661 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr42;
		case 269: goto tr43;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr44;
		case 525: goto tr45;
		case 545: goto tr36;
		case 556: goto tr33;
		case 559: goto tr33;
		case 635: goto tr33;
		case 637: goto tr33;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr33;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr33;
			} else
				goto tr36;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr33;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr33;
			} else
				goto tr36;
		} else
			goto tr36;
	} else
		goto st11;
	goto st0;
tr15:
#line 13 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_start(); }
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
	goto st26;
tr47:
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 739 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto st10;
		case 33u: goto tr47;
		case 58u: goto tr48;
		case 124u: goto tr47;
		case 126u: goto tr47;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr47;
		} else if ( (*p) >= 35u )
			goto tr47;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr47;
		} else if ( (*p) >= 48u )
			goto tr47;
	} else
		goto tr47;
	goto st11;
tr48:
#line 15 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_end(); }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 770 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 9u: goto st29;
		case 10u: goto st30;
		case 13u: goto st34;
		case 32u: goto st29;
	}
	goto tr49;
tr49:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st28;
tr53:
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 792 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto tr54;
		case 13u: goto tr55;
	}
	goto tr53;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 9u: goto st29;
		case 10u: goto tr56;
		case 13u: goto tr57;
		case 32u: goto st29;
	}
	goto tr49;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 9u: goto st31;
		case 32u: goto st31;
	}
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 9u: goto st31;
		case 10u: goto tr56;
		case 13u: goto tr60;
		case 32u: goto st31;
	}
	goto tr59;
tr59:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st32;
tr61:
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 843 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto tr54;
		case 13u: goto tr62;
	}
	goto tr61;
tr62:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st33;
tr60:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 863 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st10;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 10u )
		goto st35;
	goto st11;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 9u: goto st29;
		case 10u: goto st12;
		case 13u: goto st20;
		case 32u: goto st29;
		case 33u: goto tr15;
		case 124u: goto tr15;
		case 126u: goto tr15;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr15;
		} else if ( (*p) >= 35u )
			goto tr15;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr15;
		} else if ( (*p) >= 48u )
			goto tr15;
	} else
		goto tr15;
	goto st11;
tr33:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st36;
tr66:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st36;
tr91:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st36;
tr95:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 930 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st37;
		case 269: goto st40;
		case 522: goto tr67;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr66;
	} else if ( _widec >= 256 )
		goto st11;
	goto st0;
tr87:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st37;
tr92:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 961 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto st83;
		case 13u: goto st38;
		case 33u: goto tr15;
		case 124u: goto tr15;
		case 126u: goto tr15;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr15;
		} else if ( (*p) >= 35u )
			goto tr15;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr15;
		} else if ( (*p) >= 48u )
			goto tr15;
	} else
		goto tr15;
	goto st11;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr16;
		case 269: goto tr17;
		case 343: goto tr135;
		case 522: goto tr19;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr18;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 10u )
		goto st84;
	goto st11;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr31;
		case 269: goto tr32;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr34;
		case 525: goto tr35;
		case 545: goto tr36;
		case 556: goto tr33;
		case 559: goto tr33;
		case 635: goto tr33;
		case 637: goto tr33;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr33;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr33;
			} else
				goto tr36;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr33;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr33;
			} else
				goto tr36;
		} else
			goto tr36;
	} else
		goto st11;
	goto st0;
tr35:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1086 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st24;
		case 269: goto st40;
		case 522: goto tr71;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr66;
	} else if ( _widec >= 256 )
		goto st11;
	goto st0;
tr88:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st40;
tr93:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 1117 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st37;
	goto st11;
tr36:
#line 13 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_start(); }
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st41;
tr72:
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st41;
tr78:
#line 13 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_start(); }
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 1149 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st37;
		case 269: goto st40;
		case 289: goto tr47;
		case 300: goto st11;
		case 303: goto st11;
		case 314: goto tr48;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr67;
		case 545: goto tr72;
		case 556: goto tr66;
		case 559: goto tr66;
		case 570: goto tr73;
		case 635: goto tr66;
		case 637: goto tr66;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr47;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 315 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr47;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr47;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr66;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr66;
			} else
				goto tr72;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 571 <= _widec && _widec <= 576 )
					goto tr66;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr66;
			} else
				goto tr72;
		} else
			goto tr72;
	} else
		goto st11;
	goto st0;
tr67:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st42;
tr90:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st42;
tr94:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
#line 1236 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st81;
		case 269: goto st25;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr76;
		case 525: goto tr77;
		case 545: goto tr78;
		case 556: goto tr66;
		case 559: goto tr66;
		case 635: goto tr66;
		case 637: goto tr66;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr66;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr66;
			} else
				goto tr78;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr66;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr66;
			} else
				goto tr78;
		} else
			goto tr78;
	} else
		goto st11;
	goto st0;
tr45:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st43;
tr77:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 1314 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st82;
		case 269: goto st40;
		case 522: goto tr79;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr66;
	} else if ( _widec >= 256 )
		goto st11;
	goto st0;
tr73:
#line 15 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 1341 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 265: goto st29;
		case 266: goto st45;
		case 269: goto st46;
		case 288: goto st29;
		case 521: goto tr83;
		case 522: goto tr84;
		case 525: goto tr85;
		case 544: goto tr83;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr82;
	} else if ( _widec >= 256 )
		goto tr49;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 9u: goto st31;
		case 10u: goto st78;
		case 13u: goto st14;
		case 32u: goto st31;
	}
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 10u )
		goto st47;
	goto st11;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 9u: goto st29;
		case 10u: goto st83;
		case 13u: goto st38;
		case 32u: goto st29;
		case 33u: goto tr15;
		case 124u: goto tr15;
		case 126u: goto tr15;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr15;
		} else if ( (*p) >= 35u )
			goto tr15;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr15;
		} else if ( (*p) >= 48u )
			goto tr15;
	} else
		goto tr15;
	goto st11;
tr82:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st48;
tr89:
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 1427 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr87;
		case 269: goto tr88;
		case 522: goto tr90;
		case 525: goto tr91;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr89;
	} else if ( _widec >= 256 )
		goto tr53;
	goto st0;
tr83:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 1453 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 265: goto st29;
		case 266: goto tr92;
		case 269: goto tr93;
		case 288: goto st29;
		case 521: goto tr83;
		case 522: goto tr94;
		case 525: goto tr95;
		case 544: goto tr83;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr82;
	} else if ( _widec >= 256 )
		goto tr49;
	goto st0;
tr84:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 1483 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 265: goto st31;
		case 266: goto st79;
		case 269: goto st18;
		case 288: goto st31;
		case 521: goto tr96;
		case 522: goto tr28;
		case 525: goto tr29;
		case 544: goto tr96;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
tr96:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 1510 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 265: goto st31;
		case 266: goto tr92;
		case 269: goto tr97;
		case 288: goto st31;
		case 521: goto tr96;
		case 522: goto tr94;
		case 525: goto tr99;
		case 544: goto tr96;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr98;
	} else if ( _widec >= 256 )
		goto tr59;
	goto st0;
tr100:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st52;
tr97:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 1546 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st37;
	goto st0;
tr98:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st53;
tr101:
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 1568 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr87;
		case 269: goto tr100;
		case 522: goto tr90;
		case 525: goto tr102;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr101;
	} else if ( _widec >= 256 )
		goto tr61;
	goto st0;
tr102:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st54;
tr99:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 1604 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st37;
		case 269: goto st15;
		case 522: goto tr67;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
tr85:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 1626 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st47;
		case 269: goto st40;
		case 522: goto tr103;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr66;
	} else if ( _widec >= 256 )
		goto st11;
	goto st0;
tr103:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 1651 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 265: goto st29;
		case 266: goto st81;
		case 269: goto st25;
		case 288: goto st29;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 521: goto tr83;
		case 522: goto tr76;
		case 525: goto tr77;
		case 544: goto tr83;
		case 545: goto tr78;
		case 556: goto tr66;
		case 559: goto tr66;
		case 635: goto tr66;
		case 637: goto tr66;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr66;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr66;
			} else
				goto tr78;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr66;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr66;
			} else
				goto tr78;
		} else
			goto tr78;
	} else
		goto st11;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 46u )
		goto st58;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 48u )
		goto st59;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 13u )
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 10u )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 13u: goto st62;
		case 33u: goto tr109;
		case 124u: goto tr109;
		case 126u: goto tr109;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr109;
		} else if ( (*p) >= 35u )
			goto tr109;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr109;
		} else if ( (*p) >= 48u )
			goto tr109;
	} else
		goto tr109;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 10u )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 269: goto tr111;
		case 525: goto tr113;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr112;
	goto st0;
tr111:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 1803 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st65;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 13u )
		goto st66;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 10u )
		goto st78;
	goto st0;
tr112:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st67;
tr117:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st67;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
#line 1835 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 269: goto st64;
		case 525: goto tr118;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr117;
	goto st0;
tr113:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st68;
tr118:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st68;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
#line 1862 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st65;
		case 269: goto st64;
		case 522: goto tr119;
		case 525: goto tr118;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr117;
	goto st0;
tr119:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
#line 1885 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 269: goto st70;
		case 525: goto tr121;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr117;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 10u )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 13u: goto st66;
		case 87u: goto tr135;
	}
	goto st0;
tr121:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 1922 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st85;
		case 269: goto st64;
		case 522: goto tr123;
		case 525: goto tr118;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr117;
	goto st0;
tr123:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 1945 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 269: goto st70;
		case 343: goto tr135;
		case 525: goto tr121;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr117;
	goto st0;
tr109:
#line 13 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_start(); }
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
	goto st72;
tr124:
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 1973 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 33u: goto tr124;
		case 58u: goto tr125;
		case 124u: goto tr124;
		case 126u: goto tr124;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr124;
		} else if ( (*p) >= 35u )
			goto tr124;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr124;
		} else if ( (*p) >= 48u )
			goto tr124;
	} else
		goto tr124;
	goto st0;
tr125:
#line 15 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_end(); }
	goto st73;
tr128:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st73;
tr126:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
#line 2013 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 9u: goto tr126;
		case 13u: goto tr127;
		case 32u: goto tr128;
		case 127u: goto st0;
	}
	if ( (*p) <= 31u )
		goto st0;
	goto tr129;
tr132:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st74;
tr127:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 2037 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st75;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 9u: goto st76;
		case 13u: goto st62;
		case 32u: goto st76;
		case 33u: goto tr109;
		case 124u: goto tr109;
		case 126u: goto tr109;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr109;
		} else if ( (*p) >= 35u )
			goto tr109;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr109;
		} else if ( (*p) >= 48u )
			goto tr109;
	} else
		goto tr109;
	goto st0;
tr129:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st76;
tr133:
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
#line 2082 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 9u: goto st76;
		case 13u: goto tr132;
		case 127u: goto st0;
	}
	if ( (*p) <= 31u )
		goto st0;
	goto tr133;
	}
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
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
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
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
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
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 13: 
	case 14: 
	case 17: 
	case 18: 
	case 19: 
	case 22: 
	case 24: 
	case 25: 
	case 37: 
	case 38: 
	case 42: 
	case 43: 
	case 45: 
	case 47: 
	case 50: 
	case 56: 
	case 78: 
	case 79: 
	case 80: 
	case 81: 
	case 82: 
	case 83: 
	case 84: 
	case 85: 
	case 86: 
#line 11 "../ragel/formats/warc/warc.rl"
	{ _block_end(); }
#line 21 "../ragel/formats/warc/warc.rl"
	{ _record_end(); }
	break;
#line 2212 "schwa/formats/warc_gen.cc"
	}
	}

	_out: {}
	}

#line 127 "../ragel/formats/warc/warc.rl"

    if (cs == 
#line 2222 "schwa/formats/warc_gen.cc"
0
#line 128 "../ragel/formats/warc/warc.rl"
)
      return false;

    if (ts == nullptr)
      have = 0;
    else {
      have = pe - ts;
      std::memcpy(buffer, ts, have);
      te = buffer + (te - ts);
      ts = buffer;
    }
  }

  return true;
}


bool
WARCParser::_run(const uint8_t *const input, const size_t nbytes) {
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  int cs = 0;

  
#line 2248 "schwa/formats/warc_gen.cc"
	{
	cs = warc_start;
	}

#line 151 "../ragel/formats/warc/warc.rl"
  
#line 2255 "schwa/formats/warc_gen.cc"
	{
	short _widec;
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 77:
	if ( (*p) == 87u )
		goto tr134;
	goto st0;
st0:
cs = 0;
	goto _out;
tr134:
#line 20 "../ragel/formats/warc/warc.rl"
	{ _record_start(); }
	goto st1;
tr135:
#line 11 "../ragel/formats/warc/warc.rl"
	{ _block_end(); }
#line 21 "../ragel/formats/warc/warc.rl"
	{ _record_end(); }
#line 20 "../ragel/formats/warc/warc.rl"
	{ _record_start(); }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 2285 "schwa/formats/warc_gen.cc"
	if ( (*p) == 65u )
		goto st2;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 82u )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 67u )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 47u )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 48u: goto st6;
		case 49u: goto st57;
	}
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 46u )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 49u )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 56u )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 10u: goto st10;
		case 13u: goto st33;
	}
	goto st0;
tr54:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st10;
tr56:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 2363 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto st12;
		case 13u: goto st20;
		case 33u: goto tr15;
		case 124u: goto tr15;
		case 126u: goto tr15;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr15;
		} else if ( (*p) >= 35u )
			goto tr15;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr15;
		} else if ( (*p) >= 48u )
			goto tr15;
	} else
		goto tr15;
	goto st11;
tr55:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st11;
tr57:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 2400 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st10;
	goto st11;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr16;
		case 269: goto tr17;
		case 522: goto tr19;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr18;
	goto st0;
tr16:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 2429 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto st78;
		case 13u: goto st14;
	}
	goto st0;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 87u )
		goto tr135;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 10u )
		goto st78;
	goto st0;
tr17:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 2457 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st13;
	goto st0;
tr18:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st16;
tr24:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 2475 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st13;
		case 269: goto st15;
		case 522: goto tr25;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
tr19:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st17;
tr25:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 2503 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st79;
		case 269: goto st18;
		case 522: goto tr28;
		case 525: goto tr29;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
tr37:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st79;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
#line 2526 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto st78;
		case 13u: goto st14;
		case 87u: goto tr135;
	}
	goto st0;
tr38:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 2541 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st79;
	goto st0;
tr39:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st80;
tr28:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 2559 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st79;
		case 269: goto st18;
		case 343: goto tr135;
		case 522: goto tr28;
		case 525: goto tr29;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
tr40:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st19;
tr29:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 2589 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st79;
		case 269: goto st15;
		case 522: goto tr28;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( (*p) == 10u )
		goto st21;
	goto st11;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr31;
		case 269: goto tr32;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr34;
		case 525: goto tr35;
		case 545: goto tr36;
		case 556: goto tr33;
		case 559: goto tr33;
		case 635: goto tr33;
		case 637: goto tr33;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr33;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr33;
			} else
				goto tr36;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr33;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr33;
			} else
				goto tr36;
		} else
			goto tr36;
	} else
		goto st11;
	goto st0;
tr31:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st22;
tr34:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 2691 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr37;
		case 269: goto tr38;
		case 522: goto tr39;
		case 525: goto tr40;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr18;
	goto st0;
tr32:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 2714 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st24;
	goto st11;
tr71:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 2726 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr42;
		case 269: goto tr43;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr44;
		case 525: goto tr45;
		case 545: goto tr36;
		case 556: goto tr33;
		case 559: goto tr33;
		case 635: goto tr33;
		case 637: goto tr33;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr33;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr33;
			} else
				goto tr36;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr33;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr33;
			} else
				goto tr36;
		} else
			goto tr36;
	} else
		goto st11;
	goto st0;
tr42:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st81;
tr44:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st81;
tr76:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 2808 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr37;
		case 269: goto tr38;
		case 343: goto tr135;
		case 522: goto tr39;
		case 525: goto tr40;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr18;
	goto st0;
tr43:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 2832 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st82;
	goto st11;
tr79:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 2844 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr42;
		case 269: goto tr43;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr44;
		case 525: goto tr45;
		case 545: goto tr36;
		case 556: goto tr33;
		case 559: goto tr33;
		case 635: goto tr33;
		case 637: goto tr33;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr33;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr33;
			} else
				goto tr36;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr33;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr33;
			} else
				goto tr36;
		} else
			goto tr36;
	} else
		goto st11;
	goto st0;
tr15:
#line 13 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_start(); }
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
	goto st26;
tr47:
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 2922 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto st10;
		case 33u: goto tr47;
		case 58u: goto tr48;
		case 124u: goto tr47;
		case 126u: goto tr47;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr47;
		} else if ( (*p) >= 35u )
			goto tr47;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr47;
		} else if ( (*p) >= 48u )
			goto tr47;
	} else
		goto tr47;
	goto st11;
tr48:
#line 15 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_end(); }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 2953 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 9u: goto st29;
		case 10u: goto st30;
		case 13u: goto st34;
		case 32u: goto st29;
	}
	goto tr49;
tr49:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st28;
tr53:
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 2975 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto tr54;
		case 13u: goto tr55;
	}
	goto tr53;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 9u: goto st29;
		case 10u: goto tr56;
		case 13u: goto tr57;
		case 32u: goto st29;
	}
	goto tr49;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 9u: goto st31;
		case 32u: goto st31;
	}
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 9u: goto st31;
		case 10u: goto tr56;
		case 13u: goto tr60;
		case 32u: goto st31;
	}
	goto tr59;
tr59:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st32;
tr61:
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 3026 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto tr54;
		case 13u: goto tr62;
	}
	goto tr61;
tr62:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st33;
tr60:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st33;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 3046 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st10;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 10u )
		goto st35;
	goto st11;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 9u: goto st29;
		case 10u: goto st12;
		case 13u: goto st20;
		case 32u: goto st29;
		case 33u: goto tr15;
		case 124u: goto tr15;
		case 126u: goto tr15;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr15;
		} else if ( (*p) >= 35u )
			goto tr15;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr15;
		} else if ( (*p) >= 48u )
			goto tr15;
	} else
		goto tr15;
	goto st11;
tr33:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st36;
tr66:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st36;
tr91:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st36;
tr95:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 3113 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st37;
		case 269: goto st40;
		case 522: goto tr67;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr66;
	} else if ( _widec >= 256 )
		goto st11;
	goto st0;
tr87:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st37;
tr92:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 3144 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 10u: goto st83;
		case 13u: goto st38;
		case 33u: goto tr15;
		case 124u: goto tr15;
		case 126u: goto tr15;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr15;
		} else if ( (*p) >= 35u )
			goto tr15;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr15;
		} else if ( (*p) >= 48u )
			goto tr15;
	} else
		goto tr15;
	goto st11;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr16;
		case 269: goto tr17;
		case 343: goto tr135;
		case 522: goto tr19;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr18;
	goto st0;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 10u )
		goto st84;
	goto st11;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr31;
		case 269: goto tr32;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr34;
		case 525: goto tr35;
		case 545: goto tr36;
		case 556: goto tr33;
		case 559: goto tr33;
		case 635: goto tr33;
		case 637: goto tr33;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr33;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr33;
			} else
				goto tr36;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr33;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr33;
			} else
				goto tr36;
		} else
			goto tr36;
	} else
		goto st11;
	goto st0;
tr35:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 3269 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st24;
		case 269: goto st40;
		case 522: goto tr71;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr66;
	} else if ( _widec >= 256 )
		goto st11;
	goto st0;
tr88:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st40;
tr93:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 3300 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st37;
	goto st11;
tr36:
#line 13 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_start(); }
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st41;
tr72:
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st41;
tr78:
#line 13 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_start(); }
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 3332 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st37;
		case 269: goto st40;
		case 289: goto tr47;
		case 300: goto st11;
		case 303: goto st11;
		case 314: goto tr48;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr67;
		case 545: goto tr72;
		case 556: goto tr66;
		case 559: goto tr66;
		case 570: goto tr73;
		case 635: goto tr66;
		case 637: goto tr66;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr47;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 315 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr47;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr47;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr66;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr66;
			} else
				goto tr72;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 571 <= _widec && _widec <= 576 )
					goto tr66;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr66;
			} else
				goto tr72;
		} else
			goto tr72;
	} else
		goto st11;
	goto st0;
tr67:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st42;
tr90:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st42;
tr94:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
#line 3419 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st81;
		case 269: goto st25;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 522: goto tr76;
		case 525: goto tr77;
		case 545: goto tr78;
		case 556: goto tr66;
		case 559: goto tr66;
		case 635: goto tr66;
		case 637: goto tr66;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr66;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr66;
			} else
				goto tr78;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr66;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr66;
			} else
				goto tr78;
		} else
			goto tr78;
	} else
		goto st11;
	goto st0;
tr45:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st43;
tr77:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 3497 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st82;
		case 269: goto st40;
		case 522: goto tr79;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr66;
	} else if ( _widec >= 256 )
		goto st11;
	goto st0;
tr73:
#line 15 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 3524 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 265: goto st29;
		case 266: goto st45;
		case 269: goto st46;
		case 288: goto st29;
		case 521: goto tr83;
		case 522: goto tr84;
		case 525: goto tr85;
		case 544: goto tr83;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr82;
	} else if ( _widec >= 256 )
		goto tr49;
	goto st0;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 9u: goto st31;
		case 10u: goto st78;
		case 13u: goto st14;
		case 32u: goto st31;
	}
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 10u )
		goto st47;
	goto st11;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 9u: goto st29;
		case 10u: goto st83;
		case 13u: goto st38;
		case 32u: goto st29;
		case 33u: goto tr15;
		case 124u: goto tr15;
		case 126u: goto tr15;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr15;
		} else if ( (*p) >= 35u )
			goto tr15;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr15;
		} else if ( (*p) >= 48u )
			goto tr15;
	} else
		goto tr15;
	goto st11;
tr82:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st48;
tr89:
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 3610 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr87;
		case 269: goto tr88;
		case 522: goto tr90;
		case 525: goto tr91;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr89;
	} else if ( _widec >= 256 )
		goto tr53;
	goto st0;
tr83:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 3636 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 265: goto st29;
		case 266: goto tr92;
		case 269: goto tr93;
		case 288: goto st29;
		case 521: goto tr83;
		case 522: goto tr94;
		case 525: goto tr95;
		case 544: goto tr83;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr82;
	} else if ( _widec >= 256 )
		goto tr49;
	goto st0;
tr84:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 3666 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 265: goto st31;
		case 266: goto st79;
		case 269: goto st18;
		case 288: goto st31;
		case 521: goto tr96;
		case 522: goto tr28;
		case 525: goto tr29;
		case 544: goto tr96;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
tr96:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 3693 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 265: goto st31;
		case 266: goto tr92;
		case 269: goto tr97;
		case 288: goto st31;
		case 521: goto tr96;
		case 522: goto tr94;
		case 525: goto tr99;
		case 544: goto tr96;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr98;
	} else if ( _widec >= 256 )
		goto tr59;
	goto st0;
tr100:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st52;
tr97:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 3729 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st37;
	goto st0;
tr98:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st53;
tr101:
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 3751 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto tr87;
		case 269: goto tr100;
		case 522: goto tr90;
		case 525: goto tr102;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr101;
	} else if ( _widec >= 256 )
		goto tr61;
	goto st0;
tr102:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st54;
tr99:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st54;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 3787 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st37;
		case 269: goto st15;
		case 522: goto tr67;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr24;
	goto st0;
tr85:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 3809 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st47;
		case 269: goto st40;
		case 522: goto tr103;
	}
	if ( _widec > 511 ) {
		if ( 512 <= _widec && _widec <= 767 )
			goto tr66;
	} else if ( _widec >= 256 )
		goto st11;
	goto st0;
tr103:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 3834 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 265: goto st29;
		case 266: goto st81;
		case 269: goto st25;
		case 288: goto st29;
		case 289: goto tr15;
		case 300: goto st11;
		case 303: goto st11;
		case 379: goto st11;
		case 381: goto st11;
		case 521: goto tr83;
		case 522: goto tr76;
		case 525: goto tr77;
		case 544: goto tr83;
		case 545: goto tr78;
		case 556: goto tr66;
		case 559: goto tr66;
		case 635: goto tr66;
		case 637: goto tr66;
	}
	if ( _widec < 383 ) {
		if ( _widec < 296 ) {
			if ( _widec > 290 ) {
				if ( 291 <= _widec && _widec <= 295 )
					goto tr15;
			} else if ( _widec >= 256 )
				goto st11;
		} else if ( _widec > 297 ) {
			if ( _widec < 314 ) {
				if ( 298 <= _widec && _widec <= 313 )
					goto tr15;
			} else if ( _widec > 320 ) {
				if ( 321 <= _widec && _widec <= 382 )
					goto tr15;
			} else
				goto st11;
		} else
			goto st11;
	} else if ( _widec > 511 ) {
		if ( _widec < 554 ) {
			if ( _widec < 547 ) {
				if ( 512 <= _widec && _widec <= 546 )
					goto tr66;
			} else if ( _widec > 551 ) {
				if ( 552 <= _widec && _widec <= 553 )
					goto tr66;
			} else
				goto tr78;
		} else if ( _widec > 569 ) {
			if ( _widec < 577 ) {
				if ( 570 <= _widec && _widec <= 576 )
					goto tr66;
			} else if ( _widec > 638 ) {
				if ( 639 <= _widec && _widec <= 767 )
					goto tr66;
			} else
				goto tr78;
		} else
			goto tr78;
	} else
		goto st11;
	goto st0;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	if ( (*p) == 46u )
		goto st58;
	goto st0;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 48u )
		goto st59;
	goto st0;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 13u )
		goto st60;
	goto st0;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 10u )
		goto st61;
	goto st0;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 13u: goto st62;
		case 33u: goto tr109;
		case 124u: goto tr109;
		case 126u: goto tr109;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr109;
		} else if ( (*p) >= 35u )
			goto tr109;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr109;
		} else if ( (*p) >= 48u )
			goto tr109;
	} else
		goto tr109;
	goto st0;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 10u )
		goto st63;
	goto st0;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 269: goto tr111;
		case 525: goto tr113;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr112;
	goto st0;
tr111:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 3986 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st65;
	goto st0;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 13u )
		goto st66;
	goto st0;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 10u )
		goto st78;
	goto st0;
tr112:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st67;
tr117:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st67;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
#line 4018 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 269: goto st64;
		case 525: goto tr118;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr117;
	goto st0;
tr113:
#line 8 "../ragel/formats/warc/warc.rl"
	{ _block_start(); }
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st68;
tr118:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st68;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
#line 4045 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st65;
		case 269: goto st64;
		case 522: goto tr119;
		case 525: goto tr118;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr117;
	goto st0;
tr119:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st69;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
#line 4068 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 269: goto st70;
		case 525: goto tr121;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr117;
	goto st0;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 10u )
		goto st85;
	goto st0;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 13u: goto st66;
		case 87u: goto tr135;
	}
	goto st0;
tr121:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 4105 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 266: goto st85;
		case 269: goto st64;
		case 522: goto tr123;
		case 525: goto tr118;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr117;
	goto st0;
tr123:
#line 9 "../ragel/formats/warc/warc.rl"
	{ _block_consume((*p)); }
	goto st86;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
#line 4128 "schwa/formats/warc_gen.cc"
	_widec = (*p);
	_widec = (short)(256u + ((*p) - 0u));
	if ( 
#line 10 "../ragel/formats/warc/warc.rl"
 _block_consume_test()  ) _widec += 256;
	switch( _widec ) {
		case 269: goto st70;
		case 343: goto tr135;
		case 525: goto tr121;
	}
	if ( 512 <= _widec && _widec <= 767 )
		goto tr117;
	goto st0;
tr109:
#line 13 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_start(); }
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
	goto st72;
tr124:
#line 14 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_consume((*p)); }
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 4156 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 33u: goto tr124;
		case 58u: goto tr125;
		case 124u: goto tr124;
		case 126u: goto tr124;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr124;
		} else if ( (*p) >= 35u )
			goto tr124;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr124;
		} else if ( (*p) >= 48u )
			goto tr124;
	} else
		goto tr124;
	goto st0;
tr125:
#line 15 "../ragel/formats/warc/warc.rl"
	{ _named_field_key_end(); }
	goto st73;
tr128:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st73;
tr126:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
#line 4196 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 9u: goto tr126;
		case 13u: goto tr127;
		case 32u: goto tr128;
		case 127u: goto st0;
	}
	if ( (*p) <= 31u )
		goto st0;
	goto tr129;
tr132:
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st74;
tr127:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 18 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_end(); }
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 4220 "schwa/formats/warc_gen.cc"
	if ( (*p) == 10u )
		goto st75;
	goto st0;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 9u: goto st76;
		case 13u: goto st62;
		case 32u: goto st76;
		case 33u: goto tr109;
		case 124u: goto tr109;
		case 126u: goto tr109;
	}
	if ( (*p) < 45u ) {
		if ( (*p) > 39u ) {
			if ( 42u <= (*p) && (*p) <= 43u )
				goto tr109;
		} else if ( (*p) >= 35u )
			goto tr109;
	} else if ( (*p) > 46u ) {
		if ( (*p) > 57u ) {
			if ( 65u <= (*p) && (*p) <= 122u )
				goto tr109;
		} else if ( (*p) >= 48u )
			goto tr109;
	} else
		goto tr109;
	goto st0;
tr129:
#line 16 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_start(); }
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st76;
tr133:
#line 17 "../ragel/formats/warc/warc.rl"
	{ _named_field_val_consume((*p)); }
	goto st76;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
#line 4265 "schwa/formats/warc_gen.cc"
	switch( (*p) ) {
		case 9u: goto st76;
		case 13u: goto tr132;
		case 127u: goto st0;
	}
	if ( (*p) <= 31u )
		goto st0;
	goto tr133;
	}
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
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
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
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
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
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 13: 
	case 14: 
	case 17: 
	case 18: 
	case 19: 
	case 22: 
	case 24: 
	case 25: 
	case 37: 
	case 38: 
	case 42: 
	case 43: 
	case 45: 
	case 47: 
	case 50: 
	case 56: 
	case 78: 
	case 79: 
	case 80: 
	case 81: 
	case 82: 
	case 83: 
	case 84: 
	case 85: 
	case 86: 
#line 11 "../ragel/formats/warc/warc.rl"
	{ _block_end(); }
#line 21 "../ragel/formats/warc/warc.rl"
	{ _record_end(); }
	break;
#line 4395 "schwa/formats/warc_gen.cc"
	}
	}

	_out: {}
	}

#line 152 "../ragel/formats/warc/warc.rl"

  return cs != 
#line 4405 "schwa/formats/warc_gen.cc"
0
#line 153 "../ragel/formats/warc/warc.rl"
;
}

}  // namespace formats
}  // namespace schwa
