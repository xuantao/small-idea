#include "CppExporter.h"
#include "CppUtil.h"
#include "CppDeclare.h"
#include "CppModule.h"
#include "CppSerialize.h"
#include "CppTab.h"
#include "CppEnum.h"
#include "utility/Utility.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <set>

GCF_NAMESPACE_BEGIN

#define _NEED_TEMP_INT      0x01
#define _NEED_TEMP_BUFFER   0x02
#define _NEED_TEMP_ARRAY    0x04

#define _DEF_STREAM_    &std::cerr
#define _OUTS_          (*_stream)
#define _TAB_           utility::Tab(*_stream, tab)
#define _TAB_EX_(ex)    std::string((tab + ex) * 4, ' ')

static CppExporter* _s_cppExporter = nullptr;

static int sNeedTempMask(const IStructType* sType)
{
    int mask = 0;
    const IVarSet* vars = sType->Scope()->VarSet();
    for (int i = 0; i < vars->Size(); ++i)
    {
        const IType* type = vars->Get(i)->Type();
        if (type->TypeCat() == TypeCategory::Array)
        {
            type = static_cast<const IArrayType*>(type)->Original();
            if ((type->TypeCat() != TypeCategory::Raw || ((const IRawType*)type)->RawCat() != RawCategory::String) ||
                (type->TypeCat() != TypeCategory::Struct))
            {
                mask |= _NEED_TEMP_ARRAY;
            }
            if (type->TypeCat() == TypeCategory::Enum)
                mask |= _NEED_TEMP_INT;

        }
        else if (type->TypeCat() == TypeCategory::Enum)
        {
            mask |= _NEED_TEMP_INT | _NEED_TEMP_BUFFER;
        }
    }
    return mask;
}

namespace detail
{
    //class TabTitles : public ITabVisitor
    //{
    //public:
    //    TabTitles(int tab) : _count(0), _tab(tab) {}
    //    virtual ~TabTitles() {}

    //public:
    //    size_t Count() const { return _count; }
    //    std::string Str() const { return _title.str(); }
    //public:
    //    virtual bool OnStart(const IStructType* sType) { return true; }
    //    virtual bool OnEnd() { return true; }

    //    virtual bool OnVar(const IVariate* var, const IRawType* rType,
    //        const std::string& title, const std::string& path)
    //    {
    //        OnTitle(title);
    //        return true;
    //    }
    //    virtual bool OnVar(const IVariate* var, const IEnumType* eType,
    //        const std::string& title, const std::string& path)
    //    {
    //        OnTitle(title);
    //        return true;
    //    }
    //    virtual bool OnVar(const IVariate* var, const IRawType* rType,
    //        const std::string& title, const std::string& path, int length)
    //    {
    //        OnTitle(title);
    //        return true;
    //    }
    //    virtual bool OnVar(const IVariate* var, const IEnumType* eType,
    //        const std::string& title, const std::string& path, int length)
    //    {
    //        OnTitle(title);
    //        return true;
    //    }

    //protected:
    //    void OnTitle(const std::string& title)
    //    {
    //        if (_count)
    //        {
    //            if (_count % 5 == 0)
    //            {
    //                _title << ",\n";
    //                utility::Tab(_title, _tab);
    //            }
    //            else
    //            {
    //                _title << ", ";
    //            }
    //        }

    //        ++_count;
    //        _title << "\"" << title << "\"";
    //    }

    //protected:
    //    std::stringstream _title;
    //    int _count;
    //    int _tab;
    //};
}

CppExporter::CppExporter()
    : _stream(_DEF_STREAM_)
    , _global(nullptr)
    , _tab(0)
    , _lastIsVar(false)
{
    _expoters.push_back(cpp::Tab::Create());
    _expoters.push_back(cpp::Enum::Create());
}

CppExporter::~CppExporter()
{
    if (_s_cppExporter == this)
        _s_cppExporter = nullptr;
}

CppExporter* CppExporter::GetInstance()
{
    return _s_cppExporter;
}

void CppExporter::Release()
{

}


bool CppExporter::OnBegin(const IScope* global, const char* path, const char* name)
{
    _global = global;

    if (path) _path = path;
    else _path = "";

    if (name && *name) _name = name;
    else _name = "unnamed";

    //std::ofstream* header = new std::ofstream();
    //header->open(file + ".h");
    //if (header->is_open())
    //    _stream = header;
    //else
    //    delete header;

    //_OUTS_ << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl <<
    //    "#pragma once" << std::endl << std::endl <<
    //    "#include <vector>" << std::endl <<
    //    "#include <string>" << std::endl <<
    //    "#include <array>" << std::endl <<
    //    "#include <ostream>" << std::endl << std::endl;


    _declare = new cpp::Declare(global, _path, _name);

    _serialize = new cpp::Serialize();
    _serialize->Begin(global, _path, _name);

    for (auto exp : _expoters)
        exp->OnBegin(global, _path.c_str(), _name.c_str());

    return true;
}

void CppExporter::OnEnd()
{
    _declare->DoExport();
    delete _declare;
    _declare = nullptr;

    _serialize->End();
    delete _serialize;
    _serialize = nullptr;

    for (auto exp : _expoters)
        exp->OnEnd();


    // api declare
    //HeaderDeclare();

    //if (_stream != _DEF_STREAM_)
    //{
    //    static_cast<std::ofstream*>(_stream)->close();
    //    delete _stream;
    //    _stream = _DEF_STREAM_;
    //}

    //// build cpp file, api implementation
    //std::ofstream cpp;
    //cpp.open(_file + ".cpp");
    //if (cpp.is_open())
    //    _stream = &cpp;

    //CppImpl();

    //_stream = _DEF_STREAM_;
    //cpp.close();

    //// clear
    //_structs.clear();
    //_enums.clear();
    //_file = "";
    //_global = nullptr;
    //_tab = 0;
    //_lastIsVar = false;
}

