#include "Type.h"
#include "Utility.h"
#include "Set.h"
#include "Value.h"
#include "Scope.h"
#include "ValueUtil.h"
#include <algorithm>

CFG_NAMESPACE_BEGIN

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
    , _inherit(nullptr)
    , _vars(nullptr)
    , _scope(nullptr)
    , _ownScope(nullptr)
{
    StructVarSet* vars = new StructVarSet(this);
    NormalScope* scope = new NormalScope(name, owner);
    NormalScope* ownScope = new NormalScope(name, owner);

    scope->Bind(this);
    scope->VarSet(vars);

    ownScope->Bind(this);
    ownScope->VarSet(vars->OwnVars());

    _vars = vars;
    _scope = scope;
    _ownScope = ownScope;
}

StructType::~StructType()
{
    delete _ownScope;
    _ownScope = nullptr;

    delete _scope;
    _scope = nullptr;

    delete _vars;
    _vars = nullptr;

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

//////////////////////////////////////////////////////////////////////////
// Function
Function::Function(const std::string& name, IType* ret, IScope* owner)
    : _name(name)
    , _ret(ret)
    , _owner(owner)
{
    NormalScope* scope = new NormalScope(name, owner);
    scope->VarSet(new VarSetNormal());
    _scope = scope;
}

Function::~Function()
{
    delete _scope->VarSet();
    delete _scope;
    _scope = nullptr;
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
Module::Module(const std::string& name, IScope* owner)
    : _name(name), _owner(owner)
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
CFG_NAMESPACE_END
