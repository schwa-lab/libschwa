bin_PROGRAMS = dr-dist
dr_dist_CPPFLAGS = -I$(srcdir)/../../lib -I$(srcdir)/.. $(ZMQLIB_CFLAGS)
dr_dist_CXXFLAGS = -W -Wall -Wextra -Werror -pedantic
dr_dist_LDADD = ../../lib/libschwa.la $(ZMQLIB_LIBS)
dr_dist_SOURCES = main.cc main.h
