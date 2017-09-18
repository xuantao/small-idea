﻿#include "ValueUtil.h"
#include "Value.h"
#include "utility/Utility.h"
#include <cassert>
#include <iostream>

GCF_NAMESPACE_BEGIN

namespace value_util
{
    static const RawValue* GetRaw(const IValue* val)
    {
        if (val == nullptr)
            return nullptr;

        const RawValue* raw = nullptr;
        if (val->ValueCat() == ValueCategory::Raw)
        {
            raw = static_cast<const RawValue*>(val);
        }
        else if (val->ValueCat() == ValueCategory::Ref)
        {
            const RefValue* ref = static_cast<const RefValue*>(val);
            raw = static_cast<const RawValue*>(ref->Original());
        }
        return raw;
    }

    static std::string sRawValue(const RawValue* val)
    {
        if (val == nullptr)
            return utility::EMPTY_STR;

        std::string str;
        if (val->RawCat() == RawCategory::Bool)
            str = val->AsBool() ? "true" : "false";
        else if (val->RawCat() == RawCategory::Byte)
            str = std::to_string(val->AsByte());
        else if (val->RawCat() == RawCategory::Int)
            str = std::to_string(val->AsInt());
        else if (val->RawCat() == RawCategory::Long)
            str = std::to_string(val->AsLong());
        else if (val->RawCat() == RawCategory::Float)
            str = std::to_string(val->AsFloat());
        else if (val->RawCat() == RawCategory::Double)
            str = std::to_string(val->AsDouble());
        else if (val->RawCat() == RawCategory::String)
            str = val->AsString();
        else
            assert(false);

        return str;
    }

    bool AsRaw(RawCategory raw, const IValue* val)
    {
        if (val == nullptr)
            return false;

        if (val->ValueCat() == ValueCategory::Raw)
            return static_cast<const RawValue*>(val)->RawCat() == raw;

        assert(val->ValueCat() == ValueCategory::Ref);

        const IRawValue* original = static_cast<const IRefValue*>(val)->Original();
        return original->RawCat() == raw;
    }

    //bool Value(const IValue* val, bool& b)
    //{
    //    const RawValue* raw = GetRaw(val);
    //    if (raw)
    //        return raw->Value(b);
    //    return true;
    //}

    //bool Value(const IValue* val, int& i)
    //{
    //    const RawValue* raw = GetRaw(val);
    //    if (raw)
    //        return raw->Value(i);
    //    return true;
    //}

    //bool Value(const IValue* val, float &f)
    //{
    //    const RawValue* raw = GetRaw(val);
    //    if (raw)
    //        return raw->Value(f);
    //    return true;
    //}

    //bool Value(const IValue* val, std::string& str)
    //{
    //    const RawValue* raw = GetRaw(val);
    //    if (raw)
    //        return raw->Value(str);
    //    return true;
    //}

    std::string ToString(const IValue* val)
    {
        if (val == nullptr)
            return std::string();

        if (val->ValueCat() == ValueCategory::Raw)
            return sRawValue(static_cast<const RawValue*>(val));
        else if (val->ValueCat() == ValueCategory::Ref)
            return ToString(static_cast<const RefValue*>(val)->Var()->Value());
        else
            assert(false);

        return std::string();
    }

    std::string ToString(const IRawValue* val)
    {
        if (val == nullptr)
            return std::string();
        return sRawValue(static_cast<const RawValue*>(val));
    }

    std::string DefValue(RawCategory raw)
    {
        if (raw == RawCategory::Bool)
            return "false";
        else if (raw == RawCategory::Byte)
            return "0";
        else if (raw == RawCategory::Int)
            return "0";
        else if (raw == RawCategory::Float)
            return "0.0f";
        else if (raw == RawCategory::Double)
            return "0.0";
        else if (raw == RawCategory::Long)
            return "0";
        else if (raw == RawCategory::String)
            return std::string();
        else
            assert(false);
        return std::string();
    }

    IValue* Create(RawCategory raw)
    {
        IValue* val = nullptr;
        if (raw == RawCategory::Bool)
            val = new RawValue(false);
        else if (raw == RawCategory::Byte)
            val = new RawValue((int8_t)0);
        else if (raw == RawCategory::Int)
            val = new RawValue((int32_t)0);
        else if (raw == RawCategory::Float)
            val = new RawValue((float)0.0f);
        else if (raw == RawCategory::Float)
            val = new RawValue((double)0.0);
        else if (raw == RawCategory::Long)
            val = new RawValue((int64_t)0);
        else if (raw == RawCategory::String)
            val = new RawValue(std::string());
        else
            assert(false);
        return val;
    }

    IValue* Create(RawCategory raw, const std::string& value)
    {
        IValue* val = nullptr;
        if (raw == RawCategory::Bool)
        {
            bool b = false;
            if (utility::Convert(value, b))
                val = new RawValue(b);
        }
        else if (raw == RawCategory::Int)
        {
            int i = 0;
            if (utility::Convert(value, i))
                val = new RawValue(i);
        }
        else if (raw == RawCategory::Float)
        {
            float f = 0.0f;
            if (utility::Convert(value, f))
                val = new RawValue(f);
        }
        else if (raw == RawCategory::String)
        {
            val = new RawValue(value);
        }
        return val;
    }

    IValue* Create(IVariate* var)
    {
        if (var == nullptr)
            return nullptr;
        return new RefValue(var);
    }
}

GCF_NAMESPACE_END
