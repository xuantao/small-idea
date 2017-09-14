/*
 * visit tab struct
 * collect title, type, describe, default value
*/
#pragma once

#include "Utility.h"

GCF_NAMESPACE_BEGIN

namespace utility
{
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
        virtual bool OnVar(const IVariate* var, const IType* rType,
            const std::string& title);

    protected:
        std::vector<std::string> _title;
        std::vector<std::string> _type;
        std::vector<std::string> _desc;
        std::vector<std::string> _def;
    };
}
GCF_NAMESPACE_END
