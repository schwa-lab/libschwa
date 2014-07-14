/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include <schwa/config.h>
#include <schwa/encoding.h>
#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/formats/html.h>
#include <schwa/formats/warc.h>

namespace cf = schwa::config;
namespace fm = schwa::formats;
namespace io = schwa::io;


namespace schwa {
namespace formats {

class WARCHTMLLexer : public WARCLexer {
protected:
  const size_t _html_buffer_size;
  HTMLLexer _html_lexer;
  EncodingResult _encoding_result;
  unsigned int _nfail;
  unsigned int _nsuccess;

  virtual void _record_end(void) override;

public:
  WARCHTMLLexer(const size_t html_buffer_size) : WARCLexer(), _html_buffer_size(html_buffer_size) { }
  virtual ~WARCHTMLLexer(void) { }

  virtual bool run(std::istream &input, const size_t buffer_size) override;
};


void
WARCHTMLLexer::_record_end(void) {
  WARCLexer::_record_end();

  // Only bother processing HTTP response WARC records.
  if (_warc_type != "response")
    return;

  // Attempt to find the end of the HTTP headers which should be the start of the HTML payload.
  const char *html_ptr = reinterpret_cast<char *>(_block_buffer);
  const char *const html_ptr_end = html_ptr + _block_nbytes_consumed;
  bool found = false;
  for ( ; html_ptr != html_ptr_end - 1; ++html_ptr) {
    if (html_ptr[0] == '\n' && html_ptr[1] == '\n') {
      html_ptr += 2;
      found = true;
      break;
    }
  }

  // If we didn't find the end of the HTTP headers, bail.
  if (!found) {
    std::cerr << "No HTML content found" << std::endl;
    ++_nfail;
    return;
  }

  bool success;
  std::stringstream ss;
  ss.write(html_ptr, html_ptr_end - html_ptr);

  // Attempt to lex the HTML.
  success = _html_lexer.run(ss, _html_buffer_size);
  if (success)
    ++_nsuccess;
  else {
    // Try converting it from latin2 (ISO-8859-2) to UTF-8.
    to_utf8(Encoding::LATIN2, html_ptr, html_ptr_end - html_ptr, _encoding_result);
    ss.str("");
    ss.clear();
    ss.write(reinterpret_cast<const char *>(_encoding_result.utf8()), _encoding_result.nbytes());
    success = _html_lexer.run(ss, _html_buffer_size);
    if (success)
      ++_nsuccess;
    else {
#if 0
      std::ostringstream path;
      path << "/tmp/html/" << std::setfill('0') << std::setw(5) << _nfail << ".html";
      std::ofstream f(path.str());
      f.write(html_ptr, html_ptr_end - html_ptr);
      f.close();
#endif
      ++_nfail;
    }
  }
}


bool
WARCHTMLLexer::run(std::istream &input, const size_t buffer_size) {
  _nfail = _nsuccess = 0;

  const bool ret = _run(input, buffer_size);

  const unsigned int total = _nfail + _nsuccess;
  if (total != 0) {
    std::cerr << "   nfail: " << _nfail    << "/" << total << " (" << (100*_nfail/static_cast<float>(total)) << "%)" << std::endl;
    std::cerr << "nsuccess: " << _nsuccess << "/" << total << " (" << (100*_nsuccess/static_cast<float>(total)) << "%)" << std::endl;
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
  cf::Op<size_t> html_buffer(cfg, "html-buffer", "HTML lexer input buffer size (bytes)", fm::HTMLLexer::DEFAULT_BUFFER_SIZE);
  cf::Op<size_t> warc_buffer(cfg, "warc-buffer", "WARC lexer input buffer size (bytes)", fm::WARCLexer::DEFAULT_BUFFER_SIZE);
  cf::Op<bool> html(cfg, "html", "Lex HTML only", false);

  bool success = true;
  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    io::InputStream in(input_path());
    if (html()) {
      fm::HTMLLexer lexer(true);
      success = lexer.run(in, html_buffer());
    }
    else {
      fm::WARCHTMLLexer lexer(html_buffer());
      success = lexer.run(in, warc_buffer());
    }
  })
  return success ? 0 : 1;
}
