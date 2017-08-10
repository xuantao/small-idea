#pragma once

#include <string>
#include <map>
#include <stack>
#include "Interface.h"

CFG_NAMESPACE_BEGIN

class Driver;
class Variate;
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

    bool OnIncludeBegin(const std::string& path, const std::string& file);
    void OnIncludeEnd();

    void OnNsBegin(const std::string& name);
    void OnNsEnd();

    void OnPredefine(const std::string& name);
    void OnStructBegin(const std::string& name, CfgCategory cfg);
    void OnStructInherit(const std::string& name);
    void OnStructEnd();

    void OnEnumBegin(const std::string& name);
    void OnEnumMember(const std::string& name);
    void OnEnumMemberValue(const std::string& name, const std::string& value);
    void OnEnumMemberRefer(const std::string& name, const std::string& refer);
    void OnEnumEnd();

    void OnVariateBegin(const std::string& type, const std::string& name);
    void OnVariateValue(RawCategory raw, const std::string& value);
    void OnVariateValue(const std::string& refer);
    void OnVariateArray();
    void OnVariateArrayLength(const std::string& length);
    void OnVariateArrayRefer(const std::string& refer);
    void OnVariateConst();
    void OnVariateDesc(const std::string& desc);
    void OnVariateEnd();

protected:
    IVariate* AddEnumMember(const std::string& name);
    void UpgradeArray(int length);
    std::string ConflictName(const std::string& name) const;
    bool IsTypeProcessing(IType* type) const;
    bool IsTypeScope() const;

protected:
    Driver& _driver;
    FileData* _mergeFile;
    std::vector<FileData*> _files;
    std::vector<FileData*> _stackFile;

    std::vector<Cfg> _tabs;
    std::vector<Cfg> _jsons;

    INamespace* _gloal;
    std::vector<IScope*> _stackScope;

    Variate* _var;
};

CFG_NAMESPACE_END
