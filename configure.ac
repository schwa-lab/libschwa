AC_PREREQ([2.65])
AC_INIT([libschwa], m4_normalize(m4_include([m4/version.m4])), [schwa-lab@it.usyd.edu.au], [], [https://github.com/schwa-lab/libschwa])
: ${CXXFLAGS=""}

m4_include([m4/version-set.m4])
AC_SUBST([libschwa_la_version_info], [4:0:0])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([README])

dnl Initialise automake.
AM_INIT_AUTOMAKE([1.11 foreign -Wall -Werror parallel-tests no-define subdir-objects])
AM_OPTIONS
AM_PROG_AR

dnl Check if we're compiling with clang.
AC_MSG_CHECKING([if compiling with clang])
AC_COMPILE_IFELSE(
[AC_LANG_PROGRAM([], [[
#ifndef __clang__
       not clang
#endif
]])], [CLANG=yes], [CLANG=no])
AC_MSG_RESULT([$CLANG])

dnl If we are not debugging, default to -O3.
if test "$libschwa_cv_enable_debug" != "yes"; then
  AM_CXXFLAGS='-O3 -g'
else
  AM_CXXFLAGS='-O0 -g'
fi

dnl Add debugging flags if requested and not already in CXXFLAGS.
if test "$libschwa_cv_enable_debug" = "yes"; then
  case "${CXXFLAGS}" in
    *-g*) ;;
       *) AM_CXXFLAGS="${AM_CXXFLAGS} -g3" ;;
  esac
fi
AC_SUBST([AM_CXXFLAGS])
AM_CONDITIONAL([ENABLE_DEBUG], [test "$libschwa_cv_enable_debug" = "yes"])

dnl Initialise libtool (platform independant shared library generation).
LT_INIT
AC_SUBST([LIBTOOL_DEPS])

dnl Ensure we have a C++ compiler and we have C++11 support.
AC_PROG_CXX
AC_PROG_CXXCPP
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
AC_C_BIGENDIAN

dnl Initialise pkg-config.
PKG_PROG_PKG_CONFIG

dnl Check for headers.
AC_CHECK_HEADER([algorithm], , AC_MSG_ERROR([C++11 algorithm header not found]))
AC_CHECK_HEADER([array], , AC_MSG_ERROR([C++11 array header not found]))
AC_CHECK_HEADER([cassert], , AC_MSG_ERROR([C++11 cassert header not found]))
AC_CHECK_HEADER([cctype], , AC_MSG_ERROR([C++11 cctype header not found]))
AC_CHECK_HEADER([cerrno], , AC_MSG_ERROR([C++11 cerrno header not found]))
AC_CHECK_HEADER([chrono], , AC_MSG_ERROR([C++11 chrono header not found]))
AC_CHECK_HEADER([cinttypes], , AC_MSG_ERROR([C++11 cinttypes header not found]))
AC_CHECK_HEADER([cstddef], , AC_MSG_ERROR([C++11 cstddef header not found]))
AC_CHECK_HEADER([cstdint], , AC_MSG_ERROR([C++11 cstdint header not found]))
AC_CHECK_HEADER([cstdio], , AC_MSG_ERROR([C++11 cstdio header not found]))
AC_CHECK_HEADER([cstdlib], , AC_MSG_ERROR([C++11 cstdlib header not found]))
AC_CHECK_HEADER([cstring], , AC_MSG_ERROR([C++11 cstring header not found]))
AC_CHECK_HEADER([ctime], , AC_MSG_ERROR([C++11 ctime header not found]))
AC_CHECK_HEADER([deque], , AC_MSG_ERROR([C++11 deque header not found]))
AC_CHECK_HEADER([exception], , AC_MSG_ERROR([C++11 exception header not found]))
AC_CHECK_HEADER([fstream], , AC_MSG_ERROR([C++11 fstream header not found]))
AC_CHECK_HEADER([functional], , AC_MSG_ERROR([C++11 functional header not found]))
AC_CHECK_HEADER([iomanip], , AC_MSG_ERROR([C++11 iomanip header not found]))
AC_CHECK_HEADER([iosfwd], , AC_MSG_ERROR([C++11 iosfwd header not found]))
AC_CHECK_HEADER([iostream], , AC_MSG_ERROR([C++11 iostream header not found]))
AC_CHECK_HEADER([iterator], , AC_MSG_ERROR([C++11 iterator header not found]))
AC_CHECK_HEADER([limits], , AC_MSG_ERROR([C++11 limits header not found]))
AC_CHECK_HEADER([map], , AC_MSG_ERROR([C++11 map header not found]))
AC_CHECK_HEADER([memory], , AC_MSG_ERROR([C++11 memory header not found]))
AC_CHECK_HEADER([mutex], , AC_MSG_ERROR([C++11 mutex header not found]))
AC_CHECK_HEADER([new], , AC_MSG_ERROR([C++11 new header not found]))
AC_CHECK_HEADER([ostream], , AC_MSG_ERROR([C++11 ostream header not found]))
AC_CHECK_HEADER([random], , AC_MSG_ERROR([C++11 random header not found]))
AC_CHECK_HEADER([set], , AC_MSG_ERROR([C++11 set header not found]))
AC_CHECK_HEADER([sstream], , AC_MSG_ERROR([C++11 sstream header not found]))
AC_CHECK_HEADER([stack], , AC_MSG_ERROR([C++11 stack header not found]))
AC_CHECK_HEADER([string], , AC_MSG_ERROR([C++11 string header not found]))
AC_CHECK_HEADER([thread], , AC_MSG_ERROR([C++11 thread header not found]))
AC_CHECK_HEADER([typeinfo], , AC_MSG_ERROR([C++11 typeinfo header not found]))
AC_CHECK_HEADER([unordered_set], , AC_MSG_ERROR([C++11 unordered_set header not found]))
AC_CHECK_HEADER([utility], , AC_MSG_ERROR([C++11 utility header not found]))
AC_CHECK_HEADER([vector], , AC_MSG_ERROR([C++11 vector header not found]))
AC_CHECK_HEADER([dirent.h], , AC_MSG_ERROR([POSIX dirent.h header not found]))
AC_CHECK_HEADER([fcntl.h], , AC_MSG_ERROR([POSIX fcntl.h header not found]))
AC_CHECK_HEADER([unistd.h], , AC_MSG_ERROR([POSIX unistd.h header not found]))
AC_CHECK_HEADERS([cxxabi.h endian.h libgen.h libproc.h limits.h machine/byte_order.h])  dnl <schwa/port.{h,cc}>

