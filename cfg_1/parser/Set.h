#pragma once
#include "CfgDef.h"
#include <vector>
#include <algorithm>

CFG_NAMESPACE_BEGIN

/*
 * 类型集合
*/
class TypeSetNormal : public ITypeSet
{
public:
    TypeSetNormal(IType* belong);
    ~TypeSetNormal();

public:
    virtual IType* Belong() const { return _belong; }
    virtual IType* Get(const std::string& name) const;
    virtual IType* Get(int index) const;
    virtual int Size() const { return (int)_types.size(); }
    virtual bool Add(IType* type);

private:
    IType* _belong;
    std::vector<IType*> _types;
};


/*
 * 成员变量集合
*/
class VarSetNormal : public IVarSet
{
public:
    VarSetNormal(IType* belong);
    ~VarSetNormal();

public:
    virtual IType* Belong() const { return _belong; }
    virtual IVariate* Get(const std::string& name) const;
    virtual IVariate* Get(int index) const;
    virtual int Size() const { return (int)_vars.size(); }
    virtual bool Add(IVariate* var);

protected:
    IType* _belong;
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
    EnumVarSet(EnumType* belong);

public:
    virtual bool Add(IVariate* var);
};


/*
 * 针对struct的成员变量集合
 * struct含有继承
*/
class StructType;
class StructVarSet : public VarSetNormal
{
public:
    StructVarSet(StructType* belong);
    ~StructVarSet();

public:
    virtual IVariate* Get(const std::string& name) const;
    virtual IVariate* Get(int index) const;
    virtual int Size() const;

    virtual bool Add(IVariate* var);

protected:
    StructType* _struct;
};

CFG_NAMESPACE_END
