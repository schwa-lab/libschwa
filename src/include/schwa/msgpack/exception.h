/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_EXCEPTION_H_
#define SCHWA_MSGPACK_EXCEPTION_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>

namespace schwa {
  namespace msgpack {

    class ReadException : public IOException {
    public:
      const std::string local_msg;
      const int read;
      const int expected;

      ReadException(const std::string &msg, const int read, const int expected=-1);
      ReadException(const ReadException &o);
      virtual ~ReadException(void) throw() { }

      virtual const char* what(void) const throw() override;
    };

  }
}

#endif  // namespace SCHWA_MSGPACK_EXCEPTION_H_
