#include "svm.hpp"


namespace svm {

byte next(Context *C)
{
    return C->code[C->opi++];
}

word getv(Context *C)
{
    size_t sz = sizeof(word);
    word w;
    memcpy(&w, C->code + C->opi, sz);
    C->opi += sz;
    return w;
}

void eval(Context *C, byte op)
{
    switch(op)
    {
    case OP_HALT:
        C->running = false;
        break;
    case OP_DUMPBC:
        dump_bytecode(C->code, C->code_size, false);
        break;
    case OP_DUMPNBC:
        dump_bytecode(C->code, C->code_size, true);
        break;
        // ---------------------------------------------------
    case OP_SP:
    case OP_SPUSH:
    {
        word v = getv(C);
        push(C, v);
        break;
    }
    case OP_SPOP:
        pop(C);
        break;
    case OP_SREV:
        reverse(C, false, 0);
        break;
    case OP_SREVN:
        reverse(C, true, getv(C));
        break;
    case OP_SDUMP:
        dump_stack(C);
        break;
    case OP_SDUP:
        push(C, C->stack[C->stack_count - 1]);
        break;
        // ---------------------------------------------------
    case OP_MADD:
    {
        word v1 = pop(C);
        push(C, v1 + pop(C));
        break;
    }
    case OP_MSUB:
    {
        word v1 = pop(C);
        push(C, v1 - pop(C));
        break;
    }
    case OP_MDIV:
    {
        word v1 = pop(C);
        push(C, v1 / pop(C));
        break;
    }
    case OP_MMUL:
    {
        word v1 = pop(C);
        push(C, v1 * pop(C));
        break;
    }
        // ---------------------------------------------------
    case OP_INC:
        push(C, pop(C) + 1);
        break;
    case OP_DEC:
        push(C, pop(C) - 1);
        break;
        // ---------------------------------------------------
    case OP_XOR:
    {
        word v = pop(C);
        push(C, v ^ pop(C));
        break;
    }
    case OP_AND:
    {
        word v = pop(C);
        push(C, v & pop(C));
        break;
    }
    case OP_OR:
    {
        word v = pop(C);
        push(C, v | pop(C));
        break;
    }
    case OP_NOT:
        push(C, ~pop(C));
        break;
        // ---------------------------------------------------
    case OP_PRINT:
        printf("%d", pop(C));
        break;
    case OP_PRINTN:
    {
        word count = getv(C);
        while (count-- != 0)
        {
            printf("%d", pop(C));
        }
    }
        // ---------------------------------------------------
    case OP_CMPE:
    {
        word v = pop(C);
        (v == pop(C) ? push(C, 1) : push(C, 0));
        break;
    }
    case OP_CMPG:
    {
        word v = pop(C);
        (v > pop(C) ? push(C, 1) : push(C, 0));
        break;
    }
    case OP_CMPGE:
    {
        word v = pop(C);
        (v >= pop(C) ? push(C, 1) : push(C, 0));
        break;
    }
        // ---------------------------------------------------
    case OP_JUMP:
        C->opi = getv(C);
        break;
    case OP_JUMPT:
        if (pop(C) != 0)
            C->opi = getv(C);
        else
            C->opi += 2;
        break;
    case OP_JUMPF:
        if (pop(C) == 0)
            C->opi = getv(C);
        else
            C->opi += 2;
        break;
        // ---------------------------------------------------
    default:
        break;
    }
}

} // namespace svm
