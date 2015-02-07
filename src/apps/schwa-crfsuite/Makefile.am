bin_PROGRAMS = schwa-crfsuite
schwa_crfsuite_CPPFLAGS = $(SIMD_FLAGS) -I$(srcdir)/../../lib $(LIBSCHWA_BASE_CPPFLAGS) -Wno-format-nonliteral
schwa_crfsuite_LDADD = ../../lib/libschwa.la
schwa_crfsuite_SOURCES = dump.cc iwa.cc iwa.h learn.cc main.cc option.cc option.h readdata.h reader.cc tag.cc
