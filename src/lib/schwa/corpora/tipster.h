/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CORPORA_TIPSTER_H_
#define SCHWA_CORPORA_TIPSTER_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/dr.h>


namespace schwa {
  namespace corpora {
    namespace tipster {

      // ======================================================================
      // docrep models
      // ======================================================================
      class List;

      class Token : public dr::Ann {
      public:
        dr::Slice<uint64_t> span;
        std::string raw;

        class Schema;
      };


      class Sentence : public dr::Ann {
      public:
        dr::Slice<Token *> span;

        class Schema;
      };


      class Paragraph : public dr::Ann {
      public:
        dr::Slice<Sentence *> span;

        class Schema;
      };


      class Heading : public dr::Ann {
      public:
        dr::Pointer<Sentence> sentence;
        unsigned int level;

        Heading(void);

        class Schema;
      };


      class ListItem : public dr::Ann {
      public:
        dr::Pointer<Sentence> sentence;  //!< One of ListItem::sentence or ListItem::list will be not null, depending on what kind of list item it is.
        dr::Pointer<List> list;          //!< One of ListItem::sentence or ListItem::list will be not null, depending on what kind of list item it is.

        class Schema;
      };


      class List : public dr::Ann {
      public:
        dr::Slice<ListItem *> items;

        class Schema;
      };


      class Doc : public dr::Doc {
      public:
        std::string doc_id;  //!< The contents of the `DOCNO` element.
        std::string story_date;  //!< ISO-8601 date representation of the contents of the `DD` element.
        std::string dateline;
        dr::Pointer<Sentence> headline;

        dr::Store<Token> tokens;
        dr::Store<Sentence> sentences;
        dr::Store<Paragraph> paragraphs;
        dr::Store<Heading> headings;
        dr::Store<List> lists;

        class Schema;
      };


      class Token::Schema : public dr::Ann::Schema<Token> {
      public:
        DR_FIELD(&Token::span) span;
        DR_FIELD(&Token::raw) raw;

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


      class Doc::Schema : public dr::Doc::Schema<Doc> {
      public:
        DR_FIELD(&Doc::doc_id) doc_id;
        DR_FIELD(&Doc::story_date) story_date;
        DR_FIELD(&Doc::dateline) dateline;
        DR_POINTER(&Doc::headline, &Doc::sentences) headline;

        DR_STORE(&Doc::tokens) tokens;
        DR_STORE(&Doc::sentences) sentences;
        DR_STORE(&Doc::paragraphs) paragraphs;

        Schema(void);
        virtual ~Schema(void);
      };


      // ======================================================================
      // Importer
      // ======================================================================
      class Importer {
      private:
        class Impl;
        Impl *_impl;

      public:
        explicit Importer(const std::string &path);
        ~Importer(void);

        Doc *import(void) const;

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(Importer);
      };

    }  // namespace tipster
  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_CORPORA_TIPSTER_H_
