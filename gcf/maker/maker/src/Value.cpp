#include "Value.h"

CFG_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
// RawValue
RawValue::RawValue(bool value)
    : _raw(RawCategory::Bool)
    , _b(value)
{
}

RawValue::RawValue(int value)
    : _raw(RawCategory::Int)
    , _i(value)
{
}

RawValue::RawValue(float value)
    : _raw(RawCategory::Float)
    , _f(value)
{
}

RawValue::RawValue(const std::string& value)
    : _raw(RawCategory::String)
    , _s(value)
{
}

bool RawValue::AsBool() const
{
    assert(_raw == RawCategory::Bool);
    return _b;
}

int RawValue::AsInt() const
{
    assert(_raw == RawCategory::Int);
    return _i;
}

float RawValue::AsFloat() const
{
    assert(_raw == RawCategory::Float);
    return _f;
}

const char* RawValue::AsString() const
{
    assert(_raw == RawCategory::String);
    return _s.c_str();
}

bool RawValue::Value(bool& b) const
{
    if (_raw != RawCategory::Bool)
        return false;
    b = _b;
    return true;
}

bool RawValue::Value(int& i) const
{
    if (_raw != RawCategory::Int)
        return false;
    i = _i;
    return true;
}

bool RawValue::Value(float& f) const
{
    if (_raw != RawCategory::Float)
        return false;
    f = _f;
    return true;
}

bool RawValue::Value(std::string& str) const
{
    if (_raw != RawCategory::String)
        return false;
    str = _s;
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

CFG_NAMESPACE_END
