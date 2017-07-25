#pragma once
#include "CfgDef.h"

CFG_NAMESPACE_BEGIN

class RawValue : public IValue
{
public:
    RawValue(bool value);
    RawValue(int value);
    RawValue(float value);
    RawValue(const std::string& value);

public:
    virtual ValueCategory Category() const { return ValueCategory::Raw; }

public:
    RawCategory Raw() const { return _raw; }
    bool Value(bool& b) const;
    bool Value(int& i) const;
    bool Value(float& f) const;
    bool Value(std::string& str) const;

protected:
    RawCategory _raw;
    bool _b;
    int _i;
    float _f;
    std::string _s;
};

class RefValue : public IValue
{
public:
    RefValue(const IVariate* var) : _var(var)
    {
    }

public:
    virtual ValueCategory Category() const { return ValueCategory::Ref; }

    const IVariate* Var() const { return _var; }
    const RawValue* GetRaw() const;

protected:
    const IVariate* _var;
};

CFG_NAMESPACE_END
