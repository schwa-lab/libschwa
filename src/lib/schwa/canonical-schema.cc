/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/canonical-schema.h>


namespace schwa {
namespace canonical_schema {

// ================================================================================================
// docrep models
// ================================================================================================
Heading::Heading(void) :
    sentence(nullptr),
    level(0),
    is_author(false),
    is_dateline(false),
    is_heading(false)
  { }


ListItem::ListItem(void) : sentence(nullptr), list(nullptr) { }


List::List(void) : ordered(false) { }


Block::Block(void) : list(nullptr), heading(nullptr), paragraph(nullptr) { }


void
Doc::unswizzle_pointers(void) {
  for (auto &sentence : sentences) {
    sentence.span.start = &tokens[reinterpret_cast<size_t>(sentence.span.start) - 1];
    sentence.span.stop = &tokens[reinterpret_cast<size_t>(sentence.span.stop) - 1];
  }

  for (auto &paragraph : paragraphs) {
    paragraph.span.start = &sentences[reinterpret_cast<size_t>(paragraph.span.start) - 1];
    paragraph.span.stop = &sentences[reinterpret_cast<size_t>(paragraph.span.stop) - 1];
  }

  for (auto &heading : headings) {
    if (heading.sentence != nullptr)
      heading.sentence = &sentences[reinterpret_cast<size_t>(heading.sentence) - 1];
  }

  for (auto &hyperlink : hyperlinks) {
    hyperlink.span.start = &tokens[reinterpret_cast<size_t>(hyperlink.span.start) - 1];
    hyperlink.span.stop = &tokens[reinterpret_cast<size_t>(hyperlink.span.stop) - 1];
  }

  for (auto &li : list_items) {
    if (li.sentence != nullptr)
      li.sentence = &sentences[reinterpret_cast<size_t>(li.sentence) - 1];
    if (li.list != nullptr)
      li.list = &lists[reinterpret_cast<size_t>(li.list) - 1];
  }

  for (auto &list : lists) {
    list.span.start = &list_items[reinterpret_cast<size_t>(list.span.start) - 1];
    list.span.stop = &list_items[reinterpret_cast<size_t>(list.span.stop) - 1];
  }

  for (auto &block : blocks) {
    if (block.list != nullptr)
      block.list = &lists[reinterpret_cast<size_t>(block.list) - 1];
    if (block.heading != nullptr)
      block.heading = &headings[reinterpret_cast<size_t>(block.heading)] - 1;
    if (block.paragraph != nullptr)
      block.paragraph = &paragraphs[reinterpret_cast<size_t>(block.paragraph) - 1];
  }
}


// ================================================================================================
// docrep schemas
// ================================================================================================
Token::Schema::Schema(void) :
    dr::Ann::Schema<Token>("Token", "The token class"),
    span(*this, "span", "The beginning and end byte offsets of the token into the original document", dr::FieldMode::READ_WRITE),
    raw(*this, "raw", "The raw token", dr::FieldMode::READ_WRITE),
    norm(*this, "norm", "The normalised token", dr::FieldMode::READ_WRITE),
    pos(*this, "pos", "The POS tag for the token", dr::FieldMode::READ_WRITE)
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
    level(*this, "level", "The level of the heading (e.g. <h1> in HTML would be level 1)", dr::FieldMode::READ_WRITE),
    is_author(*this, "is_author", "Is this heading line the name of an author?", dr::FieldMode::READ_WRITE),
    is_dateline(*this, "is_dateline", "Is this heading line a dateline?", dr::FieldMode::READ_WRITE),
    is_heading(*this, "is_heading", "Is this heading line some kind of heading?", dr::FieldMode::READ_WRITE)
  { }
Heading::Schema::~Schema(void) { }


Hyperlink::Schema::Schema(void) :
    dr::Ann::Schema<Hyperlink>("Hyperlink", "The hyperlink class"),
    span(*this, "span", "The beginning and end token offsets of the hyperlink", dr::FieldMode::READ_WRITE),
    link(*this, "link", "The raw link that appeared in the source document", dr::FieldMode::READ_WRITE)
  { }
Hyperlink::Schema::~Schema(void) { }


ListItem::Schema::Schema(void) :
    dr::Ann::Schema<ListItem>("ListItem", "The list item class"),
    sentence(*this, "sentence", "Pointer to the underlying sentence", dr::FieldMode::READ_WRITE),
    list(*this, "list", "Pointer to the underlying nested list", dr::FieldMode::READ_WRITE)
  { }
ListItem::Schema::~Schema(void) { }


List::Schema::Schema(void) :
    dr::Ann::Schema<List>("List", "The list class"),
    span(*this, "span", "The span of ListItem's in the list", dr::FieldMode::READ_WRITE),
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


NamedEntity::Schema::Schema(void) :
    dr::Ann::Schema<NamedEntity>("NamedEntity", "NE class"),
    span(*this, "span", "A slice over the token objects over which this NE spans", dr::FieldMode::READ_WRITE),
    label(*this, "label", "The NE class label", dr::FieldMode::READ_WRITE)
  { }
NamedEntity::Schema::~Schema(void) { }


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
    hyperlinks(*this, "hyperlinks", "The store for the hyperlinks", dr::FieldMode::READ_WRITE),
    list_items(*this, "list_items", "The store for the list items", dr::FieldMode::READ_WRITE),
    lists(*this, "lists", "The store for the lists", dr::FieldMode::READ_WRITE),
    blocks(*this, "blocks", "The store for the blocks", dr::FieldMode::READ_WRITE),
    named_entities(*this, "named_entities", "The store for the named entities", dr::FieldMode::READ_WRITE)
  { }
Doc::Schema::~Schema(void) { }

}  // namesapce canonical_schema
}  // namespace schwa
