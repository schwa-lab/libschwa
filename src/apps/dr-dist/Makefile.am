bin_PROGRAMS = dr-dist
dr_dist_CPPFLAGS = $(SIMD_FLAGS) -I$(srcdir)/../../lib $(LIBSCHWA_BASE_CPPFLAGS) $(ZMQLIB_CFLAGS)
dr_dist_LDADD = ../../lib/libschwa.la $(ZMQLIB_LIBS)
dr_dist_SOURCES = main.cc
