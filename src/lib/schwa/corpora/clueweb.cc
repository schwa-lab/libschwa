/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/corpora/clueweb.h>

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <schwa/encoding.h>
#include <schwa/formats/html.h>
#include <schwa/formats/http.h>
#include <schwa/formats/sgml.h>
#include <schwa/formats/warc.h>
#include <schwa/io/streams.h>
#include <schwa/tokenizer/tokenizer.h>


namespace cs = ::schwa::canonical_schema;
namespace fm = ::schwa::formats;
namespace io = ::schwa::io;
namespace tk = ::schwa::tokenizer;

namespace schwa {
namespace corpora {

// ================================================================================================
// ClueWebImporter::Impl
// ================================================================================================
class ClueWebImporter::Impl : public fm::WARCParser {
private:
  const std::string &_path;
  ssize_t _upto;
  fm::HTMLLexer _html_lexer;
  fm::HTMLCharsetSniffer _html_charset_sniffer;
  fm::HTTPParser _http_parser;
  tk::Tokenizer _tokenizer;
  EncodingResult _er;
  std::vector<cs::Doc *> _docs;

  unsigned int _nfail;
  unsigned int _nskipped;
  unsigned int _nsuccess;

  bool _decode_using_charset(const std::string &charset);
  bool _decode_using_encoding(Encoding encoding);

  void _import(void);
  virtual void _record_end(void) override;

public:
  explicit Impl(const std::string &path);
  virtual ~Impl(void);

  cs::Doc *import(void);
};

ClueWebImporter::Impl::Impl(const std::string &path) :
    _path(path),
    _upto(-1),
    _nfail(0),
    _nskipped(0),
    _nsuccess(0)
  { }

ClueWebImporter::Impl::~Impl(void) {
  for (cs::Doc *doc : _docs)
    delete doc;
}


cs::Doc *
ClueWebImporter::Impl::import(void) {
  // If we haven't processed the input file yet, import it.
  if (_upto == -1)
    _import();
  return (static_cast<size_t>(_upto) == _docs.size()) ? nullptr : _docs[_upto++];
}


void
ClueWebImporter::Impl::_import(void) {
  // Open the input source and run the WARC parser.
  io::InputStream in(_path);
  run(in);

  const unsigned int total = _nfail + _nskipped + _nsuccess;
  if (total != 0) {
    const float ftotal = total;
    std::cerr << "   nfail: " << std::setw(5) << _nfail    << "/" << total << " (" << (100*_nfail/total) << "%)" << std::endl;
    std::cerr << "nskipped: " << std::setw(5) << _nskipped << "/" << total << " (" << (100*_nskipped/ftotal) << "%)" << std::endl;
    std::cerr << "nsuccess: " << std::setw(5) << _nsuccess << "/" << total << " (" << (100*_nsuccess/ftotal) << "%)" << std::endl;
  }


  // Reset our document count to zero.
  _upto = 0;
}


bool
ClueWebImporter::Impl::_decode_using_charset(const std::string &charset) {
  // Attempt to convert the string value to an enum value.
  Encoding encoding = Encoding::UTF_8;
  try {
    encoding = get_encoding(charset);
  }
  catch (UnknownEncodingException) {
    return false;
  }
  return _decode_using_encoding(encoding);
}


bool
ClueWebImporter::Impl::_decode_using_encoding(const Encoding encoding) {
  // Attempt to convert the encoded payload to UTF-8.
  try {
    to_utf8(encoding, _http_parser.message(), _http_parser.message_nbytes(), _er);
  }
  catch (DecodeException) {
    return false;
  }
  return true;
}


void
ClueWebImporter::Impl::_record_end(void) {
  bool success;
  WARCParser::_record_end();

  // Only bother processing HTTP response WARC records.
  if (_warc_type != "response")
    return;

  // Parse the HTTP message.
  success = _http_parser.run(_block_buffer, _block_nbytes_consumed);
  if (!success) {
    //std::cerr << "[" << warc_trec_id() << "][0] failed to parse HTTP headers" << std::endl;
    //std::cerr.write(reinterpret_cast<const char *>(_block_buffer), std::min(_block_nbytes_consumed, static_cast<size_t>(512)));
    //std::cerr << std::endl;
    ++_nskipped;
    return;
  }

  // Reject the reqponse if it was not a success status or if it's not a HTML response.
  if (_http_parser.status_code() != 200 || _http_parser.content_type() != "text/html") {
    //std::cerr << "[" << warc_trec_id() << "][0] " << _http_parser.status_code() << " '" << _http_parser.content_type() << "'" << std::endl;
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

  success = _html_lexer.run(_er);
  if (success)
    ++_nsuccess;
  else {
    std::ostringstream path;
    path << "/tmp/html/" << warc_trec_id() << ".html";
    std::ofstream f(path.str());
    f.write(reinterpret_cast<const char *>(_http_parser.message()), _http_parser.message_nbytes());
    std::cerr << "[" << warc_trec_id() << "]" << std::endl;
   ++_nfail;
  }
}


// ================================================================================================
// ClueWebImporter
// ================================================================================================
ClueWebImporter::ClueWebImporter(const std::string &path) : _impl(new Impl(path)) { }

ClueWebImporter::~ClueWebImporter(void) {
  delete _impl;
}

cs::Doc *
ClueWebImporter::import(void) {
  return _impl->import();
}

}  // namesapce corpora
}  // namespace schwa
