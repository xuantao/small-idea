#include "SetDecl.h"
#include "TypeDecl.h"

CFG_NAMESPACE_BEGIN

StructVarSet::StructVarSet(StructType* belong)
    : _belong(belong)
{
}

StructVarSet::~StructVarSet()
{
    std::for_each(_vars.begin(), _vars.end(), [ ](IVariate* var) { delete var; });
    _vars.clear();
    _belong = nullptr;
}

IType* StructVarSet::Belong() const
{
    return _belong;
}

IVariate* StructVarSet::Get(const std::string& name) const
{
    IVariate* var = nullptr;
    IType* inherit = _belong->GetInherited();
    if (inherit && inherit->VarSet())
    {
        var = inherit->VarSet()->Get(name);
        if (var)
            return var;
    }

    auto it = std::find_if(_vars.cbegin(), _vars.cend(),
        [&name](IVariate* var) { return var->Name() == name; });
    if (it == _vars.cend())
        return nullptr;
    return *it;
}

IVariate* StructVarSet::Get(int index) const
{
    if (index < 0)
        return nullptr;

    IType* inherit = _belong->GetInherited();
    if (inherit && inherit->VarSet())
    {
        if (index < inherit->VarSet()->Size())
            return inherit->VarSet()->Get(index);
        else
            index -= inherit->VarSet()->Size();
    }

    if (index >= _vars.size())
        return nullptr;
    return _vars[index];
}

int StructVarSet::Size() const
{
    int size = 0;
    IType* inherit = _belong->GetInherited();
    if (inherit && inherit->VarSet())
        size = inherit->VarSet()->Size();

    return (int)_vars.size() + size;
}

bool StructVarSet::Add(IVariate* var)
{
    if (var == nullptr || Get(var->Name()))
        return false;

    _vars.push_back(var);
    return true;
}

CFG_NAMESPACE_END
