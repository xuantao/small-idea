#include "Context.h"
#include <stdio.h>
#include <cassert>
#include "Driver.h"
#include "TypeDecl.h"
#include "ValueDecl.h"
#include "Variate.h"
#include "Utility.h"

CFG_NAMESPACE_BEGIN

Context::Context()
    : _driver(nullptr)
    , _scope(nullptr)
    , _var(nullptr)
    , _type(nullptr)
{
    _rawTypes[TYPE_BOOL] = new RawType(TYPE_BOOL);
    _rawTypes[TYPE_INT] = new RawType(TYPE_INT);
    _rawTypes[TYPE_FLOAT] = new RawType(TYPE_FLOAT);
    _rawTypes[TYPE_STRING] = new RawType(TYPE_STRING);

    _scope = new ScopeType("");
}

Context::~Context()
{
    delete _scope;
    _scope = nullptr;

    for (auto it = _rawTypes.begin(); it != _rawTypes.end(); ++it)
        delete it->second;
    _rawTypes.clear();
}

IType* Context::GetType(const std::string& name) const
{
    IType* type = _scope->GetType(name);
    if (type == nullptr)
    {
        auto it = _rawTypes.find(name);
        if (it != _rawTypes.cend())
            type = it->second;
    }

    return type;
}

void Context::OnParseBegin(Driver& driver, const std::string& file)
{
    _driver = &driver;
    printf("Start Parse File %s\n", file.c_str());
}

void Context::OnParseEnd()
{
    _driver = nullptr;
}

void Context::OnIncludeBegin(const std::string& file)
{
    printf("Include Begin %s\n", file.c_str());
}

void Context::OnIncludeEnd()
{
    printf("Include End\n");
}

void Context::OnPredefine(const std::string& name)
{
    printf("struct %s;\n", name.c_str());
}

void Context::OnStructBegin(const std::string& name)
{
    printf("struct %s{\n", name.c_str());
    assert(_type == nullptr);
    if (_rawTypes.find(name) != _rawTypes.end())
    {
        printf("struct redefine %s\n", name.c_str());
        _type = new StructType(GenCrashName(name));
    }
    else
    {
        _type = new StructType(name);
    }

    _rawTypes[_type->Name()] = _type;
}

void Context::OnInherit(const std::string& name)
{
    printf("  inherit:%s\n", name.c_str());

    if (_type == nullptr || _type->Category() != TypeCategory::Struct)
    {
        printf("current type is not struct\n");
        return;
    }

    auto it = _rawTypes.find(name);
    if (it == _rawTypes.end())
    {
        printf("inherit struct is not define :%s\n", name.c_str());
        return;
    }

    _struct->Inherit(it->second);
}

void Context::OnStructEnd()
{
    printf("}\n");
    //_declTyps.insert(std::make_pair(_type->Name(), _type));
    _type = nullptr;
}

void Context::OnEnumBegin(const std::string& name)
{
    printf("enum %s {\n", name.c_str());
    assert(_type == nullptr);
    if (_rawTypes.find(name) != _rawTypes.end())
    {
        printf("enum redefine %s\n", name.c_str());
        _type = new EnumType(GenCrashName(name));
    }
    else
    {
        _type = new EnumType(name);
    }

    _rawTypes[_type->Name()] = _type;
}

void Context::OnEnumMember(const std::string& name, const std::string& value/* = ""*/)
{
    if (value.empty())
        printf("%s\n", name.c_str());
    else
        printf("%s=%s\n", name.c_str(), value.c_str());

    if (_type == nullptr || _type->Category() != TypeCategory::Enum)
    {
        printf("current parse type is not enum\n");
        return;
    }

    int nVar = 0;
    Variate* var = new Variate(_enum, _enum, name);
    if (!value.empty() && util::Convert(value, nVar, 0))
        var->BindValue(new IntValue(nVar));

    _enum->AddEnum(var);
}

void Context::OnEnumMemberRef(const std::string& name, const std::string& value)
{
    if (value.empty())
        printf("%s\n", name.c_str());
    else
        printf("%s=%s\n", name.c_str(), value.c_str());

    if (_type == nullptr || _type->Category() != TypeCategory::Enum)
    {
        printf("current parse type is not enum\n");
        return;
    }

    int nVar = 0;
    Variate* var = new Variate(_enum, _enum, name);
    if (!value.empty() && util::Convert(value, nVar, 0))
        var->BindValue(new IntValue(nVar));

    _enum->AddEnum(var);
}

void Context::OnEnumEnd()
{
    printf("}\n");
    _type = nullptr;
}

