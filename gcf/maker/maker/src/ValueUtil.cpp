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
            str = utility::AsValue<bool>(val) ? "true" : "false";
        else if (val->RawCat() == RawCategory::Byte)
            str = std::to_string(utility::AsValue<int8_t>(val));
        else if (val->RawCat() == RawCategory::Int)
            str = std::to_string(utility::AsValue<int32_t>(val));
        else if (val->RawCat() == RawCategory::Long)
            str = std::to_string(utility::AsValue<int64_t>(val));
        else if (val->RawCat() == RawCategory::Float)
            str = std::to_string(utility::AsValue<float>(val));
        else if (val->RawCat() == RawCategory::Double)
            str = std::to_string(utility::AsValue<double>(val));
        else if (val->RawCat() == RawCategory::String)
            str = utility::AsValue<std::string>(val);
        else
            assert(false);

        return str;
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
        RawValue* val = nullptr;
        if (raw == RawCategory::Bool)
            val = new RawValue(detail::ValueImpl(false));
        else if (raw == RawCategory::Byte)
            val = new RawValue(detail::ValueImpl((int8_t)0));
        else if (raw == RawCategory::Int)
            val = new RawValue(detail::ValueImpl((int32_t)0));
        else if (raw == RawCategory::Float)
            val = new RawValue(detail::ValueImpl((float)0.0f));
        else if (raw == RawCategory::Float)
            val = new RawValue(detail::ValueImpl((double)0.0));
        else if (raw == RawCategory::Long)
            val = new RawValue(detail::ValueImpl((int64_t)0));
        else if (raw == RawCategory::String)
            val = new RawValue(detail::ValueImpl(std::string()));
        else
            assert(false);

        return val;
    }

    IValue* Create(RawCategory raw, const std::string& value)
    {
        IValue* val = nullptr;
        if (raw == RawCategory::Bool)
        {
            bool v = false;
            if (utility::Convert(value, v))
                val = new RawValue(detail::ValueImpl(v));
        }
        else if (raw == RawCategory::Byte)
        {
            int8_t v = 0;
            if (utility::Convert(value, v))
                val = new RawValue(detail::ValueImpl(v));
        }
        else if (raw == RawCategory::Int)
        {
            int32_t v = 0;
            if (utility::Convert(value, v))
                val = new RawValue(detail::ValueImpl(v));
        }
        else if (raw == RawCategory::Long)
        {
            int64_t v = 0;
            if (utility::Convert(value, v))
                val = new RawValue(detail::ValueImpl(v));
        }
        else if (raw == RawCategory::Float)
        {
            float f = 0.0f;
            if (utility::Convert(value, f))
                val = new RawValue(f);
        }
        else if (raw == RawCategory::Double)
        {
            double v = 0;
            if (utility::Convert(value, v))
                val = new RawValue(detail::ValueImpl(v));
        }
        else if (raw == RawCategory::String)
        {
            val = new RawValue(value);
        }
        return val;
    }

    IValue* Create(RawCategory raw, IVariate* var)
    {
        if (var == nullptr)
            return nullptr;

        return new RefValue(var, detail::ValueImpl(raw, var->Value()));
    }
}

GCF_NAMESPACE_END