void CppExporter::OnNsBegin(const std::string& name)
{
    //int tab = _tab;
    //_OUTS_ <<
    //    _TAB_EX_(0) << "namespace " << name << std::endl <<
    //    _TAB_EX_(0) << "{" << std::endl;
    //++_tab;
    _declare->OnNsBegin(name);

    for (auto exp : _expoters)
        exp->OnNsBegin(name);
}

void CppExporter::OnNsEnd()
{
    //--_tab;
    //int tab = _tab;

    //_OUTS_ << _TAB_EX_(0) << "}" << std::endl;
    _declare->OnNsEnd();

    for (auto exp : _expoters)
        exp->OnNsEnd();
}

void CppExporter::OnInclude(const std::string& file)
{
    for (auto exp : _expoters)
        exp->OnInclude(file);
}

void CppExporter::OnVariate(const IVariate* var)
{
    //CppVarData data;
    //if (!cpp_util::Convert(var, data))
    //    return;

    //int tab = _tab;
    //_OUTS_ <<
    //    _TAB_EX_(0) << "static const " << data.type << " " << data.name;
    //if (!data.value.empty())
    //    _OUTS_ << " = " << data.value;
    //_OUTS_ << ";" << std::endl;

    //_lastIsVar = true;
    _declare->OnVariate(var);
    for (auto exp : _expoters)
        exp->OnVariate(var);
}

void CppExporter::OnType(const IType* type)
{
    _declare->OnType(type);
    _serialize->OnType(type);
    for (auto exp : _expoters)
        exp->OnType(type);
    //if (_lastIsVar)
    //{
    //    _OUTS_ << std::endl;
    //    _lastIsVar = false;
    //}

    //if (type->TypeCat() == TypeCategory::Enum)
    //{
    //    const IEnumType* eType = static_cast<const IEnumType*>(type);
    //    Declare(eType);
    //    _enums.push_back(eType);
    //}
    //else if (type->TypeCat() == TypeCategory::Struct)
    //{
    //    const IStructType* sType = static_cast<const IStructType*>(type);
    //    Declare(sType);

    //    if (sType->CfgCat() == CfgCategory::Tab)
    //    {
    //        GetDepends(sType, _tabDepends);
    //        _tabs.push_back(sType);
    //    }
    //    else if (sType->CfgCat() == CfgCategory::Json)
    //    {
    //        GetDepends(sType, _jsonDepends);
    //        _jsons.push_back(sType);;
    //    }
    //}
    //else
    //{
    //    std::cerr << "does not support export type:" << type->Name() << std::endl;
    //    ERROR_NOT_ALLOW;
    //}

    //_OUTS_ << std::endl;
}

void CppExporter::OnModule(const IModule* module)
{
    cpp::Module::Export(module, _path, _name);
    for (auto exp : _expoters)
        exp->OnModule(module);
}

bool CppExporter::Declare(const IEnumType* ty)
{
    int tab = _tab;
    _TAB_ << "enum class " << ty->Name() << std::endl;
    _TAB_ << "{" << std::endl;
    ++tab;

    const IVarSet* vars = ty->Scope()->VarSet();
    for (int i = 0; i < vars->Size(); ++i)
    {
        CppVarData data;
        cpp_util::Convert(vars->Get(i), data);

        _TAB_ << data.name;
        if (!data.value.empty())
            _OUTS_ << " = " << data.value << ",";
        _OUTS_ << std::endl;
    }

    --tab;
    _TAB_ << "};" << std::endl;
    return true;
}

bool CppExporter::Declare(const IStructType* sType)
{
    int tab = _tab;
    const IVarSet* varSet = sType->OwnScope()->VarSet();
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
        _OUTS_ << " : public " << cpp_util::TypeName(sType->Owner(), sType->Inherited());
    _OUTS_ << std::endl;

    _TAB_ << "{" << std::endl;
    ++tab;

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
        ++tab;

        bool first = true;
        for (size_t i = 0; i < vars.size(); i++)
        {
            const CppVarData& data = vars[i];
            if (data.var->IsConst())
                continue;

            const IType* type = data.var->Type();
            if (type->TypeCat() != TypeCategory::Raw || data.value.empty())
                continue;

            _TAB_;
            if (first)
                _OUTS_ << ": ";
            else
                _OUTS_ << ", ";

            _OUTS_ << data.name << "(";

            if (data.value.empty())
            {
                assert(type->TypeCat() == TypeCategory::Raw);
                _OUTS_ << cpp_util::DefValue(static_cast<const IRawType*>(type)->RawCat());
            }
            else
            {
                _OUTS_ << data.value;
            }

            _OUTS_ << ")" << std::endl;
            first = false;
        }

        --tab;
        _TAB_ << "{" << std::endl;

        for (size_t i = 0; i < vars.size(); i++)
        {
            const CppVarData& data = vars[i];
            if (data.var->IsConst())
                continue;

            const IType* type = data.var->Type();
            if (type->TypeCat() != TypeCategory::Array || data.value.empty())
                continue;

            const IArrayType* aTy = static_cast<const IArrayType*>(type);
            if (aTy->Length() == 0)
                continue;

            _OUTS_ <<
                _TAB_EX_(1) << data.name << ".fill(" << data.value << ");" << std::endl;
        }

        _TAB_ << "}" << std::endl << std::endl;
    }

    /* member data */
    for (size_t i = 0; i < vars.size(); i++)
    {
        const CppVarData& data = vars[i];
        if (data.var->IsConst())
            continue;

        _TAB_ << "" << data.type << " " << data.name << ";" << std::endl;
    }

    --tab;
    _TAB_ << "};" << std::endl;

    return true;
}

bool CppExporter::HeaderDeclare()
{
    int tab = _tab;
    if (!_enums.empty())
    {
        _OUTS_ << "namespace Enum\n{" << std::endl;

        for (int i = 0; i < _enums.size(); ++i)
        {
            EnumHelperDeclare(_enums[i], 1);
            if (i + 1 != _enums.size())
                _OUTS_ << std::endl;
        }

        _OUTS_ << "} // end of namespace Enum" << std::endl << std::endl;
    }

    if (!_tabs.empty())
    {
        _OUTS_ << "namespace Tab\n{" << std::endl;

        for (auto it = _tabs.begin(); it != _tabs.end(); ++it)
            TabDeclare(*it, 1);

        _OUTS_ << "} // end of namespace Tab" << std::endl;

        _OUTS_ << std::endl << "namespace Json\n{" << std::endl;

        for (auto it = _jsons.begin(); it != _jsons.end(); ++it)
            JsonDeclare(*it, 1);

        _OUTS_ << "} // end of namespace Json" << std::endl;
    }
    return true;
}

