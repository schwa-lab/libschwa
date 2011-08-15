/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;

  action r1 { counter++; }
  action r2 { counter += 2; }
  action r5 { counter += 5; }
  action r6 { counter += 6; }

  action tokens {
    if(counter != 0){
      const char *split = te - counter;
      cout.write(ts, split - ts) << ' ';
      cout.write(split, te - split) << endl;
    }else
      cout.write(ts, te - ts) << endl;
    counter = 0;
  }

  single_quote = ("'" | "&#39;" | "&apos;" );
  single_quote_capture = ("'" @r1 | "&#39;" @r5 | "&apos;" @r6);
  letter = alpha | alpha "'" alpha;

  cant = "can" single_quote_capture "t" @r2;
  cans = "cans";
  can_poss = letter+ single_quote_capture "s" @r1;
  oclock = "o" single_quote "clock" @r5;

  main := |*
    single_quote | cant | cans | can_poss | oclock => tokens;
  *|;

}%%

#include <iostream>
#include <cstring>

using namespace std;

%% write data nofinal;

int
main(void){
  const static size_t buffer_size = 1024;
  char *buffer = new char[buffer_size];

  int cs = 0;
  const char *ts = 0;
  const char *te = 0;
  int counter = 0;
  int act = 0;

  %% write init;

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

    %% write exec;

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
