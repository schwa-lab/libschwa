/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TOKENIZER_DOCREP_STREAM_H_
#define SCHWA_TOKENIZER_DOCREP_STREAM_H_

#include <iosfwd>
#include <string>
#include <vector>

#include <schwa/_base.h>
#include <schwa/dr.h>
#include <schwa/tokenizer/stream.h>


namespace schwa {
  namespace tokenizer {

    class Token : public dr::Ann {
    public:
      dr::Slice<uint64_t> span;
      std::string raw;
      std::string norm;

      class Schema;
    };


    class Sent : public dr::Ann {
    public:
      dr::Slice<Token *> span;

      class Schema;
    };


    class Paragraph : public dr::Ann {
    public:
      dr::Slice<Sent *> span;

      class Schema;
    };


    class Doc : public dr::Doc {
    public:
      dr::Store<Token> tokens;
      dr::Store<Sent> sents;
      dr::Store<Paragraph> pars;

      class Schema;
    };


    class Token::Schema : public dr::Ann::Schema<Token> {
    public:
      DR_FIELD(&Token::span) span;
      DR_FIELD(&Token::raw) raw;
      DR_FIELD(&Token::norm) norm;

      Schema(void);
      virtual ~Schema(void);
    };


    class Sent::Schema : public dr::Ann::Schema<Sent> {
    public:
      DR_POINTER(&Sent::span, &Doc::tokens) span;

      Schema(void);
      virtual ~Schema(void);
    };


    class Paragraph::Schema : public dr::Ann::Schema<Paragraph> {
    public:
      DR_POINTER(&Paragraph::span, &Doc::sents) span;

      Schema(void);
      virtual ~Schema(void);
    };


    class Doc::Schema : public dr::Doc::Schema<Doc> {
    public:
      DR_STORE(&Doc::tokens) tokens;
      DR_STORE(&Doc::sents) sents;
      DR_STORE(&Doc::pars) pars;

      Schema(void);
      virtual ~Schema(void);
    };


    class DocrepStream : public Stream {
    protected:
      struct TmpToken {
        uint64_t start;
        uint64_t stop;
        std::string raw;
        std::string norm;
      };

      struct TmpSent {
        uint64_t start;
        uint64_t stop;
      };

      struct TmpPar {
        uint64_t start;
        uint64_t stop;
      };

      std::ostream &_out;
      dr::Writer _writer;
      Doc *_doc;
      const bool _normalise;
      std::vector<TmpToken> _tokens;
      std::vector<TmpSent> _sents;
      std::vector<TmpPar> _pars;
      uint64_t _begin_sent;
      uint64_t _begin_par;

      void ensure_doc(void);

    public:
      DocrepStream(std::ostream &out, Doc::Schema &dschema, bool normalise=true);
      virtual ~DocrepStream(void);

      void add(Type, const char *raw, size_t begin, size_t len, const char *norm=nullptr) override;
      void error(const char *raw, size_t begin, size_t len);

      void begin_sentence(void) override;
      void end_sentence(void) override;

      void begin_paragraph(void) override;
      void end_paragraph(void) override;

      void begin_heading(int depth) override;
      void end_heading(int depth) override;

      void begin_list(void) override { }
      void end_list(void) override { }

      void begin_item(void) override { }
      void end_item(void) override { }

      void begin_document(void) override;
      void end_document(void) override;
    };

  }
}

#endif  // SCHWA_TOKENIZER_DOCREP_STREAM_H_
