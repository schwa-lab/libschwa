/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_EXCEPTION_H_
#define SCHWA_FORMATS_EXCEPTION_H_

#include <schwa/_base.h>
#include <schwa/exception.h>


namespace schwa {
  namespace formats {

    /**
     * Exception subclass to indicate an error during the parsing of a format.
     **/
    class ParseError : public Exception {
    public:
      explicit ParseError(const std::string &msg);
      ParseError(const ParseError &other);
      virtual ~ParseError(void) throw();
    };

  }
}

#endif  // SCHWA_FORMATS_EXCEPTION_H_
