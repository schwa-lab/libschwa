ACLOCAL_AMFLAGS = -I m4

noinst_LIBRARIES = libre2.a

libre2_a_CXXFLAGS = -Wno-sign-compare $(PTHREAD_CFLAGS)
libre2_a_LIBADD = $(PTHREAD_LIBS)
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
TESTS = charclass_test compile_test filtered_re2_test mimics_pcre_test parse_test possible_match_test re2_test re2_arg_test regexp_test required_prefix_test search_test set_test simplify_test string_generator_test
check_LIBRARIES = libre2_unittest.a
check_PROGRAMS = charclass_test compile_test filtered_re2_test mimics_pcre_test parse_test possible_match_test re2_test re2_arg_test regexp_test required_prefix_test search_test set_test simplify_test string_generator_test

libre2_unittest_a_SOURCES = \
		re2/testing/backtrack.cc \
		re2/testing/dump.cc \
		re2/testing/exhaustive_tester.cc \
		re2/testing/null_walker.cc \
		re2/testing/regexp_generator.cc \
		re2/testing/string_generator.cc \
		re2/testing/tester.cc \
		util/pcre.cc \
		util/random.cc \
		util/thread.cc

charclass_test_LDADD = libre2.a libre2_unittest.a
charclass_test_SOURCES = re2/testing/charclass_test.cc util/test.cc

compile_test_LDADD = libre2.a libre2_unittest.a
compile_test_SOURCES = re2/testing/compile_test.cc util/test.cc

filtered_re2_test_LDADD = libre2.a libre2_unittest.a
filtered_re2_test_SOURCES = re2/testing/filtered_re2_test.cc util/test.cc

mimics_pcre_test_LDADD = libre2.a libre2_unittest.a
mimics_pcre_test_SOURCES = re2/testing/mimics_pcre_test.cc util/test.cc

parse_test_LDADD = libre2.a libre2_unittest.a
parse_test_SOURCES = re2/testing/parse_test.cc util/test.cc

possible_match_test_LDADD = libre2.a libre2_unittest.a
possible_match_test_SOURCES = re2/testing/possible_match_test.cc util/test.cc

re2_test_LDADD = libre2.a libre2_unittest.a
re2_test_SOURCES = re2/testing/re2_test.cc util/test.cc

re2_arg_test_LDADD = libre2.a libre2_unittest.a
re2_arg_test_SOURCES = re2/testing/re2_arg_test.cc util/test.cc

regexp_test_LDADD = libre2.a libre2_unittest.a
regexp_test_SOURCES = re2/testing/regexp_test.cc util/test.cc

required_prefix_test_LDADD = libre2.a libre2_unittest.a
required_prefix_test_SOURCES = re2/testing/required_prefix_test.cc util/test.cc

search_test_LDADD = libre2.a libre2_unittest.a
search_test_SOURCES = re2/testing/search_test.cc util/test.cc

set_test_LDADD = libre2.a libre2_unittest.a
set_test_SOURCES = re2/testing/set_test.cc util/test.cc

simplify_test_LDADD = libre2.a libre2_unittest.a
simplify_test_SOURCES = re2/testing/simplify_test.cc util/test.cc

string_generator_test_LDADD = libre2.a libre2_unittest.a
string_generator_test_SOURCES = re2/testing/string_generator_test.cc util/test.cc
