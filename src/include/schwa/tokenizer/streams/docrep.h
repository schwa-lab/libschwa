/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace tokenizer {

    class Token : public dr::Annotation {
    public:
      dr::Slice<uint64_t> slice;
      std::string raw;
      std::string norm;

      class Schema;
    };


    class Sent : public dr::Annotation {
    public:
      dr::Slice<Token *> slice;

      class Schema;
    };


    class Doc : public dr::Document {
    public:
      dr::Store<Token> tokens;
      dr::Store<Sent> sents;

      class Schema;
    };


    class Token::Schema : public dr::AnnotationSchema<Token> {
    public:
      DR_FIELD(&Token::slice) slice;
      DR_FIELD(&Token::raw) raw;
      DR_FIELD(&Token::norm) norm;

      Schema(void);
      virtual ~Schema(void);
    };


    class Sent::Schema : public dr::AnnotationSchema<Sent> {
    public:
      DR_POINTER(&Sent::slice, &Doc::tokens) slice;

      Schema(void);
      virtual ~Schema(void);
    };


    class Doc::Schema : public dr::DocumentSchema<Doc> {
    public:
      DR_STORE(&Doc::tokens) tokens;
      DR_STORE(&Doc::sents) sents;

      Schema(void);
      virtual ~Schema(void);
    };


    class DocrepStream : public Stream {
    protected:
      std::ostream &_out;
      dr::Writer _writer;
      Doc *_doc;
      const bool _normalise;
      bool _new_document;
      bool _new_sentence;

      void ensure_doc(void);

    public:
      DocrepStream(std::ostream &out, Doc::Schema &dschema, bool normalise=true);
      virtual ~DocrepStream(void);

      virtual void add(Type, const char *raw, offset_type, offset_type len, const char *norm=0);
      virtual void error(const char *, offset_type, offset_type);

      virtual void begin_sentence(void);
      virtual void end_sentence(void);

      virtual void begin_paragraph(void);

      virtual void end_paragraph(void);

      virtual void begin_heading(int);
      virtual void end_heading(int);

      virtual void begin_list(void);
      virtual void end_list(void);

      virtual void begin_item(void);
      virtual void end_item(void);

      virtual void begin_document(void);
      virtual void end_document(void);
    };

  }
}
