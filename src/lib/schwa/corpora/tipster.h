/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CORPORA_TIPSTER_H_
#define SCHWA_CORPORA_TIPSTER_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/dr.h>


namespace schwa {
  namespace corpora {
    namespace tipster {

      class Token : public dr::Ann {
      public:
        dr::Slice<uint64_t> span;
        std::string raw;

        class Schema;
      };


      class Sentence : public dr::Ann {
      public:
        dr::Slice<Token> span;

        class Schema;
      };


      class Paragraph : public dr::Ann {
      public:
        dr::Slice<Sentence> span;

        class Schema;
      };


      class Doc : public dr::Doc {
      public:
        std::string doc_id;  //!< The contents of the `DOCNO` element.
        std::string story_date;  //!< ISO 8601 date representation of the contents of the `DD` element.
        std::string dateline;

        dr::Store<Token> tokens;
        dr::Store<Sentence> sentences;
        dr::Store<Paragraph> paragraphs;

        class Schema;
      };


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
