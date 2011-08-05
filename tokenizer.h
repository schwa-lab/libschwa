/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa { namespace tokens {

class Stream;

class Tokenizer {
protected:
  struct State;
  bool tokenize_(Stream &dest, State &s, const char *&n1, const char *&n2,
                 const char *p, const char *pe, const char *eof) const;

  void token_(Type type, Stream &dest, State &state, const char *norm) const;

  void word_(Type type, Stream &dest, State &state, const char *norm = 0) const;
  void punct_(Type type, Stream &dest, State &state, const char *norm = 0) const;
  void end_punct_(Type type, Stream &dest, State &state, const char *norm = 0) const;

  void split_(Type type1, Type type2, Stream &dest, State &state,
              const char *norm1 = 0, const char *norm2 = 0) const;

  void terminator_(Stream &dest, State &state, const char *norm = 0) const;

  void single_quote_(Stream &dest, State &state) const;
  void double_quote_(Stream &dest, State &state) const;
  void open_single_quote_(Stream &dest, State &state) const;
  void close_single_quote_(Stream &dest, State &state) const;
  void open_double_quote_(Stream &dest, State &state) const;
  void close_double_quote_(Stream &dest, State &state) const;

  void sep_text_paragraph_(Stream &dest, State &state) const;
  void sep_html_paragraph_(Stream &dest, State &state) const;
  void begin_html_paragraph_(Stream &dest, State &state) const;
  void end_html_paragraph_(Stream &dest, State &state) const;

  void begin_html_list_(Stream &dest, State &state) const;
  void end_html_list_(Stream &dest, State &state) const;
  void begin_html_item_(Stream &dest, State &state) const;
  void end_html_item_(Stream &dest, State &state) const;

  void dash_or_item_(Stream &dest, State &state) const;
public:
	bool tokenize(Stream &dest, const char *data, offset_type len) const;
  bool tokenize(Stream &dest, const std::string &data) const;
  bool tokenize(Stream &dest, std::istream &in, size_t buffer_size) const;
  bool tokenize(Stream &dest, const char *filename) const;
};

} }
