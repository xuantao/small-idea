#include "Type.h"
#include <algorithm>
#include "Utility.h"
#include "Set.h"
#include "Value.h"
#include "ValueUtil.h"

CFG_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Enum Type
EnumType::EnumType(const std::string& name, IType* belong)
    : _name(name)
    , _belong(belong)
{
    _vars = new EnumVarSet(this);
}

EnumType::~EnumType()
{
    delete _vars;
    _vars = nullptr;
    _belong = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// StructType
StructType::StructType(const std::string& name, IType* belong)
    : _name(name)
    , _belong(belong)
    , _vars(nullptr)
    , _inherit(nullptr)
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

bool StructType::IsInherited(const IType* type) const
{
    if (_inherit == nullptr)
        return false;

    if (type == _inherit)
        return true;

    return _inherit->IsInherited(type);
}

bool StructType::Inherit(StructType* type)
{
    if (_inherit != nullptr)
        return false;

    if (type->Category() != TypeCategory::Struct)
        return false;

    if (type == this)
        return false;

    if (IsInherited(type))
        return false;

    _inherit = type;
    return true;
}

IVarSet* StructType::OwnVars() const
{
    return static_cast<StructVarSet*>(_vars)->OwnVars();
}

//////////////////////////////////////////////////////////////////////////
// ArrayType
ArrayType::ArrayType(IType* prev, int length)
    : _prev(prev)
    , _length(length)
{
    if (length >= 0)
        _name = prev->Name() + "[" + std::to_string(length) + "]";
    else
        _name = prev->Name() + "[" + "]";
}

ArrayType::~ArrayType()
{
    if (_prev->Category() == TypeCategory::Array)
        delete _prev;
    _prev = nullptr;
}

IType* ArrayType::Original() const
{
    if (_prev->Category() == TypeCategory::Array)
        return static_cast<const ArrayType*>(_prev)->Original();
    return _prev;
}

//////////////////////////////////////////////////////////////////////////
// ScopeType
ScopeType::ScopeType(const std::string& name, ScopeType* belong /* = nullptr */)
    : _name(name), _belong(belong)
{
    _types = new TypeSetNormal(this);
    _vars = new VarSetNormal(this);
}

ScopeType::~ScopeType()
{
    // vars dependence types
    delete _vars;
    delete _types;
    _vars = nullptr;
    _types = nullptr;
    _belong = nullptr;
}

CFG_NAMESPACE_END
