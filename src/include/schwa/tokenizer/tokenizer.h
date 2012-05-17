/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace io {
    class Source;
  }

  namespace tokenizer {

    class Stream;

    static const int ERROR_SKIP = 0;
    static const int ERROR_CALL = 1;
    static const int ERROR_THROW = 2;

    static const size_t BUFFER_SIZE = 20*1024;

    class Tokenizer {
    protected:
      struct State;

      bool _tokenize(Stream &dest, State &s, const char *&n1, const char *&n2, const char *p, const char *pe, const char *eof, int errors=ERROR_SKIP) const;

      void _token(Type type, Stream &dest, State &state, const char *norm=0) const;
      void _word(Type type, Stream &dest, State &state, const char *norm=0) const;
      void _punct(Type type, Stream &dest, State &state, const char *norm=0) const;
      void _end(Type type, Stream &dest, State &state, const char *norm=0) const;

      void _split(Type type1, Type type2, Stream &dest, State &state, const char *norm1=0, const char *norm2=0) const;

      void _terminator(Stream &dest, State &state, const char *norm=0) const;

      void _error(Stream &dest, State &state) const;

      void _single_quote(Stream &dest, State &state, const char *eof) const;
      void _double_quote(Stream &dest, State &state, const char *eof) const;

      void _open_single_quote(Stream &dest, State &state) const;
      void _close_single_quote(Stream &dest, State &state) const;
      void _open_double_quote(Stream &dest, State &state) const;
      void _close_double_quote(Stream &dest, State &state) const;

      void _sep_text_paragraph(Stream &dest, State &state) const;
      void _sep_html_paragraph(Stream &dest, State &state) const;
      void _begin_html_paragraph(Stream &dest, State &state) const;
      void _end_html_paragraph(Stream &dest, State &state) const;

      void _begin_html_heading(Stream &dest, State &state) const;
      void _end_html_heading(Stream &dest, State &state) const;

      void _begin_html_list(Stream &dest, State &state) const;
      void _end_html_list(Stream &dest, State &state) const;
      void _begin_html_item(Stream &dest, State &state) const;
      void _end_html_item(Stream &dest, State &state) const;

      void _dash_or_item(Stream &dest, State &state) const;
      void _number_or_item(Stream &dest, State &state) const;

      bool _die(std::ostream &msg) const;

    public:
      bool tokenize(Stream &dest, const char *data, int errors=ERROR_SKIP) const;
      bool tokenize(Stream &dest, const char *data, offset_type len, int errors=ERROR_SKIP) const;
      bool tokenize(Stream &dest, const std::string &data, int errors=ERROR_SKIP) const;
      bool tokenize(Stream &dest, io::Source &src, size_t buffer_size=BUFFER_SIZE, int errors=ERROR_SKIP) const;

      bool tokenize_stream(Stream &dest, std::istream &in, size_t buffer_size=BUFFER_SIZE, int errors=ERROR_SKIP) const;
      bool tokenize_mmap(Stream &dest, const std::string &filename, int errors=ERROR_SKIP) const;
    };

  }
}
