﻿#include "Context.h"
#include "Driver.h"
#include "Type.h"
#include "Value.h"
#include "ValueUtil.h"
#include "Variate.h"
#include "Utility.h"
#include "FileData.h"
#include <stdio.h>
#include <cassert>

#define _SCOPE_ _stackScope.back()

CFG_NAMESPACE_BEGIN

static std::ostream& operator << (std::ostream& out, RawCategory raw)
{
    return out;
}

Context::Context(Driver& driver)
    : _driver(driver)
    , _gloal(nullptr)
    , _var(nullptr)
{
    _mergeFile = new FileData("");
    _gloal = new Namespace("", nullptr);
    _stackScope.push_back(_gloal->Scope());

    _SCOPE_->TypeSet()->Add(new RawType(TYPE_BOOL, RawCategory::Bool));
    _SCOPE_->TypeSet()->Add(new RawType(TYPE_INT, RawCategory::Int));
    _SCOPE_->TypeSet()->Add(new RawType(TYPE_FLOAT, RawCategory::Float));
    _SCOPE_->TypeSet()->Add(new RawType(TYPE_STRING, RawCategory::String));
}

Context::~Context()
{
    delete _gloal;
    _gloal = nullptr;

    delete _var;
    _var = nullptr;

    delete _mergeFile;
    _mergeFile = nullptr;

    std::for_each(_files.begin(), _files.end(), [ ](FileData* fd) {delete fd; });
    _files.clear();

    _stackScope.clear();
    _stackFile.clear();
    _tabs.clear();
    _jsons.clear();
}

const IScope* Context::Global() const
{
    return _gloal->Scope();
}

IType* Context::GetType(RawCategory raw) const
{
    ITypeSet* set = _gloal->Scope()->TypeSet();
    switch (raw)
    {
    case cfg::RawCategory::Bool:
        return set->Get(TYPE_BOOL);
    case cfg::RawCategory::Int:
        return set->Get(TYPE_INT);
    case cfg::RawCategory::Float:
        return set->Get(TYPE_FLOAT);
    case cfg::RawCategory::String:
        return set->Get(TYPE_STRING);
    default:
        break;
    }
    return nullptr;
}

IType* Context::GetType(const std::string& name) const
{
    IType* type = Global()->TypeSet()->Get(name);
    return type;
}

bool Context::Export(IExporter* expoter, const std::string& file)
{
    std::string fileName = utility::TrimFileSuffix(file, '.');
    if (fileName.empty() || fileName.back() == '/' || fileName.back() == '//')
        fileName = "unnamed";

    expoter->OnBegin(Global(), fileName);
    _mergeFile->Export(expoter, true);
    expoter->OnEnd();

    return true;
}

bool Context::Export(ITabCreater* creator, const std::string& path)
{
    for (auto it = _tabs.cbegin(); it != _tabs.cend(); ++it)
    {
        creator->SetPath(utility::ContactPath(path, it->path));
        utility::Traverse(it->sType, creator);
    }
    return true;
}

bool Context::Export(IJsonCreater* creator, const std::string& path)
{
    for (auto it = _jsons.cbegin(); it != _jsons.cend(); ++it)
    {
        creator->SetPath(utility::ContactPath(path, it->path));
        creator->OnStart(it->sType);
        creator->OnEnd();
    }
    return true;
}

void Context::OnParseBegin(const std::string& file)
{
    FileData* fd = new FileData(file);
    _files.push_back(fd);
    _stackFile.push_back(fd);
}

void Context::OnParseEnd()
{
    _stackFile.pop_back();
}

bool Context::OnIncludeBegin(const std::string& path, const std::string& file)
{
    auto it = std::find_if(_files.begin(), _files.end(),
        [&file](FileData* fd) { return fd->File() == file; });

    _stackFile.back()->Inlcude(file);

    if (it != _files.end())
        return false; // already include

    FileData* fd = new FileData(file);
    _files.push_back(fd);
    _stackFile.push_back(fd);
    return true;
}

void Context::OnIncludeEnd()
{
    _stackFile.pop_back();
}

void Context::OnPredefine(const std::string& name)
{
    /* does not support predefine struct */
}

