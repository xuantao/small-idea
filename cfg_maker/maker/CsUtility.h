#pragma once
#include "Interface.h"

CFG_NAMESPACE_BEGIN

namespace cs_util
{
    std::string TypeName(const IType* type);
    std::string TypeName(const IScope* pScope, const IType* type);
    std::string Value(const IVariate* var);
    std::string Value(const IScope* scope, const IVariate* var);
}

CFG_NAMESPACE_END
