#pragma once
#include "gcf/gcf.h"
#include "utility/Utility.h"

GCF_NAMESPACE_BEGIN

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
    ElementCategory ElementCat() const override { return ElementCategory::Type; }
    const ILocation* Location() const override { return nullptr; }
    const IAttributeSet* Attributes() const override { return nullptr; }

    /* IType */
    TypeCategory TypeCat() const override { return TypeCategory::Raw; }
    const std::string& Name() const override { return _name; }
    IScope* Owner() const override { return _owner; }
    IScope* Scope() const override { return nullptr; }

    /* IRawType */
    RawCategory RawCat() const override { return _raw; }

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
    EnumType(
        const std::string& name,
        IScope* owner
    );

    ~EnumType();

public:
    /* IElement */
    ElementCategory ElementCat() const override { return ElementCategory::Type; }
    const ILocation* Location() const override { return _location; }
    const IAttributeSet* Attributes() const override { return _attributes; }

    /* IType */
    TypeCategory TypeCat() const override { return TypeCategory::Enum; }
    const std::string& Name() const override { return _name; }
    IScope* Owner() const override { return _owner; }
    IScope* Scope() const override { return _scope; }

public:
    void Location(ILocation* loc) { _location = loc; }
    void Attributes(IAttributeSet* attributs) { _attributes = attributs; }
    IAttributeSet* Attributes() { return _attributes; }

protected:
    std::string _name;
    ILocation* _location = nullptr;
    IScope* _owner = nullptr;
    IScope* _scope = nullptr;
    IVarSet* _vars = nullptr;
    IAttributeSet* _attributes = nullptr;
};

/*
 * 结构体，兼容C#只能单继承
*/
class StructType : public IStructType
{
public:
    StructType(
        const std::string& name,
        IScope* owner,
        CfgCategory cfg
    );

    ~StructType();

public:
    /* IElement */
    ElementCategory ElementCat() const override { return ElementCategory::Type; }
    const ILocation* Location() const override { return _location; }
    const IAttributeSet* Attributes() const override { return _attributes; }

    /* IType */
    TypeCategory TypeCat() const override { return TypeCategory::Struct; }
    const std::string& Name() const override { return _name; }
    IScope* Owner() const override { return _owner; }
    IScope* Scope() const override { return _scope; }

    /* IStructType */
    CfgCategory CfgCat() const override { return _cfg; }
    int32_t HashCode() const override { return _hashCode; }
    bool IsInherited(const IStructType* type) const override;
    bool Inherit(IStructType* parent) override;
    IStructType* Inherited() const override { return _inherit; }
    IScope* OwnScope() const override { return _ownScope; }

public:
    void Location(ILocation* loc) { _location = loc; }
    void Attributes(IAttributeSet* attributs) { _attributes = attributs; }
    IAttributeSet* Attributes() { return _attributes; }
    void DeclCompleted();

protected:
    ILocation* _location = nullptr;
    std::string _name;
    CfgCategory _cfg;
    int32_t _hashCode = 0;
    IScope* _owner = nullptr;
    IStructType* _inherit = nullptr;

    IScope* _scope = nullptr;
    IScope* _ownScope = nullptr;
    IAttributeSet* _attributes = nullptr;
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
    ElementCategory ElementCat() const override { return ElementCategory::Type; }
    const ILocation* Location() const override { return nullptr; }
    const IAttributeSet* Attributes() const override { return nullptr; }

    /* IType */
    TypeCategory TypeCat() const override { return TypeCategory::Array; }
    const std::string& Name() const override { return _name; }
    IScope* Owner() const override { return nullptr; }
    IScope* Scope() const override { return nullptr; }

    /* IArrayType */
    IType* Original() const override;
    IType* Prev() const override { return _prev; }
    int Length() const override { return _length; }

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
    Function(
        const std::string& name,
        IType* ret,
        IScope* owner
    );

    ~Function();

public:
    /* IElement */
    ElementCategory ElementCat() const override { return ElementCategory::Type; }
    const ILocation* Location() const override { return _location; }
    const IAttributeSet* Attributes() const override { return _attributes; }

    /* IType */
    virtual TypeCategory TypeCat() const { return TypeCategory::Fucntion; }
    virtual const std::string& Name() const { return _name; }
    virtual int32_t HashCode() const { return _hashCode; }

    /* IFunction */
    virtual IType* RetType() const { return _ret; }
    virtual IScope* Owner() const { return _owner; }
    virtual IScope* Scope() const { return _scope; }
    virtual const std::string& RawName() const { return _rawName; }

public:
    void Location(ILocation* loc) { _location = loc; }
    void Attributes(IAttributeSet* attributs) { _attributes = attributs; }
    IAttributeSet* Attributes() { return _attributes; }
    void DeclCompleted();

protected:
    std::string GetName(const IType* type);

protected:
    ILocation* _location = nullptr;
    std::string _name;
    std::string _rawName;
    int32_t _hashCode = 0;
    IType* _ret = nullptr;
    IScope* _scope = nullptr;
    IScope* _owner = nullptr;
    IAttributeSet* _attributes = nullptr;
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
    /* IElement */
    ElementCategory ElementCat() const override { return ElementCategory::Namespace; }
    const ILocation* Location() const override { return nullptr; }
    const IAttributeSet* Attributes() const override { return nullptr; }

    /* INamespace */
    const std::string& Name() const override { return _name; }
    IScope* Owner() const override { return _owner; }
    IScope* Scope() const override { return _scope; }

protected:
    std::string _name;
    ITypeSet* _tySet;
    IVarSet* _varSet;
    INsSet* _nsSet;
    IScope* _owner;
    IScope* _scope;
};

/*
 * CrossCall
*/
class CrossCall : public ICrossCall
{
public:
    CrossCall(
        const std::string& name,
        int32_t ID,
        IScope* owner
    );

    ~CrossCall();

public:
    /* IElement */
    virtual ElementCategory ElementCat() const { return ElementCategory::CrossCall; }
    const ILocation* Location() const override { return _location; }
    const IAttributeSet* Attributes() const override { return _attributes; }

    /* ICrossCall */
    virtual const std::string& Name() const { return _name; }
    virtual int32_t ID() const { return _ID; }
    virtual IScope* Owner() const { return _owner; }
    virtual IScope* Scope() const { return _scope; }

public:
    void Location(ILocation* loc) { _location = loc; }
    void Attributes(IAttributeSet* attributs) { _attributes = attributs; }
    IAttributeSet* Attributes() { return _attributes; }

protected:
    ILocation* _location = nullptr;
    std::string _name;
    int32_t _ID = 0;;
    IScope* _owner = nullptr;
    IScope* _scope = nullptr;
    IAttributeSet* _attributes = nullptr;
};

GCF_NAMESPACE_END
