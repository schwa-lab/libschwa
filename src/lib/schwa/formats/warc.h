/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_WARC_H_
#define SCHWA_FORMATS_WARC_H_

#include <iosfwd>

#include <schwa/_base.h>
#include <schwa/formats/html.h>


namespace schwa {
  namespace formats {

    class WARCLexer {
    public:
      static const size_t DEFAULT_BUFFER_SIZE;

    private:
      HTMLLexer _html_lexer;
      size_t _html_buffer_size;
      unsigned int _nfail;
      unsigned int _nsuccess;

    public:
      explicit WARCLexer(size_t html_buffer_size=HTMLLexer::DEFAULT_BUFFER_SIZE) : _html_buffer_size(html_buffer_size) { }

      bool run(std::istream &input, size_t buffer_size=DEFAULT_BUFFER_SIZE);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(WARCLexer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_WARC_H_
