#!/bin/sh

# Define a cleanup function to easily remove the autoconf/autotools files.
function cleanup {
  # Use any existing Makefiles to clean what was created
  test -f Makefile && make distclean

  # Remove automatically generated files.
  rm -rf Makefile \
    Makefile.in \
    aclocal.m4 \
    autom4te.cache \
    build-aux \
    config.h.in \
    config.h.in~ \
    config.log \
    config.status \
    configure
}


# Allow this script to be run from anywhere.
cd $(dirname ${0})

# Clean and maybe exit.
cleanup
test "${1+set}" = "set" && exit 0

# Run autoreconf.
autoreconf --install

# Ensure any missing files are writable.
chmod 0755 build-aux/*

# Cleanup.
rm -rf autom4te.cache

exit 0
