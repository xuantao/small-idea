/*
 * csharp exporter
*/
#pragma once

#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

class CsExporter : public IExporter
{
public:
    CsExporter();
    virtual ~CsExporter();

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
    std::string _path;
    std::string _name;

    std::vector<IExporter*> _exporters;
};

GCF_NAMESPACE_END
