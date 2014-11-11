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
SGMLishAttribute::SGMLishAttribute(const uint8_t *name, const uint8_t *value, SGMLishAttribute *next) :
    _name(name),
    _value(value),
    _next(next)
  { }

std::ostream &
SGMLishAttribute::pprint(std::ostream &out) const {
  return out << "{" << this << "}" << reinterpret_cast<const char *>(_name) << "='" << reinterpret_cast<const char *>(_value) << "'";
}


// ================================================================================================
// SGMLishNode
// ================================================================================================
SGMLishNode::SGMLishNode(SGMLishNodeType type, const PooledOffsetBuffer *name, const PooledOffsetBuffer *text, SGMLishAttribute *attribute) :
    _type(type),
    _name(name),
    _text(text),
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
  out << "<{" << this << '}';
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
    out << " '";
    out.write(reinterpret_cast<const char *>(_name->bytes()), _name->nitems_used());
    out << '\'';
    break;
  default:
    break;
  }

  for (SGMLishAttribute *attr = _attribute; attr != nullptr; attr = attr->next()) {
    out << ' ';
    attr->pprint(out);
  }

  if (_type == SGMLishNodeType::EMPTY_TAG)
    out << '/';
  out << ">\n";

  if (_type == SGMLishNodeType::TEXT) {
    for (unsigned int i = 0; i != indent + 1; ++i)
      out << "  ";
    out.write(reinterpret_cast<const char *>(_text->bytes()), _text->nitems_used());
    out << '\n';
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
    out.write(reinterpret_cast<const char *>(_name->bytes()), _name->nitems_used());
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
    _node_pool(nullptr),
    _state(er.utf8(), er.utf8() + er.nbytes()),
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
  // Convert the textual decimal number into a base-10 code point.
  unicode_t code_point = 0;
  for (const uint8_t *p = _text_buffer.start() + 2; *p != ';'; ++p) {
    code_point *= 10;
    code_point += *p - '0';
  }
  if (SCHWA_UNLIKELY(code_point > MAX_CODE_POINT))
    code_point = MAX_CODE_POINT;

  _text_buffer.write(code_point, static_cast<uint32_t>(fpc - _text_buffer.start()));
}


void
SGMLishLexer::_character_reference_hex(const uint8_t *const fpc) {
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

  _text_buffer.write(code_point, static_cast<uint32_t>(fpc - _text_buffer.start()));
}


void
SGMLishLexer::_character_reference_named(const uint8_t *const fpc) {
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
  unicode_t code_point;
  if (std::strcmp(name, "amp") == 0)
    code_point = '&';
  else if (std::strcmp(name, "apos") == 0)
    code_point = '\'';
  else if (std::strcmp(name, "gt") == 0)
    code_point = '>';
  else if (std::strcmp(name, "lt") == 0)
    code_point = '<';
  else if (std::strcmp(name, "quot") == 0)
    code_point = '"';
  else
    throw ValueException("Unexpected tag name");

  _text_buffer.write(code_point, static_cast<uint32_t>(fpc - _text_buffer.start()));
}


void
SGMLishLexer::_create_attr(void) {
  if (_text_buffer.empty())
    return;

  const uint8_t *bytes;
  size_t nbytes;

  // Clone attribute name into pool memory.
  bytes = _attr_name_buffer.buffer().bytes();
  nbytes = _attr_name_buffer.buffer().nitems_used();
  uint8_t *const name = _node_pool->alloc<uint8_t *>(nbytes + 1);  // FIXME
  std::memcpy(name, bytes, nbytes);
  name[nbytes] = 0;

  // Clone attribute value into pool memory.
  bytes = _text_buffer.buffer().bytes();
  nbytes = _text_buffer.buffer().nitems_used();
  uint8_t *const value = _node_pool->alloc<uint8_t *>(nbytes + 1);  // FIXME
  std::memcpy(value, bytes, nbytes);
  value[nbytes] = 0;

  // Create attribute object in pool memory.
  SGMLishAttribute *const attribute = _node_pool->alloc<SGMLishAttribute *>(sizeof(SGMLishAttribute));
  new (attribute) SGMLishAttribute(name, value, _attribute);
  _attribute = attribute;

  // Clear buffer memory.
  _attr_name_buffer.clear();
  _text_buffer.clear();
}


void
SGMLishLexer::_create_cdata_node(void) {
  // Clone tag contents into pool memory.
  const uint32_t initial_offset = _state.ts - _encoding_result.utf8();
  PooledOffsetBuffer *contents = _node_pool->alloc<PooledOffsetBuffer *>(sizeof(PooledOffsetBuffer));
  new (contents) PooledOffsetBuffer(1024, initial_offset, AlignedPoolAllocator<uint8_t>(*_node_pool));
  uint8_t utf8[4];
  for (const uint8_t *start = _state.ts; start != _state.te; ) {
    const size_t nbytes = read_utf8(&start, _state.te, utf8);
    for (size_t i = 0; i != nbytes; ++i)
      contents->write(utf8[i], (i == 0) ? nbytes : 0);
  }

  // Create node object in pool memory.
  _node = _node_pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::CDATA, nullptr, contents, nullptr);
}


void
SGMLishLexer::_create_comment_node(void) {
  // Clone tag contents into pool memory.
  const uint32_t initial_offset = _state.ts - _encoding_result.utf8();
  PooledOffsetBuffer *contents = _node_pool->alloc<PooledOffsetBuffer *>(sizeof(PooledOffsetBuffer));
  new (contents) PooledOffsetBuffer(1024, initial_offset, AlignedPoolAllocator<uint8_t>(*_node_pool));
  uint8_t utf8[4];
  for (const uint8_t *start = _state.ts; start != _state.te; ) {
    const size_t nbytes = read_utf8(&start, _state.te, utf8);
    for (size_t i = 0; i != nbytes; ++i)
      contents->write(utf8[i], (i == 0) ? nbytes : 0);
  }

  // Create node object in pool memory.
  _node = _node_pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::COMMENT, nullptr, contents, nullptr);
}


