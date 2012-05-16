#!/bin/bash

if test -e tests/$1.html; then
  INPUT=tests/$1.html;
elif test -e tests/$1.txt; then
  INPUT=tests/$1.txt;
fi

OUTPUT=tests/$1.tok;

cat $INPUT | ./test_tokenizer > compare.out
diff -i $OUTPUT compare.out
