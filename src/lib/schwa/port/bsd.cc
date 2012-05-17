/* -*- Mode: C++; indent-tabs-mode: nil -*- */

#include <schwa/std.h>
#include <schwa/port.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <unistd.h>


namespace schwa { namespace port {

const int SOCK_FLAGS = SO_NOSIGPIPE;
const int BIND_FLAGS = SO_REUSEADDR;

static const int KILOBYTE = 1024;

unsigned long
get_usage(void) {
  // rusage actually works on BSD/Mac OS X
  rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  return usage.ru_maxrss * KILOBYTE;
}


void
setup_fpu(void) { }


} }
