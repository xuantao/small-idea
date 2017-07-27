#include "CppExporter.h"
#include "Utility.h"
#include "CppStruct.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <set>

CFG_NAMESPACE_BEGIN

#define _DEF_STREAM_ &std::cerr
#define _OUTS_ (*_stream)
#define _TAB_ util::Tab(*_stream, _tab)

CppExporter::CppExporter()
    : _stream(_DEF_STREAM_)
    , _global(nullptr)
    , _tab(0)
{
}

CppExporter::~CppExporter()
{
}

void CppExporter::OnBegin(const IScopeType* global, const std::string& file)
{
    std::ofstream* header = new std::ofstream();
    header->open(file + ".h");
    if (header->is_open())
        _stream = header;
    else
        delete header;

    _OUTS_ << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl;
    _OUTS_ << "#pragma once" << std::endl << std::endl;
}

void CppExporter::OnEnd()
{
    DeclaerHeader();

    if (_stream != _DEF_STREAM_)
    {
        static_cast<std::ofstream*>(_stream)->close();
        delete _stream;
        _stream = _DEF_STREAM_;
    }

    std::ofstream cpp;
    cpp.open(_file + ".cpp");
    if (cpp.is_open())
        _stream = &cpp;

    ImplationCpp();

    _structs.clear();
    _enums.clear();
    _file = "";
    _global = nullptr;
    _tab = 0;
    _stream = _DEF_STREAM_;
}

void CppExporter::OnFileBegin(const std::string& file)
{
}

void CppExporter::OnFileEnd()
{
}

void CppExporter::OnNamespaceBegin(const std::string& name)
{
}

void CppExporter::OnNameSapceEnd()
{
}

void CppExporter::OnInclude(const std::string& file)
{
    std::string name = util::TrimSuffix(file, '.') + ".h";
    if (name.length() <= 2)
    {
        ERROR_NOT_ALLOW;
        return;
    }

    _OUTS_ << "#include \"" << name << "\"";
    if (name != file)
        _OUTS_ << " // \"" << file << "\"";
    _OUTS_ << std::endl;
}

void CppExporter::OnVariate(const IVariate* var)
{
    CppVarData data;
    if (!cpp_util::Convert(var, data))
        return;

    if (var->IsConst())
        _OUTS_ << "const ";
    _OUTS_ << data.type << " " << data.name;
    if (!data.value.empty())
        _OUTS_ << " = " << data.value;
    _OUTS_ << ";" << std::endl;
}

void CppExporter::OnType(const IType* type)
{
    if (type->Category() == TypeCategory::Enum)
    {
        const IEnumType* eType = static_cast<const IEnumType*>(type);
        Declare(eType);
        _enums.push_back(eType);
    }
    else if (type->Category() == TypeCategory::Struct)
    {
        const IStructType* sType = static_cast<const IStructType*>(type);
        Declare(sType);
        _structs.push_back(sType);
    }
    else
    {
        std::cerr << "does not support export type:" << type->Name() << std::endl;
        ERROR_NOT_ALLOW;
    }

    _OUTS_ << std::endl;
}

bool CppExporter::Declare(const IEnumType* ty)
{
    _TAB_ << "enum " << ty->Name() << std::endl;
    _TAB_ << "{" << std::endl;
    ++_tab;

    const IVarSet* vars = ty->VarSet();
    for (int i = 0; i < vars->Size(); ++i)
    {
        CppVarData data;
        cpp_util::Convert(vars->Get(i), data);

        _TAB_ << data.name;
        if (!data.value.empty())
            _OUTS_ << " = " << data.value;
        _OUTS_ << std::endl;
    }

    --_tab;
    _TAB_ << "};" << std::endl;
    return true;
}

