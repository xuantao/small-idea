#include "CppExporter.h"
#include "Utility.h"
#include "CppStruct.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <set>

CFG_NAMESPACE_BEGIN

//#define _DEF_STREAM_ &std::cerr
//#define _OUTS_ (*_stream)
//#define _TAB_ utility::Tab(*_stream, _tab)
//#define _TAB_EX_(ex) std::string((_tab + ex) * 4, ' ')

#define _NEED_TEMP_INT      0x01
#define _NEED_TEMP_BUFFER   0x02
#define _NEED_TEMP_ARRAY    0x04

static int sNeedTempMask(const IStructType* sType)
{
    int mask = 0;
    const IVarSet* vars = sType->VarSet();
    for (int i = 0; i < vars->Size(); ++i)
    {
        const IType* type = vars->Get(i)->Type();
        if (type->Category() == TypeCategory::Array)
        {
            type = static_cast<const IArrayType*>(type)->Original();
            if ((type->Category() != TypeCategory::Raw || ((const IRawType*)type)->Raw() != RawCategory::String) ||
                (type->Category() != TypeCategory::Struct))
            {
                mask |= _NEED_TEMP_ARRAY;
            }
        }

        if (type->Category() == TypeCategory::Raw)
        {
            //if (static_cast<const IRawType*>(type)->Raw() == RawCategory::String)
            //    mask |= _NEED_TEMP_BUFFER;
        }
        else if (type->Category() == TypeCategory::Struct)
        {
            //mask |= sNeedTempMask(static_cast<const IStructType*>(type));
        }
        else if (type->Category() == TypeCategory::Enum)
        {
            mask |= _NEED_TEMP_INT | _NEED_TEMP_BUFFER;
        }
    }
    return mask;
}

namespace detail
{
    class TabLoader : public IVarVistor
    {
    public:
        TabLoader(std::ostream* stream, int tab) : _stream(stream), _tab(tab)
        {
        }

    public:
        virtual bool OnStart(const IStructType* sType)
        {
            return true;
        }

        virtual bool OnEnd()
        {
            return true;
        }

        virtual bool OnVar(const IVariate* var, const IRawType* rType, const std::string& title, const std::string& path)
        {
            switch (rType->Raw())
            {
            case RawCategory::Bool:
                _TAB_ << "pTab->GetBoolean(line, \"" << title << "\" , out." << path << ", &out." << path << ");" << std::endl;
                break;
            case RawCategory::Int:
                _TAB_ << "pTab->GetInteger(line, \"" << title << "\" , out." << path << ", &out." << path << ");" << std::endl;
                break;
            case RawCategory::Float:
                _TAB_ << "pTab->GetFloat(line, \"" << title << "\" , out." << path << ", &" << path << ");" << std::endl;
                break;
            case RawCategory::String:
                _TAB_ << "pTab->GetString(line, \"" << path << "\" , out." << path << ".c_str(), cBuffer, CFG_STR_BUFFER_SIZE);" << std::endl;
                _TAB_ << "out." << path << " = cBuffer;" << std::endl;
                break;
            }
            return true;
        }

        virtual bool OnVar(const IVariate* var, const IEnumType* eType, const std::string& title, const std::string& path)
        {
            std::string tyName = utility::Contact(utility::Absolute(eType), "::");
            _TAB_ << "pTab->GetString(line, \"" << title << "\" , \"\", cBuffer, CFG_STR_BUFFER_SIZE);" << std::endl <<
                _TAB_EX_(0) << "if (!Enum::ToEnum(cBuffer, out." << path << "))" << std::endl <<
                _TAB_EX_(0) << "{" << std::endl <<
                _TAB_EX_(1) << "if (utility::Convert(cBuffer, nTemp) && Enum::ToString((" << tyName << ")nTemp))" << std::endl <<
                _TAB_EX_(2) << "out." << path << " = (" << tyName << ")nTemp;" << std::endl <<
                _TAB_EX_(1) << "else" << std::endl <<
                _TAB_EX_(2) << "; //TODO: log error" << std::endl <<
                _TAB_EX_(0) << "}" << std::endl;
            return true;
        }

