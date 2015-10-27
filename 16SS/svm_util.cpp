#include "svm.hpp"
#include <ctype.h>


void s_vm_init()
{
    opInfo.set(S_OP_END, "end", false);
    opInfo.set(S_OP_HALT, "halt", false);
    opInfo.set(S_OP_DUMPBC, "dumpbc", false);
    opInfo.set(S_OP_DUMPNBC, "dumpnbc", false);

    opInfo.set(S_OP_SPUSH, "spush", true);
    opInfo.set(S_OP_SP, "sp", true);
    opInfo.set(S_OP_SPOP, "spop", false);
    opInfo.set(S_OP_SREV, "srev", false);
    opInfo.set(S_OP_SREVN, "srevn", true);
    opInfo.set(S_OP_SDUMP, "sdump", false);
    opInfo.set(S_OP_SDUP, "sdup", false);

    opInfo.set(S_OP_MADD, "madd", false);
    opInfo.set(S_OP_MSUB, "msub", false);
    opInfo.set(S_OP_MMUL, "mmul", false);
    opInfo.set(S_OP_MDIV, "mdiv", false);

    opInfo.set(S_OP_INC, "inc", false);
    opInfo.set(S_OP_DEC, "dec", false);

    opInfo.set(S_OP_XOR, "xor", false);
    opInfo.set(S_OP_AND, "and", false);
    opInfo.set(S_OP_OR, "or", false);
    opInfo.set(S_OP_NOT, "not", false);

    opInfo.set(S_OP_PRINT, "print", false);
    opInfo.set(S_OP_PRINTN, "printn", true);

    opInfo.set(S_OP_CMPE, "cmpe", false);
    opInfo.set(S_OP_CMPG, "cmpg", false);
    opInfo.set(S_OP_CMPGE, "cmpge", false);

    opInfo.set(S_OP_JUMP, "jump", true);
    opInfo.set(S_OP_JUMPT, "jumpt", true);
    opInfo.set(S_OP_JUMPF, "jumpf", true);

    opInfo.set(S_OP_LABEL, "label", true);
    opInfo.set(S_OP_GOTO, "goto", true);
    opInfo.set(S_OP_GOTOT, "gotot", true);
    opInfo.set(S_OP_GOTOF, "gotof", true);

    //opInfo.set(S_OP_, "", false);
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
    if (opInfo.hasreg(static_cast<sOp>(O)))
        newsize += sizeof(word);
    byte *newbuff = reinterpret_cast<byte *>(calloc(C->code_size + newsize, 1));

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
            sOp O = static_cast<sOp>(code[i]);
            opsz = strlen(opInfo.name(O));
            if (opInfo.hasreg(O))
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

        char *buffer = reinterpret_cast<char *>(calloc(buffersz, sizeof *buffer));
        if (!buffer) return;

        for (word i = 0; i < sz; i++)
        {
            strncat(buffer, "        ", leftmargin); // left margin

            char addr[5];
            sprintf(addr, "%d", i);
            strncat(buffer, addr, addrsz - 1);   // address
            strcat(buffer, ":\t\t");  // spacing between address and op

            sOp O = static_cast<sOp>(code[i]);
            opsz = strlen(opInfo.name(O));
            strncat(buffer, opInfo.name(O), opsz); // op name
            if (opInfo.hasreg(O)) // op register
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
    char *buffer = reinterpret_cast<char *>(calloc(S->stack_count * 6, sizeof(byte)));
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
