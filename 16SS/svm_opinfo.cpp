#include "svm.hpp"


namespace svm {

OpInfo opInfo;


void OpInfo::set(Op O, const char *name, bool hasreg)
{
    strcpy(m_opInfoList[O].name, name);
    m_opInfoList[O].hasreg = hasreg;
    m_opInfoList[O].op = O;
}


bool OpInfo::hasreg(Op O)
{
    return m_opInfoList[O].hasreg;
}

char* OpInfo::name(Op O)
{
    return m_opInfoList[O].name;
}

Op OpInfo::find(char *name)
{
    for (word i = 0; i < OP__MAX__; i++)
        if (!strcmp(m_opInfoList[i].name, name))
            return m_opInfoList[i].op;
    return OP_END;
}

} // namespace svm
