# =============================================================================
# Automake
# =============================================================================
ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = $(SIMD_FLAGS)

lib_LTLIBRARIES = libschwa.la
noinst_LTLIBRARIES = libschwa_thirdparty_cityhash.la libschwa_thirdparty_re2.la
nobase_library_include_HEADERS =

check_LIBRARIES =
check_PROGRAMS =

libschwa_la_LDFLAGS = -no-undefined -version-info $(libschwa_la_version_info)
libschwa_thirdparty_cityhash_la_LDFLAGS = -no-undefined -version-info 4:0:0
libschwa_thirdparty_re2_la_LDFLAGS = -no-undefined -version-info 1:0:0

# =============================================================================
# Ragel
# =============================================================================
schwa/dr/query_gen.cc: $(srcdir)/../ragel/dr-query/language.rl
	ragel -C -G2 -o $@ $<

schwa/formats/html_gen.cc: $(srcdir)/../ragel/formats/html/main.rl $(srcdir)/../ragel/formats/html/html.rl
	ragel -C -G2 -o $@ $<

schwa/formats/html_charset_sniff_gen.cc: $(srcdir)/../ragel/formats/html/charset_sniff_main.rl $(srcdir)/../ragel/formats/html/charset_sniff.rl
	ragel -C -G2 -o $@ $<

schwa/formats/html_charset_sniff_content_type_gen.cc: $(srcdir)/../ragel/formats/html/content_type_main.rl
	ragel -C -G2 -o $@ $<

schwa/formats/http_gen.cc: $(srcdir)/../ragel/formats/http/main.rl $(srcdir)/../ragel/formats/http/http.rl $(srcdir)/../ragel/formats/uri/uri.rl
	ragel -C -G2 -o $@ $<

schwa/formats/warc_gen.cc: $(srcdir)/../ragel/formats/warc/warc.rl
	ragel -C -G2 -o $@ $<

schwa/formats/uri_gen.cc: $(srcdir)/../ragel/formats/uri/main.rl $(srcdir)/../ragel/formats/uri/uri.rl
	ragel -C -G2 -o $@ $<

