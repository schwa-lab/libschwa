/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/std.h>
#include <schwa/exception.h>
#include <schwa/port.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>


namespace schwa { namespace port {

const char DIR_SEP = '/';

void
gettimes(Time &usr, Time &sys, Time &total) {
  rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  usr.secs = usage.ru_utime.tv_sec;
  usr.usecs = usage.ru_utime.tv_usec;
  sys.secs = usage.ru_stime.tv_sec;
  sys.usecs = usage.ru_stime.tv_usec;

  total.secs = usr.secs + sys.secs;
  total.usecs = usr.usecs + sys.usecs;
}


void
make_directory(const std::string &dir) {
  if (mkdir(dir.c_str(), 0755)) {
    if (errno == EEXIST)
      std::cerr << "using existing directory " << dir << std::endl;
    else
      throw schwa::IOException("could not create directory", dir);
  }
}

} }
