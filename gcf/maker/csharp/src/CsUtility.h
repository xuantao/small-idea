﻿#pragma once
#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

namespace cs_util
{
    std::string ToString(const IValue* val);
    std::string TypeName(const IType* type);
    std::string TypeName(const IType* type, const IScope* pScope);
    std::string Value(const IVariate* var);
    std::string Value(const IVariate* var, const IScope* scope);
    std::string DefValue(const IType* type);
}

GCF_NAMESPACE_END
