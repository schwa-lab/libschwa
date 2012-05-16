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

      virtual void add(Type, const char *raw, offset_type, offset_type len, const char *norm=0) {
        if (!_new_sentence)
          _out << ' ';
        _new_sentence = false;

        if (norm && _normalise)
          _out << norm;
        else
          _out.write(raw, len);
      }

      virtual void error(const char *, offset_type, offset_type) { }

      virtual void begin_sentence(void) {
        _new_sentence = true;
      }
      virtual void end_sentence(void) {
        _out << '\n';
      }

      virtual void begin_paragraph(void) {
        if (!_new_document)
          _out << '\n';
        _new_document = false;
      }

      virtual void end_paragraph(void) { }

      virtual void begin_heading(int) { begin_paragraph(); }
      virtual void end_heading(int) { end_paragraph(); }

      virtual void begin_list(void) { begin_paragraph(); }
      virtual void end_list(void) { }

      virtual void begin_item(void) {
        if (_normalise)
          _out << "* ";
      }
      virtual void end_item(void) { }

      virtual void begin_document(void) {
        _new_document = true;
      }

      virtual void end_document(void) { }
    };

  }
}
