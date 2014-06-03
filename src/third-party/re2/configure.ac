AC_PREREQ([2.63])
AC_INIT([libre2], [2014-03-04])

m4_include([m4/ax_pthread.m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])
AM_PROG_AR
LT_INIT

dnl Checks for programs.
AC_PROG_CXX
AC_LANG([C++])

dnl Checks for header files.
AC_CHECK_HEADER([algorithm], , AC_MSG_ERROR([C++11 algorithm header not found]))
AC_CHECK_HEADER([cstddef], , AC_MSG_ERROR([C++11 cstddef header not found]))
AC_CHECK_HEADER([iosfwd], , AC_MSG_ERROR([C++11 iosfwd header not found]))
AC_CHECK_HEADER([iostream], , AC_MSG_ERROR([C++11 iostream header not found]))
AC_CHECK_HEADER([map], , AC_MSG_ERROR([C++11 map header not found]))
AC_CHECK_HEADER([set], , AC_MSG_ERROR([C++11 set header not found]))
AC_CHECK_HEADER([sstream], , AC_MSG_ERROR([C++11 sstream header not found]))
AC_CHECK_HEADER([stack], , AC_MSG_ERROR([C++11 stack header not found]))
AC_CHECK_HEADER([string], , AC_MSG_ERROR([C++11 string header not found]))
AC_CHECK_HEADER([unordered_set], , AC_MSG_ERROR([C++11 unordered_set header not found]))
AC_CHECK_HEADER([utility], , AC_MSG_ERROR([C++11 utility header not found]))
AC_CHECK_HEADER([vector], , AC_MSG_ERROR([C++11 vector header not found]))

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

dnl Check for pthread.
AX_PTHREAD

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
