/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/formats/sgml.h>

#include <cctype>
#include <cstring>
#include <iostream>
#include <stack>
#include <stdexcept>

#include <schwa/unicode.h>


namespace schwa {
namespace formats {

// ================================================================================================
// SGMLishAttribute
// ================================================================================================
std::ostream &
SGMLishAttribute::pprint(std::ostream &out) const {
  return out << "{" << this << "}" << reinterpret_cast<const char *>(_name) << "='" << reinterpret_cast<const char *>(_value) << "'";
}


// ================================================================================================
// SGMLishNode
// ================================================================================================
SGMLishNode::SGMLishNode(SGMLishNodeType type, const uint8_t *data, SGMLishAttribute *attribute) :
    _type(type),
    _data(data),
    _child(nullptr),
    _sibling(nullptr),
    _attribute(attribute)
  { }


void
SGMLishNode::add_child(SGMLishNode &node) {
  if (_child == nullptr)
    _child = &node;
  else {
    SGMLishNode *n = _child;
    while (n->sibling() != nullptr)
      n = n->sibling();
    n->_set_sibling(node);
  }
}


std::ostream &
SGMLishNode::_pprint(std::ostream &out, const unsigned int indent) const {
  for (unsigned int i = 0; i != indent; ++i)
    out << "  ";
  out << "<{" << this << "}";
  switch (_type) {
  case SGMLishNodeType::START_TAG: out << "START_TAG"; break;
  case SGMLishNodeType::EMPTY_TAG: out << "EMPTY_TAG"; break;
  case SGMLishNodeType::END_TAG: out << "END_TAG"; break;
  case SGMLishNodeType::COMMENT: out << "COMMENT"; break;
  case SGMLishNodeType::CDATA: out << "CDATA"; break;
  case SGMLishNodeType::XML_DECL: out << "XML_DECL"; break;
  case SGMLishNodeType::TEXT: out << "TEXT"; break;
  }

  switch (_type) {
  case SGMLishNodeType::START_TAG:
  case SGMLishNodeType::EMPTY_TAG:
  case SGMLishNodeType::END_TAG:
    out << " '" << reinterpret_cast<const char *>(_data) << "'";
    break;
  default:
    break;
  }

  for (SGMLishAttribute *attr = _attribute; attr != nullptr; attr = attr->next()) {
    out << " ";
    attr->pprint(out);
  }

  if (_type == SGMLishNodeType::EMPTY_TAG)
    out << "/";
  out << ">\n";

  if (_type == SGMLishNodeType::TEXT) {
    for (unsigned int i = 0; i != indent + 1; ++i)
      out << "  ";
    out << reinterpret_cast<const char *>(_data) << "\n";
  }
  else {
    for (SGMLishNode *child = _child; child != nullptr; child = child->sibling())
      child->_pprint(out, indent + 1);
  }

  for (unsigned int i = 0; i != indent; ++i)
    out << "  ";
  out << "</";
  switch (_type) {
  case SGMLishNodeType::START_TAG:
  case SGMLishNodeType::EMPTY_TAG:
  case SGMLishNodeType::END_TAG:
    out << reinterpret_cast<const char *>(_data);
    break;
  case SGMLishNodeType::TEXT:
    out << "TEXT";
  default:
    break;
  }
  out << ">\n";

  return out;
}


std::ostream &
SGMLishNode::pprint(std::ostream &out) const {
  return _pprint(out, 0);
}


// ================================================================================================
// SGMLishLexer
// ================================================================================================
SGMLishLexer::SGMLishLexer(const EncodingResult &er) :
    _encoding_result(er),
    _pool(nullptr),
    _state(er.utf8(), er.nbytes()),
    _attr_name_buffer(64),
    _tag_name_buffer(64),
    _text_buffer(DEFAULT_BUFFER_GROW_SIZE),
    _attribute(nullptr),
    _node(nullptr)
  {
  _init();
}


void
SGMLishLexer::_character_reference_decimal(const uint8_t *const fpc) {
  std::cout << "[SGMLishLexer::_character_reference_decimal]" << std::endl;
  (void)fpc;

  // Convert the textual decimal number into a base-10 code point.
  unicode_t code_point = 0;
  for (const uint8_t *p = _text_buffer.start() + 2; *p != ';'; ++p) {
    code_point *= 10;
    code_point += *p - '0';
  }
  if (SCHWA_UNLIKELY(code_point > MAX_CODE_POINT))
    code_point = MAX_CODE_POINT;

  // Convert the code point into a UTF-8 sequence, and append it to the text stream.
  uint8_t utf8[4];
  const size_t nbytes = write_utf8(code_point, utf8);
  _text_buffer.write(utf8, nbytes);
}


void
SGMLishLexer::_character_reference_hex(const uint8_t *const fpc) {
  std::cout << "[SGMLishLexer::_character_reference_hex]" << std::endl;
  (void)fpc;

  // Convert the textual hex  number into a base-10 code point.
  unicode_t code_point = 0;
  for (const uint8_t *p = _text_buffer.start() + 3; *p != ';'; ++p) {
    code_point *= 16;
    if (*p >= '0' && *p <= '9')
      code_point += *p - '0';
    else if (*p >= 'a' && *p <= 'f')
      code_point += (*p - 'a') + 10;
    else
      code_point += (*p - 'A') + 10;
  }
  if (SCHWA_UNLIKELY(code_point > MAX_CODE_POINT))
    code_point = MAX_CODE_POINT;

  // Convert the code point into a UTF-8 sequence, and append it to the text stream.
  uint8_t utf8[4];
  const size_t nbytes = write_utf8(code_point, utf8);
  _text_buffer.write(utf8, nbytes);
}


void
SGMLishLexer::_character_reference_named(const uint8_t *const fpc) {
  std::cout << "[SGMLishLexer::_character_reference_named]" << std::endl;
  (void)fpc;

  // Convert the named reference to lowercase.
  const char *const start = reinterpret_cast<const char *>(_text_buffer.start()) + 1;
  size_t i;
  char name[5];
  for (i = 0; i != 4; ++i) {
    if (start[i] == ';')
      break;
    name[i] = std::tolower(start[i]);
  }
  name[i] = '\0';

  // Append the appropriate raw character to the text stream.
  if (std::strcmp(name, "amp") == 0)
    _text_buffer.write(static_cast<uint8_t>('&'));
  else if (std::strcmp(name, "apos") == 0)
    _text_buffer.write(static_cast<uint8_t>('\''));
  else if (std::strcmp(name, "gt") == 0)
    _text_buffer.write(static_cast<uint8_t>('>'));
  else if (std::strcmp(name, "lt") == 0)
    _text_buffer.write(static_cast<uint8_t>('<'));
  else if (std::strcmp(name, "quot") == 0)
    _text_buffer.write(static_cast<uint8_t>('"'));
  else
    throw std::runtime_error("Unexpected tag name");
}


void
SGMLishLexer::_create_attr(void) {
  if (_text_buffer.empty())
    return;
  std::cout << "[SGMLishLexer::_create_attr]" << std::endl;

  // Clone attribute name into pool memory.
  uint8_t *const name = _pool->alloc<uint8_t *>(_attr_name_buffer.buffer().used() + 1);
  std::memcpy(name, _attr_name_buffer.buffer().buffer(), _attr_name_buffer.buffer().used());
  name[_attr_name_buffer.buffer().used()] = 0;

  // Clone attribute value into pool memory.
  uint8_t *const value = _pool->alloc<uint8_t *>(_text_buffer.buffer().used() + 1);
  std::memcpy(value, _text_buffer.buffer().buffer(), _text_buffer.buffer().used());
  value[_text_buffer.buffer().used()] = 0;

  // Create attribute object in pool memory.
  SGMLishAttribute *const attribute = _pool->alloc<SGMLishAttribute *>(sizeof(SGMLishAttribute));
  new (attribute) SGMLishAttribute(name, value, _attribute);
  _attribute = attribute;

  // Clear buffer memory.
  _attr_name_buffer.clear();
  _text_buffer.clear();
}


void
SGMLishLexer::_create_cdata_node(void) {
  std::cout << "[SGMLishLexer::_create_cdata_node]" << std::endl;

  // Clone tag contents into pool memory.
  const size_t nbytes = _state.te - _state.ts;
  uint8_t *const data = _pool->alloc<uint8_t *>(nbytes + 1);
  std::memcpy(data, _state.ts, nbytes);
  data[nbytes] = 0;

  // Create node object in pool memory.
  _node = _pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::CDATA, data);
}


void
SGMLishLexer::_create_comment_node(void) {
  std::cout << "[SGMLishLexer::_create_comment_node]" << std::endl;

  // Clone tag contents into pool memory.
  const size_t nbytes = _state.te - _state.ts;
  uint8_t *const data = _pool->alloc<uint8_t *>(nbytes + 1);
  std::memcpy(data, _state.ts, nbytes);
  data[nbytes] = 0;

  // Create node object in pool memory.
  _node = _pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::COMMENT, data);
}


