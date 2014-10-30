// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine tokenizer;
  access _state.;
  alphtype unsigned char;
  variable eof _state.eof;
  variable p _state.p;
  variable pe _state.pe;

  # All of these rules assume the input byte stream is UTF-8.
  include 'actions.rl';

  include 'rules/unicode.rl';
  include 'rules/numbers.rl';
  include 'rules/punctuation.rl';
  include 'rules/quotes.rl';
  include 'rules/default.rl';

  include 'rules/contractions.rl';
  include 'rules/abbreviations.rl';
  include 'rules/date_time.rl';
  include 'rules/units.rl';
  include 'rules/web.rl';
  include 'rules/emoticons.rl';
  include 'rules/main.rl';
}%%


#include <schwa/new-tokenizer/tokenizer.h>


namespace schwa {
namespace new_tokenizer {

%% write data noerror nofinal;

bool
Tokenizer::_tokenize(void) {
  (void)tokenizer_en_main;
  %% write init;
  %% write exec;
  return _state.cs != %%{ write error; }%%;
}

}  // namespace new_tokenizer
}  // namespace schwa
