noinst_LIBRARIES = libre2.a

libre2_a_CXXFLAGS = -Wall -O3 -pthread -Wno-sign-compare
libre2_a_LIBADD = $(PROFILERLIB) $(TCMALLOCLIB)
libre2_a_SOURCES = \
		re2/bitstate.cc \
		re2/compile.cc \
		re2/dfa.cc \
		re2/filtered_re2.cc \
		re2/filtered_re2.h \
		re2/filtered_re2.h \
		re2/mimics_pcre.cc \
		re2/nfa.cc \
		re2/onepass.cc \
		re2/parse.cc \
		re2/perl_groups.cc \
		re2/prefilter.cc \
		re2/prefilter.h \
		re2/prefilter_tree.cc \
		re2/prefilter_tree.h \
		re2/prog.cc \
		re2/prog.h \
		re2/re2.cc \
		re2/re2.h \
		re2/re2.h \
		re2/regexp.cc \
		re2/regexp.h \
		re2/set.cc \
		re2/set.h \
		re2/set.h \
		re2/simplify.cc \
		re2/stringpiece.h \
		re2/stringpiece.h \
		re2/testing/exhaustive_tester.h \
		re2/testing/regexp_generator.h \
		re2/testing/string_generator.h \
		re2/testing/tester.h \
		re2/tostring.cc \
		re2/unicode_casefold.cc \
		re2/unicode_casefold.h \
		re2/unicode_groups.cc \
		re2/unicode_groups.h \
		re2/variadic_function.h \
		re2/variadic_function.h \
		re2/walker-inl.h \
		util/arena.cc \
		util/arena.h \
		util/atomicops.h \
		util/benchmark.h \
		util/flags.h \
		util/hash.cc \
		util/logging.h \
		util/mutex.h \
		util/pcre.h \
		util/random.h \
		util/rune.cc \
		util/sparse_array.h \
		util/sparse_set.h \
		util/stringpiece.cc \
		util/stringprintf.cc \
		util/strutil.cc \
		util/test.h \
		util/utf.h \
		util/util.h \
		util/valgrind.cc \
		util/valgrind.h


# =============================================================================
# Unittests
# =============================================================================
TESTS += charclass_test compile_test
#check_LIBRARIES += libre2_unittest.a
check_PROGRAMS += charclass_test compile_test

libre2_unittest_LDADD = libre2.a
libre2_unittest_SOURCES = \
		re2/testing/backtrack.cc \
		re2/testing/dump.cc \
		re2/testing/exhaustive_tester.cc \
		re2/testing/null_walker.cc \
		re2/testing/regexp_generator.cc \
		re2/testing/string_generator.cc \
		re2/testing/tester.cc
		util/pcre.cc \
		util/random.cc \
		util/test.cc \
		util/thread.cc \

charclass_test_LDADD = libre2_unittest.a
charclass_test_SOURCES = re2/testing/charclass_test.cc

compile_test_LDADD = libre2_unittest.a
compile_test_SOURCES = re2/testing/compile_test.cc
