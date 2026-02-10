# Amber 🌱
### A low-level, stack-based intermediate representation (IR) and virtual machine written in C for educational and experimental.

I built this project as a technical "proof of concept" to:

>- Experiment with manual stack management and instruction decoding in C.
>- Understand low-level control flow and jump-based logic.
>- Prototype a backend for my upcoming high-level language, Resin.
>

Amber operates as a Stack Machine (similar to the JVM). It uses a double-pass execution model where instructions like PUSH, DUP, and EQUAL manipulate a data stack, while labels provide the targets for conditional jumps.

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

### example code
```c
counting program
PUSH 5 #start value

<SubAndComp>
PRINT
PUSH 1 
SUB #subtract 1

DUP
PUSH 0
EQUAL
JUMP_IF_FALSE <SubAndComp>

```

to run, simply run

```bash
./amber <filename>
```
