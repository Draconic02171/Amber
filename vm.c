#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//macro for sleep
#ifdef _WIN32
    //use windows bed
    #include <windows.h>
#else
    //use unix bed
    #include <unistd.h>
#endif


#define RESET         "\x1b[0m"
#define BOLD          "\x1b[1m"
#define DIM           "\x1b[2m"
#define ITALIC        "\x1b[3m"
#define UNDERLINE     "\x1b[4m"
#define BLINK         "\x1b[5m"
#define REVERSE       "\x1b[7m"
#define HIDDEN        "\x1b[8m"
#define FG_COLOR(n)   "\x1b[38;5;" #n "m"
#define BG_COLOR(n)   "\x1b[48;5;" #n "m"

typedef struct {
    int opcode;
    double operand;
} Instruction;

typedef struct {
    char Label[64];
    double Line ;
} JumpLabel;

enum InstructCode {
    PUSH,                  //<constant>    //= push a literal number into the stack
    POP,                                   //= remove the top value of the stack
    DUP,                                   //= copy the top value
    SWAP,                                  //= swap 2 values
//
    LOAD,                  //<index>       //= read vars[index] → push to stack
    STORE,                 //<index>       //= pop value from stack → write to vars[index]
//
    ADD,                                   //= pop 2 values and push addition result of those values
    SUB,                                   //= pop 2 values and push subtraction result of those values
    MUL,                                   //= pop 2 values and push multiplication result of those values
    DIV,                                   //= pop 2 values and push multiplication result of those values
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
    CALL,                  //<label>       //= calling a function
//
    READ,                                  //= read a value from standard IO (terminal)
    PRINT,                                 //= just print the top value from the stack duh
    WRITE,                                 //= converting a top value into a ascii code and print it out
    SLEEP,                                 //= pop and sleep at second rate of the pop value
//
// Markers
    LABEL,                                 //= serves as a marker for JUMP, no real use in code
    CALL_LABEL,                            //= serves as a marker for JUMP, no real use in code
};





//===================================================================
//=== specs
double          Stack[64]           = {0}; //for operations 
double          VirtualMemory[256]  = {}; //for variables and memories
Instruction     ROM[2048]           = {0}; // for store instructions

int             StackPointer        = -1;
int             ROMPointer          = 0;
int             StackSize           = sizeof(Stack) / sizeof(Stack[0]);
int             VRAMSize            = sizeof(VirtualMemory) / sizeof(VirtualMemory[0]);
//===================================================================



//===============================================================
//=== Utills Functions 
void Message(char msg[]);
void RaiseError(char title[] ,char msg[]);
void RaiseErrorAtLine(int Line, char instruction[] , char msg[]);

//=== Operation Functions
double pop();
void push(double value);
void duplicate();
void swap();

void store(double address);
void load(double address);

void add();
void subtract();
void multiply();
void divine();

void equal();
void more();
void less();

void jump(double line);
void jumpIfTrue(double line);
void jumpIfFalse(double line);

void read();
void print();
void write();
//===============================================================





//=== Virtual Machine Functions
int Match(char *String, char start, char stop) {
 
    while (*String == ' ' || *String == '\t') String++;

    int len = strlen(String);
    if (len < 2) return 0;

    int end = len - 1;
    while (end > 0 && (String[end] == ' ' || String[end] == '\t'))
        end--;

    return (String[0] == start && String[end] == stop);
}

int LocateLabel(JumpLabel *array , int Size , char *Target) {

    for (int index = 0 ; index < Size ; index ++) {
        if (strcmp(array[index].Label, Target) == 0) {
            return index;
        }
    }
    
    return -1;
}

