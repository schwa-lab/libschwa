/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/corpora/tipster.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include <schwa/encoding.h>
#include <schwa/exception.h>
#include <schwa/formats/sgml.h>
#include <schwa/io/streams.h>
#include <schwa/pool.h>
#include <schwa/unicode.h>
#include <schwa/utils/buffer.h>

#include <schwa/third-party/re2/re2.h>


namespace fm = ::schwa::formats;
using ::schwa::third_party::re2::RE2;

namespace schwa {
namespace corpora {
namespace tipster {

//!< Regex for matching surrounding whitespace. Useful for stripping.
static const RE2 RE_SURROUNDING_WHITESPACE("^\\s+|\\s+$");
//!< Regex for parsing a short US date. Useful since strptime is POSIX only.
static const RE2 RE_US_SHORT_DATE("^(0?[1-9]|1[0-2])/(0?[1-9]|[12][0-9]|3[01])/([0-9]{2})$");


// ================================================================================================
// docrep schemas
// ================================================================================================
Token::Schema::Schema(void) :
    dr::Ann::Schema<Token>("Token", "The token class"),
    span(*this, "span", "The beginning and end byte offsets of the token", dr::FieldMode::READ_WRITE),
    raw(*this, "raw", "The raw token", dr::FieldMode::READ_WRITE)
  { }
Token::Schema::~Schema(void) { }


Sentence::Schema::Schema(void) :
    dr::Ann::Schema<Sentence>("Sentence", "The sentence class"),
    span(*this, "span", "The beginning and end token offsets of the sentence", dr::FieldMode::READ_WRITE)
  { }
Sentence::Schema::~Schema(void) { }


Paragraph::Schema::Schema(void) :
    dr::Ann::Schema<Paragraph>("Paragraph", "The paragraph class"),
    span(*this, "span", "The beginning and end sentence offsets of the paragraph", dr::FieldMode::READ_WRITE)
  { }
Paragraph::Schema::~Schema(void) { }


Doc::Schema::Schema(void) :
    dr::Doc::Schema<Doc>("Doc", "The document class"),
    doc_id(*this, "doc_id", "The ID of the document (from <DOCNO>)", dr::FieldMode::READ_WRITE),
    story_date(*this, "story_date", "The ISO-8601 representation of the date of publication (from <DD>)", dr::FieldMode::READ_WRITE),
    dateline(*this, "dateline", "The dateline of the document (<DATELINE>)", dr::FieldMode::READ_WRITE),
    headline(*this, "headline", "The headline of the document (<HEADLINE>)", dr::FieldMode::READ_WRITE),
    tokens(*this, "tokens", "The store for the tokens", dr::FieldMode::READ_WRITE),
    sentences(*this, "sentences", "The store for the sentences", dr::FieldMode::READ_WRITE),
    paragraphs(*this, "paragraphs", "The store for the paragraphs", dr::FieldMode::READ_WRITE)
  { }
Doc::Schema::~Schema(void) { }


// ================================================================================================
// Importer::Impl
// ================================================================================================
class Importer::Impl {
private:
  EncodingResult _er;
  Pool _pool;
  Doc *_doc;
  fm::SGMLishParser *_parser;

  void
  _process_tree(const fm::SGMLishNode &node) const {
    // If we're at a <DOCNO>...</DOCNO> node, extract the document number.
    if (node.is_start_tag() && node.has_name("docno")) {
      const fm::SGMLishNode *child = node.child();
      if (child != nullptr && child->is_text()) {
        // Strip surrounding whitespace.
        _doc->doc_id = std::string(reinterpret_cast<const char *>(child->text()->bytes()), child->text()->nitems_used());
        RE2::GlobalReplace(&_doc->doc_id, RE_SURROUNDING_WHITESPACE, "");
      }
    }

    // If we're at a <DATELINE>...</DATELINE> node, extract and store the contents.
    if (node.is_start_tag() && node.has_name("dateline")) {
      const fm::SGMLishNode *child = node.child();
      if (child != nullptr && child->is_text()) {
        // Strip surrounding whitespace.
        _doc->dateline = std::string(reinterpret_cast<const char *>(child->text()->bytes()), child->text()->nitems_used());
        RE2::GlobalReplace(&_doc->dateline, RE_SURROUNDING_WHITESPACE, "");
      }
    }

    // If we're at a <DD>...</DD> node, extract and parse the date contents.
    if (node.is_start_tag() && node.has_name("dd")) {
      const fm::SGMLishNode *child = node.child();
      if (child != nullptr && child->is_text()) {
        // Strip surrounding whitespace.
        std::string dd = std::string(reinterpret_cast<const char *>(child->text()->bytes()), child->text()->nitems_used());
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
    }

    // If we're at a <HL>...</HL> node, extract the headline.
    if (node.is_start_tag() && node.has_name("hl")) {
      const fm::SGMLishNode *child = node.child();
      if (child != nullptr && child->is_text()) {
        std::string hl = std::string(reinterpret_cast<const char *>(child->text()->bytes()), child->text()->nitems_used());
        std::cout << "HL='" << hl << "'" << std::endl;
      }
    }

    // If we're at a <TEXT>...</TEXT> node, extract and tokenize the contents.
    if (node.is_start_tag() && node.has_name("text")) {
      const fm::SGMLishNode *child = node.child();
      if (child != nullptr && child->is_text()) {
        std::string text = std::string(reinterpret_cast<const char *>(child->text()->bytes()), child->text()->nitems_used());
        std::cout << "TEXT='" << text << "'" << std::endl;
      }
    }

    // Recursive tree processing.
    if (node.child() != nullptr)
      _process_tree(*node.child());
    if (node.sibling() != nullptr)
      _process_tree(*node.sibling());
  }

  Doc *
  _read_doc(void) {
    delete _doc;

    // Read in and parse the next SGML document.
    fm::SGMLishNode *const root = _parser->parse();
    if (root == nullptr) {
      if (!_parser->eof())
        throw schwa::Exception("Failed to parse");
      _doc = nullptr;
      return _doc;
    }

    // Process the SGML tree, extracting the necesary information.
    _doc = new Doc();
    _process_tree(*root);
    return _doc;
  }

public:
  explicit Impl(const std::string &path) :
      _pool(4 * 1024 * 1024),
      _doc(nullptr),
      _parser(nullptr)
    {
    // Read in all of the raw data.
    io::InputStream in(path);
    Buffer<> encoded_bytes(1 * 1024 * 1024);
    encoded_bytes.consume(in);

    // Decode the read in data.
    to_utf8(Encoding::ASCII, encoded_bytes, _er);

    // Construct a SGML parser around the decoded data.
    _parser = new fm::SGMLishParser(_er, _pool);
  }

  ~Impl(void) {
    delete _doc;
    delete _parser;
  }

  Doc *
  import(void) {
    // Read the next document from the input stream.
    return _read_doc();
  }
};


// ================================================================================================
// Importer
// ================================================================================================
Importer::Importer(const std::string &path) : _impl(new Impl(path)) { }

Importer::~Importer(void) {
  delete _impl;
}

Doc *
Importer::import(void) const {
  return _impl->import();
}

}  // namespace tipster
}  // namesapce formats
}  // namespace schwa
