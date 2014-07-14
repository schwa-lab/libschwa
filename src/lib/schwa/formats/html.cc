/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/formats/html.h>

#include <iostream>

#include <schwa/encoding.h>


namespace schwa {
namespace formats {

const size_t HTMLLexer::DEFAULT_BUFFER_SIZE = 1 * 1024 * 1024;  // 1MB.


HTMLLexer::HTMLLexer(const bool enable_debug) : _enable_debug(enable_debug) { }

HTMLLexer::~HTMLLexer(void) { }


void
HTMLLexer::_debug(const std::string &rule, const uint8_t *const ts, const uint8_t *const te) {
  if (_enable_debug) {
    std::cerr << rule << " " << (te - ts) << ": '";
    std::cerr.write(reinterpret_cast<const char *>(ts), te - ts);
    std::cerr << "'" << std::endl;
  }
}

bool
HTMLLexer::run(std::istream &input, const size_t buffer_size) {
  return _run(input, buffer_size);
}


bool
HTMLLexer::run(const uint8_t *const input, const size_t nbytes) {
  return _run(input, nbytes);
}


bool
HTMLLexer::run(const EncodingResult &er) {
  return _run(er.utf8(), er.nbytes());
}

}  // namesapce formats
}  // namespace schwa
