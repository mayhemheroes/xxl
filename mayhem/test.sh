#!/usr/bin/env bash
#
# mayhem/test.sh — RUN the upstream XXL selftest built by mayhem/build.sh (/mayhem/xxl-test).
#
# XXL's own test suite is its built-in selftest: built -DDEBUG and invoked with no args, main() ->
# args() -> selftest() runs test_basics/nest/ctx/eval/logic/semantics. Each check is an ASSERT()
# that raises SIGABRT + exit(1) on failure and the runner prints "TESTS PASSED" only when all six
# groups pass. This asserts real behavior/values (not just exit status): if the interpreter is
# neutered to exit(0), no group runs and "TESTS PASSED" never prints -> this oracle FAILS.
set -uo pipefail
[ -n "${SOURCE_DATE_EPOCH:-}" ] || unset SOURCE_DATE_EPOCH
cd "$SRC"

emit_ctrf() {
  local tool="$1" passed="$2" failed="$3" skipped="${4:-0}" pending="${5:-0}" other="${6:-0}"
  local tests=$(( passed + failed + skipped + pending + other ))
  cat > "${CTRF_REPORT:-$SRC/ctrf-report.json}" <<JSON
{
  "results": {
    "tool": { "name": "$tool" },
    "summary": {
      "tests": $tests,
      "passed": $passed,
      "failed": $failed,
      "pending": $pending,
      "skipped": $skipped,
      "other": $other
    }
  }
}
JSON
  printf 'CTRF {"results":{"tool":{"name":"%s"},"summary":{"tests":%d,"passed":%d,"failed":%d,"pending":%d,"skipped":%d,"other":%d}}}\n' \
    "$tool" "$tests" "$passed" "$failed" "$pending" "$skipped" "$other"
  [ "$failed" -eq 0 ]
}

RUNNER=/mayhem/xxl-test
if [ ! -x "$RUNNER" ]; then
  echo "test runner $RUNNER missing — build.sh must produce it" >&2
  emit_ctrf "xxl-selftest" 0 1
  exit 1
fi

# The six built-in test groups (each aborts the process on the first failed ASSERT).
# NB: do NOT name this GROUPS — that's a readonly bash special variable.
TEST_GROUPS="TEST_BASICS TEST_NEST TEST_CTX TEST_EVAL TEST_LOGIC TEST_SEMANTICS"
TOTAL=$(echo $TEST_GROUPS | wc -w)

# No args -> selftest; feed /dev/null on stdin so the post-test repl reads EOF and exits.
out="$("$RUNNER" </dev/null 2>&1)"
echo "$out"

passed=0
for g in $TEST_GROUPS; do
  echo "$out" | grep -q "^$g$" && passed=$((passed+1))
done

if echo "$out" | grep -q "^TESTS PASSED$" && ! echo "$out" | grep -q "^ASSERT:"; then
  emit_ctrf "xxl-selftest" "$TOTAL" 0
else
  # A group that ran but did not reach TESTS PASSED means an ASSERT fired -> at least one failure.
  failed=$(( TOTAL - passed ))
  [ "$failed" -lt 1 ] && failed=1
  emit_ctrf "xxl-selftest" "$passed" "$failed"
fi
