#include "Type.h"
#include "Set.h"
#include "Value.h"
#include "Scope.h"
#include "ValueUtil.h"
#include "utility/Utility.h"
#include <algorithm>

GCF_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Enum Type
EnumType::EnumType(const std::string& name, IScope* owner)
    : _name(name)
    , _owner(owner)
    , _vars(nullptr)
{
    NormalScope* scope = new NormalScope(name, owner);
    _vars = new EnumVarSet();
    _scope = scope;
    scope->Bind(this);
    scope->VarSet(_vars);
}

EnumType::~EnumType()
{
    delete _scope;
    _scope = nullptr;

    delete _vars;
    _vars = nullptr;

    _owner = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// StructType
StructType::StructType(const std::string& name, IScope* owner, CfgCategory cfg)
    : _name(name)
    , _owner(owner)
    , _cfg(cfg)
{
    StructTypeSet* tySet = new StructTypeSet(this);
    StructVarSet* vars = new StructVarSet(this);

    NormalScope* scope = new NormalScope(name, owner);
    NormalScope* ownScope = new NormalScope(name, owner);

    scope->Bind(this);
    scope->TypeSet(tySet);
    scope->VarSet(vars);

    ownScope->Bind(this);
    ownScope->TypeSet(tySet->OwnTys());
    ownScope->VarSet(vars->OwnVars());

    _scope = scope;
    _ownScope = ownScope;
}

StructType::~StructType()
{
    delete _scope->VarSet();
    delete _scope->TypeSet();

    delete _ownScope;
    _ownScope = nullptr;

    delete _scope;
    _scope = nullptr;

    _owner = nullptr;
    _inherit = nullptr;
}

bool StructType::IsInherited(const IStructType* type) const
{
    if (_inherit == nullptr)
        return false;

    if (type == _inherit)
        return true;

    return _inherit->IsInherited(type);
}

bool StructType::Inherit(IStructType* type)
{
    if (_inherit != nullptr)
        return false;

    if (type->TypeCat() != TypeCategory::Struct)
        return false;

    if (type == this)
        return false;

    if (IsInherited(type))
        return false;

    _inherit = type;
    return true;
}

void StructType::DeclCompleted()
{
    _hashCode = utility::HashValue(this);
}

//////////////////////////////////////////////////////////////////////////
// Function
Function::Function(const std::string& name, IType* ret, IScope* owner)
    : _name(name)
    , _rawName(name)
    , _ret(ret)
    , _owner(owner)
{
    NormalScope* scope = new NormalScope(name, owner);
    scope->VarSet(new VarSetNormal());
    scope->Bind(this);
    _scope = scope;
}

Function::~Function()
{
    delete _scope->VarSet();
    delete _scope;
    _scope = nullptr;
}

void Function::DeclCompleted()
{
    IVarSet* varSet = _scope->VarSet();
    for (int i = 0; i < varSet->Size(); ++i)
        _name += std::string("_") + GetName(varSet->Get(i)->Type());

    _hashCode = utility::HashValue(this);
}

std::string Function::GetName(const IType* type)
{
    std::string str;
    if (type->TypeCat() == TypeCategory::Array)
    {
        const IArrayType* arTy = (const IArrayType*)type;
        str = GetName(arTy->Prev()) + "_A";
        if (arTy->Length())
            str += std::to_string(arTy->Length());
        return str;
    }
    else if (type->TypeCat() == TypeCategory::Raw)
    {
        return type->Name();
    }
    else
    {
        return utility::Contact(utility::Relative(type, _owner), "_");
    }
}

//////////////////////////////////////////////////////////////////////////
// ArrayType
ArrayType::ArrayType(IType* prev, int length)
    : _prev(prev)
    , _length(length)
{
    if (length > 0)
        _name = prev->Name() + "[" + std::to_string(length) + "]";
    else
        _name = prev->Name() + "[" + "]";
}

ArrayType::~ArrayType()
{
    if (_prev->TypeCat() == TypeCategory::Array)
        delete _prev;
    _prev = nullptr;
}

IType* ArrayType::Original() const
{
    if (_prev->TypeCat() == TypeCategory::Array)
        return static_cast<const ArrayType*>(_prev)->Original();
    return _prev;
}

//////////////////////////////////////////////////////////////////////////
// Namesapce
Namespace::Namespace(const std::string& name, IScope* owner)
    : _name(name)
    , _owner(owner)
{
    NormalScope* scope = new NormalScope(name, owner);
    scope->Bind(this);

    _scope = scope;

    _tySet = new TypeSetNormal();
    scope->TypeSet(_tySet);

    _varSet = new VarSetNormal();
    scope->VarSet(_varSet);

    _nsSet = new NsSet();
    scope->NsSet(_nsSet);
}

Namespace::~Namespace()
{
    delete _scope;
    _scope = nullptr;

    delete _varSet;
    _varSet = nullptr;

    delete _tySet;
    _tySet = nullptr;

    delete _nsSet;
    _nsSet = nullptr;

    _owner = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Module
Module::Module(const std::string& name, uint32_t moduleID, IScope* owner)
    : _name(name), _owner(owner), _ID(moduleID)
{
    NormalScope* scope = new NormalScope(name, owner);
    scope->TypeSet(new ModuleSetType());
    _scope = scope;
}

Module::~Module()
{
    delete _scope->TypeSet();
    delete _scope;
    _scope = nullptr;
}
GCF_NAMESPACE_END
