#include "svm.hpp"


byte s_next(sContext *C)
{
    return C->code[C->opi++];
}

word s_getv(sContext *C)
{
    size_t sz = sizeof(word);
    word w;
    memcpy(&w, C->code + C->opi, sz);
    C->opi += sz;
    return w;
}

void s_eval(sContext *C, byte op)
{
    switch(op)
    {
    case S_OP_HALT:
        C->running = false;
        break;
    case S_OP_DUMPBC:
        s_dump_bytecode(C->code, C->code_size, false);
        break;
    case S_OP_DUMPNBC:
        s_dump_bytecode(C->code, C->code_size, true);
        break;
        // ---------------------------------------------------
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
        s_reverse(C, false, 0);
        break;
    case S_OP_SREVN:
        s_reverse(C, true, s_getv(C));
        break;
    case S_OP_SDUMP:
        s_dump_stack(C);
        break;
    case S_OP_SDUP:
        s_push(C, C->stack[C->stack_count - 1]);
        break;
        // ---------------------------------------------------
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
        // ---------------------------------------------------
    case S_OP_INC:
        s_push(C, s_pop(C) + 1);
        break;
    case S_OP_DEC:
        s_push(C, s_pop(C) - 1);
        break;
        // ---------------------------------------------------
    case S_OP_XOR:
    {
        word v = s_pop(C);
        s_push(C, v ^ s_pop(C));
        break;
    }
    case S_OP_AND:
    {
        word v = s_pop(C);
        s_push(C, v & s_pop(C));
        break;
    }
    case S_OP_OR:
    {
        word v = s_pop(C);
        s_push(C, v | s_pop(C));
        break;
    }
    case S_OP_NOT:
        s_push(C, ~s_pop(C));
        break;
        // ---------------------------------------------------
    case S_OP_PRINT:
        printf("%d", s_pop(C));
        break;
    case S_OP_PRINTN:
    {
        word count = s_getv(C);
        while (count-- != 0)
        {
            printf("%d", s_pop(C));
        }
    }
        // ---------------------------------------------------
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
        // ---------------------------------------------------
    case S_OP_JUMP:
        C->opi = s_getv(C);
        break;
    case S_OP_JUMPT:
        if (s_pop(C) != 0)
            C->opi = s_getv(C);
        else
            C->opi += 2;
        break;
    case S_OP_JUMPF:
        if (s_pop(C) == 0)
            C->opi = s_getv(C);
        else
            C->opi += 2;
        break;
        // ---------------------------------------------------
    default:
        break;
    }
}
