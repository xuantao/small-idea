#pragma once

#include <string>
#include <map>
#include "CfgDef.h"

CFG_NAMESPACE_BEGIN

class Driver;
class EnumType;
class StructType;
class Variate;

class Context
{
public:
    Context();
    ~Context();

public:
    IType* GetType(const std::string& name) const;

public:
    void OnParseBegin(Driver& driver, const std::string& file);
    void OnParseEnd();

    void OnIncludeBegin(const std::string& file);
    void OnIncludeEnd();

    void OnPredefine(const std::string& name);
    void OnStructBegin(const std::string& name);
    void OnInherit(const std::string& name);
    void OnStructEnd();

    void OnEnumBegin(const std::string& name);
    void OnEnumMember(const std::string& name, const std::string& value = "");
    void OnEnumMemberRef(const std::string& name, const std::string& value);
    void OnEnumEnd();

    void OnVariateBegin(const std::string& type, const std::string& name);
    void OnVariateValue(const std::string& value);
    void OnVariateValueRef(const std::string& value);
    void OnVariateArray(const std::string& length = "");
    void OnVariateEnd();

protected:
    std::string GenCrashName(const std::string& name) const;
    IValue* CreateValue(const IType* type, const std::string& value) const;

    IValue* CreateValueBool(const IType* type, const std::string& value) const;
    IValue* CreateValueInt(const IType* type, const std::string& value) const;
    IValue* CreateValueFloat(const IType* type, const std::string& value) const;
    IValue* CreateValueString(const IType* type, const std::string& value) const;

    const IVariate* FindVarRef(const IType* scope, const std::string& path) const;

protected:
    typedef std::map<std::string, IVariate*> MapVariate;
    typedef std::map<std::string, IType*> MapType;

    Driver* _driver;

    MapVariate _globalVars;
    MapType _rawTypes;
    ScopeType* _scope;

    Variate* _var;

    union
    {
        IType* _type;
        EnumType* _enum;
        StructType* _struct;
    };
};

CFG_NAMESPACE_END
