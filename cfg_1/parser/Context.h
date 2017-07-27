﻿#pragma once

#include <string>
#include <map>
#include "Interface.h"

CFG_NAMESPACE_BEGIN

class Driver;
class Variate;
class IType;
class EnumType;
class StructType;
class ScopeType;
class FileData;

class Context
{
public:
    Context();
    ~Context();

public:
    const IScopeType* Global() const;
    IType* GetType(const std::string& name) const;

    bool Export(IExporter* expoter, const std::string& file, bool merge);

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
    void OnEnumMember(const std::string& name);
    void OnEnumMember(const std::string& name, const std::string& value, bool refer);
    void OnEnumEnd();

    void OnVariateBegin(const std::string& type, const std::string& name);
    void OnVariateValue(RawCategory raw, const std::string& value);
    void OnVariateValue(const std::string& refer);
    void OnVariateArray(const std::string& length = "");
    void OnVariateEnd(bool isConst);

protected:
    std::string ConflictName(const std::string& name) const;

protected:
    Driver* _driver;

    std::vector<FileData*> _files;
    std::vector<FileData*> _stackFile;

    IScopeType* _scope;
    Variate* _var;

    union
    {
        IType* _type;
        EnumType* _enum;        // easy visit
        StructType* _struct;    // easy visit
    };
};

CFG_NAMESPACE_END
