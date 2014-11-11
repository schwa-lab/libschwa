/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/corpora/tipster.h>

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
#include <schwa/new-tokenizer/tokenizer.h>
#include <schwa/pool.h>
#include <schwa/unicode.h>
#include <schwa/utils/buffer.h>

#include <schwa/third-party/re2/re2.h>


namespace cs = ::schwa::canonical_schema;
namespace fm = ::schwa::formats;
namespace tk = ::schwa::new_tokenizer;
using ::schwa::third_party::re2::RE2;

namespace schwa {
namespace corpora {

//!< Regex for matching surrounding whitespace. Useful for stripping.
static const RE2 RE_SURROUNDING_WHITESPACE("^\\s+|\\s+$");
//!< Regex for parsing a short US date. Useful since strptime is POSIX only.
static const RE2 RE_US_SHORT_DATE("^(0?[1-9]|1[0-2])/(0?[1-9]|[12][0-9]|3[01])/([0-9]{2})$");

//!< Regex for attempting to identify ASCII table rows.
static const std::string RE_ROW_TEXT_CHAR = "[A-Za-z0-9" + RE2::QuoteMeta("!\"#$%&'()*+,-./:;=?@[\\]^_`{|}~") + "]";
static const std::string RE_ROW_TEXT = RE_ROW_TEXT_CHAR + "([ ]?" + RE_ROW_TEXT_CHAR + ")*";
static const RE2 RE_ROW("^\\s*" + RE_ROW_TEXT + "((\\s*\\.{2,}\\s*|\\s{2,})[-+(]?\\s?" + RE_ROW_TEXT + "\\s?[)]?)+$");


// ================================================================================================
// TipsterImporter::Impl
// ================================================================================================
class TipsterImporter::Impl {
private:
  EncodingResult _er;
  cs::Doc *_doc;
  fm::SGMLishParser *_sgml_parser;
  Pool _node_pool;
  TipsterTextLexer _text_lexer;
  tk::Tokenizer _tokenizer;

  void _handle_dateline(const fm::SGMLishNode &node);
  void _handle_date(const fm::SGMLishNode &node);
  void _handle_docno(const fm::SGMLishNode &node);
  void _handle_hl(const fm::SGMLishNode &node);
  void _handle_text(const fm::SGMLishNode &node);

  void _process_tree(const fm::SGMLishNode &node);
  void _unswizzle_pointers(void);

  cs::Doc *_read_doc(void);

public:
  explicit Impl(const std::string &path);
  ~Impl(void);

