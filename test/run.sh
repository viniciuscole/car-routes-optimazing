#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."

gcc -std=gnu99 -O2 -Wall -o trab2 $(ls src/*.c | grep -v trace_main.c) -lm
gcc -std=gnu99 -O2 -Wall -o trace $(ls src/*.c | grep -v '/main.c') -lm

fail=0
for case in with_update no_update; do
  ./trab2 "test/$case.txt" "test/$case.out"
  if cmp -s "test/$case.out" "test/$case.expected"; then
    echo "ok    $case"
  else
    echo "FAIL  $case"; diff "test/$case.expected" "test/$case.out" || true; fail=1
  fi
done

./trace < test/with_update.txt > test/with_update.trace.out
if cmp -s test/with_update.trace.out test/with_update.trace; then
  echo "ok    trace"
else
  echo "FAIL  trace"; diff test/with_update.trace test/with_update.trace.out || true; fail=1
fi

exit $fail
