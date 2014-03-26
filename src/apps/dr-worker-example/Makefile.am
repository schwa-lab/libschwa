noinst_PROGRAMS = dr-worker-example
dr_worker_example_CPPFLAGS = -I$(srcdir)/../../lib -I$(srcdir)/.. $(ZMQLIB_CFLAGS)
dr_worker_example_CXXFLAGS = $(LIBSCHWA_BASE_CXXFLAGS)
dr_worker_example_LDADD = ../../lib/libschwa.la $(ZMQLIB_LIBS)
dr_worker_example_SOURCES = main.cc
