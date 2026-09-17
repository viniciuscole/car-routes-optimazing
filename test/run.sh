#!/bin/bash
set -euo pipefail
export ASAN_OPTIONS=detect_leaks=0
cd "$(dirname "$0")/.."

gcc -std=gnu99 -O1 -g -fsanitize=address,undefined -Wall -o trab2 $(ls src/*.c | grep -v trace_main.c) -lm
gcc -std=gnu99 -O1 -g -fsanitize=address,undefined -Wall -o trace $(ls src/*.c | grep -v '/main.c') -lm

fail=0
for case in with_update no_update long_line bad_update; do
  rc=0
  ./trab2 "test/$case.txt" "test/$case.out" 2> "test/$case.err" || rc=$?
  if [ "$rc" -eq 0 ] && cmp -s "test/$case.out" "test/$case.expected"; then
    echo "ok    $case"
  else
    echo "FAIL  $case (rc=$rc)"; diff "test/$case.expected" "test/$case.out" || true; fail=1
  fi
done

for pair in trace:with_update:0 unreachable:unreachable:0 bad_edge:bad_edge:3 bad_from:bad_from:0; do
  label="${pair%%:*}"
  rest="${pair#*:}"
  base="${rest%%:*}"
  expected_rc="${rest##*:}"
  rc=0
  ./trace < "test/$base.txt" > "test/$base.trace.out" 2> "test/$base.err" || rc=$?
  if [ "$rc" -eq "$expected_rc" ] && cmp -s "test/$base.trace.out" "test/$base.trace"; then
    echo "ok    $label"
  else
    echo "FAIL  $label (rc=$rc expected=$expected_rc)"; diff "test/$base.trace" "test/$base.trace.out" || true; fail=1
  fi
done

exit $fail
