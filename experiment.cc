
#line 1 "experiment.rl"
/* -*- Mode: C++; indent-tabs-mode: nil -*- */


#line 33 "experiment.rl"


#include <iostream>
#include <cstring>

using namespace std;


#line 16 "experiment.cc"
static const int tokenizer_start = 33;
static const int tokenizer_error = 0;

static const int tokenizer_en_main = 33;


#line 41 "experiment.rl"

int
main(void){
  const static size_t buffer_size = 1024;
  char *buffer = new char[buffer_size];

  int cs = 0;
  const char *ts = 0;
  const char *te = 0;
  int counter = 0;
  int act = 0;

  
#line 37 "experiment.cc"
	{
	cs = tokenizer_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 54 "experiment.rl"

  bool done = false;
  size_t have = 0;
  while(!done){
    size_t space = buffer_size - have;
    if(space == 0)
      return false;

    char *p = buffer + have;
    cin.read(p, space);
    size_t nread = cin.gcount();
    char *pe = p + nread;

    char *eof = 0;
    if(nread < space){
      eof = pe;
      done = true;
    }

    
#line 66 "experiment.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr5:
#line 11 "experiment.rl"
	{te = p+1;{
    if(counter != 0){
      const char *split = te - counter;
      cout.write(ts, split - ts) << ' ';
      cout.write(split, te - split) << endl;
    }else
      cout.write(ts, te - ts) << endl;
    counter = 0;
  }}
	goto st33;
tr17:
#line 6 "experiment.rl"
	{ counter++; }
#line 11 "experiment.rl"
	{te = p+1;{
    if(counter != 0){
      const char *split = te - counter;
      cout.write(ts, split - ts) << ' ';
      cout.write(split, te - split) << endl;
    }else
      cout.write(ts, te - ts) << endl;
    counter = 0;
  }}
	goto st33;
tr18:
#line 7 "experiment.rl"
	{ counter += 2; }
#line 11 "experiment.rl"
	{te = p+1;{
    if(counter != 0){
      const char *split = te - counter;
      cout.write(ts, split - ts) << ' ';
      cout.write(split, te - split) << endl;
    }else
      cout.write(ts, te - ts) << endl;
    counter = 0;
  }}
	goto st33;
tr33:
#line 8 "experiment.rl"
	{ counter += 5; }
#line 11 "experiment.rl"
	{te = p+1;{
    if(counter != 0){
      const char *split = te - counter;
      cout.write(ts, split - ts) << ' ';
      cout.write(split, te - split) << endl;
    }else
      cout.write(ts, te - ts) << endl;
    counter = 0;
  }}
	goto st33;
st33:
#line 1 "NONE"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof33;
case 33:
#line 1 "NONE"
	{ts = p;}
#line 134 "experiment.cc"
	switch( (*p) ) {
		case 38: goto st1;
		case 39: goto tr5;
		case 99: goto st8;
		case 111: goto st20;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 35: goto st2;
		case 97: goto st5;
	}
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) == 51 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) == 57 )
		goto st4;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 59 )
		goto tr5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 112 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 111 )
		goto st7;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 115 )
		goto st4;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 97 )
		goto st9;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 110 )
		goto st10;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 38: goto st11;
		case 39: goto tr11;
		case 115: goto tr5;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 35: goto st12;
		case 97: goto st16;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 51 )
		goto st13;
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	if ( (*p) == 57 )
		goto st14;
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	if ( (*p) == 59 )
		goto tr16;
	goto st0;
tr11:
#line 6 "experiment.rl"
	{ counter++; }
	goto st15;
tr16:
#line 8 "experiment.rl"
	{ counter += 5; }
	goto st15;
tr22:
#line 9 "experiment.rl"
	{ counter += 6; }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 266 "experiment.cc"
	switch( (*p) ) {
		case 115: goto tr17;
		case 116: goto tr18;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 112 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 111 )
		goto st18;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( (*p) == 115 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 59 )
		goto tr22;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 38: goto st21;
		case 39: goto st25;
	}
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 35: goto st22;
		case 97: goto st30;
	}
	goto st0;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 51 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) == 57 )
		goto st24;
	goto st0;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 59 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	if ( (*p) == 99 )
		goto st26;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	if ( (*p) == 108 )
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	if ( (*p) == 111 )
		goto st28;
	goto st0;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 99 )
		goto st29;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	if ( (*p) == 107 )
		goto tr33;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 112 )
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 111 )
		goto st32;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 115 )
		goto st24;
	goto st0;
	}
	_test_eof33: cs = 33; goto _test_eof; 
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

	_test_eof: {}
	_out: {}
	}

#line 74 "experiment.rl"

    if(ts == 0)
      have = 0;
    else {
      /* There is a prefix to preserve, shift it over. */
      have = pe - ts;
      memmove(buffer, ts, have);
      te = buffer + (te - ts);
      ts = buffer;
    }
  }

  return 0;
}
