/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MSGPACK_EXCEPTION_H_
#define SCHWA_MSGPACK_EXCEPTION_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>


namespace schwa {
  namespace msgpack {

    class ReadError : public IOException {
    protected:
      const int _read;
      const int _expected;

    public:
      explicit ReadError(const std::string &msg);
      ReadError(const std::string &msg, const int read, const int expected=-1);
      ReadError(const ReadError &o);
      virtual ~ReadError(void) throw() { }

      inline int expected(void) const { return _expected; }
      inline int read(void) const { return _read; }
    };


    class WriteError : public IOException {
    public:
      explicit WriteError(const std::string &msg);
      WriteError(const WriteError &o);
      virtual ~WriteError(void) throw() { }
    };

  }
}

#endif  // namespace SCHWA_MSGPACK_EXCEPTION_H_
