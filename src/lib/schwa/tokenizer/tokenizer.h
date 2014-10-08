/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TOKENIZER_TOKENIZER_H_
#define SCHWA_TOKENIZER_TOKENIZER_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/tokenizer/common.h>


namespace schwa {
  namespace io {
    class Source;
  }

  namespace tokenizer {

    class Stream;


    class Tokenizer {
    protected:
      struct State;

      bool _tokenize(Stream &dest, State &s, const uint8_t *&n1, const uint8_t *&n2, const uint8_t *p, const uint8_t *pe, const uint8_t *eof, OnError onerror=OnError::SKIP) const;

      void _token(Type type, Stream &dest, State &state, const uint8_t *norm=nullptr) const;
      void _word(Type type, Stream &dest, State &state, const uint8_t *norm=nullptr) const;
      void _punct(Type type, Stream &dest, State &state, const uint8_t *norm=nullptr) const;
      void _end(Type type, Stream &dest, State &state, const uint8_t *norm=nullptr) const;

      void _split(Type type1, Type type2, Stream &dest, State &state, const uint8_t *norm1=nullptr, const uint8_t *norm2=nullptr) const;

      void _terminator(Stream &dest, State &state, const uint8_t *norm=nullptr) const;

      void _error(Stream &dest, State &state) const;

      void _single_quote(Stream &dest, State &state, const uint8_t *eof) const;
      void _double_quote(Stream &dest, State &state, const uint8_t *eof) const;

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

    public:
      bool tokenize(Stream &dest, const uint8_t *data, OnError onerror=OnError::SKIP) const;
      bool tokenize(Stream &dest, const uint8_t *data, size_t len, OnError onerror=OnError::SKIP) const;
      bool tokenize(Stream &dest, const std::string &data, OnError onerror=OnError::SKIP) const;
      bool tokenize(Stream &dest, io::Source &src, size_t buffer_size=DEFAULT_BUFFER_SIZE, OnError onerror=OnError::SKIP) const;

      bool tokenize_stream(Stream &dest, std::istream &in, size_t buffer_size=DEFAULT_BUFFER_SIZE, OnError onerror=OnError::SKIP) const;
      bool tokenize_mmap(Stream &dest, const std::string &filename, OnError onerror=OnError::SKIP) const;
    };

  }
}

#include <schwa/tokenizer/tokenizer_state.h>

#endif  // SCHWA_TOKENIZER_TOKENIZER_H_
