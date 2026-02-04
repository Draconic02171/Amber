#!/bin/bash

# VM Test Runner
# Compile the VM first, then run all test files

echo "======================================"
echo "VM Test Suite Runner"
echo "======================================"
echo ""

# Compile the VM (assuming vm.c is the source file)
# echo "Compiling VM..."
# gcc -o vm vm.c -lm
# if [ $? -ne 0 ]; then
#     echo "ERROR: Failed to compile VM"
#     exit 1
# fi
# echo "✓ VM compiled successfully"
# echo ""

# Test files
tests=(
    "test_arithmetic.ember"
    "test_stack.ember"
    "test_comparison.ember"
    "test_jump.ember"
    "test_conditional.ember"
    "test_factorial.ember"
    "test_comprehensive.ember"
    "test_edge_cases.ember"
)

# Run each test
for test in "${tests[@]}"; do

    clear
    echo "======================================"
    echo "Running: $test"
    echo "======================================"
    ember "$test"
    echo ""
    echo "Press Enter to continue..."
    read
done

echo "======================================"
echo "All tests completed!"
echo "======================================"
