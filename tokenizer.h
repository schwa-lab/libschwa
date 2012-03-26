/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa { namespace token {

class Stream;

class TokenError: public std::exception {
public:
  const std::string msg;

  TokenError(const std::string &msg)
    : msg(msg){}
  TokenError(const TokenError &other)
    : msg(other.msg){}

  virtual ~TokenError(void) throw(){}
  virtual const char* what(void) const throw() { return msg.c_str(); }
};

static const int ERROR_SKIP = 0;
static const int ERROR_CALL = 1;
static const int ERROR_THROW = 2;

static const size_t BUFFER_SIZE = 20*1024;

class Tokenizer {
protected:
  struct State;

  bool tokenize_(Stream &dest, State &s, const char *&n1, const char *&n2,
                 const char *p, const char *pe, const char *eof,
                 int errors = ERROR_SKIP) const;

  void token_(Type type, Stream &dest, State &state, const char *norm = 0) const;
  void word_(Type type, Stream &dest, State &state, const char *norm = 0) const;
  void punct_(Type type, Stream &dest, State &state, const char *norm = 0) const;
  void end_(Type type, Stream &dest, State &state, const char *norm = 0) const;

  void split_(Type type1, Type type2, Stream &dest, State &state,
              const char *norm1 = 0, const char *norm2 = 0) const;

  void terminator_(Stream &dest, State &state, const char *norm = 0) const;

  void error_(Stream &dest, State &state) const;

  void single_quote_(Stream &dest, State &state, const char *eof) const;
  void double_quote_(Stream &dest, State &state, const char *eof) const;

  void open_single_quote_(Stream &dest, State &state) const;
  void close_single_quote_(Stream &dest, State &state) const;
  void open_double_quote_(Stream &dest, State &state) const;
  void close_double_quote_(Stream &dest, State &state) const;

  void sep_text_paragraph_(Stream &dest, State &state) const;
  void sep_html_paragraph_(Stream &dest, State &state) const;
  void begin_html_paragraph_(Stream &dest, State &state) const;
  void end_html_paragraph_(Stream &dest, State &state) const;

  void begin_html_heading_(Stream &dest, State &state) const;
  void end_html_heading_(Stream &dest, State &state) const;

  void begin_html_list_(Stream &dest, State &state) const;
  void end_html_list_(Stream &dest, State &state) const;
  void begin_html_item_(Stream &dest, State &state) const;
  void end_html_item_(Stream &dest, State &state) const;

  void dash_or_item_(Stream &dest, State &state) const;
  void number_or_item_(Stream &dest, State &state) const;

  bool die_(std::ostream &msg) const;

public:
  bool tokenize(Stream &dest, const char *data, int errors=ERROR_SKIP) const;
	bool tokenize(Stream &dest, const char *data, offset_type len, int errors=ERROR_SKIP) const;
  bool tokenize(Stream &dest, const std::string &data, int errors=ERROR_SKIP) const;
  bool tokenize(Stream &dest, Source &src, size_t buffer_size=BUFFER_SIZE,
                int errors=ERROR_SKIP) const;

  bool tokenize_stream(Stream &dest, std::istream &in, size_t buffer_size=BUFFER_SIZE,
                       int errors=ERROR_SKIP) const;
  bool tokenize_mmap(Stream &dest, const std::string &filename, int errors=ERROR_SKIP) const;
};

} }
