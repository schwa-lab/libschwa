/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace tokenizer {

    class DebugTextStream : public Stream {
    public:
      static const int NCOLOURS;
      static const char *const COLOURS[];
      static const char *const OFF;

    protected:
      std::ostream &_out;
      bool _new_sentence;
      unsigned int _colour;

    public:
      DebugTextStream(std::ostream &out) : _out(out), _new_sentence(true), _colour(0) { }
      virtual ~DebugTextStream(void) { }

      virtual void add(Type, const char *raw, offset_type, offset_type len, const char *norm=0);
      virtual void error(const char *raw, offset_type, offset_type len);

      virtual void begin_sentence(void);
      virtual void end_sentence(void);

      virtual void begin_list(void);
      virtual void end_list(void);

      virtual void begin_item(void);
      virtual void end_item(void);

      virtual void begin_paragraph(void);
      virtual void end_paragraph(void);

      virtual void begin_heading(int depth);
      virtual void end_heading(int depth);

      virtual void begin_document(void);
      virtual void end_document(void);
    };

  }
}
