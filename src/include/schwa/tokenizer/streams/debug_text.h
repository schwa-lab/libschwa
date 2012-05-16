/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace tokenizer {

    class DebugTextStream : public Stream {
    protected:
      std::ostream &_out;
      bool _new_sentence;
      unsigned int _colour;

    public:
      DebugTextStream(std::ostream &out) : _out(out), _new_sentence(true), _colour(0) { }
      virtual ~DebugTextStream(void) { }

      virtual void add(Type, const char *raw, offset_type, offset_type len, const char *norm = 0) {
        const static int NCOLOURS = 7;
        const static char *const COLOURS[] = {
          "\033[0;31m", "\033[0;32m", "\033[1;33m", "\033[1;34m",
          "\033[0;35m", "\033[0;36m", "\033[1;37m"
        };
        const static char *const OFF = "\033[0m";

        if (!_new_sentence)
          _out << ' ';
        _new_sentence = false;

        if (norm)
          _out << COLOURS[_colour++ % NCOLOURS] << norm << OFF;
        else
          (_out << COLOURS[_colour++ % NCOLOURS]).write(raw, len) << OFF;
      }

      virtual void error(const char *raw, offset_type, offset_type len) {
        if (!_new_sentence)
          _out << ' ';
        (_out << "<error>").write(raw, len) << "</error>";
      }

      virtual void begin_sentence(void) {
        _out << "<s>";
        _new_sentence = true;
        _colour = 0;
      }
      virtual void end_sentence(void) {
        _out << "</s>\n";
      }

      virtual void begin_list(void) {
        _out << "<ul>\n";
      }

      virtual void end_list(void) {
        _out << "</ul>\n";
      }

      virtual void begin_item(void) {
        _out << "<li>\n";
      }

      virtual void end_item(void) {
        _out << "</li>\n";
      }

      virtual void begin_paragraph(void) {
        _out << "<p>\n";
      }

      virtual void end_paragraph(void) {
        _out << "</p>\n";
      }

      virtual void begin_heading(int depth) {
        _out << "<h" << depth << ">\n";
      }
      virtual void end_heading(int depth) {
        _out << "</h" << depth << ">\n";
      }

      virtual void begin_document(void) {
        _out << "<doc>\n";
      }

      virtual void end_document(void) {
        _out << "</doc>\n";
      }
    };

  }
}