dnl Check for C headers.
AC_CHECK_HEADER([assert.h], , AC_MSG_ERROR([C assert.h header not found]))
AC_CHECK_HEADER([ctype.h], , AC_MSG_ERROR([C ctype.h header not found]))
AC_CHECK_HEADER([errno.h], , AC_MSG_ERROR([C errno.h header not found]))
AC_CHECK_HEADER([stdarg.h], , AC_MSG_ERROR([C stdarg.h header not found]))
AC_CHECK_HEADER([stddef.h], , AC_MSG_ERROR([C stddef.h header not found]))
AC_CHECK_HEADER([stdint.h], , AC_MSG_ERROR([C stdint.h header not found]))
AC_CHECK_HEADER([stdio.h], , AC_MSG_ERROR([C stdio.h header not found]))
AC_CHECK_HEADER([stdlib.h], , AC_MSG_ERROR([C stdlib.h header not found]))
AC_CHECK_HEADER([string.h], , AC_MSG_ERROR([C string.h header not found]))
AC_CHECK_HEADER([sys/mman.h], , AC_MSG_ERROR([C sys/mman.h header not found]))
AC_CHECK_HEADER([sys/resource.h], , AC_MSG_ERROR([C sys/resource.h header not found]))
AC_CHECK_HEADER([sys/stat.h], , AC_MSG_ERROR([C sys/stat.h header not found]))
AC_CHECK_HEADER([sys/time.h], , AC_MSG_ERROR([C sys/time.h header not found]))
AC_CHECK_HEADER([sys/types.h], , AC_MSG_ERROR([C sys/types.h header not found]))
AC_CHECK_HEADER([time.h], , AC_MSG_ERROR([C time.h header not found]))
AC_CHECK_HEADER([unistd.h], , AC_MSG_ERROR([C unistd.h header not found]))

dnl Check for optional headers.
AC_CHECK_HEADERS([pcre.h])
AC_CHECK_HEADERS([xmmintrin.h emmintrin.h])

dnl Check for C functions.
AC_CHECK_FUNC([close], , AC_MSG_ERROR([C close function not found]))
AC_CHECK_FUNC([dup2], , AC_MSG_ERROR([C dup2 function not found]))
AC_CHECK_FUNC([fork], , AC_MSG_ERROR([C fork function not found]))
AC_CHECK_FUNC([fstat], , AC_MSG_ERROR([C fstat function not found]))
AC_CHECK_FUNC([mmap], , AC_MSG_ERROR([C mmap function not found]))
AC_CHECK_FUNC([open], , AC_MSG_ERROR([C open function not found]))
AC_CHECK_FUNC([pipe], , AC_MSG_ERROR([C pipe function not found]))
AC_CHECK_FUNC([stat], , AC_MSG_ERROR([C stat function not found]))
AC_CHECK_FUNC([strsep], , AC_MSG_ERROR([C strsep function not found]))
AC_CHECK_FUNC([munmap], , AC_MSG_ERROR([C munmap function not found]))