void Context::OnNsBegin(const std::string& name)
{
    Namespace* ns = new Namespace(name, _SCOPE_);
    if (_SCOPE_->NsSet())
        _SCOPE_->NsSet()->Add(ns);
    else
        _driver.Error("current scope:{0} does not allow declare namespace:{1}", _SCOPE_->Name(), name);

    _stackScope.push_back(ns->Scope());
    _mergeFile->NsBegin(name);
    _stackFile.back()->NsBegin(name);
}

void Context::OnNsEnd()
{
    assert(_SCOPE_->BindNs());

    _mergeFile->NsEnd();
    _stackFile.back()->NsEnd();
    _stackScope.pop_back();

    if (_SCOPE_->NsSet() == nullptr)
        delete _SCOPE_->BindNs();
}

void Context::OnModuleBegin(const std::string& name)
{
    Module* module = new Module(name, _SCOPE_);
    // do need a module set?
    _stackScope.push_back(module->Scope());
}

void Context::OnModuleEnd()
{
    //assert(_SCOPE_->BindNs());

    _stackScope.pop_back();
}

void Context::OnFuncBegin(const std::string& name)
{
    Function* func = new Function(name, _data.type, _SCOPE_);
    _data.type = nullptr;

    _SCOPE_->TypeSet()->Add(func);

    _stackScope.push_back(func->Scope());
}

void Context::OnFuncBegin(const std::string& ret, const std::string& name)
{
    IType* retTy = utility::FindType(_SCOPE_, ret);
    Function* func = new Function(name, retTy, _SCOPE_);

    _SCOPE_->TypeSet()->Add(func);
    _stackScope.push_back(func->Scope());
}

void Context::OnFuncEnd()
{
    _stackScope.pop_back();
}

void Context::OnStructBegin(const std::string& name, CfgCategory cfg)
{
    std::string newName = name;
    IElement* element = _SCOPE_->GetElement(name);

    if (element != nullptr)
    {
        _driver.Warning("struct {0} name conflict", name);
        newName = ConflictName(name);
    }

    StructType* sType = new StructType(newName, _SCOPE_, cfg);
    if (_SCOPE_->TypeSet() == nullptr)
        _driver.Error("can not declare type int this scope:{0}", _SCOPE_->Name());
    else
        _SCOPE_->TypeSet()->Add(sType);

    if (!IsTypeScope())
    {
        _stackFile.back()->Add(sType);
        _mergeFile->Add(sType);
    }

    if (cfg == CfgCategory::Tab)
        _tabs.push_back(Cfg(_stackFile.back()->Path(), sType));
    else if (cfg == CfgCategory::Json)
        _jsons.push_back(Cfg(_stackFile.back()->Path(), sType));

    _stackScope.push_back(sType->Scope());
}

void Context::OnStructInherit(const std::string& name)
{
    IType* type = _SCOPE_->BindType();
    if (type == nullptr)
    {
        _driver.Error("current scope is not define a struct, can not inherit from {0}", name);
        return;
    }

    if (type->TypeCat() != TypeCategory::Struct)
    {
        _driver.Error("current type {0} is not a struct, can not inherit from {1}", type->Name(), name);
        return;
    }

    IStructType* curSty = static_cast<IStructType*>(type);
    IType* herit = utility::FindType(_SCOPE_, name);
    if (herit == nullptr)
    {
        _driver.Error("type {0} is not defined", name);
        return;
    }

    if (herit->TypeCat() != TypeCategory::Struct)
    {
        _driver.Error("type {0} is not a struct", name);
        return;
    }

    IStructType* sType = static_cast<StructType*>(herit);
    if (curSty->Inherited() != nullptr)
    {
        _driver.Error("struct {0} has already inherit from", type->Name(), curSty->Inherited()->Name());
        return;
    }

    if (curSty->IsInherited(sType))
    {
        _driver.Error("struct {0} is alreay inherit from {1}", curSty->Name(), name);
        return;
    }

    if (IsTypeProcessing(herit))
    {
        _driver.Error("struct {0} can not completed", name);
        return;
    }

    if (!curSty->Inherit(sType))
    {
        _driver.Error("unknown inherit error");
    }
}

void Context::OnStructEnd()
{
    if (_stackScope.size() <= 1)
    {
        _driver.Error("empty scope stack");
        return;
    }

    IType* type = _stackScope.back()->BindType();
    if (type && type->TypeCat() == TypeCategory::Struct)
    {
        IStructType* sType = static_cast<IStructType*>(type);
        if (sType->CfgCat() == CfgCategory::Tab)
            CheckTab(sType);
    }

    _stackScope.pop_back();
}

