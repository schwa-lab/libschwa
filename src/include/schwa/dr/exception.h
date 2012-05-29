/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class DependencyException : public Exception {
    public:
      DependencyException(const std::string &msg) : Exception(msg) { }
      DependencyException(const DependencyException &o) : Exception(o) { }
      virtual ~DependencyException(void) throw() { }
    };

  }
}
