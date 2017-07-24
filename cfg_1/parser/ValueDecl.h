#pragma once
#include "CfgDef.h"

CFG_NAMESPACE_BEGIN

class BoolValue : public IValue
{
public:
    BoolValue(bool value)
        : _value(value)
    {
    }

public:
    virtual ValueCategory Category() const { return ValueCategory::Raw; }
    virtual const IValue* RawValue() const { return this; }

protected:
    bool _value;
};

class IntValue : public IValue
{
public:
    IntValue(int value)
        : _value(value)
    {
    }

public:
    virtual ValueCategory Category() const { return ValueCategory::Raw; }
    virtual const IValue* RawValue() const { return this; }

protected:
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
    StringValue(std::string value)
        : _value(value)
    {
    }

public:
    virtual ValueCategory Category() const { return ValueCategory::Raw; }
    virtual const IValue* RawValue() const { return this; }

protected:
    std::string _value;
};

class RefValue : public IValue
{
public:
    virtual ValueCategory Category() const { return ValueCategory::Ref; }
    virtual const IValue* RawValue() const { return this; }
};

CFG_NAMESPACE_END
