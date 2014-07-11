/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_HTML_H_
#define SCHWA_FORMATS_HTML_H_

#include <iosfwd>

#include <schwa/_base.h>


namespace schwa {
  namespace formats {

    class HTMLLexer {
    public:
      static const size_t DEFAULT_BUFFER_SIZE;

    public:
      HTMLLexer(void) { }

      bool run(std::istream &input, size_t buffer_size=DEFAULT_BUFFER_SIZE);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(HTMLLexer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_HTML_H_
