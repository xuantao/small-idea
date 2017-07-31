#include "Context.h"
#include <stdio.h>
#include <cassert>
#include "Driver.h"
#include "Type.h"
#include "Value.h"
#include "ValueUtil.h"
#include "Variate.h"
#include "Utility.h"
#include "FileData.h"

CFG_NAMESPACE_BEGIN

static std::ostream& operator << (std::ostream& out, RawCategory raw)
{
    return out;
}

Context::Context()
    : _driver(nullptr)
    , _scope(nullptr)
    , _var(nullptr)
    , _type(nullptr)
{
    _scope = new ScopeType("");
    _scope->TypeSet()->Add(new RawType(TYPE_BOOL, RawCategory::Bool));
    _scope->TypeSet()->Add(new RawType(TYPE_INT, RawCategory::Int));
    _scope->TypeSet()->Add(new RawType(TYPE_FLOAT, RawCategory::Float));
    _scope->TypeSet()->Add(new RawType(TYPE_STRING, RawCategory::String));
}

Context::~Context()
{
    delete _scope;
    _scope = nullptr;

    delete _var;
    _var = nullptr;
}

const IScopeType* Context::Global() const
{
    return _scope;
}

IType* Context::GetType(const std::string& name) const
{
    IType* type = _scope->TypeSet()->Get(name);
    return type;
}

bool Context::Export(IExporter* expoter, const std::string& path, const std::string& file, bool merge)
{
    if (merge)
    {
        std::string fileName = utility::TrimFileSuffix(file, '.');
        if (fileName.empty() || fileName.back() == '/' || fileName.back() == '//')
            fileName = "unnamed";

        expoter->OnBegin(_scope, path, fileName);
        for (auto it = _files.begin(); it != _files.end(); ++it)
            (*it)->Export(expoter, merge);
        expoter->OnEnd();
    }
    else
    {
        //std::string path;
        //if (file.back() == '\\' || file.back() == '/')
        //    path = file;
        //else
        //    path = file + '/';

        //for (auto it = _files.begin(); it != _files.end(); ++it)
        //{
        //    expoter->OnBegin(_scope, path, path + utility::TrimFileSuffix((*it)->File(), '.'));
        //    (*it)->Export(expoter, merge);
        //    expoter->OnEnd();
        //}
    }

    return true;
}

void Context::OnParseBegin(Driver& driver, const std::string& file)
{
    _driver = &driver;

    FileData* fd = new FileData(file);
    _files.push_back(fd);
    _stackFile.push_back(fd);
}

void Context::OnParseEnd()
{
    _driver = nullptr;
    _stackFile.pop_back();
}

void Context::OnIncludeBegin(const std::string& file)
{
    auto it = std::find_if(_files.begin(), _files.end(),
        [&file](FileData* fd) { return fd->File() == file; });

    if (it != _files.end())
    {
        _driver->Error("circle include file {0};", file);
        return;
    }

    _stackFile.back()->Inlcude(file);

    FileData* fd = new FileData(file);
    _files.push_back(fd);
    _stackFile.push_back(fd);
}

void Context::OnIncludeEnd()
{
    _stackFile.pop_back();
}

void Context::OnPredefine(const std::string& name)
{
    _driver->Warning("does not support predefine struct {0};", name);
}

void Context::OnStructBegin(const std::string& name, CfgCategory cfg)
{
    if (_type != nullptr)
        _driver->Warning("last type define is not completed pre:{0}, cur:{1}", _type->Name(), name);

    if (_scope->TypeSet()->Get(name) || _scope->VarSet()->Get(name))
    {
        _driver->Warning("struct {0} name conflict", name);
        _type = new StructType(ConflictName(name), _scope, cfg);
    }
    else
    {
        _type = new StructType(name, _scope, cfg);
    }

    _scope->TypeSet()->Add(_type);
    _stackFile.back()->Add(_type);

    //if (cfg == CfgCategory::Tab)
    //    _tabs.push_back(Cfg(_stackFile.back()->File(), _struct));
    //else if (cfg == CfgCategory::Json)
    //    _jsons.push_back(Cfg(_stackFile.back()->File(), _struct));
    if (cfg == CfgCategory::Tab)
        _tabs.push_back(Cfg("../out", _struct));
    else if (cfg == CfgCategory::Json)
        _jsons.push_back(Cfg("../out", _struct));
}

void Context::OnInherit(const std::string& name)
{
    if (_type == nullptr)
    {
        _driver->Error("current scope is not define a struct, can not inherit from {0}", name);
        return;
    }

    if (_type->Category() != TypeCategory::Struct)
    {
        _driver->Error("current type {0} is not a struct, can not inherit from {1}", _type->Name(), name);
        return;
    }

    IType* herit = GetType(name);
    if (herit == nullptr)
    {
        _driver->Error("type {0} is not defined", name);
        return;
    }

    if (herit->Category() != TypeCategory::Struct)
    {
        _driver->Error("type {0} is not a struct", name);
        return;
    }

    if (_struct->Inherited() != nullptr)
    {
        _driver->Error("struct {0} has already inherit from", _struct->Name(), _struct->Inherited()->Name());
        return;
    }

    if (_struct == herit)
    {
        _driver->Error("struct {0} can not inherit from self", name);
        return;
    }

    if (_struct->IsInherited(herit))
    {
        _driver->Error("struct {0} is alreay inherit from {1}", _struct->Name(), name);
        return;
    }

    _struct->Inherit(static_cast<StructType*>(herit));
}

void Context::OnStructEnd()
{
    _type = nullptr;
}

