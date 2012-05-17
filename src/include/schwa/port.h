/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace port {

    extern const char DIR_SEP;
    extern const int BIND_FLAGS;
    extern const int SOCK_FLAGS;

    extern const char *BOLD;
    extern const char *RED;
    extern const char *OFF;

    struct Time {
      int secs;
      int usecs;
    };

    void make_directory(const std::string &dir);
    void gettimes(Time &usr, Time &sys, Time &total);
    unsigned long get_usage(void);
    void setup_fpu(void);
  }
}
