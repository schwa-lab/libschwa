/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_EXCEPTION_H_
#define SCHWA_DR_EXCEPTION_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>

namespace schwa {
  namespace dr {

    class ReaderException : public Exception {
    public:
      ReaderException(const std::string &msg) : Exception(msg) { }
      ReaderException(const Exception &e) : Exception(e) { }
      virtual ~ReaderException(void) throw() { }
    };

  }
}

#endif  // SCHWA_DR_EXCEPTION_H_
