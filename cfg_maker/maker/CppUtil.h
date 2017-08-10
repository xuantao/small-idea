#pragma once

#include "Interface.h"
#include <ostream>

CFG_NAMESPACE_BEGIN

struct CppVarData
{
    std::string type;
    std::string name;
    std::string value;
    const IVariate* var;
};

namespace cpp_util
{
    std::string RawName(RawCategory raw);
    std::string TypeName(const IScope* scope, const IType* type);
    std::string Value(const IScope* scope, const IType* type, const IValue* val);

    bool Convert(const IVariate* var, CppVarData& out);
}

CFG_NAMESPACE_END
