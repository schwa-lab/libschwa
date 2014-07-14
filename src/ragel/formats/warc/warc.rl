// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine warc;
  alphtype unsigned char;

  action block_start { _block_start(); }
  action block_consume { _block_consume(fc); }
  action block_consume_test { _block_consume_test() }
  action block_end { _block_end(); }

  action named_field_key_start { _named_field_key_start(); }
  action named_field_key_consume { _named_field_key_consume(fc); }
  action named_field_key_end { _named_field_key_end(); }
  action named_field_val_start { _named_field_val_start(); }
  action named_field_val_consume { _named_field_val_consume(fc); }
  action named_field_val_end { _named_field_val_end(); }

  action record_start { _record_start(); }
  action record_end { _record_end(); }

  # Version 0.18 of the WARC specification.
  # http://archive-access.sourceforge.net/warc/
  crlf = '\r'? '\n' ;
  ctl = cntrl | 0x7f ;
  lws = crlf? /[ \t]/+ ;
  separators = [][()<>@,;:\"/?={} \t] ;
  token = ( ascii - ctl - separators )+ ;

  named_field =
      token >named_field_key_start $named_field_key_consume %named_field_key_end ':' lws? <: ( any - [\n] )* >named_field_val_start $named_field_val_consume %named_field_val_end
    | ( any - [\n] )+  # This is here as an attempt to recover from the broken UTF-16 data in ClueWeb09.
    ;

  warc_fields = ( named_field crlf )* ;
  version = 'WARC/0.18' crlf ;
  header = version warc_fields;
  block = ( any when block_consume_test $block_consume )* ;
  warc_record = header >record_start crlf block >block_start crlf crlf %block_end %record_end ;

  main := warc_record** ;
}%%


#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <schwa/formats/warc.h>


namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
WARCLexer::_run(std::istream &input, const size_t buffer_size) {
  int cs = 0;
  uint8_t *te = nullptr, *ts = nullptr;
  (void)warc_en_main;  // Shoosh compiler warning about unused variable.

  %% write init;

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

    %% write exec;

    if (cs == %%{ write error; }%%)
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

}  // namespace formats
}  // namespace schwa
