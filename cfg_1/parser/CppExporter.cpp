#include "CppExporter.h"
#include "Type.h"
#include "Value.h"
#include "Utility.h"
#include "CppStruct.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <set>

CFG_NAMESPACE_BEGIN

#define  TAB() Tab(tab)

static std::string TransValue(const RawValue* val)
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

static std::string TransValue(const IVariate* var)
{
    const RefValue* value = static_cast<const RefValue*>(var->Value());
    const IVariate* ref = value->Var();
    std::stringstream stream;
    if (var->Type() != ref->Type())
        stream << "(" << var->Type()->Name() << ")";

    const std::vector<std::string> ralative = util::Relative(var->Belong(), ref->Type());
    stream << util::Contact(ralative, "::");
    if (!ralative.empty())
        stream << "::";
    stream << ref->Name();
    return stream.str();
}

CppExporter::CppExporter(std::ostream& stream, const ScopeType* global)
    : _stream(stream)
    , _global(global)
    , _tab(0)
{
}

CppExporter::~CppExporter()
{

}

bool CppExporter::Export(const IFileData* file)
{
    std::cout << "begin export c++" << std::endl;

    _scope.push(_global);
    file->Traverse(this);
    _scope.pop();

    std::cout << "end export c++" << std::endl;
    return true;
}

bool CppExporter::Declare(const EnumType* ty, int tab)
{
    Tab(tab) << "enum " << ty->Name() << std::endl;

    Tab(tab) << "{" << std::endl;

    ++tab;
    const IVarSet* vars = ty->VarSet();
    for (int i = 0; i < vars->Size(); ++i)
    {
        CppVarData data;
        cpp_util::Convert(vars->Get(i), data);

        Tab(tab) << data.name;
        if (!data.value.empty())
            _stream << " = " << data.value;
        _stream << std::endl;
    }
    --tab;

    Tab(tab) << "};" << std::endl;
    return true;
}

bool CppExporter::Declare(const StructType* sType, int tab)
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
    Tab(tab) << "struct " << sType->Name();
    if (sType->Inherited())
        _stream << " : public " << cpp_util::TypeName(sType, sType->Inherited());
    _stream << std::endl;

    Tab(tab) << "{" << std::endl;
    ++tab;

    /* const values */
    for (size_t i = 0; i < vars.size(); i++)
    {
        const CppVarData& data = vars[i];
        if (!data.var->IsConst())
            continue;
        ++nConst;
        Tab(tab) << "static const " << data.type << " " << data.name << " = " << data.value << ";" << std::endl;
    }

    if (nConst)
        _stream << std::endl;

    /* constructor */
    if (constructor)
    {
        Tab(tab) << sType->Name() << "()" << std::endl;
        ++tab;

        bool first = true;
        for (size_t i = 0; i < vars.size(); i++)
        {
            const CppVarData& data = vars[i];
            if (data.var->IsConst())
                continue;

            const IType* type = data.var->Type();
            if (type->Category() != TypeCategory::Raw && data.value.empty())
                continue;

            Tab(tab);
            if (first)
                _stream << ": ";
            else
                _stream << ", ";

            _stream << data.name << "(";

            if (data.value.empty())
            {
                assert(type->Category() == TypeCategory::Raw);
                _stream << cpp_util::DefValue(static_cast<const RawType*>(type)->Raw());
            }
            else
            {
                _stream << data.value;
            }

            _stream << ")" << std::endl;
            first = false;
        }

        --tab;
        Tab(tab) << "{ }" << std::endl << std::endl;
    }

    /* member data */
    for (size_t i = 0; i < vars.size(); i++)
    {
        const CppVarData& data = vars[i];
        if (data.var->IsConst())
            continue;

        Tab(tab) << "" << data.type << " " << data.name << ";" << std::endl;
    }

    --tab;
    Tab(tab) << "};" << std::endl;

    return true;
}

void CppExporter::OnVariate(const IVariate* var)
{
    if (var == nullptr)
        return;

    CppVarData data;
    if (!cpp_util::Convert(var, data))
        return;

    if (var->IsConst())
        _stream << "const ";
    _stream << data.type << " " << data.name;
    if (!data.value.empty())
        _stream << " = " << data.value;
    _stream << ";" << std::endl;
}

