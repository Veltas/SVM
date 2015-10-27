#include "svm.hpp"


namespace svm {

byte Context::next()
{
    return code[opi++];
}

word Context::getv()
{
    size_t sz = sizeof(word);
    word w;
    memcpy(&w, code + opi, sz);
    opi += sz;
    return w;
}

void Context::eval(byte op)
{
    switch(static_cast<Op>(op))
    {
    case Op::HALT:
        running = false;
        break;
    case Op::DUMPBC:
        dump_bytecode(code, code_size, false);
        break;
    case Op::DUMPNBC:
        dump_bytecode(code, code_size, true);
        break;
        // ---------------------------------------------------
    case Op::SP:
    case Op::SPUSH:
    {
        word v = getv();
        push(this, v);
        break;
    }
    case Op::SPOP:
        pop(this);
        break;
    case Op::SREV:
        reverse(this, false, 0);
        break;
    case Op::SREVN:
        reverse(this, true, getv());
        break;
    case Op::SDUMP:
        dump_stack(this);
        break;
    case Op::SDUP:
        push(this, stack[stack_count - 1]);
        break;
        // ---------------------------------------------------
    case Op::MADD:
    {
        word v1 = pop(this);
        push(this, v1 + pop(this));
        break;
    }
    case Op::MSUB:
    {
        word v1 = pop(this);
        push(this, v1 - pop(this));
        break;
    }
    case Op::MDIV:
    {
        word v1 = pop(this);
        push(this, v1 / pop(this));
        break;
    }
    case Op::MMUL:
    {
        word v1 = pop(this);
        push(this, v1 * pop(this));
        break;
    }
        // ---------------------------------------------------
    case Op::INC:
        push(this, pop(this) + 1);
        break;
    case Op::DEC:
        push(this, pop(this) - 1);
        break;
        // ---------------------------------------------------
    case Op::XOR:
    {
        word v = pop(this);
        push(this, v ^ pop(this));
        break;
    }
    case Op::AND:
    {
        word v = pop(this);
        push(this, v & pop(this));
        break;
    }
    case Op::OR:
    {
        word v = pop(this);
        push(this, v | pop(this));
        break;
    }
    case Op::NOT:
        push(this, ~pop(this));
        break;
        // ---------------------------------------------------
    case Op::PRINT:
        printf("%d", pop(this));
        break;
    case Op::PRINTN:
    {
        word count = getv();
        while (count-- != 0)
        {
            printf("%d", pop(this));
        }
    }
        // ---------------------------------------------------
    case Op::CMPE:
    {
        word v = pop(this);
        (v == pop(this) ? push(this, 1) : push(this, 0));
        break;
    }
    case Op::CMPG:
    {
        word v = pop(this);
        (v > pop(this) ? push(this, 1) : push(this, 0));
        break;
    }
    case Op::CMPGE:
    {
        word v = pop(this);
        (v >= pop(this) ? push(this, 1) : push(this, 0));
        break;
    }
        // ---------------------------------------------------
    case Op::JUMP:
        opi = getv();
        break;
    case Op::JUMPT:
        if (pop(this) != 0)
            opi = getv();
        else
            opi += 2;
        break;
    case Op::JUMPF:
        if (pop(this) == 0)
            opi = getv();
        else
            opi += 2;
        break;
        // ---------------------------------------------------
    default:
        break;
    }
}

} // namespace svm
