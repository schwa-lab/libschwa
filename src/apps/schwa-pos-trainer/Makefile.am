bin_PROGRAMS = schwa-pos-trainer
schwa_pos_trainer_CPPFLAGS = $(SIMD_FLAGS) -I$(srcdir)/../../lib $(LIBSCHWA_BASE_CPPFLAGS)
schwa_pos_trainer_LDADD = ../../lib/libschwa.la -lcrfsuite
schwa_pos_trainer_SOURCES = main.cc
