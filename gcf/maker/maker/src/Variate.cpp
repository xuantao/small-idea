#include "Variate.h"
#include "Type.h"
#include "ValueUtil.h"
#include "utility/Utility.h"

GCF_NAMESPACE_BEGIN

Variate::Variate(
    const std::string file,
    int line,
    const std::string& name,
    IScope* owner
)   : _name(name)
    , _owner(owner)
    , _file(file)
    , _line(line)
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
    if (ty->TypeCat() == TypeCategory::Array)
        ty = static_cast<IArrayType*>(ty)->Original();

    // only basic type
    utility::ConvertRet allow = utility::ConvertRet::Error;
    if (ty->TypeCat() == TypeCategory::Raw)
        allow = utility::Convert(value->RawCat(), static_cast<IRawType*>(ty)->RawCat());
    else if (ty->TypeCat() == TypeCategory::Enum)
        allow = utility::Convert(value->RawCat(), RawCategory::Int);

    assert(allow != utility::ConvertRet::Error);
    if (allow == utility::ConvertRet::Error)
        return false;

    _value = value;
    return true;
}

GCF_NAMESPACE_END
