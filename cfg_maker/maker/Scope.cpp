#include "Scope.h"

CFG_NAMESPACE_BEGIN

IElement* NormalScope::Get(const std::string& name) const
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

CFG_NAMESPACE_END
