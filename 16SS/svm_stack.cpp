#include "svm.hpp"



namespace svm {

void Context::push(word v)
{
    if (stack_count < stackSz)
        stack[stack_count++] = v;
}

word Context::pop()
{
    if (stack_count > 0)
        return stack[--stack_count];
    return 0;
}

void Context::reverse(bool manualcount, word N)
{
    if (!manualcount)
        for (word i = 0; i < stack_count / 2; i++)
        {
            word i2 = stack_count - i - 1;
            word t = stack[i];
            stack[i] = stack[i2];
            stack[i2] = t;
        }
    else
        for (word i = stack_count, j = stack_count - N; i > N/2 - stack_count; i--, j++)
        {
            word t = stack[i];
            stack[i] = stack[j];
            stack[j] = t;
        }
}

} // namespace svm
