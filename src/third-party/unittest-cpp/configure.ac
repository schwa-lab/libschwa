AC_PREREQ([2.69])
AC_INIT([UnitTest++], [1.4], [victor.lavaud@gmail.com])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([UnitTest++/TestDetails.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])
AM_PROG_AR
LT_INIT

AC_SUBST([LIBUNITTEST_SO_VERSION], [1:4:0])

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])

# Checks for header files.
AC_CHECK_HEADERS([sys/time.h unistd.h setjmp.h signal.h])

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday strstr])

AC_CONFIG_FILES([Makefile UnitTest++/Makefile tests/Makefile])
AC_OUTPUT