bool CppExporter::Declare(const IStructType* sType)
{
    const IVarSet* varSet = sType->OwnVars();
    std::vector<CppVarData> vars;
    bool constructor = false;
    int nConst = 0;
    for (int i = 0; i < varSet->Size(); ++i)
    {
        CppVarData data;
        const IVariate* var = varSet->Get(i);
        if (cpp_util::Convert(var, data))
        {
            vars.push_back(data);
            if (!constructor && !var->IsConst() && !data.value.empty())
                constructor = true;
        }
        else
        {
            std::cerr << "convert var failed" << std::endl;
        }
    }

    /* struct define */
    _TAB_ << "struct " << sType->Name();
    if (sType->Inherited())
        _OUTS_ << " : public " << cpp_util::TypeName(sType, sType->Inherited());
    _OUTS_ << std::endl;

    _TAB_ << "{" << std::endl;
    ++_tab;

    /* const values */
    for (size_t i = 0; i < vars.size(); i++)
    {
        const CppVarData& data = vars[i];
        if (!data.var->IsConst())
            continue;
        ++nConst;
        _TAB_ << "static const " << data.type << " " << data.name << " = " << data.value << ";" << std::endl;
    }

    if (nConst)
        _OUTS_ << std::endl;

    /* constructor */
    if (constructor)
    {
        _TAB_ << sType->Name() << "()" << std::endl;
        ++_tab;

        bool first = true;
        for (size_t i = 0; i < vars.size(); i++)
        {
            const CppVarData& data = vars[i];
            if (data.var->IsConst())
                continue;

            const IType* type = data.var->Type();
            if (type->Category() != TypeCategory::Raw && data.value.empty())
                continue;

            _TAB_;
            if (first)
                _OUTS_ << ": ";
            else
                _OUTS_ << ", ";

            _OUTS_ << data.name << "(";

            if (data.value.empty())
            {
                assert(type->Category() == TypeCategory::Raw);
                _OUTS_ << cpp_util::DefValue(static_cast<const IRawType*>(type)->Raw());
            }
            else
            {
                _OUTS_ << data.value;
            }

            _OUTS_ << ")" << std::endl;
            first = false;
        }

        --_tab;
        _TAB_ << "{ }" << std::endl << std::endl;
    }

    /* member data */
    for (size_t i = 0; i < vars.size(); i++)
    {
        const CppVarData& data = vars[i];
        if (data.var->IsConst())
            continue;

        _TAB_ << "" << data.type << " " << data.name << ";" << std::endl;
    }

    --_tab;
    _TAB_ << "};" << std::endl;

    return true;
}

//
//bool CppExporter::Enum2String(const IEnumType* eType)
//{
//    std::vector<std::string> path = util::Absolute(eType);
//    std::string base = util::Contact(path, "_");
//    std::string varName = "s_" + base + "_val";
//    std::string strName = "s_" + base + "_str";
//
//    _TAB_ << "const char* ToString(" << util::Contact(path, "::") << " value)" << std::endl;
//    _TAB_ << "{" << std::endl;
//    ++_tab;
//
//    _TAB_ << "for (int i = 0; " << strName << "[i]; ++i)" << std::endl;
//    _TAB_ << "{" << std::endl;
//    ++_tab;
//
//    _TAB_ << "if (" << varName << "[i] == value)" << std::endl;
//    Tab(_tab + 1) << "return " << strName << "[i];" << std::endl;
//
//    --_tab;
//    _TAB_ << "}" << std::endl << std::endl;
//    _TAB_ << "return nullptr;" << std::endl;
//
//    --_tab;
//    _TAB_ << "}" << std::endl;
//    return true;
//}
//
//bool CppExporter::String2Enum(const IEnumType* eType)
//{
//    std::vector<std::string> path = util::Absolute(eType);
//    std::string base = util::Contact(path, "_");
//    std::string varName = "s_" + base + "_val";
//    std::string strName = "s_" + base + "_str";
//
//    _TAB_ << "bool ToEnum(" << util::Contact(path, "::") << "& out, const char* name)" << std::endl;
//    _TAB_ << "{" << std::endl;
//    ++_tab;
//
//    _TAB_ << "for (int i = 0; " << strName << "[i]; ++i)" << std::endl;
//    _TAB_ << "{" << std::endl;
//    ++_tab;
//
//    _TAB_ << "if (std::strcmp(" << strName << "[i], name) == 0)" << std::endl;
//    _TAB_ << "{" << std::endl;
//    ++_tab;
//
//    _TAB_ << "out = " << "(" << util::Contact(path, "::") << ")" << varName << "[i];" << std::endl;
//    _TAB_ << "return true;" << std::endl;
//
//    --_tab;
//    _TAB_ << "}" << std::endl;
//
//    --_tab;
//    _TAB_ << "}" << std::endl << std::endl;
//
//    _TAB_ << "return false;" << std::endl;
//    --_tab;
//    _TAB_ << "}" << std::endl;
//    return true;
//}

