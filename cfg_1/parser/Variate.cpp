#include "Variate.h"
#include "TypeDecl.h"

CFG_NAMESPACE_BEGIN

Variate::Variate(const IType* belong, const IType* type, const std::string& name)
    : _belong(belong), _type(type), _name(name)
{
}

void Variate::UpgradeArray(int length /* = -1 */)
{
    _type = new ArrayType(_type, length);
}

void Variate::BindValue(IValue* value)
{
    _value = value;
}

CFG_NAMESPACE_END
