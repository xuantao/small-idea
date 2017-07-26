#include "CppUtil.h"
#include "Type.h"
#include "Value.h"
#include "Utility.h"
#include <iostream>
#include <sstream>

CFG_NAMESPACE_BEGIN

namespace cpp_util
{
    static std::string sRawValue(const RawValue* val)
    {
        if (val == nullptr)
            return util::EMPTY_STR;

        std::string str;
        if (val->Raw() == RawCategory::Bool)
        {
            bool b = false;
            val->Value(b);
            str = b ? "true" : "false";
        }
        else if (val->Raw() == RawCategory::Int)
        {
            int i = 0;
            val->Value(i);
            str = std::to_string(i);
        }
        else if (val->Raw() == RawCategory::Float)
        {
            float f = 0.0f;
            val->Value(f);
            str = std::to_string(f);
        }
        else if (val->Raw() == RawCategory::String)
        {
            val->Value(str);
        }

        return str;
    }

    static std::string sRefValue(const IType* scope, const IType* type, const RefValue* val)
    {
        const IVariate* var = val->Var();
        std::stringstream stream;
        if (type != var->Type())
            stream << "(" << TypeName(scope, type) << ")";

        const std::vector<std::string> ralative = util::Relative(scope, var->Type());
        stream << util::Contact(ralative, "::");
        if (!ralative.empty())
            stream << "::";
        stream << var->Name();
        return stream.str();
    }

    std::string RawName(RawCategory raw)
    {
        if (raw == RawCategory::Bool)
            return "bool";
        else if (raw == RawCategory::Int)
            return "int";
        else if (raw == RawCategory::Float)
            return "float";
        else if (raw == RawCategory::String)
            return "std::string";
        else
            return std::string();
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

    std::string TypeName(const IType* scope, const IType* ty)
    {
        std::string name;
        if (ty->Category() == TypeCategory::Raw)
        {
            name = RawName(static_cast<const RawType*>(ty)->Raw());
        }
        else if (ty->Category() == TypeCategory::Enum || ty->Category() == TypeCategory::Struct)
        {
            name = util::Contact(util::Relative(scope, ty), "::");
        }
        else if (ty->Category() == TypeCategory::Array)
        {
            const ArrayType* arTy = static_cast<const ArrayType*>(ty);
            name = TypeName(scope, arTy->Prev());
            if (arTy->Length() >= 0)
            {
                name = "std::array<" + name + ", " + std::to_string(arTy->Length()) + ">";
            }
            else
            {
                if (name.back() == '>')
                    name = "std::vector<" + name + " >";
                else
                    name = "std::vector<" + name + ">";
            }
        }
        else
        {
            ERROR_NOT_ALLOW;
        }
        return name;
    }

    std::string Value(const IType* scope, const IType* type, const IValue* val)
    {
        if (scope == nullptr || type == nullptr || val == nullptr)
            return std::string();

        std::string value;
        if (val->Category() == ValueCategory::Raw)
            value = sRawValue(static_cast<const RawValue*>(val));
        else if (val->Category() == ValueCategory::Ref)
            value = sRefValue(scope, type, static_cast<const RefValue*>(val));
        else
            ERROR_NOT_ALLOW;
        return value;
    }

    std::string OrignalValue(const IValue* val)
    {
        if (val == nullptr)
            return std::string();

        if (val->Category() == ValueCategory::Raw)
            return sRawValue(static_cast<const RawValue*>(val));
        else if (val->Category() == ValueCategory::Ref)
            return OrignalValue(static_cast<const RefValue*>(val)->Var()->Value());
        else
            ERROR_NOT_ALLOW;

        return std::string();
    }

    bool Convert(const IVariate* var, CppVarData& out)
    {
        if (var == nullptr)
            return false;

        out.type = TypeName(var->Belong(), var->Type());
        out.name = var->Name();
        if (var->Value())
            out.value = Value(var->Belong(), var->Type(), var->Value());
        else if (var->Type()->Category() == TypeCategory::Raw)
            out.value = DefValue(static_cast<const RawType*>(var->Type())->Raw());
        out.var = var;
        return true;
    }
}

CFG_NAMESPACE_END
