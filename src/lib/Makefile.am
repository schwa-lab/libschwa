# =============================================================================
# Automake
# =============================================================================
AM_CPPFLAGS = $(SIMD_FLAGS)
AM_CXXFLAGS = $(LIBSCHWA_BASE_CXXFLAGS)

lib_LTLIBRARIES = libschwa.la

libschwa_la_LIBADD = $(PROFILERLIB) $(TCMALLOCLIB) $(srcdir)/../third-party/re2/libre2.a $(srcdir)/../third-party/cityhash/src/libcityhash.a
libschwa_la_SOURCES =


# =============================================================================
# Ragel
# =============================================================================
schwa/dr/query_gen.cc: $(srcdir)/../ragel/dr-query/language.rl
	ragel -C -G2 -o $@ $<

schwa/tokenizer/tokenizer_gen.cc: $(srcdir)/../ragel/tokenizer/tokenizer.rl $(srcdir)/../ragel/tokenizer/rules/*.rl
	ragel -C -G2 -o $@ $<

# =============================================================================
# Scafolding
# =============================================================================
LIBSCHWA_HEADER_FILES = \
		schwa/_base.h \
		schwa/config/base.h \
		schwa/config/exception.h \
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
		schwa/enums.h \
		schwa/exception.h \
		schwa/io/array_reader.h \
		schwa/io/file_source.h \
		schwa/io/istream_source.h \
		schwa/io/logging.h \
		schwa/io/logging_enums.h \
		schwa/io/mmapped_source.h \
		schwa/io/paths.h \
		schwa/io/source.h \
		schwa/io/traits.h \
		schwa/io/unsafe_array_writer.h \
		schwa/io/utils.h \
		schwa/io/write_buffer.h \
		schwa/learn/extract.h \
		schwa/learn/extract_impl.h \
		schwa/learn/feature_transformers.h \
		schwa/learn/features.h \
		schwa/learn/features_impl.h \
		schwa/macros.h \
		schwa/mpl/if.h \
		schwa/msgpack/dynamic.h \
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
		schwa/exception.cc \
		schwa/io/array_reader.cc \
		schwa/io/file_source.cc \
		schwa/io/istream_source.cc \
		schwa/io/logging.cc \
		schwa/io/mmapped_source.cc \
		schwa/io/paths.cc \
		schwa/io/utils.cc \
		schwa/io/write_buffer.cc \
		schwa/learn/extract.cc \
		schwa/learn/features.cc \
		schwa/msgpack/dynamic.cc \
		schwa/msgpack/enums.cc \
		schwa/msgpack/exception.cc \
		schwa/msgpack/wire.cc \
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
noinst_LTLIBRARIES = libschwa_drdist.la
libschwa_drdist_la_SOURCES = \
		schwa/dr-dist/helpers.cc \
		schwa/dr-dist/helpers.h \
		schwa/dr-dist/server.cc \
		schwa/dr-dist/server.h
libschwa_drdist_la_CXXFLAGS = $(libschwa_la_CXXFLAGS) $(ZMQLIB_CFLAGS)
libschwa_drdist_la_LIBADD = $(ZMQLIB_LIBS)

libschwa_la_LIBADD += libschwa_drdist.la
endif


# =============================================================================
# Automake
# =============================================================================
libschwa_la_SOURCES += $(LIBSCHWA_HEADER_FILES) $(LIBSCHWA_SOURCE_FILES)

library_includedir = $(includedir)
nobase_library_include_HEADERS = $(LIBSCHWA_HEADER_FILES)

TESTS = schwa_tests schwa_containers_tests schwa_dr_tests schwa_io_tests schwa_learn_tests schwa_mpl_tests schwa_msgpack_tests schwa_utils_tests
check_LIBRARIES = libschwa_unittest.a
check_PROGRAMS = schwa_tests schwa_containers_tests schwa_dr_tests schwa_io_tests schwa_learn_tests schwa_mpl_tests schwa_msgpack_tests schwa_utils_tests

LIBSCHWA_TEST_CPPFLAGS = -isystem $(srcdir)/../third-party/unittest-cpp/UnitTest++
LIBSCHWA_TEST_LDADD = ../third-party/unittest-cpp/UnitTest++/libUnitTest++.la libschwa_unittest.a libschwa.la

libschwa_unittest_a_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
libschwa_unittest_a_SOURCES = \
		schwa/unittest.h \
		schwa/unittest.cc

schwa_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_tests_CXXFLAGS = $(LIBSCHWA_TEST_CXXFLAGS)
schwa_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/pool_test.cc  \
		schwa/port_test.cc  \
		schwa/unicode_test.cc

schwa_containers_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_containers_tests_CXXFLAGS = $(LIBSCHWA_TEST_CXXFLAGS)
schwa_containers_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_containers_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/containers/block_vector_test.cc

schwa_dr_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_dr_tests_CXXFLAGS = $(LIBSCHWA_TEST_CXXFLAGS)
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
schwa_io_tests_CXXFLAGS = $(LIBSCHWA_TEST_CXXFLAGS)
schwa_io_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_io_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/io/mmapped_source_test.cc  \
		schwa/io/paths_test.cc  \
		schwa/io/write_buffer_test.cc

schwa_learn_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_learn_tests_CXXFLAGS = $(LIBSCHWA_TEST_CXXFLAGS)
schwa_learn_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_learn_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/learn/extract_test.cc \
		schwa/learn/feature_transformers_test.cc \
		schwa/learn/features_test.cc

schwa_mpl_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_mpl_tests_CXXFLAGS = $(LIBSCHWA_TEST_CXXFLAGS)
schwa_mpl_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_mpl_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/mpl/if_test.cc

schwa_msgpack_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_msgpack_tests_CXXFLAGS = $(LIBSCHWA_TEST_CXXFLAGS)
schwa_msgpack_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_msgpack_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/msgpack/wire_test.cc

schwa_utils_tests_CPPFLAGS = $(LIBSCHWA_TEST_CPPFLAGS)
schwa_utils_tests_CXXFLAGS = $(LIBSCHWA_TEST_CXXFLAGS)
schwa_utils_tests_LDADD = $(LIBSCHWA_TEST_LDADD)
schwa_utils_tests_SOURCES = \
		schwa/unittest_main.cc \
		schwa/utils/shlex_test.cc