void Context::OnEnumBegin(const std::string& name)
{
    std::string newName = name;
    IElement* element = _SCOPE_->GetElement(name);
    if (element != nullptr)
    {
        _driver.Error("enum {0} name conflict", name);
        newName = ConflictName(name);
    }

    if (_SCOPE_->TypeSet() == nullptr)
    {
        _driver.Error("can not declare type int this scope:{0}", _SCOPE_->Name());
        return;
    }

    EnumType* eType = new EnumType(newName, _SCOPE_);
    _SCOPE_->TypeSet()->Add(eType);

    if (!IsTypeScope())
    {
        _stackFile.back()->Add(eType);
        _mergeFile->Add(eType);
    }

    _stackScope.push_back(eType->Scope());
}

void Context::OnEnumMember(const std::string& name)
{
    AddEnumMember(name);
}

void Context::OnEnumMemberValue(const std::string& name, const std::string& value)
{
    IVariate* var = AddEnumMember(name);
    if (var == nullptr)
        return;

    IValue* val = value_util::Create(RawCategory::Int, value);
    if (val == nullptr)
        _driver.Error("parse enum key:{0} value:{1} failed", name, value);
    else
        var->BindValue(val);
}

void Context::OnEnumMemberRefer(const std::string& name, const std::string& refer)
{
    IVariate* var = AddEnumMember(name);
    if (var == nullptr)
        return;

    IValue* val = nullptr;
    IVariate* ref = utility::FindVar(_SCOPE_, refer);
    if (ref == nullptr)
    {
        _driver.Error("can not find reference value:{0}", refer);
    }
    else if (ref->Value() == nullptr
        || !ref->IsConst()
        || !value_util::AsRaw(RawCategory::Int, ref->Value())
        )
    {
        _driver.Error("reference value:{0} is not allow", refer);
    }
    else
    {
        val = value_util::Create(ref);
    }

    if (val)
        var->BindValue(val);
}

void Context::OnEnumEnd()
{
    if (_stackScope.size() > 1)
        _stackScope.pop_back();
}
//
//void Context::OnVariateBegin(const std::string& type, const std::string& name)
//{
//    if (_var)
//    {
//        _driver.Error("last var has not completed");
//        delete _var;
//    }
//
//    _var = new Variate(_SCOPE_);
//
//    IType* varType = utility::FindType(_SCOPE_, type);
//    if (varType == nullptr)
//    {
//        _driver.Error("var type:{0} name:{1}, type is not defined", type, name);
//        return;
//    }
//
//    if (varType == _SCOPE_->BindType())
//    {
//        _driver.Error("current type:{0} is not completed", type);
//        return;
//    }
//
//    _var->SetType(varType);
//    _var->SetName(name);
//}
//
//void Context::OnVariateValue(RawCategory raw, const std::string& value)
//{
//    assert(_var);
//
//    IValue* val = nullptr;
//    // string need remove the first and end /"
//    if (raw == RawCategory::String)
//        val = value_util::Create(raw, value.substr(1, value.length() - 2));
//    else
//        val = value_util::Create(raw, value);
//
//    if (val == nullptr || !_var->BindValue(val))
//        _driver.Error("convert value:{0} to raw:{1} failed", value, raw);
//}
//
//void Context::OnVariateValue(const std::string& refer)
//{
//    assert(_var);
//
//    IVariate* ref = utility::FindVar(_SCOPE_, refer);
//    if (ref == nullptr)
//    {
//        _driver.Error("can not find reference value:{0}", refer);
//        return;
//    }
//
//    if (!ref->IsConst())
//    {
//        _driver.Error("reference value:{0} must be constant", refer);
//    }
//
//    IValue* val = value_util::Create(ref);
//    if (!_var->BindValue(val))
//    {
//        _driver.Error("bind variate failed");
//        delete val;
//    }
//}
//
//void Context::OnVariateArray()
//{
//    UpgradeArray(0);
//}
//
//void Context::OnVariateArrayLength(const std::string& length)
//{
//    int len = 0;
//    if (!utility::Convert(length, len))
//        _driver.Error("convert array length[{0}] to length failed", length);
//    else if (len <= 0)
//        _driver.Warning("array with length is {0}", len);
//
//    UpgradeArray(len);
//}
//
//void Context::OnVariateArrayRefer(const std::string& refer)
//{
//    int len = 0;
//    IVariate* var = utility::FindVar(_SCOPE_, refer);
//    if (var == nullptr)
//        _driver.Error("can not find any var with name:{0}", refer);
//    else if (!value_util::Value(var->Value(), len))
//        _driver.Error("var with name:{0} cant not convert to length", refer);
//    else if (len <= 0)
//        _driver.Warning("array with length is {0}", len);
//
//    UpgradeArray(len);
//}
//
//void Context::OnVariateConst()
//{
//    assert(_var);
//
//    IType* type = _var->Type();
//    if (type == nullptr)
//    {
//        _driver.Error("current variate is invalid var:{0}", _var->Name());
//        return;
//    }
//
//    if (type->TypeCat() != TypeCategory::Raw)
//    {
//        _driver.Error("only raw type variate can be const, var:{0} type:{1}", _var->Name(), type->Name());
//        return;
//    }
//
//    _var->SetConst();
//}
//
//void Context::OnVariateDesc(const std::string& desc)
//{
//    assert(_var);
//    _var->SetDesc(utility::Replace(utility::Trim(desc, " \t"), "\t", " "));
//}
//
//void Context::OnVariateEnd()
//{
//    assert(_var);
//
//    std::auto_ptr<Variate> var(_var);
//    _var = nullptr;
//
//    if (var->Type() == nullptr)
//        return;
//
//    if (_SCOPE_->VarSet() == nullptr)
//    {
//        _driver.Error("current scope does not allow declare variate scope:{0} var:{1}", _SCOPE_->Name(), var->Name());
//        return;
//    }
//
//    if (_SCOPE_->GetElement(var->Name()))
//    {
//        _driver.Error("var name:{0} conflict", var->Name());
//        return;
//    }
//
//    if (!_SCOPE_->VarSet()->Add(var.get()))
//    {
//        _driver.Error("var name:{0} conflict", var->Name());
//        return;
//    }
//
//    if (!IsTypeScope())
//    {
//        _stackFile.back()->Add(var.get());
//        _mergeFile->Add(var.get());
//    }
//
//    var.release();
//}

