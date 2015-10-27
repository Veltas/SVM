#include "svm.hpp"


namespace svm {

void Context::setprogram(byte *program, word size)
{
    if (code_size == 0)
        code = reinterpret_cast<byte *>(calloc(size, sizeof(byte)));
    memcpy(code, program, size);
    code_size = size;
}

Context::~Context()
{
    free(code);
}

} // namespace svm
