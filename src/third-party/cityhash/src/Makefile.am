AM_CPPFLAGS = $(SIMD_FLAGS)

noinst_LIBRARIES = libcityhash.a

libcityhash_a_SOURCES = city.cc city.h
if HAVE_SSE4_2
libcityhash_a_SOURCES += citycrc.h
endif


TESTS = cityhash_unittest
check_PROGRAMS = $(TESTS)

cityhash_unittest_LDADD = libcityhash.a
cityhash_unittest_SOURCES = city-test.cc