bool CppExporter::DeclaerHeader()
{
    if (!_enums.empty())
    {
        _OUTS_ << "namespace Enum\n{" << std::endl;
        ++_tab;

        for (auto it = _enums.begin(); it != _enums.end(); ++it)
            FuncDeclare(*it);

        --_tab;
        _OUTS_ << "} // end of namespace Enum" << std::endl << std::endl;
    }

    if (!_structs.empty())
    {
        _OUTS_ << "namespace Tab\n{" << std::endl;
        ++_tab;

        for (auto it = _structs.begin(); it != _structs.end(); ++it)
            TabFuncDeclare(*it);

        --_tab;
        _OUTS_ << "} // end of namespace Tab" << std::endl;

        _OUTS_ << std::endl << "namespace Json\n{" << std::endl;
        ++_tab;

        for (auto it = _structs.begin(); it != _structs.end(); ++it)
            ; // TODO: json

        --_tab;
        _OUTS_ << "} // end of namespace Json" << std::endl;
    }
    return true;
}

bool CppExporter::ImplationCpp()
{
    _OUTS_ << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl;
    _OUTS_ << "#include \"xxx.h\"" << std::endl << std::endl;

    if (!_enums.empty())
    {
        _OUTS_ << "namespace Enum\n{" << std::endl;
        ++_tab;

        for (auto it = _enums.begin(); it != _enums.end(); ++it)
            EnumStaticData(*it);
        _OUTS_ << std::endl;

        for (auto it = _enums.begin(); it != _enums.end(); ++it)
            FuncImpl(*it);

        --_tab;
        _OUTS_ << "} // end of namespace Enum" << std::endl << std::endl;
    }

    if (!_structs.empty())
    {
        _OUTS_ << "namespace Tab\n{" << std::endl;
        ++_tab;

        for (auto it = _structs.begin(); it != _structs.end(); ++it)
            TabFuncImpl(*it);

        --_tab;
        _OUTS_ << "} // end of namespace Tab" << std::endl;

        _OUTS_ << std::endl << "namespace Json\n{" << std::endl;
        ++_tab;

        for (auto it = _structs.begin(); it != _structs.end(); ++it)
            ; // TODO: json

        --_tab;
        _OUTS_ << "} // end of namespace Json" << std::endl;
    }
    return true;
}


bool CppExporter::EnumStaticData(const IEnumType* eType)
{
    std::vector<std::string> path = util::Absolute(eType);
    std::string base = util::Contact(path, "_");
    std::string varName = "s_" + base + "_val";
    std::string strName = "s_" + base + "_str";

    const IVarSet* varSet = eType->VarSet();

    _TAB_ << "static const int " << varName << "[] = {" << std::endl;
    ++_tab;

    _TAB_;
    for (int i = 0; i < varSet->Size(); ++i)
    {
        _OUTS_ << "" << cpp_util::OrignalValue(varSet->Get(i)->Value()) << ", ";
        if (i && (i % 8) == 0)
        {
            _OUTS_ << std::endl;
            _TAB_;
        }
    }
    _OUTS_ << "-111111";

    --_tab;
    _OUTS_ << " }; // end of enum " << eType->Name() << std::endl;

    _TAB_ << "static const char* const  " << strName << "[] = {" << std::endl;
    ++_tab;

    _TAB_;
    for (int i = 0; i < varSet->Size(); ++i)
    {
        _OUTS_ << "\"" << varSet->Get(i)->Name() << "\", ";
        if (i && (i % 8) == 0)
        {
            _OUTS_ << std::endl;
            _TAB_;
        }
    }
    _OUTS_ << "nullptr";

    --_tab;
    _OUTS_ << " }; // end of enum " << eType->Name() << std::endl;

    return true;
}

bool CppExporter::FuncDeclare(const IEnumType* eType)
{
    std::vector<std::string> path = util::Absolute(eType);
    std::string name = util::Contact(path, "::");

    _TAB_ << "const char* const * Names(" << name << ");" << std::endl;
    _TAB_ << "const char* ToString(" << name << " value);" << std::endl;
    _TAB_ << "bool ToEnum(" << name << "& out, const char* name);" << std::endl;

    return true;
}

