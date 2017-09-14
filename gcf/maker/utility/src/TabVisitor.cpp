#include "utility/TabVisitor.h"


GCF_NAMESPACE_BEGIN

namespace utility
{
    std::string sGetValue(const IVariate* var)
    {
        return std::string();
    }

    bool TabVisitor::OnVar(const IVariate* var, const IType* rType,
        const std::string& title)
    {
        _title.push_back(title);
        _type.push_back(rType->Name());
        _desc.push_back(var->Desc());
        _def.push_back(sGetValue(var));
        return true;
    }
}
GCF_NAMESPACE_END