void CppExporter::OnType(const IType* type)
{
    if (type->Category() == TypeCategory::Enum)
    {
        Declare(static_cast<const EnumType*>(type), _tab);
        EnumStaticData(static_cast<const EnumType*>(type), _tab);
        Enum2String(static_cast<const EnumType*>(type), _tab);
        String2Enum(static_cast<const EnumType*>(type), _tab);
    }
    else if (type->Category() == TypeCategory::Struct)
    {
        Declare(static_cast<const StructType*>(type), _tab);
        TabLoader(static_cast<const StructType*>(type), _tab);
    }
    else
    {
        std::cerr << "does not support export type:" << type->Name() << std::endl;
        ERROR_NOT_ALLOW;
    }
}

bool CppExporter::TabLoader(const StructType* sType, int tab)
{
    Tab(tab) << "bool Load(ITabFile* pTab, int line, " << sType->Name() << "& data)" << std::endl;

    Tab(tab) << "{" << std::endl;
    ++tab;

    TabLoadStruct(sType, util::EMPTY_STR, tab);

    --tab;
    Tab(tab) << "}" << std::endl;
    return true;
}

bool CppExporter::TabLoadStruct(const StructType* sType, const std::string& owner, int tab)
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
            TabLoadRaw(var, name, tab);
        else if (typeCategory == TypeCategory::Enum)
            TabLoadEnum(var, name, tab);
        else if (typeCategory == TypeCategory::Array)
            TabLoadArray(var, name, tab);
        else if (typeCategory == TypeCategory::Struct)
            TabLoadStruct(static_cast<const StructType*>(var->Type()), name, tab);
        else
            ERROR_NOT_ALLOW;
    }
    return true;
}

bool CppExporter::TabLoadRaw(const IVariate* var, const std::string& name, int tab)
{
    const RawType* type = static_cast<const RawType*>(var->Type());

    Tab(tab);
    switch (type->Raw())
    {
    case RawCategory::Bool:
        _stream << "pTab->GetBoolean(line, \"" << name << "\" , false, " << name << ");" << std::endl;
        break;
    case RawCategory::Int:
        _stream << "pTab->GetInteger(line, \"" << name << "\" , false, &" << name << ");" << std::endl;
        break;
    case RawCategory::Float:
        _stream << "pTab->GetFloat(line, \"" << name << "\" , false, &" << name << ");" << std::endl;
        break;
    case RawCategory::String:
        {
            Tab(tab) << "{" << std::endl;
            ++tab;
            Tab(tab) << "char buffer[1024] = {0};" << std::endl;

            Tab(tab) << "pTab->GetString(line, \"" << name << "\" , \"\", buffer, 1024);" << std::endl;

            Tab(tab) << name << " = buffer;" << std::endl;

            --tab;
            Tab(tab) << "}" << std::endl;
        }
        break;
    }
    return true;
}

bool CppExporter::TabLoadEnum(const IVariate* var, const std::string& name, int tab)
{
    Tab(tab) << "pTab->GetInteger(line, \"" << name << "\" , false, &" << name << ");" << std::endl;
    return true;
}

bool CppExporter::TabLoadArray(const IVariate* var, const std::string& name, int tab)
{
    const ArrayType* type = static_cast<const ArrayType*>(var->Type());
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

        const StructType* sType = static_cast<const StructType*>(orignal);
        for (int i = 0; i < type->Length(); ++i)
            TabLoadStruct(sType, name + "_" + std::to_string(i + 1), tab);
    }
    else if (orignal->Category() == TypeCategory::Enum)
    {
        Tab(tab) << "{" << std::endl;
        ++tab;

        Tab(tab) << "std::vector<std::string> vecStr;" << std::endl;
        Tab(tab) << "char buffer[1024] = {0};" << std::endl;
        Tab(tab) << "pTab->GetString(line, \"" << name << "\" , \"\", buffer, 1024);" << std::endl;
        Tab(tab) << "vecStr = KGTool::Split(buffer, ',');" << std::endl;

        //TODO:

        --tab;
        Tab(tab) << "}" << std::endl;
    }
    else if (orignal->Category() == TypeCategory::Raw)
    {
        Tab(tab) << "{" << std::endl;
        ++tab;

        Tab(tab) << "std::vector<std::string> vecStr;" << std::endl;
        Tab(tab) << "char buffer[1024] = {0};" << std::endl;
        Tab(tab) << "pTab->GetString(line, \"" << name << "\" , \"\", buffer, 1024);" << std::endl;
        Tab(tab) << "vecStr = KGTool::Split(buffer, ',');" << std::endl;

        //TODO:

        --tab;
        Tab(tab) << "}" << std::endl;
    }
    return true;
}

