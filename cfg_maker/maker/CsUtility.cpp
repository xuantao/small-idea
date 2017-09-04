#include "CsUtility.h"
#include "Utility.h"

CFG_NAMESPACE_BEGIN

namespace cs_util
{
    std::string TypeName(const IType* type)
    {
        return type->Name();
    }

    std::string TypeName(const IScope* pScope, const IType* type)
    {
        return type->Name();
    }

    std::string Value(const IVariate* var)
    {
        return "";
    }

    std::string Value(const IScope* scope, const IVariate* var)
    {
        return "";
    }
}

CFG_NAMESPACE_END
