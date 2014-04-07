#!/bin/bash
# http://nvie.com/posts/a-successful-git-branching-model/
set -e

M4_VERSION_FILE='m4/version.m4'
M4_VERSION_SET_FILE='m4/version-set.m4'


if [[ ${#} -ne 4 ]]; then
  echo "Usage: ${0} major minor patch date"
  echo "This script should be called by dist/create-release-branch.sh"
  exit 1
fi

# Relocate to the top level project directory.
cd $(dirname ${0})/..

# Create the version.m4 file.
cat > ${M4_VERSION_FILE} <<EOF
dnl Product version for AC_INIT. Maintained by dist/bump-version.sh
${1}.${2}.${3}
EOF

# Create the version-set.m4 file.
cat > ${M4_VERSION_SET_FILE} <<EOF
dnl Product version for header files. Maintained by dist/bump-version.sh
VERSION_MAJOR=${1}
VERSION_MINOR=${2}
VERSION_PATCH=${3}
VERSION_STRING='"${1}.${2}.${3} (${4})"'

AC_SUBST(VERSION_MAJOR)
AC_SUBST(VERSION_MINOR)
AC_SUBST(VERSION_PATCH)
AC_SUBST(VERSION_STRING)

VERSION_NOPATCH=${1}.${2}
AC_SUBST(VERSION_NOPATCH)
EOF
