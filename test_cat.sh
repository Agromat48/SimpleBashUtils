#!/bin/bash

PASS=0
FAIL=0

printf "hello\n\nworld\n\t tab\n" > test.txt

check() {
  if [ "$(cat $1 test.txt)" = "$(./s21_cat $1 test.txt)" ]; then
    echo "PASS: $1"
    PASS=$((PASS + 1))
  else
    echo "FAIL: $1"
    FAIL=$((FAIL + 1))
  fi
}

check ""
check "-n"
check "-b"
check "-s"
check "-e"
check "-E"
check "-t"
check "-T"
check "-ns"
check "-bs"
check "-es"
check "-nb"
check "--number"
check "--number-nonblank"
check "--squeeze-blank"

rm test.txt

echo "PASS=$PASS FAIL=$FAIL"