int FetchInstructions(FILE* file) {

    char buffer[256];
    int count = 0;
    JumpLabel jumpLabel[256];
    int LabelCount = 0; 
    JumpLabel functionLabel[256];

    int ActualLineInFile = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        
        char label[64];
        int n;
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (buffer[0] == '\0' || buffer[0] == '#') continue;
        
        if (sscanf(buffer, " <%63[^>]> %n", label, &n) == 1) {

            strcpy(jumpLabel[LabelCount].Label, label);
            jumpLabel[LabelCount].Line = count;
            LabelCount++;

        }
    
        count++;
    }

    count = 0;
    rewind(file);

    while (fgets(buffer, sizeof(buffer), file)) {

        char temp[64];
        ActualLineInFile ++;

        int parsed = sscanf(buffer, "%s %lf", temp , &ROM[count].operand); // check if it parsed successfully or not, if yes how many

        temp[strcspn(temp, "\n")] = '\0';

        if (parsed < 1 || temp[0] == '#') continue; //skip an empty line or comment line

        if (!strcmp(temp, "PUSH"))
        {   if (parsed == 1) {

                //trying to read for finding character
                char character[5];
                int success = 0;
                
                {
                    int _index = 0;
                    int _charIndex = 0;
                    int startRead = 0;

                    while (buffer[_index] != '\n') {

                        if (_index >= sizeof(buffer) || _charIndex >= sizeof(character)) {break;}
                        
                        if (buffer[_index] == '\'') {startRead = 1;}
                        if (!startRead) {_index ++; continue;}

                        character[_charIndex] = buffer[_index];

                        _charIndex ++;
                        _index ++;

                    }
                    success = 1;
                }


                if (success != 1) {RaiseErrorAtLine(ActualLineInFile, buffer, "PUSH expected numeric value or character value (e.g. PUSH 5 , PUSH 10.5 , PUSH -667.345 , PUSH 'a')"); return 1;}

                if (!Match(character , '\'', '\'')) {RaiseErrorAtLine(ActualLineInFile, buffer, "PUSH expected numeric value or character value (e.g. PUSH 5 , PUSH 10.5 , PUSH -667.345 , PUSH 'a')"); return 1;}

                if (character[1] == '\\') {
                    switch (character[2]) {
                        case 'n': ROM[count].operand  = 10; break;
                        case 't': ROM[count].operand  = 9;  break;
                        case 'r': ROM[count].operand  = 13; break;
                        case '0': ROM[count].operand  = 0;  break;
                        case '\\': ROM[count].operand  = 92; break;
                        case '\'': ROM[count].operand  = 39; break;
                        case '\"': ROM[count].operand  = 34; break;
                        case '\?': ROM[count].operand  = 63; break;
                        case '\v': ROM[count].operand  = 11; break;
                        case '\f': ROM[count].operand  = 12; break; 
                        default: {RaiseErrorAtLine(ActualLineInFile, buffer, "unknown character type. current vm support '\\n , \\t , \\r , \\0 , \\\\, \\\' , \\\" , \\? , \\v , \\f"); return 1;}
                    }
                } 
                else 
                {
                    // printf("%c" , (char) character[1]);
                    ROM[count].operand = (double) character[1]; 
                }

                // printf("%c" , (char) ROM[count].operand);
            }
        }
        else if (!strcmp(temp, "LOAD")) {if (parsed == 1) {RaiseErrorAtLine(ActualLineInFile, buffer, "LOAD expected expected an integer memory address (e.g. LOAD 5)"); return 1;}}
        else if (!strcmp(temp, "STORE")) {if (parsed == 1) {RaiseErrorAtLine(ActualLineInFile, buffer, "STORE expected expected an integer memory address (e.g. STORE 5)"); return 1;}}
        
        if (
            !strcmp(temp, "JUMP") || 
            !strcmp(temp, "JUMP_IF_TRUE") || 
            !strcmp(temp, "JUMP_IF_FALSE")
        ) {
            
            char Label[64];
            int findLabel = sscanf(buffer , "%*s <%63[^>]>", Label);

            if (findLabel < 1) {
                RaiseErrorAtLine(
                    ActualLineInFile, 
                    buffer, 
                    "missing the label or incorrect way to specified a lebel to jump (e.g. JUMP <label>)");
            }
            
            int GoToLine = LocateLabel(jumpLabel, 256, Label);
            if (GoToLine < 0) {
                RaiseErrorAtLine(
                    ActualLineInFile, 
                    buffer, 
                    "Invalid Line or unable to locate the label line"
                );
            }
            ROM[count].operand = jumpLabel[GoToLine].Line;
        }

        int opcode;

        if      (strcmp(temp, "PUSH") == 0)             opcode = PUSH;
        else if (strcmp(temp, "POP") == 0)              opcode = POP;
        else if (strcmp(temp, "DUP") == 0)              opcode = DUP;
        else if (strcmp(temp, "SWAP") == 0)             opcode = SWAP;

        else if (strcmp(temp, "LOAD") == 0)             opcode = LOAD;
        else if (strcmp(temp, "STORE") == 0)            opcode = STORE;

        else if (strcmp(temp, "ADD") == 0)              opcode = ADD;
        else if (strcmp(temp, "SUB") == 0)              opcode = SUB;
        else if (strcmp(temp, "MUL") == 0)              opcode = MUL;
        else if (strcmp(temp, "DIV") == 0)              opcode = DIV;

        else if (strcmp(temp, "EQUAL") == 0)            opcode = EQUAL;
        else if (strcmp(temp, "LESS") == 0)             opcode = LESS;
        else if (strcmp(temp, "MORE") == 0)             opcode = MORE;

        else if (strcmp(temp, "JUMP") == 0)             opcode = JUMP;
        else if (strcmp(temp, "JUMP_IF_TRUE") == 0)     opcode = JUMP_IF_TRUE;
        else if (strcmp(temp, "JUMP_IF_FALSE") == 0)    opcode = JUMP_IF_FALSE;

        else if (strcmp(temp, "READ") == 0)             opcode = READ;
        else if (strcmp(temp, "PRINT") == 0)            opcode = PRINT;
        else if (strcmp(temp, "WRITE") == 0)            opcode = WRITE;
        else if (Match(temp , '<' , '>'))   opcode = LABEL;
        else {
            RaiseErrorAtLine(ActualLineInFile , temp, "Syntax error"); return -1;
        }

        ROM[count].opcode = opcode;

        // printf("%d\t%d\t%lf\n" ,count , ROM[count].opcode , ROM[count].operand);

        count++; 
    }

    return count; // number of instructions
}

