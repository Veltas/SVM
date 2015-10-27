#include "svm.hpp"
#include <ctype.h>


namespace svm {

void vm_init()
{
    opInfo.set(Op::END, "end", false);
    opInfo.set(Op::HALT, "halt", false);
    opInfo.set(Op::DUMPBC, "dumpbc", false);
    opInfo.set(Op::DUMPNBC, "dumpnbc", false);

    opInfo.set(Op::SPUSH, "spush", true);
    opInfo.set(Op::SP, "sp", true);
    opInfo.set(Op::SPOP, "spop", false);
    opInfo.set(Op::SREV, "srev", false);
    opInfo.set(Op::SREVN, "srevn", true);
    opInfo.set(Op::SDUMP, "sdump", false);
    opInfo.set(Op::SDUP, "sdup", false);

    opInfo.set(Op::MADD, "madd", false);
    opInfo.set(Op::MSUB, "msub", false);
    opInfo.set(Op::MMUL, "mmul", false);
    opInfo.set(Op::MDIV, "mdiv", false);

    opInfo.set(Op::INC, "inc", false);
    opInfo.set(Op::DEC, "dec", false);

    opInfo.set(Op::XOR, "xor", false);
    opInfo.set(Op::AND, "and", false);
    opInfo.set(Op::OR, "or", false);
    opInfo.set(Op::NOT, "not", false);

    opInfo.set(Op::PRINT, "print", false);
    opInfo.set(Op::PRINTN, "printn", true);

    opInfo.set(Op::CMPE, "cmpe", false);
    opInfo.set(Op::CMPG, "cmpg", false);
    opInfo.set(Op::CMPGE, "cmpge", false);

    opInfo.set(Op::JUMP, "jump", true);
    opInfo.set(Op::JUMPT, "jumpt", true);
    opInfo.set(Op::JUMPF, "jumpf", true);

    opInfo.set(Op::LABEL, "label", true);
    opInfo.set(Op::GOTO, "goto", true);
    opInfo.set(Op::GOTOT, "gotot", true);
    opInfo.set(Op::GOTOF, "gotof", true);

    //opInfo.set(Op::__, "", false);
}

void run(Context *C)
{
    C->running = true;
    while (C->running)
    {
        byte op = C->next();
        C->eval(op);
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
