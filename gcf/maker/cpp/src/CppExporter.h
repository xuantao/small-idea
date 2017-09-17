#pragma once
#include "gcf/gcf.h"
#include <ostream>
#include <stack>

GCF_NAMESPACE_BEGIN

namespace cpp
{
    class Declare;
    class Serialize;
}

class CppExporter : public IExporter
{
public:
    static CppExporter* GetInstance();

public:
    CppExporter();
    virtual ~CppExporter();

public:
    virtual void Release();

public:
    virtual bool OnBegin(const IScope* global, const char* path, const char* name);
    virtual void OnEnd();

    virtual void OnNsBegin(const std::string& name);
    virtual void OnNsEnd();

    virtual void OnInclude(const std::string& file);

    virtual void OnVariate(const IVariate* var);
    virtual void OnType(const IType* type);
    virtual void OnCrossCall(const ICrossCall* module);

protected:
    bool Declare(const IEnumType* ty);
    bool Declare(const IStructType* ty);

    bool HeaderDeclare();
    bool CppImpl();

    void GetDepends(const IStructType* sType, std::vector<const IStructType*>& deps);
protected:
    /* enum */
    bool EnumData(const IEnumType* eType, int tab);
    bool EnumHelperDeclare(const IEnumType* eType, int tab);
    bool EnumHelper(const IEnumType* eType, int tab);

    /* tab */
    void TabDeclare(const IStructType* sType, int tab);
    void TabLoader(const IStructType* sType, int tab);
    void TabLoaderStatic(const IStructType* sType, int tab);
    void TabLoadDetail(const IStructType* sType, int tab);
    void TabLoadVar(const IVariate* var, const IRawType* rType, int tab);
    void TabLoadVar(const IVariate* var, const IEnumType* rType, int tab);
    void TabLoadVar(const IVariate* var, const IStructType* rType, int tab);
    void TabLoadVar(const IVariate* var, const IRawType* rType, int length, int tab);
    void TabLoadVar(const IVariate* var, const IEnumType* rType, int length, int tab);
    void TabLoadVar(const IVariate* var, const IStructType* rType, int length, int tab);
    void TabWriteHeader(const IStructType* sType, int tab);
    void TabWriter(const IStructType* sType, int tab);
    void TabWriterStatic(const IStructType* sType, int tab);
    void TabWriterDetail(const IStructType* sType, int tab);

    /* json */
    void JsonDeclare(const IStructType* sType, int tab);
    void JsonLoader(const IStructType* sType, int tab);
    void JsonLoaderStatic(const IStructType* sType, int tab);
    void JsonLoadVar(const IVariate* var, const IRawType* rType, int tab);
    void JsonLoadVar(const IVariate* var, const IEnumType* rType, int tab);
    void JsonLoadVar(const IVariate* var, const IStructType* rType, int tab);
    void JsonLoadVar(const IVariate* var, const IRawType* rType, int length, int tab);
    void JsonLoadVar(const IVariate* var, const IEnumType* rType, int length, int tab);
    void JsonLoadVar(const IVariate* var, const IStructType* rType, int length, int tab);
    void JsonWriter(const IStructType* sType, int tab);
    void JsonWriterStatic(const IStructType* sType, int tab);

protected:
    std::ostream* _stream;
    int _tab;

    bool _lastIsVar;
    const IScope* _global;
    std::string _file;
    std::string _path;
    std::string _name;

    cpp::Declare* _declare = nullptr;
    cpp::Serialize* _serialize = nullptr;

    std::vector<const IEnumType*> _enums;
    std::vector<const IStructType*> _structs;

    std::vector<const IStructType*> _tabs;
    std::vector<const IStructType*> _tabDepends;

    std::vector<const IStructType*> _jsons;
    std::vector<const IStructType*> _jsonDepends;

    std::vector<IExporter*> _expoters;
};

GCF_NAMESPACE_END
