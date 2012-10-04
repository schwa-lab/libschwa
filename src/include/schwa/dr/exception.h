/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class BaseDocSchema;
    class Doc;

    class ReaderException : public Exception {
    public:
      ReaderException(const std::string &msg) : Exception(msg) { }
      ReaderException(const Exception &e) : Exception(e) { }
      virtual ~ReaderException(void) throw() { }
    };

  }
}