uint8_t *
SGMLishLexer::_create_poold_tag_name(void) {
  // Convert the tag name to lowercase.
  const UnicodeString us = UnicodeString::from_utf8(_tag_name_buffer.buffer().buffer(), _tag_name_buffer.buffer().used());
  const std::string lower = us.to_lower().to_utf8();

  // Clone tag name into pool memory.
  uint8_t *const name = _pool->alloc<uint8_t *>(lower.size() + 1);
  std::memcpy(name, lower.c_str(), lower.size() + 1);
  return name;
}


void
SGMLishLexer::_create_empty_tag_node(void) {
  std::cout << "[SGMLishLexer::_create_empty_tag_node]" << std::endl;

  // Clone tag name into pool memory.
  uint8_t *const name = _create_poold_tag_name();

  // Create node object in pool memory.
  _node = _pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::EMPTY_TAG, name, _attribute);

  // Clear buffer memory.
  _tag_name_buffer.clear();
  _attribute = nullptr;
}


void
SGMLishLexer::_create_end_tag_node(void) {
  std::cout << "[SGMLishLexer::_create_end_tag_node]" << std::endl;

  // Clone tag name into pool memory.
  uint8_t *const name = _create_poold_tag_name();

  // Create node object in pool memory.
  _node = _pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::END_TAG, name);

  // Clear buffer memory.
  _tag_name_buffer.clear();
  _attribute = nullptr;
}


