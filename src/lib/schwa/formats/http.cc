#include <schwa/formats/http.h>

#include <cassert>
#include <cctype>
#include <iostream>


namespace schwa {
namespace formats {

HTTPParser::HTTPParser(void) :
    _message_ptr(nullptr),
    _content_length_consumed(0),
    _content_length_stated(0),
    _status_code(0)
  { }

HTTPParser::~HTTPParser(void) { }


void
HTTPParser::_content_length_start(const uint8_t *) {
  _content_length_stated = 0;
}
void
HTTPParser::_content_length_consume(const uint8_t c) {
  _content_length_stated = 10*_content_length_stated + (c - '0');
}
void
HTTPParser::_content_length_end(const uint8_t *) {
  _seen_content_length = true;
}


void
HTTPParser::_status_code_consume(const uint8_t c) {
  _status_code = 10*_status_code + (c - '0');
}


void
HTTPParser::_content_type_type_start(const uint8_t *const fpc) {
  _content_type_ptr = fpc;
}
void
HTTPParser::_content_type_type_end(const uint8_t *const fpc) {
  for (const uint8_t *p = _content_type_ptr; p != fpc; ++p) {
    const char c = std::tolower(*reinterpret_cast<const char *>(p));
    _content_type.push_back(c);
  }
  _content_type.push_back('/');
}

void
HTTPParser::_content_type_subtype_start(const uint8_t *const fpc) {
  _content_type_ptr = fpc;
}
void
HTTPParser::_content_type_subtype_end(const uint8_t *const fpc) {
  for (const uint8_t *p = _content_type_ptr; p != fpc; ++p) {
    const char c = std::tolower(*reinterpret_cast<const char *>(p));
    _content_type.push_back(c);
  }
}

void
HTTPParser::_content_type_param_key_start(const uint8_t *const fpc) {
  _content_type_ptr = fpc;
}
void
HTTPParser::_content_type_param_key_end(const uint8_t *const fpc) {
  std::string key;
  for (const uint8_t *p = _content_type_ptr; p != fpc; ++p)
    key.push_back(std::tolower(*reinterpret_cast<const char *>(p)));

  if (key == "charset") {
    _in_content_type_charset = true;
    _content_type_charset.clear();
  }
}

void
HTTPParser::_content_type_param_val_start(const uint8_t *const fpc) {
  _content_type_ptr = fpc;
}
void
HTTPParser::_content_type_param_val_end(const uint8_t *const fpc) {
  if (!_in_content_type_charset)
    return;
  for (const uint8_t *p = _content_type_ptr; p != fpc; ++p) {
    const char c = std::toupper(*reinterpret_cast<const char *>(p));
    if (c != '"')
      _content_type_charset.push_back(c);
  }
}


void
HTTPParser::_message_body_start(const uint8_t *const fpc) {
  _message_ptr = fpc;
}
void
HTTPParser::_message_body_consume(const uint8_t) {
  ++_content_length_consumed;
}
bool
HTTPParser::_message_body_test(void) {
  return _seen_content_length ? _content_length_consumed != _content_length_stated : true;
}
void
HTTPParser::_message_body_end(const uint8_t *) { }


void
HTTPParser::_reset(void) {
  _content_length_consumed = 0;
  _content_length_stated = 0;
  _message_ptr = nullptr;
  _in_content_length = false;
  _seen_content_length = false;
  _content_type.clear();
  _content_type_charset.clear();
  _in_content_type_charset = false;
  _status_code = 0;
}


bool
HTTPParser::run(const uint8_t *const input, const size_t nbytes) {
  _reset();
  return _run(input, nbytes);
}

}  // namespace formats
}  // namespace schwa
