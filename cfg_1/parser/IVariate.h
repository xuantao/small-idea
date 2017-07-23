#pragma once

#include "CfgDef.h"
#include <string>

CFG_NAMESPACE_BEGIN

enum class TypeCategory
{
    Raw,
    Enum,
    Struct,
    Array,
};

class IType;
class IValue;

/*
 * 类型
*/
class IType
{
public:
    virtual ~IType() {}
public:
    virtual TypeCategory Category() = 0;
    virtual const std::string& Name() const = 0;
};

class RawType : public IType
{
public:
    RawType(const std::string& name) : _name(name)
    { }

public:
    virtual TypeCategory Category() const
    { return TypeCategory::Raw; }

    virtual const std::string& Name() const
    { return _name; }

protected:
    std::string _name;
};

class EnumType : public IType
{
public:
    EnumType(const std::string& name) : _name(name)
    { }

public:
    virtual TypeCategory Category() const
    { return TypeCategory::Enum; }

    virtual const std::string& Name() const
    { return _name; }

public:
    /*
     * 值与名字互转
    */
    int Trans(const std::string& name) const;
    const std::string& Trans(int value) const;

protected:
    std::string _name;
};

class StructType : public IType
{
public:
    StructType(const std::string& name) : _name(name)
    { }

public:
    virtual TypeCategory Category() const
    { return TypeCategory::Struct; }

    virtual const std::string& Name() const
    { return _name; }

public:
    int Count() const;
    const IVariate* Member(int index) const;

protected:
    std::string _name;
};

class ArrayType : public IType
{
public:
    ArrayType(const std::string& name) : _name(name)
    { }

public:
    virtual TypeCategory Category() const
    { return TypeCategory::Array; }

    virtual const std::string& Name() const
    { return _name; }

public:
    IType* RayType() const;

    int Length() const;

protected:
    std::string _name;
};

/*
 * 变量
*/
class IVariate
{
public:
    virtual ~IVariate() {}
public:
    virtual bool IsConst() = 0;
    virtual const std::string& Name() const = 0;
    virtual const IType* Type() const = 0;
    virtual IValue* Value() const = 0;
};

class Variate : public IVariate
{
public:
    Variate(const std::string& name, const IType* type)
        : _name(name), _type(type)
    { }

public:
    virtual bool IsConst() { return false; }
    virtual const std::string& Name() const { return _name; }
    virtual const IType* Type() const { return _type; }
    virtual IValue* Value() const { return _value; }

protected:
    std::string _name;
    const IType* _type;
    IValue* _value;
};

class IValue
{

};

class BoolValue : public IValue
{
public:

};

class IntValue : public IValue
{

};

class FloatValue : public IValue
{

};
CFG_NAMESPACE_END
