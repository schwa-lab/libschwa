#!/bin/bash

for d in src/lib src/apps; do
  find ${d} \( -name '*.cc' -or -name '*.h' \) | sort | xargs egrep -n -H --color "${@}"
done
