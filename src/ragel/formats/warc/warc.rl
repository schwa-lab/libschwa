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

  # The WARC format specifications:
  #   Version 0.18: http://archive-access.sourceforge.net/warc/
  #   Version 1.0: http://bibnum.bnf.fr/WARC/WARC_ISO_28500_version1_latestdraft.pdf

  # ===========================================================================
  # Version 0.18.
  # ===========================================================================
  warc_0_18_crlf = '\r'? '\n' ;
  warc_0_18_ctl = cntrl | 0x7f ;
  warc_0_18_lws = warc_0_18_crlf? [ \t]+ ;
  warc_0_18_separators = [][()<>@,;:\"/?={} \t] ;
  warc_0_18_token = ( ascii - warc_0_18_ctl - warc_0_18_separators )+ ;

  warc_0_18_named_field =
      warc_0_18_token >named_field_key_start $named_field_key_consume %named_field_key_end
      ':'
      warc_0_18_lws? <:
      ( any - [\r\n] )* >named_field_val_start $named_field_val_consume %named_field_val_end
    | ( any - [\n] )+  # This is here as an attempt to recover from the broken UTF-16 data in ClueWeb09.
    ;

  warc_0_18_warc_fields = ( warc_0_18_named_field warc_0_18_crlf )* ;
  warc_0_18_version = 'WARC/0.18' warc_0_18_crlf ;
  warc_0_18_header = warc_0_18_version warc_0_18_warc_fields;
  warc_0_18_block = ( any when block_consume_test $block_consume )* ;
  warc_0_18_warc_record =
      warc_0_18_header >record_start warc_0_18_crlf
      warc_0_18_block >block_start
      warc_0_18_crlf warc_0_18_crlf %block_end %record_end ;

  # ===========================================================================
  # Version 1.0.
  # ===========================================================================
  warc_1_0_crlf = '\r\n' ;
  warc_1_0_ctl = cntrl | 0x7f ;
  warc_1_0_lws = warc_1_0_crlf? [ \t]+ ;
  warc_1_0_separators = [][()<>@,;:\"/?={} \t] ;
  warc_1_0_token = ( ascii - warc_1_0_ctl - warc_1_0_separators )+ ;

  warc_1_0_field_content = warc_1_0_lws | ( any - warc_1_0_ctl ) $named_field_val_consume ;
  warc_1_0_field_value = [ \t]* warc_1_0_field_content* >named_field_val_start %named_field_val_end ;
  warc_1_0_field_name = warc_1_0_token >named_field_key_start $named_field_key_consume %named_field_key_end ;
  warc_1_0_named_field = warc_1_0_field_name ':' warc_1_0_field_value ;

  warc_1_0_warc_fields = ( warc_1_0_named_field warc_1_0_crlf )* ;
  warc_1_0_version = 'WARC/1.0' warc_1_0_crlf ;
  warc_1_0_header = warc_1_0_version warc_1_0_warc_fields;
  warc_1_0_block = ( any when block_consume_test $block_consume )* ;
  warc_1_0_warc_record =
      warc_1_0_header >record_start warc_1_0_crlf
      warc_1_0_block >block_start warc_1_0_crlf warc_1_0_crlf %block_end %record_end ;

  # ===========================================================================
  main := ( warc_0_18_warc_record | warc_1_0_warc_record )** ;
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
  (void)warc_en_main;  // Shoosh compiler warning about unused variable.
  const uint8_t *te = nullptr, *ts = nullptr;
  int cs = 0;

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


bool
WARCLexer::_run(const uint8_t *const input, const size_t nbytes) {
  const uint8_t *p = input, *pe = p + nbytes, *eof = pe;
  int cs = 0;

  %% write init;
  %% write exec;

  return cs != %%{ write error; }%%;
}

}  // namespace formats
}  // namespace schwa
