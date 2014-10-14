/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/corpora/tipster.h>

#include <fstream>
#include <iostream>

#include <schwa/encoding.h>
#include <schwa/exception.h>
#include <schwa/formats/sgml.h>
#include <schwa/io/streams.h>
#include <schwa/pool.h>
#include <schwa/unicode.h>
#include <schwa/utils/buffer.h>


namespace fm = ::schwa::formats;

namespace schwa {
namespace corpora {
namespace tipster {


// ================================================================================================
// Importer::Impl
// ================================================================================================
class Importer::Impl {
private:
  EncodingResult _er;
  Doc *_doc;
  fm::SGMLishParser *_parser;


  void
  _process_tree(const fm::SGMLishNode &node) const {
    // If we're at a <DD>...</DD> node, extract and parse the date contents, and don't recurse.
    if (node.is_start_tag() && node.has_name("dd")) {
      const fm::SGMLishNode *child = node.child();
      if (child != nullptr && child->is_text()) {
        std::cout << "DD='" << reinterpret_cast<const char *>(child->text()) << "'" << std::endl;
      }
      return;
    }

    // Recursive tree processing.
    if (node.child() != nullptr)
      _process_tree(*node.child());
    if (node.sibling() != nullptr)
      _process_tree(*node.sibling());
  }

  void
  _read_doc(void) {
    delete _doc;

    // Read in and parse the next SGML document.
    Pool pool(4 * 1024 * 1024);
    fm::SGMLishNode *const root = _parser->parse(pool);
    if (root == nullptr) {
      if (!_parser->eof())
        throw schwa::Exception("Failed to parse");
      _doc = nullptr;
      return;
    }

    // Process the SGML tree, extracting the necesary information.
    _doc = new Doc();
    _process_tree(*root);
  }

public:
  explicit Impl(const std::string &path) :
      _doc(nullptr),
      _parser(nullptr)
    {
    // Read in all of the raw data.
    io::InputStream in(path);
    Buffer encoded_bytes(1 * 1024 * 1024);
    encoded_bytes.consume(in);

    // Decode the read in data.
    to_utf8(Encoding::ASCII, encoded_bytes, _er);

    // Construct a SGML parser around the decoded data.
    _parser = new fm::SGMLishParser(_er);

    // Read the next document from the input stream.
    _read_doc();
  }

  ~Impl(void) {
    delete _doc;
    delete _parser;
  }

  Doc *
  import(void) {
    Doc *doc = _doc;
    _read_doc();
    return doc;
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
