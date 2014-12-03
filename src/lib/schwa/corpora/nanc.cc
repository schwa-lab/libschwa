/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/corpora/nanc.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <schwa/encoding.h>
#include <schwa/exception.h>
#include <schwa/formats/sgml.h>
#include <schwa/io/streams.h>
#include <schwa/pool.h>
#include <schwa/tokenizer/tokenizer.h>
#include <schwa/unicode.h>
#include <schwa/utils/buffer.h>

#include <schwa/third-party/re2/re2.h>


namespace cs = ::schwa::canonical_schema;
namespace fm = ::schwa::formats;
namespace tk = ::schwa::tokenizer;
using ::schwa::third_party::re2::RE2;

namespace schwa {
namespace corpora {

//!< Regex for matching surrounding whitespace. Useful for stripping.
static const RE2 RE_SURROUNDING_WHITESPACE("^\\s+|\\s+$");

//!< Regex for parsing a date out of a docid.
static const RE2 RE_DATE_IN_DOCID("^(?:latwp|nyt|reute)(9[4-7])(0[1-9]|1[0-2])(0?[1-9]|[12][0-9]|3[01])\\.\\d{4}$");
static const RE2 RE_MSGDATE("^\\s*19(9[4-7])(0[1-9]|1[0-2])(0?[1-9]|[12][0-9]|3[01])\\s*$");
static const RE2 RE_SEQNO("^\\s*(\\d{4})\\s*$");

//!< Regex for attempting to identify ASCII table rows.
static const RE2 RE_ROW("^\\s*\\|.*$");


// ================================================================================================
// NANCImporter::Impl
// ================================================================================================
class NANCImporter::Impl {
private:
  EncodingResult _er;
  cs::Doc *_doc;
  fm::SGMLishParser *_sgml_parser;
  Pool _node_pool;
  tk::Tokenizer _tokenizer;

  void _handle_docid(const fm::SGMLishNode &node);
  void _handle_headline(const fm::SGMLishNode &node);
  void _handle_msgdate(const fm::SGMLishNode &node);
  void _handle_p(const fm::SGMLishNode &node);
  void _handle_seqno(const fm::SGMLishNode &node);

  void _process_tree(const fm::SGMLishNode &node);

  cs::Doc *_read_doc(void);

public:
  explicit Impl(const std::string &path);
  ~Impl(void);

