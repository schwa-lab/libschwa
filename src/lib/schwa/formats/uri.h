/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_URI_H_
#define SCHWA_FORMATS_URI_H_

#include <schwa/_base.h>


namespace schwa {
  namespace formats {

    class URILexer {
    protected:
      bool _run(const uint8_t *input, size_t nbytes);

    public:
      explicit URILexer(void);
      virtual ~URILexer(void);

      bool run(const uint8_t *input, size_t nbytes);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(URILexer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_URI_H_
