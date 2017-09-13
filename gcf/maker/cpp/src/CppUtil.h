#pragma once

#include "gcf/gcf.h"
#include <ostream>

GCF_NAMESPACE_BEGIN

struct CppVarData
{
    std::string type;
    std::string name;
    std::string value;
    const IVariate* var;
};

namespace cpp_util
{
    std::string ToString(const IValue* val);
    std::string RawName(RawCategory raw);
    std::string TypeName(const IType* type);
    std::string TypeName(const IScope* scope, const IType* type);
    std::string Value(const IScope* scope, const IType* type, const IValue* val);
    std::string DefValue(RawCategory raw);

    bool Convert(const IVariate* var, CppVarData& out);
}

GCF_NAMESPACE_END
