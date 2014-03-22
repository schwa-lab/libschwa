# =============================================================================
# Automake
# =============================================================================
lib_LTLIBRARIES = libschwa.la

libschwa_la_CXXFLAGS = -W -Wall -Wextra -Werror -pedantic
libschwa_la_LIBADD =
libschwa_la_SOURCES =

if ENABLE_DEBUG
libschwa_la_LIBADD += $(PROFILERLIB) $(TCMALLOCLIB)
endif


# =============================================================================
# Ragel
# =============================================================================
schwa/tokenizer/tokenizer.cc: $(srcdir)/../ragel/tokenizer/tokenizer.rl $(srcdir)/../ragel/tokenizer/rules/*.rl
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
		schwa/exception.h \
		schwa/io/array_reader.h \
		schwa/io/file_source.h \
		schwa/io/istream_source.h \
		schwa/io/logging.h \
		schwa/io/logging_enums.h \
		schwa/io/mmapped_source.h \
		schwa/io/source.h \
		schwa/io/traits.h \
		schwa/io/unsafe_array_writer.h \
		schwa/io/utils.h \
		schwa/io/write_buffer.h \
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
		schwa/utils/enums.h \
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
		schwa/io/utils.cc \
		schwa/io/write_buffer.cc \
		schwa/msgpack/dynamic.cc \
		schwa/msgpack/enums.cc \
		schwa/msgpack/exception.cc \
		schwa/msgpack/wire.cc \
		schwa/port.cc \
		schwa/tokenizer/debug_text_stream.cc \
		schwa/tokenizer/docrep_stream.cc \
		schwa/tokenizer/text_stream.cc \
		schwa/tokenizer/tokenizer.cc \
		schwa/utils/shlex.cc \
		schwa/version.cc

LIBSCHWA_TEST_SOURCE_FILES = \
		schwa/containers/block_vector_test.cc  \
		schwa/dr/helpers_test.cc  \
		schwa/dr/lazy_test.cc  \
		schwa/dr/pointers_test.cc  \
		schwa/dr/reader_test.cc  \
		schwa/dr/self_pointer_test.cc  \
		schwa/dr/slices_test.cc  \
		schwa/dr/writer_test.cc  \
		schwa/io/mmapped_source_test.cc  \
		schwa/io/write_buffer_test.cc  \
		schwa/mpl/if_test.cc  \
		schwa/msgpack/wire_test.cc  \
		schwa/pool_test.cc  \
		schwa/port_test.cc  \
		schwa/utils/shlex_test.cc

LIBSCHWA_TEST_COMMON_HEADER_FILES = schwa/unittest.h
LIBSCHWA_TEST_COMMON_SOURCE_FILES = schwa/unittest.cc


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

TESTS = unittests
check_LIBRARIES = libschwa_unittest.a
check_PROGRAMS = unittests

unittests_CPPFLAGS = -I$(srcdir)/../third-party/unittest-cpp/UnitTest++
unittests_LDADD = \
		../third-party/unittest-cpp/UnitTest++/libUnitTest++.la \
		libschwa_unittest.a \
		libschwa.la
unittests_SOURCES = schwa/unittest_main.cc $(LIBSCHWA_TEST_SOURCE_FILES)

libschwa_unittest_a_SOURCES = $(LIBSCHWA_TEST_COMMON_HEADER_FILES) $(LIBSCHWA_TEST_COMMON_SOURCE_FILES)
libschwa_unittest_a_CPPFLAGS = -I$(srcdir)/../third-party/unittest-cpp/UnitTest++
