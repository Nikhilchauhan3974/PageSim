
#!/bin/bash

set -u

PROGRAM="./address_translation_demo"
PASSED=0
FAILED=0

echo "============================================"
echo " Address Translation Automated Tests"
echo "============================================"

if [ ! -x "$PROGRAM" ]; then
    echo "[ERROR] Program not found. Run: make address-translation"
    exit 1
fi

run_test() {
    name="$1"
    input="$2"
    expected="$3"

    output=$(printf '%s\n' "$input" | "$PROGRAM" 2>&1) || true

    if printf '%s\n' "$output" | grep -Fq "$expected"; then
        echo "[PASS] $name"
        PASSED=$((PASSED + 1))
    else
        echo "[FAIL] $name"
        echo "Expected: $expected"
        echo "Actual output:"
        printf '%s\n' "$output"
        FAILED=$((FAILED + 1))
    fi
}

run_test \
    "Valid address translation" \
    "2500" \
    "Physical Address: 452"

run_test \
    "Demand paging loads missing page" \
    "3500" \
    "Page fault handled: page loaded into physical memory."

run_test \
    "Out-of-range address rejected" \
    "9000" \
    "Error: Virtual address is out of range (0-8191)."

echo
echo "============================================"
echo "TEST SUMMARY"
echo "============================================"
echo "Passed: $PASSED"
echo "Failed: $FAILED"

if [ "$FAILED" -eq 0 ]; then
    echo "ALL ADDRESS TRANSLATION TESTS PASSED"
    exit 0
else
    echo "SOME TESTS FAILED"
    exit 1
fi
