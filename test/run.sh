#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."

gcc -std=gnu99 -O2 -Wall -o trab2 src/*.c -lm

fail=0
for case in with_update no_update; do
  ./trab2 "test/$case.txt" "test/$case.out"
  if cmp -s "test/$case.out" "test/$case.expected"; then
    echo "ok    $case"
  else
    echo "FAIL  $case"; diff "test/$case.expected" "test/$case.out" || true; fail=1
  fi
done
exit $fail
