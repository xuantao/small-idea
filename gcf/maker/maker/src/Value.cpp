#include "Value.h"

GCF_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
// RawValue
namespace detail
{
    ValueImpl::ValueImpl(bool value)
        : _raw(RawCategory::Bool)
        , _bool(value)
    {
    }

    ValueImpl::ValueImpl(int8_t value)
        : _raw(RawCategory::Byte)
        , _byte(value)
    {
    }

    ValueImpl::ValueImpl(int32_t value)
        : _raw(RawCategory::Int)
        , _int(value)
    {
    }

    ValueImpl::ValueImpl(int64_t value)
        : _raw(RawCategory::Long)
        , _int64_(value)
    {
    }

    ValueImpl::ValueImpl(float value)
        : _raw(RawCategory::Float)
        , _float(value)
    {
    }

    ValueImpl::ValueImpl(double value)
        : _raw(RawCategory::Double)
        , _double(value)
    {
    }

    ValueImpl::ValueImpl(const std::string& value)
        : _raw(RawCategory::String)
        , _str(value)
    {
    }

    ValueImpl::ValueImpl(RawCategory raw, IValue* val/* = nullptr*/)
        : _raw(raw)
        , _int64_(0)
    {
        if (val == nullptr)
            return;

        if (raw == RawCategory::Bool)
            val->ToValue(_bool);
        else if (raw == RawCategory::Byte)
            val->ToValue(_byte);
        else if (raw == RawCategory::Int)
            val->ToValue(_int);
        else if (raw == RawCategory::Long)
            val->ToValue(_int64_);
        else if (raw == RawCategory::Float)
            val->ToValue(_float);
        else if (raw == RawCategory::Double)
            val->ToValue(_double);
        else if (raw == RawCategory::String)
            val->ToValue(_str);
    }

    bool ValueImpl::ToValue(bool& val) const
    {
        if (_raw == RawCategory::Bool)
            val = _bool;
        else
            return false;

        return true;
    }

    bool ValueImpl::ToValue(int8_t& val) const
    {
        if (_raw == RawCategory::Byte)
            val = _byte;
        else if (_raw == RawCategory::Int)
            val = (int8_t)_int;
        else if (_raw == RawCategory::Long)
            val = (int8_t)_int64_;
        else if (_raw == RawCategory::Float)
            val = (int8_t)_float;
        else if (_raw == RawCategory::Double)
            val = (int8_t)_double;
        else
            return false;

        return true;
    }

    bool ValueImpl::ToValue(int32_t& val) const
    {
        if (_raw == RawCategory::Byte)
            val = _byte;
        else if (_raw == RawCategory::Int)
            val = _int;
        else if (_raw == RawCategory::Long)
            val = (int32_t)_int64_;
        else if (_raw == RawCategory::Float)
            val = (int32_t)_float;
        else if (_raw == RawCategory::Double)
            val = (int32_t)_double;
        else
            return false;

        return true;
    }

    bool ValueImpl::ToValue(int64_t& val) const
    {
        if (_raw == RawCategory::Byte)
            val = _byte;
        else if (_raw == RawCategory::Int)
            val = _int;
        else if (_raw == RawCategory::Long)
            val = _int64_;
        else if (_raw == RawCategory::Float)
            val = (int64_t)_float;
        else if (_raw == RawCategory::Double)
            val = (int64_t)_double;
        else
            return false;

        return true;
    }

    bool ValueImpl::ToValue(float& val) const
    {
        if (_raw == RawCategory::Byte)
            val = (float)_byte;
        else if (_raw == RawCategory::Int)
            val = (float)_int;
        else if (_raw == RawCategory::Long)
            val = (float)_int64_;
        else if (_raw == RawCategory::Float)
            val = _float;
        else if (_raw == RawCategory::Double)
            val = (float)_double;
        else
            return false;

        return true;
    }

    bool ValueImpl::ToValue(double& val) const
    {
        if (_raw == RawCategory::Byte)
            val = (double)_byte;
        else if (_raw == RawCategory::Int)
            val = (double)_int;
        else if (_raw == RawCategory::Long)
            val = (double)_int64_;
        else if (_raw == RawCategory::Float)
            val = _float;
        else if (_raw == RawCategory::Double)
            val = _double;
        else
            return false;

        return true;
    }

    bool ValueImpl::ToValue(std::string& val) const
    {
        if (_raw == RawCategory::String)
            val = _str;
        else
            return false;

        return true;
    }
}
//////////////////////////////////////////////////////////////////////////
// RefValue
IValue* RefValue::Original() const
{
    IValue* val = _var->Value();
    while (val)
    {
        if (val->ValueCat() == ValueCategory::Raw)
            break;

        if (val->ValueCat() == ValueCategory::Ref)
            val = static_cast<RefValue*>(val)->_var->Value();
        else
            val = nullptr;
    }

    return static_cast<IRawValue*>(val);
}

GCF_NAMESPACE_END
