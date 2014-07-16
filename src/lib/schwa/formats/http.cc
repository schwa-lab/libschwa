#include <schwa/formats/http.h>

#include <cassert>
#include <cctype>
#include <iostream>


namespace schwa {
namespace formats {

HTTPLexer::HTTPLexer(void) : _content_length_consumed(0), _content_length_stated(0) { }

HTTPLexer::~HTTPLexer(void) { }


void
HTTPLexer::_field_name_start(const uint8_t *) {
  _current_key.clear();
}

void
HTTPLexer::_field_name_consume(const uint8_t c_) {
  const char c = static_cast<char>(c_);
  _current_key.push_back(std::tolower(c));
}

void
HTTPLexer::_field_name_end(const uint8_t *) {
  std::cerr << "[HTTPLexer::field_name_end] '" << _current_key << "'" << std::endl;
  _in_content_length = _current_key == "content-length";
  if (_in_content_length) {
    _content_length_stated = 0;
    _seen_content_length = true;
  }
}

void
HTTPLexer::_field_value_start(const uint8_t *) {
  _current_val.clear();
}

void
HTTPLexer::_field_value_consume(const uint8_t c) {
  _current_val.push_back(static_cast<char>(c));
  if (_in_content_length) {
    _content_length_stated = 10*_content_length_stated + (c - '0');
    std::cerr << "[HTTPLexer::field_value_consume] _content_length_stated=" << _content_length_stated << std::endl;
  }
}

void
HTTPLexer::_field_value_end(const uint8_t *) {
  std::cerr << "[HTTPLexer::field_value_end] '" << _current_val << "'" << std::endl;
}


void
HTTPLexer::_message_body_start(const uint8_t *) { }

void
HTTPLexer::_message_body_consume(const uint8_t) {
  ++_content_length_consumed;  // FIXME
}

bool
HTTPLexer::_message_body_test(void) {
  return _seen_content_length ? _content_length_consumed != _content_length_stated : true;
}

void
HTTPLexer::_message_body_end(const uint8_t *) { }


void
HTTPLexer::_reset(void) {
  _content_length_consumed = 0;
  _content_length_stated = 0;
  _in_content_length = false;
  _seen_content_length = false;
}


bool
HTTPLexer::run(const uint8_t *const input, const size_t nbytes) {
  _reset();
  return _run(input, nbytes);
}

}  // namespace formats
}  // namespace schwa
