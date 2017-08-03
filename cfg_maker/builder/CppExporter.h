﻿#pragma once
#include "Interface.h"
#include <ostream>
#include <stack>

CFG_NAMESPACE_BEGIN

class CppExporter : public IExporter
{
public:
    struct VarData
    {
        std::string type;
        std::string name;
        std::string value;
    };

public:
    CppExporter();
    ~CppExporter();

public:
    virtual void OnBegin(const IScopeType* global, const std::string& file);
    virtual void OnEnd();
    virtual void OnFileBegin(const std::string& file);
    virtual void OnFileEnd();
    virtual void OnNamespaceBegin(const std::string& name);
    virtual void OnNamesapceEnd();
    virtual void OnInclude(const std::string& file);
    virtual void OnVariate(const IVariate* var);
    virtual void OnType(const IType* type);

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
    const IScopeType* _global;
    std::string _file;

    std::vector<const IEnumType*> _enums;
    std::vector<const IStructType*> _structs;

    std::vector<const IStructType*> _tabs;
    std::vector<const IStructType*> _tabDepends;

    std::vector<const IStructType*> _jsons;
    std::vector<const IStructType*> _jsonDepends;
};

CFG_NAMESPACE_END
