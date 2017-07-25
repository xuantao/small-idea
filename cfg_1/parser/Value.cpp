﻿#include "Value.h"

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
const RawValue* RefValue::GetRaw() const
{
    const IValue* val = _var->Value();
    while (val)
    {
        if (val->Category() == ValueCategory::Raw)
            break;

        if (val->Category() == ValueCategory::Ref)
            val = static_cast<const RefValue*>(val)->_var->Value();
        else
            val = nullptr;
    }

    return static_cast<const RawValue*>(val);
}

CFG_NAMESPACE_END
