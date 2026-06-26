#!/bin/bash

PASS=0
FAIL=0

echo -e "Hello World\nhello world\nfoo bar\ntest 123\nHELLO\ngrep pattern\nno match here" > test.txt
echo -e "second file\nhello again\nFOO BAR\n123 test" > test2.txt
echo -e "pattern1\npattern2" > patterns.txt

check() {
  local expected
  local got
  expected=$(eval "grep $1" 2>/dev/null)
  got=$(eval "./s21_grep $1" 2>/dev/null)
  if [ "$expected" = "$got" ]; then
    echo "PASS: s21_grep $1"
    PASS=$((PASS + 1))
  else
    echo "FAIL: s21_grep $1"
    diff <(echo "$expected") <(echo "$got") | head -5
    FAIL=$((FAIL + 1))
  fi
}

check '"hello" test.txt'
check '-i "hello" test.txt'
check '-v "hello" test.txt'
check '-n "hello" test.txt'
check '-c "hello" test.txt'
check '-l "hello" test.txt'
check '-h "hello" test.txt test2.txt'
check '-o "hel[a-z]*" test.txt'
check '-e "hello" test.txt'
check '-f patterns.txt test.txt'
check '-o "hel[a-z]*" test.txt test2.txt'
check '-in "hello" test.txt'
check '-iv "hello" test.txt'
check '-ic "hello" test.txt'
check '-vn "hello" test.txt'

rm -f test.txt test2.txt patterns.txt

echo ""
echo "PASS=$PASS FAIL=$FAIL"