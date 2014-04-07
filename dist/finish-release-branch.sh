#!/bin/bash
# http://nvie.com/posts/a-successful-git-branching-model/
set -e

# Relocate to the top level project directory.
cd $(dirname ${0})/..

# Ensure we're currently on a release branch.
branch="$(git rev-parse --abbrev-ref HEAD)"
if [[ ${branch} =~ ^release-[0-9.]*$ ]]; then
  true
else
  echo "It look like you're not currently on a release branch (${branch}). Aborting."
  exit 1
fi

# Grab the current version.
version=$(echo ${branch} | cut -d '-' -f 2)

# Confirm before going ahead.
echo "About to finish release version ${version}."
read -r -p 'Are you sure? [y/N] ' response
if [[ ${response} =~ ^([yY][eE][sS]|[yY])$ ]]; then
  true
else
  exit 0
fi

# Ensure Makefile exists so we can create the dist tarball.
if ! test -f Makefile; then
  echo 'Makefile not found. Aborting.'
  exit 1
fi

# Merge into master and tag the release.
echo 'Merging into master...'
git checkout master
git merge --no-ff ${branch}
git tag -a ${version} -m "Release ${version}."

echo "Release branch ${version} has been merged into master and tagged. Please push upstream."
echo '$ git push origin master'
echo '$ git push --tags'
echo

# Create the release tarball.
make --quiet dist
echo "Created the release tarball libschwa-${version}.tar.gz. Please create the GitHub release."
echo

# Merge back into develop.
echo 'Merging into develop. This might cause conflicts...'
git checkout develop
git merge --no-ff ${branch}
