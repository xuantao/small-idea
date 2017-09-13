#include "Variate.h"
#include "Type.h"
#include "ValueUtil.h"

GCF_NAMESPACE_BEGIN

Variate::Variate(const std::string& name, IScope* owner)
    : _name(name)
    , _owner(owner)
{
}

Variate::Variate(IScope* owner)
    : _owner(owner)
{
}

Variate::~Variate()
{
    delete _value;
    _value = nullptr;

    if (_type && _type->TypeCat() == TypeCategory::Array)
        delete _type;
    _type = nullptr;
}

void Variate::UpgradeArray(int length /* = 0 */)
{
    _type = new ArrayType(_type, length);
}

bool Variate::BindValue(IValue* value)
{
    // allow clear
    if (value == nullptr)
    {
        delete _value;
        _value = nullptr;
        return true;
    }

    if (_type == nullptr)
        return false;   // not a valid variate

    IType* ty = _type;
    bool allow = false;
    if (ty->TypeCat() == TypeCategory::Array)
        ty = static_cast<IArrayType*>(ty)->Original();

    // only basic type
    if (ty->TypeCat() == TypeCategory::Raw)
        allow = value_util::AsRaw(static_cast<IRawType*>(ty)->RawCat(), value);
    else if (ty->TypeCat() == TypeCategory::Enum)
        allow = value_util::AsRaw(RawCategory::Int, value);

    if (!allow)
        return false;

    _value = value;
    return true;
}

GCF_NAMESPACE_END
