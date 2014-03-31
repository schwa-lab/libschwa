#!/bin/bash
set -e

M4_VERSION_FILE='m4/version.m4'
M4_VERSION_SET_FILE='m4/version-set.m4'

# Relocate to the top level project directory.
cd $(dirname ${0})/..

# Try and work out what the new version should be.
version_major=0
version_minor=0
version_patch=0
current_version_string='unknown'
if test -f ${M4_VERSION_SET_FILE}; then
  version_major=$(grep '^VERSION_MAJOR=' ${M4_VERSION_SET_FILE} | cut -d = -f 2)
  version_minor=$(($(grep '^VERSION_MINOR=' ${M4_VERSION_SET_FILE} | cut -d = -f 2) + 1))
  version_patch=0
  current_version_string=$(grep '^VERSION_STRING=' ${M4_VERSION_SET_FILE} | cut -d = -f 2)
fi
if [[ ${#} -ne 0 ]]; then
  version_major=${1}
  shift
fi
if [[ ${#} -ne 0 ]]; then
  version_minor=${1}
  shift
fi
if [[ ${#} -ne 0 ]]; then
  version_patch=${1}
  shift
fi
version_date="$(date +%Y-%m-%d)"
version="${version_major}.${version_minor}.${version_patch}"
version_string="'\"${version} (${version_date})\"'"

# Confirm before going ahead.
echo "Current version: ${current_version_string}"
echo "    New version: ${version_string}"
read -r -p 'Are you sure? [y/N] ' response
if [[ ${response} =~ ^([yY][eE][sS]|[yY])$ ]]; then
  true
else
  exit 0
fi

# Create the release branch.
git checkout -b release-${version} develop

# Bump the version and commit the change.
./dist/bump-version.sh ${version_major} ${version_minor} ${version_patch} "${version_date}"
git commit -a -m "Bumped version to ${version}."

echo "Release branch successfully created and the version has been bumped to ${version}."
echo "Once you're ready to release, call dist/finish-release-branch.sh."