void Context::OnEnumBegin(const std::string& name)
{
    if (_type != nullptr)
        _driver->Error("last type [{0}] define is not completed", _type->Name());

    if (_scope->TypeSet()->Get(name) || _scope->VarSet()->Get(name))
    {
        _driver->Error("enum {0} name conflict", name);
        _type = new EnumType(ConflictName(name), _scope);
    }
    else
    {
        _type = new EnumType(name, _scope);
    }

    _scope->TypeSet()->Add(_type);
    _stackFile.back()->Add(_type);
}

void Context::OnEnumMember(const std::string& name)
{
    if (_type == nullptr || _type->Category() != TypeCategory::Enum)
    {
        _driver->Error("active type is not an enum");
        return;
    }

    Variate* var = new Variate(_enum, _enum, name);
    var->SetConst();

    if (!_enum->VarSet()->Add(var))
    {
        _driver->Error("enum {0} member {1} conflict", _enum->Name(), name);
        delete var;
        var = nullptr;
    }
}

void Context::OnEnumMember(const std::string& name, const std::string& value, bool refer)
{
    if (_type == nullptr || _type->Category() != TypeCategory::Enum)
    {
        _driver->Error("active type is not an enum");
        return;
    }

    IValue* val = nullptr;
    Variate* var = new Variate(_enum, _enum, name);
    if (refer)
    {
        IVariate* ref = utility::FindVar(_enum, value);
        if (ref == nullptr)
        {
            _driver->Error("can not find reference value:{0}", value);
        }
        else if (ref->Value() == nullptr
            || !ref->IsConst()
            || !value_util::IsRaw(ref->Value(), RawCategory::Int)
            )
        {
            _driver->Error("reference value:{0} is not allow", value);
        }
        else
        {
            val = value_util::Create(ref);
        }
    }
    else
    {
        val = value_util::Create(RawCategory::Int, value);
        if (val == nullptr)
            _driver->Error("parse enum key:{0} value:{1} failed", name, value);
    }

    if (val)
        var->BindValue(val);

    var->SetConst();
    if (!_enum->VarSet()->Add(var))
    {
        _driver->Error("enum {0} member {1} conflict", _enum->Name(), name);

        delete var;
        var = nullptr;
    }
}

void Context::OnEnumEnd()
{
    _type = nullptr;
}

void Context::OnVariateBegin(const std::string& type, const std::string& name)
{
    IType* varType = GetType(type);
    if (varType == nullptr)
    {
        _driver->Error("var type:{0} name:{1}, type is not defined", type, name);
        return;
    }

    if (varType->Category() == TypeCategory::Struct && varType == _struct)
    {
        _driver->Error("struct {0} is not completed", type);
        return;
    }

    if (_type == nullptr)
    {
        if (_scope->VarSet()->Get(name))
        {
            _driver->Error("var name:{0} conflict", name);
            return;
        }

        _var = new Variate(_scope, varType, name);
        _scope->VarSet()->Add(_var);
    }
    else if (_type->Category() == TypeCategory::Struct)
    {
        if (_struct->Inherited() && _struct->Inherited()->VarSet()->Get(name))
            _driver->Warning("var name:{0} conflict in base struct", name);

        _var = new Variate(_struct, varType, name);
        if (!_struct->VarSet()->Add(_var))
        {
            delete _var;
            _var = nullptr;
            _driver->Error("var name:{0} conflict", name);
        }
    }
    else
    {
        _driver->Error("var {0} {1} known error", type, name);
    }
}

void Context::OnVariateValue(RawCategory raw, const std::string& value)
{
    if (_var == nullptr)
    {
        _driver->Error("current var is not exist value:{0}", value);
        return;
    }

    IValue* val = value_util::Create(raw, value);
    if (val)
        _var->BindValue(val);
    else
        _driver->Error("convert value:{0} to raw:{1} failed", value, raw);
}

void Context::OnVariateValue(const std::string& refer)
{
    if (_var == nullptr)
    {
        _driver->Error("current var is not exist value:{0}", refer);
        return;
    }

    IVariate* ref = utility::FindVar(_type ? _type : _scope, refer);
    if (ref == nullptr)
        _driver->Error("can not find reference value:{0}", refer);
    else if (!ref->IsConst())
        _driver->Error("reference value:{0} is not allow", refer);
    else
        _var->BindValue(value_util::Create(ref));
}

void Context::OnVariateArray(const std::string& length/* = ""*/)
{
    if (_var == nullptr)
    {
        _driver->Error("current var is not exist");
        return;
    }

    int len = -1;
    if (!length.empty() && !utility::Convert(length, len, -1))
        _driver->Error("convert value:{0} to int failed", length);

    _var->UpgradeArray(len);
}

void Context::OnVariateEnd(bool isConst, const std::string& desc)
{
    if (_var)
    {
        const IType* varType = _var->Type();
        if (isConst)
            _var->SetConst();

        if (_var->Value() == nullptr && varType->Category() == TypeCategory::Raw)
            _var->BindValue(value_util::Create(static_cast<const RawType*>(varType)->Raw()));

        if (!desc.empty())
            _var->SetDesc(utility::Replace(utility::Trim(desc, " \t"), "\t", " "));

        if (_type)
        {
            _type->VarSet()->Add(_var);
        }
        else
        {
            _scope->VarSet()->Add(_var);
            _stackFile.back()->Add(_var);
        }
    }
    else
    {
        _driver->Error("var is not exist");
    }
    _var = nullptr;
}

std::string Context::ConflictName(const std::string& name) const
{
    int index = 0;

    std::string ret = name + "_crash";
    while (_scope->TypeSet()->Get(ret))
        ret = name + "_crash_" + std::to_string(++index);
    while (_scope->VarSet()->Get(ret))
        ret = name + "_crash_" + std::to_string(++index);

    return ret;
}

CFG_NAMESPACE_END