void
SGMLishLexer::_create_start_tag_node(void) {
  std::cout << "[SGMLishLexer::_create_start_tag_node]" << std::endl;

  // Clone tag name into pool memory.
  uint8_t *const name = _create_poold_tag_name();

  // Create node object in pool memory.
  _node = _pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::START_TAG, name, _attribute);

  // Clear buffer memory.
  _tag_name_buffer.clear();
  _attribute = nullptr;
}


void
SGMLishLexer::_create_text_node(void) {
  std::cout << "[SGMLishLexer::_create_text_node]" << std::endl;

  // Clone text content into pool memory.
  uint8_t *const text = _pool->alloc<uint8_t *>(_text_buffer.buffer().used() + 1);
  std::memcpy(text, _text_buffer.buffer().buffer(), _text_buffer.buffer().used());
  text[_text_buffer.buffer().used()] = 0;

  // Create node object in pool memory.
  _node = _pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::TEXT, text);

  // Clear buffer memory.
  _text_buffer.clear();
  _attribute = nullptr;
}


void
SGMLishLexer::_create_xml_decl_node(void) {
  std::cout << "[SGMLishLexer::_create_xml_decl_node]" << std::endl;

  // Create node object in pool memory.
  _node = _pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::XML_DECL, nullptr, _attribute);

  // Clear buffer memory.
  _text_buffer.clear();
  _attribute = nullptr;
}


// ================================================================================================
// SGMLishParser
// ================================================================================================
SGMLishParser::SGMLishParser(const EncodingResult &er) : _lexer(er), _root(nullptr) { }


SGMLishNode *
SGMLishParser::parse(Pool &pool) {
  std::stack<SGMLishNode *> stack;
  _root = nullptr;

  const char *stack_name = nullptr;
  const char *node_name = nullptr;

  while (true) {
    // Lex the next node from the input stream.
    SGMLishNode *const node = _lexer.lex(pool);
    if (node == nullptr)
      break;

    // Try and form the tree structure, keeping a stack of the currently open tags.
    switch (node->type()) {
    case SGMLishNodeType::START_TAG:
      // Push the node to the stack of opened tags, and promote it to root if necessary.
      if (!stack.empty())
        stack.top()->add_child(*node);
      stack.push(node);
      if (_root == nullptr)
        _root = node;
      break;
    case SGMLishNodeType::END_TAG:
      // Can't close a tag if there are no currently opened tags.
      if (stack.empty())
        throw std::runtime_error("ParseError: encountered an end tag but the stack is empty");
      // Ensure the tag we're closing is the same tag as what's top-most on the stack of open tags.
      stack_name = reinterpret_cast<const char *>(stack.top()->name());
      node_name = reinterpret_cast<const char *>(node->name());
      if (std::strcmp(stack_name, node_name) != 0)
        throw std::runtime_error("ParseError: encountered an end tag whose name does not match the top of the stack");
      // Close the top-most opened tag.
      stack.pop();
      break;
    case SGMLishNodeType::EMPTY_TAG:
      // Make the node a child if we're not the top-level element, and promote it to root if necessary.
      if (!stack.empty())
        stack.top()->add_child(*node);
      if (_root == nullptr)
        _root = node;
      break;
    case SGMLishNodeType::TEXT:
    case SGMLishNodeType::COMMENT:
    case SGMLishNodeType::CDATA:
      // Only keep these tags if we're inside a top-level document.
      if (!stack.empty())
        stack.top()->add_child(*node);
      break;
    case SGMLishNodeType::XML_DECL:
      // Discard this. I doubt we ever want it.
      break;
    }

    // End of the current document if we've just closed all open tags.
    if (node->type() == SGMLishNodeType::END_TAG && stack.empty())
      break;
  }

  if (!stack.empty())
    throw std::runtime_error("ParseError: EOF hit but the stack is not empty");
  return _root;
}

}  // namesapce formats
}  // namespace schwa
