/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/canonical-schema.h>


namespace schwa {
namespace canonical_schema {

// ================================================================================================
// docrep models
// ================================================================================================
Heading::Heading(void) : sentence(nullptr), level(0) { }


ListItem::ListItem(void) : sentence(nullptr), list(nullptr) { }


List::List(void) : ordered(false) { }


Block::Block(void) : list(nullptr), heading(nullptr), paragraph(nullptr) { }


// ================================================================================================
// docrep schemas
// ================================================================================================
Token::Schema::Schema(void) :
    dr::Ann::Schema<Token>("Token", "The token class"),
    span(*this, "span", "The beginning and end byte offsets of the token into the original document", dr::FieldMode::READ_WRITE),
    raw(*this, "raw", "The raw token", dr::FieldMode::READ_WRITE),
    norm(*this, "norm", "The normalised token", dr::FieldMode::READ_WRITE)
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


Heading::Schema::Schema(void) :
    dr::Ann::Schema<Heading>("Heading", "The heading class"),
    sentence(*this, "sentence", "Pointer to the underlying sentence", dr::FieldMode::READ_WRITE),
    level(*this, "level", "The level of the heading (e.g. <h1> in HTML would be level 1)", dr::FieldMode::READ_WRITE)
  { }
Heading::Schema::~Schema(void) { }


ListItem::Schema::Schema(void) :
    dr::Ann::Schema<ListItem>("ListItem", "The list item class"),
    sentence(*this, "sentence", "Pointer to the underlying sentence", dr::FieldMode::READ_WRITE),
    list(*this, "list", "Pointer to the underlying nested list", dr::FieldMode::READ_WRITE)
  { }
ListItem::Schema::~Schema(void) { }


List::Schema::Schema(void) :
    dr::Ann::Schema<List>("List", "The list class"),
    items(*this, "items", "The items of teh list", dr::FieldMode::READ_WRITE),
    ordered(*this, "ordered", "Whether or not this list has a semantic ordering on the items", dr::FieldMode::READ_WRITE)
  { }
List::Schema::~Schema(void) { }


Block::Schema::Schema(void) :
    dr::Ann::Schema<Block>("Block", "Abstract model representing the sequential top-level blocks of the document"),
    list(*this, "list", "Pointer to the List object if this is a list", dr::FieldMode::READ_WRITE),
    heading(*this, "heading", "Pointer to the Heading object if this is a heading", dr::FieldMode::READ_WRITE),
    paragraph(*this, "paragraph", "Pointer to the Paragraph object if this is a paragraph", dr::FieldMode::READ_WRITE)
  { }
Block::Schema::~Schema(void) { }


Doc::Schema::Schema(void) :
    dr::Doc::Schema<Doc>("Doc", "The document class"),
    doc_id(*this, "doc_id", "The ID of the document", dr::FieldMode::READ_WRITE),
    date(*this, "date", "The ISO-8601 representation of the date of publication", dr::FieldMode::READ_WRITE),
    dateline(*this, "dateline", "The dateline of the document", dr::FieldMode::READ_WRITE),
    headline(*this, "headline", "The headline of the document", dr::FieldMode::READ_WRITE),
    encoding(*this, "encoding", "The name of the encoding of the original document", dr::FieldMode::READ_WRITE),
    tokens(*this, "tokens", "The store for the tokens", dr::FieldMode::READ_WRITE),
    sentences(*this, "sentences", "The store for the sentences", dr::FieldMode::READ_WRITE),
    paragraphs(*this, "paragraphs", "The store for the paragraphs", dr::FieldMode::READ_WRITE),
    headings(*this, "headings", "The store for the headings", dr::FieldMode::READ_WRITE),
    list_items(*this, "list_items", "The store for the list items", dr::FieldMode::READ_WRITE),
    lists(*this, "lists", "The store for the lists", dr::FieldMode::READ_WRITE),
    blocks(*this, "blocks", "The store for the blocks", dr::FieldMode::READ_WRITE)
  { }
Doc::Schema::~Schema(void) { }

}  // namesapce canonical_schema
}  // namespace schwa
