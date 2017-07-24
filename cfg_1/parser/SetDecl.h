#pragma once
#include "CfgDef.h"
#include <vector>
#include <algorithm>

CFG_NAMESPACE_BEGIN

class TypeSetDecl : public ITypeSet
{
public:
    TypeSetDecl(IType* belong) :_belong(belong)
    {
    }

    ~TypeSetDecl()
    {
        std::for_each(_types.begin(), _types.end(), [ ](IType* ty) { delete ty; });
        _types.clear();
    }

public:
    virtual IType* Belong() const
    {
        return _belong;
    }

    virtual IType* Get(const std::string& name) const
    {
        auto it = std::find_if(_types.cbegin(), _types.cend(),
            [&name](IType* ty) { return ty->Name() == name; });
        if (it == _types.cend())
            return nullptr;
        return *it;
    }

    virtual IType* Get(int index) const
    {
        if (index < 0 || index > Size())
            return nullptr;
        return _types[index];
    }

    virtual int Size() const
    {
        return (int)_types.size();
    }

    virtual bool Add(IType* type)
    {
        if (type == nullptr || Get(type->Name()))
            return false;

        _types.push_back(type);
        return true;
    }

private:
    IType* _belong;
    std::vector<IType*> _types;
};


class VarSetDecl : public IVarSet
{
public:
    VarSetDecl(IType* belong) : _belong(belong)
    {
    }

    ~VarSetDecl()
    {
        std::for_each(_vars.begin(), _vars.end(), [ ](IVariate* var) { delete var; });
        _vars.clear();
    }

public:
    virtual IType* Belong() const
    {
        return _belong;
    }

    virtual IVariate* Get(const std::string& name) const
    {
        auto it = std::find_if(_vars.cbegin(), _vars.cend(),
            [&name](IVariate* var) { return var->Name() == name; });
        if (it == _vars.cend())
            return nullptr;
        return *it;
    }

    virtual IVariate* Get(int index) const
    {
        if (index < 0 || index > Size())
            return nullptr;
        return _vars[index];
    }

    virtual int Size() const
    {
        return (int)_vars.size();
    }

    virtual bool Add(IVariate* var)
    {
        if (var == nullptr || Get(var->Name()))
            return false;

        _vars.push_back(var);
        return true;
    }

private:
    IType* _belong;
    std::vector<IVariate*> _vars;
};

class StructType;
class StructVarSet : public IVarSet
{
public:
    StructVarSet(StructType* belong);
    ~StructVarSet();

public:
    virtual IType* Belong() const;
    virtual IVariate* Get(const std::string& name) const;
    virtual IVariate* Get(int index) const;
    virtual int Size() const;

    virtual bool Add(IVariate* var);

protected:
    StructType* _belong;
    std::vector<IVariate*> _vars;
};

CFG_NAMESPACE_END
