#!/bin/bash
# http://www.webupd8.org/2010/01/how-to-create-deb-package-ubuntu-debian.html
set -e

MAINTAINER_NAME='Tim Dawborn'
MAINTAINER_EMAIL='tim.dawborn@gmail.com'
WORKING_DIR=/tmp/create-deb-package


# Relocate to the top level project directory.
cd $(dirname ${0})/..

# Check required dependencies for building a deb file.
sudo apt-get install build-essential autoconf automake autotools-dev dh-make debhelper devscripts fakeroot xutils lintian pbuilder

# Create the distribution.
make dist
version=$(tail -n +2 m4/version.m4)

# Create and relocate to the working directory.
rm -rf ${WORKING_DIR}
mkdir -p ${WORKING_DIR}
cp libschwa-${version}.tar.gz ${WORKING_DIR}
cd ${WORKING_DIR}

# Extract the tarball and go into the extracted folder.
tar xzf libschwa-${version}.tar.gz
cd libschwa-${version}

# Start the debianisation.
dh_make --email "${MAINTAINER_EMAIL}" --multi --file ../libschwa-${version}.tar.gz

# Update the generated debian files.
cat > debian/control <<EOF
Source: libschwa
Section: unknown
Priority: extra
Maintainer: ${MAINTAINER_NAME} <${MAINTAINER_EMAIL}>
Build-Depends: debhelper (>= 8.0.0), autotools-dev, libgcc-4.8-dev, libzmq3-dev
Standards-Version: 3.9.2
Homepage: https://github.com/schwa-lab/libschwa

Package: libschwa
Architecture: any
Depends: \${shlibs:Depends}, \${misc:Depends}
Description: Schwa Lab core NLP tools.
 Schwa Lab core NLP tools.
EOF
cp LICENCE debian/copyright

# Build the source as a debian package.
dpkg-buildpackage -rfakeroot

# Install the generated deb file on packages.schwa.org.
read -r -p 'Copy deb file to packages.schwa.org? [y/N] ' response
if [[ ${response} =~ ^([yY][eE][sS]|[yY])$ ]]; then
  scp ../libschwa*.deb deb@ch2:packages/ubuntu/pool/main/precise
  ssh deb@ch2 'packages/ubuntu/update-amd64.sh precise'
fi