void Context::OnVariateBegin(const std::string& type, const std::string& name)
{
    printf("%s %s", type.c_str(), name.c_str());
    IType* varType = GetType(type);
    if (varType == nullptr)
    {
        printf("variate type is not define :%s\n", type.c_str());
        return;
    }

    if (varType->Category() == TypeCategory::Struct && varType == _struct)
    {
        printf("struct is not define completed: %s\n", type.c_str());
        return;
    }

    if (_type == nullptr)
    {
        if (_globalVars.find(name) != _globalVars.end())
        {
            printf("define global var name crash %s\n", name.c_str());
            return;
        }

        _var = new Variate(nullptr, varType, name);
        _globalVars[name] = _var;
    }
    else if (_type->Category() == TypeCategory::Struct)
    {
        if (_struct->Contain(name))
        {
            printf("struct member already own member:%s\n", name.c_str());
            return;
        }

        _var = new Variate(_struct, varType, name);
        _struct->AddVar(_var);
    }
    else
    {
        printf("unknown scope to define variate %s %s;\n", type.c_str(), name.c_str());
    }
}

void Context::OnVariateValue(const std::string& value)
{
    printf(" = %s", value.c_str());
    if (_var == nullptr)
    {
        printf("current state is not define a variate\n");
        return;
    }

    //if (var)
    //    var->SetValue(new StringValue());
}

void Context::OnVariateValueRef(const std::string& value)
{
    printf(" = %s", value.c_str());
    //if (var)
    //    var->SetValue(new StringValue());
}

void Context::OnVariateArray(const std::string& length/* = ""*/)
{
    printf("[%s]", length.c_str());
    if (_var == nullptr)
    {
        printf("current state is not define a variate\n");
        return;
    }

    int len = -1;
    if (!util::Convert(length, len, -1))
        printf("parse int failed :%s\n", length.c_str());

    _var->UpgradeArray(len);
}

void Context::OnVariateEnd()
{
    printf(";\n");
    _var = nullptr;
}

std::string Context::GenCrashName(const std::string& name) const
{
    int index = 0;
    std::string ret = name + "_crash";

    while (_rawTypes.find(ret) != _rawTypes.end())
        ret = name + "_crash_" + std::to_string(++index);

    return ret;
}
//
//IValue* Context::CreateValue(const IType* type, const std::string& value) const
//{
//    if (type->Category() == TypeCategory::Raw)
//    {
//        if (type->Name() == TYPE_BOOL)
//            return CreateValueBool(type, value);
//        else if (type->Name() == TYPE_INT)
//            return CreateValueInt(type, value);
//        else if (type->Name() == TYPE_FLOAT)
//            return CreateValueFloat(type, value);
//        else if (type->Name() == TYPE_STRING)
//            return CreateValueString(type, value);
//        else
//            assert(false);
//    }
//    else if (type->Category() == TypeCategory::Enum)
//    {
//        //TODO:
//    }
//    else
//    {
//        printf("parse value failed\n");
//    }
//    return nullptr;
//}
//
//IValue* Context::CreateValueBool(const IType* type, const std::string& value) const
//{
//    bool v = false;
//    if (!util::Str2Bool(value, v, false))
//    {
//        printf("parse bool value failed:%s\n", value.c_str());
//        return nullptr;
//    }
//    return new BoolValue(type, v);
//}
//
//IValue* Context::CreateValueInt(const IType* type, const std::string& value) const
//{
//    int v = 0;
//    if (!util::Str2Int(value, v, 0))
//    {
//        printf("parse int value failed:%s\n", value.c_str());
//        return nullptr;
//    }
//    return new IntValue(type, v);
//}
//
//IValue* Context::CreateValueFloat(const IType* type, const std::string& value) const
//{
//    float v = 0.0f;
//    if (!util::Str2Float(value, v))
//    {
//        printf("parse float value failed:%s\n", value.c_str());
//        return nullptr;
//    }
//    return new FloatValue(type, v);
//}
//
//IValue* Context::CreateValueString(const IType* type, const std::string& value) const
//{
//    return new StringValue(type, value);
//}

const IVariate* Context::FindVarRef(const IType* scope, const std::string& path) const
{
    std::vector<std::string> ps = util::Split(path, '.');

    IType* type = _scope;
    for (int i = 0; i < (int)ps.size() - 1; ++i)
    {
        //type = _scope->GetType(ps[i]);
        if (type->Category() == TypeCategory::Raw)
            return nullptr;

        //if (type->Category() == TypeCategory::Struct)
        //    ((StructType*)type)->
        {
        }
    }

    if (type == nullptr)
    {
    }
    return nullptr;
}

CFG_NAMESPACE_END
