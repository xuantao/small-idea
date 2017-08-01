#pragma once
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
    virtual void OnBegin(const IScopeType* global, const std::string& path, const std::string& file);
    virtual void OnEnd();
    virtual void OnFileBegin(const std::string& file);
    virtual void OnFileEnd();
    virtual void OnNamespaceBegin(const std::string& name);
    virtual void OnNameSapceEnd();
    virtual void OnInclude(const std::string& file);
    virtual void OnVariate(const IVariate* var);
    virtual void OnType(const IType* type);

protected:
    bool Declare(const IEnumType* ty);
    bool Declare(const IStructType* ty);

    bool EnumStaticData(const IEnumType* eType);
    bool FuncDeclare(const IEnumType* eType);
    bool FuncImpl(const IEnumType* eType);

    bool TabFuncDeclare(const IStructType* sType);
    bool TabFuncImpl(const IStructType* sType);

    bool TabLoaderFunc(const IStructType* ty);
    //bool TabLoad(const IStructType* sType, const std::string& title, const std::string& name);
    //bool TabLoad(const IRawType* type, const std::string& title, const std::string& name);
    //bool TabLoad(const IEnumType* type, const std::string& title, const std::string& name);
    //bool TabLoad(const IArrayType* type, const std::string& title, const std::string& name);

    bool JsonFuncDeclare(const IStructType* sType);
    bool JsonFuncImpl(const IStructType* sType);

    bool DeclaerHeader();
    bool ImplationCpp();

    void GetDepends(const IStructType* sType, std::vector<const IStructType*>& deps);
protected:
    std::ostream* _stream;
    int _tab;

    const IScopeType* _global;
    std::string _path;
    std::string _file;

    std::vector<const IEnumType*> _enums;
    std::vector<const IStructType*> _structs;

    std::vector<const IStructType*> _tabs;
    std::vector<const IStructType*> _tabDepends;

    std::vector<const IStructType*> _jsons;
    std::vector<const IStructType*> _jsonDepends;
};

CFG_NAMESPACE_END
