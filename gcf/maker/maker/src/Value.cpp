#include "Value.h"

GCF_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
// RawValue
RawValue::RawValue(bool value)
    : _raw(RawCategory::Bool)
    , _bool(value)
{
}

RawValue::RawValue(int value)
    : _raw(RawCategory::Int)
    , _int(value)
{
}

RawValue::RawValue(float value)
    : _raw(RawCategory::Float)
    , _float(value)
{
}

RawValue::RawValue(const std::string& value)
    : _raw(RawCategory::String)
    , _str(value)
{
}

bool RawValue::AsBool() const
{
    assert(_raw == RawCategory::Bool);
    return _bool;
}

int8_t RawValue::AsByte() const
{
    assert(_raw == RawCategory::Byte);
    return _byte;
}

int32_t RawValue::AsInt() const
{
    assert(_raw == RawCategory::Int);
    return _int;
}

float RawValue::AsFloat() const
{
    assert(_raw == RawCategory::Float);
    return _float;
}

double RawValue::AsDouble() const
{
    assert(_raw == RawCategory::Double);
    return _double;
}


int64_t RawValue::AsLong() const
{
    assert(_raw == RawCategory::Long);
    return _int64_;
}


const char* RawValue::AsString() const
{
    assert(_raw == RawCategory::String);
    return _str.c_str();
}

bool RawValue::Value(bool& b) const
{
    if (_raw != RawCategory::Bool)
        return false;
    b = _bool;
    return true;
}

bool RawValue::Value(int& i) const
{
    if (_raw != RawCategory::Int)
        return false;
    i = _int;
    return true;
}

bool RawValue::Value(float& f) const
{
    if (_raw != RawCategory::Float)
        return false;
    f = _float;
    return true;
}

bool RawValue::Value(std::string& str) const
{
    if (_raw != RawCategory::String)
        return false;
    str = _str;
    return true;
}

//////////////////////////////////////////////////////////////////////////
// RefValue
IRawValue* RefValue::Original() const
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