  inline cs::Doc *import(void) { return _read_doc(); }
};


NANCImporter::Impl::Impl(const std::string &path) :
    _doc(nullptr),
    _sgml_parser(nullptr),
    _node_pool(4 * 1024 * 1024)
  {
  // Read in all of the raw data.
  io::InputStream in(path);
  Buffer<> encoded_bytes(1 * 1024 * 1024);
  encoded_bytes.consume(in);

  // Decode the read in data.
  to_utf8(Encoding::LATIN1, encoded_bytes, _er);

  // Construct a SGML parser around the decoded data.
  _sgml_parser = new fm::SGMLishParser(_er);

  // Add in a number of invalid SGML escape sequences that exist in the data.
  _sgml_parser->add_escape_sequence("d0", ' ');
  _sgml_parser->add_escape_sequence("d1", ' ');
  _sgml_parser->add_escape_sequence("d2", ' ');
  _sgml_parser->add_escape_sequence("d3", ' ');
  _sgml_parser->add_escape_sequence("d4", ' ');
  _sgml_parser->add_escape_sequence("fs", ' ');
  _sgml_parser->add_escape_sequence("ht", ' ');
  _sgml_parser->add_escape_sequence("lr", ' ');
  _sgml_parser->add_escape_sequence("md", 0x2014);  // U+2014 em-dash
  _sgml_parser->add_escape_sequence("qc", ' ');
  _sgml_parser->add_escape_sequence("qc", ' ');
  _sgml_parser->add_escape_sequence("ql", ' ');
  _sgml_parser->add_escape_sequence("qr", ' ');
  _sgml_parser->add_escape_sequence("tf", ' ');
  _sgml_parser->add_escape_sequence("tl", ' ');
  _sgml_parser->add_escape_sequence("ur", ' ');
  for (uint32_t byte = 0; byte != 256; ++byte) {
    char name[5];
    // Strange unknowns.
    std::sprintf(name, "cx%02x", byte);
    _sgml_parser->add_escape_sequence(name, ' ');

    // Malformed byte value escape sequences (missing # from &#xbd; notation).
    // Convert the Latin1 encoded bytes to UTF-8, and then read off the single Unicode code point.
    EncodingResult utf8;
    const uint8_t bytes[1] = {static_cast<uint8_t>(byte)};
    latin1_to_utf8(bytes, 1, utf8);

    const uint8_t *start = utf8.bytes();
    const uint8_t *const end = start + utf8.nitems();
    assert(start != end);
    const unicode_t code_point = read_utf8(&start, end);
    assert(start == end);

    std::sprintf(name, "x%02x", byte);
    _sgml_parser->add_escape_sequence(name, code_point);
  }

  // Tell the SGML parser that <p> tags are leaf tags (they cannot have children).
  _sgml_parser->add_leaf_tag_name("p");
}

NANCImporter::Impl::~Impl(void) {
  delete _doc;
  delete _sgml_parser;
}


void
NANCImporter::Impl::_handle_docid(const fm::SGMLishNode &node) {
  // Strip surrounding whitespace.
  _doc->doc_id = std::string(reinterpret_cast<const char *>(node.text()->bytes()), node.text()->nitems_used());
  RE2::GlobalReplace(&_doc->doc_id, RE_SURROUNDING_WHITESPACE, "");

  // If the document is from {latwp,nyt,reuff,reute}, extract the data from the docid.
  std::string date_day, date_month, date_year;
  if (RE2::FullMatch(_doc->doc_id, RE_DATE_IN_DOCID, &date_year, &date_month, &date_day)) {
    std::ostringstream ss;
    ss << "19" << date_year << "-" << date_month << "-" << date_day;
    _doc->date = ss.str();
  }
}


void
NANCImporter::Impl::_handle_headline(const fm::SGMLishNode &node) {
  // Strip surrounding whitespace.
  _doc->headline = std::string(reinterpret_cast<const char *>(node.text()->bytes()), node.text()->nitems_used());
  RE2::GlobalReplace(&_doc->headline, RE_SURROUNDING_WHITESPACE, "");
}


void
NANCImporter::Impl::_handle_msgdate(const fm::SGMLishNode &node) {
  // If the document is from WSJ, set the date and construct a docid.
  third_party::re2::StringPiece text(reinterpret_cast<const char *>(node.text()->bytes()), node.text()->nitems_used());

  std::string date_day, date_month, date_year;
  if (RE2::FullMatch(text, RE_MSGDATE, &date_year, &date_month, &date_day)) {
    std::ostringstream ss;
    ss << "19" << date_year << "-" << date_month << "-" << date_day;
    _doc->date = ss.str();

    // Construct a docid in the same format was {latwp,nyt,reuff,reute}.
    ss.str("");
    ss.clear();
    ss << "wsj" << date_year << date_month << date_day << "." << _doc->doc_id;
    _doc->doc_id = ss.str();
  }
}


void
NANCImporter::Impl::_handle_seqno(const fm::SGMLishNode &node) {
  third_party::re2::StringPiece text(reinterpret_cast<const char *>(node.text()->bytes()), node.text()->nitems_used());

  std::string seqno;
  if (RE2::FullMatch(text, RE_SEQNO, &seqno))
    _doc->doc_id = seqno;
}


void
NANCImporter::Impl::_handle_p(const fm::SGMLishNode &node) {
  // Find the newline boundaries in the paragraph.
  std::vector<std::pair<BaseOffsetBuffer::iterator, BaseOffsetBuffer::iterator>> line_bounds;
  BaseOffsetBuffer::iterator first = node.text()->begin();
  BaseOffsetBuffer::iterator last = node.text()->end();
  for (BaseOffsetBuffer::iterator current = first; current != last; ++current) {
    if (*current == '\n' && current != first) {
      line_bounds.push_back(std::make_pair(first, current));
      first = current;
    }
  }
  if (first != last)
    line_bounds.push_back(std::make_pair(first, last));

  // Run an initial pass over the lines to identify lines that look like table rows.
  std::vector<bool> is_table_row(line_bounds.size(), false);
  for (size_t i = 0; i != line_bounds.size(); ++i) {
    third_party::re2::StringPiece sp(reinterpret_cast<const char *>(line_bounds[i].first.get_bytes()), line_bounds[i].second - line_bounds[i].first);
    if (RE2::FullMatch(sp, RE_ROW))
      is_table_row[i] = true;
  }

  // Copy lines that we haven't classified as being a table row into the buffer to send to the tokenizer. what we've classified as a table row.
  tk::OffsetInputStream<> ois(node.text()->nitems_used());
  for (size_t i = 0; i != line_bounds.size(); ++i)
    if (!is_table_row[i])
      ois.write(line_bounds[i].first, line_bounds[i].second);

  // Tokenize the paragraph.
  const size_t nsentences_before = _doc->sentences.size();
  _tokenizer.tokenize(ois, *_doc);
  const size_t nsentences_after = _doc->sentences.size();

  // Create the Paragraph and Block objects, and add them to the document.
  if (nsentences_before != nsentences_after) {
    cs::Paragraph paragraph;
    paragraph.span.start = reinterpret_cast<cs::Sentence *>(nsentences_before + 1);
    paragraph.span.stop = reinterpret_cast<cs::Sentence *>(nsentences_after + 1);
    _doc->paragraphs.push_back(paragraph);

    cs::Block block;
    block.paragraph = reinterpret_cast<cs::Paragraph *>(_doc->paragraphs.size() - 1 + 1);
    _doc->blocks.push_back(block);
  }
}


void
NANCImporter::Impl::_process_tree(const fm::SGMLishNode &node) {
  const fm::SGMLishNode *child;

  // If we're at a <DOCID> node, extract the document number.
  if (node.is_start_tag() && node.has_name("docid")) {
    child = node.child();
    if (child != nullptr && child->is_text())
      _handle_docid(*child);
  }

  // If we're at a <SEQNO> node, extract the sequence number (WSJ).
  if (node.is_start_tag() && node.has_name("seqno")) {
    child = node.child();
    if (child != nullptr && child->is_text())
      _handle_seqno(*child);
  }

  // If we're at a <MSGDATE> node, extract the document date (WSJ).
  if (node.is_start_tag() && node.has_name("msgdate")) {
    child = node.child();
    if (child != nullptr && child->is_text())
      _handle_msgdate(*child);
  }

  // If we're at a <HEADLINE> or <HL> node, extract the document headline text.
  if (node.is_start_tag() && (node.has_name("headline") || node.has_name("hl"))) {
    child = node.child();
    if (child != nullptr && child->is_text())
      _handle_headline(*child);
  }

  // If we're at a <P> node, extract and tokenize the contents.
  if (node.is_start_tag() && node.has_name("p")) {
    child = node.child();
    if (child != nullptr && child->is_text())
      _handle_p(*child);
  }

  // Recursive tree processing.
  if (node.child() != nullptr)
    _process_tree(*node.child());
  if (node.sibling() != nullptr)
    _process_tree(*node.sibling());
}


cs::Doc *
NANCImporter::Impl::_read_doc(void) {
  delete _doc;
  _node_pool.drain();

  // Read in and parse the next SGML document.
  fm::SGMLishNode *const root = _sgml_parser->parse(_node_pool);
  if (root == nullptr) {
    if (!_sgml_parser->eof())
      throw schwa::Exception("Failed to parse");
    _doc = nullptr;
    return _doc;
  }

  // Create the document object.
  _doc = new cs::Doc();
  _doc->encoding = encoding_name(_er.encoding());

  // Process the SGML tree, extracting the necesary information.
  _process_tree(*root);

  // Unswizzle the docrep pointers and return the document.
  _doc->unswizzle_pointers();
  return _doc;
}


// ================================================================================================
// NANCImporter
// ================================================================================================
NANCImporter::NANCImporter(const std::string &path) : _impl(new Impl(path)) { }

NANCImporter::~NANCImporter(void) {
  delete _impl;
}

cs::Doc *
NANCImporter::import(void) {
  return _impl->import();
}

}  // namesapce corpora
}  // namespace schwa
