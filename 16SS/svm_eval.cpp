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
    switch(static_cast<Op>(op))
    {
    case Op::HALT:
        C->running = false;
        break;
    case Op::DUMPBC:
        dump_bytecode(C->code, C->code_size, false);
        break;
    case Op::DUMPNBC:
        dump_bytecode(C->code, C->code_size, true);
        break;
        // ---------------------------------------------------
    case Op::SP:
    case Op::SPUSH:
    {
        word v = getv(C);
        push(C, v);
        break;
    }
    case Op::SPOP:
        pop(C);
        break;
    case Op::SREV:
        reverse(C, false, 0);
        break;
    case Op::SREVN:
        reverse(C, true, getv(C));
        break;
    case Op::SDUMP:
        dump_stack(C);
        break;
    case Op::SDUP:
        push(C, C->stack[C->stack_count - 1]);
        break;
        // ---------------------------------------------------
    case Op::MADD:
    {
        word v1 = pop(C);
        push(C, v1 + pop(C));
        break;
    }
    case Op::MSUB:
    {
        word v1 = pop(C);
        push(C, v1 - pop(C));
        break;
    }
    case Op::MDIV:
    {
        word v1 = pop(C);
        push(C, v1 / pop(C));
        break;
    }
    case Op::MMUL:
    {
        word v1 = pop(C);
        push(C, v1 * pop(C));
        break;
    }
        // ---------------------------------------------------
    case Op::INC:
        push(C, pop(C) + 1);
        break;
    case Op::DEC:
        push(C, pop(C) - 1);
        break;
        // ---------------------------------------------------
    case Op::XOR:
    {
        word v = pop(C);
        push(C, v ^ pop(C));
        break;
    }
    case Op::AND:
    {
        word v = pop(C);
        push(C, v & pop(C));
        break;
    }
    case Op::OR:
    {
        word v = pop(C);
        push(C, v | pop(C));
        break;
    }
    case Op::NOT:
        push(C, ~pop(C));
        break;
        // ---------------------------------------------------
    case Op::PRINT:
        printf("%d", pop(C));
        break;
    case Op::PRINTN:
    {
        word count = getv(C);
        while (count-- != 0)
        {
            printf("%d", pop(C));
        }
    }
        // ---------------------------------------------------
    case Op::CMPE:
    {
        word v = pop(C);
        (v == pop(C) ? push(C, 1) : push(C, 0));
        break;
    }
    case Op::CMPG:
    {
        word v = pop(C);
        (v > pop(C) ? push(C, 1) : push(C, 0));
        break;
    }
    case Op::CMPGE:
    {
        word v = pop(C);
        (v >= pop(C) ? push(C, 1) : push(C, 0));
        break;
    }
        // ---------------------------------------------------
    case Op::JUMP:
        C->opi = getv(C);
        break;
    case Op::JUMPT:
        if (pop(C) != 0)
            C->opi = getv(C);
        else
            C->opi += 2;
        break;
    case Op::JUMPF:
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
