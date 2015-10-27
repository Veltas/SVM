#include "svm.hpp"
#include <ctype.h>


namespace svm {

void vm_init()
{
    opInfo.set(OP_END, "end", false);
    opInfo.set(OP_HALT, "halt", false);
    opInfo.set(OP_DUMPBC, "dumpbc", false);
    opInfo.set(OP_DUMPNBC, "dumpnbc", false);

    opInfo.set(OP_SPUSH, "spush", true);
    opInfo.set(OP_SP, "sp", true);
    opInfo.set(OP_SPOP, "spop", false);
    opInfo.set(OP_SREV, "srev", false);
    opInfo.set(OP_SREVN, "srevn", true);
    opInfo.set(OP_SDUMP, "sdump", false);
    opInfo.set(OP_SDUP, "sdup", false);

    opInfo.set(OP_MADD, "madd", false);
    opInfo.set(OP_MSUB, "msub", false);
    opInfo.set(OP_MMUL, "mmul", false);
    opInfo.set(OP_MDIV, "mdiv", false);

    opInfo.set(OP_INC, "inc", false);
    opInfo.set(OP_DEC, "dec", false);

    opInfo.set(OP_XOR, "xor", false);
    opInfo.set(OP_AND, "and", false);
    opInfo.set(OP_OR, "or", false);
    opInfo.set(OP_NOT, "not", false);

    opInfo.set(OP_PRINT, "print", false);
    opInfo.set(OP_PRINTN, "printn", true);

    opInfo.set(OP_CMPE, "cmpe", false);
    opInfo.set(OP_CMPG, "cmpg", false);
    opInfo.set(OP_CMPGE, "cmpge", false);

    opInfo.set(OP_JUMP, "jump", true);
    opInfo.set(OP_JUMPT, "jumpt", true);
    opInfo.set(OP_JUMPF, "jumpf", true);

    opInfo.set(OP_LABEL, "label", true);
    opInfo.set(OP_GOTO, "goto", true);
    opInfo.set(OP_GOTOT, "gotot", true);
    opInfo.set(OP_GOTOF, "gotof", true);

    //opInfo.set(OP_, "", false);
}

void run(Context *C)
{
    C->running = true;
    while (C->running)
    {
        byte op = next(C);
        eval(C, op);
    }
}

void addinst(Context *C, byte O, word V)
{
    word newsize = 1;
    if (opInfo.hasreg(static_cast<Op>(O)))
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

void dump_bytecode(byte *code, word sz, bool nice)
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
            Op O = static_cast<Op>(code[i]);
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

            Op O = static_cast<Op>(code[i]);
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

void dump_stack(Context *S)
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

} // namespace svm