void Context::OnVariate(const std::string& name)
{
    if (_data.type == nullptr)
    {
        // error
    }

    // check whether value match type

    if (_SCOPE_->VarSet() == nullptr)
    {
        // error
    }

    Variate* var = new Variate(name, _SCOPE_);
    var->SetType(_data.type);
    var->BindValue(_data.value);
    var->SetDesc(_data.desc);
    if (_data.isConst)
        var->SetConst();

    _SCOPE_->VarSet()->Add(var);

    _data.desc = "";
    _data.isConst = false;
    _data.type = nullptr;
    _data.value = nullptr;
}

void Context::SetConst()
{
    assert(_data.isConst == false);
    _data.isConst = true;
}

void Context::SetDesc(const std::string& desc)
{
    _data.desc = desc;
}

void Context::SetType(RawCategory raw)
{
    assert(_data.type == nullptr);

    _data.type = GetType(raw);
    if (_data.type == nullptr)
        _driver.Error("Can't find Raw type:{0}", raw);
}

void Context::SetType(const std::string& type)
{
    assert(_data.type == nullptr);

    _data.type  = utility::FindType(_SCOPE_, type);
    if (_data.type == nullptr)
        _driver.Error("Can't find type type:{0}", type);
}

void Context::SetArray()
{
    UpgradeArray(0);
}

void Context::SetArrayLength(const std::string& length)
{
    int len = 0;
    if (!utility::Convert(length, len))
        _driver.Error("convert array length[{0}] to length failed", length);
    else if (len <= 0)
        _driver.Warning("array with length is {0}", len);

    UpgradeArray(len);
}

void Context::SetArrayRefer(const std::string& refer)
{
    int len = 0;
    IVariate* var = utility::FindVar(_SCOPE_, refer);
    if (var == nullptr)
        _driver.Error("can not find any var with name:{0}", refer);
    else if (!value_util::Value(var->Value(), len))
        _driver.Error("var with name:{0} cant not convert to length", refer);
    else if (len <= 0)
        _driver.Warning("array with length is {0}", len);

    UpgradeArray(len);
}

