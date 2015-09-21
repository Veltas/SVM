//Main.c


#include "svm.h"


typedef enum
{
    ACT_Compile,
    ACT_Run,
    ACT_Dump,
} Action;


int main(int argc, char *argv[])
{
    s_vm_init();

    Action act;
    bool isbytecode = false;

    if (argc < 2)
    {
        printf("Usage: SVM16SS <filename>.s/sc [-c][-d]\n");
        return 0;
    }

    if (!strcmp(argv[1] + (strlen(argv[1]) - 3), ".sc"))
        isbytecode = true;

    if (argc == 3)
    {
        if (!strcmp(argv[2], "-c"))
        {
            if (isbytecode)
            {
                printf("Cannot compile bytecode\n");
                return 0;
            }
            printf("Compiling %s to %s.sc\n", argv[1], argv[1]);
            act = ACT_Compile;
        }
        else if (!strcmp(argv[2], "-d"))
        {
            act = ACT_Dump;
        }
        else
        {
            printf("Invalid switch\n");
            return 0;
        }
    }
    else act = ACT_Run;

    word bcodesize = 0;
    byte *bcode;

    if (!isbytecode)
    {
        char *buffer;
        FILE *fr = fopen(argv[1], "rb");
        if (!fr)
        {
            printf("No such file\n");
            return 0;
        }

        fseek(fr, 0L, SEEK_END);
        long bufferlen = ftell(fr);
        rewind(fr);
        buffer = calloc(bufferlen, sizeof *buffer);

        if (!buffer)
        {
            printf("Couldnt read file\n");
            return 0;
        }

        fread(buffer, bufferlen, sizeof *buffer, fr);
        fclose(fr);

        if (!s_compile(buffer, bufferlen, &bcode, &bcodesize))
        {
            puts("Compilation failed\n");
            free(buffer);
            return 1;
        }
        free(buffer);
    }
    else
    {
        FILE *fr = fopen(argv[1], "rb");
        if (!fr)
        {
            printf("No such file\n");
            return 0;
        }
        fseek(fr, 0, SEEK_END);
        bcodesize = ftell(fr);
        rewind(fr);
        bcode = calloc(bcodesize, sizeof *bcode);
        fread(bcode, bcodesize, sizeof *bcode, fr);
        fclose(fr);
    }

    switch (act)
    {
    case ACT_Compile:
    {
        char *newfilename = calloc(strlen(argv[1]) + 4, sizeof(char));
        if (!newfilename)
        {
            printf("Allocation failed\n");
            free(bcode);
            return 0;
        }

        strcpy(newfilename, argv[1]);
        strcat(newfilename, ".sc");
        FILE *fw = fopen(newfilename, "wb");
        free(newfilename);
        if (!fw)
        {
            printf("Couldnt open file for write\n");
            free(bcode);
            return 0;
        }

        fwrite(bcode, bcodesize, sizeof *bcode, fw);
        fclose(fw);
        break;
    }
    case ACT_Dump:
        s_dump_bytecode(bcode, bcodesize, true);
        break;
    case ACT_Run:
    {
        sContext *ctx = s_new();
        s_context_setprogram(ctx, bcode, bcodesize);
        s_run(ctx);
        free(ctx);
        break;
    }
    }

    free(bcode);

    return 0;
}
