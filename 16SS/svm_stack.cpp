#include "svm.hpp"



namespace svm {

void push(Context *C, word v)
{
    if (C->stack_count < stackSz)
        C->stack[C->stack_count++] = v;
}

word pop(Context *C)
{
    if (C->stack_count > 0)
        return C->stack[--C->stack_count];
    return 0;
}

void reverse(Context *C, bool manualcount, word N)
{
    if (!manualcount)
        for (word i = 0; i < C->stack_count / 2; i++)
        {
            word i2 = C->stack_count - i - 1;
            word t = C->stack[i];
            C->stack[i] = C->stack[i2];
            C->stack[i2] = t;
        }
    else
        for (word i = C->stack_count, j = C->stack_count - N; i > N/2 - C->stack_count; i--, j++)
        {
            word t = C->stack[i];
            C->stack[i] = C->stack[j];
            C->stack[j] = t;
        }
}

} // namespace svm