void Context::SetValue(RawCategory raw, const std::string& value)
{
    assert(_data.value == nullptr);

    IType* type = _data.type;
    if (type->TypeCat() == TypeCategory::Array)
        type = static_cast<IArrayType*>(type)->Original();

    if (
        type->TypeCat() != TypeCategory::Raw
        || static_cast<IRawType*>(type)->RawCat() != raw
        )
    {
        _driver.Error("current type:{0} cant not match value:{1}", type->Name(), value);
        return;
    }

    if (raw == RawCategory::String)
        _data.value = value_util::Create(raw, value.substr(1, value.length() - 2));
    else
        _data.value = value_util::Create(raw, value);

    if (_data.value == nullptr)
        _driver.Error("create with type:{0} value:{1} failed", raw, value);
}

void Context::SetValue(const std::string& refer)
{
    assert(_data.value == nullptr);

    IVariate* ref = utility::FindVar(_SCOPE_, refer);
    if (ref == nullptr)
    {
        _driver.Error("can not find reference value:{0}", refer);
        return;
    }

    if (!ref->IsConst())
    {
        _driver.Error("reference value:{0} must be constant", refer);
    }

    if (_data.type == nullptr)
    {
        _driver.Error("current type is empty value:{0}", refer);
        return;
    }

    IType* type = _data.type;
    if (type->TypeCat() == TypeCategory::Array)
        type = static_cast<IArrayType*>(type)->Original();

    if (ref->Type() != type)
    {
        _driver.Error("current type:{0} cant not match value:{1}", type->Name(), refer);
        return;
    }

    _data.value = value_util::Create(ref);
    if (_data.value == nullptr)
        _driver.Error("create reference value:{0} failed", refer);
}

IVariate* Context::AddEnumMember(const std::string& name)
{
    if (_SCOPE_->VarSet() == nullptr)
    {
        _driver.Error("this scope can not declare member scope:{0}", _SCOPE_->Name());
        return nullptr;
    }

    IType* ty = _SCOPE_->BindType();
    if (ty == nullptr || ty->TypeCat() != TypeCategory::Enum)
        return nullptr;

    IEnumType* eType = static_cast<IEnumType*>(ty);
    Variate* var = new Variate(_SCOPE_);
    var->SetType(eType);
    var->SetName(name);
    var->SetConst();

    if (!_SCOPE_->VarSet()->Add(var))
    {
        _driver.Error("enum {0} member {1} conflict", eType->Name(), name);
        delete var;
        var = nullptr;
    }
    return var;
}

void Context::UpgradeArray(int length)
{
    assert(_data.type);

    //if (_var->Type()->TypeCat() == TypeCategory::Array)
    //{
    //    _driver.Error("does not support multi dimensional array");
    //    return;
    //}

    if (length < 0)
        length = 0;

    _data.type = new ArrayType(_data.type, length);
}

std::string Context::ConflictName(const std::string& name) const
{
    int index = 0;

    std::string ret = name + "_crash";
    while (_SCOPE_->GetElement(ret))
        ret = name + "_crash_" + std::to_string(++index);

    return ret;
}

bool Context::IsTypeProcessing(IType* type) const
{
    for (auto it = _stackScope.cbegin(); it != _stackScope.cend(); ++it)
    {
        if (type == (*it)->BindType())
            return true;
    }
    return false;
}

bool Context::IsTypeScope() const
{
    for (auto it = _stackScope.cbegin(); it != _stackScope.cend(); ++it)
    {
        if ((*it)->BindType())
            return true;
    }
    return false;
}

void Context::CheckTab(IStructType* type)
{
    type->Scope()->VarSet()->Traverse([&](IVariate* var) {
        TabVarChecker(type->Name(), var);
        return true;
    });
}

bool Context::TabVarChecker(const std::string& path, IVariate* var)
{
    IType* type = var->Type();
    if (type->TypeCat() == TypeCategory::Array)
    {
        IArrayType* arType = static_cast<IArrayType*>(type);
        IType* original = arType->Original();
        if (original->TypeCat() == TypeCategory::Struct)
        {
            if (arType->Length() == 0)
                _driver.Error("tab file only allow struct array with fixed length, var:{0}.{1} type:{2}", path, var->Name(), type->Name());
            type = original;
        }
    }

    if (type->TypeCat() == TypeCategory::Struct)
    {
        std::string p = path + '.' + var->Name();
        type->Scope()->VarSet()->Traverse([&](IVariate* v) {
            TabVarChecker(p, v);
            return true;
        });
    }
    return true;
}

void Context::OnCommonBegin()
{

}

void Context::OnCommonEnd()
{

}

CFG_NAMESPACE_END
