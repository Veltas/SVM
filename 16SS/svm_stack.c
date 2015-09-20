// svm_stack.c

#include "svm.h"



void s_push(sSegment *C, word v)
{
    if (C->stack_count < STACK_SZ)
        C->stack[C->stack_count++] = v;
}

word s_pop(sSegment *C)
{
    if (C->stack_count > 0)
        return C->stack[--C->stack_count];
    return 0;
}

void s_reverse(sSegment *C)
{
    for (word i = 0; i < C->stack_count / 2; i++)
    {
        word t = C->stack[i];
        C->stack[i] = C->stack[C->stack_count - i - 1];
        C->stack[C->stack_count - i - 1] = t;
    }
}
