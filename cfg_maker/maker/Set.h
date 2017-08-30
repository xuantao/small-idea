#pragma once
#include "Interface.h"
#include <algorithm>

CFG_NAMESPACE_BEGIN

/*
 * 类型集合
*/
class TypeSetNormal : public ITypeSet
{
public:
    TypeSetNormal();
    ~TypeSetNormal();

public:
    virtual IType* Get(const std::string& name) const;
    virtual IType* Get(int index) const;
    virtual int Size() const { return (int)_types.size(); }
    virtual bool Add(IType* type);
    virtual bool Traverse(const std::function<bool(IType*)>& func) const;
private:
    std::vector<IType*> _types;
};

/*
 * function type set
 * for module
*/
class ModuleSetType : public TypeSetNormal
{
public:
    ModuleSetType();
    ~ModuleSetType();

public:
    virtual bool Add(IType* type);
};

/*
 * 成员变量集合
*/
class VarSetNormal : public IVarSet
{
public:
    VarSetNormal();
    ~VarSetNormal();

public:
    virtual IVariate* Get(const std::string& name) const;
    virtual IVariate* Get(int index) const;
    virtual int Size() const { return (int)_vars.size(); }
    virtual bool Add(IVariate* var);
    virtual bool Traverse(const std::function<bool(IVariate*)>& func) const;
protected:
    std::vector<IVariate*> _vars;
};

/*
 * 枚举的成员变量集合
 * 枚举的成员含有初始值生成规则
*/
class EnumType;
class EnumVarSet : public VarSetNormal
{
public:
    EnumVarSet();

public:
    virtual bool Add(IVariate* var);
};

/*
 * 针对struct的成员变量集合
 * struct含有继承
*/
class StructVarSet : public IVarSet
{
public:
    StructVarSet(IStructType* belong);
    ~StructVarSet();

public:
    virtual IVariate* Get(const std::string& name) const;
    virtual IVariate* Get(int index) const;
    virtual int Size() const;
    virtual bool Add(IVariate* var);
    virtual bool Traverse(const std::function<bool(IVariate*)>& func) const;
public:
    IVarSet* OwnVars() { return &_self; }
    const IVarSet* OwnVars() const { return &_self; }

protected:
    VarSetNormal _self;
    IStructType* _struct;
};

/*
 * NsSet
*/
class NsSet : public INsSet
{
public:
    virtual ~NsSet();
public:
    virtual INamespace* Get(const std::string& name) const;
    virtual INamespace* Get(int index) const;
    virtual int Size() const { return (int)_ns.size(); }

    virtual bool Add(INamespace* ns);
    virtual bool Traverse(const std::function<bool(INamespace*)>& func) const;
protected:
    std::vector<INamespace*> _ns;
};
CFG_NAMESPACE_END
