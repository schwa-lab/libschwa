/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/formats/warc.h>

#include <cassert>
#include <cctype>
#include <iostream>


namespace schwa {
namespace formats {

const size_t WARCLexer::DEFAULT_BUFFER_SIZE = 4096;

WARCLexer::WARCLexer(void) :
    _block_buffer(nullptr),
    _block_buffer_nbytes(0),
    _block_nbytes_stated(0),
    _block_nbytes_consumed(0),
    _in_key_content_length(false),
    _in_key_warc_type(false)
  { }

WARCLexer::~WARCLexer(void) {
  delete [] _block_buffer;
}


void
WARCLexer::_record_start(void) {
  //std::cerr << "[WARCLexer::_record_start]" << std::endl;
  _block_nbytes_consumed = 0;
  _block_nbytes_stated = 0;
  _warc_type.clear();
  _current_key.clear();
}


void
WARCLexer::_record_end(void) {
  //std::cerr << "[WARCLexer::_record_end]" << std::endl;
}


void
WARCLexer::_block_start(void) {
  //std::cerr << "[WARCLexer::_block_start]" << std::endl;
  assert(_block_nbytes_stated != 0);
  if (_block_nbytes_stated > _block_buffer_nbytes) {
    delete [] _block_buffer;
    _block_buffer_nbytes = _block_nbytes_stated;
    _block_buffer = new uint8_t[_block_buffer_nbytes];
  }
}


void
WARCLexer::_block_consume(const uint8_t c) {
  //std::cerr << "[WARCLexer::_block_consume] " << static_cast<unsigned int>(c) << std::endl;
  _block_buffer[_block_nbytes_consumed++] = c;
}


bool
WARCLexer::_block_consume_test(void) {
  //std::cerr << "[WARCLexer::_block_consume_test] " << _block_nbytes_consumed << " != " << _block_nbytes_stated << std::endl;
  return _block_nbytes_consumed != _block_nbytes_stated;
}


void
WARCLexer::_block_end(void) {
  //std::cerr << "[WARCLexer::_block_end]" << std::endl;
  assert(_block_nbytes_consumed == _block_nbytes_stated);
}


void
WARCLexer::_named_field_key_start(void) {
  _current_key.clear();
  _in_key_content_length = false;
  _in_key_warc_type = false;
}


void
WARCLexer::_named_field_key_consume(const uint8_t c_) {
  const char c = static_cast<char>(c_);
  _current_key.push_back(std::tolower(c));
}


void
WARCLexer::_named_field_key_end(void) {
  //std::cerr << "[WARCLexer::_named_field_key_end] '" << _current_key << "'" << std::endl;
  if (_current_key == "content-length")
    _in_key_content_length = true;
  else if (_current_key == "warc-type")
    _in_key_warc_type = true;
}


void
WARCLexer::_named_field_val_start(void) {
  //std::cerr << "[WARCLexer::_named_field_val_start]" << std::endl;
  if (_in_key_content_length)
    _block_nbytes_stated = 0;
  else if (_in_key_warc_type)
    _warc_type.clear();
}


void
WARCLexer::_named_field_val_consume(const uint8_t c_) {
  //std::cerr << "[WARCLexer::_named_field_val_consume]" << std::endl;
  const char c = static_cast<char>(c_);
  if (_in_key_content_length)
    _block_nbytes_stated = 10*_block_nbytes_stated + (c - '0');
  else if (_in_key_warc_type)
    _warc_type.push_back(std::tolower(c));
}


void
WARCLexer::_named_field_val_end(void) {
  //std::cerr << "[WARCLexer::_named_field_val_end] '" << _current_key << "'" << std::endl;
  //if (_in_key_content_length)
    //std::cerr << "_block_nbytes_stated=" << _block_nbytes_stated << std::endl;
  //else if (_in_key_warc_type)
    //std::cerr << "_warc_type=" << _warc_type << std::endl;
}


bool
WARCLexer::run(std::istream &input, const size_t buffer_size) {
  return _run(input, buffer_size);
}


bool
WARCLexer::run(const uint8_t *input, const size_t nbytes) {
  return _run(input, nbytes);
}

}  // namesapce formats
}  // namespace schwa
