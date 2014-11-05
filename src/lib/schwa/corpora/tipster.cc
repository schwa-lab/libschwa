/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/corpora/tipster.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include <schwa/encoding.h>
#include <schwa/exception.h>
#include <schwa/formats/plain-text.h>
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


// ================================================================================================
// TipsterImporter::Impl
// ================================================================================================
class TipsterImporter::Impl {
private:
  EncodingResult _er;
  Pool _pool;
  cs::Doc *_doc;
  fm::SGMLishParser *_sgml_parser;
  fm::PlainTextLexer _plain_text_lexer;
  tk::Tokenizer _tokenizer;

  void
  _process_tree(const fm::SGMLishNode &node) {
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
        _plain_text_lexer.lex(*child->text());
      }
    }

    // If we're at a <TEXT>...</TEXT> node, extract and tokenize the contents.
    if (node.is_start_tag() && node.has_name("text")) {
      const fm::SGMLishNode *child = node.child();
      if (child != nullptr && child->is_text()) {
        std::string text = std::string(reinterpret_cast<const char *>(child->text()->bytes()), child->text()->nitems_used());
        std::cout << "TEXT='" << text << "'" << std::endl;
        _plain_text_lexer.lex(*child->text());

        const auto child_text_begin_it = child->text()->begin();
        for (const auto &pair : _plain_text_lexer.paragraph_indexes()) {
          // Copy the subset of the OffsetBuffer that belongs to the current paragraph into a tokenizer offset input stream.
          tk::OffsetInputStream<> ois(pair.second - pair.first);
          ois.write(child_text_begin_it + pair.first, child_text_begin_it + pair.second);

          // Tokenize the paragraph.
          _tokenizer.tokenize(ois);
          // TODO
        }
      }
    }

    // Recursive tree processing.
    if (node.child() != nullptr)
      _process_tree(*node.child());
    if (node.sibling() != nullptr)
      _process_tree(*node.sibling());
  }

  cs::Doc *
  _read_doc(void) {
    delete _doc;

    // Read in and parse the next SGML document.
    fm::SGMLishNode *const root = _sgml_parser->parse();
    if (root == nullptr) {
      if (!_sgml_parser->eof())
        throw schwa::Exception("Failed to parse");
      _doc = nullptr;
      return _doc;
    }

    // Process the SGML tree, extracting the necesary information.
    _doc = new cs::Doc();
    _doc->encoding = encoding_name(_er.encoding());
    _process_tree(*root);
    return _doc;
  }

public:
  explicit Impl(const std::string &path) :
      _pool(4 * 1024 * 1024),
      _doc(nullptr),
      _sgml_parser(nullptr)
    {
    // Read in all of the raw data.
    io::InputStream in(path);
    Buffer<> encoded_bytes(1 * 1024 * 1024);
    encoded_bytes.consume(in);

    // Decode the read in data.
    to_utf8(Encoding::ASCII, encoded_bytes, _er);

    // Construct a SGML parser around the decoded data.
    _sgml_parser = new fm::SGMLishParser(_er, _pool);
  }

  ~Impl(void) {
    delete _doc;
    delete _sgml_parser;
  }

  cs::Doc *
  import(void) {
    // Read the next document from the input stream.
    return _read_doc();
  }
};


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

}  // namesapce corpora
}  // namespace schwa
