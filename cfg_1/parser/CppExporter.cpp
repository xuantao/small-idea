#include "CppExporter.h"
#include "Type.h"
#include "Value.h"
#include "Utility.h"
#include <iostream>
#include <sstream>

CFG_NAMESPACE_BEGIN


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

CppExporter::CppExporter(const ScopeType* global, std::ostream& stream)
    : _global(global)
    , _stream(stream)
    , _tab(0)
{
}

CppExporter::~CppExporter()
{

}

bool CppExporter::Export()
{
    std::cout << "begin export c++" << std::endl;

    _scope.push(_global);
    Export(_global);
    _scope.pop();

    std::cout << "end export c++" << std::endl;
    return true;
}

bool CppExporter::Export(const EnumType* ty)
{
    Tab();
    _stream << "enum " << ty->Name() << " {" << std::endl;
    ++_tab;

    const IVarSet* vars = ty->VarSet();
    for (int i = 0; i < vars->Size(); ++i)
    {
        VarData data;
        Trans(vars->Get(i), data);

        Tab();
        _stream << data.name;
        if (!data.value.empty())
            _stream << " = " << data.value;
        _stream << std::endl;
    }

    --_tab;
    Tab();
    _stream << "};" << std::endl;
    return true;
}

bool CppExporter::Export(const StructType* ty)
{
    Tab();
    _stream << "struct " << ty->Name();
    if (ty->Inherited())
        _stream << " : " << ty->Inherited()->Name() << " {";
    _stream << std::endl;

    ++_tab;

    const IVarSet* vars = ty->OwnVars();
    for (int i = 0; i < vars->Size(); ++i)
    {
        VarData data;
        Trans(vars->Get(i), data);

        Tab();
        _stream << data.type << " " << data.name;
        if (!data.value.empty())
            _stream << " = " << data.value;
        _stream << std::endl;
    }

    --_tab;
    Tab();
    _stream << "};" << std::endl;
    return true;
}

bool CppExporter::Export(const ScopeType* ty)
{
    IVarSet* vars = ty->VarSet();
    for (int i = 0; i < vars->Size(); ++i)
    {
        VarData data;
        Trans(vars->Get(i), data);

        Tab();
        _stream << data.type << " " << data.name;
        if (!data.value.empty())
            _stream << " = " << data.value;
        _stream << std::endl;
    }

    ITypeSet* types = ty->TypeSet();
    for (int i = 0; i < types->Size(); ++i)
    {
        const IType* type = types->Get(i);
        int tab = _tab++;
        _scope.push(type);

        if (type->Category() == TypeCategory::Enum)
            Export(static_cast<const EnumType*>(type));
        else if (type->Category() == TypeCategory::Struct)
            Export(static_cast<const StructType*>(type));
        else if (type->Category() == TypeCategory::Scope)
            Export(static_cast<const ScopeType*>(type));

        _scope.pop();
        _tab = tab;
    }

    return true;
}

void CppExporter::Trans(const IVariate* var, VarData& data)
{
    const IType* type = var->Type();
    if (type->Category() == TypeCategory::Raw)
    {
        data.type = type->Name();
    }
    else if (type->Category() == TypeCategory::Array)
    {
        data.type = static_cast<const ArrayType*>(type)->Original()->Name();
    }
    else
    {
        const auto ralative = util::Relative(type, _scope.top());
        data.type = util::Contact(ralative, "::");
    }

    data.name = var->Name();

    const IValue* value = var->Value();
    if (value == nullptr)
        return;

    if (value->Category() == ValueCategory::Raw)
        data.value = TransValue(static_cast<const RawValue*>(value));
    else if (value->Category() == ValueCategory::Ref)
        data.value = TransValue(var);
    else
        assert(false);
}

void CppExporter::Tab()
{
    _stream << std::string(_tab * 4, ' ');
}
CFG_NAMESPACE_END
