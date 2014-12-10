// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  alphtype unsigned char;

  action b1  { _state.b1 = _state.p; }
  action b2  { _state.b2 = _state.p; }
  action b3  { _state.b3 = _state.p; }

  action s1 { _state.suffix += 1; }
  action s2 { _state.suffix += 2; }
  action s3 { _state.suffix += 3; }
  action s4 { _state.suffix += 4; }
  action s5 { _state.suffix += 5; }
  action s6 { _state.suffix += 6; }
  action s7 { _state.suffix += 7; }
  action s8 { _state.suffix += 8; }
  action s9 { _state.suffix += 9; }
  action s10 { _state.suffix += 10; }

  action S1 { _state.suffix = 1; }
  action S2 { _state.suffix = 2; }
  action S3 { _state.suffix = 3; }
  action S4 { _state.suffix = 4; }
  action S5 { _state.suffix = 5; }
  action S6 { _state.suffix = 6; }
  action S7 { _state.suffix = 7; }
  action S8 { _state.suffix = 8; }
  action S9 { _state.suffix = 9; }
  action S10 { _state.suffix = 10; }

}%%
