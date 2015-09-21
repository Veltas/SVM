// svm_compiler.c


#include "svm.h"
#include <ctype.h>


static struct
{
    sOp op;
    word reg;
    bool hasreg;
} AST_[512];
static word AST_count = 0;

bool s_compile(char *program, word sz, byte **pcompiled, word *compiledsize)
{
    char token[128] = {0};
    word tokenIndex = 0;
    word bcneededsize = 0;
    bool needreg = false;
    bool islasttoken = false;

    for (word i = 0; i < sz; i++)
    {
        char c = program[i];

        if (i == sz - 1 && isgraph(c))
        {
            token[tokenIndex++] = c;
            islasttoken = true;
        }

        if (c == ' ' || c == '\n' || islasttoken)
        {
            if (tokenIndex > 0)
            {
                if (!needreg)
                {
                    // make it lower case
                    for (word i = 0; i < tokenIndex; i++)
                        token[i] = tolower(token[i]);

                    sOp O = s_opinfo_find(token);
                    if (O == S_OP_END)
                    {
                        printf("No such operator %s at %d\n", token, bcneededsize);
                        return false;
                    }

                    if (s_opinfo_hasreg(O))
                        needreg = true;

                    AST_[AST_count].op = O;
                    AST_[AST_count++].hasreg = needreg;
                    bcneededsize += 1;
                    tokenIndex = 0;
                    memset(token, 0, 128);
                }
                else
                {
                    // Checks if its a number
                    for (word i = 0; i < tokenIndex; i++)
                        if (!isdigit(token[i]))
                        {
                            printf("Invalid value %s at %d\n", token, bcneededsize);
                            return false;
                        }

                    needreg = false;
                    AST_[AST_count - 1].reg = (word)atoi(token);
                    bcneededsize += sizeof(word);
                    tokenIndex = 0;
                    memset(token, 0, 128);
                }
            }
        }
        else if (isgraph(c))
            token[tokenIndex++] = c;
    }

    byte *bytecode = calloc(bcneededsize, sizeof *bytecode);
    if (!bytecode)
    {
        printf("Bytecode allocation failed\n");
        return false;
    }

    for (word i = 0, bi = 0; i < AST_count; i++)
    {
        bytecode[bi++] = AST_[i].op;
        if (AST_[i].hasreg)
        {
            memcpy(bytecode + bi, &AST_[i].reg, sizeof(word));
            bi += 2;
        }
    }

    *pcompiled = bytecode;
    *compiledsize = bcneededsize;

    return true;
}
