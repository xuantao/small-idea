#pragma once
#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

namespace detail
{
    class ValueImpl
    {
    public:
        ValueImpl(RawCategory raw, IValue* val = nullptr);
        ValueImpl(bool val);
        ValueImpl(int8_t val);
        ValueImpl(int32_t val);
        ValueImpl(float val);
        ValueImpl(double val);
        ValueImpl(int64_t val);
        ValueImpl(const std::string& val);

    public:
        RawCategory RawCat() const { return _raw; }

        bool ToValue(bool& val) const;
        bool ToValue(int8_t& val) const;
        bool ToValue(int32_t& val) const;
        bool ToValue(int64_t& val) const;
        bool ToValue(float& val) const;
        bool ToValue(double& val) const;
        bool ToValue(std::string& val) const;

    protected:
        RawCategory _raw = RawCategory::Int;
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
}

class RawValue : public IRawValue
{
public:
    RawValue(const detail::ValueImpl& val) : _val(val)
    {
    }

public:
    virtual ValueCategory ValueCat() const { return ValueCategory::Raw; }
    virtual RawCategory RawCat() const { return _val.RawCat(); }

    virtual bool ToValue(bool& val) const { return _val.ToValue(val); }
    virtual bool ToValue(int8_t& val) const { return _val.ToValue(val); }
    virtual bool ToValue(int32_t& val) const { return _val.ToValue(val); }
    virtual bool ToValue(int64_t& val) const { return _val.ToValue(val); }
    virtual bool ToValue(float& val) const { return _val.ToValue(val); }
    virtual bool ToValue(double& val) const { return _val.ToValue(val); }
    virtual bool ToValue(std::string& val) const { return _val.ToValue(val); }

protected:
    detail::ValueImpl _val;
};

class RefValue : public IRefValue
{
public:
    RefValue(IVariate* var, const detail::ValueImpl& val) : _var(var), _val(val)
    {
    }

public:
    virtual ValueCategory ValueCat() const { return ValueCategory::Ref; }
    virtual RawCategory RawCat() const { return _val.RawCat(); }

    virtual bool ToValue(bool& val) const { return _val.ToValue(val); }
    virtual bool ToValue(int8_t& val) const { return _val.ToValue(val); }
    virtual bool ToValue(int32_t& val) const { return _val.ToValue(val); }
    virtual bool ToValue(int64_t& val) const { return _val.ToValue(val); }
    virtual bool ToValue(float& val) const { return _val.ToValue(val); }
    virtual bool ToValue(double& val) const { return _val.ToValue(val); }
    virtual bool ToValue(std::string& val) const { return _val.ToValue(val); }

    virtual IVariate* Var() const { return _var; }
    virtual IValue* Original() const;

protected:
    IVariate* _var;
    detail::ValueImpl _val;
};

GCF_NAMESPACE_END
