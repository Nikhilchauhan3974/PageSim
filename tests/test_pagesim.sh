#!/bin/bash

echo "============================================"
echo "        PageSim Automated Test Suite"
echo "============================================"
echo

# Make sure the simulator exists
if [ ! -f "./pagesim" ]; then
    echo "[FAIL] pagesim executable not found."
    echo "Run 'make' first."
    exit 1
fi

PASS=0
FAIL=0

run_test() {
    TEST_NAME="$1"
    INPUT="$2"
    EXPECTED="$3"

    echo "--------------------------------------------"
    echo "Test: $TEST_NAME"
    echo "--------------------------------------------"

    OUTPUT=$(printf "%s\n" "$INPUT" | ./pagesim)

    if echo "$OUTPUT" | grep -q "$EXPECTED"; then
        echo "[PASS] $TEST_NAME"
        PASS=$((PASS + 1))
    else
        echo "[FAIL] $TEST_NAME"
        FAIL=$((FAIL + 1))
    fi

    echo
}

# ============================================
# Test 1: FIFO
# ============================================

run_test \
    "FIFO algorithm" \
    "1" \
    "Replacement algorithm : FIFO"

# ============================================
# Test 2: LRU
# ============================================

run_test \
    "LRU algorithm" \
    "2" \
    "Replacement algorithm : LRU"

# ============================================
# Test 3: Optimal
# ============================================

run_test \
    "Optimal algorithm" \
    "3" \
    "Replacement algorithm : OPTIMAL"

# ============================================
# Test 4: Clock
# ============================================

run_test \
    "Clock / Second Chance algorithm" \
    "4" \
    "Replacement algorithm : CLOCK / SECOND CHANCE"

# ============================================
# Test 5: FIFO statistics
# ============================================

run_test \
    "FIFO statistics" \
    "1" \
    "Page faults      : 9"

# ============================================
# Test 6: LRU statistics
# ============================================

run_test \
    "LRU statistics" \
    "2" \
    "Page faults      : 8"

# ============================================
# Test 7: Optimal statistics
# ============================================

run_test \
    "Optimal statistics" \
    "3" \
    "Page faults      : 7"

# ============================================
# Test 8: Clock statistics
# ============================================

run_test \
    "Clock statistics" \
    "4" \
    "Page faults      : 9"

# ============================================
# Final result
# ============================================

echo "============================================"
echo "              TEST SUMMARY"
echo "============================================"

echo "Passed : $PASS"
echo "Failed : $FAIL"

echo

if [ "$FAIL" -eq 0 ]; then
    echo "ALL TESTS PASSED"
    exit 0
else
    echo "SOME TESTS FAILED"
    exit 1
fi
