#include "CppStruct.h"
#include "Type.h"
#include "Value.h"
#include "Utility.h"
#include <set>
#include <iostream>

CFG_NAMESPACE_BEGIN


CppStruct::CppStruct(const StructType* type)
    : _struct(type)
    , _contructor(false)
{
}

CppStruct::~CppStruct()
{

}

bool CppStruct::Declare(std::ostream& stream, int tab/* = 0*/)
{
    util::Tab(stream, tab);
    stream << "struct " << _struct->Name();
    if (_struct->Inherited())
        stream << " : public " << cpp_util::TypeName(_struct, _struct->Inherited());
    stream << std::endl;

    util::Tab(stream, tab);
    stream << "{" << std::endl;

    ++tab;
    if (_contructor)
    {
        bool first = true;

        util::Tab(stream, tab);
        stream << _struct->Name() << "()" << std::endl;
        ++tab;
        for (size_t i = 0; i < _vars.size(); i++)
        {
            const CppVarData& data = _vars[i];
            if (data.value.empty())
                continue;

            util::Tab(stream, tab);
            if (first)
                stream << ": ";
            else
                stream << ", ";
            stream << data.name << "(" << data.value << ")" << std::endl;

            first = false;
        }

        --tab;
        util::Tab(stream, tab);
        stream << "{ }" << std::endl << std::endl;
    }

    for (size_t i = 0; i < _consts.size(); i++)
    {
        const CppVarData& data = _consts[i];
        util::Tab(stream, tab);
        stream << "static const " << data.type << " " << data.name << " = " << data.value << ";" << std::endl;
    }

    if (!_consts.empty())
        stream << std::endl;

    for (size_t i = 0; i < _vars.size(); i++)
    {
        const CppVarData& data = _vars[i];
        util::Tab(stream, tab);
        stream << "" << data.type << " " << data.name << ";" << std::endl;
    }

    --tab;
    util::Tab(stream, tab);
    stream << "};" << std::endl;
    return true;
}

bool CppStruct::Json(std::ostream& stream, const std::string& owner, int tab/* = 0*/)
{
    std::set<std::string> allVars;
    std::set<std::string> conficts;

    const IVarSet* varSet = _struct->VarSet();
    for (int i = 0; i < varSet->Size(); ++i)
    {
        const IVariate* var = varSet->Get(i);
        if (var->IsConst())
            continue;

        if (allVars.find(var->Name()) != allVars.end())
            conficts.insert(var->Name());
        else
            allVars.insert(var->Name());
    }

    for (int i = 0; i < varSet->Size(); ++i)
    {
        const IVariate* var = varSet->Get(i);
        if (var->IsConst())
            continue;

        std::string name;
        if (conficts.find(var->Name()) != conficts.end())
        {
            if (owner.empty())
                name = var->Belong()->Name() + "::" + var->Name();
            else
                name = owner + "." + var->Belong()->Name() + "::" + var->Name();
        }
        else
        {
            if (owner.empty())
                name = var->Name();
            else
                name = owner + "." + var->Name();
        }

        Json(stream, var, name, tab);

        if (i + 1 == varSet->Size())
            stream << std::endl;
        else
            stream << "," << std::endl;
    }

    return true;
}

bool CppStruct::Json(std::ostream& stream, const IVariate* var, const std::string& name, int tab)
{
    const IType* type = var->Type();
    const IValue* value = var->Value();

    if (type->Category() == TypeCategory::Struct)
    {
        const StructType* sType = static_cast<const StructType*>(type);
        //TODO: 这里是可以优化的
        CppStruct cpp(sType);
        cpp.Resolve();
        cpp.Json(stream, name, tab);
        return true;
    }

    if (type->Category() == TypeCategory::Raw)
    {
        util::Tab(stream, tab);
        stream << "\"" << name << "\" = {" << std::endl;
        ++tab;

        const RawType* rawType = static_cast<const RawType*>(type);
        util::Tab(stream, tab);
        stream << "\"type\" = " << "\"" << type->Name() << "\"," << std::endl;

        util::Tab(stream, tab);
        stream << "\"default\" = \"";
        if (value == nullptr)
            stream << cpp_util::DefValue(rawType->Raw());
        else
            stream << cpp_util::OrignalValue(value);
        stream << "\"" << std::endl;

        --tab;
        util::Tab(stream, tab);
        stream << "}";
    }
    else if (type->Category() == TypeCategory::Enum)
    {
        util::Tab(stream, tab);
        stream << "\"" << name << "\" = {" << std::endl;
        ++tab;

        util::Tab(stream, tab);
        stream << "\"type\" = " << "\"enum\"," << std::endl;

        util::Tab(stream, tab);
        stream << "\"enum\" = " << "\"" <<
            util::Contact(util::Relative(var->Belong(), type), ".") << "\"," << std::endl;

        util::Tab(stream, tab);
        stream << "\"default\" = \"";
        if (value == nullptr)
            stream << cpp_util::DefValue(RawCategory::Int);
        else
            stream << cpp_util::OrignalValue(value);
        stream << "\"" << std::endl;

        --tab;
        util::Tab(stream, tab);
        stream << "}";
    }
    else if (type->Category() == TypeCategory::Array)
    {
        const ArrayType* arType = static_cast<const ArrayType*>(type);
        const IType* original = arType->Original();
        if (original != arType->Prev())
        {
            std::cerr << "not support 多维数组" << std::endl;
            return false;
        }

        util::Tab(stream, tab);
        stream << "\"" << name << "\" = {" << std::endl;
        ++tab;

        util::Tab(stream, tab);
        stream << "\"type\" = " << "\"enum\"," << std::endl;

        util::Tab(stream, tab);
        stream << "\"enum\" = " << "\"" <<
            util::Contact(util::Relative(var->Belong(), type), ".") << "\"," << std::endl;

        if (value != nullptr)
        {
            util::Tab(stream, tab);
            stream << "\"default\" = \"";
            stream << cpp_util::OrignalValue(value);
            stream << "\"" << std::endl;
        }

        --tab;
        util::Tab(stream, tab);
        stream << "}";
    }
    else
    {
        ERROR_NOT_ALLOW;
    }


    return true;
}

bool CppStruct::Resolve()
{
    const IVarSet* set = _struct->OwnVars();
    for (int i = 0; i < set->Size(); ++i)
    {
        CppVarData data;
        const IVariate* var = set->Get(i);
        if (cpp_util::Convert(var, data))
        {
            if (var->IsConst())
                _consts.push_back(data);
            else
                _vars.push_back(data);

            if (!_contructor && !data.value.empty())
                _contructor = true;
        }
        else
        {
            //TODO: error
        }
    }
    return true;
}


//@
CFG_NAMESPACE_END
