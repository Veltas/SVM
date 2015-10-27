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

/*
 * Each op should be defined here
 *  op info should be added in s_vm_init in svm_util.c
 *  op behavior should be added in s_eval in svm_eval.c
 */
enum class Op: byte
{
    // VM
    END = 0,   // nothing
    HALT,      // stops running
    DUMPBC,    // dumps bytecode
    DUMPNBC,   // dumps nice bytecode

    // Stack
    SPUSH,     // <value> [push 1] push element on stack
    SP,        // <value> [push 1] alais for spush
    SPOP,      // [pop 1] pops the last element
    SREV,      // reverses the whole stack
    SREVN,     // <value> reverses top of the stack with given length
    SDUMP,      // dumps the stack
    SDUP,      // [push 1] duplicates the last element on stack

    // Math
    MADD,      // [pop 2, push 1] math add
    MSUB,      // [pop 2, push 1] math subtract
    MMUL,      // [pop 2, push 1] math multipy
    MDIV,      // [pop 2, push 1] math divide

    // Increament decreament
    INC,       // [pop 1, push 1] increase the last element if possible
    DEC,       // [pop 1, push 1] decrease the last element if possible

    // Bitwise stuff
    XOR,       // [pop 2, push 1] bitwise xor
    AND,       // [pop 2, push 1] bitwise and
    OR,        // [pop 2, push 1] bitwise or
    NOT,       // [pop 1, push 1] bitwise not

    // Print
    PRINT,     // [pop 1] prints the last element on stack
    PRINTN,    // [pop n] prints count elements from stack

    // Comparations
    CMPE,      // [pop 2, push 1] compare equal
    CMPG,      // [pop 2, push 1] compare greater
    CMPGE,     // [pop 2, push 1] compare equal or greater

    // Jump
    JUMP,      // <number> jump to offset
    JUMPT,     // <number> [pop 1] jump to offset if last element on stack is true
    JUMPF,     // <number> [pop 1] jump to offset if last element on stack is false

    // These are used only by the compiler
    LABEL,     // <key> defines a label
    GOTO,      // <key> jumps to a label
    GOTOT,     // <key> [pop 1] jumps to a label if last element on stack is true
    GOTOF,     // <key> [pop 1] jumps to a label if last element on stack is true

    __MAX__
};
constexpr byte maxOps = static_cast<byte>(Op::__MAX__);

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
        Op op = Op::END;
    } m_opInfoList[maxOps];
};
extern OpInfo opInfo;

struct Context
{
    bool running = false;
    word opi = 0;
    word code_size = 0;
    byte *code = nullptr;
    word stack_count = 0;
    word stack[STACK_SZ] = {0};

    void setprogram(byte *program, word size);
    ~Context();
};

// eval
byte next(Context *C);
word getv(Context *C);
void eval(Context *C, byte op);

// stack
void push(Context *C, word v);
word pop(Context *C);
void reverse(Context *C, bool manualcount, word N);

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
