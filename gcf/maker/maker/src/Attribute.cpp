#include "Attribute.h"

GCF_NAMESPACE_BEGIN

AttributeSet::~AttributeSet()
{
    for (auto attr : _attributes)
        delete attr;
    _attributes.clear();
}

const IAttribute* AttributeSet::Get(const std::string& name) const
{
    for (auto attr : _attributes)
    {
        if (attr->Name() == name)
            return attr;
    }

    return nullptr;
}

const IAttribute* AttributeSet::Get(int index) const
{
    return _attributes[index];
}

bool AttributeSet::Traverse(const std::function<bool(const IAttribute*)>& func) const
{
    for (auto attr : _attributes)
    {
        if (!func(attr))
            return false;
    }
    return true;
}

bool AttributeSet::Push(const std::string& name, std::vector<IValue*>& params)
{
    if (Get(name) != nullptr)
        return false;

    auto attr = new Attribute(name, params);
    _attributes.push_back(attr);
    return true;
}

GCF_NAMESPACE_END
