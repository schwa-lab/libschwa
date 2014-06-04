m4_define([cityhash_major], [1])
m4_define([cityhash_minor], [1])
m4_define([cityhash_patchlevel], [1])

# Libtool shared library interface versions (current:revision:age)
# Update this value for every release!  (A:B:C will map to foo.so.(A-C).C.B)
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
m4_define([cityhash_ltversion], [4:0:0])

AC_PREREQ([2.65])
AC_INIT([CityHash], [cityhash_major.cityhash_minor.cityhash_patchlevel], [cityhash-discuss@googlegroups.com])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.10 no-define foreign])
LT_PREREQ([2.2])
LT_INIT

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_CONFIG_SRCDIR([src/city.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])
AC_C_BIGENDIAN

dnl Check for SIMD extensions.
dnl Defines: HAVE_MMX, HAVE_SSE, HAVE_SSE2, HAVE_SSE3, HAVE_SSSE3, HAVE_SSE4.1, HAVE_SSE4.2, HAVE_AVX, SIMD_FLAGS
AX_EXT
AM_CONDITIONAL(HAVE_SSE4_2, test "x$HAVE_SSE4_2" = "x1")

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Check for __builtin_expect
AC_MSG_CHECKING([if the compiler supports __builtin_expect])
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM(, [[return __builtin_expect(1, 1) ? 1 : 0;]])],
  [
    cityhash_have_builtin_expect=yes
    AC_MSG_RESULT([yes])
], [
    cityhash_have_builtin_expect=no
    AC_MSG_RESULT([no])
])
if test x$cityhash_have_builtin_expect = xyes ; then
    AC_DEFINE([HAVE_BUILTIN_EXPECT], [1], [Define to 1 if the compiler supports __builtin_expect.])
fi

AC_OUTPUT
