#include "TypeDecl.h"
#include <algorithm>
#include "Utility.h"
#include "SetDecl.h"
#include "ValueDecl.h"

CFG_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Enum Type
EnumType::EnumType(const std::string& name, IType* belong)
    : _name(name), _belong(belong)
{
    _vars = new VarSetDecl(this);
}

EnumType::~EnumType()
{
    delete _vars;
    _vars = nullptr;
    _belong = nullptr;
}

int EnumType::Trans(const std::string& name) const
{
    IVariate* var = _vars->Get(name);
    if (var == nullptr)
        return -1;

    return RawValue(var);
}

const std::string& EnumType::Trans(int value) const
{
    for (int i = 0; i < _vars->Size(); ++i)
    {
        IVariate* var = _vars->Get(i);
        if (RawValue(var) == value)
            return var->Name();
    }
    return util::STR_EMPTY;
}

bool EnumType::AddEnum(const std::string& name, IVariate* var)
{
    if (_vars->Get(name))
        return false;

    if (var->Value() == nullptr)
    {
        if (_vars->Size() == 0)
        {
            var->BindValue(new IntValue(0));
        }
        else
        {
            IVariate* prev = _vars->Get(_vars->Size() - 1);
            var->BindValue(new IntValue(RawValue(prev) + 1));
        }
    }

    _vars->Add(var);
    return true;
}

int EnumType::RawValue(IVariate* var) const
{
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// StructType
StructType::StructType(const std::string& name, IType* belong)
    : _name(name)
    , _belong(belong)
    , _vars(nullptr)
{
    _vars = new StructVarSet(this);
}

StructType::~StructType()
{
    delete _vars;
    _vars = nullptr;
    _belong = nullptr;
    _inherit = nullptr;
}

bool StructType::Inherit(IType* type)
{
    if (_inherit != nullptr)
        return false;

    if (_inherit->Category() != TypeCategory::Struct)
        return false;

    _inherit = type;
    return true;
}

//////////////////////////////////////////////////////////////////////////
// ArrayType
ArrayType::ArrayType(const IType* raw, int length)
    : _raw(raw), _length(length)
{
    char buff[32] = {0};
    if (length >= 0)
        _name = raw->Name() + "[" + itoa(length, buff, 32) + "]";
    else
        _name = raw->Name() + "[" + "]";
}

//////////////////////////////////////////////////////////////////////////
// ScopeType
ScopeType::ScopeType(const std::string& name, ScopeType* belong /* = nullptr */)
    : _name(name), _belong(belong)
{
    _types = new TypeSetDecl(this);
    _vars = new VarSetDecl(this);
}

ScopeType::~ScopeType()
{
    delete _types;
    delete _vars;
    _types = nullptr;
    _vars = nullptr;
    _belong = nullptr;
}

CFG_NAMESPACE_END
