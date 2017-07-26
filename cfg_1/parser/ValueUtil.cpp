#include "ValueUtil.h"
#include "Utility.h"
#include "Value.h"
#include "Interface.h"
#include <cassert>
#include <iostream>

CFG_NAMESPACE_BEGIN

namespace value_util
{
    static const RawValue* GetRaw(const IValue* val)
    {
        if (val == nullptr)
            return nullptr;

        const RawValue* raw = nullptr;
        if (val->Category() == ValueCategory::Raw)
        {
            raw = static_cast<const RawValue*>(val);
        }
        else if (val->Category() == ValueCategory::Ref)
        {
            const RefValue* ref = static_cast<const RefValue*>(val);
            raw = static_cast<const RawValue*>(ref->Original());
        }
        return raw;
    }

    bool IsRaw(const IValue* val, RawCategory raw, bool ref/* = true*/)
    {
        if (val == nullptr)
            return false;

        if (ref && val->Category() == ValueCategory::Ref)
            return IsRaw(static_cast<const RefValue*>(val)->Original(), raw);
        else if (val->Category() == ValueCategory::Raw)
            return static_cast<const RawValue*>(val)->Raw() == raw;
        else
            assert(false);
        return false;
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
            val = new RawValue("");
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
            if (util::Convert(value, b))
                val = new RawValue(b);
        }
        else if (raw == RawCategory::Int)
        {
            int i = 0;
            if (util::Convert(value, i))
                val = new RawValue(i);
        }
        else if (raw == RawCategory::Float)
        {
            float f = 0.0f;
            if (util::Convert(value, f))
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

CFG_NAMESPACE_END
