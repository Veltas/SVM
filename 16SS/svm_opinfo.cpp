#include "svm.hpp"


sOpInfo opInfo;


void sOpInfo::set(sOp O, const char *name, bool hasreg)
{
    strcpy(m_opInfoList[O].name, name);
    m_opInfoList[O].hasreg = hasreg;
    m_opInfoList[O].op = O;
}


bool sOpInfo::hasreg(sOp O)
{
    return m_opInfoList[O].hasreg;
}

char* sOpInfo::name(sOp O)
{
    return m_opInfoList[O].name;
}

sOp sOpInfo::find(char *name)
{
    for (word i = 0; i < S_OP__MAX__; i++)
        if (!strcmp(m_opInfoList[i].name, name))
            return m_opInfoList[i].op;
    return S_OP_END;
}
