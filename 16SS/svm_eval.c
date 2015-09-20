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
    case S_OP_PUT:
        printf("%d", s_pop(C));
        break;
    case S_OP_PUTC:
        printf("%c", s_pop(C));
        break;
    case S_OP_PRINT:
        do
        {
            printf("%d", s_pop(C));
        } while (C->stack_count != 0);
        break;
    case S_OP_PRINTC:
        do
        {
            printf("%c", s_pop(C));
        } while (C->stack_count != 0);
        break;
    default:
        break;
    }
}
