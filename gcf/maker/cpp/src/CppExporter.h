#pragma once
#include "gcf/gcf.h"
#include <ostream>
#include <stack>

GCF_NAMESPACE_BEGIN

class CppExporter : public IExporter
{
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
    const IScope* _global;
    std::string _file;
    std::string _path;
    std::string _name;

    std::vector<IExporter*> _exporters;
};

GCF_NAMESPACE_END
