AC_PREREQ(2.63)
AC_INIT([libschwa], m4_normalize(m4_include([m4/version.m4])), [schwa-lab@it.usyd.edu.au])

m4_include([m4/version-set.m4])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([README])

# Initialise automake.
AM_INIT_AUTOMAKE([1.11 foreign -Wall -Werror parallel-tests subdir-objects])
AM_OPTIONS
AM_PROG_AR

# If CFLAGS was not set on entry and we are not debugging, default to -O3.
libschwa_test_CFLAGS="${CFLAGS+set}"
if test "$libschwa_test_CFLAGS" != set && test "$libschwa_cv_enable_debug" != 'yes'; then
  CFLAGS='-O3'
fi

# Initialise libtool (platform independant shared library generation).
LT_INIT
AC_SUBST([LIBTOOL_DEPS])

# Ensure we have a C++ compiler and we have C++11 support.
AC_PROG_CXX
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

# Add debugging flags if requested and not already in CFLAGS.
if test "$libschwa_cv_enable_debug" = "yes"; then
  case "${CFLAGS}" in
    *-g*) ;;
       *) AM_CFLAGS="${AM_CFLAGS} -g" ;;
  esac
fi
AM_CONDITIONAL([ENABLE_DEBUG], [test "$libschwa_cv_enable_debug" = "yes"])

# Check for headers.
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([cxxabi.h endian.h libgen.h libproc.h limits.h machine/byte_order.h])  # <schwa/port.{h,cc}>

# Work out how to inline the "host to big endian" functions for various based on what headers we found.
if test "$ac_cv_header_endian_h" = "yes"; then
  AC_SUBST([ENDIAN_CONVERSION_HEADER], [endian.h])
  AC_SUBST([ENDIAN_CONVERSION_H_TO_BE16], [htobe16])
  AC_SUBST([ENDIAN_CONVERSION_H_TO_BE32], [htobe32])
  AC_SUBST([ENDIAN_CONVERSION_H_TO_BE64], [htobe64])
  AC_SUBST([ENDIAN_CONVERSION_BE16_TO_H], [be16toh])
  AC_SUBST([ENDIAN_CONVERSION_BE32_TO_H], [be32toh])
  AC_SUBST([ENDIAN_CONVERSION_BE64_TO_H], [be64toh])
elif test "$ac_cv_header_machine_byte_order_h" = "yes"; then
  AC_SUBST([ENDIAN_CONVERSION_HEADER], [machine/byte_order.h])
  AC_SUBST([ENDIAN_CONVERSION_H_TO_BE16], [OSSwapHostToBigConstInt16])
  AC_SUBST([ENDIAN_CONVERSION_H_TO_BE32], [OSSwapHostToBigConstInt32])
  AC_SUBST([ENDIAN_CONVERSION_H_TO_BE64], [OSSwapHostToBigConstInt64])
  AC_SUBST([ENDIAN_CONVERSION_BE16_TO_H], [OSSwapBigToHostConstInt16])
  AC_SUBST([ENDIAN_CONVERSION_BE32_TO_H], [OSSwapBigToHostConstInt32])
  AC_SUBST([ENDIAN_CONVERSION_BE64_TO_H], [OSSwapBigToHostConstInt64])
else
  AC_MSG_ERROR([Could not work out how to deal with endianness conversion on your platform])
fi

# Check for Google perftools to link against.
AC_CHECK_LIB([profiler], [ProfilerStart], [PROFILERLIB=-lprofiler])
AC_SUBST([PROFILERLIB])
AC_CHECK_LIB([tcmalloc], [malloc], [TCMALLOCLIB=-ltcmalloc])
AC_SUBST([TCMALLOCLIB])

# Configure autoconf inputs and outputs.
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  src/apps/Makefile
  src/apps/dr/Makefile
  src/apps/dr-count/Makefile
  src/apps/dr-head/Makefile
  src/apps/dr-tail/Makefile
  src/apps/dr-ui/Makefile
  src/apps/schwa-tokenizer/Makefile
  src/lib/Makefile
  src/lib/schwa/_base.h:src/lib/schwa/_base.h.in
  src/lib/schwa/port.h:src/lib/schwa/port.h.in
  src/third-party/Makefile
  dist/libschwa.pc:dist/libschwa.pc.in
])

# Configure third party modules
AC_CONFIG_SUBDIRS([
  src/third-party/unittest-cpp
])

AC_OUTPUT
