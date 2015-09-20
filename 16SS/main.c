//Main.c


#include "svm.h"


char *strnew(char *str)
{
    char *s = calloc(strlen(str) + 1, sizeof *s);
    strcpy(s, str);
    return s;
}

void strreverse(char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++)
    {
        size_t i2 = len - i - 1;
        char tmp = str[i];
        str[i] = str[i2];
        str[i2] = tmp;
    }
}

int main(int argc, char *argv[])
{
    s_vm_init();
    sSegment *ctx = s_new();
/*
    s_addinst(ctx, S_OP_PUSH, 99);
    s_addinst(ctx, S_OP_PUSH, 1);
    s_addinst(ctx, S_OP_ADD, 0);
    s_addinst(ctx, S_OP_PUT, 0);
    s_addinst(ctx, S_OP_POP, 0);

    char text[] = "\nHello world!\n";
    strreverse(text);
    for (int i = 0; i < strlen(text); i++)
        s_addinst(ctx, S_OP_PUSH, text[i]);
    s_addinst(ctx, S_OP_PRINTC, 0);
    s_addinst(ctx, S_OP_HALT, 0); */

    if (argc != 2)
    {
        printf("Usage: SVM16SS <filename>\n");
        return 0;
    }

    char *buffer;
    FILE *fh = fopen(argv[1], "rb");
    if (!fh)
    {
        printf("No such file\n");
        return 0;
    }

    fseek(fh, 0L, SEEK_END);
    long s = ftell(fh);
    rewind(fh);
    buffer = malloc(s);

    if (!buffer)
    {
        printf("Couldnt read file");
        return 0;
    }

    fread(buffer, s, 1, fh);
    fclose(fh); fh = NULL;

    word psz = 0;
    byte *bcode = s_compile(buffer, strlen(buffer), &psz);
    if (!bcode)
    {
        puts("Couldnt compile");
        return 1;
    }
    s_segment_setprogram(ctx, bcode, psz);

    s_run(ctx);
	return 0;
}
