#include "ValueUtil.h"
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
        {
            bool b = false;
            val->Value(b);
            str = b ? "true" : "false";
        }
        else if (val->RawCat() == RawCategory::Int)
        {
            int i = 0;
            val->Value(i);
            str = std::to_string(i);
        }
        else if (val->RawCat() == RawCategory::Float)
        {
            float f = 0.0f;
            val->Value(f);
            str = std::to_string(f);
        }
        else if (val->RawCat() == RawCategory::String)
        {
            val->Value(str);
        }

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

    bool Value(const IValue* val, bool& b)
    {
        const RawValue* raw = GetRaw(val);
        if (raw)
            return raw->Value(b);
        return true;
    }

    bool Value(const IValue* val, int& i)
    {
        const RawValue* raw = GetRaw(val);
        if (raw)
            return raw->Value(i);
        return true;
    }

    bool Value(const IValue* val, float &f)
    {
        const RawValue* raw = GetRaw(val);
        if (raw)
            return raw->Value(f);
        return true;
    }

    bool Value(const IValue* val, std::string& str)
    {
        const RawValue* raw = GetRaw(val);
        if (raw)
            return raw->Value(str);
        return true;
    }

    std::string ToString(const IValue* val)
    {
        if (val == nullptr)
            return std::string();

        if (val->ValueCat() == ValueCategory::Raw)
            return sRawValue(static_cast<const RawValue*>(val));
        else if (val->ValueCat() == ValueCategory::Ref)
            return ToString(static_cast<const RefValue*>(val)->Var()->Value());
        else
            ERROR_NOT_ALLOW;

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
        else if (raw == RawCategory::Int)
            return "0";
        else if (raw == RawCategory::Float)
            return "0.0f";
        else if (raw == RawCategory::String)
            return std::string();
        else
            return std::string();
    }

    IValue* Create(RawCategory raw)
    {
        IValue* val = nullptr;
        if (raw == RawCategory::Bool)
            val = new RawValue(false);
        else if (raw == RawCategory::Int)
            val = new RawValue(0);
        else if (raw == RawCategory::Float)
            val = new RawValue(0.0f);
        else if (raw == RawCategory::String)
            val = new RawValue(std::string());
        else
            ERROR_NOT_ALLOW;
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
