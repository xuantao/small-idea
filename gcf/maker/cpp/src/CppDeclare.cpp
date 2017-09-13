#include "CppDeclare.h"
#include "CppUtil.h"
#include "utility/Utility.h"
#include <fstream>
#include <iostream>

GCF_NAMESPACE_BEGIN

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')

namespace cpp
{
    Declare::Declare(const IScope* global, const std::string& path, const std::string& fileName)
        : _global(global)
        , _path(path)
        , _name(fileName)
    {
        std::ofstream* file = new std::ofstream(utility::ContactPath(path, fileName) + ".h");

        if (file->is_open()) _header = file;
        else _header = &std::cout;

        *_header << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl <<
            "#pragma once" << std::endl << std::endl <<
            "#include <vector>" << std::endl <<
            "#include <string>" << std::endl <<
            "#include <array>" << std::endl << std::endl;
    }

    Declare::~Declare()
    {
        *_header << std::endl;
        if (_header != &std::cout)
            delete _header;
        _header = nullptr;
    }

    void Declare::DoExport()
    {
        if (!_vars.empty())
            CppVars();

        _tab = 0;
        _lastIsVar = false;
    }

    void Declare::OnNsBegin(const std::string& name)
    {
        *_header << _TAB(0) << "namespace " << name << std::endl <<
            _TAB(0) << "{" << std::endl;

        ++_tab;
    }

    void Declare::OnNsEnd()
    {
        --_tab;
        *_header << _TAB(0) << "}" << std::endl << std::endl;
    }

    void Declare::OnInclude(const std::string& file)
    {
    }

    void Declare::OnVariate(const IVariate* var)
    {
        CppVarData data;
        if (!cpp_util::Convert(var, data))
            return;

        *_header << _TAB(0) << "extern ";
        if (var->IsConst())
            *_header << "const ";
        *_header << data.type << " " << data.name << ";" << std::endl;

        _lastIsVar = true;

        _vars.push_back(var);
    }

    void Declare::OnType(const IType* type)
    {
        if (_lastIsVar)
        {
            *_header << std::endl;
            _lastIsVar = false;
        }

        if (type->TypeCat() == TypeCategory::Enum)
        {
            const IEnumType* eType = static_cast<const IEnumType*>(type);
            Decl(eType);
        }
        else if (type->TypeCat() == TypeCategory::Struct)
        {
            const IStructType* sType = static_cast<const IStructType*>(type);
            Decl(sType);
        }
        else
        {
            std::cerr << "does not support export type:" << type->Name() << std::endl;
            ERROR_NOT_ALLOW;
        }

        *_header << std::endl;
    }

    bool Declare::Decl(const IEnumType* ty)
    {
        *_header << _TAB(0) << "enum class " << ty->Name() << std::endl <<
            _TAB(0) << "{" << std::endl;

        const IVarSet* vars = ty->Scope()->VarSet();
        for (int i = 0; i < vars->Size(); ++i)
        {
            CppVarData data;
            cpp_util::Convert(vars->Get(i), data);

            *_header << _TAB(1) << data.name;
            if (!data.value.empty())
                *_header << " = " << data.value << ",";
            *_header << std::endl;
        }

        *_header << _TAB(0) << "};" << std::endl;
        return true;
    }

    bool Declare::Decl(const IStructType* sType)
    {
        const IVarSet* varSet = sType->OwnScope()->VarSet();
        const ITypeSet* tySet = sType->OwnScope()->TypeSet();

        /* struct define */
        *_header << _TAB(0) << "struct " << sType->Name();
        if (sType->Inherited())
            *_header << _TAB(0) << " : public " << cpp_util::TypeName(sType->Owner(), sType->Inherited());
        *_header << std::endl << _TAB(0) << "{" << std::endl;

        // inner struct
        for (int i = 0; i < tySet->Size(); ++i)
        {
            ++_tab;
            Decl((IStructType*)tySet->Get(i));
            --_tab;
            *_header << std::endl;
        }

        *_header << _TAB(1) << "static const uint32_t HASH_CODE = " << utility::HashValue(sType) << ";" << std::endl;

        std::vector<CppVarData> vars;
        for (int i = 0; i < varSet->Size(); ++i)
        {
            CppVarData data;
            const IVariate* var = varSet->Get(i);
            if (cpp_util::Convert(var, data))
                vars.push_back(data);
            else
                std::cerr << "convert var failed" << std::endl;
        }
        /* const values */
        for (size_t i = 0; i < vars.size(); i++)
        {
            const CppVarData& data = vars[i];
            if (!data.var->IsConst())
                continue;

            *_header << _TAB(1) << "static const " << data.type << " " << data.name;

            IType* ty = data.var->Type();
            if (
                (ty->TypeCat() == TypeCategory::Enum) ||
                (ty->TypeCat() == TypeCategory::Raw && ((IRawType*)ty)->RawCat() == RawCategory::Int)
                )
            {
                *_header << " = " << data.value << ";" << std::endl;
            }
            else
            {
                *_header << ";" << std::endl;
                _vars.push_back(data.var);
            }
        }

        *_header << std::endl;

        /* member data */
        for (size_t i = 0; i < vars.size(); i++)
        {
            const CppVarData& data = vars[i];
            if (data.var->IsConst())
                continue;

            const IType* type = data.var->Type();
            *_header << _TAB(1) << "" << data.type << " " << data.name;
            if (
                (type->TypeCat() == TypeCategory::Raw && ((const IRawType*)type)->RawCat() != RawCategory::String) ||
                !data.value.empty()
                )
            {
                *_header << " = ";
                if (!data.value.empty())
                    *_header << data.value;
                else if (type->TypeCat() == TypeCategory::Raw)
                    *_header << cpp_util::DefValue(((const IRawType*)type)->RawCat());
            }

            *_header << ";";
            if (!data.var->Desc().empty())
                *_header << " //< " << data.var->Desc();
            *_header << std::endl;
        }

        *_header << _TAB(0) << "};" << std::endl;
        return true;
    }

    bool Declare::CppVars()
    {
        std::ofstream file(utility::ContactPath(_path, _name) + ".cpp");
        if (!file.is_open())
            return false;

        file << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl <<
            "#include \"" << _name << ".h\"" << std::endl << std::endl;

        for (auto& var : _vars)
        {
            CppVarData data;
            if (!cpp_util::Convert(var, data))
                continue;

            if (var->Owner()->BindType() == nullptr) file << "static ";
            if (var->IsConst()) file << "const ";

            file << data.type << " ";

            std::string path = utility::Contact(utility::Absolute(var->Owner()), "::");
            if (!path.empty()) file << path << "::";

            file << var->Name();

            const IType* type = var->Type();
            if (
                (type->TypeCat() == TypeCategory::Raw && ((const IRawType*)type)->RawCat() != RawCategory::String) ||
                !data.value.empty()
                )
            {
                file << " = ";
                if (!data.value.empty())
                    file << data.value;
                else if (type->TypeCat() == TypeCategory::Raw)
                    file << cpp_util::DefValue(((const IRawType*)type)->RawCat());
            }

            file << ";" << std::endl;
        }

        return true;
    }
}

GCF_NAMESPACE_END
