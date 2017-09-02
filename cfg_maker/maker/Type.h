#pragma once
#include "Interface.h"

CFG_NAMESPACE_BEGIN

/*
 * 原生类型
*/
class RawType : public IRawType
{
public:
    RawType(const std::string& name, RawCategory raw, IScope* owner)
        : _name(name)
        , _raw(raw)
        , _owner(owner)
    {
    }

public:
    /* IElement */
    virtual ElementCategory ElementCat() const { return ElementCategory::Type; }

    /* IType */
    virtual TypeCategory TypeCat() const { return TypeCategory::Raw; }
    virtual const std::string& Name() const { return _name; }
    virtual IScope* Owner() const { return _owner; }
    virtual IScope* Scope() const { return nullptr; }

    /* IRawType */
    virtual RawCategory RawCat() const { return _raw; }

protected:
    std::string _name;
    RawCategory _raw;
    IScope* _owner = nullptr;
};

/*
 * 枚举
*/
class EnumType : public IEnumType
{
public:
    EnumType(const std::string& name, IScope* owner);
    ~EnumType();

public:
    /* IElement */
    virtual ElementCategory ElementCat() const { return ElementCategory::Type; }

    /* IType */
    virtual TypeCategory TypeCat() const { return TypeCategory::Enum; }
    virtual const std::string& Name() const { return _name; }
    virtual IScope* Owner() const { return _owner; }
    virtual IScope* Scope() const { return _scope; }

protected:
    std::string _name;
    IScope* _owner;
    IScope* _scope;
    IVarSet* _vars;
};

/*
 * 结构体，兼容C#只能单继承
*/
class StructType : public IStructType
{
public:
    StructType(const std::string& name, IScope* owner, CfgCategory cfg);
    ~StructType();

public:
    /* IElement */
    virtual ElementCategory ElementCat() const { return ElementCategory::Type; }

    /* IType */
    virtual TypeCategory TypeCat() const { return TypeCategory::Struct; }
    virtual const std::string& Name() const { return _name; }
    virtual IScope* Owner() const { return _owner; }
    virtual IScope* Scope() const { return _scope; }

    /* IStructType */
    virtual CfgCategory CfgCat() const { return _cfg; }
    virtual bool IsInherited(const IStructType* type) const;
    virtual bool Inherit(IStructType* parent);
    virtual IStructType* Inherited() const { return _inherit; }
    virtual IScope* OwnScope() const { return _ownScope; }

protected:
    std::string _name;
    CfgCategory _cfg;
    IScope* _owner = nullptr;
    IStructType* _inherit = nullptr;

    IScope* _scope = nullptr;
    IScope* _ownScope = nullptr;
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
    /* IElement */
    virtual ElementCategory ElementCat() const { return ElementCategory::Type; }

    /* IType */
    virtual TypeCategory TypeCat() const { return TypeCategory::Array; }
    virtual const std::string& Name() const { return _name; }
    virtual IScope* Owner() const { return nullptr; }
    virtual IScope* Scope() const { return nullptr; }

    /* IArrayType */
    virtual IType* Original() const;
    virtual IType* Prev() const { return _prev; }
    virtual int Length() const { return _length; }

protected:
    std::string _name;
    IType* _prev;
    int _length;
};

/*
 * 函数
*/
class Function : public IFunction
{
public:
    Function(const std::string& name, IType* ret, IScope* owner);
    ~Function();

public:
    virtual ElementCategory ElementCat() const { return ElementCategory::Type; }
    virtual TypeCategory TypeCat() const { return TypeCategory::Fucntion; }
    virtual const std::string& Name() const { return _name; }
    virtual IType* RetType() const { return _ret; }
    virtual IScope* Owner() const { return _owner; }
    virtual IScope* Scope() const { return _scope; }

protected:
    std::string _name;
    IType* _ret = nullptr;
    IScope* _scope = nullptr;
    IScope* _owner = nullptr;
};

/*
 * 名字空间
*/
class Namespace : public INamespace
{
public:
    Namespace(const std::string& name, IScope* owner);
    ~Namespace();

public:
    virtual ElementCategory ElementCat() const { return ElementCategory::Namespace; }
    virtual const std::string& Name() const { return _name; }
    virtual IScope* Owner() const { return _owner; }
    virtual IScope* Scope() const { return _scope; }

protected:
    std::string _name;
    ITypeSet* _tySet;
    IVarSet* _varSet;
    INsSet* _nsSet;
    IScope* _owner;
    IScope* _scope;
};

/*
 * module
*/
class Module : public IModule
{
public:
    Module(const std::string& name, IScope* owner);
    ~Module();

public:
    virtual ElementCategory ElementCat() const { return ElementCategory::Module; }
    virtual const std::string& Name() const { return _name; }
    virtual IScope* Owner() const { return _owner; }
    virtual IScope* Scope() const { return _scope; }

protected:
    std::string _name;
    IScope* _owner = nullptr;
    IScope* _scope = nullptr;
};

CFG_NAMESPACE_END
