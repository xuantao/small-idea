#pragma once
#include "CfgDef.h"
#include "Interface.h"

CFG_NAMESPACE_BEGIN

/*
 * 原生类型
*/
class RawType : public IRawType
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
    virtual RawCategory Raw() const { return _raw; }

protected:
    std::string _name;
    RawCategory _raw;
};

/*
 * 枚举
*/
class EnumType : public IEnumType
{
public:
    EnumType(const std::string& name, IType* belong);
    ~EnumType();

public:
    virtual IType* Belong() const { return _belong; }
    virtual TypeCategory Category() const { return TypeCategory::Enum; }
    virtual const std::string& Name() const { return _name; }

    virtual ITypeSet* TypeSet() const { return nullptr; }
    virtual IVarSet* VarSet() const { return _vars; }

protected:
    std::string _name;
    IType* _belong;
    IVarSet* _vars;
};

/*
 * 结构体，兼容C#只能单继承
*/
class StructType : public IStructType
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

    virtual bool IsInherited(const IType* type) const;
    virtual IStructType* Inherited() const { return _inherit; }
    virtual IVarSet* OwnVars() const;

public:
    bool Inherit(StructType* type);

protected:
    std::string _name;
    IType* _belong;
    IVarSet* _vars;
    StructType* _inherit;
};

/*
 * 数组
*/
class ArrayType : public IArrayType
{
public:
    ArrayType(IType* prev, int length);
    ~ArrayType();

public:
    virtual IType* Belong() const { return nullptr; }
    virtual TypeCategory Category() const { return TypeCategory::Array; }
    virtual const std::string& Name() const { return _name; }

    virtual ITypeSet* TypeSet() const { return nullptr; }
    virtual IVarSet* VarSet() const { return nullptr; }

    virtual IType* Original() const;
    virtual IType* Prev() const { return _prev; }
    virtual int Length() const { return _length; }

protected:
    std::string _name;
    IType* _prev;
    int _length;
};

/*
 * scope
*/
class ScopeType : public IScopeType
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
