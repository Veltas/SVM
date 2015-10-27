#include "svm.hpp"


namespace
{
	sOpInfo sOpInfoList[S_OP__MAX__] = {0};
}


void s_setopinfo_(sOp O, const char *name, bool hasreg)
{
    strcpy(sOpInfoList[O].name, name);
    sOpInfoList[O].hasreg = hasreg;
    sOpInfoList[O].op = O;
}


sOpInfo* s_opinfo(sOp O)
{
    return &sOpInfoList[O];
}

bool s_opinfo_hasreg(sOp O)
{
    return sOpInfoList[O].hasreg;
}

char* s_opinfo_name(sOp O)
{
    return sOpInfoList[O].name;
}

sOp s_opinfo_find(char *name)
{
    for (word i = 0; i < S_OP__MAX__; i++)
        if (!strcmp(sOpInfoList[i].name, name))
            return sOpInfoList[i].op;
    return S_OP_END;
}
