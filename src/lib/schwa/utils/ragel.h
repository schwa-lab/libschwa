/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_RAGEL_H_
#define SCHWA_UTILS_RAGEL_H_

#include <schwa/_base.h>


namespace schwa {

  template <typename IT=const uint8_t *>
  class RagelState {
  public:
    IT p;
    IT pe;
    IT eof;
    IT ts;
    IT te;
    int act;
    int cs;

    RagelState(void) { }
    RagelState(IT start, IT end) : p(start), pe(end), eof(pe) { }
    ~RagelState(void) { }

    inline bool
    at_eof(void) const {
      return p == pe;
    }

    void
    reset(IT start, IT end) {
      p = start;
      pe = end;
      eof = pe;
    }

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(RagelState);
  };

}  // namespace schwa

#endif  // SCHWA_UTILS_RAGEL_H_