SGMLishLexer::PooledOffsetBuffer *
SGMLishLexer::_create_poold_tag_name(void) {
  const uint8_t *const start = _tag_name_buffer.buffer().bytes();
  const uint8_t *const end = start + _tag_name_buffer.buffer().nitems_used();

  // Convert the tag name to lowercase.
  const uint32_t initial_offset = _tag_name_buffer.start() - _encoding_result.utf8();
  PooledOffsetBuffer *name = _node_pool->alloc<PooledOffsetBuffer *>(sizeof(PooledOffsetBuffer));
  new (name) PooledOffsetBuffer(32, initial_offset, AlignedPoolAllocator<uint8_t>(*_node_pool));
  uint8_t utf8[4];
  unicode_t code_points[3];
  for (const uint8_t *bytes = start; bytes != end; ) {
    const unicode_t code_point = read_utf8(&bytes, end);
    const size_t nbytes_utf8 = write_utf8(code_point, utf8);
    const size_t ncode_points = unicode::to_lower(code_point, code_points);
    for (size_t i = 0; i != ncode_points; ++i) {
      const size_t nbytes = write_utf8(code_points[i], utf8);
      for (size_t j = 0; j != nbytes; ++j)
        name->write(utf8[j], (i == 0 && j == 0) ? nbytes_utf8 : 0);
    }
  }

  return name;
}


void
SGMLishLexer::_create_empty_tag_node(void) {
  // Clone tag name into pool memory.
  PooledOffsetBuffer *const name = _create_poold_tag_name();

  // Create node object in pool memory.
  _node = _node_pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::EMPTY_TAG, name, nullptr, _attribute);

  // Clear buffer memory.
  _tag_name_buffer.clear();
  _attribute = nullptr;
}


void
SGMLishLexer::_create_end_tag_node(void) {
  // Clone tag name into pool memory.
  PooledOffsetBuffer *const name = _create_poold_tag_name();

  // Create node object in pool memory.
  _node = _node_pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::END_TAG, name, nullptr, nullptr);

  // Clear buffer memory.
  _tag_name_buffer.clear();
  _attribute = nullptr;
}


void
SGMLishLexer::_create_start_tag_node(void) {
  // Clone tag name into pool memory.
  PooledOffsetBuffer *const name = _create_poold_tag_name();

  // Create node object in pool memory.
  _node = _node_pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::START_TAG, name, nullptr, _attribute);

  // Clear buffer memory.
  _tag_name_buffer.clear();
  _attribute = nullptr;
}


void
SGMLishLexer::_create_text_node(void) {
  // Clone text content into pool memory.
  PooledOffsetBuffer *text = _node_pool->alloc<PooledOffsetBuffer *>(sizeof(PooledOffsetBuffer));
  new (text) PooledOffsetBuffer(_text_buffer.buffer(), AlignedPoolAllocator<uint8_t>(*_node_pool));

  // Create node object in pool memory.
  _node = _node_pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::TEXT, nullptr, text, nullptr);

  // Clear buffer memory.
  _text_buffer.clear();
  _attribute = nullptr;
}


void
SGMLishLexer::_create_xml_decl_node(void) {
  // Create node object in pool memory.
  _node = _node_pool->alloc<SGMLishNode *>(sizeof(SGMLishNode));
  new (_node) SGMLishNode(SGMLishNodeType::XML_DECL, nullptr, nullptr, _attribute);

  // Clear buffer memory.
  _text_buffer.clear();
  _attribute = nullptr;
}


// ================================================================================================
// SGMLishParser::ParseError
// ================================================================================================
SGMLishParser::ParseError::ParseError(const std::string &msg) : formats::ParseError(msg) { }

SGMLishParser::ParseError::ParseError(const ParseError &other) : formats::ParseError(other) { }

SGMLishParser::ParseError::~ParseError(void) throw() { }


// ================================================================================================
// SGMLishParser
// ================================================================================================
SGMLishParser::SGMLishParser(const EncodingResult &er) : _lexer(er), _root(nullptr) { }


SGMLishNode *
SGMLishParser::parse(Pool &node_pool) {
  std::stack<SGMLishNode *> stack;
  _root = nullptr;

  const PooledOffsetBuffer *name0 = nullptr, *name1 = nullptr;

  while (true) {
    // Lex the next node from the input stream.
    SGMLishNode *const node = _lexer.lex(node_pool);
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
        throw ParseError("Encountered an end tag but the parsing stack is empty");
      // Ensure the tag we're closing is the same tag as what's top-most on the stack of open tags.
      name0 = stack.top()->name();
      name1 = node->name();
      if (name0->nitems_used() != name1->nitems_used() || std::memcmp(name0->bytes(), name1->bytes(), name0->nitems_used()) != 0)
        throw ParseError("Encountered an end tag whose name does not match the top of the stack");
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

  // If the stack isn't empty, something went wrong.
  if (!stack.empty()) {
    if (_lexer.at_eof())
      throw ParseError("EOF hit but the parse stack is not empty. Probably malformed SGML.");
    else
      throw ParseError("SGMLishLexer failed to lex. Probably invalid SGML (unescaped &).");
  }

  return _root;
}

}  // namesapce formats
}  // namespace schwa
