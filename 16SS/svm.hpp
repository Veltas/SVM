#if !defined(SVM_H_)
#define SVM_H_


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define STACK_SZ 128


namespace svm {

typedef uint8_t byte;
typedef int16_t word;


struct Context
{
    bool running;
    word opi;
    word code_size;
    byte *code;
    word stack_count;
    word stack[STACK_SZ];
};

/*
 * Each op should be defined here
 *  op info should be added in s_vm_init in svm_util.c
 *  op behavior should be added in s_eval in svm_eval.c
 */
enum Op
{
    // VM
    OP_END = 0,   // nothing
    OP_HALT,      // stops running
    OP_DUMPBC,    // dumps bytecode
    OP_DUMPNBC,   // dumps nice bytecode

    // Stack
    OP_SPUSH,     // <value> [push 1] push element on stack
    OP_SP,        // <value> [push 1] alais for spush
    OP_SPOP,      // [pop 1] pops the last element
    OP_SREV,      // reverses the whole stack
    OP_SREVN,     // <value> reverses top of the stack with given length
    OP_SDUMP,      // dumps the stack
    OP_SDUP,      // [push 1] duplicates the last element on stack

    // Math
    OP_MADD,      // [pop 2, push 1] math add
    OP_MSUB,      // [pop 2, push 1] math subtract
    OP_MMUL,      // [pop 2, push 1] math multipy
    OP_MDIV,      // [pop 2, push 1] math divide

    // Increament decreament
    OP_INC,       // [pop 1, push 1] increase the last element if possible
    OP_DEC,       // [pop 1, push 1] decrease the last element if possible

    // Bitwise stuff
    OP_XOR,       // [pop 2, push 1] bitwise xor
    OP_AND,       // [pop 2, push 1] bitwise and
    OP_OR,        // [pop 2, push 1] bitwise or
    OP_NOT,       // [pop 1, push 1] bitwise not

    // Print
    OP_PRINT,     // [pop 1] prints the last element on stack
    OP_PRINTN,    // [pop n] prints count elements from stack

    // Comparations
    OP_CMPE,      // [pop 2, push 1] compare equal
    OP_CMPG,      // [pop 2, push 1] compare greater
    OP_CMPGE,     // [pop 2, push 1] compare equal or greater

    // Jump
    OP_JUMP,      // <number> jump to offset
    OP_JUMPT,     // <number> [pop 1] jump to offset if last element on stack is true
    OP_JUMPF,     // <number> [pop 1] jump to offset if last element on stack is false

    // These are used only by the compiler
    OP_LABEL,     // <key> defines a label
    OP_GOTO,      // <key> jumps to a label
    OP_GOTOT,     // <key> [pop 1] jumps to a label if last element on stack is true
    OP_GOTOF,     // <key> [pop 1] jumps to a label if last element on stack is true

    OP__MAX__
};

class OpInfo
{
public:
    void set(Op O, const char *name, bool hasreg);
    bool hasreg(Op O);
    char* name(Op O);
    Op find(char *name);
    
private:
    struct
    {
        char name[32] = "";
        bool hasreg = false;
        Op op = OP_END;
    } m_opInfoList[OP__MAX__];
};
extern OpInfo opInfo;

// eval
byte next(Context *C);
word getv(Context *C);
void eval(Context *C, byte op);

// stack
void push(Context *C, word v);
word pop(Context *C);
void reverse(Context *C, bool manualcount, word N);

// context
Context* newcontext();
void context_setprogram(Context *C, byte *program, word size);

// util
void addinst(Context *C, byte O, word V);
void run(Context *C);
void vm_init();
void dump_bytecode(byte *code, word sz, bool nice);
void dump_stack(Context *S);

// compiler
bool compile(char *program, word sz, byte **pcompiled, word *compiledsize);

} // namespace svm


#endif
