#!/bin/bash
# http://www.webupd8.org/2010/01/how-to-create-deb-package-ubuntu-debian.html
set -e

MAINTAINER_NAME='Tim Dawborn'
MAINTAINER_EMAIL='tim.dawborn@gmail.com'

# Ensure we have a correct looking argument.
if [[ ${#} -ne 1 ]]; then
  echo "Usage: ${0} libschwa-X.Y.Z.tar.gz"
  exit 1
fi

if ! $(echo "${1}" | grep -q '^libschwa-[0-9]*\.[0-9]*\.[0-9]*\.tar\.gz$'); then
  echo "Usage: ${0} libschwa-X.Y.Z.tar.gz"
  exit 1
fi
VERSION=$(echo "${1}" | grep -o '[0-9]*\.[0-9]*\.[0-9]*')

if [[ "$(uname)" != "Linux" ]]; then
  echo 'Error: this script must be run on a Linux machine.'
  exit 1
fi

# Create the working directory.
WORKING_DIR=(mktemp -d -t $(basename ${0}))
trap "rm -rf ${WORKING_DIR}" EXIT

# Check required dependencies for building a deb file.
sudo apt-get install build-essential autoconf automake autotools-dev dh-make debhelper devscripts fakeroot xutils lintian pbuilder

# Extract the tarball and go into the extracted folder.
tar xzf ${1} -C ${WORKING_DIR}
cd ${WORKING_DIR}/libschwa-*

# Start the debianisation.
dh_make --email "${MAINTAINER_EMAIL}" --multi --file ../libschwa-*.tar.gz

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
#cp CHANGELOG debian/changelog  FIXME this needs to be in a specific format
cp LICENCE debian/copyright

# Build the source as a debian package.
dpkg-buildpackage -rfakeroot -us -uc

# Install the generated deb file on packages.schwa.org.
read -r -p 'Copy deb file to packages.schwa.org? [y/N] ' response
if [[ ${response} =~ ^([yY][eE][sS]|[yY])$ ]]; then
  LSB_RELEASE=$(lsb_release -c -s)
  scp ../libschwa*.deb setup@ch2:/var/www/sites/packages/ubuntu/pool/main/${LSB_RELEASE}
  ssh setup@ch2 "/var/www/sites/packages/ubuntu/update.sh"
fi
