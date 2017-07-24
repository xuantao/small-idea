#pragma once
#include "CfgDef.h"

CFG_NAMESPACE_BEGIN

class BoolValue : public IValue
{
public:
    BoolValue(const IType* type, bool value)
        : _type(type), _value(value)
    {
    }

public:
    virtual ValueCategory Category() const { return ValueCategory::Raw; }
    virtual const IValue* RawValue() const { return this; }

protected:
    const IType* _type;
    bool _value;
};

class IntValue : public IValue
{
public:
    IntValue(const IType* type, int value)
        : _type(type), _value(value)
    {
    }

public:
    virtual ValueCategory Category() const { return ValueCategory::Raw; }
    virtual const IValue* RawValue() const { return this; }

protected:
    const IType* _type;
    int _value;
};

class FloatValue : public IValue
{
public:
    FloatValue(float value)
        : _value(value)
    {
    }

public:
    virtual ValueCategory Category() const { return ValueCategory::Raw; }
    virtual const IValue* RawValue() const { return this; }

protected:
    float _value;
};

class StringValue : public IValue
{
public:
    StringValue(const IType* type, std::string value)
        : _type(type), _value(value)
    {
    }

public:
    virtual ValueCategory Category() const { return ValueCategory::Raw; }
    virtual const IValue* RawValue() const { return this; }

protected:
    const IType* _type;
    std::string _value;
};

class RefValue : public IValue
{
public:
    virtual ValueCategory Category() const { return ValueCategory::Ref; }
    virtual const IValue* RawValue() const { return this; }
};

CFG_NAMESPACE_END
