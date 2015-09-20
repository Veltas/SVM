// svm_util.c


#include "svm.h"

void s_setopinfo_(sOp O, char *name, bool hasreg);
void s_vm_init()
{
    s_setopinfo_(S_OP_END,      "end",      false);
    s_setopinfo_(S_OP_HALT,     "halt",     false);
    s_setopinfo_(S_OP_SPUSH,    "spush",    true);
    s_setopinfo_(S_OP_SP,       "sp",       true);
    s_setopinfo_(S_OP_SPOP,     "spop",     false);
    s_setopinfo_(S_OP_SREV,     "srev",     false);
    s_setopinfo_(S_OP_MADD,     "madd",     false);
    s_setopinfo_(S_OP_MSUB,     "msub",     false);
    s_setopinfo_(S_OP_MMUL,     "mmul",     false);
    s_setopinfo_(S_OP_MDIV,     "mdiv",     false);
    s_setopinfo_(S_OP_PUT,      "put",      false);
    s_setopinfo_(S_OP_PUTC,     "putc",     false);
    s_setopinfo_(S_OP_PRINT,    "print",    false);
    s_setopinfo_(S_OP_PRINTC,   "printc",   false);
}

void s_run(sSegment *C)
{
    C->running = true;
    while (C->running)
    {
        byte op = s_next(C);
        s_eval(C, op);
    }
}

void s_addinst(sSegment *C, byte O, word V)
{
    word newsize = 1;
    if (s_opinfo_hasreg(O))
        newsize += sizeof(word);
    byte *newbuff = calloc(C->code_size + newsize, 1);

    if (C->code && C->code_size > 0)
        memcpy(newbuff, C->code, C->code_size);

    newbuff[C->code_size] = O;
    memcpy(newbuff + C->code_size + 1, &V, sizeof V);

    if (C->code && C->code_size > 0)
        free(C->code);

    C->code_size += newsize;
    C->code = newbuff;
}

void s_dump_bytecode(byte *code, word sz)
{
    printf("[ BYTECODE DUMP -");
    for (word i = 0; i < sz; i++)
        printf("%d-", code[i]);
    printf("]\n");
}
