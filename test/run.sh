#!/bin/bash
set -euo pipefail
export ASAN_OPTIONS=detect_leaks=0
cd "$(dirname "$0")/.."

gcc -std=gnu99 -O1 -g -fsanitize=address,undefined -Wall -o trab2 $(ls src/*.c | grep -v trace_main.c) -lm
gcc -std=gnu99 -O1 -g -fsanitize=address,undefined -Wall -o trace $(ls src/*.c | grep -v '/main.c') -lm

fail=0
for case in with_update no_update long_line bad_update; do
  ./trab2 "test/$case.txt" "test/$case.out" 2>/dev/null
  if cmp -s "test/$case.out" "test/$case.expected"; then
    echo "ok    $case"
  else
    echo "FAIL  $case"; diff "test/$case.expected" "test/$case.out" || true; fail=1
  fi
done

for pair in trace:with_update unreachable:unreachable; do
  label="${pair%%:*}"
  base="${pair##*:}"
  ./trace < "test/$base.txt" > "test/$base.trace.out"
  if cmp -s "test/$base.trace.out" "test/$base.trace"; then
    echo "ok    $label"
  else
    echo "FAIL  $label"; diff "test/$base.trace" "test/$base.trace.out" || true; fail=1
  fi
done

exit $fail
