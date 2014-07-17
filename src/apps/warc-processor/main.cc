/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#include <schwa/config.h>
#include <schwa/encoding.h>
#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/formats/html.h>
#include <schwa/formats/http.h>
#include <schwa/formats/warc.h>

namespace cf = schwa::config;
namespace fm = schwa::formats;
namespace io = schwa::io;


namespace schwa {
namespace formats {

class WARCHTMLParser : public WARCParser {
protected:
  HTMLLexer _html_lexer;
  HTMLCharsetSniffer _html_charset_sniffer;
  HTTPParser _http_parser;
  EncodingResult _encoding_result;
  unsigned int _nfail;
  unsigned int _nskipped;
  unsigned int _nsuccess;

  bool _decode_using_charset(const std::string &charset);
  bool _decode_using_encoding(Encoding encoding);

  virtual void _record_end(void) override;

public:
  WARCHTMLParser(void);
  virtual ~WARCHTMLParser(void) { }

  virtual bool run(std::istream &input, const size_t buffer_size) override;
};


WARCHTMLParser::WARCHTMLParser(void) {
  _encoding_result.reserve(1 * 1024 * 1024);
}


bool
WARCHTMLParser::_decode_using_charset(const std::string &charset) {
  // Attempt to convert the string value to an enum value.
  try {
    return _decode_using_encoding(get_encoding(charset));
  }
  catch (UnknownEncodingException) {
    std::cerr << "[" << warc_trec_id() << "][1] Failed to convert charset '" << charset << "' to encoding" << std::endl;
    return false;
  }
}


bool
WARCHTMLParser::_decode_using_encoding(const Encoding encoding) {
  // Attempt to convert the encoded payload to UTF-8.
  try {
    to_utf8(encoding, _http_parser.message(), _http_parser.message_nbytes(), _encoding_result);
    return true;
  }
  catch (DecodeException) {
    return false;
  }
}


void
WARCHTMLParser::_record_end(void) {
  bool success;
  WARCParser::_record_end();

  // Only bother processing HTTP response WARC records.
  if (_warc_type != "response")
    return;

  // Parse the HTTP message.
  success = _http_parser.run(_block_buffer, _block_nbytes_consumed);
  if (!success) {
    std::cerr.write(reinterpret_cast<const char *>(_block_buffer), _block_nbytes_consumed);
    std::cerr << std::endl;
    assert(success);
  }

  // Reject the reqponse if it was not a success status or if it's not a HTML response.
  if (_http_parser.status_code() != 200 || _http_parser.content_type() != "text/html") {
    std::cerr << "[" << warc_trec_id() << "][0] " << _http_parser.status_code() << " '" << _http_parser.content_type() << "'" << std::endl;
    ++_nskipped;
    return;
  }

  bool successfully_decoded = false;
  if (_http_parser.has_charset())
    successfully_decoded = _decode_using_charset(_http_parser.charset());

  if (!successfully_decoded) {
    // Attempt to extract the charset from the <meta> tag.
    _html_charset_sniffer.run(_http_parser.message(), _http_parser.message_nbytes());
    if (_html_charset_sniffer.has_charset())
      successfully_decoded = _decode_using_charset(_html_charset_sniffer.charset());
  }

  if (!successfully_decoded) {
    // TODO detect character encoding.
    // Guess WINDOWS_1252.
    successfully_decoded = _decode_using_encoding(Encoding::WINDOWS_1252);
  }

  // If we didn't manage to correctly convert the payload to UTF-8, bail.
  if (!successfully_decoded) {
    ++_nskipped;
    return;
  }

  success = _html_lexer.run(_encoding_result);
  if (success)
    ++_nsuccess;
  else
   ++_nfail;
}


bool
WARCHTMLParser::run(std::istream &input, const size_t buffer_size) {
  _nfail = _nskipped = _nsuccess = 0;

  const bool ret = _run(input, buffer_size);

  const unsigned int total = _nfail + _nskipped + _nsuccess;
  if (total != 0) {
    const float ftotal = total;
    std::cerr << "   nfail: " << std::setw(5) << _nfail    << "/" << total << " (" << (100*_nfail/total) << "%)" << std::endl;
    std::cerr << "nskipped: " << std::setw(5) << _nskipped << "/" << total << " (" << (100*_nskipped/ftotal) << "%)" << std::endl;
    std::cerr << "nsuccess: " << std::setw(5) << _nsuccess << "/" << total << " (" << (100*_nsuccess/ftotal) << "%)" << std::endl;
  }

  return ret;
}

}  // namespace formats
}  // namesapce schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("warc-processor", "Test WARC file processor.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<size_t> warc_buffer(cfg, "warc-buffer", "WARC lexer input buffer size (bytes)", fm::WARCParser::DEFAULT_BUFFER_SIZE);
  cf::Op<bool> html(cfg, "html", "Lex HTML only", false);

  bool success = true;
  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    io::InputStream in(input_path());
    if (html()) {
      fm::HTMLLexer lexer(true);

      const size_t nbytes = 4 * 1024 * 1024;
      std::unique_ptr<char []> buf(new char[nbytes]);
      std::istream &input = in;
      input.read(buf.get(), nbytes);
      assert(input.gcount() < static_cast<ssize_t>(nbytes));

      success = lexer.run(reinterpret_cast<const uint8_t *>(buf.get()), nbytes);
    }
    else {
      fm::WARCHTMLParser lexer;
      success = lexer.run(in, warc_buffer());
    }
  })
  return success ? 0 : 1;
}
