#include "Scope.h"

CFG_NAMESPACE_BEGIN

IElement* NormalScope::GetElement(const std::string& name) const
{
    IElement* element = nullptr;

    if (!element && _tySet)
        element = _tySet->Get(name);

    if (!element && _varSet)
        element = _varSet->Get(name);

    if (!element && _nsSet)
        element = _nsSet->Get(name);

    return element;
}

IScope* NormalScope::GetScope(const std::string& name) const
{
    IElement* element = GetElement(name);
    if (element)
    {
        if (element->ElementCat() == ElementCategory::Type)
            return static_cast<IType*>(element)->Scope();
        else if (element->ElementCat() == ElementCategory::Namespace)
            return static_cast<INamespace*>(element)->Scope();
    }
    return nullptr;
}

CFG_NAMESPACE_END
