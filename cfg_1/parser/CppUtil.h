#pragma once

#include "CfgDef.h"
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
    std::string DefValue(RawCategory raw);
    std::string TypeName(const IType* scope, const IType* type);
    std::string Value(const IType* scope, const IType* type, const IValue* val);
    std::string OrignalValue(const IValue* val);

    bool Convert(const IVariate* var, CppVarData& out);
}

CFG_NAMESPACE_END
