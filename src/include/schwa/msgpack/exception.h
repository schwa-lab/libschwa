/* -*- Mode: C++; indent-tabs-mode: nil -*- */

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

      virtual const char* what(void) const throw() { return local_msg.c_str(); }
    };

  }
}
