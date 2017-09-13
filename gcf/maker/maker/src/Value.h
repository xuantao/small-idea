#pragma once
#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

class RawValue : public IRawValue
{
public:
    RawValue(bool value);
    RawValue(int value);
    RawValue(float value);
    RawValue(const std::string& value);

public:
    virtual ValueCategory ValueCat() const { return ValueCategory::Raw; }
    virtual RawCategory RawCat() const { return _raw; }

    virtual bool AsBool() const;
    virtual int AsInt() const;
    virtual float AsFloat() const;
    virtual const char* AsString() const;

public:
    bool Value(bool& b) const;
    bool Value(int& i) const;
    bool Value(float& f) const;
    bool Value(std::string& str) const;

protected:
    RawCategory _raw;
    union
    {
        bool _b;
        int _i;
        float _f;
    };
    std::string _s;
};

class RefValue : public IRefValue
{
public:
    RefValue(IVariate* var) : _var(var)
    {
    }

public:
    virtual ValueCategory ValueCat() const { return ValueCategory::Ref; }

    virtual IVariate* Var() const { return _var; }
    virtual IRawValue* Original() const;

protected:
    IVariate* _var;
};

GCF_NAMESPACE_END
