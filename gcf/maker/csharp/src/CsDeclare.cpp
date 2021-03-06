﻿#include "CsDeclare.h"
#include "CsUtility.h"
#include "utility/Utility.h"
#include <fstream>

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')

GCF_NAMESPACE_BEGIN

namespace cs
{
    Declare::Declare()
    {

    }

    Declare::~Declare()
    {

    }

    bool Declare::Init(const IScope* global, const std::string& path, const std::string& name)
    {
        _global = global;
        _path = path;
        _name = name;
        return true;
    }

    void Declare::DoExport()
    {
        int elements = 0;
        if (_global->VarSet())
            elements += _global->VarSet()->Size();
        if (_global->TypeSet())
            elements += _global->TypeSet()->Size();
        if (_global->NsSet())
            elements += _global->NsSet()->Size();

        if (elements == 0)
            return; // empty

        std::ofstream file(utility::ContactPath(_path, _name) + ".cs");
        if (!file.is_open())
            return;

        file << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl <<
            "using System;" << std::endl <<
            "using System.Collections.Generic;" << std::endl << std::endl;

        IVarSet* varSet = _global->VarSet();
        ITypeSet* tySet = _global->TypeSet();
        INsSet* nsSet = _global->NsSet();

        if (varSet && varSet->Size())
            ExportVars(file, varSet, nullptr, true);

        if (tySet)
        {
            for (int i = 0; i < tySet->Size(); ++i)
            {
                IType* ty = tySet->Get(i);
                if (ty->TypeCat() == TypeCategory::Enum)
                    ExportType(file, static_cast<const IEnumType*>(ty));
                else if (ty->TypeCat() == TypeCategory::Struct)
                    ExportType(file, static_cast<const IStructType*>(ty));
            }
        }

        if (nsSet)
        {
            for (int i = 0; i < nsSet->Size(); ++i)
            {
                ExportNs(file, nsSet->Get(i));
            }
        }
    }

    void Declare::OnType(const IType* type)
    {
        //_types.push_back(type);
    }

    void Declare::ExportType(std::ostream& stream, const IEnumType* type)
    {
        stream <<
            _TAB(0) << "public enum " << type->Name() << std::endl <<
            _TAB(0) << "{" << std::endl;

        IVarSet* varSet = type->Scope()->VarSet();
        for (int i = 0; i < varSet->Size(); ++i)
        {
            const IVariate* var = varSet->Get(i);
            stream << _TAB(1) << var->Name();
            std::string strVal = cs_util::Value(var, type->Scope());
            if (!strVal.empty())
                stream << " = " << strVal;
            stream << "," << std::endl;
        }

        stream << _TAB(0) << "}" << std::endl;
    }

    void Declare::ExportType(std::ostream& stream, const IStructType* type)
    {
        stream << _TAB(0);
        if (type->CfgCat() == CfgCategory::Tab)
            stream << "public partial class ";
        else
            stream << "public class ";
        stream << type->Name();

        if (type->Inherited())
            stream << _TAB(0) << " : " << cs_util::TypeName(type->Inherited(), type->Owner());
        stream << std::endl << _TAB(0) << "{" << std::endl;
        ++_tab;

        ITypeSet* tySet = type->OwnScope()->TypeSet();
        if (tySet && tySet->Size())
        {
            for (int i = 0; i < tySet->Size(); ++i)
            {
                IType* ty = tySet->Get(i);
                if (ty->TypeCat() == TypeCategory::Enum)
                    ExportType(stream, static_cast<const IEnumType*>(ty));
                else if (ty->TypeCat() == TypeCategory::Struct)
                    ExportType(stream, static_cast<const IStructType*>(ty));
            }
            stream << std::endl;
        }

        stream <<
            _TAB(0) << "public ";
        if (type->Inherited()) stream << "new ";
        stream << "const int HASH_CODE = " << utility::HashValue(type) << ";" <<
            std::endl;

        IVarSet* varSet = type->OwnScope()->VarSet();
        if (varSet && varSet->Size())
            ExportVars(stream, varSet, type->Scope(), false);

        --_tab;
        stream << _TAB(0) << "}" << std::endl;
    }

    void Declare::ExportNs(std::ostream& stream, const INamespace* ns)
    {
        IVarSet* varSet = ns->Scope()->VarSet();
        ITypeSet* tySet = ns->Scope()->TypeSet();
        INsSet* nsSet = ns->Scope()->NsSet();

        if (
            (varSet && varSet->Size() == 0) &&
            (tySet && tySet->Size() == 0) &&
            (nsSet && nsSet->Size() == 0)
            )
        {
            return;
        }

        stream <<
            _TAB(0) << "namespace " << ns->Name() << std::endl <<
            _TAB(0) << "{" << std::endl;
        ++_tab;

        if (varSet && varSet->Size())
            ExportVars(stream, varSet, ns->Scope(), true);

        if (tySet)
        {
            for (int i = 0; i < tySet->Size(); ++i)
            {
                IType* ty = tySet->Get(i);
                if (ty->TypeCat() == TypeCategory::Enum)
                    ExportType(stream, static_cast<const IEnumType*>(ty));
                else if (ty->TypeCat() == TypeCategory::Struct)
                    ExportType(stream, static_cast<const IStructType*>(ty));
            }
        }

        if (nsSet)
        {
            for (int i = 0; i < nsSet->Size(); ++i)
            {
                ExportNs(stream, nsSet->Get(i));
                stream << std::endl;
            }
        }

        --_tab;
        stream << _TAB(0) << "}" << std::endl;
    }

    void Declare::ExportVars(std::ostream& stream, const IVarSet* vars, const IScope* scope, bool isGlobal)
    {
        if (isGlobal)
        {
            stream <<
                _TAB(0) << "public static class GlobalVar" << std::endl <<
                _TAB(0) << "{" << std::endl;
            ++_tab;
        }

        // const var at beginning
        for (int i = 0; i < vars->Size(); ++i)
        {
            const IVariate* var = vars->Get(i);
            if (!var->IsConst() && !isGlobal) continue;

            stream << _TAB(0) << "public ";
            if (var->IsConst())
                stream << "const ";
            else if (isGlobal)
                stream << "static ";
            stream << cs_util::TypeName(var->Type()) << " " << var->Name();

            std::string val = cs_util::Value(var, scope);
            if (val.empty())
                val = cs_util::DefValue(var->Type());
            if (!val.empty())
                stream << " = " << val;
            stream << ";" << std::endl;
        }

        stream << std::endl;

        for (int i = 0; i < vars->Size(); ++i)
        {
            const IVariate* var = vars->Get(i);
            if (var->IsConst() || isGlobal) continue;

            stream <<
                _TAB(0) << "public " <<
                cs_util::TypeName(var->Type()) << " " << var->Name();

            std::string val = cs_util::Value(var, scope);
            if (val.empty())
                val = cs_util::DefValue(var->Type());
            if (!val.empty())
                stream << " = " << val;

            stream << ";" << std::endl;
        }

        if (isGlobal)
        {
            --_tab;
            stream << _TAB(0) << "}" << std::endl;
        }
    }
}

GCF_NAMESPACE_END
