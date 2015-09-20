// svm_compiler.c


#include "svm.h"
#include <ctype.h>


char* strtolower(char *str)
{
    size_t l = strlen(str);
    for (size_t i = 0; i <= l; i++)
        str[i] = tolower(str[i]);
    return str;
}

word appendcode(byte **code, word currsz, char *N, word V)
{
    word newsize = 1;
    sOp O = s_opinfo_find(N);

    if (O == S_OP_END) return 0;
    if (s_opinfo_hasreg(O))
        newsize += sizeof V;
    if (*code)
    {
        byte *ptr = realloc(*code, sizeof(byte) * (currsz + newsize));
        if (ptr)
            *code = ptr;
        else return 0;
    }
    else
    {
        byte *ptr = malloc(sizeof(byte) * newsize);
        if (ptr)
            *code = ptr;
        else return 0;
    }

    byte *c = *code;
    c[currsz] = (byte)O;
    if (s_opinfo_hasreg(O))
        memcpy(c + currsz + 1, &V, sizeof V);

    return newsize;
}

byte* s_compile(char *program, word sz, word *compilesz)
{
    // takes whitespaces and newlines as delimiter
    // converts op codes to lowercase

    // starts by getting an op
    // if reached space, check if op has register
    // if op has register, read the next characters until whitespace
    // if op has no register, read next op

    char tmp[128] = {0};
    char tmp2[128] = {0};
    int tmpi = 0;
    bool needreg = false;
    bool islast = false;

    byte *code = NULL;
    word codesz = 0;

    for (word i = 0; i < sz; i++)
    {
        char c = program[i];
        if (i == sz - 1 && isgraph(c))
        {
            tmp[tmpi++] = c;
            islast = true;
        }
        if (c == ' ' || c == '\n' || islast)
        {
            if (tmpi > 0) // a token
            {
                if (!needreg) // its an operator
                {
                    strtolower(tmp);
                    sOp O = s_opinfo_find(tmp);

                    if (s_opinfo_hasreg(O))
                    {
                        needreg = true;
                        strcpy(tmp2, tmp);
                        memset(tmp, 0, 128);
                        tmpi = 0;
                    }
                    else
                    {
                        size_t sz = appendcode(&code, codesz, tmp, 0);
                        if (sz > 0)
                            codesz += sz;
                        else return 0;

                        memset(tmp, 0, 128);
                        tmpi = 0;
                    }
                }
                else
                {
                    word V = (word)atoi(tmp);
                    word sz = appendcode(&code, codesz, tmp2, V);
                    if (sz == 0)
                        return NULL;
                    codesz += sz;
                    memset(tmp, 0, 128);
                    memset(tmp2, 0, 128);
                    tmpi = 0;
                    needreg = false;
                }
            }
        }
        if (isgraph(c))
            tmp[tmpi++] = c;
    }

    *compilesz = codesz;
    return code;
}
