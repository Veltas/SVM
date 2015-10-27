#include "svm.hpp"


enum class Action
{
    Compile,
    Run,
    Dump,
};


int main(int argc, char *argv[])
{
    svm::vm_init();

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
            act = Action::Compile;
        }
        else if (!strcmp(argv[2], "-d"))
        {
            act = Action::Dump;
        }
        else
        {
            printf("Invalid switch\n");
            return 0;
        }
    }
    else act = Action::Run;

    svm::word bcodesize = 0;
    svm::byte *bcode;

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
        buffer = reinterpret_cast<char *>(calloc(bufferlen, sizeof *buffer));

        if (!buffer)
        {
            printf("Couldnt read file\n");
            return 0;
        }

        fread(buffer, bufferlen, sizeof *buffer, fr);
        fclose(fr);

        if (!svm::compile(buffer, bufferlen, &bcode, &bcodesize))
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
        bcode = reinterpret_cast<svm::byte *>(calloc(bcodesize, sizeof *bcode));
        fread(bcode, bcodesize, sizeof *bcode, fr);
        fclose(fr);
    }

    switch (act)
    {
    case Action::Compile:
    {
        char *newfilename = reinterpret_cast<char *>(calloc(strlen(argv[1]) + 4, sizeof(char)));
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
    case Action::Dump:
        svm::dump_bytecode(bcode, bcodesize, true);
        break;
    case Action::Run:
    {
        svm::Context *ctx = svm::newcontext();
        svm::context_setprogram(ctx, bcode, bcodesize);
        svm::run(ctx);
        free(ctx);
        break;
    }
    }

    free(bcode);
}
