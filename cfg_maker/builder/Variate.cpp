#include "Variate.h"
#include "Type.h"

CFG_NAMESPACE_BEGIN

Variate::Variate(IType* belong, IType* type, const std::string& name)
    : _belong(belong)
    , _type(type)
    , _name(name)
    , _value(nullptr)
    , _const(false)
{
}

Variate::~Variate()
{
    delete _value;
    _value = nullptr;

    if (_type->Category() == TypeCategory::Array)
        delete _type;
    _type = nullptr;
}

void Variate::UpgradeArray(int length /* = 0 */)
{
    _type = new ArrayType(_type, length);
}

void Variate::BindValue(IValue* value)
{
    _value = value;
}

void Variate::SetConst()
{
    _const = true;
}
CFG_NAMESPACE_END
