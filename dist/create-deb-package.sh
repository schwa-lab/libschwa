#!/bin/bash
set -e

WORKING_DIR=/tmp/create-deb-package


if [[ ${#} -ne 1 ]]; then
  echo "Usage: ${0} https://github.com/schwa-lab/libschwa/releases/libschwa-XXX.tar.gz"
  exit 1
fi

# Check required dependencies for building a deb file.
if ! $(which checkinstall); then
  echo "'checkinstall' not found. Please install it."
  exit 1
fi

# Create and relocate to the working directory.
rm -rf ${WORKING_DIR}
mkdir -p ${WORKING_DIR}
cd ${WORKING_DIR}

# Obtain the tarball.
wget "${1}"

# Extract the tarball and go into the extracted folder.
tar xzf libschwa-*.tar.gz
cd $(find . -type d -maxdepth 1 -mindepth 1)
pwd
