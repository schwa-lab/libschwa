/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_RAGEL_H_
#define SCHWA_UTILS_RAGEL_H_

#include <schwa/_base.h>
#include <schwa/exception.h>


namespace schwa {

  template <typename IT=const uint8_t *>
  class RagelState {
  public:
    using iterator = IT;

    IT p;
    IT pe;
    IT eof;
    IT ts;
    IT te;
    int act;
    int cs;

    RagelState(void) { }
    RagelState(iterator start, iterator end) : p(start), pe(end), eof(pe) { }
    ~RagelState(void) { }

    inline bool
    at_eof(void) const {
      return p == pe;
    }

    void
    reset(iterator start, iterator end) {
      p = start;
      pe = end;
      eof = pe;
    }

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(RagelState);
  };


  class RagelException : public Exception {
  public:
    explicit RagelException(const std::string &msg) : Exception(msg) { }
    RagelException(const RagelException &o) : Exception(o) { }
    virtual ~RagelException(void) throw() { }
  };

}  // namespace schwa

#endif  // SCHWA_UTILS_RAGEL_H_