int Execute(int Size) {

    while (ROMPointer < Size) {

        switch (ROM[ROMPointer].opcode) {
            case PUSH           : push(ROM[ROMPointer].operand); break;
            case POP            : pop(); break;
            case DUP            : duplicate(); break;
            case SWAP           : swap(); break;

            case STORE          : store(ROM[ROMPointer].operand); break;
            case LOAD           : load(ROM[ROMPointer].operand); break;

            case ADD            : add(); break;
            case SUB            : subtract(); break;
            case MUL            : multiply(); break;
            case DIV            : divine(); break;

            case EQUAL          : equal(); break;
            case MORE           : more(); break;
            case LESS           : less(); break;

            case JUMP           : jump(ROM[ROMPointer].operand); break;
            case JUMP_IF_TRUE   : jumpIfTrue(ROM[ROMPointer].operand); break;
            case JUMP_IF_FALSE  : jumpIfFalse(ROM[ROMPointer].operand); break;

            case READ           : read(); break;
            case PRINT          : print(); break;
            case WRITE          : write(); break;   

            default: break;
        }

        // printf(
        //     "%d : %lf = %lf\n" , 
        //     ROM[ROMPointer].opcode,
        //     ROM[ROMPointer].operand,
        //     Stack[StackPointer]
        // );

        ROMPointer ++;
    }

    return 0;
}

int main(int argc, char* argv[]) {

    printf("Starting VM\n");
    printf("Stack Size : %d\nVirtualMemory Size : %d\n" , StackSize , VRAMSize);
    printf(FG_COLOR(2) BOLD "\n|| Output ||\n" RESET);

    if (argc < 2) {RaiseError("IO Error" , "Please feed the instruction file"); return 1;}

    for (int index = 0; index < VRAMSize; index ++) {
        VirtualMemory[index] = NAN;
    }

    FILE *file = fopen(argv[1] , "r");
    if (!file) {RaiseError("IO Error" , "Failed to open a file"); return 1;}

    int TotalLine = FetchInstructions(file);

    Execute(TotalLine);

    return 0;
}