        virtual bool OnVar(const IVariate* var, const IRawType* rType, const std::string& title, const std::string& path, int length)
        {
            if (rType->Raw() == RawCategory::String)
            {
                _OUTS_ <<
                    _TAB_EX_(0) << "pTab->GetString(line, \"" << title << "\" , \"\", cBuffer, CFG_STR_BUFFER_SIZE);" << std::endl <<
                    _TAB_EX_(0) << "out." << path << " = utility::Split(cBuffer, \",\");" << std::endl;
            }
            else
            {
                _OUTS_ <<
                    _TAB_EX_(0) << "pTab->GetString(line, \"" << title << "\" , \"\", cBuffer, CFG_STR_BUFFER_SIZE);" << std::endl <<
                    _TAB_EX_(0) << "vecStr = utility::Split(cBuffer, \",\");" << std::endl;

                if (length <= 0)
                {
                    _OUTS_ <<
                        _TAB_EX_(0) << "out." << path << ".resize(vecStr.size());" << std::endl <<
                        _TAB_EX_(0) << "for (size_t i = 0; i < vecStr.size(); ++i)" << std::endl;
                }
                else
                {
                    _OUTS_ <<
                        _TAB_EX_(0) << "for (size_t i = 0; i < std::min(vecStr.size(), out." << path << ".size()); ++i)" << std::endl;
                }

                _OUTS_ <<
                    _TAB_EX_(0) << "{" << std::endl <<
                    _TAB_EX_(1) << "if (!utility::Convert(vecStr[i].c_str(), out." << path << "[i]))" << std::endl <<
                    _TAB_EX_(2) << "; //TODO: log error" << std::endl <<
                    _TAB_EX_(0) << "}" << std::endl;
            }
            return true;
        }

        virtual bool OnVar(const IVariate* var, const IEnumType* eType, const std::string& title, const std::string& path, int length)
        {
            std::string tyName = utility::Contact(utility::Absolute(eType), "::");
            _OUTS_ <<
                _TAB_EX_(0) << "pTab->GetString(line, \"" << title << "\" , \"\", cBuffer, CFG_STR_BUFFER_SIZE);" << std::endl <<
                _TAB_EX_(0) << "vecStr = utility::Split(cBuffer, \",\");" << std::endl;

            if (length <= 0)
            {
                _OUTS_ <<
                    _TAB_EX_(0) << "out." << path << ".resize(vecStr.size());" << std::endl <<
                    _TAB_EX_(0) << "for (size_t i = 0; i < vecStr.size(); ++i)" << std::endl;
            }
            else
            {
                _OUTS_ <<
                    _TAB_EX_(0) << "for (size_t i = 0; i < std::min(vecStr.size(), out." << path << ".size()); ++i)" << std::endl;
            }

            _OUTS_ <<
                _TAB_EX_(0) << "{" << std::endl <<
                _TAB_EX_(1) << "if (!Enum::ToEnum(vecStr[i].c_str(), out." << path << "[i]))" << std::endl <<
                _TAB_EX_(1) << "{" << std::endl <<
                _TAB_EX_(2) << "if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((" << tyName << ")nTemp))" << std::endl <<
                _TAB_EX_(3) << "out." << path << "[i] = (" << tyName << ")nTemp;" << std::endl <<
                _TAB_EX_(2) << "else" << std::endl <<
                _TAB_EX_(3) << "; //TODO: log error" << std::endl <<
                _TAB_EX_(1) << "}" << std::endl <<
                _TAB_EX_(0) << "}" << std::endl;
            return true;
        }

    protected:
        bool _newLine;
        std::ostream* _stream;
        int _tab;
    };
}

static bool _OnVar(const IVariate* var, const IRawType* rType, std::ostream* _stream, int _tab)
{
    switch (rType->Raw())
    {
    case RawCategory::Bool:
        break;
    case RawCategory::Int:
        break;
    case RawCategory::Float:
        _TAB_ << "utility::Convert(iter->Next(), out." << var->Name() << ");" << std::endl;
        break;
    case RawCategory::String:
        _TAB_ << "out." << var->Name() << " = iter.Next();" << std::endl;
        break;
    }
    return true;
}

