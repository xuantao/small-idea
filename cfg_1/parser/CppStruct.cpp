#include "CppStruct.h"
#include "Type.h"

CFG_NAMESPACE_BEGIN

CppStruct::CppStruct(const StructType* type)
    : _struct(type)
{ }

CppStruct::~CppStruct()
{

}

bool CppStruct::Resolve()
{

    return true;
}

CFG_NAMESPACE_END
