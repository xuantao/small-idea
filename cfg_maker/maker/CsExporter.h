/*
 * csharp exporter
*/
#pragma once

#include "Interface.h"

CFG_NAMESPACE_BEGIN

class CsExporter : public IExporter
{
public:
    CsExporter();
    virtual ~CsExporter();

public:
    virtual void OnBegin(const IScope* global, const std::string& file);
    virtual void OnEnd();

    virtual void OnNsBegin(const std::string& name);
    virtual void OnNsEnd();

    virtual void OnInclude(const std::string& file);

    virtual void OnVariate(const IVariate* var);
    virtual void OnType(const IType* type);
    virtual void OnModule(const IModule* module);

protected:
    std::vector<const IVariate*> _vars;
    std::vector<const IType*> _types;
};

CFG_NAMESPACE_END

