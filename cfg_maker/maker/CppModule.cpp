#include "CppModule.h"
#include <iostream>

CFG_NAMESPACE_BEGIN

void CppModule::Export(const IModule* module)
{

}

void CppModule::DeclareFunc(const IFunction* func)
{

}

std::ostream& CppModule::Write(int tab/* = 0*/)
{
    return std::cout;
}

CFG_NAMESPACE_END
