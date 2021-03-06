// svm_section.c


#include "svm.h"


sContext* s_new()
{
    sContext *ctx = calloc(1, sizeof *ctx);
    if (!ctx) return NULL;

    ctx->running = false;
    ctx->code = NULL;
    ctx->stack_count = 0;
    ctx->code_size = 0;
    ctx->opi = 0;

    return ctx;
}

void s_context_setprogram(sContext *C, byte *program, word size)
{
    if (C->code_size == 0)
        C->code = calloc(size, sizeof(byte));
    memcpy(C->code, program, size);
    C->code_size = size;
}
