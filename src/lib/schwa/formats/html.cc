/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/formats/html.h>

#include <cctype>
#include <iomanip>
#include <iostream>

#include <schwa/encoding.h>


namespace schwa {
namespace formats {

const std::unordered_set<std::string> HTMLLexer::VOID_TAGS = {{
    "area",
    "base",
    "br",
    "col",
    "embed",
    "hr",
    "img",
    "input",
    "keygen",
    "link",
    "menuitem",
    "meta",
    "param",
    "source",
    "track",
    "wbr",
}};

HTMLLexer::HTMLLexer(const bool enable_debug) :
    _encoding_result(nullptr),
    _enable_debug(enable_debug || true),
    _comment_start_ptr(nullptr),
    _cr_start_ptr(nullptr),
    _tag_name_start_ptr(nullptr),
    _tag_self_closing(false)
  { }

HTMLLexer::~HTMLLexer(void) { }


void
HTMLLexer::_debug(const std::string &rule, const uint8_t *const ts, const uint8_t *const te) {
  if (_enable_debug) {
    std::cerr << rule << " " << (te - ts) << ": '";
    std::cerr.write(reinterpret_cast<const char *>(ts), te - ts);
    std::cerr << "'" << std::endl;
  }
}


void
HTMLLexer::_decimal_character_reference_start(const uint8_t *const fpc) {
  _cr_start_ptr = fpc;
}

void
HTMLLexer::_decimal_character_reference_end(const uint8_t *const fpc) {
  unicode_t code_point = 0;
  for (const uint8_t *p = _cr_start_ptr; p != fpc; ++p)
    code_point = 10*code_point + (*p - '0');
  write_utf8(code_point, _text);
}


void
HTMLLexer::_hex_character_reference_start(const uint8_t *const fpc) {
  _cr_start_ptr = fpc;
}

void
HTMLLexer::_hex_character_reference_end(const uint8_t *const fpc) {
  unicode_t code_point = 0;
  for (const uint8_t *p = _cr_start_ptr; p != fpc; ++p) {
    const uint8_t c = *p;
    code_point *= 16;
    if (c >= '0' && c <= '9')
      code_point += c - '0';
    else if (c >= 'A' && c <= 'F')
      code_point += 10 + (c - 'A');
    else
      code_point += 10 + (c - 'a');
  }
  write_utf8(code_point, _text);
}


void
HTMLLexer::_named_character_reference_start(const uint8_t *const fpc) {
  _cr_start_ptr = fpc;
}

void
HTMLLexer::_named_character_reference_end(const uint8_t *const fpc) {
  const std::string name(reinterpret_cast<const char *>(_cr_start_ptr), fpc - _cr_start_ptr);
  const auto &it = NAMED_CHARACTER_REFERENCES.find(name);
  if (it != NAMED_CHARACTER_REFERENCES.end())
    write_utf8(it->second, _text);
}


void
HTMLLexer::_text_character_start(const uint8_t *const fpc) {
  _cr_start_ptr = fpc;
}

void
HTMLLexer::_text_character_end(const uint8_t *const fpc) {
  std::cerr << "[_text_character_end] " << (fpc - _cr_start_ptr) << " '";
  std::cerr.write(reinterpret_cast<const char *>(_cr_start_ptr), fpc - _cr_start_ptr);
  std::cerr << "'" << std::endl;
  _text.write(reinterpret_cast<const char *>(_cr_start_ptr), fpc - _cr_start_ptr);
}


void
HTMLLexer::_cdata_start(const uint8_t *) {
  std::cerr << "[_cdata_start]" << std::endl;
  _text.clear();
  _text.str("");
}

void
HTMLLexer::_cdata_end(const uint8_t *) {
  std::cerr << "[_cdata_end] " << _text.tellp() << std::endl;
  _text.seekg(0);
  std::cerr << "'" <<  _text.str() << "'" << std::endl;
  _text.clear();
  _text.str("");
}


void
HTMLLexer::_comment_start(const uint8_t *const fpc) {
  std::cerr << "[_comment_start]" << std::endl;
  _comment_start_ptr = fpc;
}

void
HTMLLexer::_comment_end(const uint8_t *const fpc) {
  std::cerr << "[_comment_end] " << (fpc - _comment_start_ptr) << std::endl;
  std::cerr << "'";
  std::cerr.write(reinterpret_cast<const char *>(_comment_start_ptr), fpc - _comment_start_ptr);
  std::cerr << "'" << std::endl;
}


void
HTMLLexer::_doctype_start(const uint8_t *const fpc) {
  std::cerr << "[_doctype_start]" << std::endl;
  _comment_start_ptr = fpc;
}

void
HTMLLexer::_doctype_end(const uint8_t *const fpc) {
  std::cerr << "[_doctype_end] " << (fpc - _comment_start_ptr) << std::endl;
  std::cerr << "'";
  std::cerr.write(reinterpret_cast<const char *>(_comment_start_ptr), fpc - _comment_start_ptr);
  std::cerr << "'" << std::endl;
}


void
HTMLLexer::_open_tag_start(const uint8_t *const fpc) {
  _tag_start_ptr = fpc;
  _tag_self_closing = false;
}

void
HTMLLexer::_open_tag_end(const uint8_t *const fpc) {
  std::cerr << "[_open_tag] " << (fpc - _tag_start_ptr) << std::endl;
  std::cerr << "'";
  std::cerr.write(reinterpret_cast<const char *>(_tag_start_ptr), fpc - _tag_start_ptr);
  std::cerr << "'" << std::endl;
}

void
HTMLLexer::_open_tag_name_start(const uint8_t *const fpc) {
  _tag_name_start_ptr = fpc;
}

void
HTMLLexer::_open_tag_name_end(const uint8_t *const fpc) {
  std::cerr << "[_open_tag_name] " << (fpc - _tag_name_start_ptr) << std::endl;
  std::cerr << "'";
  std::cerr.write(reinterpret_cast<const char *>(_tag_name_start_ptr), fpc - _tag_name_start_ptr);
  std::cerr << "'" << std::endl;
}

void
HTMLLexer::_open_tag_self_closing(void) {
  std::cerr << "[_open_tag_self_closing]" << std::endl;
  _tag_self_closing = true;
}


void
HTMLLexer::_close_tag_start(const uint8_t *const fpc) {
  _tag_start_ptr = fpc;
  _tag_self_closing = false;
}

void
HTMLLexer::_close_tag_end(const uint8_t *const fpc) {
  std::cerr << "[_close_tag] " << (fpc - _tag_start_ptr) << std::endl;
  std::cerr << "'";
  std::cerr.write(reinterpret_cast<const char *>(_tag_start_ptr), fpc - _tag_start_ptr);
  std::cerr << "'" << std::endl;
}

void
HTMLLexer::_close_tag_name_start(const uint8_t *const fpc) {
  _tag_name_start_ptr = fpc;
}

void
HTMLLexer::_close_tag_name_end(const uint8_t *const fpc) {
  std::cerr << "[_close_tag_name] " << (fpc - _tag_name_start_ptr) << std::endl;
  std::cerr << "'";
  std::cerr.write(reinterpret_cast<const char *>(_tag_name_start_ptr), fpc - _tag_name_start_ptr);
  std::cerr << "'" << std::endl;
}


void
HTMLLexer::_seen_tag(const uint8_t *const ts, const uint8_t *const te) {
  if (_encoding_result == nullptr)
    return;
  std::cerr << "Tag '";
  std::cerr.write(reinterpret_cast<const char *>(ts), te - ts);
  std::cerr << "'" << std::endl;
}



void
HTMLLexer::_seen_text(const uint8_t *const ts, const uint8_t *const te) {
  if (_encoding_result == nullptr)
    return;
  std::cerr << "Text '";
  std::cerr.write(reinterpret_cast<const char *>(ts), te - ts);
  std::cerr << "'" << std::endl;
}


bool
HTMLLexer::run(const uint8_t *const input, const size_t nbytes) {
  _encoding_result = nullptr;
  _text.clear();
  _text.str("");

  return _run(input, nbytes);
}


bool
HTMLLexer::run(const EncodingResult &er) {
  _encoding_result = &er;
  _text.clear();
  _text.str("");

  return _run(er.utf8(), er.nbytes());
}

}  // namesapce formats
}  // namespace schwa
