#include "Set.h"
#include "Type.h"
#include "Value.h"
#include "ValueUtil.h"

CFG_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// TypeSetNormal
TypeSetNormal::TypeSetNormal()
{
}

TypeSetNormal::~TypeSetNormal()
{
    std::for_each(_types.rbegin(), _types.rend(), [ ](IType* ty) { delete ty; });
    _types.clear();
}

IType* TypeSetNormal::Get(const std::string& name) const
{
    auto it = std::find_if(_types.cbegin(), _types.cend(),
        [&name](IType* ty) { return ty->Name() == name; });
    if (it == _types.cend())
        return nullptr;
    return *it;
}

IType* TypeSetNormal::Get(int index) const
{
    if (index < 0 || index > Size())
        return nullptr;
    return _types[index];
}

bool TypeSetNormal::Add(IType* type)
{
    if (type == nullptr || Get(type->Name()))
        return false;

    _types.push_back(type);
    return true;
}

bool TypeSetNormal::Traverse(const std::function<bool(IType *)>& func) const
{
    for (auto it = _types.cbegin(); it != _types.cend(); ++it)
    {
        if (!func(*it))
            return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
// VarSetNormal
VarSetNormal::VarSetNormal()
{
}

VarSetNormal::~VarSetNormal()
{
    std::for_each(_vars.rbegin(), _vars.rend(), [ ](IVariate* var) { delete var; });
    _vars.clear();
}

IVariate* VarSetNormal::Get(const std::string& name) const
{
    auto it = std::find_if(_vars.cbegin(), _vars.cend(),
        [&name](IVariate* var) { return var->Name() == name; });
    if (it == _vars.cend())
        return nullptr;
    return *it;
}

IVariate* VarSetNormal::Get(int index) const
{
    if (index < 0 || index > Size())
        return nullptr;
    return _vars[index];
}

bool VarSetNormal::Add(IVariate* var)
{
    if (var == nullptr || Get(var->Name()))
        return false;

    _vars.push_back(var);
    return true;
}

bool VarSetNormal::Traverse(const std::function<bool(IVariate *)>& func) const
{
    for (auto it = _vars.cbegin(); it != _vars.cend(); ++it)
    {
        if (!func(*it))
            return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
// EnumVarSet
EnumVarSet::EnumVarSet()
    : VarSetNormal()
{
}

bool EnumVarSet::Add(IVariate* var)
{
    if (Get(var->Name()))
        return false;

    if (var->Value() == nullptr)
    {
        if (Size() == 0)
        {
            var->BindValue(new RawValue((int)0));
        }
        else
        {
            IVariate* prev = _vars.back();
            int val = 0;
            if (value_util::Value(prev->Value(), val))
                var->BindValue(new RawValue(val + 1));
            else
                return false;
        }
    }

    return VarSetNormal::Add(var);
}

//////////////////////////////////////////////////////////////////////////
// StructVarSet
StructVarSet::StructVarSet(IStructType* belong)
    : _self(), _struct(belong)
{
}

StructVarSet::~StructVarSet()
{
    _struct = nullptr;
}

IVariate* StructVarSet::Get(const std::string& name) const
{
    IVariate* var = _self.Get(name);
    if (var)
        return var;

    IStructType* inherit = _struct->Inherited();
    if (inherit && inherit->Scope() && inherit->Scope()->VarSet())
        var = inherit->Scope()->VarSet()->Get(name);
    return var;
}

IVariate* StructVarSet::Get(int index) const
{
    IStructType* inherit = _struct->Inherited();
    if (inherit && inherit->Scope() && inherit->Scope()->VarSet())
    {
        IVarSet* set = inherit->Scope()->VarSet();
        if (index < set->Size())
            return set->Get(index);
        else
            index -= set->Size();
    }

    return _self.Get(index);
}

int StructVarSet::Size() const
{
    int size = 0;
    IStructType* inherit = _struct->Inherited();
    if (inherit && inherit->Scope() && inherit->Scope()->VarSet())
        size = inherit->Scope()->VarSet()->Size();
    return _self.Size() + size;
}

bool StructVarSet::Add(IVariate* var)
{
    return _self.Add(var);
}

bool StructVarSet::Traverse(const std::function<bool(IVariate *)>& func) const
{
    bool needStop = true;
    IStructType* inherit = _struct->Inherited();
    if (inherit && inherit->Scope() && inherit->Scope()->VarSet())
        needStop = inherit->Scope()->VarSet()->Traverse(func);

    if (needStop)
        needStop = _self.Traverse(func);
    return needStop;
}

//////////////////////////////////////////////////////////////////////////
// NsSet
NsSet::~NsSet()
{
    std::for_each(_ns.rbegin(), _ns.rend(), [ ](INamespace* ns) { delete ns; });
    _ns.clear();
}


INamespace* NsSet::Get(const std::string& name) const
{
    auto it = std::find_if(_ns.cbegin(), _ns.cend(),
        [&name](INamespace* ns) { return ns->Name() == name; });
    if (it == _ns.cend())
        return nullptr;
    return *it;
}

INamespace* NsSet::Get(int index) const
{
    if (index < 0 || index > Size())
        return nullptr;
    return _ns[index];
}

bool NsSet::Add(INamespace* ns)
{
    if (ns == nullptr || Get(ns->Name()))
        return false;

    _ns.push_back(ns);
    return true;
}

bool NsSet::Traverse(const std::function<bool(INamespace *)>& func) const
{
    for (auto it = _ns.cbegin(); it != _ns.cend(); ++it)
    {
        if (!func(*it))
            return false;
    }
    return true;
}

CFG_NAMESPACE_END
