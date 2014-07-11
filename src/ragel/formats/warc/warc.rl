// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */

%%{
  machine warc;
  alphtype unsigned char;

  action update_warc_type {
    warc_type.push_back(fc);
  }
  action update_content_length {
    warc_content_length = 10*warc_content_length + (fc - '0');
  }
  action finalise_content_length {
    if (warc_content_length > warc_content_buffer_nbytes) {
      warc_content_buffer_nbytes = warc_content_length;
      warc_content_buffer.reset(new uint8_t[warc_content_buffer_nbytes]);
    }
  }
  action consume_content {
    warc_content_buffer[warc_content_consumed++] = fc;
  }
  action test_consume_length {
    warc_content_consumed != warc_content_length
  }
  action warc_record_start {
    warc_content_consumed = 0;
    warc_content_length = 0;
    warc_type.clear();
  }
  action warc_record_end {
    if (warc_type == "response") {
      const char *html_ptr = reinterpret_cast<char *>(warc_content_buffer.get());
      const char *const html_ptr_end = html_ptr + warc_content_length;
      bool found = false;
      for ( ; html_ptr != html_ptr_end - 1; ++html_ptr) {
        if (html_ptr[0] == '\n' && html_ptr[1] == '\n') {
          html_ptr += 2;
          found = true;
          break;
        }
      }

      if (found) {
        std::stringstream ss;
        ss.write(html_ptr, html_ptr_end - html_ptr);
        const bool success = _html_lexer.run(ss, _html_buffer_size);
        if (success)
          ++_nsuccess;
        else
          ++_nfail;
      }
      else {
        std::cout << "No HTML content found" << std::endl;
      }
    }
  }

  # RFC3986 (URI)
  rfc3986 = ( any - [\n] )+ ;  # TODO.

  ipv4_number = '2' ( '0'..'4' digit ) | '25' '0'..'5' | '1' digit digit | digit digit | digit ;
  ipv4_address = ipv4_number ( '.' ipv4_number ){3} ;

  # RFC1884 (IPv6 addresses)
  ipv6_address = '' ;  # TODO.

  # RFC3629 (UTF-8)
  utf8_character_1 = 0x01..0x7f ;
  utf8_character_2 = 0xc0..0xdf 0x80..0xbf ;
  utf8_character_3 = 0xe0..0xef 0x80..0xbf 0x80..0xbf ;
  utf8_character_4 = 0xf0..0xf7 0x80..0xbf 0x80..0xbf 0x80..0xbf ;
  utf8_character = utf8_character_1 | utf8_character_2 | utf8_character_3 | utf8_character_4 ;

  # Version 0.18 of the WARC specification.
  # http://archive-access.sourceforge.net/warc/
  char = utf8_character ;
  crlf = '\r'? '\n' ;
  ctl = cntrl | 0x7f ;
  lws = crlf? /[ \t]/+ ;
  octet = any ;
  separators = [][()<>@,;:\"/?={} \t] ;
  text = octet - ctl | lws ;
  token = ( ascii - ctl - separators )+ ;

  uri = '<' rfc3986 '>' | rfc3986 ;
  qdtext = text - ["] ;
  quoted_pair = '\\' char ;
  quoted_string = '"' ( qdtext | quoted_pair )* '"' ;

  mt_attribute = token ;
  mt_value = token | quoted_string ;
  mt_parameter = mt_attribute '=' mt_value ;
  mt_type = token ;
  mt_subtype = token ;
  media_type = mt_type '/' mt_subtype ( ';' mt_parameter )* ;

  named_field =
      'Content-Length'i ':' lws? digit+ $update_content_length %finalise_content_length
    | 'Content-Type'i ':' lws? media_type?
    | 'WARC-Block-Digest'i ':' lws? ( token ':' token )?
    | 'WARC-Concurrent-To'i ':' lws? ( uri ( lws uri )* )?
    | 'WARC-Date'i ':' lws? digit{4} '-' digit{2} '-' digit{2} 'T' digit{2} ':' digit{2} ':' digit{2} [\-+] digit{4}
    | 'WARC-Filename'i ':' lws? ( ascii - space - cntrl )*
    | 'WARC-Identified-Payload-Type'i ':' lws? media_type?
    | 'WARC-IP-Address'i ':' lws? ( ipv4_address | ipv6_address )?
    | 'WARC-Payload-Digest'i ':' lws? ( token ':' token )?
    | 'WARC-Profile'i ':' lws? uri?
    | 'WARC-Record-ID'i ':' lws? uri
    | 'WARC-Refers-To'i ':' lws? uri?
    | 'WARC-Segment-Number'i ':' lws? digit*
    | 'WARC-Segment-Origin-ID'i ':' lws? uri?
    | 'WARC-Segment-Total-Length'i ':' lws? digit*
    | 'WARC-Target-URI'i ':' lws? uri?
    | 'WARC-Truncated'i ':' lws? token?
    | 'WARC-Type'i ':' lws? token $update_warc_type
    | 'WARC-Warcinfo-ID'i ':' lws? uri?
    | token ':' lws? ( ascii - space - cntrl )*
    ;

  warc_fields = ( named_field crlf )* ;
  version = 'WARC/0.18' crlf ;
  header = version warc_fields;
  block = ( any when test_consume_length $consume_content )* ;
  warc_record = header >warc_record_start crlf block crlf crlf %warc_record_end ;

  main := warc_record** ;

}%%


#include <cstring>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <schwa/formats/warc.h>


namespace schwa {
namespace formats {

%% write data noerror nofinal;

bool
WARCLexer::run(std::istream &input, const size_t buffer_size) {
  int cs = 0;
  uint64_t warc_content_consumed = 0, warc_content_length = 0;
  std::unique_ptr<uint8_t[]> warc_content_buffer;
  size_t warc_content_buffer_nbytes = 0;
  std::string warc_type;

  _nfail = _nsuccess = 0;

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

  const float total = _nfail + _nsuccess;
  std::cout << "   nfail: " << _nfail    << "/" << total << " (" << (100*_nfail/total) << "%)" << std::endl;
  std::cout << "nsuccess: " << _nsuccess << "/" << total << " (" << (100*_nsuccess/total) << "%)" << std::endl;
  return true;
}

}  // namespace formats
}  // namespace schwa