bool CppExporter::CppImpl()
{
    int tab = 0;

    _OUTS_ << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl;
    _OUTS_ << "#include \"" << _name << ".h\"" << std::endl <<
        "#include \"CfgUtility.h\"" << std::endl <<
        "#include \"CfgTabParser.h\"" << std::endl <<
        "#include <json/reader.h>" << std::endl << std::endl <<
        "#undef min" << std::endl <<
        "using namespace cfg;" << std::endl << std::endl;

    if (!_enums.empty())
    {
        _OUTS_ << "namespace Enum\n{" << std::endl;

        for (auto it = _enums.begin(); it != _enums.end(); ++it)
        {
            EnumData(*it, 1);
            _OUTS_ << std::endl;
        }

        for (auto it = _enums.begin(); it != _enums.end(); ++it)
        {
            EnumHelper(*it, 1);
            if (it + 1 != _enums.end())
                _OUTS_ << std::endl;
        }

        _OUTS_ << "} // end of namespace Enum" << std::endl << std::endl;
    }

    if (!_tabs.empty())
    {
        _OUTS_ << "namespace Tab\n{" << std::endl;

        for (size_t i = 0; i < _tabDepends.size(); ++i)
        {
            TabWriterStatic(_tabDepends[i], 1);
            _OUTS_ << std::endl;
        }

        for (size_t i = 0; i < _tabDepends.size(); ++i)
        {
            TabLoaderStatic(_tabDepends[i], 1);
            _OUTS_ << std::endl;
        }

        for (size_t i = 0; i < _tabs.size(); ++i)
        {
            TabWriteHeader(_tabs[i], 1);
            _OUTS_ << std::endl;
            TabWriter(_tabs[i], 1);
            _OUTS_ << std::endl;
            TabLoader(_tabs[i], 1);
            if (i + 1 != _tabs.size())
                _OUTS_ << std::endl;
        }

        _OUTS_ << "} // end of namespace Tab" << std::endl;

        _OUTS_ << std::endl << "namespace Json\n{" << std::endl;

        for (size_t i = 0; i < _jsonDepends.size(); ++i)
        {
            JsonWriterStatic(_jsonDepends[i], 1);
            _OUTS_ << std::endl;
        }

        for (size_t i = 0; i < _jsonDepends.size(); ++i)
        {
            JsonLoaderStatic(_jsonDepends[i], 1);
            _OUTS_ << std::endl;
        }

        for (size_t i = 0; i < _jsons.size(); ++i)
        {
            JsonWriter(_jsons[i], 1);
            _OUTS_ << std::endl;
            JsonLoader(_jsons[i], 1);
            if (i + 1 != _jsons.size())
                _OUTS_ << std::endl;
        }

        _OUTS_ << "} // end of namespace Json" << std::endl;
    }
    return true;
}

bool CppExporter::EnumData(const IEnumType* eType, int tab)
{
    std::vector<std::string> path = utility::Absolute(eType);
    std::string base = utility::Contact(path, "_");
    std::string varName = "s_" + base + "_val";
    std::string strName = "s_" + base + "_str";

    const IVarSet* varSet = eType->Scope()->VarSet();

    _TAB_ << "static const int " << varName << "[] = {" << std::endl;
    ++tab;

    _TAB_;
    for (int i = 0; i < varSet->Size(); ++i)
    {
        _OUTS_ << "" << cpp_util::ToString(varSet->Get(i)->Value()) << ", ";
        if (i && (i % 8) == 0)
        {
            _OUTS_ << std::endl;
            _TAB_;
        }
    }
    _OUTS_ << "CFG_INVALID_ENUM";

    --tab;
    _OUTS_ << " }; // end of enum " << eType->Name() << std::endl;

    _TAB_ << "static const char* const  " << strName << "[] = {" << std::endl;
    ++tab;

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

    --tab;
    _OUTS_ << " }; // end of enum " << eType->Name() << std::endl;

    return true;
}

bool CppExporter::EnumHelperDeclare(const IEnumType* eType, int tab)
{
    std::vector<std::string> path = utility::Absolute(eType);
    std::string tyName = utility::Contact(path, "::");
    _TAB_ << "/* " << tyName << " */" << std::endl;
    _TAB_ << "const char* const * Names(" << tyName << ");" << std::endl;
    _TAB_ << "const char* ToString(" << tyName << " value);" << std::endl;
    _TAB_ << "bool ToEnum(const char* name, " << tyName << "& out);" << std::endl;

    return true;
}

bool CppExporter::EnumHelper(const IEnumType* eType, int tab)
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
        _TAB_EX_(2) << "if (" << varName << "[i] == (int)value)" << std::endl <<
        _TAB_EX_(3) << "return " << strName << "[i];" << std::endl <<
        _TAB_EX_(1) << "}" << std::endl << std::endl <<
        _TAB_EX_(1) << "return \"\";" << std::endl <<
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

void CppExporter::TabDeclare(const IStructType* sType, int tab)
{
    std::vector<std::string> path = utility::Absolute(sType);
    std::string tyName = utility::Contact(path, "::");

    _OUTS_ <<
        _TAB_EX_(0) << "void WriteHeader(std::ostream& stream, const " << tyName << "& def);" << std::endl <<
        _TAB_EX_(0) << "void Write(std::ostream& stream, const " << tyName << "& data);" << std::endl <<
        _TAB_EX_(0) << "bool Load(const char* data, size_t size, std::vector<" << tyName << ">& out, const char* chunk = nullptr);" << std::endl;
}