static bool _OnVar(const IVariate* var, const IEnumType* eType, std::ostream* _stream, int _tab)
{
    std::string tyName = utility::Contact(utility::Absolute(eType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "str = iter.Next();" << std::endl <<
        _TAB_EX_(0) << "if (!Enum::ToEnum(cBuffer, out." << var->Name() << "))" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "if (utility::Convert(cBuffer, val) && Enum::ToString((" << tyName << ")val))" << std::endl <<
        _TAB_EX_(2) << "out." << var->Name() << " = (" << tyName << ")nTemp;" << std::endl <<
        _TAB_EX_(1) << "else" << std::endl <<
        _TAB_EX_(2) << "; //TODO: log error" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
    return true;
}

static bool _OnVar(const IVariate* var, const IRawType* rType, int length, std::ostream* _stream, int _tab)
{
    if (rType->Raw() == RawCategory::String)
    {
        _TAB_ << "out." << var->Name() << " = utility::Split(iter.Next(), \",\");" << std::endl;
    }
    else
    {
        _TAB_ << "vec = utility::Split(iter.Next(), \",\");" << std::endl;

        if (length <= 0)
        {
            _OUTS_ <<
                _TAB_EX_(0) << "out." << var->Name() << ".resize(vec.size());" << std::endl <<
                _TAB_EX_(0) << "for (size_t i = 0; i < vec.size(); ++i)" << std::endl;
        }
        else
        {
            _OUTS_ <<
                _TAB_EX_(0) << "for (size_t i = 0; i < std::min(vec.size(), out." << var->Name() << ".size()); ++i)" << std::endl;
        }

        _OUTS_ <<
            _TAB_EX_(0) << "{" << std::endl <<
            _TAB_EX_(1) << "if (!utility::Convert(vec[i].c_str(), out." << var->Name() << "[i]))" << std::endl <<
            _TAB_EX_(2) << "; //TODO: log error" << std::endl <<
            _TAB_EX_(0) << "}" << std::endl;
    }
    return true;
}

static bool _OnVar(const IVariate* var, const IEnumType* eType, int length, std::ostream* _stream, int _tab)
{
    std::string tyName = utility::Contact(utility::Absolute(eType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "vec = utility::Split(iter.Next(), \",\");" << std::endl;

    if (length <= 0)
    {
        _OUTS_ <<
            _TAB_EX_(0) << "out." << var->Name() << ".resize(vec.size());" << std::endl <<
            _TAB_EX_(0) << "for (size_t i = 0; i < vec.size(); ++i)" << std::endl;
    }
    else
    {
        _OUTS_ <<
            _TAB_EX_(0) << "for (size_t i = 0; i < std::min(vec.size(), out." << var->Name() << ".size()); ++i)" << std::endl;
    }

    _OUTS_ <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "if (!Enum::ToEnum(vec[i].c_str(), out." << var->Name() << "[i]))" << std::endl <<
        _TAB_EX_(1) << "{" << std::endl <<
        _TAB_EX_(2) << "if (utility::Convert(vec[i].c_str(), val) && Enum::ToString((" << tyName << ")val))" << std::endl <<
        _TAB_EX_(3) << "out." << var->Name() << "[i] = (" << tyName << ")nTemp;" << std::endl <<
        _TAB_EX_(2) << "else" << std::endl <<
        _TAB_EX_(3) << "; //TODO: log error" << std::endl <<
        _TAB_EX_(1) << "}" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
    return true;
}

static bool _OnVar(const IVariate* var, const IStructType* rType, int length, std::ostream* _stream, int _tab)
{
    _OUTS_ <<
        _TAB_EX_(0) << "for (size_t i = 0; i < out." << var->Name() << ".size(); ++i)" << std::endl <<
        _TAB_EX_(1) << "sLoad(iter, out." << var->Name() << "[i]);" << std::endl;
    return true;
}

static void _loader(const IStructType* sType, std::ostream* _stream, int _tab)
{
    std::string tyName = utility::Contact(utility::Absolute(sType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "static bool sLoad(utility::TabIter& iter, " << tyName << "& out)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl;

    int mask = sNeedTempMask(sType);
    if (mask & _NEED_TEMP_INT)
        _OUTS_ << _TAB_EX_(1) << "int val = 0;" << std::endl;
    if (mask & _NEED_TEMP_BUFFER)
        _OUTS_ << _TAB_EX_(1) << "const char* str = nullptr;" << std::endl;
    if (mask & _NEED_TEMP_ARRAY)
        _OUTS_ << _TAB_EX_(1) << "std::vector<string> vec;" << std::endl;
    if (mask)
        _OUTS_ << std::endl;

    const IVarSet* vars = sType->VarSet();
    for (int i = 0; i < vars->Size(); ++i)
    {
        const IVariate* var = vars->Get(i);
        const IType* ty = var->Type();

        if (ty->Category() == TypeCategory::Raw)
        {
            _OnVar(var, (const IRawType*)ty, _stream, _tab + 1);
        }
        else if (ty->Category() == TypeCategory::Enum)
        {
            if (i) _OUTS_ << std::endl;
            _OnVar(var, (const IEnumType*)ty, _stream, _tab + 1);
        }
        else if (ty->Category() == TypeCategory::Struct)
        {
            _OUTS_ <<
                _TAB_EX_(1) << "sLoad(iter, out." << var->Name() << ");" << std::endl;
        }
        else if (ty->Category() == TypeCategory::Array)
        {
            if (i) _OUTS_ << std::endl;

            const IArrayType* aTy = (const IArrayType*)ty;
            const IType* original = aTy->Original();
            if (original->Category() == TypeCategory::Raw)
            {
                _OnVar(var, (const IRawType*)original, aTy->Length(), _stream, _tab + 1);
            }
            else if (original->Category() == TypeCategory::Enum)
            {
                _OnVar(var, (const IEnumType*)original, aTy->Length(), _stream, _tab + 1);
            }
            else if (original->Category() == TypeCategory::Struct)
            {
                _OnVar(var, (const IStructType*)original, aTy->Length(), _stream, _tab + 1);
            }
        }
    }

    _OUTS_ <<
        _TAB_EX_(1) << "return true;" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

CppExporter::CppExporter()
    : _stream(_DEF_STREAM_)
    , _global(nullptr)
    , _tab(0)
{
}

CppExporter::~CppExporter()
{
}

void CppExporter::OnBegin(const IScopeType* global, const std::string& path, const std::string& file)
{
    _global = global;
    _path = path;
    _file = file;

    //std::ofstream* header = new std::ofstream();
    //header->open(path + file + ".h");
    //if (header->is_open())
    //    _stream = header;
    //else
    //    delete header;

    _OUTS_ << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl <<
        "#pragma once" << std::endl << std::endl <<
        "#include <vector>" << std::endl <<
        "#include <string>" << std::endl <<
        "#include <array>" << std::endl << std::endl <<
        "class ITabFile;" << std::endl << std::endl;
}

void CppExporter::OnEnd()
{
    return;

    // api declare
    DeclaerHeader();

    if (_stream != _DEF_STREAM_)
    {
        static_cast<std::ofstream*>(_stream)->close();
        delete _stream;
        _stream = _DEF_STREAM_;
    }

    // build cpp file, api implemention
    std::ofstream cpp;
    cpp.open(_path + _file + ".cpp");
    if (cpp.is_open())
        _stream = &cpp;

    ImplationCpp();

    _stream = _DEF_STREAM_;
    cpp.close();

    // clear
    _structs.clear();
    _enums.clear();
    _file = "";
    _global = nullptr;
    _tab = 0;
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
    std::string name = utility::TrimFileSuffix(file, '.') + ".h";
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

    _OUTS_ << "static const " << data.type << " " << data.name;
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
        _loader(sType, &std::cout, 0);
        //Declare(sType);
        //_structs.push_back(sType);

        //if (sType->Cfg() == CfgCategory::Tab)
        //{
        //    GetDepends(sType, _tabDepends);
        //    _tabs.push_back(sType);
        //}
        //else
        //{
        //    GetDepends(sType, _jsonDepends);
        //    _jsons.push_back(sType);;
        //}
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
            _OUTS_ << " = " << data.value << ",";
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
    _OUTS_ << "#include \"" << _file << ".h\"" << std::endl <<
        "#include \"CfgUtility.h\"" << std::endl <<
        "#include \"ITabFile.h\"" << std::endl << std::endl <<
        "using namespace cfg;" << std::endl << std::endl;

    if (!_enums.empty())
    {
        _OUTS_ << "namespace Enum\n{" << std::endl;
        ++_tab;

        for (auto it = _enums.begin(); it != _enums.end(); ++it)
        {
            EnumStaticData(*it);
            _OUTS_ << std::endl;
        }

        for (auto it = _enums.begin(); it != _enums.end(); ++it)
        {
            FuncImpl(*it);
            if (it + 1 != _enums.end())
                _OUTS_ << std::endl;
        }

        --_tab;
        _OUTS_ << "} // end of namespace Enum" << std::endl << std::endl;
    }

    if (!_structs.empty())
    {
        _OUTS_ << "namespace Tab\n{" << std::endl;
        ++_tab;

        for (auto it = _structs.begin(); it != _structs.end(); ++it)
        {
            TabFuncImpl(*it);
            if (it + 1 != _structs.end())
                _OUTS_ << std::endl;
        }

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
    std::vector<std::string> path = utility::Absolute(eType);
    std::string base = utility::Contact(path, "_");
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
    _OUTS_ << "CFG_INVALID_ENUM";

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
    std::vector<std::string> path = utility::Absolute(eType);
    std::string name = utility::Contact(path, "::");

    _TAB_ << "const char* const * Names(" << name << ");" << std::endl;
    _TAB_ << "const char* ToString(" << name << " value);" << std::endl;
    _TAB_ << "bool ToEnum(const char* name," << name << "& out);" << std::endl;

    return true;
}

bool CppExporter::FuncImpl(const IEnumType* eType)
{
    std::vector<std::string> path = utility::Absolute(eType);
    std::string tyName = utility::Contact(path, "::");
    std::string varName = "s_" + utility::Contact(path, "_") + "_val";
    std::string strName = "s_" + utility::Contact(path, "_") + "_str";

    // const char* const * Names(Enum)
    _TAB_ << "const char* const * Names(" << tyName << ")" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "return " << strName << ";" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
    _OUTS_ << std::endl;

    // const char* ToString(Enum value)
    _OUTS_ <<
        _TAB_EX_(0) << "const char* ToString(" << tyName << " value)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "for (int i = 0; " << strName << "[i]; ++i)" << std::endl <<
        _TAB_EX_(1) << "{" << std::endl <<
        _TAB_EX_(2) << "if (" << varName << "[i] == value)" << std::endl <<
        _TAB_EX_(3) << "return " << strName << "[i];" << std::endl <<
        _TAB_EX_(1) << "}" << std::endl << std::endl <<
        _TAB_EX_(1) << "return nullptr;" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
    _OUTS_ << std::endl;

    // bool ToEnum(Enum& out, const char* name)
    _TAB_ << "bool ToEnum(const char* name, " << tyName << "& out)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "for (int i = 0; " << strName << "[i]; ++i)" << std::endl <<
        _TAB_EX_(1) << "{" << std::endl <<
        _TAB_EX_(2) << "if (std::strcmp(" << strName << "[i], name) == 0)" << std::endl <<
        _TAB_EX_(2) << "{" << std::endl <<
        _TAB_EX_(3) << "out = " << "(" << tyName << ")" << varName << "[i];" << std::endl <<
        _TAB_EX_(3) << "return true;" << std::endl <<
        _TAB_EX_(2) << "}" << std::endl <<
        _TAB_EX_(1) << "}" << std::endl << std::endl <<
        _TAB_EX_(1) << "return false;" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
    return true;
}

bool CppExporter::TabFuncDeclare(const IStructType* sType)
{
    std::vector<std::string> path = utility::Absolute(sType);
    std::string tyName = utility::Contact(path, "::");

    _TAB_ << "int Load(ITabFile* pTab, std::vector<" << tyName << ">& out);" << std::endl;

    return true;
}

bool CppExporter::TabFuncImpl(const IStructType* sType)
{
    std::vector<std::string> path = utility::Absolute(sType);
    std::string tyName = utility::Contact(path, "::");

    TabLoaderFunc(sType);

    _OUTS_ << std::endl;
    _OUTS_ <<
        _TAB_EX_(0) << "int Load(ITabFile* pTab, std::vector<" << tyName << ">& out)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "size_t prev = out.size();" << std::endl <<
        _TAB_EX_(1) << "if (pTab == nullptr || pTab->GetHeight() <= 4) " << std::endl <<
        _TAB_EX_(2) << "return 0;" << std::endl << std::endl;

    _OUTS_ <<
        _TAB_EX_(1) << "// load default data" << std::endl <<
        _TAB_EX_(1) << tyName << " def;" << std::endl <<
        _TAB_EX_(1) << "Load(pTab, 4, def);" << std::endl << std::endl;

    _OUTS_ <<
        _TAB_EX_(1) << "for (int i = 5; i < pTab->GetHeight(); ++i)" << std::endl <<
        _TAB_EX_(1) << "{" << std::endl <<
        _TAB_EX_(2) << tyName << " data = def;" << std::endl <<
        _TAB_EX_(2) << "if (Load(pTab, i, data))" << std::endl <<
        _TAB_EX_(3) << "out.push_back(data);" << std::endl <<
        _TAB_EX_(1) << "}" << std::endl << std::endl <<
        _TAB_EX_(1) << "return out.size() - prev;" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
    return true;
}

bool CppExporter::TabLoaderFunc(const IStructType* sType)
{
    std::vector<std::string> path = utility::Absolute(sType);
    std::string tyName = utility::Contact(path, "::");
    int mask = sNeedTempMask(sType);

    _TAB_ << "static bool Load(ITabFile* pTab, int line, " << tyName << "& out)" << std::endl;

    _TAB_ << "{" << std::endl;
    ++_tab;

    if (mask & _NEED_TEMP_INT)
        _TAB_ << "int nTemp = 0;" << std::endl;
    if (mask & _NEED_TEMP_BUFFER)
        _TAB_ << "char cBuffer[CFG_STR_BUFFER_SIZE] = {0};" << std::endl;
    if (mask & _NEED_TEMP_ARRAY)
        _TAB_ << "std::vector<std::string> vecStr;" << std::endl;
    if (mask)
        _OUTS_ << std::endl;

    //TabLoad(sType, "", "data");
    detail::TabLoader loader(_stream, _tab);
    utility::Traverse(sType, &loader);

    _TAB_ << "return true;" << std::endl;
    --_tab;
    _TAB_ << "}" << std::endl;
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

void CppExporter::GetDepends(const IStructType* sType, std::vector<const IStructType*>& deps)
{
    if (deps.end() != std::find(deps.begin(), deps.end(), sType))
        return;

    const IVarSet* vars = sType->VarSet();
    for (int i = 0; i < vars->Size(); ++i)
    {
        const IType* type = vars->Get(i)->Type();

        if (type->Category() == TypeCategory::Struct)
        {
            GetDepends(static_cast<const IStructType*>(type), deps);
        }
        else if (type->Category() == TypeCategory::Array)
        {
            const IArrayType* aType = static_cast<const IArrayType*>(type);
            if (aType->Original()->Category() == TypeCategory::Struct)
                GetDepends(static_cast<const IStructType*>(aType->Original()), deps);
        }
    }

    assert(deps.end() == std::find(deps.begin(), deps.end(), sType));
    deps.push_back(sType);
}

CFG_NAMESPACE_END