dnl Check for pthread.
dnl Defines: PTHREAD_CC, PTHREAD_CFLAGS, PTHREAD_LIBS, HAVE_PTHREAD
AX_PTHREAD

dnl Check for SIMD extensions.
dnl Defines: HAVE_MMX, HAVE_SSE, HAVE_SSE2, HAVE_SSE3, HAVE_SSSE3, HAVE_SSE4.1, HAVE_SSE4.2, HAVE_AVX, SIMD_FLAGS
AX_EXT

dnl Check for posix_memalign
dnl Defines: HAVE_POSIX_MEMALIGN
AX_FUNC_POSIX_MEMALIGN

dnl Check for __builtin_expect.
AC_MSG_CHECKING([if the compiler supports __builtin_expect])
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM(, [[return __builtin_expect(1, 1) ? 1 : 0;]])],
  [
    libschwa_have_builtin_expect=yes
    AC_MSG_RESULT([yes])
  ],
  [
    libschwa_have_builtin_expect=no
    AC_MSG_RESULT([no])
  ]
)
if test x$libschwa_have_builtin_expect = xyes ; then
  AC_DEFINE([HAVE_BUILTIN_EXPECT], [1], [Define to 1 if the compiler supports __builtin_expect.])
  AC_SUBST([SCHWA_LIKELY], ['(__builtin_expect(!!(x), 1))'])
  AC_SUBST([SCHWA_UNLIKELY], ['(__builtin_expect(!!(x), 0))'])
else
  AC_SUBST([SCHWA_LIKELY], ['(x)'])
  AC_SUBST([SCHWA_UNLIKELY], ['(x)'])
fi

dnl Work out how to inline the "host to big endian" functions for various based on what headers we found.
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

dnl Check for Google perftools to link against.
AC_CHECK_LIB([profiler], [ProfilerStart], [PROFILERLIB=-lprofiler])
AC_SUBST([PROFILERLIB])
AC_CHECK_LIB([tcmalloc], [malloc], [TCMALLOCLIB=-ltcmalloc])
AC_SUBST([TCMALLOCLIB])

dnl Check if we have ØMQ >= 3.
PKG_CHECK_MODULES([ZMQLIB], [libzmq >= 3], [have_libzmq=yes], [have_libzmq=no])
AM_CONDITIONAL([HAVE_LIBZMQ],  [test "$have_libzmq" = "yes" && test "$libschwa_cv_enable_libzmq" = "yes"])

dnl Construct our base set of CPPFLAGS, depending on the compiler.
LIBSCHWA_BASE_CPPFLAGS='-Wall -Wextra -Werror -pedantic -Wformat-security -Wpointer-arith -Wformat-nonliteral -Winit-self -Wfloat-equal -ffast-math -fstack-protector'
if test "x$GXX" = xyes; then
  dnl Add compiler specific flags.
  if test "x$CLANG" = xyes; then
    LIBSCHWA_BASE_CPPFLAGS="$LIBSCHWA_BASE_CPPFLAGS -fdiagnostics-show-option -fdiagnostics-show-template-tree -pedantic-errors"
  else
    LIBSCHWA_BASE_CPPFLAGS="$LIBSCHWA_BASE_CPPFLAGS -Wdouble-promotion"
  fi
fi
AC_SUBST([LIBSCHWA_BASE_CPPFLAGS])

dnl Configure autoconf inputs and outputs.
AC_CONFIG_HEADERS([src/lib/schwa/_config.h])
AC_CONFIG_FILES([
  Makefile
  src/apps/Makefile
  src/apps/ccg-pprint/Makefile
  src/apps/corpora-to-dr/Makefile
  src/apps/dr/Makefile
  src/apps/dr-count/Makefile
  src/apps/dr-dist/Makefile
  src/apps/dr-grep/Makefile
  src/apps/dr-head/Makefile
  src/apps/dr-less/Makefile
  src/apps/dr-offsets/Makefile
  src/apps/dr-sample/Makefile
  src/apps/dr-tail/Makefile
  src/apps/dr-worker-example/Makefile
  src/apps/mp-less/Makefile
  src/apps/schwa-crfsuite/Makefile
  src/apps/schwa-tokenizer/Makefile
  src/lib/Makefile
  src/lib/schwa/macros.h:src/lib/schwa/macros.h.in
  src/lib/schwa/port.h:src/lib/schwa/port.h.in
  src/third-party/Makefile
  dist/libschwa.pc:dist/libschwa.pc.in
])

dnl Configure third party modules
AC_CONFIG_SUBDIRS([
  src/third-party/unittest-cpp
])

AC_OUTPUT
