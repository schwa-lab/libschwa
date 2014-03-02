# Configuration options.
AC_DEFUN([AM_OPTIONS], [

AC_MSG_CHECKING(if --enable-debug option specified)
AC_ARG_ENABLE(debug, [AS_HELP_STRING([--enable-debug], [Configure for debug symbols.])], r=$enableval, r=no)
case "$r" in
no) libschwa_cv_enable_debug=no;;
 *) libschwa_cv_enable_debug=yes;;
esac
AC_MSG_RESULT($libschwa_cv_enable_debug)

AC_MSG_CHECKING(if --enable-libzmq option specified)
AC_ARG_ENABLE(libzmq, [AS_HELP_STRING([--enable-libzmq], [Enable building the libzmq components.])], r=$enableval, r=yes)
case "$r" in
no) libschwa_cv_enable_libzmq=no;;
 *) libschwa_cv_enable_libzmq=yes;;
esac
AC_MSG_RESULT($libschwa_cv_enable_libzmq)

])
