bin_PROGRAMS = schwa-pos-tagger
schwa_pos_tagger_CPPFLAGS = $(SIMD_FLAGS) -I$(srcdir)/../../lib $(LIBSCHWA_BASE_CPPFLAGS)
schwa_pos_tagger_LDADD = ../../lib/libschwa.la -lcrfsuite
schwa_pos_tagger_SOURCES = main.cc
