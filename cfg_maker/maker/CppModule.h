#pragma once
#include "Interface.h"
#include <ostream>

CFG_NAMESPACE_BEGIN

class CppModule
{
public:
    CppModule();

public:
    void Export(const IModule* module);

protected:
    void DeclareFunc(const IFunction* func);


    std::ostream& Write(int tab = 0);
};

CFG_NAMESPACE_END
