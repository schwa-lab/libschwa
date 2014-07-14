/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_HTML_H_
#define SCHWA_FORMATS_HTML_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>


namespace schwa {
  class EncodingResult;

  namespace formats {

    class HTMLLexer {
    public:
      static const size_t DEFAULT_BUFFER_SIZE;

    protected:
      bool _enable_debug;

      virtual void _debug(const std::string &rule, const uint8_t *ts, const uint8_t *te);
      bool _run(std::istream &input, size_t buffer_size=DEFAULT_BUFFER_SIZE);
      bool _run(const uint8_t *input, size_t nbytes);

    public:
      explicit HTMLLexer(bool enable_debug=false);
      virtual ~HTMLLexer(void);

      virtual bool run(std::istream &input, size_t buffer_size=DEFAULT_BUFFER_SIZE);
      virtual bool run(const uint8_t *input, size_t nbytes);
      virtual bool run(const EncodingResult &er);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(HTMLLexer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_HTML_H_
