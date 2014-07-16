/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/formats/html.h>

#include <cctype>
#include <iomanip>
#include <iostream>

#include <schwa/encoding.h>


namespace schwa {
namespace formats {

// http://www.w3.org/html/wg/drafts/html/master/syntax.html#formatting
const std::unordered_set<std::string> HTMLLexer::FORMATTING_TAGS = {{
    "a",
    "b",
    "big",
    "code",
    "em",
    "font",
    "i",
    "nobr",
    "s",
    "small",
    "strike",
    "strong",
    "tt",
    "u",
}};

// http://www.w3.org/html/wg/drafts/html/master/single-page.html#void-elements
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
    _enable_debug(enable_debug),
    _cdata_start_ptr(nullptr),
    _comment_start_ptr(nullptr),
    _cr_start_ptr(nullptr),
    _tag_name_start_ptr(nullptr),
    _tag_self_closing(false),
    _in_body(false)
  {
  _tag_name.reserve(32);
}

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
HTMLLexer::_set_tag_name(const uint8_t *const tag_name_end_ptr) {
  _tag_name.clear();
  for (const uint8_t *p = _tag_name_start_ptr; p != tag_name_end_ptr; ++p)
    _tag_name.push_back(std::tolower(*p));
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
  _text.write(reinterpret_cast<const char *>(_cr_start_ptr), fpc - _cr_start_ptr);
}


void
HTMLLexer::_cdata_start(const uint8_t *const fpc) {
  _cdata_start_ptr = fpc;
}

void
HTMLLexer::_cdata_end(const uint8_t *const fpc) {
  (void)fpc;
  //std::cerr << "[_cdata_end] " << (fpc - _cdata_start_ptr) << std::endl;
  //std::cerr << "'";
  //std::cerr.write(reinterpret_cast<const char *>(_cdata_start_ptr), fpc - _cdata_start_ptr);
  //std::cerr << "'" << std::endl;
}


void
HTMLLexer::_comment_start(const uint8_t *const fpc) {
  _comment_start_ptr = fpc;
}

void
HTMLLexer::_comment_end(const uint8_t *const fpc) {
  (void)fpc;
  //std::cerr << "[_comment_end] " << (fpc - _comment_start_ptr) << std::endl;
  //std::cerr << "'";
  //std::cerr.write(reinterpret_cast<const char *>(_comment_start_ptr), fpc - _comment_start_ptr);
  //std::cerr << "'" << std::endl;
}


void
HTMLLexer::_doctype_start(const uint8_t *const fpc) {
  _comment_start_ptr = fpc;
}

void
HTMLLexer::_doctype_end(const uint8_t *const fpc) {
  (void)fpc;
  //std::cerr << "[_doctype_end] " << (fpc - _comment_start_ptr) << std::endl;
  //std::cerr << "'";
  //std::cerr.write(reinterpret_cast<const char *>(_comment_start_ptr), fpc - _comment_start_ptr);
  //std::cerr << "'" << std::endl;
}


void
HTMLLexer::_open_tag_start(const uint8_t *const fpc) {
  _tag_start_ptr = fpc;
  _tag_self_closing = false;
}

void
HTMLLexer::_open_tag_end(const uint8_t *) {
  //std::cerr << "[_open_tag] '" << _tag_name << "' " << _tag_self_closing << std::endl;
  // Add the tag to the stack of open tags iff it's not self closing and not void.
  if (!_tag_self_closing && VOID_TAGS.find(_tag_name) == VOID_TAGS.end())
    _open_tags.push_back(_tag_name);

  // Mark that we're inside the <body> of the HTML document.
  if (_tag_name == "body")
    _in_body = true;
}

void
HTMLLexer::_open_tag_name_start(const uint8_t *const fpc) {
  _tag_name_start_ptr = fpc;
}

void
HTMLLexer::_open_tag_name_end(const uint8_t *const fpc) {
  _set_tag_name(fpc);
}

void
HTMLLexer::_open_tag_self_closing(void) {
  _tag_self_closing = true;
}


void
HTMLLexer::_close_tag_start(const uint8_t *const fpc) {
  _tag_start_ptr = fpc;
  _tag_self_closing = false;
}

void
HTMLLexer::_close_tag_end(const uint8_t *) {
  // Mark that we're no longer inside the <body> of the HTML document.
  if (_tag_name == "body")
    _in_body = false;

  //std::cerr << "[_close_tag] '" << _tag_name << "' ";
  //if (_open_tags.empty())
    //std::cerr << "<empty stack>";
  //else {
    //std::cerr << "'" << _open_tags.back() << "'";
    //if (_open_tags.back() == _tag_name)
      //_open_tags.pop_back();
  //}
  //std::cerr << std::endl;
}

void
HTMLLexer::_close_tag_name_start(const uint8_t *const fpc) {
  _tag_name_start_ptr = fpc;
}

void
HTMLLexer::_close_tag_name_end(const uint8_t *const fpc) {
  _set_tag_name(fpc);
}


void
HTMLLexer::_seen_tag(const uint8_t *const ts, const uint8_t *const te) {
  if (_encoding_result == nullptr)
    return;
  (void)ts;
  (void)te;
  //std::cerr << "Tag '";
  //std::cerr.write(reinterpret_cast<const char *>(ts), te - ts);
  //std::cerr << "'" << std::endl;
}



void
HTMLLexer::_seen_text(const uint8_t *const ts, const uint8_t *const te) {
  if (_encoding_result == nullptr)
    return;
  (void)ts;
  (void)te;
  //std::cerr << "Text '";
  //std::cerr.write(reinterpret_cast<const char *>(ts), te - ts);
  //std::cerr << "'" << std::endl;
}


void
HTMLLexer::_reset(const EncodingResult *er) {
  _encoding_result = er;
  _open_tags.clear();
  _text.clear();
  _text.str("");

  _cdata_start_ptr = nullptr;
  _comment_start_ptr = nullptr;
  _cr_start_ptr = nullptr;
  _tag_start_ptr = nullptr;
  _tag_name_start_ptr = nullptr;
  _in_body = false;
}


void
HTMLLexer::_begin_document(void) {
  //std::cerr << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
}


void
HTMLLexer::_end_document(void) {
  //std::cerr << "Stack: " << _open_tags.size() << ":";
  //while (!_open_tags.empty()) {
    //std::cerr << " '" << _open_tags.back() << "'";
    //_open_tags.pop_back();
  //}
  //std::cerr << std::endl;
  //std::cerr << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
  //std::exit(0);
}


bool
HTMLLexer::run(const uint8_t *const input, const size_t nbytes) {
  _reset(nullptr);
  return _run(input, nbytes);
}


bool
HTMLLexer::run(const EncodingResult &er) {
  _reset(&er);
  return _run(er.utf8(), er.nbytes());
}

}  // namesapce formats
}  // namespace schwa
