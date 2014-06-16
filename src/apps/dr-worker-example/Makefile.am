noinst_PROGRAMS = dr-worker-example
dr_worker_example_CPPFLAGS = $(SIMD_FLAGS) -I$(srcdir)/../../lib $(LIBSCHWA_BASE_CPPFLAGS) $(ZMQLIB_CFLAGS)
dr_worker_example_LDADD = ../../lib/libschwa.la $(ZMQLIB_LIBS)
dr_worker_example_SOURCES = main.cc
