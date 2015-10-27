#include "svm.hpp"


namespace svm {

OpInfo opInfo;


void OpInfo::set(Op O, const char *name, bool hasreg)
{
    strcpy(m_opInfoList[static_cast<byte>(O)].name, name);
    m_opInfoList[static_cast<byte>(O)].hasreg = hasreg;
    m_opInfoList[static_cast<byte>(O)].op = O;
}


bool OpInfo::hasreg(Op O)
{
    return m_opInfoList[static_cast<byte>(O)].hasreg;
}

char* OpInfo::name(Op O)
{
    return m_opInfoList[static_cast<byte>(O)].name;
}

Op OpInfo::find(char *name)
{
    for (word i = 0; i < maxOps; i++)
        if (!strcmp(m_opInfoList[i].name, name))
            return m_opInfoList[i].op;
    return Op::END;
}

} // namespace svm
