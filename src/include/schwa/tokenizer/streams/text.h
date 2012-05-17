/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace tokenizer {

    class TextStream: public Stream {
    protected:
      std::ostream &_out;
      bool _normalise;
      bool _new_document;
      bool _new_sentence;

    public:
      TextStream(std::ostream &out, bool normalise=true) : _out(out), _normalise(normalise), _new_document(true), _new_sentence(true) { }
      virtual ~TextStream(void) { }

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
