/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_EXCEPTION_H_
#define SCHWA_MSGPACK_EXCEPTION_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>


namespace schwa {
  namespace msgpack {

    class ReadException : public IOException {
    protected:
      const int _read;
      const int _expected;

    public:
      explicit ReadException(const std::string &msg);
      ReadException(const std::string &msg, const int read, const int expected=-1);
      ReadException(const ReadException &o);
      virtual ~ReadException(void) throw() { }

      inline int expected(void) const { return _expected; }
      inline int read(void) const { return _read; }
    };

  }
}

#endif  // namespace SCHWA_MSGPACK_EXCEPTION_H_
