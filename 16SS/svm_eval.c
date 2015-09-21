// svm_eval.c


#include "svm.h"


byte s_next(sSegment *C)
{
    return C->code[C->opi++];
}

word s_getv(sSegment *C)
{
    size_t sz = sizeof(word);
    word w;
    memcpy(&w, C->code + C->opi, sz);
    C->opi += sz;
    return w;
}

void s_eval(sSegment *C, byte op)
{
    switch(op)
    {
    case S_OP_HALT:
        C->running = false;
        break;
    case S_OP_DMPBC:
        s_dump_bytecode(C->code, C->code_size, false);
        break;
    case S_OP_DMPBCN:
        s_dump_bytecode(C->code, C->code_size, true);
        break;
    case S_OP_SP:
    case S_OP_SPUSH:
    {
        word v = s_getv(C);
        s_push(C, v);
        break;
    }
    case S_OP_SPOP:
        s_pop(C);
        break;
    case S_OP_SREV:
        s_reverse(C);
        break;
    case S_OP_SDMP:
        s_dump_stack(C);
        break;
    case S_OP_SDUP:
        s_push(C, C->stack[C->stack_count - 1]);
        break;
    case S_OP_MADD:
    {
        word v1 = s_pop(C);
        s_push(C, v1 + s_pop(C));
        break;
    }
    case S_OP_MSUB:
    {
        word v1 = s_pop(C);
        s_push(C, v1 - s_pop(C));
        break;
    }
    case S_OP_MDIV:
    {
        word v1 = s_pop(C);
        s_push(C, v1 / s_pop(C));
        break;
    }
    case S_OP_MMUL:
    {
        word v1 = s_pop(C);
        s_push(C, v1 * s_pop(C));
        break;
    }
    case S_OP_INC:
        s_push(C, s_pop(C) + 1);
        break;
    case S_OP_DEC:
        s_push(C, s_pop(C) - 1);
        break;
    case S_OP_PUT:
        printf("%d", s_pop(C));
        break;
    case S_OP_PUTC:
        printf("%c", s_pop(C));
        break;
    case S_OP_PUTN:
    {
        word count = s_getv(C);
        for (word i = 0; i < count; i++)
            printf("%d", s_pop(C));
        break;
    }
    case S_OP_PUTNC:
    {
        word count = s_getv(C);
        for (word i = 0; i < count; i++)
            printf("%c", s_pop(C));
        break;
    }
    case S_OP_PUTALL:
        do
        {
            printf("%d", s_pop(C));
        } while (C->stack_count != 0);
        break;
    case S_OP_PUTALLC:
        do
        {
            printf("%c", s_pop(C));
        } while (C->stack_count != 0);
        break;
    case S_OP_CMPE:
    {
        word v = s_pop(C);
        (v == s_pop(C) ? s_push(C, 1) : s_push(C, 0));
        break;
    }
    case S_OP_CMPG:
    {
        word v = s_pop(C);
        (v > s_pop(C) ? s_push(C, 1) : s_push(C, 0));
        break;
    }
    case S_OP_CMPGE:
    {
        word v = s_pop(C);
        (v >= s_pop(C) ? s_push(C, 1) : s_push(C, 0));
        break;
    }
    case S_OP_JMPBT:
        if (s_pop(C) != 0)
        {
            C->opi--;
            C->opi -= s_getv(C);
        }
        break;
    case S_OP_JMPBF:
        if (s_pop(C) == 0)
        {
            C->opi--;
            C->opi -= s_getv(C);
        }
        break;
    case S_OP_JMPB:
        C->opi--;
        C->opi -= s_getv(C);
        break;
    case S_OP_JMPFT:
        if (s_pop(C) != 0)
            C->opi += s_getv(C) + 1;
        break;
    case S_OP_JMPFF:
        if (s_pop(C) == 0)
            C->opi += s_getv(C) + 1;
        break;
    case S_OP_JMPF:
        C->opi += s_getv(C) + 1;
        break;
    case S_OP_JMPTT:
        if (s_pop(C) != 0)
            C->opi = s_getv(C);
        break;
    case S_OP_JMPTF:
        if (s_pop(C) == 0)
            C->opi = s_getv(C);
        break;
    case S_OP_JMPT:
        C->opi = s_getv(C);
        break;
    default:
        break;
    }
}
