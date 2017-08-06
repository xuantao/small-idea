#include "CppUtil.h"
#include "Interface.h"
#include "Utility.h"
#include "ValueUtil.h"
#include <iostream>
#include <sstream>

CFG_NAMESPACE_BEGIN

namespace cpp_util
{
    static std::string sRefValue(const IType* scope, const IType* type, const IRefValue* val)
    {
        const IVariate* var = val->Var();
        std::stringstream stream;

        if (type->Category() == TypeCategory::Array)
            type = static_cast<const IArrayType*>(type)->Original();

        if (type != var->Type())
            stream << "(" << TypeName(scope, type) << ")";

        const std::vector<std::string> ralative = utility::Relative(scope, var->Type());
        stream << utility::Contact(ralative, "::");
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

    std::string TypeName(const IType* scope, const IType* ty)
    {
        std::string name;
        if (ty->Category() == TypeCategory::Raw)
        {
            name = RawName(static_cast<const IRawType*>(ty)->Raw());
        }
        else if (ty->Category() == TypeCategory::Enum || ty->Category() == TypeCategory::Struct)
        {
            name = utility::Contact(utility::Relative(scope, ty), "::");
        }
        else if (ty->Category() == TypeCategory::Array)
        {
            const IArrayType* arTy = static_cast<const IArrayType*>(ty);
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

    std::string Value(const IType* scope, const IType* type, const IValue* val)
    {
        if (scope == nullptr || type == nullptr || val == nullptr)
            return std::string();

        std::string value;
        if (val->Category() == ValueCategory::Raw)
            value = value_util::ToString(static_cast<const IRawValue*>(val));
        else if (val->Category() == ValueCategory::Ref)
            value = sRefValue(scope, type, static_cast<const IRefValue*>(val));
        else
            ERROR_NOT_ALLOW;
        return value;
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
            out.value = value_util::DefValue(static_cast<const IRawType*>(var->Type())->Raw());

        if (var->Type()->Category() == TypeCategory::Raw &&
            !out.value.empty() &&
            static_cast<const IRawType*>(var->Type())->Raw() == RawCategory::String)
        {
            out.value = "\"" + out.value + "\"";
        }

        out.var = var;
        return true;
    }
}

CFG_NAMESPACE_END
