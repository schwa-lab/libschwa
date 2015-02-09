/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CANONICAL_SCHEMA_H_
#define SCHWA_CANONICAL_SCHEMA_H_

#include <schwa/_base.h>
#include <schwa/dr.h>


namespace schwa {
  namespace canonical_schema {

    // ========================================================================
    // docrep models
    // ========================================================================
    class Doc;
    class Block;
    class Heading;
    class Hyperlink;
    class List;
    class ListItem;
    class Paragraph;
    class Sentence;
    class Token;


    class Token : public dr::Ann {
    public:
      dr::Slice<uint64_t> span;  //!< The span of the token into the original document.
      std::string raw;           //!< UTF-8 version of the raw token.
      std::string norm;          //!< If the tokenizer decided to normalise the raw token, this contains the normalised UTF-8 token.
      std::string pos;           //!< UTF-8 POS tag.

      class Schema;
    };


    class Sentence : public dr::Ann {
    public:
      dr::Slice<Token *> span;  //!< A sentence is a sequential span of tokens.

      class Schema;
    };


    class Paragraph : public dr::Ann {
    public:
      dr::Slice<Sentence *> span;  //!< A paragraph is a sequential span of sentences.

      class Schema;
    };


    class Heading : public dr::Ann {
    public:
      dr::Pointer<Sentence> sentence;
      unsigned int level;

      Heading(void);

      class Schema;
    };


    class Hyperlink : public dr::Ann {
    public:
      dr::Slice<Token *> span;  //!< A hyperlink is over a series of tokens.
      std::string link;

      class Schema;
    };


    /**
     * A ListItem object represents an item in a List, which maps to a LI element in HTML. One
     * and only one of the pointers on this object will be not-null.
     **/
    class ListItem : public dr::Ann {
    public:
      dr::Pointer<Sentence> sentence;
      dr::Pointer<List> list;

      ListItem(void);

      class Schema;
    };


    /**
     * A List object represents a list such as a LI or OL element in HTML. The List::ordered
     * attribute dictates whether or not the items have an ordering associated with them.
     **/
    class List : public dr::Ann {
    public:
      dr::Slice<ListItem *> span;
      bool ordered;

      List(void);

      class Schema;
    };


    /**
     * A block is an abstract model that represents top-level elements of a document. One and only
     * one of the pointers in this model will be not-null. Iterating through the store of Block
     * objects will yield the overall document structure as it originally appeared.
     **/
    class Block : public dr::Ann {
    public:
      dr::Pointer<List> list;
      dr::Pointer<Heading> heading;
      dr::Pointer<Paragraph> paragraph;

      Block(void);

      class Schema;
    };


    class Doc : public dr::Doc {
    public:
      std::string doc_id;
      std::string date;  //!< ISO-8601 date representation.
      std::string dateline;
      std::string headline;
      std::string encoding;  //!< The name of the encoding of the original document.

      dr::Store<Token> tokens;
      dr::Store<Sentence> sentences;
      dr::Store<Paragraph> paragraphs;
      dr::Store<Heading> headings;
      dr::Store<Hyperlink> hyperlinks;
      dr::Store<ListItem> list_items;
      dr::Store<List> lists;
      dr::Store<Block> blocks;

      class Schema;

      void unswizzle_pointers(void);
    };


    // ======================================================================
    // docrep schemas
    // ======================================================================
    class Token::Schema : public dr::Ann::Schema<Token> {
    public:
      DR_FIELD(&Token::span) span;
      DR_FIELD(&Token::raw) raw;
      DR_FIELD(&Token::norm) norm;
      DR_FIELD(&Token::pos) pos;

      Schema(void);
      virtual ~Schema(void);
    };


    class Sentence::Schema : public dr::Ann::Schema<Sentence> {
    public:
      DR_POINTER(&Sentence::span, &Doc::tokens) span;

      Schema(void);
      virtual ~Schema(void);
    };


    class Paragraph::Schema : public dr::Ann::Schema<Paragraph> {
    public:
      DR_POINTER(&Paragraph::span, &Doc::sentences) span;

      Schema(void);
      virtual ~Schema(void);
    };


    class Heading::Schema : public dr::Ann::Schema<Heading> {
    public:
      DR_POINTER(&Heading::sentence, &Doc::sentences) sentence;
      DR_FIELD(&Heading::level) level;

      Schema(void);
      virtual ~Schema(void);
    };


    class Hyperlink::Schema : public dr::Ann::Schema<Hyperlink> {
    public:
      DR_POINTER(&Hyperlink::span, &Doc::tokens) span;
      DR_FIELD(&Hyperlink::link) link;

      Schema(void);
      virtual ~Schema(void);
    };


    class ListItem::Schema : public dr::Ann::Schema<ListItem> {
    public:
      DR_POINTER(&ListItem::sentence, &Doc::sentences) sentence;
      DR_POINTER(&ListItem::list, &Doc::lists) list;

      Schema(void);
      virtual ~Schema(void);
    };


    class List::Schema : public dr::Ann::Schema<List> {
    public:
      DR_POINTER(&List::span, &Doc::list_items) span;
      DR_FIELD(&List::ordered) ordered;

      Schema(void);
      virtual ~Schema(void);
    };


    class Block::Schema : public dr::Ann::Schema<Block> {
    public:
      DR_POINTER(&Block::list, &Doc::lists) list;
      DR_POINTER(&Block::heading, &Doc::headings) heading;
      DR_POINTER(&Block::paragraph, &Doc::paragraphs) paragraph;

      Schema(void);
      virtual ~Schema(void);
    };


    class Doc::Schema : public dr::Doc::Schema<Doc> {
    public:
      DR_FIELD(&Doc::doc_id) doc_id;
      DR_FIELD(&Doc::date) date;
      DR_FIELD(&Doc::dateline) dateline;
      DR_FIELD(&Doc::headline) headline;
      DR_FIELD(&Doc::encoding) encoding;

      DR_STORE(&Doc::tokens) tokens;
      DR_STORE(&Doc::sentences) sentences;
      DR_STORE(&Doc::paragraphs) paragraphs;
      DR_STORE(&Doc::headings) headings;
      DR_STORE(&Doc::hyperlinks) hyperlinks;
      DR_STORE(&Doc::list_items) list_items;
      DR_STORE(&Doc::lists) lists;
      DR_STORE(&Doc::blocks) blocks;

      Schema(void);
      virtual ~Schema(void);
    };

  }
}

#endif  // SCHWA_CANONICAL_SCHEMA_H_