void CppExporter::TabLoader(const IStructType* sType, int tab)
{
    //detail::TabTitles titles(tab + 2);
    //utility::Traverse(sType, &titles);

    //if (titles.Count() == 0)
    //    return;

    //std::string tyName = utility::Contact(utility::Absolute(sType), "::");
    //_OUTS_ <<
    //    _TAB_EX_(0) << "bool Load(const char* data, size_t size, std::vector<" << tyName << ">& out, const char* chunk /*= nullptr*/)" << std::endl <<
    //    _TAB_EX_(0) << "{" << std::endl;

    //_OUTS_ <<
    //    _TAB_EX_(1) << "static const std::array<const char*, " << titles.Count() << "> titles = {" << std::endl <<
    //    _TAB_EX_(2) << titles.Str() << std::endl <<
    //    _TAB_EX_(1) << "};" << std::endl << std::endl;

    //// define temp val
    //int mask = sNeedTempMask(sType);
    //if (mask & _NEED_TEMP_INT)
    //    _OUTS_ << _TAB_EX_(1) << "int val = 0;" << std::endl;
    //if (mask & _NEED_TEMP_BUFFER)
    //    _OUTS_ << _TAB_EX_(1) << "const char* str = nullptr;" << std::endl;
    //if (mask & _NEED_TEMP_ARRAY)
    //    _OUTS_ << _TAB_EX_(1) << "std::vector<std::string> vec;" << std::endl;
    //if (mask)
    //    _OUTS_ << std::endl;

    //// load default value
    //_OUTS_ <<
    //    _TAB_EX_(1) << "TabParser<" << titles.Count() << "> parser(titles);" << std::endl <<
    //    _TAB_EX_(1) << "if (!parser.Parse(data, size, chunk))" << std::endl <<
    //    _TAB_EX_(2) << "return false;" << std::endl << std::endl <<
    //    _TAB_EX_(1) << "// load default data" << std::endl <<
    //    _TAB_EX_(1) << tyName << " def;" << std::endl <<
    //    _TAB_EX_(1) << "if (!parser.NextLine() || !sLoad(parser.LineIter(), def))" << std::endl <<
    //    _TAB_EX_(2) << "return false;" << std::endl << std::endl;

    //// while load
    //_OUTS_ <<
    //    _TAB_EX_(1) << "while (parser.NextLine())" << std::endl <<
    //    _TAB_EX_(1) << "{" << std::endl <<
    //    _TAB_EX_(2) << tyName << " data = def;" << std::endl <<
    //    _TAB_EX_(2) << "if (sLoad(parser.LineIter(), data))" << std::endl <<
    //    _TAB_EX_(3) << "out.push_back(data);" << std::endl <<
    //    _TAB_EX_(1) << "}" << std::endl;

    //_OUTS_ <<
    //    _TAB_EX_(1) << "return true;" << std::endl <<
    //    _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::TabLoaderStatic(const IStructType* sType, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(sType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "static bool sLoad(TabLineIter& iter, " << tyName << "& out)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl;

    int mask = sNeedTempMask(sType);
    if (mask & _NEED_TEMP_INT)
        _OUTS_ << _TAB_EX_(1) << "int val = 0;" << std::endl;
    if (mask & _NEED_TEMP_BUFFER)
        _OUTS_ << _TAB_EX_(1) << "const char* str = nullptr;" << std::endl;
    if (mask & _NEED_TEMP_ARRAY)
        _OUTS_ << _TAB_EX_(1) << "std::vector<std::string> vec;" << std::endl;
    if (mask)
        _OUTS_ << std::endl;

    TabLoadDetail(sType, tab + 1);

    _OUTS_ <<
        _TAB_EX_(1) << "return true;" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::TabLoadDetail(const IStructType* sType, int tab)
{
    if (sType->Inherited())
    {
        std::string tyName = utility::Contact(utility::Absolute(sType->Inherited()), "::");
        _OUTS_ << _TAB_EX_(0) << "sLoad(iter, (" << tyName << "&)out);" << std::endl;
    }

    const IVarSet* vars = sType->OwnScope()->VarSet();
    bool bnewLine = false;
    for (int i = 0; i < vars->Size(); ++i)
    {
        const IVariate* var = vars->Get(i);
        const IType* ty = var->Type();

        if (var->IsConst())
            continue;

        if (ty->TypeCat() == TypeCategory::Raw)
        {
            TabLoadVar(var, (const IRawType*)ty, tab);
        }
        else if (ty->TypeCat() == TypeCategory::Enum)
        {
            if (i) _OUTS_ << std::endl;
            TabLoadVar(var, (const IEnumType*)ty, tab);
        }
        else if (ty->TypeCat() == TypeCategory::Struct)
        {
            const IStructType* sTy = (const IStructType*)ty;
            TabLoadVar(var, sTy, tab);
        }
        else if (ty->TypeCat() == TypeCategory::Array)
        {
            if (i) _OUTS_ << std::endl;

            const IArrayType* aTy = (const IArrayType*)ty;
            const IType* original = aTy->Original();
            if (original->TypeCat() == TypeCategory::Raw)
            {
                TabLoadVar(var, (const IRawType*)original, aTy->Length(), tab);
            }
            else if (original->TypeCat() == TypeCategory::Enum)
            {
                TabLoadVar(var, (const IEnumType*)original, aTy->Length(), tab);
            }
            else if (original->TypeCat() == TypeCategory::Struct)
            {
                TabLoadVar(var, (const IStructType*)original, aTy->Length(), tab);
            }
        }
    }
}

void CppExporter::TabLoadVar(const IVariate* var, const IRawType* rType, int tab)
{
    switch (rType->RawCat())
    {
    case RawCategory::Bool:
    case RawCategory::Int:
    case RawCategory::Float:
        _TAB_ << "iter.MoveNext();" << std::endl;
        _TAB_ << "utility::Convert(iter.Value(), out." << var->Name() << ");" << std::endl;
        break;
    case RawCategory::String:
        _OUTS_ <<
            _TAB_EX_(0) << "iter.MoveNext();" << std::endl <<
            _TAB_EX_(0) << "if (iter.Value() && iter.Value()[0])" << std::endl <<
            _TAB_EX_(1) << "out." << var->Name() << " = iter.Value();" << std::endl;
        break;
    }
}

void CppExporter::TabLoadVar(const IVariate* var, const IEnumType* eType, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(eType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "iter.MoveNext();" << std::endl <<
        _TAB_EX_(0) << "if (!Enum::ToEnum(iter.Value(), out." << var->Name() << "))" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "if (utility::Convert(iter.Value(), val) && Enum::ToString((" << tyName << ")val))" << std::endl <<
        _TAB_EX_(2) << "out." << var->Name() << " = (" << tyName << ")val;" << std::endl <<
        _TAB_EX_(1) << "else" << std::endl <<
        _TAB_EX_(2) << "utility::Log(\"chunk:%s line:%d title:%s Convert failed from type:%s value:%s\", iter.Chunk(), iter.LineNO(), iter.Title(), \"" << tyName << "\", iter.Value());" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::TabLoadVar(const IVariate* var, const IStructType* rType, int tab)
{
    _OUTS_ <<
        _TAB_EX_(0) << "sLoad(iter, out." << var->Name() << ");" << std::endl;
}

void CppExporter::TabLoadVar(const IVariate* var, const IRawType* rType, int length, int tab)
{
    _TAB_ << "iter.MoveNext();" << std::endl;
    if (rType->RawCat() == RawCategory::String)
    {
        _TAB_ << "out." << var->Name() << " = utility::Split(iter.Value(), \",\");" << std::endl;
    }
    else
    {
        _TAB_ << "vec = utility::Split(iter.Value(), \",\");" << std::endl;

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

        _OUTS_ << _TAB_EX_(1) << "utility::Convert(vec[i].c_str(), out." << var->Name() << "[i]);" << std::endl;
    }
}

void CppExporter::TabLoadVar(const IVariate* var, const IEnumType* eType, int length, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(eType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "iter.MoveNext();" << std::endl <<
        _TAB_EX_(0) << "vec = utility::Split(iter.Value(), \",\");" << std::endl;

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
        _TAB_EX_(3) << "out." << var->Name() << "[i] = (" << tyName << ")val;" << std::endl <<
        _TAB_EX_(2) << "else" << std::endl <<
        _TAB_EX_(3) << "utility::Log(\"chunk:%s line:%d title:%s Convert failed from type:%s value:%s\", iter.Chunk(), iter.LineNO(), iter.Title(), \"" << tyName << "\", vec[i].c_str());" << std::endl <<
        _TAB_EX_(1) << "}" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::TabLoadVar(const IVariate* var, const IStructType* rType, int length, int tab)
{
    _OUTS_ <<
        _TAB_EX_(0) << "for (size_t i = 0; i < out." << var->Name() << ".size(); ++i)" << std::endl <<
        _TAB_EX_(1) << "sLoad(iter, out." << var->Name() << "[i]);" << std::endl;
}

void CppExporter::TabWriteHeader(const IStructType* sType, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(sType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "void WriteHeader(std::ostream& stream, const " << tyName << "& def)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl;

    //TabVisitor visitor;
    //utility::Traverse(sType, &visitor);

    //_OUTS_ <<
    //    _TAB_EX_(1) << "static const char* title = \"" << utility::Contact(visitor.Title(), "\\t") << "\";" << std::endl <<
    //    _TAB_EX_(1) << "static const char* type = \"" << utility::Contact(visitor.Type(), "\\t") << "\";" << std::endl <<
    //    _TAB_EX_(1) << "static const char* desc = \"" << utility::Contact(visitor.Describe(), "\\t") << "\";" << std::endl << std::endl <<
    //    _TAB_EX_(1) << "stream << title << std::endl << type << std::endl << desc << std::endl;" << std::endl <<
    //    _TAB_EX_(1) << "Write(stream, def);" << std::endl;

    _OUTS_ <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::TabWriter(const IStructType* sType, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(sType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "void Write(std::ostream& stream, const " << tyName << "& data)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "sWrite(stream, data);" << std::endl <<
        _TAB_EX_(1) << "stream << std::endl;" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::TabWriterStatic(const IStructType* sType, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(sType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "void sWrite(std::ostream& stream, const " << tyName << "& data)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl;

    TabWriterDetail(sType, tab + 1);

    _OUTS_ <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::TabWriterDetail(const IStructType* sType, int tab)
{
    const IVarSet* vars = sType->OwnScope()->VarSet();
    if (sType->Inherited())
    {
        std::string inherit = utility::Contact(utility::Absolute(sType->Inherited()), "::");
        _TAB_ << "sWrite(stream, (const " << inherit << "&)data);" << std::endl;

        if (vars->Size())
            _TAB_ << "stream << \"\\t\";" << std::endl;
    }

    bool bnewLine = false;
    for (int i = 0; i < vars->Size(); ++i)
    {
        const IVariate* var = vars->Get(i);
        const IType* ty = var->Type();

        if (var->IsConst())
            continue;

        if (ty->TypeCat() == TypeCategory::Raw)
        {
            _TAB_ << "stream << data." << var->Name() << ";" << std::endl;
        }
        else if (ty->TypeCat() == TypeCategory::Enum)
        {
            _TAB_ << "stream << Enum::ToString(data." << var->Name() << ");" << std::endl;
        }
        else if (ty->TypeCat() == TypeCategory::Struct)
        {
            _TAB_ << "sWrite(stream, data." << var->Name() << ");" << std::endl;
        }
        else if (ty->TypeCat() == TypeCategory::Array)
        {
            if (i) _OUTS_ << std::endl;

            const IArrayType* aTy = (const IArrayType*)ty;
            const IType* original = aTy->Original();

            _TAB_ << "for (size_t i = 0; i < data." << var->Name() << ".size(); ++i)" << std::endl <<
                _TAB_EX_(0) << "{" << std::endl <<
                _TAB_EX_(1) << "if (i) stream << \"\\t\";" << std::endl;

            if (original->TypeCat() == TypeCategory::Raw)
                _OUTS_ << _TAB_EX_(1) << "stream << data." << var->Name() << "[i];" << std::endl;
            else if (original->TypeCat() == TypeCategory::Enum)
                _OUTS_ << _TAB_EX_(1) << "stream << Enum::ToString(data." << var->Name() << "[i]);" << std::endl;
            else if (original->TypeCat() == TypeCategory::Struct)
                _OUTS_ << _TAB_EX_(1) << "sWrite(stream, data." << var->Name() << "[i]);" << std::endl;

            _TAB_ << "}" << std::endl;
        }

        if (i + 1 < vars->Size())
            _TAB_ << "stream << \"\\t\";" << std::endl;
    }
}

void CppExporter::JsonDeclare(const IStructType* sType, int tab)
{
    std::vector<std::string> path = utility::Absolute(sType);
    std::string tyName = utility::Contact(path, "::");

    _TAB_ << "void Write(std::ostream& stream, const " << tyName << "& data);" << std::endl;
    _TAB_ << "bool Load(const char* data, size_t size, " << tyName << "& out);" << std::endl;
}

void CppExporter::JsonLoader(const IStructType* sType, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(sType), "::");

    _OUTS_ <<
        _TAB_EX_(0) << "bool Load(const char* data, size_t size, " << tyName << "& out)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "if (data == nullptr || data[0] == 0)" << std::endl <<
        _TAB_EX_(2) << "return false;" << std::endl << std::endl <<
        _TAB_EX_(1) << "Json::Value node;" << std::endl <<
        _TAB_EX_(1) << "Json::Reader reader;" << std::endl <<
        _TAB_EX_(1) << "if (!reader.parse(std::string(data, size), node, false))" << std::endl <<
        _TAB_EX_(2) << "return false;" << std::endl << std::endl <<
        _TAB_EX_(1) << "if (node.type() != Json::ValueType::objectValue)" << std::endl <<
        _TAB_EX_(2) << "return false;" << std::endl << std::endl <<
        _TAB_EX_(1) << " return sLoad(node, out);" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;

}

void CppExporter::JsonLoaderStatic(const IStructType* sType, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(sType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "bool sLoad(const Json::Value& node, " << tyName << "& out)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl;

    if (sType->Inherited())
    {
        std::string baseName = utility::Contact(utility::Absolute(sType->Inherited()), "::");
        _OUTS_ << _TAB_EX_(1) << "sLoad(node, (" << baseName << "&)out);" << std::endl << std::endl;
    }

    const IVarSet* vars = sType->OwnScope()->VarSet();
    bool bnewLine = false;
    for (int i = 0; i < vars->Size(); ++i)
    {
        const IVariate* var = vars->Get(i);
        const IType* ty = var->Type();

        if (ty->TypeCat() == TypeCategory::Raw)
        {
            JsonLoadVar(var, (const IRawType*)ty, tab + 1);
        }
        else if (ty->TypeCat() == TypeCategory::Enum)
        {
            if (i) _OUTS_ << std::endl;
            JsonLoadVar(var, (const IEnumType*)ty, tab + 1);
        }
        else if (ty->TypeCat() == TypeCategory::Struct)
        {
            const IStructType* sTy = (const IStructType*)ty;
            JsonLoadVar(var, sTy, tab + 1);
        }
        else if (ty->TypeCat() == TypeCategory::Array)
        {
            if (i) _OUTS_ << std::endl;

            const IArrayType* aTy = (const IArrayType*)ty;
            const IType* original = aTy->Original();
            if (original->TypeCat() == TypeCategory::Raw)
            {
                JsonLoadVar(var, (const IRawType*)original, aTy->Length(), tab + 1);
            }
            else if (original->TypeCat() == TypeCategory::Enum)
            {
                JsonLoadVar(var, (const IEnumType*)original, aTy->Length(), tab + 1);
            }
            else if (original->TypeCat() == TypeCategory::Struct)
            {
                JsonLoadVar(var, (const IStructType*)original, aTy->Length(), tab + 1);
            }
        }
    }

    _OUTS_ <<
        _TAB_EX_(1) << "return true;" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::JsonLoadVar(const IVariate* var, const IRawType* rType, int tab)
{
    _OUTS_ <<
        _TAB_EX_(0) << "if (node.isMember(\"" << var->Name() << "\"))" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "const Json::Value& mem = node[\"" << var->Name() << "\"];" << std::endl;

    if (rType->RawCat() == RawCategory::Bool)
    {
        _OUTS_ <<
            _TAB_EX_(1) << "if (mem.type() == Json::ValueType::booleanValue)" << std::endl <<
            _TAB_EX_(2) << "out." << var->Name() << " = mem.asBool();" << std::endl;
    }
    else if (rType->RawCat() == RawCategory::Int)
    {
        _OUTS_ <<
            _TAB_EX_(1) << "if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)" << std::endl <<
            _TAB_EX_(2) << "out." << var->Name() << " = mem.asInt();" << std::endl;
    }
    else if (rType->RawCat() == RawCategory::Float)
    {
        _OUTS_ <<
            _TAB_EX_(1) << "if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)" << std::endl <<
            _TAB_EX_(2) << "out." << var->Name() << " = (float)mem.asInt();" << std::endl <<
            _TAB_EX_(1) << "else if (mem.type() == Json::ValueType::realValue)" << std::endl <<
            _TAB_EX_(2) << "out." << var->Name() << " = (float)mem.asDouble();" << std::endl;
    }
    else if (rType->RawCat() == RawCategory::String)
    {
        _OUTS_ <<
            _TAB_EX_(1) << "if (mem.type() == Json::ValueType::stringValue)" << std::endl <<
            _TAB_EX_(2) << "out." << var->Name() << " = mem.asString();" << std::endl;
    }

    _OUTS_ <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::JsonLoadVar(const IVariate* var, const IEnumType* rType, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(rType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "if (node.isMember(\"" << var->Name() << "\"))" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "const Json::Value& mem = node[\"" << var->Name() << "\"];" << std::endl <<
        _TAB_EX_(1) << "if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)" << std::endl <<
        _TAB_EX_(2) << "out." << var->Name() << " = (" << tyName << ")mem.asInt();" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::JsonLoadVar(const IVariate* var, const IStructType* rType, int tab)
{
    _OUTS_ <<
        _TAB_EX_(0) << "if (node.isMember(\"" << var->Name() << "\"))" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "const Json::Value& mem = node[\"" << var->Name() << "\"];" << std::endl <<
        _TAB_EX_(1) << "if (mem.type() == Json::ValueType::objectValue)" << std::endl <<
        _TAB_EX_(2) << "sLoad(mem, out." << var->Name() << ");" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::JsonLoadVar(const IVariate* var, const IRawType* rType, int length, int tab)
{
    _OUTS_ <<
        _TAB_EX_(0) << "if (node.isMember(\"" << var->Name() << "\"))" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "const Json::Value& mem = node[\"" << var->Name() << "\"];" << std::endl <<
        _TAB_EX_(1) << "if (mem.type() == Json::ValueType::arrayValue)" << std::endl <<
        _TAB_EX_(1) << "{" << std::endl;

    if (length > 0)
    {
        _OUTS_ << _TAB_EX_(2) << "for (int i = 0; i < std::min((int)mem.size(), (int)out." << var->Name() << ".size()); ++i)" << std::endl;
    }
    else
    {
        _OUTS_ <<
            _TAB_EX_(2) << "out." << var->Name() << ".resize(mem.size());" << std::endl <<
            _TAB_EX_(2) << "for (int i = 0; i < (int)mem.size(); ++i)" << std::endl;
    }
    _OUTS_ << _TAB_EX_(2) << "{" << std::endl;

    if (rType->RawCat() == RawCategory::Bool)
    {
        _OUTS_ <<
            _TAB_EX_(3) << "if (mem[i].type() == Json::ValueType::booleanValue)" << std::endl <<
            _TAB_EX_(4) << "out." << var->Name() << "[i]  = mem[i].asBool();" << std::endl;
    }
    else if (rType->RawCat() == RawCategory::Int)
    {
        _OUTS_ <<
            _TAB_EX_(3) << "if (mem[i].type() == Json::ValueType::intValue || mem[i].type() == Json::ValueType::uintValue)" << std::endl <<
            _TAB_EX_(4) << "out." << var->Name() << "[i]  = mem[i].asInt();" << std::endl;
    }
    else if (rType->RawCat() == RawCategory::Float)
    {
        _OUTS_ <<
            _TAB_EX_(3) << "if (mem[i].type() == Json::ValueType::intValue || mem[i].type() == Json::ValueType::uintValue)" << std::endl <<
            _TAB_EX_(4) << "out." << var->Name() << "[i]  = (float)mem[i].asInt();" << std::endl <<
            _TAB_EX_(3) << "else if (mem[i].type() == Json::ValueType::realValue)" << std::endl <<
            _TAB_EX_(4) << "out." << var->Name() << "[i]  = (float)mem[i].asDouble();" << std::endl;
    }
    else if (rType->RawCat() == RawCategory::String)
    {
        _OUTS_ <<
            _TAB_EX_(3) << "if (mem[i].type() == Json::ValueType::stringValue)" << std::endl <<
            _TAB_EX_(4) << "out." << var->Name() << "[i] = mem[i].asString();" << std::endl;
    }

    _OUTS_ <<
        _TAB_EX_(2) << "}" << std::endl <<
        _TAB_EX_(1) << "}" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::JsonLoadVar(const IVariate* var, const IEnumType* rType, int length, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(rType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "if (node.isMember(\"" << var->Name() << "\"))" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "const Json::Value& mem = node[\"" << var->Name() << "\"];" << std::endl <<
        _TAB_EX_(1) << "if (mem.type() == Json::ValueType::arrayValue)" << std::endl <<
        _TAB_EX_(1) << "{" << std::endl;

    if (length > 0)
    {
        _OUTS_ << _TAB_EX_(2) << "for (int i = 0; i < std::min((int)mem.size(), (int)out." << var->Name() << ".size()); ++i)" << std::endl;
    }
    else
    {
        _OUTS_ <<
            _TAB_EX_(2) << "out." << var->Name() << ".resize(mem.size());" << std::endl <<
            _TAB_EX_(2) << "for (int i = 0; i < (int)mem.size(); ++i)" << std::endl;
    }
    _OUTS_ << _TAB_EX_(2) << "{" << std::endl;

    _OUTS_ <<
        _TAB_EX_(3) << "if (mem[i].type() == Json::ValueType::intValue || mem[i].type() == Json::ValueType::uintValue)" << std::endl <<
        _TAB_EX_(4) << "out." << var->Name() << "[i] = (" << tyName << ")mem[i].asInt();" << std::endl;

    _OUTS_ <<
        _TAB_EX_(2) << "}" << std::endl <<
        _TAB_EX_(1) << "}" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::JsonLoadVar(const IVariate* var, const IStructType* rType, int length, int tab)
{
    _OUTS_ <<
        _TAB_EX_(0) << "if (node.isMember(\"" << var->Name() << "\"))" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "const Json::Value& mem = node[\"" << var->Name() << "\"];" << std::endl <<
        _TAB_EX_(1) << "if (mem.type() == Json::ValueType::arrayValue)" << std::endl <<
        _TAB_EX_(1) << "{" << std::endl;

    if (length > 0)
    {
        _OUTS_ << _TAB_EX_(2) << "for (int i = 0; i < std::min((int)mem.size(), (int)out." << var->Name() << ".size()); ++i)" << std::endl;
    }
    else
    {
        _OUTS_ <<
            _TAB_EX_(2) << "out." << var->Name() << ".resize(mem.size());" << std::endl <<
            _TAB_EX_(2) << "for (int i = 0; i < (int)mem.size(); ++i)" << std::endl;
    }
    _OUTS_ << _TAB_EX_(2) << "{" << std::endl;

    _OUTS_ <<
        _TAB_EX_(3) << "if (mem[i].type() == Json::ValueType::objectValue)" << std::endl <<
        _TAB_EX_(4) << "sLoad(mem[i], out." << var->Name() << "[i]);" << std::endl;

    _OUTS_ <<
        _TAB_EX_(2) << "}" << std::endl <<
        _TAB_EX_(1) << "}" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::JsonWriter(const IStructType* sType, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(sType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "void Write(std::ostream& stream, const " << tyName << "& data)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl <<
        _TAB_EX_(1) << "stream << \"{\" << std::endl;" << std::endl <<
        _TAB_EX_(1) << "sWrite(stream, data, 1);" << std::endl <<
        _TAB_EX_(1) << "stream << std::endl << \"}\" << std::endl;" << std::endl <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::JsonWriterStatic(const IStructType* sType, int tab)
{
    std::string tyName = utility::Contact(utility::Absolute(sType), "::");
    _OUTS_ <<
        _TAB_EX_(0) << "void sWrite(std::ostream& stream, const " << tyName << "& data, int tab)" << std::endl <<
        _TAB_EX_(0) << "{" << std::endl;

    const IVarSet* vars = sType->OwnScope()->VarSet();
    if (sType->Inherited())
    {
        std::string baseName = utility::Contact(utility::Absolute(sType->Inherited()), "::");
        _OUTS_ <<
            _TAB_EX_(1) << "sWrite(stream, (" << baseName << "&)data, tab);" << std::endl <<
            _TAB_EX_(1) << "stream << \",\" << std::endl;" << std::endl << std::endl;
    }

    for (int i = 0; i < vars->Size(); ++i)
    {
        const IVariate* var = vars->Get(i);
        const IType* ty = var->Type();
        _OUTS_ <<
            _TAB_EX_(1) << "stream << std::string(\" \", tab * 4) << \"\\\"" << var->Name() << "\\\":\";" << std::endl;

        if (ty->TypeCat() == TypeCategory::Raw)
        {
            if (static_cast<const IRawType*>(ty)->RawCat() == RawCategory::String)
                _OUTS_ << _TAB_EX_(1) << "stream << \"\\\"\" << data." << var->Name() << "<< \"\\\"\";" << std::endl;
            else
                _OUTS_ << _TAB_EX_(1) << "stream << data." << var->Name() << ";" << std::endl;
        }
        else if (ty->TypeCat() == TypeCategory::Enum)
        {
            _OUTS_ << _TAB_EX_(1) << "stream << (int)data." << var->Name() << ";" << std::endl;
        }
        else if (ty->TypeCat() == TypeCategory::Struct)
        {
            _OUTS_ <<
                _TAB_EX_(1) << "stream << \"{\" << std::endl;" << std::endl <<
                _TAB_EX_(1) << "sWrite(stream, data." << var->Name() << ", tab + 1);" << std::endl <<
                _TAB_EX_(1) << "stream << std::endl << std::string(\" \", tab * 4) << \"}\";" << std::endl;
        }
        else if (ty->TypeCat() == TypeCategory::Array)
        {
            const IArrayType* aTy = (const IArrayType*)ty;
            const IType* original = aTy->Original();

            _OUTS_ <<
                _TAB_EX_(1) << "stream << \"[\";" << std::endl <<
                _TAB_EX_(1) << "for (size_t i = 0; i < data." << var->Name() << ".size(); ++i)" << std::endl <<
                _TAB_EX_(1) << "{" << std::endl <<
                _TAB_EX_(2) << "if (i) stream << \",\";" << std::endl;

            if (original->TypeCat() == TypeCategory::Raw)
            {
                if (static_cast<const IRawType*>(ty)->RawCat() == RawCategory::String)
                    _OUTS_ << _TAB_EX_(2) << "stream << \"\\\"\" << data." << var->Name() << "[i] << \"\\\"\";" << std::endl;
                else
                    _OUTS_ << _TAB_EX_(2) << "stream << data." << var->Name() << "[i];" << std::endl;
            }
            else if (original->TypeCat() == TypeCategory::Enum)
            {
                _OUTS_ << _TAB_EX_(2) << "stream << (int)data." << var->Name() << "[i];" << std::endl;
            }
            else if (original->TypeCat() == TypeCategory::Struct)
            {
                _OUTS_ <<
                    _TAB_EX_(2) << "stream << \"{\" << std::endl;" << std::endl <<
                    _TAB_EX_(2) << "sWrite(stream, data." << var->Name() << "[i], tab + 1);" << std::endl <<
                    _TAB_EX_(2) << "stream << std::endl << std::string(\" \", tab * 4) << \"}\";" << std::endl;
            }

            _OUTS_ <<
                _TAB_EX_(1) << "}" << std::endl <<
                _TAB_EX_(1) << "stream << \"]\";" << std::endl;

        }

        if (i + 1 < vars->Size())
            _OUTS_ << _TAB_EX_(1) << "stream << \",\" << std::endl;" << std::endl;
    }
    _OUTS_ <<
        //_TAB_EX_(1) << "stream << std::endl;" <<
        _TAB_EX_(0) << "}" << std::endl;
}

void CppExporter::GetDepends(const IStructType* sType, std::vector<const IStructType*>& deps)
{
    if (deps.end() != std::find(deps.begin(), deps.end(), sType))
        return;

    if (sType->Inherited())
        GetDepends(sType->Inherited(), deps);

    const IVarSet* vars = sType->OwnScope()->VarSet();
    for (int i = 0; i < vars->Size(); ++i)
    {
        const IType* type = vars->Get(i)->Type();

        if (type->TypeCat() == TypeCategory::Struct)
        {
            GetDepends(static_cast<const IStructType*>(type), deps);
        }
        else if (type->TypeCat() == TypeCategory::Array)
        {
            const IArrayType* aType = static_cast<const IArrayType*>(type);
            if (aType->Original()->TypeCat() == TypeCategory::Struct)
                GetDepends(static_cast<const IStructType*>(aType->Original()), deps);
        }
    }

    assert(deps.end() == std::find(deps.begin(), deps.end(), sType));
    deps.push_back(sType);
}

GCF_NAMESPACE_END
