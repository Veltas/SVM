// svm_util.c


#include "svm.h"
#include <ctype.h>


void s_setopinfo_(sOp O, char *name, bool hasreg);
void s_vm_init()
{
    s_setopinfo_(S_OP_END, "end", false);
    s_setopinfo_(S_OP_HALT, "halt", false);
    s_setopinfo_(S_OP_DUMPBC, "dumpbc", false);
    s_setopinfo_(S_OP_DUMPNBC, "dumpnbc", false);

    s_setopinfo_(S_OP_SPUSH, "spush", true);
    s_setopinfo_(S_OP_SP, "sp", true);
    s_setopinfo_(S_OP_SPOP, "spop", false);
    s_setopinfo_(S_OP_SREV, "srev", false);
    s_setopinfo_(S_OP_SREVN, "srevn", true);
    s_setopinfo_(S_OP_SDUMP, "sdump", false);
    s_setopinfo_(S_OP_SDUP, "sdup", false);

    s_setopinfo_(S_OP_MADD, "madd", false);
    s_setopinfo_(S_OP_MSUB, "msub", false);
    s_setopinfo_(S_OP_MMUL, "mmul", false);
    s_setopinfo_(S_OP_MDIV, "mdiv", false);

    s_setopinfo_(S_OP_INC, "inc", false);
    s_setopinfo_(S_OP_DEC, "dec", false);

    s_setopinfo_(S_OP_XOR, "xor", false);
    s_setopinfo_(S_OP_AND, "and", false);
    s_setopinfo_(S_OP_OR, "or", false);
    s_setopinfo_(S_OP_NOT, "not", false);

    s_setopinfo_(S_OP_PRINT, "print", false);
    s_setopinfo_(S_OP_PRINTN, "printn", true);

    s_setopinfo_(S_OP_CMPE, "cmpe", false);
    s_setopinfo_(S_OP_CMPG, "cmpg", false);
    s_setopinfo_(S_OP_CMPGE, "cmpge", false);

    s_setopinfo_(S_OP_JUMP, "jump", true);
    s_setopinfo_(S_OP_JUMPT, "jumpt", true);
    s_setopinfo_(S_OP_JUMPF, "jumpf", true);

    s_setopinfo_(S_OP_LABEL, "label", true);
    s_setopinfo_(S_OP_GOTO, "goto", true);
    s_setopinfo_(S_OP_GOTOT, "gotot", true);
    s_setopinfo_(S_OP_GOTOF, "gotof", true);

    //s_setopinfo_(S_OP_, "", false);
}

void s_run(sContext *C)
{
    C->running = true;
    while (C->running)
    {
        byte op = s_next(C);
        s_eval(C, op);
    }
}

void s_addinst(sContext *C, byte O, word V)
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

void s_dump_bytecode(byte *code, word sz, bool nice)
{
    if (!nice)
    {
        printf("[ BYTECODE DUMP -");
        for (word i = 0; i < sz; i++)
            printf("%d-", code[i]);
        printf("]\n");
    }
    else
    {
        size_t buffersz = 0;
        byte regsz = 5;
        byte opsz = 0; // len OpInfo.name
        byte addrsz = 6;
        byte spacebtwopandreg = 2;
        byte leftmargin = 3;
        byte spacebtwaddrandop = 2;

        // first calculate needed size
        for (word i = 0; i < sz; i++)
        {
            sOp O = code[i];
            opsz = strlen(s_opinfo_name(O));
            if (s_opinfo_hasreg(O))
            {
                buffersz += regsz;
                buffersz += spacebtwopandreg;
                i += 2; // i++ in loop will do another inc
            }
            buffersz += opsz;
            buffersz += addrsz;
            buffersz += leftmargin;
            buffersz += spacebtwaddrandop;
            buffersz++; // for newline
        }

        char *buffer = calloc(buffersz, sizeof *buffer);
        if (!buffer) return;

        for (word i = 0; i < sz; i++)
        {
            strncat(buffer, "        ", leftmargin); // left margin

            char addr[5];
            sprintf(addr, "%d", i);
            strncat(buffer, addr, addrsz - 1);   // address
            strcat(buffer, ":\t\t");  // spacing between address and op

            sOp O = code[i];
            opsz = strlen(s_opinfo_name(O));
            strncat(buffer, s_opinfo_name(O), opsz); // op name
            if (s_opinfo_hasreg(O)) // op register
            {
                strcat(buffer, "\t\t");

                word V;
                memcpy(&V, code + i + 1, sizeof V);
                char Vstr[5];
                sprintf(Vstr, "%d", V);
                strcat(buffer, Vstr);

                i += 2; // i++ in loop will do another inc
            }
            strcat(buffer, "\n"); // newline!
        }

        printf("\nBytecode Dump: {\n%s\n}\n", buffer);

        free(buffer);
    }
}

void s_dump_stack(sContext *S)
{
    char *buffer = calloc(S->stack_count * 6, sizeof(byte));
    if (!buffer) return;

    for (word i = 0; i < S->stack_count; i++)
    {
        char V[5];
        sprintf(V, "%d", S->stack[i]);
        strncat(buffer, V, 5);
        strcat(buffer, " ");
    }
    printf("\nStack Dump: {\n%s\n}\n", buffer);

    free(buffer);
}