bool CppExporter::EnumStaticData(const EnumType* eType, int tab)
{
    std::vector<std::string> path = util::Absolute(eType);
    std::string base = util::Contact(path, "_");
    std::string varName = "s_" + base + "_val";
    std::string strName = "s_" + base + "_str";

    const IVarSet* varSet = eType->VarSet();

    TAB() << "static const int " << varName << "[] = {" << std::endl;
    ++tab;

    TAB();
    for (int i = 0; i < varSet->Size(); ++i)
    {
        _stream << "" << cpp_util::OrignalValue(varSet->Get(i)->Value()) << ", ";
        if (i && (i % 8) == 0)
        {
            _stream << std::endl;
            TAB();
        }
    }
    _stream << "-111111";

    --tab;
    _stream << " }; // end of enum " << eType->Name() << std::endl;

    TAB() << "static const char* const  " << strName << "[] = {" << std::endl;
    ++tab;

    TAB();
    for (int i = 0; i < varSet->Size(); ++i)
    {
        _stream << "\"" << varSet->Get(i)->Name() << "\", ";
        if (i && (i % 8) == 0)
        {
            _stream << std::endl;
            TAB();
        }
    }
    _stream << "nullptr";

    --tab;
    _stream << " }; // end of enum " << eType->Name() << std::endl;

    return true;
}

bool CppExporter::Enum2String(const EnumType* eType, int tab)
{
    std::vector<std::string> path = util::Absolute(eType);
    std::string base = util::Contact(path, "_");
    std::string varName = "s_" + base + "_val";
    std::string strName = "s_" + base + "_str";

    TAB() << "const char* ToString(" << util::Contact(path, "::") << " value)" << std::endl;
    TAB() << "{" << std::endl;
    ++tab;

    TAB() << "for (int i = 0; " << strName << "[i]; ++i)" << std::endl;
    TAB() << "{" << std::endl;
    ++tab;

    TAB() << "if (" << varName << "[i] == value)" << std::endl;
    Tab(tab + 1) << "return " << strName << "[i];" << std::endl;

    --tab;
    TAB() << "}" << std::endl << std::endl;
    TAB() << "return nullptr;" << std::endl;

    --tab;
    TAB() << "}" << std::endl;
    return true;
}

bool CppExporter::String2Enum(const EnumType* eType, int tab)
{
    std::vector<std::string> path = util::Absolute(eType);
    std::string base = util::Contact(path, "_");
    std::string varName = "s_" + base + "_val";
    std::string strName = "s_" + base + "_str";

    TAB() << "bool ToEnum(" << util::Contact(path, "::") << "& out, const char* name)" << std::endl;
    TAB() << "{" << std::endl;
    ++tab;

    TAB() << "for (int i = 0; " << strName << "[i]; ++i)" << std::endl;
    TAB() << "{" << std::endl;
    ++tab;

    TAB() << "if (std::strcmp(" << strName << "[i], name) == 0)" << std::endl;
    TAB() << "{" << std::endl;
    ++tab;

    TAB() << "out = " << "(" << util::Contact(path, "::") << ")" << varName << "[i];" << std::endl;
    TAB() << "return true;" << std::endl;

    --tab;
    TAB() << "}" << std::endl;

    --tab;
    TAB() << "}" << std::endl << std::endl;

    TAB() << "return false;" << std::endl;
    --tab;
    TAB() << "}" << std::endl;
    return true;
}

std::ostream& CppExporter::Tab(int tab)
{
    return util::Tab(_stream, tab);
}

CFG_NAMESPACE_END
