#pragma once
#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

class RawValue : public IRawValue
{
public:
    RawValue(bool value);
    RawValue(int8_t value);
    RawValue(int32_t value);
    RawValue(float value);
    RawValue(double value);
    RawValue(int64_t value);
    RawValue(const std::string& value);

public:
    virtual ValueCategory ValueCat() const { return ValueCategory::Raw; }
    virtual RawCategory RawCat() const { return _raw; }

    virtual bool AsBool() const;
    virtual int8_t AsByte() const;
    virtual int32_t AsInt() const;
    virtual float AsFloat() const;
    virtual double AsDouble() const;
    virtual int64_t AsLong() const;
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
        bool _bool;
        int8_t _byte;
        int32_t _int;
        float _float;
        double _double;
        int64_t _int64_;
    };
    std::string _str;
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
