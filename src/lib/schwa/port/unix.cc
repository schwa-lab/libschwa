/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/std.h>
#include <schwa/port.h>

#include <errno.h>
#include <fpu_control.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <unistd.h>


namespace schwa { namespace port {

const int SOCK_FLAGS = MSG_NOSIGNAL;
const int BIND_FLAGS = SO_REUSEADDR;

unsigned long
get_usage(void) {
  std::ifstream statm("/proc/self/statm");
  if (!statm)
    return 0;

  unsigned long usage = 0;
  statm >> usage;
  return usage * getpagesize();
}


void
setup_fpu(void) {
  fpu_control_t cw = 0x1372;
  _FPU_SETCW(cw);
}

} }
