// svm.h

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
typedef uint16_t word;


typedef struct sSegment_s sSegment;
struct sSegment_s
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
typedef enum sOp_e sOp;
enum sOp_e
{
    S_OP_END = 0,
    S_OP_HALT,
    S_OP_DMPBC,
    S_OP_DMPBCN,

    S_OP_SPUSH,
    S_OP_SP,
    S_OP_SPOP,
    S_OP_SREV,
    S_OP_SDMP,
    S_OP_SDUP,

    S_OP_MADD,
    S_OP_MSUB,
    S_OP_MMUL,
    S_OP_MDIV,
    S_OP_INC,
    S_OP_DEC,

    S_OP_PUT,
    S_OP_PUTC,
    S_OP_PUTN,
    S_OP_PUTNC,
    S_OP_PUTALL,
    S_OP_PUTALLC,

    S_OP_CMPE,
    S_OP_CMPG,
    S_OP_CMPGE,

    S_OP_JMPBT,
    S_OP_JMPBF,
    S_OP_JMPB,
    S_OP_JMPFT,
    S_OP_JMPFF,
    S_OP_JMPF,
    S_OP_JMPTT,
    S_OP_JMPTF,
    S_OP_JMPT,

    S_OP__MAX__
};
typedef struct sOpInfo_s
{
    char name[16];
    bool hasreg;
    sOp op;
} sOpInfo;


// eval
byte s_next(sSegment *C);
word s_getv(sSegment *C);
void s_eval(sSegment *C, byte op);

// stack
void s_push(sSegment *C, word v);
word s_pop(sSegment *C);
void s_reverse(sSegment *C);

// context
sSegment* s_new();
void s_segment_setprogram(sSegment *C, byte *program, word size);

// util
void s_addinst(sSegment *C, byte O, word V);
void s_run(sSegment *C);
void s_vm_init();
void s_dump_bytecode(byte *code, word sz, bool nice);
void s_dump_stack(sSegment *S);

// opinfo
sOpInfo* s_opinfo(sOp O);
bool s_opinfo_hasreg(sOp O);
char* s_opinfo_name(sOp O);
sOp s_opinfo_find(char *name);

// compiler
byte* s_compile(char *program, word sz, word *compilesz);


#endif
