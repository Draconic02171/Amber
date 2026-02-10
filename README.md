# Amber 🌱
### An intermediate representation language made for educational and experimental.

I made this language for testing, experimenting, and prepare myself for building a refined version of VM for my high level interpret language

``` c 
//Instructions
    PUSH,                  //<constant>    //= push a literal number into the stack
    POP,                                   //= remove the top value of the stack
    DUP,                                   //= copy the top value
    SWAP,                                  //= swap 2 values
//
    LOAD,                  //<index>       //= read vars[index] → push to stack
    STORE,                 //<index>       //= pop value from stack → write to vars[index]
// unused
    ADD,                                   //= pop 2 values and push addition result of those values
    SUB,                                   //= pop 2 values and push subtraction result of those values
    MUL,                                   //= pop 2 values and push multiplication result of those values
    DIV,                                   //= pop 2 values and push division result of those values
//
//
    EQUAL,                                 //= pop 2 values and push 1 (true) or 0 (false) 
    LESS,                                  //= pop 2 values and push 1 (true) or 0 (false) 
    MORE,                                  //= pop 2 values and push 1 (true) or 0 (false) 
//
    JUMP,                  //<label>       //= set instruction pointer to <line>
    JUMP_IF_TRUE,          //<label>       //= pop value if value == 1, jump else continue
    JUMP_IF_FALSE,         //<label>       //= pop value if value == 0, jump else continue
//
    READ,                                  //= read a value from standard IO (terminal)
    PRINT,                                 //= just print the top value from the stack duh
    WRITE,                                 //= converting a top value into a ascii code and print it out
//
// Markers
    LABEL,                                 //= serves as a marker for JUMP, no real use in code
```

How to write a jump?

It requires a label marker and actual jump line. Looks something like this in the code


```c

<loop>
PUSH 5
ADD

JUMP <loop>

#or

PUSH 5

<comp>
DUP
PUSH 20
EQUAL
JUMP_IF_TRUE <end>

PUSH 1
ADD
JUMP <comp>

<end>
PRINT
```

to run, simply run

```bash
./amber <filename>
```
