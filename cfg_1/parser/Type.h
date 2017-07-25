#pragma once
#include "CfgDef.h"
#include <string>
#include <vector>

CFG_NAMESPACE_BEGIN

/*
 * 原生类型
*/
class RawType : public IType
{
public:
    RawType(const std::string& name, RawCategory raw)
        : _name(name)
        , _raw(raw)
    {
    }

public:
    virtual IType* Belong() const { return nullptr; }
    virtual TypeCategory Category() const { return TypeCategory::Raw; }
    virtual const std::string& Name() const { return _name; }

    virtual ITypeSet* TypeSet() const { return nullptr; }
    virtual IVarSet* VarSet() const { return nullptr; }

    RawCategory Raw() const { return _raw; }

protected:
    std::string _name;
    RawCategory _raw;
};

/*
 * 枚举
*/
class EnumType : public IType
{
public:
    EnumType(const std::string& name, IType* belong);
    ~EnumType();

public:
    virtual IType* Belong() const { return nullptr; }
    virtual TypeCategory Category() const { return TypeCategory::Enum; }
    virtual const std::string& Name() const { return _name; }

    virtual ITypeSet* TypeSet() const { return nullptr; }
    virtual IVarSet* VarSet() const { return _vars; }
public:
    /*
     * 值与名字互转
    */
    int Trans(const std::string& name) const;
    const std::string& Trans(int value) const;

protected:
    std::string _name;
    IType* _belong;
    IVarSet* _vars;
};

/*
 * 结构体，兼容C#只能单继承
*/
class StructType : public IType
{
public:
    StructType(const std::string& name, IType* belong);
    ~StructType();

public:
    virtual IType* Belong() const { return _belong; }
    virtual TypeCategory Category() const { return TypeCategory::Struct; }
    virtual const std::string& Name() const { return _name; }

    virtual ITypeSet* TypeSet()  const { return nullptr; }
    virtual IVarSet* VarSet() const { return _vars; }

public:
    IType* Inherited() const { return _inherit; }
    bool Inherit(IType* type);

protected:
    std::string _name;
    IType* _belong;
    IVarSet* _vars;
    IType* _inherit;
};

/*
 * 数组
*/
class ArrayType : public IType
{
public:
    ArrayType(const IType* prev, int length);
    ~ArrayType();

public:
    virtual IType* Belong() const { return nullptr; }
    virtual TypeCategory Category() const { return TypeCategory::Scope; }
    virtual const std::string& Name() const { return _name; }

    virtual ITypeSet* TypeSet() const { return nullptr; }
    virtual IVarSet* VarSet() const { return nullptr; }

public:
    const IType* Prev() const { return _prev; }
    int Length() const { return _length; }

protected:
    std::string _name;
    const IType* _prev;
    int _length;
};

/*
 * scope
*/
class ScopeType : public IType
{
public:
    ScopeType(const std::string& name, ScopeType* belong = nullptr);
    ~ScopeType();

public:
    virtual ScopeType* Belong() const { return _belong; }
    virtual TypeCategory Category() const { return TypeCategory::Scope; }
    virtual const std::string& Name() const { return _name; }

    virtual ITypeSet* TypeSet() const { return _types; }
    virtual IVarSet* VarSet() const { return _vars; }

protected:
    std::string _name;
    ScopeType* _belong;
    ITypeSet* _types;
    IVarSet* _vars;
};
CFG_NAMESPACE_END