schwa/tokenizer/tokenizer_gen.cc: $(srcdir)/../ragel/tokenizer/tokenizer.rl $(srcdir)/../ragel/tokenizer/rules/*.rl
	ragel -C -G2 -o $@ $<

# =============================================================================
# Scafolding
# =============================================================================
libschwa_la_LIBADD =
libschwa_la_SOURCES =

LIBSCHWA_HEADER_FILES = \
		schwa/_base.h \
		schwa/config/base.h \
		schwa/config/group.h \
		schwa/config/main.h \
		schwa/config/main_impl.h \
		schwa/config/op.h \
		schwa/config/op_impl.h \
		schwa/config/serialisation.h \
		schwa/config/serialisation_impl.h \
		schwa/config.h \
		schwa/containers/block_vector.h \
		schwa/containers/block_vector_impl.h \
		schwa/containers.h \
		schwa/dr/config.h \
		schwa/dr/exception.h \
		schwa/dr/field_defs.h \
		schwa/dr/field_defs_impl.h \
		schwa/dr/fields.h \
		schwa/dr/helpers.h \
		schwa/dr/istore.h \
		schwa/dr/query.h \
		schwa/dr/reader.h \
		schwa/dr/runtime.h \
		schwa/dr/schema.h \
		schwa/dr/type_info.h \
		schwa/dr/wire.h \
		schwa/dr/writer.h \
		schwa/dr.h \
		schwa/dr-dist/helpers.h \
		schwa/dr-dist/server.h \
		schwa/dr-dist/worker_main.h \
		schwa/encoding.h \
		schwa/encoding_gen.h \
		schwa/enums.h \
		schwa/exception.h \
		schwa/formats/html.h \
		schwa/formats/http.h \
		schwa/formats/warc.h \
		schwa/formats/uri.h \
		schwa/io/array_reader.h \
		schwa/io/enums.h \
		schwa/io/file_source.h \
		schwa/io/istream_source.h \
		schwa/io/logging.h \
		schwa/io/mmapped_source.h \
		schwa/io/paths.h \
		schwa/io/source.h \
		schwa/io/streams.h \
		schwa/io/traits.h \
		schwa/io/unsafe_array_writer.h \
		schwa/io/write_buffer.h \
		schwa/learn.h \
		schwa/learn/extract.h \
		schwa/learn/extract_impl.h \
		schwa/learn/feature_transformers.h \
		schwa/learn/features.h \
		schwa/learn/features_impl.h \
		schwa/macros.h \
		schwa/mpl/if.h \
		schwa/msgpack/dynamic.h \
		schwa/msgpack/dynamic_impl.h \
		schwa/msgpack/enums.h \
		schwa/msgpack/exception.h \
		schwa/msgpack/wire.h \
		schwa/msgpack/wire_impl.h \
		schwa/msgpack.h \
		schwa/pool.h \
		schwa/port.h \
		schwa/tokenizer/common.h \
		schwa/tokenizer/debug_text_stream.h \
		schwa/tokenizer/docrep_stream.h \
		schwa/tokenizer/exception.h \
		schwa/tokenizer/stream.h \
		schwa/tokenizer/text_stream.h \
		schwa/tokenizer/tokenizer.h \
		schwa/tokenizer/tokenizer_state.h \
		schwa/tokenizer.h \
		schwa/unicode.h \
		schwa/unicode_gen.h \
		schwa/utils/counter.h \
		schwa/utils/enums.h \
		schwa/utils/hash.h \
		schwa/utils/shlex.h \
		schwa/utils/string.h \
		schwa/version.h

LIBSCHWA_SOURCE_FILES = \
		schwa/config/base.cc \
		schwa/config/group.cc \
		schwa/config/main.cc \
		schwa/config/op.cc \
		schwa/config/serialisation.cc \
		schwa/dr/config.cc \
		schwa/dr/field_defs.cc \
		schwa/dr/query.cc \
		schwa/dr/query_gen.cc \
		schwa/dr/reader.cc \
		schwa/dr/runtime.cc \
		schwa/dr/schema.cc \
		schwa/dr/type_info.cc \
		schwa/dr/writer.cc \
		schwa/encoding.cc \
		schwa/exception.cc \
		schwa/formats/html.cc \
		schwa/formats/html_gen.cc \
		schwa/formats/html_charset_sniff_gen.cc \
		schwa/formats/html_charset_sniff_content_type_gen.cc \
		schwa/formats/html_named_character_references_gen.cc \
		schwa/formats/http.cc \
		schwa/formats/http_gen.cc \
		schwa/formats/warc.cc \
		schwa/formats/warc_gen.cc \
		schwa/formats/uri.cc \
		schwa/formats/uri_gen.cc \
		schwa/io/array_reader.cc \
		schwa/io/enums.cc \
		schwa/io/file_source.cc \
		schwa/io/istream_source.cc \
		schwa/io/logging.cc \
		schwa/io/mmapped_source.cc \
		schwa/io/paths.cc \
		schwa/io/streams.cc \
		schwa/io/write_buffer.cc \
		schwa/learn/extract.cc \
		schwa/learn/features.cc \
		schwa/msgpack/dynamic.cc \
		schwa/msgpack/exception.cc \
		schwa/port.cc \
		schwa/tokenizer/debug_text_stream.cc \
		schwa/tokenizer/docrep_stream.cc \
		schwa/tokenizer/text_stream.cc \
		schwa/tokenizer/tokenizer.cc \
		schwa/tokenizer/tokenizer_gen.cc \
		schwa/unicode.cc \
		schwa/unicode_gen.cc \
		schwa/utils/shlex.cc \
		schwa/version.cc


if HAVE_LIBZMQ
noinst_LTLIBRARIES += libschwa_drdist.la
libschwa_drdist_la_CPPFLAGS = $(LIBSCHWA_BASE_CPPFLAGS) $(ZMQLIB_CFLAGS)
libschwa_drdist_la_LIBADD = $(ZMQLIB_LIBS)
libschwa_drdist_la_SOURCES = \
		schwa/dr-dist/helpers.cc \
		schwa/dr-dist/helpers.h \
		schwa/dr-dist/server.cc \
		schwa/dr-dist/server.h

libschwa_la_LIBADD += libschwa_drdist.la
endif


# =============================================================================
# libschwa Automake
# =============================================================================
libschwa_la_CPPFLAGS = $(LIBSCHWA_BASE_CPPFLAGS)
libschwa_la_LIBADD += $(PROFILERLIB) $(TCMALLOCLIB) libschwa_thirdparty_cityhash.la libschwa_thirdparty_re2.la
libschwa_la_SOURCES += $(LIBSCHWA_HEADER_FILES) $(LIBSCHWA_SOURCE_FILES)

library_includedir = $(includedir)
nobase_library_include_HEADERS += $(LIBSCHWA_HEADER_FILES)

check_LIBRARIES += libschwa_unittest.a
check_PROGRAMS += schwa_tests schwa_containers_tests schwa_dr_tests schwa_io_tests schwa_learn_tests schwa_mpl_tests schwa_msgpack_tests schwa_utils_tests

LIBSCHWA_TEST_CPPFLAGS = $(LIBSCHWA_BASE_CPPFLAGS) -isystem $(srcdir)/../third-party/unittest-cpp/UnitTest++
LIBSCHWA_TEST_LDADD = ../third-party/unittest-cpp/UnitTest++/libUnitTest++.la libschwa_unittest.a libschwa.la

libschwa_unittest_a_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
libschwa_unittest_a_SOURCES = \
		schwa/unittest.h \
		schwa/unittest.cc

schwa_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/pool_test.cc  \
		schwa/port_test.cc  \
		schwa/unicode_test.cc

schwa_containers_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_containers_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_containers_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/containers/block_vector_test.cc

schwa_dr_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_dr_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_dr_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/dr/fields_test.cc  \
		schwa/dr/helpers_test.cc  \
		schwa/dr/lazy_test.cc  \
		schwa/dr/pointers_test.cc  \
		schwa/dr/reader_test.cc  \
		schwa/dr/self_pointer_test.cc  \
		schwa/dr/slices_test.cc  \
		schwa/dr/writer_test.cc

schwa_io_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_io_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_io_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/io/mmapped_source_test.cc  \
		schwa/io/paths_test.cc  \
		schwa/io/write_buffer_test.cc

schwa_learn_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_learn_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_learn_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/learn/extract_test.cc \
		schwa/learn/feature_transformers_test.cc \
		schwa/learn/features_test.cc

schwa_mpl_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_mpl_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_mpl_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/mpl/if_test.cc

schwa_msgpack_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_msgpack_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_msgpack_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/msgpack/wire_test.cc

schwa_utils_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_utils_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_utils_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/utils/shlex_test.cc


# =============================================================================
# libschwa_thirdparty_cityhash Automake
# =============================================================================
CITYHASH_HEADER_FILES = \
		schwa/third-party/cityhash/city.h

CITYHASH_SOURCE_FILES = \
		schwa/third-party/cityhash/city.cc

libschwa_thirdparty_cityhash_la_SOURCES = $(CITYHASH_HEADER_FILES) $(CITYHASH_SOURCE_FILES)
nobase_library_include_HEADERS += $(CITYHASH_HEADER_FILES)

check_PROGRAMS += cityhash_tests

cityhash_tests_LDADD = libschwa_thirdparty_cityhash.la
cityhash_tests_SOURCES = schwa/third-party/cityhash/city_test.cc

# =============================================================================
# libschwa_thirdparty_re2 Automake
# =============================================================================
RE2_HEADER_FILES = \
		schwa/third-party/re2/filtered_re2.h \
		schwa/third-party/re2/prefilter.h \
		schwa/third-party/re2/prefilter_tree.h \
		schwa/third-party/re2/prog.h \
		schwa/third-party/re2/re2.h \
		schwa/third-party/re2/regexp.h \
		schwa/third-party/re2/set.h \
		schwa/third-party/re2/stringpiece.h \
		schwa/third-party/re2/testing/exhaustive_tester.h \
		schwa/third-party/re2/testing/regexp_generator.h \
		schwa/third-party/re2/testing/string_generator.h \
		schwa/third-party/re2/testing/tester.h \
		schwa/third-party/re2/unicode_casefold.h \
		schwa/third-party/re2/unicode_groups.h \
		schwa/third-party/re2/util/arena.h \
		schwa/third-party/re2/util/atomicops.h \
		schwa/third-party/re2/util/benchmark.h \
		schwa/third-party/re2/util/flags.h \
		schwa/third-party/re2/util/logging.h \
		schwa/third-party/re2/util/mutex.h \
		schwa/third-party/re2/util/pcre.h \
		schwa/third-party/re2/util/random.h \
		schwa/third-party/re2/util/sparse_array.h \
		schwa/third-party/re2/util/sparse_set.h \
		schwa/third-party/re2/util/test.h \
		schwa/third-party/re2/util/thread.h \
		schwa/third-party/re2/util/utf.h \
		schwa/third-party/re2/util/util.h \
		schwa/third-party/re2/util/valgrind.h \
		schwa/third-party/re2/variadic_function.h \
		schwa/third-party/re2/walker-inl.h

RE2_SOURCE_FILES = \
		schwa/third-party/re2/bitstate.cc \
		schwa/third-party/re2/compile.cc \
		schwa/third-party/re2/dfa.cc \
		schwa/third-party/re2/filtered_re2.cc \
		schwa/third-party/re2/mimics_pcre.cc \
		schwa/third-party/re2/nfa.cc \
		schwa/third-party/re2/onepass.cc \
		schwa/third-party/re2/parse.cc \
		schwa/third-party/re2/perl_groups.cc \
		schwa/third-party/re2/prefilter.cc \
		schwa/third-party/re2/prefilter_tree.cc \
		schwa/third-party/re2/prog.cc \
		schwa/third-party/re2/re2.cc \
		schwa/third-party/re2/regexp.cc \
		schwa/third-party/re2/set.cc \
		schwa/third-party/re2/simplify.cc \
		schwa/third-party/re2/tostring.cc \
		schwa/third-party/re2/unicode_casefold.cc \
		schwa/third-party/re2/unicode_groups.cc \
		schwa/third-party/re2/util/arena.cc \
		schwa/third-party/re2/util/hash.cc \
		schwa/third-party/re2/util/rune.cc \
		schwa/third-party/re2/util/stringpiece.cc \
		schwa/third-party/re2/util/stringprintf.cc \
		schwa/third-party/re2/util/strutil.cc \
		schwa/third-party/re2/util/valgrind.cc

libschwa_thirdparty_re2_la_CPPFLAGS = $(PTHREAD_CFLAGS)
libschwa_thirdparty_re2_la_LIBADD = $(PTHREAD_CFLAGS) $(PTHREAD_LIBS)
libschwa_thirdparty_re2_la_SOURCES = $(RE2_HEADER_FILES) $(RE2_SOURCE_FILES)
nobase_library_include_HEADERS += $(RE2_HEADER_FILES)

LIBRE2_TEST_CPPFLAGS = $(PTHREAD_CFLAGS)
LIBRE2_TEST_LDADD = $(PTHREAD_CFLAGS) $(PTHREAD_LIBS) libschwa_thirdparty_re2_unittest.a libschwa_thirdparty_re2.la

check_LIBRARIES += libschwa_thirdparty_re2_unittest.a
check_PROGRAMS += libre2_charclass_test libre2_compile_test libre2_filtered_re2_test libre2_mimics_pcre_test libre2_parse_test libre2_possible_match_test libre2_re2_test libre2_re2_arg_test libre2_regexp_test libre2_required_prefix_test libre2_search_test libre2_set_test libre2_simplify_test libre2_string_generator_test

libschwa_thirdparty_re2_unittest_a_SOURCES = \
		schwa/third-party/re2/testing/backtrack.cc \
		schwa/third-party/re2/testing/dump.cc \
		schwa/third-party/re2/testing/exhaustive_tester.cc \
		schwa/third-party/re2/testing/null_walker.cc \
		schwa/third-party/re2/testing/regexp_generator.cc \
		schwa/third-party/re2/testing/string_generator.cc \
		schwa/third-party/re2/testing/tester.cc \
		schwa/third-party/re2/util/pcre.cc \
		schwa/third-party/re2/util/random.cc \
		schwa/third-party/re2/util/thread.cc

libre2_charclass_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_charclass_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_charclass_test_SOURCES = schwa/third-party/re2/testing/charclass_test.cc schwa/third-party/re2/util/test.cc

libre2_compile_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_compile_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_compile_test_SOURCES = schwa/third-party/re2/testing/compile_test.cc schwa/third-party/re2/util/test.cc

libre2_filtered_re2_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_filtered_re2_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_filtered_re2_test_SOURCES = schwa/third-party/re2/testing/filtered_re2_test.cc schwa/third-party/re2/util/test.cc

libre2_mimics_pcre_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_mimics_pcre_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_mimics_pcre_test_SOURCES = schwa/third-party/re2/testing/mimics_pcre_test.cc schwa/third-party/re2/util/test.cc

libre2_parse_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_parse_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_parse_test_SOURCES = schwa/third-party/re2/testing/parse_test.cc schwa/third-party/re2/util/test.cc

libre2_possible_match_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_possible_match_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_possible_match_test_SOURCES = schwa/third-party/re2/testing/possible_match_test.cc schwa/third-party/re2/util/test.cc

libre2_re2_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_re2_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_re2_test_SOURCES = schwa/third-party/re2/testing/re2_test.cc schwa/third-party/re2/util/test.cc

libre2_re2_arg_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_re2_arg_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_re2_arg_test_SOURCES = schwa/third-party/re2/testing/re2_arg_test.cc schwa/third-party/re2/util/test.cc

libre2_regexp_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_regexp_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_regexp_test_SOURCES = schwa/third-party/re2/testing/regexp_test.cc schwa/third-party/re2/util/test.cc

libre2_required_prefix_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_required_prefix_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_required_prefix_test_SOURCES = schwa/third-party/re2/testing/required_prefix_test.cc schwa/third-party/re2/util/test.cc

libre2_search_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_search_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_search_test_SOURCES = schwa/third-party/re2/testing/search_test.cc schwa/third-party/re2/util/test.cc

libre2_set_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_set_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_set_test_SOURCES = schwa/third-party/re2/testing/set_test.cc schwa/third-party/re2/util/test.cc

libre2_simplify_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_simplify_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_simplify_test_SOURCES = schwa/third-party/re2/testing/simplify_test.cc schwa/third-party/re2/util/test.cc

libre2_string_generator_test_CPPFLAGS = $(LIBRE2_TEST_CPPFLAGS)
libre2_string_generator_test_LDADD = $(LIBRE2_TEST_LDADD)
libre2_string_generator_test_SOURCES = schwa/third-party/re2/testing/string_generator_test.cc schwa/third-party/re2/util/test.cc


TESTS = $(check_PROGRAMS)
