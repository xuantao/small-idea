#include "CppUtil.h"
#include "utility/Utility.h"
#include <iostream>
#include <sstream>

CFG_NAMESPACE_BEGIN

namespace cpp_util
{
    static std::string sRefValue(const IScope* scope, const IType* type, const IRefValue* val)
    {
        const IVariate* var = val->Var();
        std::stringstream stream;

        if (type->TypeCat() == TypeCategory::Array)
            type = static_cast<const IArrayType*>(type)->Original();

        if (type != var->Type())
            stream << "(" << TypeName(scope, type) << ")";

        const std::vector<std::string> ralative = utility::Relative(var->Type(), scope);
        stream << utility::Contact(ralative, "::");
        if (!ralative.empty())
            stream << "::";
        stream << var->Name();
        return stream.str();
    }

    std::string sToString(const IRawValue* val)
    {
        if (val == nullptr)
            return utility::EMPTY_STR;

        std::string str;
        if (val->RawCat() == RawCategory::Bool)
            str = val->AsBool() ? "true" : "false";
        else if (val->RawCat() == RawCategory::Int)
            str = std::to_string(val->AsInt());
        else if (val->RawCat() == RawCategory::Float)
            str = std::to_string(val->AsFloat());
        else if (val->RawCat() == RawCategory::String)
            str = val->AsString();

        return str;
    }

    std::string ToString(const IValue* val)
    {
        if (val == nullptr)
            return std::string();

        if (val->ValueCat() == ValueCategory::Raw)
            return sToString(static_cast<const IRawValue*>(val));
        else if (val->ValueCat() == ValueCategory::Ref)
            return ToString(static_cast<const IRefValue*>(val)->Var()->Value());
        else
            ERROR_NOT_ALLOW;

        return std::string();
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

    std::string TypeName(const IType* type)
    {
        if (type == nullptr)
            return utility::EMPTY_STR;

        IScope* scope = type->Owner();
        while (scope && scope->Owner())
            scope = scope->Owner();

        return TypeName(scope, type);
    }

    std::string TypeName(const IScope* scope, const IType* type)
    {
        std::string name;
        if (type->TypeCat() == TypeCategory::Raw)
        {
            name = RawName(static_cast<const IRawType*>(type)->RawCat());
        }
        else if (type->TypeCat() == TypeCategory::Enum || type->TypeCat() == TypeCategory::Struct)
        {
            name = utility::Contact(utility::Relative(type, scope), "::");
        }
        else if (type->TypeCat() == TypeCategory::Array)
        {
            const IArrayType* arTy = static_cast<const IArrayType*>(type);
            name = TypeName(scope, arTy->Prev());
            if (arTy->Length() > 0)
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

    std::string Value(const IScope* scope, const IType* type, const IValue* val)
    {
        if (scope == nullptr || type == nullptr || val == nullptr)
            return std::string();

        std::string value;
        if (val->ValueCat() == ValueCategory::Raw)
            value = cpp_util::ToString(static_cast<const IRawValue*>(val));
        else if (val->ValueCat() == ValueCategory::Ref)
            value = sRefValue(scope, type, static_cast<const IRefValue*>(val));
        else
            ERROR_NOT_ALLOW;
        return value;
    }

    bool Convert(const IVariate* var, CppVarData& out)
    {
        if (var == nullptr)
            return false;

        out.type = TypeName(var->Owner(), var->Type());
        out.name = var->Name();
        if (var->Value())
            out.value = Value(var->Owner(), var->Type(), var->Value());
        else if (var->Type()->TypeCat() == TypeCategory::Raw)
            out.value = cpp_util::DefValue(static_cast<const IRawType*>(var->Type())->RawCat());

        if (
            !out.value.empty() &&
            var->Type()->TypeCat() == TypeCategory::Raw &&
            static_cast<const IRawType*>(var->Type())->RawCat() == RawCategory::String &&
            var->Value()->ValueCat() == ValueCategory::Raw
            )
        {
            out.value = "\"" + out.value + "\"";
        }

        out.var = var;
        return true;
    }
}

CFG_NAMESPACE_END
