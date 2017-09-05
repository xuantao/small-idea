#include "CsUtility.h"
#include "Utility.h"
#include "ValueUtil.h"

CFG_NAMESPACE_BEGIN

namespace cs_util
{
    std::string sArrayName(const IArrayType* arTy, const std::string& tName)
    {
        IType* type = arTy->Prev();
        std::string prev;
        if (type->TypeCat() == TypeCategory::Array)
            prev = sArrayName((const IArrayType*)type, tName);
        else
            prev = tName;

        if (arTy->Length())
            return prev + "[]";
        else
            return "List<" + prev + ">";
    }

    std::string sDefRawValue(RawCategory raw)
    {
        if (raw == RawCategory::Bool)
            return "false";
        else if (raw == RawCategory::Int)
            return "0";
        else if (raw == RawCategory::Float)
            return "0.0f";
        else
            return "\"\"";
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
            return "string";
        else
            return std::string();
    }

    std::string TypeName(const IType* type)
    {
        return TypeName(type, nullptr);
    }

    std::string TypeName(const IType* type, const IScope* scope)
    {
        std::string name;
        if (type->TypeCat() == TypeCategory::Array)
        {
            const IArrayType* arTy = static_cast<const IArrayType*>(type);
            return sArrayName(arTy, TypeName(arTy->Original(), scope));
        }
        else if (type->TypeCat() == TypeCategory::Raw)
        {
            const IRawType* rawTy = static_cast<const IRawType*>(type);
            return RawName(rawTy->RawCat());
        }
        else
        {
            return utility::Contact(utility::Relative(type, scope), ".");
        }
    }

    std::string Value(const IVariate* var)
    {
        return Value(var, nullptr);
    }

    std::string Value(const IVariate* var, const IScope* scope)
    {
        const IValue* val = var->Value();
        if (val == nullptr)
            return utility::EMPTY_STR;

        if (val->ValueCat() == ValueCategory::Raw)
        {
            const IRawValue* rawVal = static_cast<const IRawValue*>(val);
            std::string str = value_util::ToString(rawVal);
            if (rawVal->RawCat() == RawCategory::String)
                str = "\"" + str + "\"";

            return str;
        }

        const IRefValue* refVal = static_cast<const IRefValue*>(val);
        const IScope* belong = refVal->Var()->Owner();

        std::string path = utility::Contact(utility::Relative(belong, scope), ".");
        if (belong->BindNs())
        {
            if (path.empty())
                path = "GlobalVar";
            else
                path = path + ".GlobalVar";
        }

        std::string strVal;
        if (path.empty())
            strVal = refVal->Var()->Name();
        else
            strVal = path + "." + refVal->Var()->Name();
        if (var->Type() != refVal->Var()->Type())
            strVal = "(" + cs_util::TypeName(var->Type()) + ")" + strVal;
        return strVal;
    }

    std::string DefValue(const IType* type)
    {
        if (type->TypeCat() == TypeCategory::Raw)
            return sDefRawValue(((const IRawType*)type)->RawCat());

        if (type->TypeCat() == TypeCategory::Enum)
        {
            const IEnumType* eTy = (const IEnumType*)type;
            if (eTy->Scope()->VarSet()->Size())
                return Value(eTy->Scope()->VarSet()->Get(0));
            return "";
        }
        else if (type->TypeCat() == TypeCategory::Struct)
        {
            return "new " + TypeName(type) + "()";
        }
        else if (type->TypeCat() == TypeCategory::Array)
        {
            const IArrayType* eTy = (const IArrayType*)type;
        }


        return "";
    }
}

CFG_NAMESPACE_END
