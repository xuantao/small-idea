﻿#include "CsUtility.h"
#include "utility/Utility.h"

GCF_NAMESPACE_BEGIN

namespace cs_util
{
    std::string sToString(const IRawValue* val)
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
        {
            str = std::to_string(utility::AsValue<float>(val));
            if (!str.empty())
                str += "f";
        }
        else if (val->RawCat() == RawCategory::Double)
            str = std::to_string(utility::AsValue<double>(val));
        else if (val->RawCat() == RawCategory::String)
            val->ToValue(str);
        else
            assert(false);

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

        return std::string();
    }

    std::string sArrayName(const IArrayType* arTy, const std::string& tName)
    {
        IType* type = arTy->Prev();
        std::string prev;
        if (type->TypeCat() == TypeCategory::Array)
            prev = sArrayName((const IArrayType*)type, tName);
        else
            prev = tName;

        if (arTy->Length())
            return "FixedArray<" + prev + ", ArrayLength_" + std::to_string(arTy->Length()) + ">";
        else
            return "List<" + prev + ">";
    }

    std::string sDefRawValue(RawCategory raw)
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
            return "\"\"";
        else
            assert(false);
        return std::string();
    }

    std::string RawName(RawCategory raw)
    {
        if (raw == RawCategory::Bool)
            return "bool";
        else if (raw == RawCategory::Byte)
            return "byte";
        else if (raw == RawCategory::Int)
            return "int";
        else if (raw == RawCategory::Float)
            return "float";
        else if (raw == RawCategory::Double)
            return "double";
        else if (raw == RawCategory::Long)
            return "long";
        else if (raw == RawCategory::String)
            return "string";
        else
            assert(false);
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
            std::string str = cs_util::ToString(rawVal);
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
            const IArrayType* arTy = (const IArrayType*)type;
            return "new " + TypeName(type) + "()";
        }

        return "";
    }

    static void sReadVar(std::ostream& stream, IType* type, int tab, int dep, bool useNs)
    {
        if (type->TypeCat() == TypeCategory::Array)
        {
            stream << "delegate (";
            if (useNs)
                stream << "Serialize.";
            stream <<
                "IReader r" << dep << ", ref " << cs_util::TypeName(type) << " v" << dep << ", string n" << dep << ") {" << std::endl;

            stream << utility::Tab(tab) << "return ";
            if (useNs)
                stream << "Serialize.Utility.";
            stream << "DoRead(r" << dep << ", ref v" << dep << ", n" << dep << ", ";
            sReadVar(stream, static_cast<IArrayType*>(type)->Prev(), tab + 1, dep + 1, useNs);

            stream << ");" << std::endl <<
                utility::Tab(tab - 1) << "}";
        }
        else
        {
            if (useNs)
                stream << "Serialize.Utility.";
            stream << "Read";
        }
    }

    void ReadVar(std::ostream& stream,
        IType* type,
        const std::string& reader,
        const std::string& valName,
        const std::string& rdName,
        int tab,
        bool useNs)
    {
        if (type->TypeCat() == TypeCategory::Array)
        {
            stream << utility::Tab(tab);
            if (useNs)
                stream << "Serialize.Utility.";
            stream << "DoRead(" << reader << ", ref " << valName << ", \"" << rdName << "\", ";
            sReadVar(stream, static_cast<IArrayType*>(type)->Prev(), tab + 1, 1, useNs);
            stream << ");" << std::endl;
        }
        else if (type->TypeCat() == TypeCategory::Fucntion)
        {
            assert(false);
        }
        else
        {
            stream << utility::Tab(tab);
            if (useNs)
                stream << "Serialize.Utility.";
            stream << "Read(" << reader << ", ref " << valName << ", \"" << rdName << "\");" << std::endl;
        }
    }

    void ReadVar(std::ostream& stream, IVariate* var, int tab, bool useNs)
    {
        ReadVar(stream, var->Type(), "reader", "val." + var->Name(), var->Name(), tab, useNs);
    }

    void ReadVarDetail(std::ostream& stream, IType* type, int tab, bool useNs)
    {
        sReadVar(stream, type, tab, 1, useNs);
    }

    static void sWriteVar(std::ostream& stream, IType* type, int tab, int dep, bool useNs)
    {
        if (type->TypeCat() == TypeCategory::Array)
        {
            stream <<
                "(w" << dep << ", v" << dep << ", n" << dep << ") => {" << std::endl;

            stream << utility::Tab(tab) << "return ";
            if (useNs)
                stream << "Serialize.Utility.";
            stream << "DoWrite(w" << dep << ", v" << dep << ", n" << dep << ", ";

            sWriteVar(stream, static_cast<IArrayType*>(type)->Prev(), tab + 1, dep + 1, useNs);
            stream << ");" << std::endl <<
                utility::Tab(tab - 1) << "}";
        }
        else
        {
            if (useNs)
                stream << "Serialize.Utility.";
            stream << "Write";
        }
    }

    void WriteVar(
        std::ostream& stream,
        IType* type,
        const std::string& writer,
        const std::string& valName,
        const std::string& wrName,
        int tab,
        bool useNs
    )
    {
        if (type->TypeCat() == TypeCategory::Array)
        {
            stream << utility::Tab(tab);
            if (useNs)
                stream << "Serialize.Utility.";
            stream << "DoWrite(" << writer << ", " << valName << ", \"" << wrName << "\", ";
            sWriteVar(stream, static_cast<IArrayType*>(type)->Prev(), tab + 1, 1, useNs);
            stream << ");" << std::endl;
        }
        else if (type->TypeCat() == TypeCategory::Fucntion)
        {
            assert(false);
        }
        else
        {
            stream << utility::Tab(tab);
            if (useNs)
                stream << "Serialize.Utility.";
            stream << "Write(" << writer << ", " << valName << ", \"" << wrName << "\");" << std::endl;
        }
    }

    void WriteVar(std::ostream& stream, IVariate* var, int tab, bool useNs)
    {
        WriteVar(stream, var->Type(), "writer", "val." + var->Name(), var->Name(), tab, useNs);
    }

    void WriteVarDetail(std::ostream& stream, IType* type, int tab, bool useNs)
    {
        sWriteVar(stream, type, tab, 1, useNs);
    }
}

GCF_NAMESPACE_END
