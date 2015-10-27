#if !defined(SVM_H_)
#define SVM_H_


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define STACK_SZ 128


typedef uint8_t byte;
typedef int16_t word;


struct sContext
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
enum sOp
{
    // VM
    S_OP_END = 0,   // nothing
    S_OP_HALT,      // stops running
    S_OP_DUMPBC,    // dumps bytecode
    S_OP_DUMPNBC,   // dumps nice bytecode

    // Stack
    S_OP_SPUSH,     // <value> [push 1] push element on stack
    S_OP_SP,        // <value> [push 1] alais for spush
    S_OP_SPOP,      // [pop 1] pops the last element
    S_OP_SREV,      // reverses the whole stack
    S_OP_SREVN,     // <value> reverses top of the stack with given length
    S_OP_SDUMP,      // dumps the stack
    S_OP_SDUP,      // [push 1] duplicates the last element on stack

    // Math
    S_OP_MADD,      // [pop 2, push 1] math add
    S_OP_MSUB,      // [pop 2, push 1] math subtract
    S_OP_MMUL,      // [pop 2, push 1] math multipy
    S_OP_MDIV,      // [pop 2, push 1] math divide

    // Increament decreament
    S_OP_INC,       // [pop 1, push 1] increase the last element if possible
    S_OP_DEC,       // [pop 1, push 1] decrease the last element if possible

    // Bitwise stuff
    S_OP_XOR,       // [pop 2, push 1] bitwise xor
    S_OP_AND,       // [pop 2, push 1] bitwise and
    S_OP_OR,        // [pop 2, push 1] bitwise or
    S_OP_NOT,       // [pop 1, push 1] bitwise not

    // Print
    S_OP_PRINT,     // [pop 1] prints the last element on stack
    S_OP_PRINTN,    // [pop n] prints count elements from stack

    // Comparations
    S_OP_CMPE,      // [pop 2, push 1] compare equal
    S_OP_CMPG,      // [pop 2, push 1] compare greater
    S_OP_CMPGE,     // [pop 2, push 1] compare equal or greater

    // Jump
    S_OP_JUMP,      // <number> jump to offset
    S_OP_JUMPT,     // <number> [pop 1] jump to offset if last element on stack is true
    S_OP_JUMPF,     // <number> [pop 1] jump to offset if last element on stack is false

    // These are used only by the compiler
    S_OP_LABEL,     // <key> defines a label
    S_OP_GOTO,      // <key> jumps to a label
    S_OP_GOTOT,     // <key> [pop 1] jumps to a label if last element on stack is true
    S_OP_GOTOF,     // <key> [pop 1] jumps to a label if last element on stack is true

    S_OP__MAX__
};
struct sOpInfo
{
    char name[32];
    bool hasreg;
    sOp op;
};


// eval
byte s_next(sContext *C);
word s_getv(sContext *C);
void s_eval(sContext *C, byte op);

// stack
void s_push(sContext *C, word v);
word s_pop(sContext *C);
void s_reverse(sContext *C, bool manualcount, word N);

// context
sContext* s_new();
void s_context_setprogram(sContext *C, byte *program, word size);

// util
void s_addinst(sContext *C, byte O, word V);
void s_run(sContext *C);
void s_vm_init();
void s_dump_bytecode(byte *code, word sz, bool nice);
void s_dump_stack(sContext *S);

// opinfo
sOpInfo* s_opinfo(sOp O);
bool s_opinfo_hasreg(sOp O);
char* s_opinfo_name(sOp O);
sOp s_opinfo_find(char *name);

// compiler
bool s_compile(char *program, word sz, byte **pcompiled, word *compiledsize);


#endif
