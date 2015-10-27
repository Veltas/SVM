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
        push(v);
        break;
    }
    case Op::SPOP:
        pop();
        break;
    case Op::SREV:
        reverse(false, 0);
        break;
    case Op::SREVN:
        reverse(true, getv());
        break;
    case Op::SDUMP:
        dump_stack(this);
        break;
    case Op::SDUP:
        push(stack[stack_count - 1]);
        break;
        // ---------------------------------------------------
    case Op::MADD:
    {
        word v1 = pop();
        push(v1 + pop());
        break;
    }
    case Op::MSUB:
    {
        word v1 = pop();
        push(v1 - pop());
        break;
    }
    case Op::MDIV:
    {
        word v1 = pop();
        push(v1 / pop());
        break;
    }
    case Op::MMUL:
    {
        word v1 = pop();
        push(v1 * pop());
        break;
    }
        // ---------------------------------------------------
    case Op::INC:
        push(pop() + 1);
        break;
    case Op::DEC:
        push(pop() - 1);
        break;
        // ---------------------------------------------------
    case Op::XOR:
    {
        word v = pop();
        push(v ^ pop());
        break;
    }
    case Op::AND:
    {
        word v = pop();
        push(v & pop());
        break;
    }
    case Op::OR:
    {
        word v = pop();
        push(v | pop());
        break;
    }
    case Op::NOT:
        push(~pop());
        break;
        // ---------------------------------------------------
    case Op::PRINT:
        printf("%d", pop());
        break;
    case Op::PRINTN:
    {
        word count = getv();
        while (count-- != 0)
        {
            printf("%d", pop());
        }
    }
        // ---------------------------------------------------
    case Op::CMPE:
    {
        word v = pop();
        (v == pop() ? push(1) : push(0));
        break;
    }
    case Op::CMPG:
    {
        word v = pop();
        (v > pop() ? push(1) : push(0));
        break;
    }
    case Op::CMPGE:
    {
        word v = pop();
        (v >= pop() ? push(1) : push(0));
        break;
    }
        // ---------------------------------------------------
    case Op::JUMP:
        opi = getv();
        break;
    case Op::JUMPT:
        if (pop() != 0)
            opi = getv();
        else
            opi += 2;
        break;
    case Op::JUMPF:
        if (pop() == 0)
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
