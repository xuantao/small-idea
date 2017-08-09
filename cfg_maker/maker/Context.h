#pragma once

#include <string>
#include <map>
#include <stack>
#include "Interface.h"

CFG_NAMESPACE_BEGIN

class Driver;
class Variate;
class IType;
class EnumType;
class StructType;
class ScopeType;
class FileData;

struct Cfg
{
    Cfg() {}
    Cfg(const std::string& _path, IStructType* type)
        : path(_path)
        , sType(type)
    {
    }

    std::string path;
    IStructType* sType;
};

class Context
{
public:
    Context(Driver& driver);
    ~Context();

public:
    const IScope* Global() const;
    IType* GetType(const std::string& name) const;

    const std::vector<Cfg>& TabCfgs() const { return _tabs; }
    const std::vector<Cfg>& JsonCfgs() const { return _jsons; }

    bool Export(IExporter* expoter, const std::string& file);
    bool Export(ITabCreater* creator, const std::string& path);
    bool Export(IJsonCreater* creator, const std::string& path);
public:
    void OnParseBegin(const std::string& file);
    void OnParseEnd();

    void OnIncludeBegin(const std::string& file);
    void OnIncludeEnd();

    void OnPredefine(const std::string& name);
    void OnStructBegin(const std::string& name, CfgCategory cfg);
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
    void OnVariateEnd(bool isConst, const std::string& desc);

protected:
    IScope* Scope() const { return _stackScope.top(); }
    std::string ConflictName(const std::string& name) const;

protected:
    Driver& _driver;

    std::vector<FileData*> _files;
    std::vector<FileData*> _stackFile;

    std::vector<Cfg> _tabs;
    std::vector<Cfg> _jsons;

    INamespace* _gloal;
    std::stack<IScope*> _stackScope;

    Variate* _var;
    union
    {
        IType* _type;
        EnumType* _enum;        // easy visit
        StructType* _struct;    // easy visit
    };
};

CFG_NAMESPACE_END
