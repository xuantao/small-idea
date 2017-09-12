/*
 * visit tab struct
 * collect title, type, describe, default value
*/
#pragma once

#include "Interface.h"

CFG_NAMESPACE_BEGIN

//TODO: do not use cpp only utility
class TabVisitor : public ITabVisitor
{
public:
    TabVisitor() {}
    virtual ~TabVisitor() {}

public:
    const std::vector<std::string>& Title() const { return _title; }
    const std::vector<std::string>& Type() const { return _type; }
    const std::vector<std::string>& Describe() const { return _desc; }
    const std::vector<std::string>& Default() const { return _def; }

public:
    virtual bool OnStart(const IStructType* sType);
    virtual bool OnEnd();
    virtual bool OnVar(const IVariate* var, const IRawType* rType,
        const std::string& title, const std::string& path);
    virtual bool OnVar(const IVariate* var, const IEnumType* eType,
        const std::string& title, const std::string& path);
    virtual bool OnVar(const IVariate* var, const IRawType* rType,
        const std::string& title, const std::string& path, int length);
    virtual bool OnVar(const IVariate* var, const IEnumType* eType,
        const std::string& title, const std::string& path, int length);

protected:
    std::vector<std::string> _title;
    std::vector<std::string> _type;
    std::vector<std::string> _desc;
    std::vector<std::string> _def;
};

CFG_NAMESPACE_END
