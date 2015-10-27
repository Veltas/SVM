#include "svm.hpp"
#include <ctype.h>


namespace svm {

namespace
{
    struct AST_s
    {
        Op op = static_cast<Op>(0);
        word reg = 0;
        bool hasreg = false;
        char key[32] = {0};
    } AST_[512];
    word AST_count = 0;

    struct JumpTable_s
    {
        char key[32] = {0};
        word offset = 0;
    } JumpTable_[512];
    word JumpTable_count = 0;
}

bool compile(char *program, word sz, byte **pcompiled, word *compiledsize)
{
    char token[32] = {0};
    word tokenIndex = 0;
    word bcneededsize = 0;
    bool needreg = false;
    bool islasttoken = false;
    bool islabel = false; // used by label
    bool isgoto = false;

    for (word i = 0; i < sz; i++)
    {
        char c = program[i];

        if (tokenIndex > 32)
        {
            printf("Invalid token %s\n", token);
            return false;
        }

        if (i == sz - 1 && isgraph(c))
        {
            token[tokenIndex++] = c;
            islasttoken = true;
        }

        if (c == ' ' || c == '\n' || islasttoken)
        {
            if (tokenIndex > 0)
            {
                if (islabel)
               {
                    memset(JumpTable_[JumpTable_count].key, 0, 32);
                    strcpy(JumpTable_[JumpTable_count].key, token);
                    JumpTable_[JumpTable_count++].offset = bcneededsize;

                    islabel = false;
                    tokenIndex = 0;
                    memset(token, 0, sizeof token);
                    continue;
                }

                if (!needreg)
                {
                    // make it lower case
                    for (word i = 0; i < tokenIndex; i++)
                        token[i] = tolower(token[i]);

                    Op O = opInfo.find(token);

                    switch (O)
                    {
                    case OP_END:
                        printf("No such operator %s at %d\n", token, bcneededsize);
                        return false;
                    case OP_LABEL:
                        islabel = true;
                        tokenIndex = 0;
                        memset(token, 0, sizeof token);
                        continue;
                    case OP_GOTO:
                    case OP_GOTOT:
                    case OP_GOTOF:
                        isgoto = true;
                        break;
                    }

                    if (opInfo.hasreg(O))
                        needreg = true;

                    AST_[AST_count].op = O;
                    AST_[AST_count++].hasreg = needreg;
                    bcneededsize += 1;
                    tokenIndex = 0;
                    memset(token, 0, sizeof token);
                }
                else
                {
                    // Checks if its a number
                    if (!isgoto)
                    {
                        for (word i = 0; i < tokenIndex; i++)
                            if (!isdigit(token[i]))
                            {
                                printf("Invalid value %s at %d\n", token, bcneededsize);
                                return false;
                            }
                        AST_[AST_count - 1].reg = (word)atoi(token);
                    }
                    else
                    {
                        memset(AST_[AST_count - 1].key, 0, 32);
                        strcpy(AST_[AST_count - 1].key, token);
                        isgoto = false;
                    }

                    needreg = false;
                    bcneededsize += sizeof(word);
                    tokenIndex = 0;
                    memset(token, 0, sizeof token);
                }
            }
        }
        else if (isgraph(c))
            token[tokenIndex++] = c;
    }

    byte *bytecode = reinterpret_cast<byte *>(calloc(bcneededsize, sizeof *bytecode));
    if (!bytecode)
    {
        printf("Bytecode allocation failed\n");
        return false;
    }

    for (word i = 0, bi = 0; i < AST_count; i++)
    {
        if (AST_[i].op >= OP_GOTO && AST_[i].op <= OP_GOTOF)
        {
            bool found = false;
            word j = 0;
            for (; j < JumpTable_count; j++)
            {
                if (!strcmp(JumpTable_[j].key, AST_[i].key))
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                printf("No such label %s is defined\n", token);
                return false;
            }

            switch (AST_[i].op)
            {
            case OP_GOTO:
                bytecode[bi++] = OP_JUMP;
                break;
            case OP_GOTOT:
                bytecode[bi++] = OP_JUMPT;
                break;
            case OP_GOTOF:
                bytecode[bi++] = OP_JUMPF;
                break;
            }

            word offset = JumpTable_[j].offset;
            memcpy(bytecode + bi, &offset, sizeof(word));
            bi += sizeof(word);
        }
        else
        {
            bytecode[bi++] = AST_[i].op;
            if (AST_[i].hasreg)
            {
                memcpy(bytecode + bi, &AST_[i].reg, sizeof(word));
                bi += sizeof(word);
            }
        }
    }

    *pcompiled = bytecode;
    *compiledsize = bcneededsize;

    return true;
}

} // namespace svm
