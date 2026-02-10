# VM Test Suite

This test suite provides comprehensive testing for your stack-based virtual machine.

## Test Files

### 1. `test_arithmetic.vm`
Tests basic arithmetic operations:
- Addition (ADD)
- Subtraction (SUB)
- Multiplication (MUL)
- Division (DIV)
- Complex expressions

**Expected Output:**
```
8.000000
6.000000
42.000000
5.000000
30.000000
20.000000
```

### 2. `test_stack.vm`
Tests stack manipulation operations:
- PUSH
- POP
- DUPLICATE

**Expected Output:**
```
42.000000
100.000000
100.000000
20.000000
21.000000
20.000000
```

### 3. `test_comparison.vm`
Tests comparison operations:
- EQUAL (==)
- LESS (<)
- MORE (>)

**Expected Output:**
```
1.000000
0.000000
1.000000
0.000000
1.000000
0.000000
1.000000
```

### 4. `test_jump.vm`
Tests jump operations:
- JUMP
- JUMP_IF_FALSE
- Labels

**Expected Output:**
```
100.000000
0.000000
777.000000
1.000000
2.000000
3.000000
```

### 5. `test_conditional.vm`
Tests conditional branching (if-else logic):
- Simulates if-else statements using jumps
- Tests multiple conditions

**Expected Output:**
```
1.000000
100.000000
-1.000000
```

### 6. `test_factorial.vm`
Tests a complex calculation:
- Factorial computation (5! = 120)

**Expected Output:**
```
120.000000
```

### 7. `test_comprehensive.vm`
Combines multiple operations:
- Arithmetic + comparisons + jumps
- Nested operations
- Multiple duplications

**Expected Output:**
```
20.000000
9999.000000
20.000000
777.000000
15.000000
```

### 8. `test_edge_cases.vm`
Tests edge cases:
- Negative numbers
- Zero operations
- Floating point numbers
- Large numbers

**Expected Output:**
```
-2.000000
-10.000000
20.000000
-5.000000
1.000000
5.000000
6.280000
1000000.000000
```

## Running the Tests

### Option 1: Run all tests automatically
```bash
./run_tests.sh
```

### Option 2: Run individual tests
```bash
# First compile the VM
gcc -o vm vm.c -lm

# Then run specific test
./vm test_arithmetic.vm
./vm test_stack.vm
# etc...
```

## Notes

- Each test file is self-contained and can be run independently
- Tests use comments (# prefix) for documentation
- Labels are defined using `<label_name>` syntax
- Expected outputs are documented in this README