//=== OPERATION
double pop() {

    if (StackPointer < 0) {
        RaiseError("Runtime Error" , "attempted to pop an empty stack (Stack Underflow)");
        exit(1);
    }

    double value = Stack[StackPointer];
    StackPointer --;

    return value;
}
void push(double value) {

    if (StackPointer >= StackSize) {
        RaiseError("Runtime Error" , "attempted to push a full stack (Stack Overflow)");
        exit(1);
    }

    StackPointer++;
    Stack[StackPointer] = value;
}
void duplicate() {
    if (StackPointer < 0) {
        RaiseError("Runtime Error", "attempted to duplicate an empty stack");
        exit(1);
    }
    if (StackPointer >= StackSize - 1) {  // ADD THIS
        RaiseError("Runtime Error", "Stack overflow during duplicate");
        exit(1);
    }
    push(Stack[StackPointer]);
}
void swap() {
    double a = pop(); //top
    double b = pop(); //bottom

    push(a); //bottom
    push(b); //top
}

void store(double address) {

    if (address >= VRAMSize) {
        RaiseError(
            "Out of memory bound",
             "Trying to store value to address that doesn't exist (exceeds the virtual memory size)");
    }
    else if (address < 0) {
        RaiseError(
            "Out of memory bound",
            "Trying to store value to address that doesn't exist (negative address)"
        );
    }
    VirtualMemory[(int) address] = pop();
}
void load(double address) { 
    if (address >= VRAMSize) {
        RaiseError(
            "Out of memory bound",
             "Trying to access address that doesn't exist (exceeds the virtual memory size)");
    }
    else if (isnan(VirtualMemory[(int) address])) {
        RaiseError(
            "Invalid access",
            "Trying to get the value from unassigned address (no value in that address)"
        );
    }
    push(VirtualMemory[(int) address]);
}

void add() {
    double right = pop();
    double left = pop();
    push(left + right);
}
void subtract() {
    double right = pop();
    double left = pop();
    push(left - right);
}
void multiply() {
    double right = pop();
    double left = pop();
    push(left * right);
}
void divine() {
    double right = pop();
    double left = pop();
    if (right == 0) RaiseError("Arithmetic Error", "Division by zero");
    push(left / right);
}

void equal() {
    double right = pop();
    double left = pop();
    push(left == right ? 1 : 0); 
}
void more() {
    double right = pop();
    double left = pop();
    push(left > right ? 1 : 0); 
}
void less() {
    double right = pop();
    double left = pop();
    push(left < right ? 1 : 0); 
}
void jump(double line) {
    ROMPointer = (int) line;
}       
void jumpIfTrue(double line) {
    double value = pop();
    if (value == 1) {jump(line);}
}
void jumpIfFalse(double line) {
    double value = pop();
    if (value == 0) {jump(line);}
}

void read() {
    char buffer[128];

    if (!fgets(buffer, sizeof(buffer), stdin)) {
        RaiseError("IO error", "Input stream closed or read error");
        exit(1);
    }
    
    char *end;
    double value = strtod(buffer, &end);

    char *pointer = end;
    while (*pointer == ' ' || *pointer == '\t') pointer++;

    if (*pointer != '\n' && *pointer != '\0') {
        RaiseError("IO error", "Invalid number format");
    }
    
    push(value);
}
void print() {
    if (StackPointer < 0) {
        RaiseError("Runtime Error", "print from empty stack (illegal)");
        return;
    }
    printf("%lf\n" , Stack[StackPointer]);
}
void write() {
    double character = pop();
    printf("%c" , (char) character);
}
void sleep() {
}
//=== DEBUG

void Message(char msg[]) {
    printf(FG_COLOR(1) BOLD "VM : " RESET FG_COLOR(1)  "%s" RESET "\n" , msg);
}
void RaiseError(char title[] ,char msg[]) {
    printf(FG_COLOR(1) BOLD "ERROR : " RESET FG_COLOR(1)  "%s" RESET "\n" , title);
    printf(FG_COLOR(1) BOLD "Description : " RESET FG_COLOR(1)  "%s" RESET "\n" , msg);
    exit(1);
}
void RaiseErrorAtLine(int Line, char instruction[] , char msg[]) {
    printf(FG_COLOR(1) BOLD "ERROR AT LINE :"RESET FG_COLOR(1)" %d" RESET"\n" ,Line);
    printf(FG_COLOR(1) BOLD "Instruction :"RESET FG_COLOR(1)" \"%s\"" RESET"\n" ,instruction);
    printf(FG_COLOR(1) BOLD "Description :"RESET FG_COLOR(1)" %s" RESET"\n" ,msg);
    exit(1);
}
