/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <string>

#include <schwa/config.h>
#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/formats/html.h>
#include <schwa/formats/warc.h>

namespace cf = schwa::config;
namespace fm = schwa::formats;
namespace io = schwa::io;


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("warc-processor", "Test WARC file processor.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<size_t> html_buffer(cfg, "html-buffer", "HTML lexer input buffer size (bytes)", fm::HTMLLexer::DEFAULT_BUFFER_SIZE);
  cf::Op<size_t> warc_buffer(cfg, "warc-buffer", "WARC lexer input buffer size (bytes)", fm::WARCLexer::DEFAULT_BUFFER_SIZE);

  bool success = true;
  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    io::InputStream in(input_path());
    fm::WARCLexer lexer(html_buffer());
    success = lexer.run(in, warc_buffer());
    std::cout << "lexer returned " << success << std::endl;
  })
  return success ? 0 : 1;
}