bool CppExporter::FuncImpl(const IEnumType* eType)
{
    std::vector<std::string> path = util::Absolute(eType);
    std::string tyName = util::Contact(path, "::");
    std::string varName = "s_" + util::Contact(path, "_") + "_val";
    std::string strName = "s_" + util::Contact(path, "_") + "_str";

    // const char*[] Names(Enum)
    _TAB_ << "const char*[] Names(" << tyName << ")" << std::endl;
    _TAB_ << "{" << std::endl;
    ++_tab;
    _TAB_ << "return " << strName << ";" << std::endl;
    --_tab;
    _TAB_ << "}" << std::endl;

    // const char* ToString(Enum value)
    _TAB_ << "const char* ToString(" << tyName << " value)" << std::endl;
    _TAB_ << "{" << std::endl;
    ++_tab;

    _TAB_ << "for (int i = 0; " << strName << "[i]; ++i)" << std::endl;
    _TAB_ << "{" << std::endl;
    ++_tab;

    _TAB_ << "if (" << varName << "[i] == value)" << std::endl;
    ++_tab;
    _TAB_ << "return " << strName << "[i];" << std::endl;
    --_tab;

    --_tab;
    _TAB_ << "}" << std::endl << std::endl;
    _TAB_ << "return nullptr;" << std::endl;

    --_tab;
    _TAB_ << "}" << std::endl;


    // bool ToEnum(Enum& out, const char* name)
    _TAB_ << "bool ToEnum(" << tyName << "& out, const char* name)" << std::endl;
    _TAB_ << "{" << std::endl;
    ++_tab;

    _TAB_ << "for (int i = 0; " << strName << "[i]; ++i)" << std::endl;
    _TAB_ << "{" << std::endl;
    ++_tab;

    _TAB_ << "if (std::strcmp(" << strName << "[i], name) == 0)" << std::endl;
    _TAB_ << "{" << std::endl;
    ++_tab;

    _TAB_ << "out = " << "(" << tyName << ")" << varName << "[i];" << std::endl;
    _TAB_ << "return true;" << std::endl;

    --_tab;
    _TAB_ << "}" << std::endl;

    --_tab;
    _TAB_ << "}" << std::endl << std::endl;

    _TAB_ << "return false;" << std::endl;
    --_tab;
    _TAB_ << "}" << std::endl;
    return true;
}

bool CppExporter::TabFuncDeclare(const IStructType* sType)
{
    std::vector<std::string> path = util::Absolute(sType);
    std::string tyName = util::Contact(path, "::");

    _TAB_ << "int Load(ITabFile* pTab, std::vector<" << tyName << ">& out);" << std::endl;

    return true;
}

bool CppExporter::TabFuncImpl(const IStructType* sType)
{
    std::vector<std::string> path = util::Absolute(sType);
    std::string tyName = util::Contact(path, "::");

    TabLoader(sType);
    _OUTS_ << std::endl;

    _TAB_ << "int Load(ITabFile* pTab, std::vector<" << tyName << ">& out)" << std::endl;
    _TAB_ << "{" << std::endl;
    ++_tab;

    _TAB_ << "if (pTab == nullptr || pTab->GetHeight() <= 4) " << std::endl;
    ++_tab;
    _TAB_ << "return 0;" << std::endl;
    --_tab;
    _OUTS_ << std::endl;

    _TAB_ << tyName << " def;" << std::endl;
    _TAB_ << "Load(pTab, 4, def);" << std::endl;
    _OUTS_ << std::endl;

    _TAB_ << "out.resize(pTab->GetHeight() - 4, def);" << std::endl;
    _TAB_ << "for (int i = 5; i < pTab->GetHeight(); ++i)" << std::endl;
    _TAB_ << "{" << std::endl;
    ++_tab;
    _TAB_ << "Load(pTab, i, def)" << std::endl;
    --_tab;
    _TAB_ << "}" << std::endl;
    --_tab;
    _TAB_ << "}" << std::endl;
    return true;
}

bool CppExporter::TabLoader(const IStructType* sType)
{
    std::vector<std::string> path = util::Absolute(sType);
    std::string tyName = util::Contact(path, "::");

    _TAB_ << "static bool Load(ITabFile* pTab, int line, " << tyName << "& data)" << std::endl;

    _TAB_ << "{" << std::endl;
    ++_tab;

    TabLoad(sType, "data");

    _TAB_ << "return true;" << std::endl;
    --_tab;
    _TAB_ << "}" << std::endl;
    return true;
}