  inline cs::Doc *import(void) { return _read_doc(); }
};


TipsterImporter::Impl::Impl(const std::string &path) :
    _doc(nullptr),
    _sgml_parser(nullptr),
    _node_pool(4 * 1024 * 1024)
  {
  // Read in all of the raw data.
  io::InputStream in(path);
  Buffer<> encoded_bytes(1 * 1024 * 1024);
  encoded_bytes.consume(in);

  // Decode the read in data.
  to_utf8(Encoding::ASCII, encoded_bytes, _er);

  // Construct a SGML parser around the decoded data.
  _sgml_parser = new fm::SGMLishParser(_er);
}

TipsterImporter::Impl::~Impl(void) {
  delete _doc;
  delete _sgml_parser;
}


void
TipsterImporter::Impl::_handle_dateline(const fm::SGMLishNode &node) {
  // Strip surrounding whitespace.
  _doc->dateline = std::string(reinterpret_cast<const char *>(node.text()->bytes()), node.text()->nitems_used());
  RE2::GlobalReplace(&_doc->dateline, RE_SURROUNDING_WHITESPACE, "");
}


void
TipsterImporter::Impl::_handle_date(const fm::SGMLishNode &node) {
  // Strip surrounding whitespace.
  std::string dd = std::string(reinterpret_cast<const char *>(node.text()->bytes()), node.text()->nitems_used());
  RE2::GlobalReplace(&dd, RE_SURROUNDING_WHITESPACE, "");

  // Parse and convert the date into ISO-8601 format.
  std::string date_month, date_day, date_year;
  if (RE2::FullMatch(dd, RE_US_SHORT_DATE, &date_month, &date_day, &date_year)) {
    std::ostringstream ss;
    ss << "19" << date_year << "-";
    if (date_month.size() == 1)
      ss << "0";
    ss << date_month << "-";
    if (date_day.size() == 1)
      ss << "0";
    ss << date_day;
    _doc->story_date = ss.str();
  }
}


void
TipsterImporter::Impl::_handle_docno(const fm::SGMLishNode &node) {
  // Strip surrounding whitespace.
  _doc->doc_id = std::string(reinterpret_cast<const char *>(node.text()->bytes()), node.text()->nitems_used());
  RE2::GlobalReplace(&_doc->doc_id, RE_SURROUNDING_WHITESPACE, "");
}


void
TipsterImporter::Impl::_handle_hl(const fm::SGMLishNode &node) {
  (void)node;
  // TODO tokenize the resultant sentence.
  //_text_lexer.lex(*node.text());
  //std::cerr << "<HL> #pars=" << _text_lexer.paragraph_indexes().size() << std::endl;
}


void
TipsterImporter::Impl::_handle_text(const fm::SGMLishNode &node) {
  // Use the text lexer to identify paragraph boundaries.
  _text_lexer.lex(*node.text());

  const auto child_text_begin_it = node.text()->begin();
  for (const auto &pair : _text_lexer.paragraph_indexes()) {
    // Find the newline boundaries in the paragraph.
    std::vector<std::pair<BaseOffsetBuffer::iterator, BaseOffsetBuffer::iterator>> line_bounds;
    BaseOffsetBuffer::iterator first = child_text_begin_it + pair.first;
    BaseOffsetBuffer::iterator last = child_text_begin_it + pair.second;
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

    // Try and capture partial rows, table headings, and table captions.
    for (size_t i = 0; i < line_bounds.size(); ++i) {
      if (is_table_row[i]) {
        // Scan backwards.
        for (size_t j = i; j != 0; --j) {
          if (is_table_row[j - 1] || line_bounds[j - 1].second - line_bounds[j - 1].first <= 1)
            break;
          is_table_row[j - 1] = true;
        }
        // Scan forwards.
        for (size_t j = i + 1; j != line_bounds.size(); ++j) {
          if (is_table_row[j] || line_bounds[j].second - line_bounds[j].first <= 1)
            break;
          is_table_row[j] = true;
        }
        // Skip the rest of the table.
        for ( ; i != line_bounds.size() && is_table_row[i]; ++i) { }
      }
    }

    // Copy lines that we haven't classified as being a table row into the buffer to send to the tokenizer. what we've classified as a table row.
    tk::OffsetInputStream<> ois(pair.second - pair.first);
    for (size_t i = 0; i != line_bounds.size(); ++i) {
      if (!is_table_row[i])
        ois.write(line_bounds[i].first, line_bounds[i].second);
      //else {
        //std::string s(reinterpret_cast<const char *>(line_bounds[i].first.get_bytes()), line_bounds[i].second - line_bounds[i].first);
        //std::cerr << i << ") " << s << std::endl;
      //}
    }

    // Tokenize the paragraph.
    const size_t nsentences_before = _doc->sentences.size();
    _tokenizer.tokenize(ois, *_doc);
    const size_t nsentences_after = _doc->sentences.size();

    // Create the Paragraph object and add it to the document.
    if (nsentences_before != nsentences_after) {
      cs::Paragraph paragraph;
      paragraph.span.start = reinterpret_cast<cs::Sentence *>(nsentences_before);
      paragraph.span.stop = reinterpret_cast<cs::Sentence *>(nsentences_after);
      _doc->paragraphs.push_back(paragraph);
    }
  }
}


void
TipsterImporter::Impl::_process_tree(const fm::SGMLishNode &node) {
  const fm::SGMLishNode *child;

  // If we're at a <DOCNO> node, extract the document number.
  if (node.is_start_tag() && node.has_name("docno")) {
    child = node.child();
    if (child != nullptr && child->is_text())
      _handle_docno(*child);
  }

  // If we're at a <DATELINE> node, extract and store the contents.
  if (node.is_start_tag() && node.has_name("dateline")) {
    child = node.child();
    if (child != nullptr && child->is_text())
      _handle_dateline(*child);
  }

  // If we're at a <DD> or <DATE> node, extract and parse the date contents.
  if (node.is_start_tag() && (node.has_name("dd") || node.has_name("date"))) {
    child = node.child();
    if (child != nullptr && child->is_text())
      _handle_date(*child);
  }

  // If we're at a <HL>...</HL> node, extract the headline.
  if (node.is_start_tag() && node.has_name("hl")) {
    child = node.child();
    if (child != nullptr && child->is_text())
      _handle_hl(*child);
  }

  // If we're at a <TEXT> node, extract and tokenize the contents.
  if (node.is_start_tag() && node.has_name("text")) {
    child = node.child();
    if (child != nullptr && child->is_text())
      _handle_text(*child);
  }

  // Recursive tree processing.
  if (node.child() != nullptr)
    _process_tree(*node.child());
  if (node.sibling() != nullptr)
    _process_tree(*node.sibling());
}


void
TipsterImporter::Impl::_unswizzle_pointers(void) {
  for (auto &sentence : _doc->sentences) {
    sentence.span.start = &_doc->tokens[reinterpret_cast<size_t>(sentence.span.start)];
    sentence.span.stop = &_doc->tokens[reinterpret_cast<size_t>(sentence.span.stop)];
  }

  for (auto &paragraph : _doc->paragraphs) {
    paragraph.span.start = &_doc->sentences[reinterpret_cast<size_t>(paragraph.span.start)];
    paragraph.span.stop = &_doc->sentences[reinterpret_cast<size_t>(paragraph.span.stop)];
  }
}


cs::Doc *
TipsterImporter::Impl::_read_doc(void) {
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
  _unswizzle_pointers();
  return _doc;
}


// ================================================================================================
// TipsterImporter
// ================================================================================================
TipsterImporter::TipsterImporter(const std::string &path) : _impl(new Impl(path)) { }

TipsterImporter::~TipsterImporter(void) {
  delete _impl;
}

cs::Doc *
TipsterImporter::import(void) const {
  return _impl->import();
}


// ================================================================================================
// TipsterTextLexer
// ================================================================================================
TipsterTextLexer::TipsterTextLexer(void) { }

void
TipsterTextLexer::_create_paragraph(void) {
  _create_paragraph(_par_start_index, _state.ts.get_index());
  _par_start_index = _state.te.get_index();
}


void
TipsterTextLexer::_create_paragraph(const size_t start_index, const size_t end_index) {
  _paragraphs.emplace_back(std::make_pair(start_index, end_index));
}


void
TipsterTextLexer::lex(const BaseOffsetBuffer &buffer) {
  // Reset our internal state.
  _state.reset(buffer.begin(), buffer.end());
  _par_start_index = 0;
  _paragraphs.clear();

  // Run the Ragel lexer.
  const bool success = _lex();
  if (!success)
    throw RagelException("Failed to lex using TipsterTextLexer");

  // Add the final paragraph.
  if (_par_start_index != buffer.nitems_used())
    _create_paragraph(_par_start_index, buffer.nitems_used());
}

}  // namesapce corpora
}  // namespace schwa
