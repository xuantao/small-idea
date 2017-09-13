#pragma once

#include <string>
#include <map>
#include <stack>
#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

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
    IType* GetType(RawCategory raw) const;
    IType* GetType(const std::string& name) const;

    const std::vector<Cfg>& TabCfgs() const { return _tabs; }
    const std::vector<Cfg>& JsonCfgs() const { return _jsons; }
    const std::vector<IModule*>& Modules() const { return _modules; }

    bool Export(IExporter* expoter, const std::string& file);
    bool Export(ITabCreater* creator, const std::string& path);
    bool Export(IJsonCreater* creator, const std::string& path);

public:
    bool IsVarDeclaring() const;

    void OnParseBegin(const std::string& file);
    void OnParseEnd();

    bool OnIncludeBegin(const std::string& path, const std::string& file);
    void OnIncludeEnd();

    void OnNsBegin(const std::string& name);
    void OnNsEnd();

    void OnModuleBegin(const std::string& name);
    void OnModuleEnd();

    void OnFuncBegin(const std::string& name);
    void OnFuncEnd();

    void OnPredefine(const std::string& name);
    void OnStructBegin(const std::string& name, CfgCategory cfg);
    void OnStructInherit(const std::string& name);
    void OnStructEnd();

    void OnEnumBegin(const std::string& name);
    void OnEnumMember(const std::string& name);
    void OnEnumMemberValue(const std::string& name, const std::string& value);
    void OnEnumMemberRefer(const std::string& name, const std::string& refer);
    void OnEnumEnd();

    void OnVariate(const std::string& name);

public:
    void SetConst();
    void SetDesc(const std::string& desc);
    void SetType(RawCategory raw);
    void SetType(const std::string& type);
    void SetArray();
    void SetArrayLength(const std::string& length);
    void SetArrayRefer(const std::string& refer);
    void SetValue(RawCategory raw, const std::string& value);
    void SetValue(const std::string& refer);

protected:
    IVariate* AddEnumMember(const std::string& name);
    void UpgradeArray(int length);
    std::string ConflictName(const std::string& name) const;
    bool IsTypeProcessing(IType* type) const;
    bool IsTypeScope() const;
    void CheckTab(IStructType* type);
    bool TabVarChecker(const std::string& path, IVariate* var);
    void OnCommonBegin();
    void OnCommonEnd();

protected:
    Driver& _driver;
    FileData* _mergeFile = nullptr;
    std::vector<FileData*> _files;
    std::vector<FileData*> _stackFile;

    std::vector<Cfg> _tabs;
    std::vector<Cfg> _jsons;
    std::vector<IModule*> _modules;

    INamespace* _gloal = nullptr;
    std::vector<IScope*> _stackScope;

    struct
    {
        std::string desc;
        bool isConst = false;
        IType* type = nullptr;
        IValue* value = nullptr;
    } _data;
};

GCF_NAMESPACE_END