bool CppExporter::TabLoad(const IStructType* sType, const std::string& owner)
{
    const IVarSet* varSet = sType->VarSet();
    for (int i = 0; i < varSet->Size(); ++i)
    {
        const IVariate* var = varSet->Get(i);
        TypeCategory typeCategory = var->Type()->Category();
        std::string name;
        if (owner.empty())
            name = var->Name();
        else
            name = owner + '.' + var->Name();

        if (typeCategory == TypeCategory::Raw)
            TabLoad(var, static_cast<const IRawType*>(var->Type()), name);
        else if (typeCategory == TypeCategory::Enum)
            TabLoad(var, static_cast<const IEnumType*>(var->Type()), name);
        else if (typeCategory == TypeCategory::Array)
            TabLoad(var, static_cast<const IArrayType*>(var->Type()), name);
        else if (typeCategory == TypeCategory::Struct)
            TabLoad(static_cast<const IStructType*>(var->Type()), name);
        else
            ERROR_NOT_ALLOW;
    }
    return true;
}

bool CppExporter::TabLoad(const IVariate* var, const IRawType* type, const std::string& name)
{
    _TAB_;
    switch (type->Raw())
    {
    case RawCategory::Bool:
        _OUTS_ << "pTab->GetBoolean(line, \"" << name << "\" , " << name << ", " << name << ");" << std::endl;
        break;
    case RawCategory::Int:
        _OUTS_ << "pTab->GetInteger(line, \"" << name << "\" , " << name << ", &" << name << ");" << std::endl;
        break;
    case RawCategory::Float:
        _OUTS_ << "pTab->GetFloat(line, \"" << name << "\" , " << name << ", &" << name << ");" << std::endl;
        break;
    case RawCategory::String:
        {
            _TAB_ << "{" << std::endl;
            ++_tab;
            _TAB_ << "char buffer[1024] = {0};" << std::endl;

            _TAB_ << "pTab->GetString(line, \"" << name << "\" , " << name << ".c_str(), buffer, 1024);" << std::endl;

            _TAB_ << name << " = buffer;" << std::endl;

            --_tab;
            _TAB_ << "}" << std::endl;
        }
        break;
    }
    return true;
}

bool CppExporter::TabLoad(const IVariate* var, const IEnumType* type, const std::string& name)
{
    _TAB_ << "pTab->GetInteger(line, \"" << name << "\" , false, &" << name << ");" << std::endl;
    return true;
}

bool CppExporter::TabLoad(const IVariate* var, const IArrayType* type, const std::string& name)
{
    const IType* orignal = type->Original();
    if (orignal != type->Prev())
    {
        // 多维数组
        ERROR_NOT_ALLOW;
        return false;
    }

    if (orignal->Category() == TypeCategory::Struct)
    {
        if (type->Length() < 0)
        {
            ERROR_NOT_ALLOW;
            return false;
        }

        const IStructType* sType = static_cast<const IStructType*>(orignal);
        for (int i = 0; i < type->Length(); ++i)
            TabLoad(sType, name + "_" + std::to_string(i + 1));
    }
    else if (orignal->Category() == TypeCategory::Enum)
    {
        _TAB_ << "{" << std::endl;
        ++_tab;

        _TAB_ << "std::vector<std::string> vecStr;" << std::endl;
        _TAB_ << "char buffer[1024] = {0};" << std::endl;
        _TAB_ << "pTab->GetString(line, \"" << name << "\" , \"\", buffer, 1024);" << std::endl;
        _TAB_ << "vecStr = KGTool::Split(buffer, ',');" << std::endl;

        //TODO:

        --_tab;
        _TAB_ << "}" << std::endl;
    }
    else if (orignal->Category() == TypeCategory::Raw)
    {
        _TAB_ << "{" << std::endl;
        ++_tab;

        _TAB_ << "std::vector<std::string> vecStr;" << std::endl;
        _TAB_ << "char buffer[1024] = {0};" << std::endl;
        _TAB_ << "pTab->GetString(line, \"" << name << "\" , \"\", buffer, 1024);" << std::endl;
        _TAB_ << "vecStr = KGTool::Split(buffer, ',');" << std::endl;

        //TODO:

        --_tab;
        _TAB_ << "}" << std::endl;
    }
    return true;
}

bool CppExporter::JsonFuncDeclare(const IStructType* sType)
{
    return true;
}

bool CppExporter::JsonFuncImpl(const IStructType* sType)
{
    return true;
}


CFG_NAMESPACE_END
